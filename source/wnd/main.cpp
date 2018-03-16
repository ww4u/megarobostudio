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
    QPixmap pixmap(":/res/image/logo/full.png");
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


