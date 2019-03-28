﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//! namespace
using mega_device;

namespace test
{
    class Program
    {
        static int testMrq( int port = 1234 )
        {
            MRQ mrq;

            mrq = new MRQ();

            int ret = mrq.miOpen("device1", "127.0.0.1", port );
            if (ret != 0)
            {
                Console.WriteLine("fail:{0}", ret);
                return ret;
            }

            string intf;
            if (mrq.getLINK_INTFC(out intf) == 0)
            {
                Console.WriteLine( intf );
            }

            mrq.miClose();

            return 0;
        }

        static int testMrqDownload(int port = 1234)
        {
            MRQ mrq;

            mrq = new MRQ();

            int ret = mrq.miOpen("device1", "127.0.0.1", port);

            //! tp
            float[] dataSet = { 0, 0, 1, 36 };
            mrq.download(0, 0, "tp", dataSet);
            mrq.waitEnd(0, 0);
            mrq.call(0, 0);
            mrq.waitIdle(0, 0);

            //! tpv
            float[] dataSet3 = { 0, 0,0, 1, 36,36 };
            mrq.download(0, 0, "tpv", dataSet3);
            mrq.waitEnd(0, 0);
            mrq.call(0, 0);
            //mrq.waitIdle(0, 0);

            //mrq.call(0, 0);

            mrq.miClose();

            return 0;
        }

        static void testTrig()
        {
            MRQ mrq;

            mrq = new MRQ();

            int ret = mrq.miOpen("device1", "127.0.0.1", 1234);

            string outVal;

            string[] srcs = new string[] { "TRIG1", "TRIG2" };
            string[] onoff = new string[] { "ON", "OFF" };
            string[] levType = new string[] { "RESERVE", "LOW", "RISE", "FALL","HIGH" };                
            for (byte ch = 0; ch < 4; ch++)
            { 
                foreach (string src in srcs)
                {
                    foreach (string cmd in onoff)
                    {
                        mrq.setTRIGGER_LEVELSTATE(ch, src, cmd);
                        mrq.getTRIGGER_LEVELSTATE(ch, src, out outVal);
                        Console.WriteLine("ch:{0} src:{1} raw:{2} return:{3}", ch, src, cmd, outVal);
                    }

                    foreach (string cmd in levType)
                    {
                        mrq.setTRIGGER_LEVELTYPE(ch, src, cmd);
                        mrq.getTRIGGER_LEVELTYPE(ch, src, out outVal);
                        Console.WriteLine("ch:{0} src:{1} raw:{2} return:{3}", ch, src, cmd, outVal);
                    }
                }
            }
        }

        static void testIdn()
        {
            //! 创建
            Sinanju robo;
            robo = new Sinanju();
            //! open
            robo.miOpen("mrx-t4");

            string idn;
            robo.getIdn(out idn);
            Console.WriteLine(idn);

            //! 关闭
            robo.miClose();
        }

        static void testH2()
        {
            H2 robo;
            robo = new H2();

            robo.miOpen("MRX-H2M","192.168.1.159");

            string idn;
            int ret = robo.getIdn(out idn);
            Console.WriteLine(ret);
            Console.WriteLine(idn);

            //robo.move(0, 0, 10, 20, 1);
            //robo.waitIdle(0, 0);

            //robo.preMove(10, 20, 30, 50, 1);
            //robo.waitEnd(0, 0);

            //robo.call(0, 0);
            //robo.waitIdle(0, 0);

            //robo.stepX(10, 1);
            //robo.waitIdle(0, 0);

            //robo.stepY(-50, 1);
            //robo.waitIdle(0, 0);

            robo.to(10, 20, 2);
            robo.waitIdle(0, 0);

            Console.WriteLine( "{0},{1}", robo.pose()[0], robo.pose()[1] );

            robo.miClose();
        }

        static void logDbg(string[] arys)
        {
            if (arys != null)
            {
                foreach (string str in arys)
                { Console.WriteLine(str); }
            }
        }

        static void testDevMgr()
        {
            DevMgr robo;
            robo = new DevMgr();

            robo.miOpen("localhost" );

            logDbg(robo.getDevices());
            logDbg(robo.getRobots());
            logDbg(robo.getResources());

            Console.WriteLine(robo.getOpc());

            robo.find();
            //robo.closeBus();
            //robo.openBus();
            while (robo.getOpc() == 0)
            {
                Console.WriteLine("wait");
            }
            Console.WriteLine(robo.getOpc());

            //robo.stop();
            //robo.reset();
            //robo.terminate();


            robo.miClose();
        }

        static void testMrh_t()
        {
            MRH_T hub;
            hub = new MRH_T();

            hub.miOpen("hub1", "127.0.0.1");

            //string idn;
            //int ret = hub.getIdn(out idn);
            //Console.WriteLine(idn);
            int ret;

            string[] xins = new string[] { "X1", "X2", "X3", "X4" };
            int val;
            foreach (string subStr in xins)
            {
                ret = hub.getXIn(subStr, out val);
                Console.WriteLine( "{0} {1} {2}", subStr, ret, val );
            }

        }

        static void Main(string[] args)
        {
            //testIdn();
            //testH2();

            //testMrqDownload();

            //testTrig();

            testMrh_t();

            //testDevMgr();
            //int failCnt = 0;
            //int ret; 
            //for (int i = 0; i < 10; i++)
            //{
            //    ret = testMrq();
            //    if (ret != 0)
            //    { failCnt++;  }

            //    Console.WriteLine("1234: {0}, fail:{1}", i, failCnt );
            //}

            //for (int i = 0; i < 1; i++)
            //{
            //    ret = testMrq( 1235 );
            //    if (ret != 0)
            //    { failCnt++; }

            //    Console.WriteLine("1235: {0}, fail:{1}", i, failCnt);
            //}
        }
    }
}
