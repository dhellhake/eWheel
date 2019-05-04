using System;

namespace TracePersistence
{
    public class DrivePackage : DataPackage
    {
        public static readonly int PayLoadLength = 1;

        public int DriveState { get; private set; }

        private DrivePackage()
        { }
        public DrivePackage(int timeStamp, byte[] payLoad)
            : base(timeStamp)
        {
            this.DriveState = payLoad[0];
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
