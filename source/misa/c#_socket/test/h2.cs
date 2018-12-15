using System;

namespace mega_device
{
    public class H2 : MegaRobo
    {
        public H2()
        {
        }

        public int move(float x1, float y1, float x2, float y2, float dt, int ax = 0, int page = 0)
        {
            return roboSet(string.Format("MOVE {0},{1},{2},{3},{4},{5},{6}",
                                           ax, page,
                                           x1, y1, x2, y2, dt
                                           ) );
        }

        public int center( int ax = 0,int page = 1 )
        {
            return roboSet(string.Format("ZERO {0},{1}",
                                           ax, page
                                           ));
        }

        public float[] pose()
        {
            int ret;
            string recv;
            int retCount;
            ret = miQuery("POSE?", out recv, out retCount);
            if (ret != 0)
            { return null; }

            string[] buf = recv.Split(',');
            if (buf.Length < 2)
            { return null; }

            //! convert the value 
            float[] vals;

            //! convert
            try
            {
                vals = new float[2];
                vals[0] = Convert.ToSingle(buf[0]);
                vals[1] = Convert.ToSingle(buf[1]);
            }
            catch (Exception e)
            {
                vals = null;
            }

            return vals;
        }

        public string[] GetH2Posi()
        {
            float[] poses = pose();
            if (null == poses)
            { return null;  }

            //! convert the values
            string[] vals;
            try
            {
                vals = new string[2];

                vals[0] = string.Format("{0}", poses[0]);
                vals[1] = string.Format("{0}", poses[1]);
            }
            catch (Exception e)
            { vals = null;  }

            return vals;
        }
    }
}
