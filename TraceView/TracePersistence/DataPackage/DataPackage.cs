namespace TracePersistence
{
    public class DataPackage
    {
        public int TimeStamp { get; private set; } 

        protected DataPackage()
        { }
        public DataPackage(int timeStamp)
        {
            this.TimeStamp = TimeStamp;
        }
    }
}
