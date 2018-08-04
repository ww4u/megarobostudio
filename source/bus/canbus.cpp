#include "canbus.h"

//! use mrq
#include "../../device/board/MRQ.h"
#include "./receivecache.h"

namespace MegaDevice {

#define send_id( id )   ( ((id)<<12) + 0xfff  )
#define recv_id( id )   ( ((id)<<4) + 0xf )
#define max_nodes       4096    //! 12bit + 12bit = 24bit

#define FRAME_LEN   8

#define can_device_desc mDevType, mDevId, mCanId

#define check_handle()      if ( mHandle <= 0 )\
                            { return -1; }

CANBus::CANBus()
{
    mHandle = 0;
    mDevType = 6;
    mDevId = 0;
    mCanId = 0;

    mName = "CAN-BUS";
    mBusType = e_bus_can;

    mFrames = 0;
}

CANBus::~CANBus()
{
    close();

    //! clear the device id
    delete_all( mEnumDevices );

    delete_all( mDevices );
}

int CANBus::open( int devType,
                  int devId, int canId,
                  const QString &desc )
{
    mDevType = devType;
    mDevId = devId;
    mCanId = canId;

    //! full dll path
    QString dllName;
    int busType;
    if ( mPId == 1 )            //! usb can ii
    {
        dllName = "usbcanii.dll";
        busType = VCI_MR_USBCAN;
    }
    else if ( mPId == 3 )       //! mini usb can
    {
        dllName = "miniusbcan.dll";
        busType = VCI_MR_USBCAN;
    }
    else if ( mPId == 0 )       //! mrh-e
    {
        dllName = "MegaCanDevice.dll";
        busType = VCI_MR_USBCAN;
    }
    else if ( mPId == 4 )       //! mcp
    {
        dllName = "";
        busType = VCI_MCP_CAN;
    }
    else    //!  ( mPId == 2 )
    {
        dllName = "MegaCanDevice.dll";
        busType = VCI_MR_LANCAN;
    }

    //! load dll
    QString fullPath ;
    fullPath = QCoreApplication::applicationDirPath() + QDir::separator() + dllName;
    fullPath = QDir::toNativeSeparators( fullPath );
//    Q_ASSERT( mApi.mDll.isLoaded() == false );
logDbg();
    if ( !mApi.load( fullPath, mPId ) )
    {
        sysLog( fullPath );
        sysError( dllName, QObject::tr("load fail") );
        sysError( mApi.mDll.errorString() );
        return -1;
    }

    //! find
    if ( busType == VCI_MR_LANCAN )
    {
        char strs[ 1024 ] = { 0 };                          //! for the resources
        if ( 0 == mApi.find( VCI_MR_LANCAN, strs ) )
        {
            sysError( QObject::tr("device find fail") );
            close();
            return -1;
        }

        //! match the device id
        QByteArray rawData;
        rawData.setRawData( strs, qstrlen( strs ) );

        //! split
        QString rawStr = QString( rawData );
        QStringList rsrcList = rawStr.split( ';', QString::SkipEmptyParts );

        int id = rsrcList.indexOf( desc );
        if ( id == -1 )
        {
            sysError( QObject::tr("no device: ") + desc );
            close();
        }
        else
        {
            mDevId = id;
            sysLog( desc, QString::number(mDevId) );
        }
    }
    else if ( busType == VCI_MCP_CAN )
    {

    }
    else if ( busType == VCI_MR_USBCAN )
    {

    }
    else
    { return -1; }
logDbg();
    //! open
    mHandle = mApi.open( mDevType, mDevId, mCanId );
logDbg()<<mDevType<<mDevId<<mCanId<<mHandle;
    if ( mHandle != 1 )
    {
        sysError( QObject::tr("CAN open fail") );
        close();
        return -2;
    }

    //! init
    if ( busType == VCI_MR_LANCAN )
    {}

    else if ( busType == VCI_MR_USBCAN
              || busType == VCI_MCP_CAN )
    {
        if ( 0 != initBus() )
        {
            close();
            return -3;
        }
    }
    else
    {
        return -1;
    }
logDbg();
    //! open cache
    Q_ASSERT( NULL == m_pRecvCache );
    m_pRecvCache = new receiveCache();
    if ( NULL == m_pRecvCache )
    {logDbg();
        close();
        return -3;
    }logDbg();
    m_pRecvCache->attachBus( this );
    m_pRecvCache->start();
//    m_pRecvCache->start( QThread::TimeCriticalPriority );
logDbg();
    return 0;
}
void CANBus::close()
{
    if ( mHandle != 0 )
    {
        //! clean cache
        if ( NULL != m_pRecvCache )
        {
            m_pRecvCache->requestInterruption();
            m_pRecvCache->wait();
            delete m_pRecvCache;
            m_pRecvCache = NULL;

            //! bypass off
            receiveCache::setByPass( mDevId, 0 );
        }

        int ret;
        ret = mApi.close( mDevType, mDevId );
        logDbg()<<ret<<mDevType<<mDevId;
        mHandle = 0;

        mApi.unload();
    }
}

struct canSpeedTimer
{
    int speed;
    int timer0, timer1;
};

#define _K_( k )    (k)*1000
#define _M_( m )    (m)*_K_(1000)
static canSpeedTimer _bps_timer_matrix[]
{
    { _M_(1),   0X00, 0x14, },
    { _K_(800), 0X00, 0x16, },
    { _K_(666), 0X80, 0xB6, },
    { _K_(500), 0X00, 0x1C, },

    { _K_(400), 0X80, 0xFA, },
    { _K_(250), 0X01, 0x1C, },
    { _K_(200), 0X81, 0xFA, },
    { _K_(125), 0X03, 0x1C, },

    { _K_(100), 0X04, 0x1C, },
    { _K_(80),  0X83, 0xFF, },
    { _K_(50),  0X09, 0x1C, },
    { _K_(40),  0X87, 0xFF, },

    { _K_(20),  0X18, 0x1C, },
    { _K_(10),  0X31, 0x1C, },
    { _K_(5),   0XBF, 0xFF, },
};

int CANBus::initBus()
{
    mApi.clear( mDevType, mDevId, mCanId );

    INIT_CONFIG canConfig;

    //! config
//    canConfig.AccCode = 0xffffffff;
    canConfig.AccCode = 0x00000000;
    canConfig.AccMask = 0xffffffff;
    canConfig.Filter = 0;
    canConfig.Mode = 0;

    //! speed
    int speedId = matchSpeed( mSpeed );
    if ( speedId < 0 )
    {
        sysError( QObject::tr("Invalid speed"), QString::number(mSpeed) );
        return -1;
    }

    canConfig.baud = mSpeed;
//    canConfig.baud = 0;
    canConfig.Timing0 = _bps_timer_matrix[speedId].timer0;
    canConfig.Timing1 = _bps_timer_matrix[speedId].timer1;
//    canConfig.Timing0 = 0;
//    canConfig.Timing1 = 0;
logDbg()<<canConfig.Timing0<<canConfig.Timing1<<mSpeed<<mDevType<<mDevId<<mCanId;
    int ret = mApi.init( mDevType, mDevId, mCanId, &canConfig );
    if ( ret != 1 )
    {
        sysError( QObject::tr("Can init fail") );
        return -1;
    }

    ret = mApi.start( mDevType, mDevId, mCanId );
    if ( ret != 1 )
    {
        sysError( QObject::tr("Can start fail") );
        return -1;
    }

    return 0;
}

int CANBus::matchSpeed( int speed )
{
    //! find the timer
    for ( int i = 0; i < sizeof_array(_bps_timer_matrix); i++ )
    {
        //! config the timer
        if ( _bps_timer_matrix[i].speed == mSpeed )
        {
            return i;
        }
    }

    return -1;
}

//! total size
int CANBus::size()
{
    if ( mHandle == 0 )
    { return 0; }

    int len;

    Q_ASSERT( m_pRecvCache != NULL );
    len = m_pRecvCache->frameCount();

    return len;
}

int CANBus::flush( DeviceId &id )
{
    check_handle();

    IBus::lock();
    mApi.clear( mDevType, mDevId, mCanId );
    IBus::unlock();

    if ( m_pRecvCache != NULL )
    { m_pRecvCache->flush( id ); }

    return 0;
}

int CANBus::clear()
{
    if ( m_pRecvCache != NULL )
    { m_pRecvCache->clear(); }

    return 0;
}

int CANBus::doSend( const QString &buf )
{
    Q_ASSERT( mPId == 0 );

    check_handle();

    IBus::lock();
    int ret = mApi.write( mDevType, mDevId, buf.toLatin1().data(), buf.size() );
    IBus::unlock();

    logDbg()<<ret<<buf;
    if ( ret != 1 )
    { return -1; }
    else
    { return 0; }
}

int CANBus::doWrite( byte *pBuf, int len )
{
    if ( mApi.write == NULL )
    { return -1; }

    check_handle();

    IBus::lock();
    int ret = mApi.write( mDevType, mDevId, (char*)pBuf, len );
    IBus::unlock();
    if ( ret != 1 )
    { return -2; }

    return 0;
}
int CANBus::doRead( byte *pOutBuf, int len, int tmo, int &retLen )
{
    retLen = 0;

    if ( mApi.read == NULL )
    { return -1; }

    check_handle();

    IBus::lock();
    int ret = mApi.read( mDevType, mDevId, (char*)pOutBuf, len, tmo );
    IBus::unlock();
    retLen = ret;

    return 0;
}

int CANBus::doWrite(DeviceId &nodeId, byte *pBuf, int len)
{
    Q_ASSERT( NULL != pBuf );
    Q_ASSERT( len > 0 && len < 9 );

    check_handle();

    CAN_OBJ canObj;
    memset( &canObj, 0, sizeof(CAN_OBJ));

    canObj.ID = nodeId.recvId();                //! \note receive id
    canObj.TimeStamp = 0;
    canObj.TimeFlag = 0;

    canObj.SendType = (mPId == 1 ? 1 : 0);      //! \note USB CAN II be 1, do not know why

    canObj.RemoteFlag = 0;
    canObj.ExternFlag = 1;
    canObj.DataLen = len;

    for ( int i = 0; i < len; i++)
    {
        canObj.Data[i] = pBuf[i];
    }
//logDbg()<<canObj.ID<<canObj.Data[0]<<canObj.Data[1]<<canObj.Data[2]<<canObj.Data[3]<<canObj.Data[4];
    int ret;//logDbg();
    IBus::lock();//logDbg();
//    for( int kk = 0; kk < 50; kk++ )
    {
        ret = mApi.transmit( can_device_desc, &canObj, 1);//logDbg();
//        ret = 0;
    }

    mFrames += 1;
    if ( mWtInterval > 0 )
    { IBus::wait_us( mWtInterval ); }

    IBus::unlock();
    if ( 1 !=  ret )
    {
        sysError( __FUNCTION__, QString::number(__LINE__) );
        return -1;
    }
    else
    { return 0; }
}

int CANBus::doWrite( QList<frameData> &canFrames )
{
    if ( canFrames.size() > 0 )
    {}
    else
    { return -1; }

    check_handle();

    CAN_OBJ objs[ canFrames.size() ];

    //! copy the data
    for ( int i = 0; i < canFrames.size(); i++ )
    {
        memset( &objs[i], 0, sizeof(CAN_OBJ));

        objs[i].ID = canFrames[i].frameId();

        objs[i].SendType = (mPId == 1 ? 1 : 0 );
        objs[i].RemoteFlag = 0;
        objs[i].ExternFlag = 1;
        objs[i].DataLen = canFrames.at(i).size();

        //! copy data
        Q_ASSERT( objs[i].DataLen > 0 && objs[i].DataLen <= 8 );
        memcpy( objs[i].Data, canFrames.at(i).constData(), objs[i].DataLen );
    }
IBus::lock();
    int ret =  mApi.transmit( can_device_desc, objs, canFrames.size() );
    mFrames += canFrames.size();
IBus::unlock();
    if ( ret != canFrames.size() )
    {
        sysError( __FUNCTION__, QString::number(__LINE__) );
        return -1;
    }
    else
    { return 0; }
}


int CANBus::doReceive( QList<frameData> &canFrames )
{
    check_handle();

    CAN_OBJ canObj[128];
//    CAN_OBJ canObj[1];
    int ret;
IBus::lock();
//    //! check data
//    if ( mApi.getSize( can_device_desc ) > 0 )
//    {}
//    else
//    { IBus::unlock(); return -1; }

    //! no need to lock as the receive is from the buffer

    ret = mApi.receive( can_device_desc,
                        &canObj[0],
                        sizeof_array(canObj),
                        mRecvTmo );
    IBus::unlock();

#ifdef ARCH_32

#else
    QThread::msleep( mRecvTmo );
#endif

    //! read fail
    if ( ret > sizeof_array( canObj ) || ret < 1 )
    { return -1; }

    //! for each frame
    frameData tFrame;
    for ( int i = 0; i < ret; i++ )
    {
        //! check data len
        if ( canObj[i].DataLen > 0 && canObj[i].DataLen <= 8 )
        {}
        else
        { return -1; }

        Q_ASSERT( canObj[i].DataLen > 0 && canObj[i].DataLen <= 8 );

        tFrame.clear();
        tFrame.setFrameId( canObj[i].ID );
        tFrame.setDevId( mDevId );
        tFrame.append( (const char*)canObj[i].Data, canObj[i].DataLen );
        canFrames.append( tFrame );
    }
    if ( ret > 1 )
    {/*sysLog( QString::number(ret), QString::number(__LINE__));*/
//        for ( int i = 0; i < ret; i++  )
//        { logDbg()<<canFrames.at(i)<<ret<<canFrames.at(i).size(); }
    }

    return 0;
}

//! read from cache
int CANBus::doRead( DeviceId &nodeId, byte *pBuf, int cap, int *pLen )
{
    Q_ASSERT( pBuf != NULL );
    Q_ASSERT( pLen != NULL );

    int ret, retLen, frameId;
    byte frameBuf[ FRAME_LEN ];
    ret = doFrameRead( nodeId, &frameId, frameBuf, &retLen );
    if ( ret != 0 )
    {
        *pLen = 0;logDbg();
        return ret;
    }

    if ( cap != retLen )
    {
        for( int i = 0; i < retLen; i++ )
        { logDbg()<<QString::number( frameBuf[i], 16 ); }
        *pLen = retLen;logDbg()<<retLen<<cap<<frameId<<nodeId.sendId()<<nodeId.recvId();
        return -2;
    }

    *pLen = cap;
    memcpy( pBuf, frameBuf, *pLen );

    return 0;
}

//! read from cache
//! nodeId -- device recv id
int CANBus::doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int *pLen )
{
    //! read from cache by timeout
    Q_ASSERT( NULL != m_pRecvCache );

    return m_pRecvCache->readAFrame( nodeId, pFrameId, pBuf, pLen, mRdTmo );
}

