#ifndef CANBUS_H
#define CANBUS_H

#include "../../include/mcstd.h"

#include "ibus.h"

#include "canapi.h"

//! def
#define CAN_BROAD_ID    0X01
#define CAN_BROAD_CHAN  128
#define SUB_GROUP_BASE  129

namespace MegaDevice {

class CANBus : public IBus
{
private:
    static QString _visaRsrc;

public:
    CANBus();
    ~CANBus();

public:
    int open( const modelSysPref &pref,
              int devType = 6,
              int devId=0, int seqId = 0, int canId=0,
              const QString &desc="");
    void close();

protected:
    int initBus();
    int matchSpeed( int speed );

public:
    virtual int size();
    virtual int flush( DeviceId &nodeId );
    virtual int clear();

    virtual int doSend( const QString &buf );

    //! raw write
    virtual int doWrite( byte *pBuf, int len );
    virtual int doRead( byte *pOutBuf, int len, int tmo, int &retLen );

    virtual int doWrite( DeviceId &nodeId, byte *pBuf, int len );
    virtual int doWrite( QList<frameData> &canFrames );

    //! read from bus
    virtual int doReceive( QList<frameData> &canFrames );

//    virtual int doRead( DeviceId &nodeId, byte *pBuf, int cap, int *pLen );

//    virtual int doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int *pLen );
//    virtual int doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int eachFrameSize, int n );
//    virtual int doSplitRead( DeviceId &nodeId, int packOffset,
//                             byte *pBuf,
//                             int cap,
//                             int *pLen );

    virtual int enumerate( const modelSysPref &pref );
	
    int getDevType();
    int getCanId();

    qint64 frames();

protected:
    void beginEnumerate();
    void endEnumerate();

    int autoEnumerate( const modelSysPref &pref );
    virtual int rawEnumerate( const modelSysPref &pref );

    int collectHash( );

    int collectHash( QMap< int, quint32 > &sendHashMap );
    int collectSendId( QMap< int, quint32 > &sendSendMap );
    int collectRecvId( QMap< int, quint32 > &sendRecvMap );

    void buildDeviceIds( QMap< int, quint32 > &sendHashMap,
                         QMap< int, quint32 > &sendRecvMap );

    void buildDeviceIds( QMap< int, quint32 > &sendHashMap,
                         QMap< int, quint32 > &sendSendMap,
                         QMap< int, quint32 > &sendRecvMap );

    int assignIds( const modelSysPref &pref );

protected:
    int mHandle;
    int mDevType;

    int mCanId;

    CANApi mApi;
    qint64 mFrames;

};

}

#endif // CANBUS_H
