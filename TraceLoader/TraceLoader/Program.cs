using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using TracePersistence;
using TracePersistence.utility;

namespace TraceLoader
{
    public class Program
    {
        private static readonly string path = @"C:\Users\domin\Desktop\cache\Traces\";

        private static BLEDevice eWheel { get; set; }

        public static void Main(string[] args)
        {
            Program.eWheel = new BLEDevice("CC41-A");
            Program.eWheel.DataPacketReceived += EWheel_DataPacketReceived;

            Console.Write("Connecting...");
            while (Program.eWheel.Status != BLEDeviceStatus.Connected)
                Thread.Sleep(200);
            Console.WriteLine(".. Done!");


            while (true)
            {
                string inputLine = Console.ReadLine();

                if (inputLine.ToLower().Trim() == "exit")
                    break;
                else if (inputLine.ToLower().Trim() == "load")
                    LoadTraceFromDevice();
                else if (inputLine.ToLower().Trim().StartsWith("convert"))
                    ConvertBinToCsv(inputLine.Split(' ')[1]);
                if (inputLine.ToLower().Trim().StartsWith("test"))
                    Test(Convert.ToInt32(inputLine.Split(' ')[1]));
            }   
        }

        private static void EWheel_DataPacketReceived(object sender, DataPacketReceivedEventArgs e)
        {
            Console.WriteLine("Received DataPacket; Type: " + e.DataPacket.Type + " , Length: " + e.DataPacket.Length);
        }

        private static void Test(int len)
        {
            List<byte> payload = new List<byte>(); 
            UInt32[] crc_data = new UInt32[] {
                0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444,
                0x55555555, 0x66666666, 0x77777777, 0x88888888, 0x99999999, 0x44444444
            };
            foreach (UInt32 cd in crc_data)
                payload.AddRange(BitConverter.GetBytes(cd));

            UInt16 type = BitConverter.ToUInt16(BitConverter.GetBytes(90), 0);
            UInt16 length = (UInt16)payload.Count;
                        

            List<byte> packet = new List<byte>();
            packet.AddRange(BitConverter.GetBytes(type));
            packet.AddRange(BitConverter.GetBytes(length));
            packet.AddRange(payload);
            packet.AddRange(CRC.CRC32(payload.ToArray()));

            Program.eWheel.WriteBytes(packet.ToArray());
        }

        private static void LoadTraceFromDevice()
        {

        }

        private static void ConvertBinToCsv(string pathToBin)
        {
            if (!File.Exists(pathToBin) || !pathToBin.ToLower().Trim().EndsWith(".bin"))
            {
                Console.WriteLine("not a valid file!");
                return;
            }
            string fileName = Path.GetFileName(pathToBin.Split('.')[0]) + ".csv";


        }

        private static string GetFileName()
        {
            string result = string.Empty;

            result += "Trace_" 
                + DateTime.Now.Year 
                + "-" + (DateTime.Now.Month < 10 ? "0" + DateTime.Now.Month.ToString() : DateTime.Now.Month.ToString())
                + "-" + (DateTime.Now.Day < 10 ? "0" + DateTime.Now.Day.ToString() : DateTime.Now.Day.ToString())
                + "-" + (DateTime.Now.Hour < 10 ? "0" + DateTime.Now.Hour.ToString() : DateTime.Now.Hour.ToString())
                + "-" + (DateTime.Now.Minute < 10 ? "0" + DateTime.Now.Minute.ToString() : DateTime.Now.Minute.ToString());

            return result;
        }
    }
}
