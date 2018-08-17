using System;

namespace mega_device
{
    public class MRQ : CS_MRQ
    {
        public MRQ() 
        {
        }

        public int move(int ax, int page, float t, float angle, float endV)
        {
            return roboSet(string.Format("MOVE {0},{1},{2},{3},{4}",
                                          ax, page,
                                          t, angle,
                                          endV ) );
        }

        public int preMove(int ax, int page, float t, float angle, float endV)
        {
            return roboSet(string.Format("PREMOVE {0},{1},{2},{3},{4}",
                                              ax, page,
                                              t, angle,
                                              endV));
        }

        public int getIncAngle(int ax, out float val0)
        {
            return roboGet_float(string.Format("ANGLE:INCREASE? {0}", ax),
                                   out val0);
        }

        public int getAbsAngle( int ax, out float val0 )
        {
            return roboGet_float(string.Format("ANGLE:ABSOLUTE? {0}", ax),
                                   out val0);
        }

        public int getDistance(int ax, out float val0)
        {
            return roboGet_float(string.Format("DISTANCE? {0}", ax),
                                   out val0);
        }

        public int setFanDuty(int duty)
        {
            return roboSet(string.Format("FANDUTY {0}", duty));
        }

        public int setLedDuty(int ax, int duty)
        {
            return roboSet(string.Format("LEDDUTY {0},{1}", ax, duty));
        }

        public int setFan(int duty, int freq)
        {
            return roboSet(string.Format("FAN {0},{1},{2}", 0,duty, freq));
        }

        public int setLed(int ax, int duty, int freq)
        {
            return roboSet(string.Format("LED {0},{1},{2}", ax, duty, freq));
        }

        public int getZeroValid(out int valid)
        {
            return roboGet_int(string.Format("ENCODER:ZEROVALID?"), out valid );
        }

        public int getZero(int ax, out float val)
        {
            return roboGet_float(string.Format("ENCODER:ZERO? {0}", ax), out val);
        }
    }

}
