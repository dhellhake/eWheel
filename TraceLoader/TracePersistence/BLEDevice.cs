using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TracePersistence.utility;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Devices.Enumeration;
using Windows.Storage.Streams;

namespace TracePersistence
{
    public enum BLEDeviceStatus
    {
        Enumerating,
        Connected,
        Receiving
    }

    public enum BLETransmissionStatus
    {
        Idle,
        Sending,
        Waiting
    }

    public enum BLESwc
    {
        INVALID = 0,
        SELF = 1,
        ComLink = 2
    }

    [Flags]
    public enum BLEFlag
    {
        NONE = 0,
        ACK = 1,
        FIN = 2
    }

    public enum BLECmd
    {
        INVALID = 0,
        ACK_CMD = 1,

        SET_FLASHPAGE = 2,
        REQUEST_FLASHPAGE = 3,
        RESPONSE_FLASHPAGE = 3,

        SET_DRIVECONFIG = 63,
        REQUEST_DRIVECONFIG = 64,
        RESPONSE_DRIVECONFIG = 65
    }


    public class BLEDevice
    {
        public string Name { get; set; }
        public BLEDeviceStatus Status { get; private set; }
        public BLETransmissionStatus TransmissionStatus { get; private set; }

        private GattCharacteristic DeviceCharacteristic { get; set; }

        private List<byte> ReceiveBuffer { get; set; }
        private List<DataPacket> IncompletePackets { get; set; }

        public event EventHandler<DataPacketReceivedEventArgs> DataPacketReceived;

        public BLEDevice(string name)
        {
            this.Name = name;
            this.Status = BLEDeviceStatus.Enumerating;
            this.TransmissionStatus = BLETransmissionStatus.Idle;
            this.ReceiveBuffer = new List<byte>();
            this.IncompletePackets = new List<DataPacket>();

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
            deviceWatcher.Removed += DeviceWatcher_Removed;

            // Start the watcher.
            deviceWatcher.Start();
        }


