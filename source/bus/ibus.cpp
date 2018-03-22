
#include ".\ibus.h"
#include ".\receiveCache.h"

#define hw_hb( d )  (byte)(((d)>>24)&0xff)
#define hw_lb( d )  (byte)(((d)>>16)&0xff)
#define lw_hb( d )  (byte)(((d)>>8)&0xff)
#define lw_lb( d )  (byte)(((d)>>0)&0xff)

#define expand_dw( dw ) lw_lb( dw ), lw_hb( dw ), hw_lb( dw ), hw_hb( dw )
#define expand_w( w )   lw_lb( w ),  lw_hb( w )

#define FAIL_TRY        failTry()

namespace MegaDevice {

static QMutex _bus_mutext;

IBus::IBus()
{
    mWtInterval = time_us(100);
    mFailTry = 2;

    mRdTmo = time_ms(200);
    mRdInterval = time_us(50);
    mRdTick = time_us(1000);

    mEnumerateTmo = time_ms( 50 );     //! ref to the device count, each = 200us

    m_pRecvCache = NULL;

    mBusType = e_bus_unk;
    mSpeed = 1000000;
    mPId = 0;
}

void IBus::setName( const QString & name )
{
    mName = name;
}
QString IBus::name()
{ return mName; }

void IBus::setBusType( IBus::eBusType type )
{
    mBusType = type;
}
IBus::eBusType IBus::busType()
{ return mBusType; }

void IBus::setSpeed( int speed )
{ mSpeed = speed; }
int IBus::speed()
{ return mSpeed; }

void IBus::setPId( int pid )
{ mPId = pid; }
int IBus::pId()
{ return mPId; }

int IBus::open(QString dev)
{ return 0; }
int IBus::open( int devType, int devId, int canId )
{ return 0; }
void IBus::close()
{ return; }

int IBus::size()
{ return 0;}
int IBus::flush( DeviceId &id )
{ return 0; }
int IBus::clear()
{ return 0; }
int IBus::doWrite( DeviceId &nodeId, byte *pBuf, int len )
{ return -1; }
int IBus::doReceive( int *pFrameId, byte *pBuf, int *pLen )
{ return -1; }
int IBus::doRead( DeviceId &nodeId, byte *pBuf, int cap, int *pLen )
{ return -1; }

int IBus::doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int *pLen )
{ return -1; }
int IBus::doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int eachFrameSize, int n )
{ return -1; }
int IBus::doSplitRead( DeviceId &nodeId, int packOffset, byte *pBuf, int cap, int *pLen )
{ return -1; }

int IBus::enumerate( const modelSysPref &pref )
{
    return 0;
}

void IBus::lock()
{
    _bus_mutext.lock();
}
void IBus::unlock()
{
    _bus_mutext.unlock();
}

void IBus::attachReceiveCache( receiveCache *pCache )
{
    Q_ASSERT( NULL != pCache );
    m_pRecvCache = pCache;

    m_pRecvCache->attachBus( this );
}

