#include "tpdownloader.h"

TpDownloader::TpDownloader(QObject *parent) : QThread(parent)
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
    logDbg()<<ret;

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

    while( mTpList.size() > 0 )
    {
        //! terminate
        if ( isInterruptionRequested() )
        { break; }

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
        ret = batchDownload( sendCache );
        if ( ret != 0 )
        { break; }

        //! delete the send cache
        mTpMutex.lock();
            for ( int i = 0; i < sendCache.size(); i++ )
            { mTpList.removeAt(i); }
        mTpMutex.unlock();

        foreach( TpRow *pRow, sendCache )
        {
            Q_ASSERT( NULL != pRow );
            if ( pRow->gc() )
            { delete pRow; }
        }
    }

    m_pMRV->tpEndSend( mAx );

    return 0;
}

int TpDownloader::batchDownload( QList<TpRow*> &rows  )
{
    Q_ASSERT( NULL != m_pMRV );

    int ret=0;
    foreach( TpRow *pRow, rows )
    {
        Q_ASSERT( NULL != pRow );
        ret = m_pMRV->tpSend( pRow, mAx );
    }

    return ret;
}
