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
        Orientation = 1,
        VESC = 2,
        Drive = 3
    }

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

        private ReceiveState ReceiveState { get; set; }
        private List<byte> ReceiveBuffer { get; set; }

        public event EventHandler<ChassisEventArgs> OrientationPackageReceived;
        public event EventHandler<VESCEventArgs> VESCPackageReceived;
        public event EventHandler<DrivePackageEventArgs> DrivePackageReceived;

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

        protected virtual void OnOrientationPackageReceived(ChassisPackage package)
        {
            this.OrientationPackageReceived?.Invoke(this, new ChassisEventArgs(package));
        }

        protected virtual void OnVESCPackageReceived(VESCPackage package)
        {
            this.VESCPackageReceived?.Invoke(this, new VESCEventArgs(package));
        }
        protected virtual void OnDrivePackageReceived(DrivePackage package)
        {
            this.DrivePackageReceived?.Invoke(this, new DrivePackageEventArgs(package));
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
                    case ReceiveState.Orientation:
                        this.ReceiveBuffer.Add(b);

                        if (this.ReceiveBuffer.Count == ChassisPackage.PayLoadLength + 4)
                        {
                            int timeStamp = BitConverter.ToInt32(this.ReceiveBuffer.ToArray(), 0);
                            this.ReceiveBuffer.RemoveRange(0, 4);

                            this.OnOrientationPackageReceived(new ChassisPackage(timeStamp, this.ReceiveBuffer.ToArray()));
                            this.ReceiveState = ReceiveState.Idle;
                        }
                        break;
                    case ReceiveState.VESC:
                        this.ReceiveBuffer.Add(b);

                        if (this.ReceiveBuffer.Count == VESCPackage.PayLoadLength + 4)
                        {
                            int timeStamp = BitConverter.ToInt32(this.ReceiveBuffer.ToArray(), 0);
                            this.ReceiveBuffer.RemoveRange(0, 4);

                            this.OnVESCPackageReceived(new VESCPackage(timeStamp, this.ReceiveBuffer.ToArray()));
                            this.ReceiveState = ReceiveState.Idle;
                        }
                        break;
                    case ReceiveState.Drive:
                        this.ReceiveBuffer.Add(b);

                        if (this.ReceiveBuffer.Count == DrivePackage.PayLoadLength + 4)
                        {
                            int timeStamp = BitConverter.ToInt32(this.ReceiveBuffer.ToArray(), 0);
                            this.ReceiveBuffer.RemoveRange(0, 4);

                            this.OnDrivePackageReceived(new DrivePackage(timeStamp, this.ReceiveBuffer.ToArray()));
                            this.ReceiveState = ReceiveState.Idle;
                        }
                        break;
                }
            }
        }
    }
}
