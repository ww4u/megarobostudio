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

        static void Main(string[] args)
        {
            testIdn();
        }
    }
}
