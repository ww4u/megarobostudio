
using System.Text;

namespace mega_device
{
	public class miDevice
	{
		protected int mVi = 0;
        protected const int _read_cap = 256;
		public miDevice()
		{}

        ~miDevice()
        {
            if (mVi != 0)
            {
                visa32.viClose(mVi);
                mVi = 0;
            }
        }

        public int miOpen( string name, string addr="127.0.0.1", int port = 1234 )
		{
            int viSta;
            int viDef;
            viSta = visa32.viOpenDefaultRM(out viDef);
            if (viSta != visa32.VI_SUCCESS)
            { return -1;  }

            viSta = visa32.viOpen(viDef,
                                   string.Format("tcpip::{0}::{1}::socket", addr, port),
                                    0,
                                    2000,
                                    out mVi
                                );
            if (viSta != visa32.VI_SUCCESS)
            { return -1; }

            //! set name
            viSta = visa32.viPrintf( mVi, "%s\n", name);
            if (viSta != visa32.VI_SUCCESS)
            { return -1; }

            //! set attribute
            viSta = visa32.viSetAttribute(mVi, visa32.VI_ATTR_TERMCHAR, 0x0a);
            viSta = visa32.viSetAttribute(mVi, visa32.VI_ATTR_TERMCHAR_EN, visa32.VI_TRUE );

            return 0; 
        }
		public int miClose()
		{
            if (mVi != 0)
            {
                visa32.viClose(mVi);
                mVi = 0;
            }

            return 0;
        }
		
		protected int _miSend( string send )
		{
            if (mVi == 0)
            { return -1; }

            int viSta;
            viSta = visa32.viPrintf( mVi, "%s\n", send);
            if (viSta != visa32.VI_SUCCESS)
            { return -1; }
            viSta = visa32.viFlush(mVi, visa32.VI_IO_OUT_BUF);
            if (viSta != visa32.VI_SUCCESS)
            { return -1; }

            return 0;
        }

        public int miWaitRead()
        { return 0;  }

        protected int _miRecv(out string recv, out int retCount)
        {
            recv = "";
            retCount = 0;

            if (mVi == 0)
            { return -1; }

            int viSta;
            byte[] buffer = new byte[_read_cap];
            viSta = visa32.viRead(mVi, buffer, buffer.Length, out retCount);
            if (viSta == visa32.VI_SUCCESS
                 || viSta == visa32.VI_SUCCESS_TERM_CHAR)
            {

            }
            else
            { return -1; }

            if (retCount > 0)
            { }
            else
            { return -1; }

            //! trim  the \"
            byte []realBuffer = new byte[ retCount ];
            System.Array.Copy(buffer, realBuffer, retCount);

            recv = System.Text.Encoding.Default.GetString(realBuffer);
            recv = recv.Replace("\"", string.Empty );

            retCount = recv.Length;

            return 0;
        }

        public int miSend(string send)
        {
            int viSta;
            StringBuilder builder = new StringBuilder();
            viSta = visa32.viLock(mVi, visa32.VI_EXCLUSIVE_LOCK, int.MaxValue, string.Empty, builder );
            if (viSta != visa32.VI_SUCCESS)
            { return -1; }

                int ret;
                ret = _miSend(send);

            visa32.viUnlock(mVi);

            return 0;
        }

        public int miRecv(out string recv, out int retCount)
        {
            recv = null;
            retCount = 0;

            int viSta;
            StringBuilder builder = new StringBuilder();
            viSta = visa32.viLock(mVi, visa32.VI_EXCLUSIVE_LOCK, int.MaxValue, string.Empty, builder);
            if (viSta != visa32.VI_SUCCESS)
            { return -1; }

                int ret;
                ret = _miRecv( out recv, out retCount );

            visa32.viUnlock(mVi);

            return 0;
        }

        public int miQuery(string send, out string recv, out int retCount)
        {
            recv = null;
            retCount = 0;

            int viSta;
            StringBuilder builder = new StringBuilder();
            viSta = visa32.viLock(mVi, visa32.VI_EXCLUSIVE_LOCK, int.MaxValue, string.Empty, builder);
            if (viSta != visa32.VI_SUCCESS)
            { return -1; }

                int ret;

                ret = _miSend(send);
                if (ret != 0)
                { return ret; }

                ret = _miRecv(out recv, out retCount);
                if (ret != 0)
                { return ret; }

            visa32.viUnlock(mVi);

            return 0; 
        }
    }
}