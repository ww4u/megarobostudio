using System;

namespace mega_device
{
    public class H2 : MegaRobo
    {
        public H2()
        {
        }
        /// <summary>
        /// (x1,y1)->(x2,y2)
        /// waitIdle needed
        /// </summary>
        /// <param name="x1"></param>
        /// <param name="y1"></param>
        /// <param name="x2"></param>
        /// <param name="y2"></param>
        /// <param name="dt"></param>
        /// <param name="ax"></param>
        /// <param name="page"></param>
        /// <returns></returns>
        public int move(float x1, float y1, float x2, float y2, float dt, int ax = 0, int page = 0)
        {
            return roboSet(string.Format("MOVE {0},{1},{2},{3},{4},{5},{6}",
                                           ax, page,
                                           x1, y1, x2, y2, dt
                                           ) );
        }
        /// <summary>
        /// (x1,y1)->(x2,y2)
        /// not moving instantly
        /// waitEnd needed
        /// </summary>
        /// <param name="x1"></param>
        /// <param name="y1"></param>
        /// <param name="x2"></param>
        /// <param name="y2"></param>
        /// <param name="dt"></param>
        /// <param name="ax"></param>
        /// <param name="page"></param>
        /// <returns></returns>
        public int preMove(float x1, float y1, float x2, float y2, float dt, int ax = 0, int page = 0)
        {
            return roboSet(string.Format("PREMOVE {0},{1},{2},{3},{4},{5},{6}",
                                           ax, page,
                                           x1, y1, x2, y2, dt
                                           ));
        }

        public int stepX(float dx, float dt, int ax = 0, int page = 0)
        {
            return step(dx, 0, dt, ax, page);
        }

        public int stepY(float dy, float dt, int ax = 0, int page = 0)
        {
            return step(0, dy, dt, ax, page);
        }

        /// <summary>
        /// step dx, dy
        /// </summary>
        /// <param name="dx"></param>
        /// <param name="dy"></param>
        /// <param name="dt"></param>
        /// <param name="ax"></param>
        /// <param name="page"></param>
        /// <returns></returns>
        public int step(float dx, float dy, float dt, int ax = 0, int page = 0)
        {
            float sx, sy, tx, ty;
            if (dx < 0)
            { sx = -dx; tx = 0; }
            else
            { sx = 0; tx = dx; }

            if (dy < 0)
            { sy = -dy; ty = 0; }
            else
            { sy = 0; ty = dy; }

            return move(sx, sy, tx, ty, dt, ax, page);
        }

        /// <summary>
        /// go to the dst from the current position
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="t"></param>
        /// <param name="ax"></param>
        /// <param name="page"></param>
        /// <returns></returns>
        public int to(float x, float y, float t, int ax = 0, int page = 0)
        {
            float []curPos = pose();
            if (curPos == null)
            { return -1; }

            return move(curPos[0], curPos[1], x, y, t, ax, page);
        }

        public int center( int ax = 0,int page = 1 )
        {
            return roboSet(string.Format("ZERO {0},{1}",
                                           ax, page
                                           ));
        }
        /// <summary>
        /// current pos
        /// </summary>
        /// <returns></returns>
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
