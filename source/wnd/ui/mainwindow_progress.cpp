#include "mainwindow.h"

void MainWindow::slot_download_cancel( const QString &name, int id )
{
    //! \note cancel all
    DeviceDownloader::cancelActives();
}

ProgressGroup *MainWindow::progress()
{
    if ( NULL != m_pProgress )
    { }
    else
    {
        m_pProgress = new ProgressGroup();
        Q_ASSERT( NULL != m_pProgress );

        connect( m_pProgress,
                 SIGNAL(sigCancel( const QString &, int)),
                 this,
                 SLOT(slot_download_cancel( const QString &, int)) );
    }

    Q_ASSERT( NULL != m_pProgress );

    return m_pProgress;
}

