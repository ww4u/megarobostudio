#include "receivecache.h"

frameEvent::frameEvent()
{
    mEventId = event_none;
    mbEn = false;
}

void frameEvent::setId( eventId id )
{
    mEventId = id;
}
eventId frameEvent::getId()
{
    return mEventId;
}

void frameEvent::setEnable( bool b )
{ mbEn = b; }
bool frameEvent::getEnable()
{ return mbEn; }

void frameEvent::setMainSubCode( int mainCode,
                                 int subCode )
{
    clear();
    append( (char)(mainCode & 0xff) );
    append( (char)(subCode & 0xff) );
}

//! -- frame data
frameData::frameData()
{}

void frameData::setFrameId( int frameId )
{ mFrameId = frameId; }
int frameData::getFrameId()
{ return mFrameId; }

//! -- frame house
frameHouse::frameHouse()
{
    mSendId = 0;
    mRecvId = 0;
}

void frameHouse::setId( int sendId, int recvId )
{
    mSendId = sendId;
    mRecvId = recvId;
}
void frameHouse::setSendId( int sendId )
{ mSendId = sendId; }
void frameHouse::setRecvId( int recvId )
{ mRecvId = recvId; }
int frameHouse::sendId()
{
    return mSendId;
}
int frameHouse::recvId()
{
    return mRecvId;
}

void frameHouse::lock()
{
    mMutex.lock();
}
void frameHouse::unlock()
{
    mMutex.unlock();
}

void frameHouse::clear()
{
    frameHouse::lock();

    QQueue<frameData>::clear();

    frameHouse::unlock();
}

//! -- frame warehouse
frameWarehouse::frameWarehouse()
{}

frameWarehouse::~frameWarehouse()
{
    delete_all( mHouses );
}

void frameWarehouse::inFrame( frameData & data )
{
    //! find the house
    frameHouse *pHouse = findHouseBySendId( data.getFrameId() );
    if ( NULL == pHouse )
    {
        pHouse = new frameHouse();
        Q_ASSERT( NULL != pHouse );
        pHouse->setSendId( data.getFrameId());

        pHouse->lock();
        pHouse->append( data );
        pHouse->unlock();

        mHouses.append( pHouse );
    }
    else
    {
        pHouse->lock();
        pHouse->append( data );
        pHouse->unlock();
    }

//    logDbg()<<pHouse->sendId()<<data.getFrameId()<<pHouse->size()<<QThread::currentThreadId();
}

frameHouse *frameWarehouse::findHouseByRecvId( int recvId )
{
    foreach( frameHouse *pHouse, mHouses )
    {
        Q_ASSERT( NULL != pHouse );

        if ( pHouse->recvId() == recvId )
        { return pHouse; }
    }

    return NULL;
}
frameHouse *frameWarehouse::findHouseBySendId( int sendId )
{
    foreach( frameHouse *pHouse, mHouses )
    {
        Q_ASSERT( NULL != pHouse );

        if ( pHouse->sendId() == sendId )
        { return pHouse; }
    }

    return NULL;
}

QList< frameHouse *> *frameWarehouse::houses()
{ return &mHouses; }

//! -- receive cache
receiveCache::receiveCache( QObject *parent ) : QThread( parent )
{
    m_pBus = NULL;
}
receiveCache::~receiveCache()
{
    delete_all( mEvents );
}
void receiveCache::attachBus( MegaDevice::IBus *pBus
                               )
{
    Q_ASSERT( NULL != pBus );

    lock();
    m_pBus = pBus;
    unlock();
}
void receiveCache::detachBus()
{
    lock();
    m_pBus = NULL;
    unlock();
}

int receiveCache::setFrameEventEnable( frameEvent &evt, bool b )
{
    frameEvent *pEvt;

    //! find
    pEvt = findFrameEvent( evt );
    if ( NULL != pEvt )
    {
        pEvt->setEnable(b);
        return 0;
    }
    //! add a new one
    else
    {
        pEvt = new frameEvent();
        Q_ASSERT( NULL != pEvt );

        *pEvt = evt;
        pEvt->setEnable( b );
        mEvents.append( pEvt );

        return 0;
    }
}
bool receiveCache::getFrameEventEnable( frameEvent &evt )
{
    frameEvent *pEvt;

    pEvt = findFrameEvent( evt );
    if ( NULL != pEvt )
    { return pEvt->getEnable(); }
    else
    { return false; }
}

frameEvent* receiveCache::findFrameEvent( frameEvent &evt )
{
    return findFrameEvent( evt.getId() );
}

frameEvent* receiveCache::findFrameEvent( eventId id )
{
    foreach( frameEvent *pEvt, mEvents )
    {
        if ( pEvt->getId() == id )
        { return pEvt; }
    }

    return NULL;
}

void receiveCache::append( frameData &ary )
{
    //! signal on some event
    if ( detectEvent( ary ) )
    { logDbg()<<ary.getFrameId()<<ary.size(); }
    else
    {
        lockWarehouse();
        mFrameWarehouse.inFrame( ary );
        unlockWarehouse();
    }
}

