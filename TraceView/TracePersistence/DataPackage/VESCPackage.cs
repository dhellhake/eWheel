using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TracePersistence
{
    public class VESCPackage : DataPackage
    {
        public static readonly int PayLoadLength = 16;
        public float Avl_RPM { get; private set; }
        public float Avl_Duty { get; private set; }
        public float Avl_TempFET { get; private set; }
        public float Tar_Duty { get; private set; }

        private VESCPackage()
        { }
        public VESCPackage(int timeStamp, byte[] payLoad)
            : base(timeStamp)
        {
            this.Avl_RPM = BitConverter.ToSingle(payLoad, 0);
            this.Avl_Duty = BitConverter.ToSingle(payLoad, 4);
            this.Avl_TempFET = BitConverter.ToSingle(payLoad, 8);
            this.Tar_Duty = BitConverter.ToSingle(payLoad, 12);
        }
    }


    public class VESCEventArgs : EventArgs
    {
        public VESCPackage DataPackage { get; private set; }

        public VESCEventArgs(VESCPackage package)
        {
            this.DataPackage = package;
        }
    }
}
