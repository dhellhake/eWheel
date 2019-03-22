using System;

namespace TracePersistence
{
    public class OrientationPackage : DataPackage
    {
        public static readonly int PayLoadLength = 8;

        public float Roll { get; private set; }
        public float Pitch { get; private set; }

        private OrientationPackage()
        { }
        public OrientationPackage(int timeStamp, byte[] payLoad)
            : base(timeStamp)
        {
            this.Roll = BitConverter.ToSingle(payLoad, 0);
            this.Pitch = BitConverter.ToSingle(payLoad, 4);
        }
    }
}
