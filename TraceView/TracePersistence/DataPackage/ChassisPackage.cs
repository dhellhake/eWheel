using System;

namespace TracePersistence
{
    public class ChassisPackage : DataPackage
    {
        public static readonly int PayLoadLength = 16;

        public float Chassis_Roll { get; private set; }
        public float Chassis_Pitch { get; private set; }
        public float Road_Roll { get; private set; }
        public float Road_Pitch { get; private set; }

        private ChassisPackage()
        { }
        public ChassisPackage(int timeStamp, byte[] payLoad)
            : base(timeStamp)
        {
            this.Chassis_Pitch = BitConverter.ToSingle(payLoad, 0);
            this.Chassis_Roll = BitConverter.ToSingle(payLoad, 4);
            this.Road_Pitch = BitConverter.ToSingle(payLoad, 8);
            this.Road_Roll = BitConverter.ToSingle(payLoad, 12);
        }
    }

    public class ChassisEventArgs : EventArgs
    {
        public ChassisPackage DataPackage { get; private set; }

        public ChassisEventArgs(ChassisPackage package)
        {
            this.DataPackage = package;
        }
    }
}
