#include "./ui/mainwindow.h"
#include <QApplication>

class CommonHelper
{
public:
    static void setStyle(const QString &style) {
        QFile qss(style);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
};

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


#include "../../arith/pathplan/pathplan.h"
#include "../../arith/kinematic/kinematic.h"
int __main(int argc, char *argv[] )
{
    QApplication app( argc, argv );

    //! test plan
    endPoint points[]=
    {
        { 250,0,502.75,0,0,1,},
        { 250,0,462.25,0,1,1,},
    };

    int resLen;
    int ret = ns_pathplan::GetPvtLen( (double*)points, sizeof_array(points), 5, 0, &resLen );

    logDbg()<<resLen<<ret;

    double *pData = new double[ resLen ];
    Q_ASSERT( NULL != pData );
    ns_pathplan::GetPvtInfo( pData, resLen );

    tracePoint *pTrace = (tracePoint*)pData;
    int traceCount = resLen * sizeof(double)/ sizeof(tracePoint);
    for( int i = 0; i < traceCount; i++ )
    {
        logDbg()<<pTrace[i].t<<pTrace[i].x<<pTrace[i].y<<pTrace[i].z<<pTrace[i].vx<<pTrace[i].vy<<pTrace[i].vz;
    }

    //! arm
    jointsAngle angle={ 0, MATH_PI / 2, -MATH_PI / 2, -MATH_PI / 2 };
    jointsTrace *pJointsTrace = new jointsTrace[ traceCount ];
    Q_ASSERT( NULL != pJointsTrace );

//    logDbg()<<offsetof(tracePoint,y);
    logDbg()<<sizeof(tracePoint)/sizeof(double)<<traceCount<<sizeof(jointsTrace);

    ret = ns_kinematic::GetArmPosition( (double*)(&angle),

                    &pTrace->datas + offsetof( tracePoint, x )/sizeof(double), sizeof(tracePoint)/sizeof(double),
                    &pTrace->datas + offsetof( tracePoint, vx )/sizeof(double),sizeof(tracePoint)/sizeof(double),
                    &pTrace->datas + offsetof( tracePoint, t )/sizeof(double),sizeof(tracePoint)/sizeof(double),

                    traceCount,
                    (double*)pJointsTrace

                    );
    logDbg()<<ret;
    for ( int i = 0; i < ret; i++ )
    {
        logDbg()<<i<<pJointsTrace[i].t
                <<pJointsTrace[i].p[0]
                <<pJointsTrace[i].p[1]
                <<pJointsTrace[i].p[2]
                <<pJointsTrace[i].p[3]
                <<pJointsTrace[i].v[0]
                <<pJointsTrace[i].v[1]
                <<pJointsTrace[i].v[2]
                <<pJointsTrace[i].v[3];
    }

    delete []pJointsTrace;
    delete []pData;

    return 0;
}

//#include "../ui/progressgroup.h"
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

//    ProgressGroup gp;

//    gp.progressInfo( 0, "hello" );
//    gp.progressInfo( 1, "hell2" );

//    gp.show();

//    a.exec();

//    return 0;
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //! --meta type
    qRegisterMetaType<appWidget::servContext>("appWidget::servContext");
    qRegisterMetaType<eventId>("eventId");
    qRegisterMetaType<frameData>("frameData");
    qRegisterMetaType<RoboMsg>("RoboMsg");

    //! --splash
    QPixmap pixmap(":/res/image/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();

    //! translator
    QTranslator translator;

    qDebug()<<QDir::currentPath();
    translator.load( "megaroboide_chn", "../../translate" );
    a.installTranslator( &translator );

    //! style
//    CommonHelper::setStyle(":/res/qss/white.qss");

    //! dpc set thread
    QThread thread;
    dpcObj dpc;
    dpc.moveToThread( &thread );
    thread.start();

    MainWindow w(&dpc);
    w.show();

    splash.finish(&w);

    int ret = a.exec();

    //! kill the service thread
    thread.terminate();
    thread.wait();

    return ret;
}


