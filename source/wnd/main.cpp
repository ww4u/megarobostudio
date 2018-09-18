#include "./ui/mainwindow.h"
#include <QApplication>
#include "./ui/notice.h"
#include "../com/comassist.h"
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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //! single exist
    QSharedMemory shareMem("share_megarobo_studio");
    if ( shareMem.create(64) )
    {}
    else
    {
        QMessageBox::information( NULL,
                                  QObject::tr("Info"),
                                  QObject::tr("MEGAROBO Studio is in running!") );
        return 0;
    }

//    a.setApplicationVersion(  );
//    a.setOrganizationName( tr("MEGAROBO") );
//    a.setApplicationName( tr("MegaRobo Studio") );

    //! --meta type
    //! \note register at first
    //! \see TPV_REGEION_TYPE_ID
//    qRegisterMetaType<tpvRegion>();
//    qRegisterMetaType<RoboMsg>();

    logDbg()<<qMetaTypeId<tpvRegion>();
    logDbg()<<qMetaTypeId<RoboMsg>();

    qRegisterMetaType<appWidget::servContext>();
    qRegisterMetaType<eventId>();
    qRegisterMetaType<frameData>();
    qRegisterMetaType<RpcRequest>();

//    qRegisterMetaType<appWidget::servContext>("appWidget::servContext");
//    qRegisterMetaType<eventId>("eventId");
//    qRegisterMetaType<frameData>("frameData");
//    qRegisterMetaType<RoboMsg>("RoboMsg");

    //! --splash
    QPixmap pixmap(":/res/image/logo/full.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage( a.applicationVersion() );
    a.processEvents();
qDebug()<<a.applicationDirPath();
    //! load pref
    //! translator

    modelSysPref pref;
    QTranslator translator;
    {

        pref.load( pref_file_name );
        if ( pref.mLangIndex != 0 )
        {
            QLocale::Language lang;
            QLocale::Country area;
            if ( pref.mLangIndex == 1 )
            {
                lang = QLocale::Chinese;
                area = QLocale::China;
            }
            else if ( pref.mLangIndex == 2 )
            {
                lang = QLocale::Chinese;
                area = QLocale::Taiwan;
            }
            else
            {
                lang = QLocale::AnyLanguage;
                area = QLocale::AnyCountry;
            }

            QLocale local( lang, area );
            local.setDefault( QLocale( QLocale::English) );

            if ( translator.load( local,
                                  QLatin1String("megarobostudio"),
                                  QLatin1String("_"),
                                  a.applicationDirPath() + "/translate"
                                  )
                 && a.installTranslator(&translator) )
            {
            }
            else
            {
                QMessageBox::information( NULL,
                                        QObject::tr("Info"),
                                        QObject::tr("language loss"));
            }
        }
        if ( pref.mStyleIndex != 0 )
        {
            //! style
            CommonHelper::setStyle( a.applicationDirPath() + "/style" + "/mega.qss" );

            QResource::registerResource( a.applicationDirPath() + "/style" + "/res.rcc" );
        }
        else
        {
            //! style
            CommonHelper::setStyle( a.applicationDirPath() + "/style" + "/classic.qss" );

            QResource::registerResource( a.applicationDirPath() + "/style" + "/black.rcc" );
        }
    }

    //! notice
    do
    {
        if ( pref.mbShowNotice )
        {}
        else
        { break; }

        QString fullName = "errant.txt";
        if( comAssist::ammendFileName( fullName ) )
        {}
        else
        { break; }

        Notice noticeDlg;

        if ( noticeDlg.load( fullName ) )
        {}
        else
        { break; }

        noticeDlg.exec();

        pref.mbShowNotice = noticeDlg.nextShowAble();
        pref.save( pref_file_name );

    }while( 0 );

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

