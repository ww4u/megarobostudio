#include "tpdownloader.h"

TpDownloader::TpDownloader(QObject *parent) : DeviceDownloader(parent)
{
    m_pMRV = NULL;
    mAx = 0;
}

void TpDownloader::run()
{

    Q_ASSERT( NULL != m_pMRV );
    m_pMRV->acquireDownloader();

    int ret;

    ret = downloadProc();

    m_pMRV->releaseDownloader();
}

void TpDownloader::attachDevice( MegaDevice::deviceMRV *pMrv, int ax )
{
    Q_ASSERT( NULL != pMrv );

    m_pMRV = pMrv;
    mAx = ax;
}

void TpDownloader::append( QList<TpRow*> &rows )
{
    mTpMutex.lock();

    mTpList.append( rows );

    mTpMutex.unlock();
}

int TpDownloader::downloadProc()
{
    Q_ASSERT( NULL != m_pMRV );

    int ret;
    quint32 cacheSize;
    QList<TpRow*> sendCache;

    m_pMRV->tpBeginSend( mAx );

    sysQueue()->postMsg( e_download_started,
                         m_pMRV->name(),
                         tpvRegion( mAx, 0 ) );
    //! invaid t
    TpRow preRow;
    preRow.mT = -1;

    while( mTpList.size() > 0 )
    {
        //! terminate
        if ( isInterruptionRequested() )
        {
            m_pMRV->setPVT_RESET( mAx );
            break;
        }

        //! get buffer
        ret = m_pMRV->getPVT_BUFFERCHECK( mAx, &cacheSize );
        if ( ret != 0 )
        { return ret; }

        //! extract the buffer
        sendCache.clear();
        if ( mTpList.size() > (int)cacheSize )
        {
            mTpMutex.lock();
                sendCache = mTpList.mid( 0, cacheSize );
            mTpMutex.unlock();
        }
        else
        {
            mTpMutex.lock();
                sendCache = mTpList;
            mTpMutex.unlock();
        }

        //! send
        ret = batchDownload( sendCache, preRow );
        if ( ret != 0 )
        { break; }

        //! time tick in net
        sysQueue()->postMsg( e_download_processing,
                              m_pMRV->name(),
                              tpvRegion( mAx, 0 ),
                              sendCache.size(),
                              mTpList.size(),
                              sysTimeStamp()
                              );

        //! delete the send cache
        mTpMutex.lock();
            for ( int i = 0; i < sendCache.size(); i++ )
            { mTpList.removeAt(0); }
        mTpMutex.unlock();

        foreach( TpRow *pRow, sendCache )
        {
            Q_ASSERT( NULL != pRow );
            if ( pRow->gc() )
            { delete pRow; }
        }
    }

    m_pMRV->tpEndSend( mAx, preRow );

    sysQueue()->postMsg( e_download_completed,
                         m_pMRV->name(),
                         tpvRegion( mAx, 0 ) );

    //! request status
    m_pMRV->requestMotionState( mAx, MRV_MOTION_SWITCH_1_MAIN );

    return 0;
}

int TpDownloader::batchDownload( QList<TpRow*> &rows, TpRow &preItem  )
{
    Q_ASSERT( NULL != m_pMRV );

    int ret=0;

    //! the first
    if ( preItem.mT < 0 )
    {}
    else
    { rows.prepend( &preItem ); }

    //! left one
    diffT( rows );

    //! send the items
    for ( int i = 0; i < rows.size() - 1; i++ )
    {
        Q_ASSERT( rows.at(i) != NULL );
        ret = m_pMRV->tpSend( rows.at(i), mAx );
        if ( ret != 0 )
        { return ret; }
    }

    //! the current time
    if( rows.size() > 0 )
    { preItem = *rows[ rows.size() - 1 ]; }

    return ret;
}

int TpDownloader::diffT( QList<TpRow*> &rows )
{
    for ( int i = 0; i < rows.size() - 1; i++ )
    {
        rows[i]->mTL = rows[ i + 1 ]->mT - rows[ i ]->mT;
    }

    return 0;
}
