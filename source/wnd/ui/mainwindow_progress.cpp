#include "mainwindow.h"

void MainWindow::slot_download_cancel( const QString &name, int id )
{
    DeviceDownloader::cancelActives();
}

ProgressGroup *MainWindow::progress()
{
    if ( NULL != m_pProgress )
    { }
    else
    {
//        m_pProgress = new ProgressGroup(this);
        m_pProgress = new ProgressGroup();

        connect( m_pProgress,
                 SIGNAL(sigCancel( const QString &, int)),
                 this,
                 SLOT(slot_download_cancel( const QString &, int)) );
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

//        logDbg()<<name<<axes;
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
        progress()->progressShow( name, axes, false );
    }
    else if ( msg.getMsg() == e_download_terminated )
    {
        sysLog( name,
                QString("%1").arg( axes ),
                tr("Terminated") );

        progress()->progressShow( name, axes, false );
    }
    else if ( msg.getMsg() == e_download_canceled )
    {
        sysPrompt( tr("Download canceled") );

        on_actionReset_triggered();
    }
    else
    { return false; }

    return true;
}
