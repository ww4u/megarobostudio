#include "tpvdownloader.h"
#include "../../device/mrq/deviceMRQ.h"

#include "./devicemrq_msg.h"

int tpvDownloader::_downloaderInterval = 0;

void tpvDownloader::setInterval( int interval )
{
    _downloaderInterval = interval;
}
int tpvDownloader::interval()
{
    return _downloaderInterval;
}

tpvDownloader::tpvDownloader( QObject *pObj ) : DeviceDownloader( pObj )
{
    mTryInterval = time_s( 1 );

    m_pMRQ = NULL;
}
tpvDownloader::~tpvDownloader()
{}
void tpvDownloader::run()
{
    Q_ASSERT( NULL != m_pMRQ );

    //sysLog( QString::number(mRegion.mAx), QString::number(mRegion.mPage), "down start" );
    m_pMRQ->acquireDownloader();

    int ret = downloadProc();

    m_pMRQ->releaseDownloader();

    if ( ret == ERR_INTERRUPT_REQUESTED
         && tpvDownloader::_activeLoaders.size() == 0 )
    {
        sysQueue()->postMsg( e_download_canceled,
                             m_pMRQ->name(),
                             mRegion );
    }

    //sysLog( QString::number(mRegion.mAx), QString::number(mRegion.mPage), "down end" );
}

int tpvDownloader::downloadProc()
{
    Q_ASSERT( NULL != m_pMRQ );

    sysQueue()->postMsg( e_download_started,
                         m_pMRQ->name(),
                         mRegion );

    m_pMRQ->lpc( mRegion.axes() )->postMsg( (eRoboMsg)MegaDevice::mrq_msg_program,
                                            mRegion );

    //! download
    int ret;
    QQueue< tpvRow *> transferTpvs;
    //! \todo try times by out setting
    for ( int i = 0; i < 2; i++ )
    {
        //! copy send buf
        mQueueMutex.lock();
        transferTpvs = mTpvs;
        mQueueMutex.unlock();

        ret = transmissionProc( transferTpvs );
        if ( ret == 0 )
        { break; }
        else if ( ret == ERR_INTERRUPT_REQUESTED )
        { break; }
        else
        { sysError( QObject::tr("download fail"), QString::number(__LINE__) ); }
    }

    //! gc
    mQueueMutex.lock();
    for ( int i = 0; i < mTpvs.size(); i++ )
    {
        Q_ASSERT( NULL != mTpvs.at(i) );
        if ( mTpvs.at(i)->gc() )
        { delete mTpvs.at(i); }
    }
    mTpvs.clear();
    mQueueMutex.unlock();

    if ( ret != 0 )
    {
        m_pMRQ->lpc( mRegion.axes() )->postMsg( (eRoboMsg)MegaDevice::mrq_msg_error,
                                                 mRegion );

        sysError( QObject::tr("download fail"), QString::number(__LINE__) );
    }

    if ( ret == ERR_INTERRUPT_REQUESTED )
    {
        sysQueue()->postMsg( e_download_terminated,
                             m_pMRQ->name(),
                             mRegion );

    }

    sysQueue()->postMsg( e_download_completed,
                         m_pMRQ->name(),
                         mRegion );

    return ret;
}

int tpvDownloader::batchDownload( QQueue< tpvRow *> &transQueue,
                                  int batchSize,
                                  int &total,
                                  int &now )
{
    int ret;
    tpvRow *pItem;

    Q_ASSERT( NULL != m_pMRQ );

    while( transQueue.size() > 0 && batchSize > 0 )
    {
        pItem = transQueue.head();

        //! check interrupt
        if ( isInterruptionRequested() )
        { return ERR_INTERRUPT_REQUESTED; }

        //! download the item
        ret = m_pMRQ->tpvDownload( mRegion, pItem );

        //! the download speed
        if ( tpvDownloader::_downloaderInterval > 0 )
        { QThread::usleep( tpvDownloader::_downloaderInterval ); }

        //! fail
        if ( ret != 0 )
        { return ret; }
        //! success
        else
        {
            transQueue.dequeue();

            batchSize--;
        }

        //! time tick in net
        sysQueue()->postMsg( e_download_processing,
                              m_pMRQ->name(),
                              mRegion,
                              now,
                              total,
                              sysTimeStamp()
                              );
        //! tune the total
        now++;
        if ( now > total )
        { total += 10; }
    }

    return 0;
}

int tpvDownloader::transmissionProc( QQueue< tpvRow *> &transQueue )
{
    int ret;
    int total, now;

    //! in transmisstion
    ret = m_pMRQ->beginTpvDownload( mRegion );
    if ( ret != 0 )
    { sysError( __FUNCTION__, QString::number(__LINE__) ); /*Q_ASSERT(false);*/ return ret; }

    //! acc the progress
    total = transQueue.size();
    now = 0;
    while( transQueue.size() > 0 )
    {
        //! check interrupt
        if ( isInterruptionRequested() )
        { return ERR_INTERRUPT_REQUESTED; }

        //! check remain data
        quint16 batchSize;
        ret = m_pMRQ->getMOTIONPLAN_REMAINPOINT(  mRegion.axes(),
                                                  (MRQ_MOTION_SWITCH_1)mRegion.page(),
                                                  &batchSize );
        if ( ret != 0 )
        { sysError( __FUNCTION__, QString::number(__LINE__) ); /*Q_ASSERT(false);*/ return ret; }
        else
        {}

        //! \errant keep one empty at least
        if ( batchSize > 1 )
        {
            ret = batchDownload( transQueue,
                                 batchSize - 1, total, now );

            if ( ret != 0 )
            { sysError( __FUNCTION__, QString::number(__LINE__) ); return ret; }
        }
        //! wait batch size
        else
        {}

        QThread::usleep( mTryInterval );
    }

    ret = m_pMRQ->endTpvDownload( mRegion );
    if ( ret != 0 )
    { sysError( __FUNCTION__, QString::number(__LINE__) ); /*Q_ASSERT(false);*/ return ret; }

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
    tpvRow *pSendRow;
    mQueueMutex.lock();

    int iEnd = from + (len < 0 ? rows.length() : len );
    iEnd = qMin( iEnd, rows.length() );
    for ( int i = from; i < iEnd; i++ )
    {
        Q_ASSERT( rows.at(i) != NULL );

        pSendRow = new tpvRow();
        Q_ASSERT( NULL != pSendRow );

        *pSendRow = *rows.at(i);
        pSendRow->setGc( true );

        mTpvs.enqueue( pSendRow );
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
