using System;
using System.Collections.Generic;
using Windows.Foundation.Metadata;

namespace TracePersistence
{
    public class DataPacket
    {
        public UInt16 Type { get; private set; }
        public UInt16 Length { get; private set; }

        public List<byte> Data { get; private set; }
        protected DataPacket()
        {
            this.Data = new List<byte>();
        }
        public DataPacket(UInt16 type, UInt16 length)
            : this()
        {
            this.Type = type;
            this.Length = length;
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
