#include "tpvdownloader.h"
#include "../../device/mrq/deviceMRQ.h"

#include "./devicemrq_msg.h"

tpvDownloader::tpvDownloader( QObject *pObj ) : QThread( pObj )
{
    mTryInterval = time_ms( 500 );

    m_pMRQ = NULL;
    mAxesId = 0;
    mPageId = 0;
}

void tpvDownloader::run()
{
    Q_ASSERT( NULL != m_pMRQ );

    m_pMRQ->acquireDownloader();

    downloadProc();

    m_pMRQ->releaseDownloader();
}

void tpvDownloader::downloadProc()
{
    Q_ASSERT( NULL != m_pMRQ );

    sysQueue()->postMsg( e_download_started,
                         m_pMRQ->name(),
                         mAxesId );

    m_pMRQ->lpc( mAxesId )->postMsg( (eRoboMsg)MegaDevice::mrq_msg_program,
                                     mAxesId );

    //! download
    int ret;
    for ( int i = 0; i < 2; i++ )
    {
        ret = transmissionProc();
        if ( ret == 0 )
        { break; }
    }
    if ( ret != 0 )
    {
        m_pMRQ->lpc( mAxesId )->postMsg( (eRoboMsg)MegaDevice::mrq_msg_error,
                                         mAxesId );

        logDbg()<<"*********"<<m_pMRQ->name();
    }

    sysQueue()->postMsg( e_download_completed,
                         m_pMRQ->name(),
                         mAxesId );
}

int tpvDownloader::batchDownload( int batchSize,
                                  int &total,
                                  int &now )
{
    int ret;
    tpvRow *pItem;

    Q_ASSERT( NULL != m_pMRQ );

    while( mTpvs.size() > 0 && batchSize > 0 )
    {
        pItem = mTpvs.head();

        //! download the item
        ret = m_pMRQ->tpvDownload( mAxesId, mPageId, pItem );

        //! gc the item
        if ( pItem->gc() )
        { delete pItem; }

        //! fail
        if ( ret != 0 )
        { return ret; }
        //! success
        else
        {
            mQueueMutex.lock();
            mTpvs.dequeue();
            mQueueMutex.unlock();

            batchSize--;
        }

        //! time tick in net
        sysQueue()->postMsg( e_download_processing,
                          m_pMRQ->name(),
                          mAxesId,
                          now,
                          total
                          );
        //! tune the total
        now++;
        if ( now > total )
        { total += 10; }
    }

    return 0;
}

int tpvDownloader::transmissionProc()
{
    int ret;
    int total, now;

    //! in transmisstion
    ret = m_pMRQ->beginTpvDownload( mAxesId, mPageId );
    if ( ret != 0 )
    { logDbg();return ret; }

    //! acc the progress
    total = mTpvs.size();
    now = 0;
    while( mTpvs.size() > 0 )
    {
        //! check remain
        quint16 batchSize;
        ret = m_pMRQ->getMOTIONPLAN_REMAINPOINT( mAxesId, (MRQ_MOTION_SWITCH_1)mPageId, &batchSize );
        if ( ret != 0 )
        { logDbg(); return ret; }
        else
        {}

        logDbg()<<batchSize;
        if ( batchSize > 0 )
        {
            ret = batchDownload( batchSize, total, now );
            if ( ret != 0 )
            { return ret; }
        }
        //! wait batch size
        else
        {}

        QThread::usleep( mTryInterval );
    }

    ret = m_pMRQ->endTpvDownload( mAxesId, mPageId );
    return ret;
}

void tpvDownloader::attachDevice( MegaDevice::deviceMRQ *pDev,
                   int axesId )
{
    Q_ASSERT( NULL != pDev );

    m_pMRQ = pDev;
    mAxesId = axesId;
}

void tpvDownloader::append( QList<tpvRow*> &rows, int from, int len )
{
    mQueueMutex.lock();

    int iEnd = from + (len < 0 ? rows.length() : len );
    iEnd = qMin( iEnd, rows.length() );
    for ( int i = from; i < iEnd; i++ )
    {
        Q_ASSERT( rows.at(i) != NULL );
        mTpvs.enqueue( rows[i] );
    }

    mQueueMutex.unlock();
}

QString tpvDownloader::name()
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->name();
}
int  tpvDownloader::axes()
{
    return mAxesId;
}