//int receiveCache::readAFrame( frameData &ary, int tmous )
//{
//    int ret;

//    //! try to read
//    do
//    {
//        mCacheMutex.lock();
//        if ( mRecvCache.size() > 0 )
//        {
//            ret = 0;
//            ary = mRecvCache.dequeue();
//            mCacheMutex.unlock();

//            return 0;
//        }
//        else
//        { ret = -1; }
//        mCacheMutex.unlock();

//        tmous -= m_pBus->getTick();
//        m_pBus->wait_us( m_pBus->getTick() );

//    }while( tmous>0 );

//    return ret;
//}

int receiveCache::readAFrame( MegaDevice::DeviceId &nodeId,
                              int *pFrameId,
                              byte *pBuf,
                              int *pLen,
                              int tmous )
{
    Q_ASSERT( NULL != pFrameId && NULL != pBuf && NULL != pLen );

    int ret = -1;
    frameHouse *pHouse;
    logDbg()<<tmous<<m_pBus->rdTick();
    do
    {
        do
        {
            //! 1. find house
            //! \note send id is from the device
            pHouse = mFrameWarehouse.findHouseBySendId( nodeId.sendId() );
            if ( NULL == pHouse )
            { break; }

            //! 2. find
            pHouse->lock();
            if ( pHouse->size() > 0 )
            {
                frameData ary;
                ary = pHouse->dequeue();
                pHouse->unlock();

                *pFrameId = ary.getFrameId();
                memcpy( pBuf, ary.data(), ary.length() );

                *pLen = ary.length();
                return 0;
            }
            else
            {
                pHouse->unlock();
            }

        }while(0);

        tmous -= m_pBus->rdTick();
        m_pBus->wait_us( m_pBus->rdTick() );


    }while( tmous > 0 );
logDbg()<<pHouse->size()<<pHouse->sendId()<<nodeId.sendId()<<QThread::currentThreadId();
    return ret;
}

//! read all
int receiveCache::readFrame( MegaDevice::DeviceId &nodeId, frameHouse &frames )
{
    //! no node id
    foreach( frameHouse *pHouse, *mFrameWarehouse.houses() )
    {
        Q_ASSERT( NULL != pHouse );

        frames.append( *pHouse );

        pHouse->clear();
    }

    return frames.size();
}

//! flush some house
void receiveCache::flush( MegaDevice::DeviceId &nodeId )
{
    frameHouse *pHouse = mFrameWarehouse.findHouseBySendId( nodeId.sendId() );
    if ( NULL == pHouse )
    { return; }
logDbg()<<nodeId.sendId()<<nodeId.recvId()<<pHouse->size();
    pHouse->clear();
}

//! clear all house
void receiveCache::clear()
{
    lockWarehouse();

    foreach( frameHouse *pHouse, *mFrameWarehouse.houses() )
    {
        Q_ASSERT( pHouse != NULL );
        pHouse->clear();
    }

//    qDeleteAll( *mFrameWarehouse.houses() );

    unlockWarehouse();
}

//! all houses count
int receiveCache::frameCount()
{
//    return mRecvCache.size();

    int sum = 0;
    foreach( frameHouse *pHouse, *mFrameWarehouse.houses() )
    {
        Q_ASSERT( NULL != pHouse );

        sum += pHouse->size();
        logDbg()<<sum;
    }

    return sum;
}

void receiveCache::lock()
{ mThreadMutex.lock(); }
void receiveCache::unlock()
{ mThreadMutex.unlock(); }

void receiveCache::lockWarehouse()
{ mCacheMutex.lock(); }
void receiveCache::unlockWarehouse()
{ mCacheMutex.unlock(); }

void receiveCache::run()
{
    Q_ASSERT( NULL!=m_pBus );

    int len;
    int ret, frameId;
logDbg();
    Q_FOREVER
    {
        do
        {
            //! have detatched
            if ( m_pBus == NULL )
            {
                QThread::msleep( 10 );
                break;
            }

            lock();
            //! try to read a frame
            ret = m_pBus->doReceive( &frameId, mFrameBuf, &len );
            unlock();

            //! success
            if ( ret == 0 )
            {
                frameData frame;
                frame.append( (const char*)mFrameBuf, len );
                frame.setFrameId( frameId );

                //! interrupt doing
                append( frame );

                logDbg()<<len<<QString::number( frameId, 16 );
            }
            else
            {}

            //! read interval
            m_pBus->wait_us( m_pBus->rdInterval() );

        }while( 0 );
    }
}

bool receiveCache::detectEvent( frameData &ary )
{
    foreach( frameEvent *pEvt, mEvents )
    {
        //! find event
        if ( ary.startsWith( *pEvt) && pEvt->getEnable() )
        {
            emit sig_event( pEvt->getId(), ary );
            logDbg()<<QString::number( ary.getFrameId(), 16)<<ary.toHex(' ');
        }
    }
logDbg()<<mEvents.size();
    return false;
}
