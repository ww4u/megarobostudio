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
            Console.WriteLine( idn );

            //! 关闭
            robo.miClose();
        }

        static void testMrqs()
        {
            MRQ dev1 = new MRQ();
            dev1.miOpen("device1", "127.0.0.1", 1234 );

            string idn;
            if (0 == dev1.getIdn(out idn))
            { Console.WriteLine(idn); }

            MRQ dev2 = new MRQ();
            dev2.miOpen("device1", "127.0.0.1");

            MRQ[] devs = { dev1, dev2 };

            for (int lp = 0; lp < 10; lp++)
            {
                foreach (MRQ mrq in devs)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        mrq.call(i, 1);
                        mrq.waitIdle(i, 1);
                    }
                }

                Console.WriteLine(lp);
            }
        }

        static void Main(string[] args)
        {
            //testIdn();

            testMrqs();
        }
    }
}
