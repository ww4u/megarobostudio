#include "receivecache.h"
#include "../../source/sys/sysapi.h"

#include "../../device/vrobot.h"
#include "../../inst/instmgr.h"

frameEvent::frameEvent()
{
    mEventId = event_none;
    mbEn = false;
    mRepeatAble = true;
    mAckAble = false;

    mPatternLen = 0;
    for ( int i = 0; i < sizeof_array( mPatterns); i++ )
    {
        mPatterns[i] = -1;
    }
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

void frameEvent::setRepeatAble( bool b )
{ mRepeatAble = b; }
bool frameEvent::repeatAble()
{ return mRepeatAble; }

void frameEvent::setAckAble( bool b )
{ mAckAble = b; }
bool frameEvent::ackAble()
{ return mAckAble; }

void frameEvent::setMainSubCode( int mainCode,
                                 int subCode )
{
    clear();
    append( (char)(mainCode & 0xff) );
    append( (char)(subCode & 0xff) );
}

void frameEvent::setMainSubCode( int mainCode,
                     int subCode,
                     int patterns[],
                     int pattLen )
{
    setMainSubCode( mainCode, subCode );

    Q_ASSERT( pattLen <= sizeof_array(mPatterns) );
    for ( int i = 0; i < pattLen; i++ )
    {
        mPatterns[i] = patterns[i];
    }
    mPatternLen = pattLen;
}

bool frameEvent::match( frameData &ary )
{
    if ( ary.length() < 2 + mPatternLen )
    { return false; }

    do
    {
        //! check pattern
        if ( ary.at(0) != at(0) )
        { break; }

        if ( ary.at(1) != at(1) )
        { break; }


        for ( int i = 0; i < mPatternLen; i++ )
        {
            if ( mPatterns[i] != -1 )
            {
                 if ( (byte)mPatterns[i] != ary.at(i+2) )
                 { return false; }
            }
            else
            {}
        }

//        //! check repeat
//        if ( receiveCache::queueInEvent( mRepeatAble, ary ) )
//        {}
//        else
//        { return false; }

        return true;

    }while( 0 );

    return false;
}

int frameEvent::ackEvent( MegaDevice::IBus *pBus,
                          MegaDevice::DeviceId &did,
                          frameData &data )
{
    if ( NULL == pBus )
    { return -1; }

    //! write back frame data
    return pBus->write( did, (byte*)data.data(), data.size() );
//    logDbg()<<size();
//    return 0;
}

//! -- frame data
frameData::frameData()
{
    mFrameId = 0;
    mTimeStamp = 0;

    mDevId = 0;
}

void frameData::setFrameId( int frameId )
{ mFrameId = frameId; }
int frameData::frameId()
{ return mFrameId; }

void frameData::setDevId( int devId )
{ mDevId = devId; }
int frameData::devId()
{ return mDevId; }

void frameData::setTimeStamp( quint64 t )
{ mTimeStamp = t; }
quint64 frameData::timeStamp()
{ return mTimeStamp; }

bool frameData::isLike( frameData &data )
{
    if ( mFrameId != data.frameId() )
    { return false; }

    if ( mDevId != data.devId() )
    { return false; }

    if ( (QByteArray)(*this) != (QByteArray)data )
    { return false; }

    return true;
}

//! -- frame house
frameHouse::frameHouse()
{
    mSendId = 0;
    mRecvId = 0;

    mBytes = 0;
}

void frameHouse::append(const frameData &t)
{
    QQueue< frameData >::append( t );
    mBytes += t.length();
}

void frameHouse::append(const frameHouse &t)
{
    QQueue< frameData >::append( t );
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
    mBytes = 0;

    frameHouse::unlock();
}

int frameHouse::bytes()
{
    return mBytes;
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
    frameHouse *pHouse = findHouseBySendId( data.frameId() );
    if ( NULL == pHouse )
    {
        pHouse = new frameHouse();
        Q_ASSERT( NULL != pHouse );
        pHouse->setSendId( data.frameId());

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
QMutex receiveCache::_threadMutex;
quint64 receiveCache::_timeStamp = 0;

QList< frameEvent* > receiveCache::_frameEvents;
QMutex receiveCache::_frameEventsMutex;

QSemaphore receiveCache::_eventSema;

bool receiveCache::_eventEn = true;

QMap<int, bool> receiveCache::_mapBypass;

QQueue< frameData > receiveCache::_cacheEvents;
QMutex receiveCache::_cacheEventsMutex;

void receiveCache::lock()
{ _threadMutex.lock(); }
void receiveCache::unlock()
{ _threadMutex.unlock(); }

void receiveCache::lockFrameEvents()
{ receiveCache::_frameEventsMutex.lock(); }
void receiveCache::unlockFrameEvents()
{ receiveCache::_frameEventsMutex.unlock(); }

void receiveCache::accEvent()
{ receiveCache::_eventSema.release(); }
void receiveCache::decEvent()
{ receiveCache::_eventSema.acquire(); }

int receiveCache::availableEvent()
{ return receiveCache::_eventSema.available(); }

quint64 receiveCache::timeStamp()
{ return receiveCache::_timeStamp; }

void receiveCache::cli()
{
    receiveCache::_eventEn = false;
}
void receiveCache::sti()
{
    receiveCache::_eventEn = true;
}

void receiveCache::setByPass( int devId, bool b )
{
    if ( _mapBypass.contains(devId) )
    {
        receiveCache::_mapBypass[ devId ] = b;
    }
    else
    {
        receiveCache::_mapBypass.insert( devId, b );
    }
}
bool receiveCache::getByPass( int devId )
{
    if ( receiveCache::_mapBypass.contains(devId) )
    { return receiveCache::_mapBypass[devId]; }
    else
    { return false; }
}

bool receiveCache::queueInEvent( bool bRepeatable, frameData &frame )
{
    //! check repeatable
    _cacheEventsMutex.lock();

    if ( bRepeatable )
    {
    }
    else
    {
        //! iter
        foreach( frameData data, receiveCache::_cacheEvents )
        {
            if ( data.isLike( frame ) )
            {
                sysLog( QObject::tr("repeat detect") );
                _cacheEventsMutex.unlock();
                return false;
            }
        }
    }

    receiveCache::_cacheEvents.enqueue( frame );

    _cacheEventsMutex.unlock();

    return true;
}
bool receiveCache::dequeueEvent( )
{
    //! dequeue out
    _cacheEventsMutex.lock();

    if ( receiveCache::_cacheEvents.size() > 0 )
    { receiveCache::_cacheEvents.dequeue(); }

    _cacheEventsMutex.unlock();

    return true;
}

receiveCache::receiveCache( QObject *parent ) : QThread( parent )
{
    m_pBus = NULL;
}
receiveCache::~receiveCache()
{
    receiveCache::lockFrameEvents();
//    delete_all( receiveCache::_frameEvents );
    receiveCache::unlockFrameEvents();
}
void receiveCache::attachBus( MegaDevice::IBus *pBus
                               )
{
    Q_ASSERT( NULL != pBus );

    receiveCache::lock();
    m_pBus = pBus;
    receiveCache::unlock();
}
void receiveCache::detachBus()
{
    receiveCache::lock();
    m_pBus = NULL;
    receiveCache::unlock();
}

int receiveCache::setFrameEventEnable( frameEvent &evt, bool b )
{
    frameEvent *pEvt;

    //! find
    receiveCache::lockFrameEvents();
    pEvt = findFrameEvent( evt );
    if ( NULL != pEvt )
    {
        pEvt->setEnable(b);
        receiveCache::unlockFrameEvents();
        return 0;
    }
    //! add a new one
    else
    {
        receiveCache::unlockFrameEvents();

        pEvt = new frameEvent();
        Q_ASSERT( NULL != pEvt );

        *pEvt = evt;
        pEvt->setEnable( b );
        receiveCache::lockFrameEvents();
        receiveCache::_frameEvents.append( pEvt );
        receiveCache::unlockFrameEvents();

        return 0;
    }
}
bool receiveCache::getFrameEventEnable( frameEvent &evt )
{
    frameEvent *pEvt;

    receiveCache::lockFrameEvents();
    pEvt = findFrameEvent( evt );
    if ( NULL != pEvt )
    {
        bool bEn = pEvt->getEnable();
        receiveCache::unlockFrameEvents();
        return bEn;
    }
    else
    {
        receiveCache::unlockFrameEvents();
        return false;
    }
}

frameEvent* receiveCache::findFrameEvent( frameEvent &evt )
{
    return findFrameEvent( evt.getId() );
}

frameEvent* receiveCache::findFrameEvent( eventId id )
{
    foreach( frameEvent *pEvt, receiveCache::_frameEvents )
    {
        if ( pEvt->getId() == id )
        { return pEvt; }
    }

    return NULL;
}

void receiveCache::clearFrameEvent()
{
    receiveCache::lockFrameEvents();
    delete_all( receiveCache::_frameEvents );
    receiveCache::unlockFrameEvents();
}

void receiveCache::append( frameData &ary )
{
    //! signal on some event
    if ( receiveCache::_eventEn && detectEvent( ary ) )
    {
//        logDbg()<<ary.frameId()<<ary.size();
    }
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
    Q_ASSERT( NULL != m_pBus );
//    logDbg()<<tmous<<m_pBus->rdTick()<<nodeId.sendId()<<tmous;

//    QThread::usleep( 100 );     //! wait a little
    int loopCount = 0;

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
//logDbg()<<loopCount;
                *pFrameId = ary.frameId();
                memcpy( pBuf, ary.data(), ary.length() );

                *pLen = ary.length();
                return 0;
            }
            else
            {
                pHouse->unlock();
            }

        }while(0);

        Q_ASSERT( NULL != m_pBus );
        tmous -= m_pBus->rdTick();
        m_pBus->wait_us( m_pBus->rdTick() );

        loopCount++;
    }while( tmous > 0 );
logDbg()<<pHouse->size()<<pHouse->sendId()<<nodeId.sendId()<<QThread::currentThreadId()<<loopCount;
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
//logDbg()<<nodeId.sendId()<<nodeId.recvId()<<pHouse->size();
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
int receiveCache::frameCount( int id )
{
    int sum = 0;
    foreach( frameHouse *pHouse, *mFrameWarehouse.houses() )
    {
        Q_ASSERT( NULL != pHouse );

        if ( id == -1 || pHouse->mSendId == id )
        { sum += pHouse->size(); }
    }

    return sum;
}

//! data bytes
int receiveCache::frameBytes( int id )
{
    int sum = 0;
    foreach( frameHouse *pHouse, *mFrameWarehouse.houses() )
    {
        Q_ASSERT( NULL != pHouse );

        if ( id == -1 || pHouse->mSendId == id )
        { sum += pHouse->bytes(); }
    }

    return sum;
}


void receiveCache::lockWarehouse()
{ mCacheMutex.lock(); }
void receiveCache::unlockWarehouse()
{ mCacheMutex.unlock(); }

void receiveCache::run()
{
    Q_ASSERT( NULL!=m_pBus );

    //! highest
//    QThread::setPriority( QThread::TimeCriticalPriority );

    int ret;
    QList< frameData > frameCache;
    Q_FOREVER
    {
        if ( isInterruptionRequested() )
        { break; }

        //! bypass
        //! \todo busid
        if ( receiveCache::getByPass( 0 ) )
        {
            QThread::msleep( 500 );
            continue;
        }

        do
        {
            frameCache.clear();

            //! have detatched
            if ( m_pBus == NULL )
            {
                QThread::msleep( 1000 );
                break;
            }

            receiveCache::lock();

            //! try to read a frame
            Q_ASSERT( NULL != m_pBus );
            ret = m_pBus->doReceive( frameCache );

            receiveCache::unlock();
//sysLog( QString::number(ret), QString::number(__LINE__) );
            //! success
            if ( ret == 0 )
            {
                //! acc the stamp
                receiveCache::_timeStamp++;

                for ( int i = 0; i < frameCache.size(); i++ )
                {
//                    append( frameCache.at(i) );
                    frameCache[i].setTimeStamp( receiveCache::_timeStamp );
//                    logDbg()<<frameCache[i].timeStamp();
                    append( frameCache[i] );
//                    logDbg()<< frameCache.at(i).length() << QString::number( frameCache.at(i).getFrameId(), 16 );
                }
            }
//            else
//            {
//                //! read interval
//                //! the shortest reply time: 160us
//                m_pBus->wait_us( m_pBus->rdInterval() );
//            }

        }while( 0 );
    }
}

bool receiveCache::detectEvent( frameData &ary )
{
    bool bEvent = false;

    receiveCache::lockFrameEvents();

    foreach( frameEvent *pEvt, receiveCache::_frameEvents )
    {
        Q_ASSERT( NULL != pEvt );
        //! find event
        //! \todo check repeat events
        if ( pEvt->getEnable() && pEvt->match(ary) )
        {
            do
            {
                if ( pEvt->ackAble() )
                {}
                else
                { break; }

                //! inst
                MegaDevice::InstMgr *pMgr = MegaDevice::InstMgr::proxy();
                if ( NULL == pMgr )
                { break; }

                VRobot * pRobo = pMgr->findRobotBySendId( ary.frameId(), m_pBus->devId(), 0 );
                if ( NULL == pRobo )
                { break; }

                MegaDevice::DeviceId deviceId = pRobo->getDeviceId();
                pEvt->ackEvent( m_pBus, deviceId, ary );

            }while( 0 );

            emit sig_event( pEvt->getId(), ary );

            receiveCache::accEvent();

//            logDbg()<<QString::number( ary.frameId(), 16)<<ary.toHex(' ')<<ary.timeStamp();
            bEvent = true;
        }
    }

    receiveCache::unlockFrameEvents();

    return bEvent;
}
