using System;

namespace mega_device
{
    public class DevMgr : MegaRobo
    {
        /// <summary>
        /// is the operation complete
        /// </summary>
        /// <returns></returns>
        public int getOpc()
        {
            int ret;
            string recv;
            int retCount;
            ret = miQuery("*OPC?", out recv, out retCount);
            if (ret != 0)
            { return -1; }

            return Int32.Parse(recv);
        }

        /// <summary>
        /// search the bus
        /// </summary>
        public void find()
        {
            roboSet("FIND");
        }

        /// <summary>
        /// get the devices
        /// </summary>
        /// <returns></returns>
        public string[] getDevices()
        {
            int ret;
            string recv;
            int retCount;
            ret = miQuery("DEVICE?", out recv, out retCount);
            if (ret != 0)
            { return null; }

            return recv.Split(',');
        }

        /// <summary>
        /// get the robots
        /// </summary>
        /// <returns></returns>
        public string[] getRobots()
        {
            int ret;
            string recv;
            int retCount;
            ret = miQuery("ROBOT?", out recv, out retCount);
            if (ret != 0)
            { return null; }

            return recv.Split(',');
        }

        /// <summary>
        /// get the resource( device + robot )
        /// </summary>
        /// <returns></returns>
        public string[] getResources()
        {
            int ret;
            string recv;
            int retCount;
            ret = miQuery("RESOURCE?", out recv, out retCount);
            if (ret != 0)
            { return null; }

            return recv.Split(',');
        }

        /// <summary>
        /// open the bus
        /// </summary>
        public void openBus()
        { roboSet("BUS:OPEN"); }

        /// <summary>
        /// close the bus
        /// </summary>
        public void closeBus()
        { roboSet("BUS:CLOSE"); }

        /// <summary>
        /// stop the devices on the bus
        /// </summary>
        public void stop()
        { roboSet("DEVICE:STOP"); }

        /// <summary>
        /// terminate the devices
        /// </summary>
        public void terminate()
        { roboSet("DEVICE:TERMINATE"); }

        /// <summary>
        /// reset the devices
        /// </summary>
        public void reset()
        { roboSet("DEVICE:RESET");  }
    }
}