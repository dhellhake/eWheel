using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
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
                Thread.Sleep(20);
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
            }   
        }

        private static List<DataPacket> PacketBuffer = new List<DataPacket>();

        private static void EWheel_DataPacketReceived(object sender, DataPacketReceivedEventArgs e)
        {
            PacketBuffer.Add(e.DataPacket);
        }

        private static void LoadTraceFromDevice()
        {
            List<byte> payload = new List<byte>();

            // Write page 0
            payload.Clear();
            payload.Add(3);
            payload.Add(0);
            payload.Add(0);

            for (int x = 0; x < 528; x++)
                payload.Add((byte)x);

            Program.eWheel.SendData(payload.ToArray(), BLESwc.SELF, BLESwc.ComLink);
            while (PacketBuffer.Count < 1)
                Thread.Sleep(10);

            // Read page 0
            payload.Clear();
            payload.Add(2);
            payload.Add(0);
            payload.Add(0);
            Program.eWheel.SendData(payload.ToArray(), BLESwc.SELF, BLESwc.ComLink);
            while (PacketBuffer.Count < 2)
                Thread.Sleep(10);
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
