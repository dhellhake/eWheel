using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.IO.Ports;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace DiagMonitor
{
    enum DebugMessage
    {
        LSM6D_Acc_X = 1,
        LSM6D_Acc_Y = 2,
        LSM6D_Acc_Z = 3,
        LSM6D_Gyro_X = 4,
        LSM6D_Gyro_Y = 5,
        LSM6D_Gyro_Z = 6,
    }

    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application
    {
        private void AppStartUp()
        {
            SerialPort mySerialPort = new SerialPort("COM3");

            mySerialPort.BaudRate = 921600;
            mySerialPort.Parity = Parity.None;
            mySerialPort.StopBits = StopBits.One;
            mySerialPort.DataBits = 8;
            mySerialPort.Handshake = Handshake.None;


            mySerialPort.Open();

            mySerialPort.DiscardInBuffer();


            byte low;
            sbyte high;
            float accX = 0f, accY = 0f, accZ = 0f;
            float gyrX = 0f, gyrY = 0f, gyrZ = 0f;

            if (System.IO.File.Exists(@"C:\Users\domin\OneDrive\Desktop\cache\record.csv"))
                System.IO.File.Delete(@"C:\Users\domin\OneDrive\Desktop\cache\record.csv");

            while (true)
            {
                while (mySerialPort.BytesToRead < 3)
                    System.Threading.Thread.Sleep(100);

                DebugMessage message = (DebugMessage)mySerialPort.ReadByte();

                switch (message)
                {
                    case DebugMessage.LSM6D_Acc_X:
                        low = Convert.ToByte(mySerialPort.ReadByte());
                        high = unchecked((sbyte)mySerialPort.ReadByte());

                        accX = (float)(((int)low) | ((int)(high << 8))) * 0.061f * (2 >> 1) / 1000f;
                        break;
                    case DebugMessage.LSM6D_Acc_Y:
                        low = Convert.ToByte(mySerialPort.ReadByte());
                        high = unchecked((sbyte)mySerialPort.ReadByte());

                        accY = (float)(((int)low) | ((int)(high << 8))) * 0.061f * (2 >> 1) / 1000f;


                        break;
                    case DebugMessage.LSM6D_Acc_Z:
                        low = Convert.ToByte(mySerialPort.ReadByte());
                        high = unchecked((sbyte)mySerialPort.ReadByte());

                        accZ = (float)(((int)low) | ((int)(high << 8))) * 0.061f * (2 >> 1) / 1000f;

                        break;


                    case DebugMessage.LSM6D_Gyro_X:
                        low = Convert.ToByte(mySerialPort.ReadByte());
                        high = unchecked((sbyte)mySerialPort.ReadByte());

                        gyrX = (float)(((int)low) | ((int)(high << 8))) * 0.061f * (2 >> 1) / 1000f;

                        break;
                    case DebugMessage.LSM6D_Gyro_Y:
                        low = Convert.ToByte(mySerialPort.ReadByte());
                        high = unchecked((sbyte)mySerialPort.ReadByte());

                        gyrY = (float)(((int)low) | ((int)(high << 8))) * 0.061f * (2 >> 1) / 1000f;

                        break;
                    case DebugMessage.LSM6D_Gyro_Z:
                        low = Convert.ToByte(mySerialPort.ReadByte());
                        high = unchecked((sbyte)mySerialPort.ReadByte());

                        gyrZ = (float)(((int)low) | ((int)(high << 8))) * 0.061f * (2 >> 1) / 1000f;

                        break;
                }

                if (accX != 0f && accY != 0f && accZ != 0 && gyrX != 0f && gyrY != 0f && gyrZ != 0)
                {
                    //double pitch = Math.Atan(accY / (Math.Sqrt(Math.Pow(accX, 2) + Math.Pow(accZ, 2))));
                    //double roll = Math.Atan((accX * -1) / accZ);

                    double pitch = (Math.Atan2(accX, Math.Sqrt(accY * accY + accZ * accZ)) * 180.0) / Math.PI;
                    double roll = (Math.Atan2(-accY, accZ) * 180.0) / Math.PI;
                    double yaw = 180 * Math.Atan(accZ / Math.Sqrt(accX * accX + accZ * accZ)) / Math.PI;


                    accX = 0f;
                    accY = 0f;
                    accZ = 0f;
                    gyrX = 0f;
                    gyrY = 0f;
                    gyrZ = 0f;
                }
            }

    }
}
