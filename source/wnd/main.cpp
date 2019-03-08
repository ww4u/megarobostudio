#include "./ui/mainwindow.h"
#include <QApplication>
#include "./ui/notice.h"
#include "../com/comassist.h"
#include "login.h"

#include "main_help.h"

#include "main_dbg.cpp"



QDataStream &operator<<(QDataStream &out, const VRobot* &myObj)
{
    out.writeRawData( (const char*)myObj, 4 );
    return out;
}
QDataStream &operator>>(QDataStream &in, VRobot* &myObj)
{
    quint32 data;
    in.readRawData( (char*)(&data), 4 );
    myObj = (VRobot*)data;

    return in;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

#ifdef ARCH_32
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
#endif

    //! --meta type
    //! \note register at first
    //! \see TPV_REGEION_TYPE_ID
//    qRegisterMetaType<tpvRegion>();
    qRegisterMetaType<RoboMsg>();

    //! for signal/slot
    qRegisterMetaType<appWidget::servContext>();
    qRegisterMetaType<eventId>();
    qRegisterMetaType<frameData>();
    qRegisterMetaType<RpcRequest>();

    qRegisterMetaType<model_msg>();

    //! vrobot
    qRegisterMetaType<VRobot*>();
    qRegisterMetaTypeStreamOperators<VRobot*>();


    //! --splash
    QPixmap pixmap( QApplication::applicationDirPath() + "/image/full.png" );
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage( a.applicationVersion() );
    a.processEvents();

    //! load pref
    //! translator
    modelSysPref pref;
    {
        pref.load( user_pref_file_name, pref_file_name );

        CommonHelper::selectStyle( pref.mStyleIndex );
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
        pref.save( user_pref_file_name );

    }while( 0 );

    //! login
    do
    {
        LogIn logIn;
        if ( pref.mbAutoLogin )
        {
            break;
        }
        else if ( logIn.exec() != QDialog::Accepted )
        {
            return 0;
        }
        else
        {
            pref.mbAutoLogin = logIn.getAutoLogin();
            pref.mSysMode = (SysMode)logIn.getUserRole();
            pref.save( user_pref_file_name );
        }
    }while(0 );

    QDir::setCurrent( user_pref_path );

    //! dpc set thread
    QThread thread;
    dpcObj dpc;
    dpc.moveToThread( &thread );
    thread.start();

    MainWindow w(&dpc);
    w.show();

    int ret;
    //!login
    //! \note QTimer::singleShot( 0, qApp, quit() )
    //! can not work as the timer post slot is processed in the splash.finish( &w )
    //! process events
    splash.finish(&w);
    ret = a.exec();

    //! kill the service thread
    thread.terminate();
    thread.wait();

    return ret;
}