//! read a few frame
int CANBus::doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int eachFrameSize, int n )
{
    frameHouse frames;

    //! read frames
    m_pRecvCache->readFrame( nodeId, frames );

    int i = 0;
    for ( i = 0; i < frames.size() && i < n ; i++ )
    {
        pFrameId[i] = frames[i].frameId();
        memcpy( pBuf + i * eachFrameSize,
                frames[i].data(),
                qMin( frames[i].size(), eachFrameSize )
                );
    }

    return i;
}

int CANBus::doSplitRead( DeviceId &nodeId, int packOffset, byte *pBuf, int cap, int *pLen )
{
    int ret, retLen;
    byte frameBuf[ FRAME_LEN ];

    int slice, total;

    int outLen = 0;
    int frameId;
    do
    {
        ret = doFrameRead( nodeId, &frameId, frameBuf, &retLen );
        if ( ret != 0 )
        { return ret; }

        if ( retLen <= packOffset )
        { return -1; }

        total = frameBuf[ packOffset ] & 0x0f;
        slice = ( frameBuf[ packOffset ] >> 4 ) & 0x0f;

        //! cat the data
        for ( int i = packOffset + 1; i < retLen && outLen < cap; i++, outLen++ )
        {
            pBuf[ outLen ] = frameBuf[ i ];
        }
    }while( slice < total );

    *pLen = outLen;

    return 0;
}

