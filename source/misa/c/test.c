
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

    int ret;
    char buf[128];
    ret = MRQ_getSensorUartData( vi, "UART1", "S1", buf, 128 );
    printf( "%d\n", ret );

//    viPrintf( vi, "*IDN?\n" );

//    char buf[64];
//    int retCount;
//    miRecv( vi, buf, 64, &retCount );

//    char intf[64];
//    MRQ_getLINK_INTFC( vi, intf );
//    printf( "intf:%s\n", intf );

//    robo_getIdn( vi, intf );
//    printf( "idn:%s\n", intf );

//    MRQ_move( vi, 0,0,1,20,0 );
//    robo_waitIdle( vi, 0,0, 2000 );

//    robo_call( vi, 0,0, 1, -1 );
//    robo_waitIdle( vi, 0,0, 2000 );

//    int switchTime;
//    MRQ_setSENSORUART_SWITCHTIME( vi, "UART1", "S1", 120 );
//    MRQ_getSENSORUART_SWITCHTIME( vi, "UART1", "S1", &switchTime );
//    printf( "sw:%d\n", switchTime );

//    MRQ_getSENSORUART_SWITCHTIME( vi, "UART1", "S2", &switchTime );
//    printf( "sw2:%d\n", switchTime );

    miClose( vi );

    return 0;
}

static int testT4()
{
    ViSession vi = miOpen( "mrx-t4");

    char recvStr[64];

    robo_getIdn( vi, recvStr );
    printf("idn:%s\n", recvStr );

    Sinanju_Center( vi, 0, 0 );
    robo_waitIdle( vi, 0,0, 20000);

    Sinanju_Fold( vi, 0, 0 );
    robo_waitIdle( vi, 0,0, 20000);

    return 0;
}

static int TestMgr()
{
    ViSession vi = miOpen( "localhost");

    char recvStr[512];

    int ret;
    ret = miMgr_getResource( vi, recvStr );
    ret = miMgr_getRobot( vi, recvStr );
    ret = miMgr_getDevice( vi, recvStr );

//    robo_getIdn( vi, recvStr );
//    printf("idn:%s\n", recvStr );

//    Sinanju_Center( vi, 0, 0 );
//    robo_waitIdle( vi, 0,0, 20000);

//    Sinanju_Fold( vi, 0, 0 );
//    robo_waitIdle( vi, 0,0, 20000);

    return 0;
}

int main()
{
//    testPara();

    testDevice();

//    testT4();

//    TestMgr();

    return 0;
}
