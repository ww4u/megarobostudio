using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using mega_device;

namespace test
{
    class Program
    {
        static void Main(string[] args)
        {
            MRQ dev = new MRQ();

            dev.miOpen("device1");

            string link;
            dev.getLINK_INTFC(out link);

            UInt32 sTime;
            dev.getSENSORUART_SWITCHTIME("UART1", "S1", out sTime);
            Console.WriteLine(sTime);

            dev.setSENSORUART_SWITCHTIME("UART1", "S1", 130);
            dev.getSENSORUART_SWITCHTIME("UART1", "S1", out sTime);

            Console.WriteLine(sTime);

            dev.getSENSORUART_SWITCHTIME("UART1", "S2", out sTime);
            Console.WriteLine(sTime);

            dev.setSENSORUART_SWITCHTIME("UART1", "S2", 140);
            dev.getSENSORUART_SWITCHTIME("UART1", "S2", out sTime);

            Console.WriteLine(link);

            dev.move(0, 0, 1, -20, 0);
            dev.waitIdle(0, 0, 2000);

            dev.call(0, 0, 1, -1 );
            dev.waitIdle(0, 0, 2000);

            Sinanju sinanju = new Sinanju();
            sinanju.miOpen("MRX-T4");
            string recvStr;
            sinanju.getIdn(out recvStr);
            Console.WriteLine("IDN:{0}", recvStr );

            sinanju.fold();
            sinanju.waitIdle(0, 0, 2000 );
        }
    }
}
