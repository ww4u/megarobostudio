using System;
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

        static void Main(string[] args)
        {
            //testIdn();
            testH2();
        }
    }
}
