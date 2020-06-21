using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Diagnostic
{
    public class Program
    {
        private static CP2102 CP2102 = new CP2102();

        static void Main(string[] args)
        {

            while (true)
            {
                string inputLine = Console.ReadLine().ToLower().Trim();

                if (inputLine.StartsWith("exit"))
                    break;
                else if (inputLine.StartsWith("mode"))
                {
                    string[] p = inputLine.Split(' ');
                    if (p.Length > 1)
                        Program.Mode(p[1]);
                }
                else if (inputLine.StartsWith("reset"))
                    CP2102.SWReset();
                else if (inputLine.StartsWith("erase"))
                {
                    string[] p = inputLine.Split(' ');
                    if (p.Length > 1)
                        Program.SectorErase(p[1]);
                }
                else if (inputLine.StartsWith("read"))
                {
                    string[] p = inputLine.Split(' ');
                    if (p.Length > 1)
                        Program.RequestFlashPage(p[1]);
                }
                else if (inputLine.StartsWith("enable trace"))
                    CP2102.EnableTrace();


            }
        }

        private static void RequestFlashPage(string param)
        {
            short pageIndex = 0;

            if (short.TryParse(param, out pageIndex))
                PlotPage(CP2102.RequestFlashPage(pageIndex));
        }

        private static void SectorErase(string param)
        {
            short sectorIndex = 0;

            if (short.TryParse(param, out sectorIndex))
                Console.WriteLine(CP2102.SectorErase(sectorIndex));
        }

        private static void Mode(string param)
        {
            if (param.StartsWith("?"))
                Console.WriteLine(CP2102.RequestMode());
            else if (param.StartsWith("diag"))
                Console.WriteLine(CP2102.RequestDiagnosticMode());
        }



        private static void PlotPage(List<byte> page)
        {
            Console.WriteLine();
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 32; col++)
                {
                    string colstr = (page[(row * 32) + col]).ToString("X");

                    if (colstr.Length == 1)
                        colstr = "0" + colstr;

                    Console.Write(colstr + " ");
                }
                Console.WriteLine();
            }
        }
    }
}
