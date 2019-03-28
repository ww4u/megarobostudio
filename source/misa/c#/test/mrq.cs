using System;

namespace mega_device
{
    public class MRQ : CS_MRQ
    {
        public MRQ() 
        {
        }
        /// <summary>
        /// only support "tp" and "tpv"
        /// </summary>
        /// <param name="fmt"></param>
        /// <param name="dataSets"></param>
        /// <returns></returns>
        protected override string formatDataSets(string fmt, float[] dataSets)
        {
            String localFmt;
            String fmtedStr;

            localFmt = fmt.Trim().ToLower();

            if (localFmt == "tp")
            {
                if (dataSets.Length < 1 || dataSets.Length % 2 != 0)
                { return "";  }

                //! fmt 
            }
            else if (localFmt == "tpv")
            {
                if (dataSets.Length < 1 || dataSets.Length % 3 != 0)
                { return ""; }

                //! format the data
            }
            else
            { return ""; }

            //! format 
            fmtedStr = "";
            //! format the data
            foreach (float dat in dataSets)
            {
                fmtedStr += String.Format("{0},", dat);
            }
            //! remove the last ,
            return fmtedStr.Substring(0, fmtedStr.Length - 1);
        }

        public int move(int ax, int page, float t, float angle, float endV=0.0f)
        {
            return roboSet(string.Format("MOVE {0},{1},{2},{3},{4}",
                                          ax, page,
                                          t, angle,
                                          endV ) );
        }

        public int preMove(int ax, int page, float t, float angle, float endV = 0.0f)
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

        public int getSensorUartData(string uart, string sens, out byte [] datas )
        {
            return roboGet_stream( string.Format("SENSORUART:DATA? {0},{1}", uart, sens),
                                    out datas
                                    );
        }
    }

}
