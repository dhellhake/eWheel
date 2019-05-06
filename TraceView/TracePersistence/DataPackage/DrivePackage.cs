using System;

namespace TracePersistence
{
    public class DrivePackage : DataPackage
    {
        public static readonly int PayLoadLength = 8;

        public int DriveState { get; private set; }
        public float AvlRelACPD { get; private set; }

        private DrivePackage()
        { }
        public DrivePackage(int timeStamp, byte[] payLoad)
            : base(timeStamp)
        {
            this.DriveState = payLoad[0];
            this.AvlRelACPD = BitConverter.ToSingle(payLoad, 4);
        }
    }
    
    public class DrivePackageEventArgs : EventArgs
    {
        public DrivePackage DataPackage { get; private set; }

        public DrivePackageEventArgs(DrivePackage package)
        {
            this.DataPackage = package;
        }
    }
}
