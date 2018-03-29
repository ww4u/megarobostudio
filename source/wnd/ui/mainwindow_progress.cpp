#include "mainwindow.h"

ProgressGroup *MainWindow::progress()
{
    if ( NULL != m_pProgress )
    { }
    else
    {
        m_pProgress = new ProgressGroup(this);

//        connect( m_pProgress,
//                 SIGNAL(sigCancel( const QString &, int)),
//                 this,
//                 SLOT(slot_download_cancel( const QString &, int)) );
    }

    Q_ASSERT( NULL != m_pProgress );

    return m_pProgress;
}

bool MainWindow::progressProc(  const QString &name,
                    int axes,
                    RoboMsg msg )
{
    if ( msg.getMsg() == e_download_started )
    {
        progress()->progressInfo( name, axes, QString("%1:CH%2 %3").arg(name).arg(axes+1).arg( tr("started") ) );
        progress()->progressProg( name, axes, 0 );
        progress()->progressShow( name, axes, true );

        logDbg()<<name<<axes;
    }
    else if ( msg.getMsg() == e_download_processing )
    {
        int now, total;

        now = msg.at(2).toInt();
        total = msg.at(3).toInt();

//        logDbg()<<name<<axes;

        progress()->progressInfo( name, axes, QString("%1:CH%2 %3").arg(name).arg(axes+1).arg( tr("downloading") ) );
        progress()->progressRange( name, axes, 0, total );
        progress()->progressProg( name, axes, now );
    }
    else if ( msg.getMsg() == e_download_completed )
    {
//        logDbg()<<name<<axes;

        progress()->progressShow( name, axes, false );
    }
    else if ( msg.getMsg() == e_download_terminated )
    {
//        sysLog( msg.at(0).toString(),
//                QString("%1").arg( msg.at(1).toInt()),
//                tr("Terminated") );

//        ui->btnDown->setEnabled( true );

//        logDbg()<<name<<axes;

        progress()->progressShow( name, axes, false );
    }
    else
    { return false; }

    return true;
}