int CANBus::enumerate( const modelSysPref &pref )
{
    int ret;

    receiveCache::cli();

    beginEnumerate();
logDbg();
    if ( pref.mbAutoAssignId )
    { ret = autoEnumerate( pref ); }
    else
    { ret = rawEnumerate( pref ); }
logDbg();
    endEnumerate();

    receiveCache::sti();

    return ret;
}

int CANBus::getDevType()
{ return mDevType; }
int CANBus::getCanId()
{ return mCanId; }

qint64 CANBus::frames()
{ return mFrames; }

void CANBus::beginEnumerate()
{logDbg()<<mEnumDevices.size();
    //! destroy the enumerate
    delete_all( mEnumDevices );

    mFrames = 0;
}

void CANBus::endEnumerate()
{logDbg()<<mDevices.size()<<mEnumDevices.size();
    //! clear the current
    delete_all( mDevices );

    //! copy the enums
    mDevices = mEnumDevices;

    //! \note keep the deviceIds
    mEnumDevices.clear();
}

int CANBus::autoEnumerate( const modelSysPref &pref )
{
    int ret;

    ret = collectHash();
    if ( ret != 0 ) return ret;

    ret = assignIds( pref );
    if ( ret != 0 ) return ret;

    return ret;
}
int CANBus::rawEnumerate( const modelSysPref &pref )
{
    int ret;
logDbg();
    //! send-hash
    QMap< int, quint32 > sendHashMap;
    ret = collectHash( sendHashMap );
    if ( ret != 0 )
    { return ret; }
logDbg();
    //! send-recv
    QMap< int, quint32 > sendRecvMap;
    ret = collectRecvId( sendRecvMap );
    if ( ret != 0 )
    { return ret; }
logDbg();
    //! check the size
    if ( sendRecvMap.size() != sendHashMap.size() )
    {
        sysError( QObject::tr("ID match fail") );
        return -1;
    }

    //! check the ids
    QList<int> sendHashKeys, sendRecvKeys;
    sendHashKeys = sendHashMap.keys();
    sendRecvKeys = sendRecvMap.keys();

    qSort( sendHashKeys );
    qSort( sendRecvKeys );

    if ( sendHashKeys != sendRecvKeys )
    {
        sysError( QObject::tr("ID match fail") );
        return -1;
    }
logDbg();
    //! build
    buildDeviceIds( sendHashMap, sendRecvMap );
logDbg();
    return 0;
}

