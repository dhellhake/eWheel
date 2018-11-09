using System.IO.Ports;
using System.Threading.Tasks;

namespace DiagMonitor
{
    enum DebugMessage
    {
        LSM6D_Yaw = 1,
        LSM6D_Pitch = 2,
        LSM6D_Roll = 3,
        SYS_SysTick = 5,
    }

    public class DiagnosisPort : Bindable
    {
        private SerialPort _SerialPort;

        public DiagnosisPort()
        {
            this._SerialPort = new SerialPort("COM3");

            this._SerialPort.BaudRate = 921600;
            this._SerialPort.Parity = Parity.None;
            this._SerialPort.StopBits = StopBits.One;
            this._SerialPort.DataBits = 8;
            this._SerialPort.Handshake = Handshake.None;
            
            this._SerialPort.Open();

            
            Task portWatch = Task.Run(async delegate 
            {
                while (true)
                {
                    while (this._SerialPort.BytesToRead < 1)
                        await Task.Delay(50);

                    DebugMessage message = (DebugMessage)this._SerialPort.ReadByte();


                    byte[] bytes = new byte[4];
                    float result = 0f;
                    switch (message)
                    {
                        case DebugMessage.LSM6D_Pitch:
                            //4 Bytes of Payload
                            for (int x = 0; x < bytes.Length; x++)
                                bytes[x] = (byte)this._SerialPort.ReadByte();

                            this.Pitch = System.BitConverter.ToSingle(bytes, 0); ;
                            OnPropertyChanged("Pitch");
                            break;

                        case DebugMessage.SYS_SysTick:
                            //4 Bytes of Payload
                            for (int x = 0; x < bytes.Length; x++)
                                bytes[x] = (byte)this._SerialPort.ReadByte();

                            this.SysTick = System.BitConverter.ToInt32(bytes, 0);
                            OnPropertyChanged("SysTick");
                            break;
                    }
                }
            });
        }

        public int SysTick { get; set; }

        public float Pitch { get; set; }
    }
}
