using System;
using System.Threading;

namespace mega_device
{
    public class MegaRobo : miDevice
    {
        public const int mega_robo_unk = -1;
        public const int mega_robo_idle = 0;
        public const int mega_robo_run_reqed = 1;
        public const int mega_robo_program = 2;
        public const int mega_robo_calcing = 3;

        public const int mega_robo_calcend = 4;
        public const int mega_robo_standby = 5;
        public const int mega_robo_prerun = 6;
        public const int mega_robo_running = 7;

        public const int mera_robo_prestop = 8;


        public MegaRobo()
        {
        }

        public int roboSet( string args )
        {
            int ret;
            ret = miSend(args);
            return ret;
        }

        public int roboGet_str( string args, out string val0 )
        {
            //! init the out
            val0 = null;

            int ret;
            
            string strRecv;
            int retCount;
            ret = miQuery( args, out strRecv, out retCount);
            if (ret != 0) { return ret; }
            if (retCount < 1) { return -1; }
            string[] paras = strRecv.Split(',');
            if (paras.Length < 1)
            { return -1; }
            val0 = paras[0];
            return 0;
        }
        public int roboGet_int(string args, out int val0 )
        {
            //! init the out
            val0 = 0;

            int ret;
            string strRecv;
            int retCount;
            
            ret = miQuery(args, out strRecv, out retCount);

            if (ret != 0) { return ret; }
            if (retCount < 1) { return -1; }
            string[] paras = strRecv.Split(',');
            if (paras.Length < 1)
            { return -1; }
            val0 = Convert.ToInt32(paras[0]);
            return 0;
        }
        public int roboGet_float(string args, out float val0 )
        {
            //! init the out
            val0 = 0;

            int ret;
            string strRecv;
            int retCount;
            ret = miQuery( args, out strRecv, out retCount);
            if (ret != 0) { return ret; }
            if (retCount < 1) { return -1; }
            string[] paras = strRecv.Split(',');
            if (paras.Length < 1)
            { return -1; }
            val0 = Convert.ToSingle(paras[0]);
            return 0;
        }
        public int roboGet_float_4(string args,
                                   out float val0,
                                   out float val1,
                                   out float val2,
                                   out float val3
                                   )
        {
            //! init the out
            val0 = 0;
            val1 = 0;
            val2 = 0;
            val3 = 0;

            int ret;
            
            string strRecv;
            int retCount;
            ret = miQuery( args, out strRecv, out retCount);
            if (ret != 0) { return ret; }
            if (retCount < 1) { return -1; }
            string[] paras = strRecv.Split(',');
            if (paras.Length < 4)
            { return -1; }
            val0 = Convert.ToSingle(paras[0]);
            val1 = Convert.ToSingle(paras[1]);
            val2 = Convert.ToSingle(paras[2]);
            val3 = Convert.ToSingle(paras[3]);

            return 0;
        }

        public int roboGet_stream(string args,
                                   out byte []ary
                                   )
        {
            ary = null; 

            int ret;

            string strRecv;
            int retCount;
            ret = miQuery( args, out strRecv, out retCount);
            if (ret != 0) { return ret; }
            if (retCount < 2 ) { return -1; }

            if (strRecv[0] != '#')
            { return -1; }

            int headLen;
            headLen = strRecv[1] - '0';
            if (headLen > 0 && headLen <= 9)
            { }
            else
            { return -1; }

            string strPad = strRecv.Substring(2, headLen);

            int padLen = Convert.ToInt32(strPad);
            if (padLen <= 0)
            { return -1; }

            if (strRecv.Length < (2 + headLen + padLen))
            { return -1; }

            ary = new byte[ padLen ];
            for (int i = 0; i < padLen; i++)
            {
                ary[i] = (byte)strRecv[2 + headLen + i];
            }

            return 0;
        }

        public int getIdn( out string val )
        {
            return roboGet_str( "*IDN?", out val );
        }

        public int lrn(string val)
        {
            return roboSet(string.Format("*LRN \"{0}\"", val));
        }

        public int hrst()
        {
            return roboSet( "HRST" );
        }

        public int getVersion( out string val )
        {
            return roboGet_str("VERSION?", out val);
        }

        public int program( int ax, int page, string file )
        {
            return roboSet(string.Format("PROGRAM {0},{1},\"{2}\"", ax, page, file));
        }

        public int call(int ax, int page, int cycle=1, int mode=-1)
        {
            return roboSet(string.Format("CALL {0},{1},{2},{3}", ax, page, cycle, mode));
        }

        public int run(int ax, int page)
        {
            return roboSet(string.Format("RUN {0},{1}", ax, page));
        }

        public int stop(int ax, int page)
        {
            return roboSet(string.Format("STOP {0},{1}", ax, page));
        }

        public int getState(int ax, int page, out int val)
        {
            return roboGet_int(string.Format("STATE? {0},{1}", ax,page), out val);
        }

        public int waitx(int ax, int page, int dst, int tmoms, int tickms = 500)
        {
            int stat;

            while (tmoms > 0)
            {
                Thread.Sleep(tickms);
                tmoms -= tickms;

                if (getState(ax, page, out stat) != 0)
                { return -1;  }

                if (stat == dst)
                { return 0; }
            }

            return -1; 
        }

        public int waitEnd(int ax, int page, int tmoms=20000)
        {
            return waitx(ax, page, mega_robo_calcend, tmoms);
        }

        public int waitIdle(int ax, int page, int tmoms=20000)
        {
            return waitx(ax, page, mega_robo_idle, tmoms);
        }
    }
}