int CANBus::collectHash( )
{
    byte buf[] = { MRQ_mc_CAN, MRQ_sc_CAN_NETMANAGEHASH_Q };
    int ret;

    Q_ASSERT( m_pRecvCache != NULL );
    m_pRecvCache->clear();
logDbg();

    //! 0. can intf
    DeviceId broadId( CAN_BROAD_ID );
    byte buf0[] = { MRQ_mc_LINK, MRQ_sc_LINK_INTFC, MRQ_LINK_INTFC_CAN };
    ret = doWrite( broadId, buf0, sizeof(buf0) );logDbg();
    if ( ret != 0 )
    { return ret; }

    //! 1. broadcast
    ret = doWrite( broadId, buf, sizeof(buf) );
    if ( ret != 0 )
    { return ret; }

    wait_us( mEnumerateTmo );
logDbg();
    //! 2. frame count
    int frame = size();logDbg()<<frame;
    if ( frame < 1 )
    { /*sysLog( QString::number(frame), QString::number(__LINE__) );*/ logDbg(); return -1; }

    //! 3. read all frame
    int collectFrameSize = 6;
    byte readBuf[ collectFrameSize * frame ];
    int frameIds[ frame ];
//sysLog( QString::number(frame), QString::number(__LINE__) );
    ret = doFrameRead( broadId, frameIds, readBuf, collectFrameSize, frame );
//sysLog( QString::number(ret), QString::number(__LINE__) );
    if ( ret != frame )
    { return -1; }
logDbg();
    //! 4. all frames
    DeviceId *pDeviceId;
    uint32 hashId;
    QList<quint32> hashIdList;
    for ( int i = 0; i < frame; i++ )
    {
        pDeviceId = new DeviceId();
        if ( NULL == pDeviceId )
        { return -1; }

        //! get hash id
        memcpy( &hashId, (readBuf + i * collectFrameSize) + 2, 4 );

        pDeviceId->setSendId( frameIds[i] );logDbg()<<frameIds[i];
        pDeviceId->setSignature( hashId );logDbg()<<QString::number( hashId, 16 );

        if ( hashIdList.contains(hashId) )
        {
            delete pDeviceId;
        }
        else
        {
            mEnumDevices.append( pDeviceId );
            hashIdList.append( hashId );
        }
        logDbg()<<i;
    }
logDbg();
    //! 5. sort the device by id
    qSort( mEnumDevices.begin(),
           mEnumDevices.end(),
           deviceIdLessThan );

    return 0;
}

