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

CANBus::CANBus()
{
    mHandle = 0;
    mDevType = 6;
    mDevId = 0;
    mCanId = 0;

    mName = "CAN-BUS";
    mBusType = e_bus_can;
}

CANBus::~CANBus()
{
    close();

    //! clear the device id
    delete_all( mEnumDevices );

    delete_all( mDevices );
}

int CANBus::open( int devType, int devId, int canId )
{
    mDevType = devType;
    mDevId = devId;
    mCanId = canId;

    //! full dll path
    QString dllName;
    if ( mPId == 1 )
    { dllName = "usbcanii.dll"; }
    else
    { dllName = "MegaCanDevice.dll"; }

    QString fullPath ;
    fullPath = QCoreApplication::applicationDirPath() + QDir::separator() + dllName;

    fullPath = QDir::toNativeSeparators( fullPath );
    if ( !mApi.load( fullPath) )
    {
        sysLog( fullPath );
        sysError( dllName, QObject::tr("load fail") );
        sysError( mApi.mDll.errorString() );
        return -1;
    }

    mHandle = mApi.open( mDevType, mDevId, mCanId );
    if ( mHandle != 1 )
    {
        sysError( QObject::tr("can open fail") );
        return -2;
    }

    //! init
    if ( 0 != initBus() )
    {
        close();
        return -3;
    }

    return 0;
}
void CANBus::close()
{
    if ( mHandle != 0 )
    {
        int ret;
        ret = mApi.close( mDevType, mDevId );
        logDbg()<<ret;
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
    canConfig.AccCode = 0xffffffff;
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
    canConfig.Timing0 = _bps_timer_matrix[speedId].timer0;
    canConfig.Timing1 = _bps_timer_matrix[speedId].timer1;

    int ret = mApi.init( mDevType, mDevId, mCanId, &canConfig );
    if ( ret != 1 )
    {
        sysError( QObject::tr("Can init fail") );
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
    mApi.clear( mDevType, mDevId, mCanId );

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

int CANBus::doWrite(DeviceId &nodeId, byte *pBuf, int len)
{
    Q_ASSERT( NULL != pBuf );
    Q_ASSERT( len > 0 && len < 9 );

    CAN_OBJ canObj;

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
//logDbg()<<canObj.ID<<pBuf[0]<<pBuf[1]<<mWtInterval;
    int ret;
    IBus::lock();
    ret = mApi.transmit( can_device_desc, &canObj, 1);
    IBus::unlock();
    if ( 1 !=  ret )
    {
        logDbg();
        return -1;
    }

    IBus::wait_us( mWtInterval );

    return 0;
}

//! read from bus
int CANBus::doReceive( int *pFrameId, byte *pBuf, int *pLen )
{
    CAN_OBJ canObj;
    int ret;

    IBus::lock();
    //! receive fail
    //! \note timout invalid and can not be 0
    ret = mApi.receive( can_device_desc, &canObj, 1, 1 );
    IBus::unlock();

    if ( ret != 1 )
    { return -1; }
    //! success
    else
    {  }

    if ( canObj.DataLen < 1 || canObj.DataLen > 8 )
    { return -2; }

    //! export
    *pFrameId = canObj.ID;
    for ( int i = 0; i < canObj.DataLen; i++ )
    {
        pBuf[i] = canObj.Data[i];
    }
    *pLen = canObj.DataLen;

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
        pFrameId[i] = frames[i].getFrameId();
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

    beginEnumerate();

    ret = collectHash();
    if ( ret != 0 ) return ret;

    ret = assignIds( pref );
    if ( ret != 0 ) return ret;

    endEnumerate();

    return 0;
}

int CANBus::getDevType()
{ return mDevType; }
int CANBus::getDevId()
{ return mDevId; }
int CANBus::getCanId()
{ return mCanId; }

void CANBus::beginEnumerate()
{
    //! destroy the enumerate
    delete_all( mEnumDevices );
}

int CANBus::collectHash()
{
    byte buf[] = { mc_CAN, sc_CAN_NETMANAGEHASH_Q };
    int ret;

    m_pRecvCache->clear();

    //! 1. broadcast
    DeviceId broadId( CAN_BROAD_ID );
    ret = doWrite( broadId, buf, sizeof(buf) );
    if ( ret != 0 )
    { return ret; }

    logDbg()<<mEnumerateTmo;
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
    DeviceId *pDeviceId;
    uint32 hashId;
    for ( int i = 0; i < frame; i++ )
    {
        pDeviceId = new DeviceId();
        Q_ASSERT( NULL != pDeviceId );

        //! get hash id
        memcpy( &hashId, (readBuf + i * collectFrameSize) + 2, 4 );

        pDeviceId->setSendId( frameIds[i] );logDbg()<<frameIds[i];
        pDeviceId->setSignature( hashId );logDbg()<<QString::number( hashId, 16 );

        mEnumDevices.append( pDeviceId );
        logDbg()<<i;
    }

    return 0;

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

void CANBus::endEnumerate()
{
    //! clear the current
    delete_all( mDevices );

    //! copy the enums
    mDevices = mEnumDevices;

    //! \note keep the deviceIds
    mEnumDevices.clear();
}

}