//! write
//! no para
int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode)
{
    byte buf[] = { mainCode, subCode };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

//! 1 para
int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0 )
{
    byte buf[] = { mainCode, subCode, v0 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt16 v0)
{
    byte buf[] = { mainCode, subCode, expand_w(v0) };

    return doWrite( nodeId, buf, sizeof( buf ) );

}

int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt32 v0)
{
    byte buf[] = { mainCode, subCode,
                   expand_dw( v0 )
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            float v0 )
{
    uint32 byts;

    memcpy( &byts, &v0, 4 );

    byte buf[] = { mainCode, subCode,
                   expand_dw(byts),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

//! 2 para
int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte v1)
{
    byte buf[] = { mainCode, subCode,
                   v0, v1
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            UInt16 v1)
{
    byte buf[] = { mainCode, subCode,
                   v0,
                   expand_w(v1),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            UInt32 v1)
{
    byte buf[] = { mainCode, subCode,
                   v0,
                   expand_dw(v1),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            float v1)
{
    uint32 byts;

    memcpy( &byts, &v1, 4 );

    byte buf[] = { mainCode, subCode,
                   v0,
                   expand_dw(byts),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt16 v0,
            float v1)
{
    uint32 byts;
    memcpy( &byts, &v1, 4 );

    byte buf[] = { mainCode, subCode,
                   expand_w(v0),
                   expand_dw(byts),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

//! 3 para
int IBus::write( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte v1,
            byte v2 )
{
    byte buf[] = { mainCode, subCode,
                   v0, v1, v2
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           sbyte v2)
{
    byte byts;

    memcpy( &byts, &v2, 1 );

    byte buf[] = { mainCode, subCode,
                   v0, v1, byts
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           Int16 v2)
{
    UInt16 byts;

    memcpy( &byts, &v2, 2 );

    byte buf[] = { mainCode, subCode,
                   v0, v1,
                   expand_w(byts),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           UInt16 v2)
{
    UInt16 byts;

    memcpy( &byts, &v2, 2 );

    byte buf[] = { mainCode, subCode,
                   v0, v1,
                   expand_w(byts),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           UInt32 v2)
{

    byte buf[] = { mainCode, subCode,
                   v0, v1,
                   expand_dw(v2),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

int IBus::write( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           float v2)
{
    UInt32 byts;

    memcpy( &byts, &v2, 4 );

    byte buf[] = { mainCode, subCode,
                   v0, v1,
                   expand_dw(byts),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

//! 4 para
int IBus::write( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           byte v2,
           float f3 )
{
    UInt32 byts;

    memcpy( &byts, &f3, 4 );

    byte buf[] = { mainCode, subCode,
                   v0, v1, v2,
                   expand_dw(byts),
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}
//! 6 para
int IBus::write( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           byte v2,
           byte v3,
           byte v4,
           byte v5
           )
{
    byte buf[] = { mainCode, subCode,
                   v0, v1, v2,
                   v3, v4, v5,
                 };

    return doWrite( nodeId, buf, sizeof( buf ) );
}

//! no para
int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            bool bQuery  )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            byte * v1,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            byte * v1,
            byte * v2,
            bool bQuery)
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            byte * v1,
            quint32 * v2,
            bool bQuery)
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            byte * v1,
            byte * v2,
            byte * v3,
            byte * v4,
            byte * v5,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, v3, v4, v5, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt16 * v0,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt32 * v0,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            f32 * v0,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

//! 1 para
int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            byte * v2,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            int8 * v2,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            int32 * v2,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            byte * v2,
            byte * v3,
            byte * v4,
            byte * v5,
            bool bQuery
            )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, v3, v4, v5, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            UInt16 * v1,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            UInt32 * v1,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            Int16 * v1,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            Int32 * v1,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            f32 * v1,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt16 v0,
            f32 * v1,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

//! 2 para
int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte v1,
            byte * v2,
            bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte v1,
            byte * v2,
            byte * v3,
            byte * v4,
            byte * v5,
            bool bQuery
            )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, v3, v4, v5, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           sbyte * v2,
           bool bQuery)
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           Int16 * v2,
           bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           UInt16 * v2,
           bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           UInt32 * v2,
           bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

int IBus::read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           f32 * v2,
           bool bQuery )
{
    int ret;
    for ( int i = 0; i < FAIL_TRY; i++ )
    {
        ret = _read( nodeId, mainCode, subCode, v0, v1, v2, bQuery );
        if ( 0 == ret )
        { break; }
    }
    return ret;
}

//! ******* sub apis ********
int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            bool bQuery  )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );
        ret = write( nodeId, mainCode, subCode );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[3];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    *v0 = readBuf[2];
    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            byte * v1,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );
        ret = write( nodeId, mainCode, subCode );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    *v0 = readBuf[2];
    *v1 = readBuf[3];
    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            byte * v1,
            byte * v2,
            bool bQuery)
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );
        ret = write( nodeId, mainCode, subCode );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[5];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    *v0 = readBuf[2];
    *v1 = readBuf[3];
    *v2 = readBuf[4];
    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            byte * v1,
            quint32 * v2,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );
        ret = write( nodeId, mainCode, subCode );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[6];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    *v0 = readBuf[2];
    *v1 = readBuf[3];

    memcpy( v2, readBuf + 2, 4 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte * v0,
            byte * v1,
            byte * v2,
            byte * v3,
            byte * v4,
            byte * v5,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );
        ret = write( nodeId, mainCode, subCode );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+6];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    *v0 = readBuf[2];
    *v1 = readBuf[3];
    *v2 = readBuf[4];

    *v3 = readBuf[5];
    *v4 = readBuf[6];
    *v5 = readBuf[7];

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt16 * v0,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );
        ret = write( nodeId, mainCode, subCode );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v0, readBuf + 2, 2 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt32 * v0,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );
        ret = write( nodeId, mainCode, subCode );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v0, readBuf + 2, 4 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            f32 * v0,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v0, readBuf + 2, 4 );

    return 0;
}

