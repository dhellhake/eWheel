using System;
using System.Collections.Generic;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Devices.Enumeration;
using Windows.Storage.Streams;

namespace TracePersistence
{

    /// <summary>
    /// BLEService == DEBUG_CMD
    /// </summary>
    public enum BLEService
    {
        None = 0,
        Orientation = 1
    }

    public enum BLEDeviceStatus
    {
        Connected,
        Enumerating
    }

    public enum ReceiveState
    {
        Idle = 0,
        ReceiveOrientation = 1
    }

    public class BLEDevice
    {
        public string Name { get; set; }
        public BLEDeviceStatus Status { get; private set; }

        private GattCharacteristic DeviceCharacteristic { get; set; }

        private ReceiveState ReceiveState { get; set; }
        private List<byte> ReceiveBuffer { get; set; }

        public event EventHandler<OrientationEventArgs> OrientationPackageReceived;

        public BLEDevice(string name)
        {
            this.Name = name;
            this.ReceiveState = ReceiveState.Idle;
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


        public void Subscribe(BLEService service)
        {
            this.WriteBytes(new byte[] { (byte)service, 1 });
        }
        public void UnSubscribe(BLEService service)
        {
            this.WriteBytes(new byte[] { (byte)service, 0 });
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

        private async void WriteBytes(byte[] bytes)
        {
            var writer = new DataWriter();
            writer.WriteBytes(bytes);

            GattCommunicationStatus result = await this.DeviceCharacteristic.WriteValueAsync(writer.DetachBuffer());
            if (result == GattCommunicationStatus.Success)
            {
                // Successfully wrote to device
            }
        }

        protected virtual void OnOrientationDataPackageReceived(OrientationPackage package)
        {
            this.OrientationPackageReceived?.Invoke(this, new OrientationEventArgs(package));
        }

        private void Characteristic_ValueChangedAsync(GattCharacteristic sender, GattValueChangedEventArgs args)
        {
            byte[] data = new byte[args.CharacteristicValue.Length];
            DataReader.FromBuffer(args.CharacteristicValue).ReadBytes(data);

            foreach (byte b in data)
            {
                switch (this.ReceiveState)
                {
                    case ReceiveState.Idle:
                        this.ReceiveBuffer.Clear();
                        this.ReceiveState = (ReceiveState)b;
                        break;
                    case ReceiveState.ReceiveOrientation:
                        this.ReceiveBuffer.Add(b);

                        if (this.ReceiveBuffer.Count == OrientationPackage.PayLoadLength + 4)
                        {
                            int timeStamp = BitConverter.ToInt32(this.ReceiveBuffer.ToArray(), 0);
                            this.ReceiveBuffer.RemoveRange(0, 4);

                            this.OnOrientationDataPackageReceived(new OrientationPackage(timeStamp, this.ReceiveBuffer.ToArray()));
                            this.ReceiveState = ReceiveState.Idle;
                        }
                        break;
                }
            }
        }
    }
}