        #region Enumerating & Connecting
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
            BluetoothLEDevice bleDevice = await BluetoothLEDevice.FromIdAsync(deviceInfo.Id);
            GattDeviceServicesResult result = await bleDevice.GetGattServicesAsync();

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
                            catch
                            {
                            }
                        }
                    }
                }
            }
        }
        #endregion

        #region Transmission
        public async void SendData(BLECmd cmd, byte[] data, BLESwc srcSwc, BLESwc dstSwc)
        {
            await Task.Run(() =>
            {
                this.TransmissionStatus = BLETransmissionStatus.Sending;

                int dataIndex = 0;
                while (dataIndex < data.Length)
                {
                    while (this.TransmissionStatus == BLETransmissionStatus.Waiting)
                        Task.Delay(1).Wait();

                    if (data.Length - dataIndex > 64) //more than one byte
                    {
                        List<byte> payload = new List<byte>();
                        for (int x = dataIndex; x < dataIndex + 64; x++)
                            payload.Add(data[x]);

                        this.SendDataPacket(cmd, payload.ToArray(), srcSwc, dstSwc, (UInt16)dataIndex, BLEFlag.NONE);
                        this.TransmissionStatus = BLETransmissionStatus.Waiting;
                        dataIndex += 64;
                    }
                    else if (data.Length - dataIndex > 0)
                    {
                        List<byte> payload = new List<byte>();
                        for (int x = dataIndex; x < dataIndex + (data.Length - dataIndex); x++)
                            payload.Add(data[x]);

                        int rst = payload.Count % 4;
                        if (rst != 0)
                            for (int x = 0; x < 4 - rst; x++)
                                payload.Add(0);


                        this.SendDataPacket(cmd, payload.ToArray(), srcSwc, dstSwc, (UInt16)dataIndex, BLEFlag.FIN);
                        this.TransmissionStatus = BLETransmissionStatus.Idle;
                        break;
                    }
                }
            });
        }

        private void SendDataPacket(BLECmd cmd, byte[] bytes, BLESwc srcSwc, BLESwc dstSwc, UInt16 sequence, BLEFlag flags)
        {
            List<byte> payload = new List<byte>();
            payload.AddRange(bytes);

            if (bytes.Length > 0)
                for (int x = bytes.Length; x < 4; x++)
                    payload.Add(0);

            byte sSwc = (byte)srcSwc;
            byte dSwc = (byte)dstSwc;
            UInt16 length = (UInt16)payload.Count;

            List<byte> packet = new List<byte>();
            packet.Add(sSwc);
            packet.Add(dSwc);
            packet.AddRange(BitConverter.GetBytes(sequence));
            packet.AddRange(BitConverter.GetBytes(length));
            packet.Add((byte)flags);
            packet.Add((byte)cmd);
            packet.AddRange(CRC.CRC32(payload.ToArray()).Reverse().ToArray());
            packet.AddRange(payload);

            this.WriteBytes(packet.ToArray());
        }

        private void SendAck(BLESwc srcSwc, BLESwc dstSwc)
        {
            this.SendDataPacket(BLECmd.ACK_CMD, new byte[] { }, srcSwc, dstSwc, 0, BLEFlag.ACK);
        }

        public async void WriteBytes(byte[] bytes)
        {
            var writer = new DataWriter();
            writer.WriteBytes(bytes);

            GattCommunicationStatus result = await this.DeviceCharacteristic.WriteValueAsync(writer.DetachBuffer());
            if (result != GattCommunicationStatus.Success)
                throw new Exception("Transmission failed");
        }
        #endregion

        #region Receiving
        protected virtual void OnDataPacketReceived(DataPacket dataPacket)
        {
            this.DataPacketReceived?.Invoke(this, new DataPacketReceivedEventArgs(dataPacket));
        }

        private void Characteristic_ValueChangedAsync(GattCharacteristic sender, GattValueChangedEventArgs args)
        {
            byte[] data = new byte[args.CharacteristicValue.Length];
            DataReader.FromBuffer(args.CharacteristicValue).ReadBytes(data);

            //Buffer validation
            if (this.ReceiveBuffer.Count == 0)
                if (data.Length < 8 || data[7] == (byte)BLECmd.INVALID)
                    return;

            this.ReceiveBuffer.AddRange(data);

            if (this.ReceiveBuffer.Count >= 8 + 4)
            {
                BLESwc sSwc = (BLESwc)this.ReceiveBuffer[0];
                BLESwc dSwc = (BLESwc)this.ReceiveBuffer[1];
                UInt16 sequence = BitConverter.ToUInt16(new byte[] { 0, 0, this.ReceiveBuffer[2], this.ReceiveBuffer[3] }.Reverse().ToArray(), 0);
                UInt16 length = BitConverter.ToUInt16(new byte[] { 0, 0, this.ReceiveBuffer[4], this.ReceiveBuffer[5] }.Reverse().ToArray(), 0);
                BLEFlag flags = (BLEFlag)this.ReceiveBuffer[6];
                BLECmd cmd = (BLECmd)this.ReceiveBuffer[7];

                if ((flags & BLEFlag.ACK) == BLEFlag.ACK)
                {
                    this.ReceiveBuffer.RemoveRange(0, 8 + 4);
                    this.TransmissionStatus = BLETransmissionStatus.Sending;
                } else
                {
                    UInt32 crc32_act = BitConverter.ToUInt32(new byte[]
                    {
                        this.ReceiveBuffer[8 + 0],
                        this.ReceiveBuffer[8 + 1],
                        this.ReceiveBuffer[8 + 2],
                        this.ReceiveBuffer[8 + 3],
                    }, 0);


                    if (length == 0)
                    {
                        this.OnDataPacketReceived(new DataPacket(cmd, sSwc, dSwc));
                        // Remove Packet
                        this.ReceiveBuffer.RemoveRange(0, 12);
                    }
                    else if (this.ReceiveBuffer.Count >= length + 12)
                    {
                        List<byte> payload = new List<byte>();
                        for (int x = 0; x < length; x++)
                            payload.Add(this.ReceiveBuffer[12 + x]);

                        UInt32 crc32 = BitConverter.ToUInt32(CRC.CRC32(payload.ToArray()).Reverse().ToArray(), 0);

                        if (crc32_act == crc32)
                        {
                            DataPacket parentPacket = this.IncompletePackets.FirstOrDefault(pP => pP.SourceSwc == sSwc && pP.DestinationSwc == dSwc);
                            if (parentPacket == null)
                            {
                                parentPacket = new DataPacket((BLECmd)data[7], sSwc, dSwc);
                                this.IncompletePackets.Add(parentPacket);
                            }

                            for (int x = sequence; x < sequence + length; x++)
                                parentPacket.Data[x] = payload[x - sequence];

                            if ((flags & BLEFlag.FIN) == BLEFlag.FIN)
                            {
                                this.OnDataPacketReceived(parentPacket);
                                this.IncompletePackets.Remove(parentPacket);
                            }
                            else
                                this.SendAck(sSwc, dSwc);
                        }
                        else
                            Console.WriteLine("CRC missmatch");

                        // Remove Packet
                        this.ReceiveBuffer.RemoveRange(0, 8 + length + 4);
                    }

                }
            }
        }
        #endregion
    }
}