//! 1 para
int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+1+1];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 1 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            byte * v2,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+3];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 1 );
    memcpy( v2, readBuf + 4, 1 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            int8 * v2,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+3];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 1 );
    memcpy( v2, readBuf + 4, 1 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            int32 * v2,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 1 );
    memcpy( v2, readBuf + 4, 4 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte * v1,
            byte * v2,
            byte * v3,
            byte * v4,
            byte * v5,
            bool bQuery
            )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+6];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 1 );
    memcpy( v2, readBuf + 4, 1 );
    memcpy( v3, readBuf + 5, 1 );
    memcpy( v4, readBuf + 6, 1 );
    memcpy( v5, readBuf + 7, 1 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            UInt16 * v1,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+1+2];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 2 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            UInt32 * v1,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+1+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 4 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            Int16 * v1,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+1+2];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 2 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            Int32 * v1,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+1+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 4 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            f32 * v1,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+1+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 3, 4 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            UInt16 v0,
            f32 * v1,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v1, readBuf + 4, 4 );

    return 0;
}

//! 2 para
int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte v1,
            byte * v2,
            bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0, v1 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+1];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v2, readBuf + 4, 1 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
            byte subCode,
            byte v0,
            byte v1,
            byte * v2,
            byte * v3,
            byte * v4,
            byte * v5,
            bool bQuery
            )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0, v1 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    *v2 = readBuf[4];
    *v3 = readBuf[5];
    *v4 = readBuf[6];
    *v5 = readBuf[7];

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           sbyte * v2,
           bool bQuery)
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0, v1 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+1];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    *v2 = readBuf[4];

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           Int16 * v2,
           bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0, v1 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+2];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v2, readBuf + 4, 2 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           UInt16 * v2,
           bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0, v1 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+2];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v2, readBuf + 4, 2 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           UInt32 * v2,
           bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0, v1 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    memcpy( v2, readBuf + 4, 4 );

    return 0;
}

int IBus::_read( DeviceId &nodeId, byte mainCode,
           byte subCode,
           byte v0,
           byte v1,
           f32 * v2,
           bool bQuery )
{
    int ret;

    if( bQuery )
    {
        flush( nodeId );

        ret = write( nodeId, mainCode, subCode, v0, v1 );
        if ( ret != 0 )
        { return ret; }
    }

    int retLen;
    byte readBuf[2+2+4];
    ret = doRead( nodeId, readBuf, sizeof(readBuf), &retLen );
    if ( ret != 0 )
    { return ret; }

    //! \todo check main/sub code

    memcpy( v2, readBuf + 4, 4 );

    return 0;
}

}

