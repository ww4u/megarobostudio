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
            int ret;
            MRQ dev = new MRQ();

            dev.miOpen("device1");

            byte[] ary;
            dev.getSensorUartData("UART1", "s1", out ary);
            if (ary == null)
            { }
            else
            { }

            string link;
            dev.getLINK_INTFC(out link);
            Console.WriteLine(link);

            //UInt32 sTime;
            //dev.getSENSORUART_SWITCHTIME("UART1", "S1", out sTime);
            //Console.WriteLine(sTime);

            //dev.setSENSORUART_SWITCHTIME("UART1", "S1", 130);
            //dev.getSENSORUART_SWITCHTIME("UART1", "S1", out sTime);

            //Console.WriteLine(sTime);

            //dev.getSENSORUART_SWITCHTIME("UART1", "S2", out sTime);
            //Console.WriteLine(sTime);

            //dev.setSENSORUART_SWITCHTIME("UART1", "S2", 140);
            //dev.getSENSORUART_SWITCHTIME("UART1", "S2", out sTime);

            //dev.move(0, 0, 1, 20, 0);
            //ret = dev.waitIdle(0, 0, 6000);
            //Console.WriteLine(ret);

            //dev.call(0, 0, 1, -1);
            //dev.waitIdle(0, 0, 2000);

            //Sinanju sinanju = new Sinanju();
            //sinanju.miOpen("MRX-T4");
            //string recvStr;
            //sinanju.getIdn(out recvStr);
            //Console.WriteLine("IDN:{0}", recvStr);

            //sinanju.fold();
            //sinanju.waitIdle(0, 0, 6000);

            //Motors mot;
            //mot = new Motors();
            //mot.miOpen("mrx-motors");
            //mot.call( 0, 0 );
            //mot.waitIdle(0, 0);

            //mot.call(0, 1);
            //mot.waitIdle(0, 1);
        }
    }
}