//! send: hash
int CANBus::collectHash( QMap< int, quint32 > &sendHashMap )
{
    int ret;

    Q_ASSERT( m_pRecvCache != NULL );
    m_pRecvCache->clear();
logDbg();
    //! 0. can intf
    DeviceId broadId( CAN_BROAD_ID );
    byte buf0[] = { MRQ_mc_LINK, MRQ_sc_LINK_INTFC, MRQ_LINK_INTFC_CAN };
    ret = doWrite( broadId, buf0, sizeof(buf0) );logDbg();
    if ( ret != 0 )
    { return ret; }
logDbg();
    //! 1. broadcast
    byte buf[] = { MRQ_mc_CAN, MRQ_sc_CAN_NETMANAGEHASH_Q };
    ret = doWrite( broadId, buf, sizeof(buf) );
    if ( ret != 0 )
    { return ret; }
logDbg();
    wait_us( mEnumerateTmo );
logDbg();
    //! 2. frame count
    int frame = size();logDbg()<<frame;
    if ( frame < 1 )
    { logDbg(); return -1; }

    //! 3. read all frame
    int collectFrameSize = 6;
    byte readBuf[ collectFrameSize * frame ];
    int frameIds[ frame ];

    ret = doFrameRead( broadId, frameIds, readBuf, collectFrameSize, frame );
    if ( ret != frame )
    { return -1; }

    //! 4. all frames
    uint32 hashId;
    for ( int i = 0; i < frame; i++ )
    {
        //! get hash id
        memcpy( &hashId, (readBuf + i * collectFrameSize) + 2, 4 );

        sendHashMap.insert( frameIds[i], hashId );
    }

    return 0;
}

