#include "sampleproxy.h"

sampleTimer::sampleTimer( int interval )
{
    mPeriod = interval;
    mNow = interval;
    mbRun = false;
}

sampleTimer &sampleTimer::operator=( const sampleTimer &samp )
{
    this->mbRun = samp.mbRun;
    this->mNow = samp.mNow;
    this->mPeriod = samp.mPeriod;

    return *this;
}

void sampleTimer::start()
{
    mbRun = true;
    mNow = mPeriod;
}
void sampleTimer::stop()
{
    mbRun = false;
    mNow = mPeriod;
}

void sampleTimer::setInterval( int interval )
{
    mPeriod = interval;
    mNow = interval;
}
int sampleTimer::getInterval()
{ return mPeriod; }

bool sampleTimer::tick( int time )
{
    if ( !mbRun )
    { return false; }

    if ( mNow < time )
    {
        mNow = mPeriod;
        return true;
    }
    else
    {
        mNow -= time;
        return false;
    }
}

sampleProxy::sampleProxy()
{
    mItem = MRQ_REPORT_STATE_TORQUE;
    mCacheSize = 1024;

    mSignature = 0;
    mAx = 0;
}

sampleProxy::sampleProxy( const sampleProxy &proxy )
{
    *this = proxy;
}
sampleProxy::sampleProxy( sampleProxy *proxy )
{
    Q_ASSERT( NULL != proxy );
    *this = *proxy;
}

sampleProxy::~sampleProxy()
{
    delete_all( mSpyCache );
}

sampleProxy &sampleProxy::operator=( const sampleProxy &proxy )
{
    mChName = proxy.mChName;
    mItem = proxy.mItem;
    mTimer = proxy.mTimer;

    mCacheSize = proxy.mCacheSize;

    return *this;
}

//! only timer not same
bool sampleProxy::isLike( const sampleProxy &proxy )
{
    do
    {
        if ( mChName != proxy.mChName )
        { break; }

        if ( mItem != proxy.mItem )
        { break; }

        return true;

    }while(0);

    return false;
}

void sampleProxy::attach( MRQ_REPORT_STATE stat,
                        const QString &chName,
                        bool bRun,
                        int cacheSize,
                        int interval )
{
    mItem = stat;
    mChName = chName;
    mCacheSize = cacheSize;

    if ( bRun )
    { mTimer.start(); }
    else
    { mTimer.stop(); }
    mTimer.setInterval( interval );
}

void sampleProxy::start()
{ mTimer.start(); }
void sampleProxy::stop()
{ mTimer.stop(); }

void sampleProxy::clear()
{
    delete_all( mSpyCache );
}

void sampleProxy::setChName( const QString &chName )
{ mChName = chName; }
QString sampleProxy::chName()
{ return mChName; }

#define assign_value( v )   pVal = (char*)&v; size = sizeof(v);
bool sampleProxy::sample( int tickus,
                          spyItem *pItem,
                          MegaDevice::deviceMRQ *pMrq,
                          int ax
                           )
{
    //! tick success
    if ( mTimer.tick(tickus) )
    {}
    else
    { //logDbg();
        return false; }

    Q_ASSERT( NULL != pItem );
    Q_ASSERT( NULL != pMrq );

    quint32 val32, val16, val8;
    char *pVal;
    int size;
    int ret;

    QMetaType::Type metaType = pMrq->getREPORT_TYPE( mItem );
    if ( metaType == QMetaType::UChar )
    {
        ret = pMrq->getREPORT_DATA( ax, mItem, &val8 );
        assign_value( val8 );
    }
    else if ( metaType == QMetaType::UShort )
    {
        ret = pMrq->getREPORT_DATA( ax, mItem, &val16 );
        assign_value( val16 );
    }
    else //if ( metaType == QMetaType::UInt )
    {
        ret = pMrq->getREPORT_DATA( ax, mItem, &val32 );
        assign_value( val32 );
    }

    if ( ret == 0 )
    {}
    else
    { return false; }

    //! signature
    mSignature = pMrq->getModel()->getSignature();
    mAx = ax;

    //! attach data
    pItem->mMs = QDateTime::currentMSecsSinceEpoch();
    pItem->mVal.clear();

    //! debug rand data
//    val = qrand();
//    static int _rand_id = 0;
//    val = (_rand_id++ & 0xff);

//    val = qrand()*10.0/RAND_MAX;

    //! \note to big endian
    for ( int i = 0; i < size; i++ )
    {
        pItem->mVal.prepend( (pVal) + i, 1 );
    }

    return true;
}

int sampleProxy::logInCache( spyItem *pSpyItem )
{
    Q_ASSERT( NULL != pSpyItem );

    //! use the first
    if ( mSpyCache.size() >= mCacheSize )
    {
        spyItem *pOut;
        pOut = mSpyCache.dequeue();
        *pOut = *pSpyItem;
        mSpyCache.append( pOut );
    }
    //! not full
    else
    {
        spyItem *pNewOne = new spyItem();
        if ( NULL != pNewOne )
        {
            *pNewOne = *pSpyItem;
            mSpyCache.append( pNewOne );
        }
    }

    return mSpyCache.size();
}

int sampleProxy::cacheLen()
{ return mSpyCache.size(); }
