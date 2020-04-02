using System;
using System.Collections.Generic;
using System.Linq;
using TracePersistence.utility;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Devices.Enumeration;
using Windows.Storage.Streams;

namespace TracePersistence
{
    public enum BLEDeviceStatus
    {
        Connected,
        Enumerating
    }

    public enum ReceiveState
    {
        Idle = 0,
        Orientation = 1,
        VESC = 2,
        Drive = 3
    }

    public class BLEDevice
    {
        public string Name { get; set; }
        public BLEDeviceStatus Status { get; private set; }

        private GattCharacteristic DeviceCharacteristic { get; set; }

        private List<byte> ReceiveBuffer { get; set; }

        public event EventHandler<DataPacketReceivedEventArgs> DataPacketReceived;

        public BLEDevice(string name)
        {
            this.Name = name;
            this.ReceiveBuffer = new List<byte>();

            // Query for extra properties you want returned
            string[] requestedProperties = { "System.Devices.Aep.DeviceAddress", "System.Devices.Aep.IsConnected" };

            DeviceWatcher deviceWatcher =
                        DeviceInformation.CreateWatcher(
                                BluetoothLEDevice.GetDeviceSelectorFromPairingState(false),
                                requestedProperties,
                                DeviceInformationKind.AssociationEndpoint);

            // Register event handlers before starting the watcher.
            // Added, Updated and Removed are required to get all nearby devices
            deviceWatcher.Added += DeviceWatcher_Added;
            //deviceWatcher.Updated += DeviceWatcher_Updated;
            deviceWatcher.Removed += DeviceWatcher_Removed;

            // EnumerationCompleted and Stopped are optional to implement.
            //deviceWatcher.EnumerationCompleted += DeviceWatcher_EnumerationCompleted;
            //deviceWatcher.Stopped += DeviceWatcher_Stopped;

            // Start the watcher.
            deviceWatcher.Start();
            this.Status = BLEDeviceStatus.Enumerating;
        }

        protected virtual void OnDataPacketReceived(DataPacket dataPacket)
        {
            this.DataPacketReceived?.Invoke(this, new DataPacketReceivedEventArgs(dataPacket));
        }


        private void DeviceWatcher_Removed(DeviceWatcher sender, DeviceInformationUpdate args)
        {
        }

        private void DeviceWatcher_Added(DeviceWatcher sender, DeviceInformation args)
        {
            if (args.Name == this.Name)
                this.ConnectDevice(args);
        }

        private async void ConnectDevice(DeviceInformation deviceInfo)
        {
            // Note: BluetoothLEDevice.FromIdAsync must be called from a UI thread because it may prompt for consent.
            BluetoothLEDevice bluetoothLeDevice = await BluetoothLEDevice.FromIdAsync(deviceInfo.Id);
            GattDeviceServicesResult result = await bluetoothLeDevice.GetGattServicesAsync();

            if (result.Status == GattCommunicationStatus.Success)
            {
                IReadOnlyList<GattDeviceService> services = result.Services;
                int cnt = services.Count;

                foreach (GattDeviceService service in services)
                {
                    GattCharacteristicsResult gattCharacteristicsResult = await service.GetCharacteristicsAsync();

                    foreach (GattCharacteristic characteristic in gattCharacteristicsResult.Characteristics)
                    {
                        if (characteristic.CharacteristicProperties.HasFlag(GattCharacteristicProperties.Write | GattCharacteristicProperties.Notify))
                        {
                            try
                            {
                                GattCommunicationStatus status = await characteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue.Notify);
                                if (status == GattCommunicationStatus.Success)
                                {
                                    this.DeviceCharacteristic = characteristic;
                                    characteristic.ValueChanged += Characteristic_ValueChangedAsync;
                                    this.Status = BLEDeviceStatus.Connected;
                                }
                            }
                            catch (Exception ex)
                            {
                            }
                        }
                    }
                }
            }
        }

        public async void WriteBytes(byte[] bytes)
        {
            var writer = new DataWriter();
            writer.WriteBytes(bytes);

            GattCommunicationStatus result = await this.DeviceCharacteristic.WriteValueAsync(writer.DetachBuffer());
            if (result == GattCommunicationStatus.Success)
            {

            }
        }


        private void Characteristic_ValueChangedAsync(GattCharacteristic sender, GattValueChangedEventArgs args)
        {
            byte[] data = new byte[args.CharacteristicValue.Length];
            DataReader.FromBuffer(args.CharacteristicValue).ReadBytes(data);

            this.ReceiveBuffer.AddRange(data);

            if (this.ReceiveBuffer.Count >= 4)
            {
                UInt16 type = BitConverter.ToUInt16(new byte[] { 0, 0, this.ReceiveBuffer[1], this.ReceiveBuffer[0] }.Reverse().ToArray(), 0);
                UInt16 pkgLength = BitConverter.ToUInt16(new byte[] {0, 0, this.ReceiveBuffer[3], this.ReceiveBuffer[2] }.Reverse().ToArray(), 0);

                if (this.ReceiveBuffer.Count >= 4 + pkgLength + 4)
                {
                    List<byte> payload = new List<byte>();
                    for (int x = 0; x < pkgLength; x++)
                        payload.Add(this.ReceiveBuffer[4 + x]);

                    UInt32 crc32 = BitConverter.ToUInt32(CRC.CRC32(payload.ToArray()).Reverse().ToArray(), 0);
                    UInt32 crc32_act = BitConverter.ToUInt32(new byte[]
                    {
                        this.ReceiveBuffer[4 + pkgLength + 0],
                        this.ReceiveBuffer[4 + pkgLength + 1],
                        this.ReceiveBuffer[4 + pkgLength + 2],
                        this.ReceiveBuffer[4 + pkgLength + 3],
                    }, 0);

                    if (crc32_act == crc32)
                    {
                        DataPacket pkt = new DataPacket(type, pkgLength);

                        for (int x = 0; x < pkgLength; x++)
                            pkt.Data.Add(this.ReceiveBuffer[4 + x]);

                        this.ReceiveBuffer.RemoveRange(0, 4 + pkgLength + 4);

                        this.OnDataPacketReceived(pkt);
                    }
                }
            }
        }
    }
}
