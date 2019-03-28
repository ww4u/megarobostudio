using System;
//using String;
namespace mega_device
{
    public class MRH_T : MegaRobo
    {
        public MRH_T()
        {
        }

        /// <summary>
        /// yOut
        /// </summary>
        /// <param name="y12">Y1,Y2</param>
        /// <param name="hl">H,L</param>
        /// <returns></returns>
        public int setYOut(string y12, string hl)
        {
            return roboSet(string.Format("PROJECT:YWRITE {0},{1}",
                                           y12, hl
                                           ));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="x1234">X1,X2,X3,X4</param>
        /// <param name="hl"></param>
        /// <returns></returns>
        public int getXIn(string x1234, out int hl)
        {
            hl = 0;

            string recvStr;
            int ret;
            ret = roboGet_str(string.Format("PROJECT:XREAD? {0}", x1234 ),
                        out recvStr );
            if (ret != 0)
            { return ret;  }

            recvStr = recvStr.Trim();
            //Console.WriteLine(recvStr);
            //! split the x read
            if (string.Compare(recvStr, "L", true) == 0)
            { hl = 0; }
            else if (string.Compare(recvStr, "H", true) == 0)
            { hl = 1; }
            else
            { return -1; }

            return 0;
        }
    }
}
