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
            string[] ports = SerialPort.GetPortNames();
            this._SerialPort = new SerialPort("COM4");

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
                    while (this._SerialPort.BytesToRead > 0)
                        _SerialPort.Write(new byte[] { byte.Parse(this._SerialPort.ReadByte() + "") }, 0, 1);

                    await Task.Delay(100);                  
                }
            });
        }

        public int SysTick { get; set; }

        public float Pitch { get; set; }
    }
}