int CANBus::collectRecvId( QMap< int, quint32 > &sendRecvMap )
{
    byte buf[] = { MRQ_mc_CAN, MRQ_sc_CAN_RECEIVEID_Q };
    int ret;

    m_pRecvCache->clear();

    //! 1. broadcast
    DeviceId broadId( CAN_BROAD_ID );
    ret = doWrite( broadId, buf, sizeof(buf) );
    if ( ret != 0 )
    { return ret; }

    wait_us( mEnumerateTmo );

    //! 2. frame count
    int frame = size();logDbg()<<frame;
    if ( frame < 1 )
    { logDbg(); return -1; }

    //! 3. read all frame
    int collectFrameSize = 6;
    byte readBuf[ collectFrameSize * frame ];
    int frameIds[ frame ];

    ret = doFrameRead( broadId, frameIds, readBuf, collectFrameSize, frame );
    if ( ret != frame )
    { return -1; }

    //! 4. all frames
    uint32 recvId;
    for ( int i = 0; i < frame; i++ )
    {
        //! get recv id
        memcpy( &recvId, (readBuf + i * collectFrameSize) + 2, 4 );

        sendRecvMap.insert( frameIds[i], recvId );
    }

    return 0;
}

void CANBus::buildDeviceIds( QMap< int, quint32 > &sendHashMap,
                     QMap< int, quint32 > &sendRecvMap )
{
    DeviceId *pDeviceId;

    //! match the sendId: recvid, signature
    QMapIterator<int, quint32> iter(sendHashMap);
    while (iter.hasNext())
    {
        iter.next();

        pDeviceId = new DeviceId();
        if ( NULL == pDeviceId )
        { break; }

        pDeviceId->setSendId( iter.key() );
        pDeviceId->setSignature( iter.value() );

        Q_ASSERT( sendRecvMap.contains( iter.key() ) );
        pDeviceId->setRecvId( sendRecvMap.value( iter.key()) );

        mEnumDevices.append( pDeviceId );
    }

    //!  sort the device by id
    qSort( mEnumDevices.begin(),
           mEnumDevices.end(),
           deviceIdLessThan );
}

//! \todo check repeatable
int CANBus::assignIds( const modelSysPref &pref )
{
    //! find the same id
    int id = 1;
    foreach( DeviceId * pId, mEnumDevices )
    {
        pId->setSendId( pref.mSendIdFrom + id );
        pId->setRecvId( pref.mRecvIdFrom + id );

        //! \todo group id

        id++;

        if ( id > max_nodes )
        { return -1; }
    }

    return 0;
}

}
