using System;

namespace TracePersistence
{
    public class DataPacket
    {
        public BLESwc SourceSwc { get; private set; }
        public BLESwc DestinationSwc { get; private set; }
        public byte[] Data { get; private set; }

        protected DataPacket()
        {
            this.Data = new byte[536];
        }
        public DataPacket(BLESwc sSwc, BLESwc dSwc)
            : this()
        {
            this.SourceSwc = sSwc;
            this.DestinationSwc = dSwc;
        }
    }

    public class DataPacketReceivedEventArgs : EventArgs
    {
        public DataPacket DataPacket { get; private set; }

        public DataPacketReceivedEventArgs(DataPacket packet)
        {
            this.DataPacket = packet;
;
        }
    }
}
