
#include "../../robot/robotfact.h"
#include "../../robot/megatron/megatron.h"

#include "../../instServer/instserver.h"
#include "../../com/robomsg.h"

int _main( int argc, char *argv[] )
{
    QApplication a(argc, argv);

    RoboMsg msg;
    for ( int i = 0; i < 10; i++ )
    {
        msg.clear();
//        msg.setMsg( (eRoboMsg)i );
        msg.setMsg( e_download_processing );

        msg.append( QVariant(i) );

//        RoboNet::postMsg( msg );
    }

//    RoboNet::process( );

//    MegaDevice::instServer server;

//    server.start();

//    MegaDevice::deviceMRQ dev;

//    dev.open();

////    dev.write( "LINK:INTFC NONE\r\n", strlen("LINK:INTFC NONE\r\n") );
//    dev.write( "TEST:ADD 1,2\r\n");

//    qDebug()<<dev.size();

//    char buf[16];
//    qDebug()<<dev.read( buf, dev.size() );

//    dev.close();

//    robotMegatron robot;

//    logDbg()<<robot.mId;
//    logDbg()<<robot.mName;

//    QLabel label;

//    label.setPixmap( QPixmap::fromImage( robot.getImage() ) );
//    QImage image = robot.getImage();

//    logDbg()<<image.width()<<image.height();

//    label.show();

    a.exec();

    return 0;
}

//class A
//{
//public:
//    int val;
//    virtual int value()=0;
////    { return 0; }
//    A() : val(0)
//    {
//        logDbg()<<val<<value();
//    }
//};

//class B : public A
//{
//public:
//    B()
//    {
////        val = 1;
//    }
//    virtual int value()
//    { return 1; }
//};


//#include "../ui/progressgroup.h"
int __main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    B b;

    QMap< tpvRegion, int > maps;
    tpvRegion region;
    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 10; j++ )
        {
            region.setRegion( i,j );
//            Q_ASSERT( !maps.contains( tpvRegion(i,j) ) );
            maps.insert( region, i*10+j );

            Q_ASSERT ( maps[ region] == (i * 10 + j) );
            Q_ASSERT( maps.contains( region ) );
        }
    }

    foreach (int value, maps )
    {
        logDbg()<<value;
    }

    foreach( tpvRegion region, maps.keys() )
    { logDbg()<<region.axes()<<region.page(); }

    foreach( tpvRegion region, maps.keys() )
    { logDbg()<<region.axes()<<region.page(); }



    for ( int i = 3; i >=0; i-- )
    {
        for ( int j = 9; j >=0; j-- )
        {
            logDbg()<<i<<j;
            region.setRegion( i,j );
            Q_ASSERT( maps.contains( region ));
//            Q_ASSERT( tpvRegion(i,j) == tpvRegion(i,j) );
        }
    }

//    for ( int i = 0; i < 4; i++ )
//    {
//        for ( int j = 0; j < 10; j++ )
//        {
////            maps.insert( tpvRegion(i,j), 0 );
//            logDbg()<<i<<j;
//            Q_ASSERT( maps.contains(tpvRegion(i,j)));
//            logDbg()<<maps.value( tpvRegion(i,j) );
////            logDbg()<<maps[ tpvRegion(i,j) ];
////            Q_ASSERT( maps.find( tpvRegion(i,j) ) != maps.end() );
//        }
//    }

//    ProgressGroup gp;

//    gp.progressInfo( 0, "hello" );
//    gp.progressInfo( 1, "hell2" );

//    gp.show();

    int id = qRegisterMetaType<tpvRegion>();

    logDbg()<<id;

//    tpvRegion reg(1);

//    QVariant var = QVariant::fromValue( reg );

////    logDbg()<<var.type()<<var.typeName();
//    logDbg()<<var.type();
//    logDbg()<<var.typeName();

//    tpvRegion reg2 = var.value<tpvRegion>();
//    logDbg()<<reg2.mAx<<reg2.mPage;

//    a.exec();

    return 0;
}
