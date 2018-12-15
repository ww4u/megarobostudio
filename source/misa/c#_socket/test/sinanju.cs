using System;

namespace mega_device
{
    public class Sinanju : MegaRobo
    {
        public Sinanju()
        {
        }

        public int move(int ax, int page, int mode,
                         float x1, float y1, float z1, float h1,
                         float x2, float y2, float z2, float h2,
                         float t)
        {
            return roboSet(string.Format("MVOE {0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11}",
                                           ax, page, mode,
                                           x1, y1, z1, h1,
                                           x2, y2, z2, h2,
                                           t));
        }

        public int preMove(int ax, int page, int mode,
                         float x1, float y1, float z1, float h1,
                         float x2, float y2, float z2, float h2,
                         float t)
        {
            return roboSet(string.Format("PREMVOE {0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11}",
                                           ax, page, mode,
                                           x1, y1, z1, h1,
                                           x2, y2, z2, h2,
                                           t));
        }

        public int goTo(int ax, int page, int mode,
                         float x, float y, float z,
                         float t)
        {
            return roboSet(string.Format("GOTO {0},{1},{2},{3},{4},{5},{6}",
                                           ax, page, mode,
                                           x, y, z,
                                           t));
        }

        public int stepX(int ax, int page, int mode,
                          float d, float t)
        {
            return roboSet(string.Format("STEP:X {0},{1},{2},{3},{4}",
                                           ax, page, mode,
                                           d,
                                           t));
        }

        public int stepY(int ax, int page, int mode,
                          float d, float t)
        {
            return roboSet(string.Format("STEP:Y {0},{1},{2},{3},{4}",
                                           ax, page, mode,
                                           d,
                                           t));
        }

        public int stepZ(int ax, int page, int mode,
                          float d, float t)
        {
            return roboSet(string.Format("STEP:Z {0},{1},{2},{3},{4}",
                                           ax, page, mode,
                                           d,
                                           t));
        }

        public int stepH(int ax, int page, int mode,
                          float d, float t)
        {
            return roboSet(string.Format("STEP:H {0},{1},{2},{3},{4}",
                                           ax, page, mode,
                                           d,
                                           t));
        }

        public int center(int ax, int page )
        {
            return roboSet(string.Format("CENTER {0},{1}",
                                           ax, page));
        }

        public int fold( int ax = 0, int page = 0 )
        {
            return roboSet(string.Format("FOLD {0},{1}", ax, page ) );
        }

        public int getPose(out float val0,
                            out float val1,
                            out float val2,
                            out float val3)
        {
            return roboGet_float_4( string.Format("POSE?"),
                                    out val0,
                                    out val1,
                                    out val2,
                                    out val3 );
        }

        public int getDistance(out float val0,
                            out float val1,
                            out float val2,
                            out float val3)
        {
            return roboGet_float_4( string.Format("DISTANCE?"),
                                    out val0,
                                    out val1,
                                    out val2,
                                    out val3);
        }

        public int getRad(out float val0,
                            out float val1,
                            out float val2,
                            out float val3)
        {
            return roboGet_float_4(string.Format("JOINT:RAD?"),
                                    out val0,
                                    out val1,
                                    out val2,
                                    out val3);
        }

    }
}
