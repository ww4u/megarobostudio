#include "tpvdownloader.h"
#include "../../device/mrq/deviceMRQ.h"

#include "./devicemrq_msg.h"

tpvDownloader::tpvDownloader( QObject *pObj ) : QThread( pObj )
{
    mTryInterval = time_ms( 500 );

    m_pMRQ = NULL;
}
tpvDownloader::~tpvDownloader()
{}
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
                         mRegion );

    m_pMRQ->lpc( mRegion.axes() )->postMsg( (eRoboMsg)MegaDevice::mrq_msg_program,
                                            mRegion );

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
        m_pMRQ->lpc( mRegion.axes() )->postMsg( (eRoboMsg)MegaDevice::mrq_msg_error,
                                         mRegion );

        logDbg()<<"*********"<<m_pMRQ->name();
    }

    sysQueue()->postMsg( e_download_completed,
                         m_pMRQ->name(),
                         mRegion );
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
        ret = m_pMRQ->tpvDownload( mRegion, pItem );

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
                          mRegion,
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
    ret = m_pMRQ->beginTpvDownload( mRegion );
    if ( ret != 0 )
    { logDbg();return ret; }

    //! acc the progress
    total = mTpvs.size();
    now = 0;
    while( mTpvs.size() > 0 )
    {
        //! check remain
        quint16 batchSize;
        ret = m_pMRQ->getMOTIONPLAN_REMAINPOINT( mRegion.axes(),
                                                 (MRQ_MOTION_SWITCH_1)mRegion.page(),
                                                 &batchSize );
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

    ret = m_pMRQ->endTpvDownload( mRegion );
    return ret;
}

void tpvDownloader::attachDevice( MegaDevice::deviceMRQ *pDev,
                   const tpvRegion &region )
{
    Q_ASSERT( NULL != pDev );

    m_pMRQ = pDev;
    mRegion = region;
}

void tpvDownloader::setRegion( const tpvRegion &region )
{ mRegion = region; }

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
{ return mRegion.axes(); }
