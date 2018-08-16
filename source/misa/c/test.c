
#include "stdio.h"


#include "misa.h"


static int testPara()
{
    struPara para;

    parasInit( &para, "123,342");
    printf("%d\n", parasCount( &para ) );
    printf( paraAt( &para, 0) );
    printf( paraAt( &para, 1) );
    parasDeInit( &para );

    parasInit( &para, "123");
    printf("%d\n", parasCount( &para ) );
    printf( paraAt( &para, 0) );
    parasDeInit( &para );

    return 0;
}

static int testDevice()
{
    ViSession vi = miOpen( "Device1" );
    if ( vi == 0 )
    { return -1; }

    viPrintf( vi, "*IDN?\n" );

    char buf[64];
    int retCount;
    miRecv( vi, buf, 64, &retCount );

    char intf[64];
    MRQ_getLINK_INTFC( vi, intf );
    printf( "intf:%s\n", intf );

    int switchTime;
    MRQ_setSENSORUART_SWITCHTIME( vi, "UART1", "S1", 120 );
    MRQ_getSENSORUART_SWITCHTIME( vi, "UART1", "S1", &switchTime );
    printf( "sw:%d\n", switchTime );

    MRQ_getSENSORUART_SWITCHTIME( vi, "UART1", "S2", &switchTime );
    printf( "sw2:%d\n", switchTime );

    miClose( vi );

    return 0;
}

int main()
{
//    testPara();

    testDevice();

    return 0;
}
