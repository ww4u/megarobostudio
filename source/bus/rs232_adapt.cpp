#include "rs232bus.h"
#include "receivecache.h"
#include "../../sys/sysapi.h"

namespace MegaDevice {

int Rs232Bus::size()
{
    if ( mViDevice == 0 )
    { return 0; }

    Q_ASSERT( m_pRecvCache != NULL );
    return m_pRecvCache->frameCount();
}
int Rs232Bus::flush( DeviceId &nodeId )
{
    IBus::lock();

    viFlush( mViDevice, VI_READ_BUF_DISCARD );

    if ( NULL != m_pRecvCache )
    {
        m_pRecvCache->flush( mDeviceId );
    }

    IBus::unlock();

    return 0;
}
int Rs232Bus::clear()
{
    if ( m_pRecvCache != NULL )
    { m_pRecvCache->clear(); }

    return 0;
}

int Rs232Bus::doSend( const QString &buf )
{
    QByteArray ary = buf.toLatin1();
    return write( ary );
}

//! raw write
int Rs232Bus::doWrite( byte *pBuf, int len )
{
    QByteArray ary( (const char*)pBuf, len );

    return write( ary );
}
int Rs232Bus::doRead( byte *pOutBuf, int len, int tmo, int &retLen )
{
    int ret;

    //! read
    U8List datList;
    ret = read( datList );
    if ( ret != 0 )
    { return ret; }

    if ( len > datList.size() )
    {  retLen = datList.size(); }
    else
    { retLen = len; }

    //! copy
    for ( int i = 0; i < retLen; i++ )
    {
        pOutBuf[i] = datList.at( i );
    }

    return 0;
}

int Rs232Bus::doWrite( DeviceId &nodeId, byte *pBuf, int len )
{
    return doWrite( pBuf, len );
}
int Rs232Bus::doWrite( QList<frameData> &canFrames )
{
    int ret;
    foreach ( frameData dat, canFrames )
    {
        ret = write( dat );
        if ( ret != 0 )
        { return ret; }
    }

    return 0;
}

//! read from bus
int Rs232Bus::doReceive( QList<frameData> &canFrames )
{
    U8List datList;
    int ret;

    ret = read( datList );
    if ( ret != 0 )
    { return ret; }

    //! read only one frame
    frameData tFrame;
    tFrame.clear();
    tFrame.setFrameId( mDeviceId.mSendId );     //logDbg()<<mDeviceId.mSendId;
    tFrame.setDevId( mDevId );
    for ( int i = 0; i < datList.size() && i < FRAME_LEN; i++ )
    {
        tFrame.append( datList.at(i) );
    }

    canFrames.append( tFrame );

    return 0;
}

int Rs232Bus::rawEnumerate( const modelSysPref &pref )
{
    int ret;
logDbg();
    //! send-hash
    QMap< int, quint32 > sendHashMap;
    ret = collectHash( sendHashMap );
    if ( ret != 0 )
    { return ret; }

    //! send-send
    QMap< int, quint32 > sendSendMap;
    ret = collectSendId( sendSendMap );
    if ( ret != 0 )
    { return ret; }

    //! send-recv
    QMap< int, quint32 > sendRecvMap;
    ret = collectRecvId( sendRecvMap );
    if ( ret != 0 )
    { return ret; }

    //! check the size
    if ( sendRecvMap.size() != sendHashMap.size() )
    {
        sysError( QObject::tr("ID match fail") );
        return -1;
    }

    if ( sendSendMap.size() != sendHashMap.size() )
    {
        sysError( QObject::tr("ID match fail") );
        return -1;
    }

    //! check the ids
    QList<int> sendHashKeys, sendSendKeys, sendRecvKeys;
    sendHashKeys = sendHashMap.keys();
    sendSendKeys = sendSendMap.keys();
    sendRecvKeys = sendRecvMap.keys();

    qSort( sendHashKeys );
    qSort( sendRecvKeys );

    //! mathch
    if ( sendHashKeys != sendRecvKeys
         || sendHashKeys != sendRecvKeys )
    {
        sysError( QObject::tr("ID match fail") );
        return -1;
    }
//logDbg();

    buildDeviceIds( sendHashMap, sendSendMap, sendRecvMap );

//logDbg();
    return 0;
}


//int Rs232Bus::doRead( DeviceId &nodeId, byte *pBuf, int cap, int *pLen )
//{
//    return -1;
//}

//int Rs232Bus::doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int *pLen )
//{
//    //! read from cache by timeout
//    Q_ASSERT( NULL != m_pRecvCache );

//    return m_pRecvCache->readAFrame( nodeId, pFrameId, pBuf, pLen, mRdTmo );
//}
//int Rs232Bus::doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int eachFrameSize, int n )
//{
//    return -1;
//}
//int Rs232Bus::doSplitRead( DeviceId &nodeId, int packOffset,
//                         byte *pBuf,
//                         int cap,
//                         int *pLen )
//{
//    return -1;
//}

//!
//int Rs232Bus::enumerate( const modelSysPref &pref )
//{
//    return -1;
//}

}
