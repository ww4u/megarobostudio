#include "samplethread.h"

//! QThread::IdlePriority
sampleThread::sampleThread( QObject *pObj ) : QThread( pObj )
{
    mSampleInterval = time_s(5);
}

sampleThread::~sampleThread()
{}

void sampleThread::setSampleInterval( int interval )
{
    mSampleInterval = interval;
}

void sampleThread::attachDbMeta( dbMeta *pMeta )
{
    Q_ASSERT( NULL != pMeta );
    mSampleMgr.attachDbMeta( pMeta );
}

void sampleThread::attachInstMgr( MegaDevice::InstMgr *pInstMgr )
{
    Q_ASSERT( NULL != pInstMgr );
    mSampleMgr.attachInstMgr( pInstMgr );
}

void sampleThread::attachSampleProxy( MRQ_REPORT_STATE stat,
                        const QString &chName,
                        bool bRun,
                        int cacheSize,
                        int intervalms
                        )
{
    sampleProxy proxy;

    proxy.attach( stat, chName, bRun, cacheSize, intervalms );

    attachSampleProxy( proxy );
}

void sampleThread::attachSampleProxy( sampleProxy & proxy )
{
    mThreadMutex.lock();

    mSampleMgr.attachProxy( proxy );

    mThreadMutex.unlock();
}

sampleProxy *sampleThread::getSampleProxy( int stat,
                             const QString &chName )
{
    foreach( sampleProxy *pProxy, mSampleMgr.mSamples )
    {
        Q_ASSERT( NULL != pProxy );

        if ( pProxy->chName() == chName && pProxy->mItem == stat )
        { return pProxy; }
    }

    return NULL;
}

void sampleThread::clear()
{
    mThreadMutex.lock();

    mSampleMgr.clear( this );

    mThreadMutex.unlock();
}

void sampleThread::stop()
{
    mThreadMutex.lock();

    mSampleMgr.stop();

    mThreadMutex.unlock();
}

void sampleThread::run()
{
    Q_FOREVER
    {
        if ( isInterruptionRequested() )
        { break; }

        mThreadMutex.lock();

        mSampleMgr.sampleProc( mSampleInterval, this );

        mThreadMutex.unlock();

        QThread::usleep( mSampleInterval );

    }
}
