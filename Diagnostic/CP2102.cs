using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Threading;

namespace Diagnostic
{
    public enum DIAGNOSTIC_CMD : byte
    {
        REQUEST_DIAGNOSTIC_MODE = 0x01,
        RESPONSE_DIAGNOSTIC_MODE = 0x02,

        REQUEST_SECTOR_ERASE = 0x03,
        RESPONSE_SECTOR_ERASE = 0x04,

        REQUEST_MODE = 0x05,
        RESPONSE_MODE = 0x06,

        REQUEST_FLASH_PAGE = 0x07,
        RESPONSE_FLASH_PAGE = 0x08,

        REQUEST_ENABLE_TRACE = 0x09,

        SW_RESET = 0xFF
    };

    public enum DIAGNOSTIC_MODE : byte
    {
        INVALID = 0x00,
	    TRACE = 0x01,
	    DIAGNOSTIC = 0x02
    };

    public enum SECTOR_ERASE_RESULT : byte
    {
        BUSY_REPEAT = 0x01,
        SUCCESS = 0x02,
        INVALID_MODE = 0x03,
        ERROR = 0x04
    };

    public class CP2102
    {
        private SerialPort SerialPort;

        private List<byte> ReceiveBuffer { get; set; }

        public CP2102()
        {
            string[] portNames = SerialPort.GetPortNames();
            this.SerialPort = new SerialPort("COM5", 921600, Parity.None, 8, StopBits.One);

            this.SerialPort.DataReceived += SerialPort_DataReceived; ;
            this.SerialPort.Open();

            this.ReceiveBuffer = new List<byte>();
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            List<byte> received = new List<byte>();

            while (((SerialPort)sender).BytesToRead > 0)
                received.Add((byte)((SerialPort)sender).ReadByte());

            if (this.ReceiveBuffer.Count > 0 || received[0] == 0xAA)
                this.ReceiveBuffer.AddRange(received);
        }


        public List<byte> RequestFlashPage(short pageIndex)
        {
            List<byte> message = new List<byte>();
            message.Add(0xAA);
            message.AddRange(BitConverter.GetBytes((short)2).Reverse());
            message.Add((byte)DIAGNOSTIC_CMD.REQUEST_FLASH_PAGE);
            message.AddRange(BitConverter.GetBytes(pageIndex).Reverse());

            this.SerialPort.Write(message.ToArray(), 0, message.Count);

            int timeout = 10000;

            while (timeout > 0)
            {
                //timeout -= 100;
                Thread.Sleep(100);
                if (this.ReceiveBuffer.Count >= 256 + 4 &&
                    ((DIAGNOSTIC_CMD)this.ReceiveBuffer[3]) == DIAGNOSTIC_CMD.RESPONSE_FLASH_PAGE)
                {
                    List<byte> result = new List<byte>();
                    result.AddRange(this.ReceiveBuffer.GetRange(4, 256));
                    this.ReceiveBuffer.RemoveRange(0, 256 + 4);
                    return result;
                }
            }

            return null;
        }

        public bool RequestDiagnosticMode()
        {
            List<byte> message = new List<byte>();
            message.Add(0xAA);
            message.AddRange(BitConverter.GetBytes((short)4).Reverse());
            message.Add((byte)DIAGNOSTIC_CMD.REQUEST_DIAGNOSTIC_MODE);
            message.Add(0xDE);
            message.Add(0xAD);
            message.Add(0xBE);
            message.Add(0xEF);

            this.SerialPort.Write(message.ToArray(), 0, message.Count);
            Thread.Sleep(500);

            int timeout = 10000;

            while (timeout > 0 && this.RequestMode() != DIAGNOSTIC_MODE.DIAGNOSTIC)
            {
                timeout -= 100;
                Thread.Sleep(100);
            }

            return timeout > 0;
        }

        public void SWReset()
        {
            List<byte> message = new List<byte>();
            message.Add(0xAA);
            message.AddRange(BitConverter.GetBytes((short)4).Reverse());
            message.Add((byte)DIAGNOSTIC_CMD.SW_RESET);
            message.Add(0xDE);
            message.Add(0xAD);
            message.Add(0xBE);
            message.Add(0xEF);

            this.SerialPort.Write(message.ToArray(), 0, message.Count);
        }


        public void EnableTrace()
        {
            List<byte> message = new List<byte>();
            message.Add(0xAA);
            message.AddRange(BitConverter.GetBytes((short)0).Reverse());
            message.Add((byte)DIAGNOSTIC_CMD.REQUEST_ENABLE_TRACE);

            this.SerialPort.Write(message.ToArray(), 0, message.Count);
        }

        public SECTOR_ERASE_RESULT SectorErase(short sectorIndex)
        {
            List<byte> message = new List<byte>();
            message.Add(0xAA);
            message.AddRange(BitConverter.GetBytes((short)2).Reverse());
            message.Add((byte)DIAGNOSTIC_CMD.REQUEST_SECTOR_ERASE);
            message.AddRange(BitConverter.GetBytes(sectorIndex).Reverse());

            this.SerialPort.Write(message.ToArray(), 0, message.Count);

            int timeout = 10000;

            while (timeout > 0)
            {
                timeout -= 200;
                Thread.Sleep(200);

                if (this.ReceiveBuffer.Count >= 5 &&
                    ((DIAGNOSTIC_CMD)this.ReceiveBuffer[3]) == DIAGNOSTIC_CMD.RESPONSE_SECTOR_ERASE)
                {
                    SECTOR_ERASE_RESULT result = (SECTOR_ERASE_RESULT)this.ReceiveBuffer[4];
                    this.ReceiveBuffer.RemoveRange(0, 5);
                    return result;
                }
            }

            return SECTOR_ERASE_RESULT.ERROR;
        }

        public DIAGNOSTIC_MODE RequestMode()
        {
            List<byte> message = new List<byte>();
            message.Add(0xAA);
            message.AddRange(BitConverter.GetBytes((short)0).Reverse());
            message.Add((byte)DIAGNOSTIC_CMD.REQUEST_MODE);

            this.SerialPort.Write(message.ToArray(), 0, message.Count);

            int timeout = 10000;
            while (timeout > 0)
            {
                timeout -= 100;
                Thread.Sleep(100);
                if (this.ReceiveBuffer.Count >= 5 &&
                    ((DIAGNOSTIC_CMD)this.ReceiveBuffer[3]) == DIAGNOSTIC_CMD.RESPONSE_MODE)
                {
                    DIAGNOSTIC_MODE result = (DIAGNOSTIC_MODE)this.ReceiveBuffer[4];
                    this.ReceiveBuffer.RemoveRange(0, 5);
                    return result;
                }
            }

            return DIAGNOSTIC_MODE.INVALID;
        }
    }
}
