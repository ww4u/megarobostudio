
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

#include <QThread>
class myThread : public QThread
{
protected:
    ViSession mVi;

public:
    myThread( ViSession vi, QObject *p=NULL ) : QThread( p )
    {
        mVi = vi;
    }

protected:
    virtual void run()
    {
        char buf[64];
        int retCnt;
        int ret;
        for ( int i = 0; i <  1000; i++ )
        {
            ret = miQuery( mVi, "*idn?\n", 6, buf, 64, &retCnt );
            msleep( 1 );
        }
    }

};

static int testDevice()
{
    ViSession vi = miOpen( "Device1" );
    if ( vi == 0 )
    { return -1; }

    myThread t1( vi );
    myThread t2( vi );

    t1.start();
    t2.start();

    t1.wait();
    t2.wait();

//    int ret;
//    char buf[128];
//    ret = MRQ_getSensorUartData( vi, "UART1", "S1", buf, 128 );
//    printf( "%d\n", ret );

//    miSend( vi, "*IDN?\n", 6 );

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

static int testH2()
{
    ViSession vi = miOpen( "MRX-H2");

    h2_move( vi, 0,0, 0,0,10,100, 10 );

    float pos[2];
    h2_getPose( vi,pos, pos+1 );
    printf("%g,%g\n", pos[0],pos[1]);

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

//    testDevice();

//    testT4();

//    TestMgr();

    testH2();

    return 0;
}
