#ifndef CANBUS_H
#define CANBUS_H

#include "../../include/mcstd.h"

#include "ibus.h"

#include "canapi.h"

//! def
#define CAN_BROAD_ID    0X01
#define CAN_BROAD_CHAN  128

namespace MegaDevice {

class CANBus : public IBus
{
public:
    CANBus();
    ~CANBus();

public:
    int open( int devType = 6,
              int devId=0, int canId=0,
              const QString &desc="");
    void close();

protected:
    int initBus();
    int matchSpeed( int speed );

public:
    virtual int size();
    virtual int flush( DeviceId &nodeId );
    virtual int clear();

    virtual int doWrite( DeviceId &nodeId, byte *pBuf, int len );
    virtual int doWrite( QList<frameData> &canFrames );

    //! read from bus
//    virtual int doReceive( int *pFrameId, byte *pBuf, int *pLen );
    virtual int doReceive( QList<frameData> &canFrames );

    virtual int doRead( DeviceId &nodeId, byte *pBuf, int cap, int *pLen );

    virtual int doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int *pLen );
    virtual int doFrameRead( DeviceId &nodeId, int *pFrameId, byte *pBuf, int eachFrameSize, int n );
    virtual int doSplitRead( DeviceId &nodeId, int packOffset,
                             byte *pBuf,
                             int cap,
                             int *pLen );


    virtual int enumerate( const modelSysPref &pref );
	
//    QString getDesc();
    int getDevType();
    int getDevId();
    int getCanId();

protected:
    void beginEnumerate();
    void endEnumerate();

    int autoEnumerate( const modelSysPref &pref );
    int rawEnumerate( const modelSysPref &pref );

    int collectHash( );

    int collectHash( QMap< int, quint32 > &sendHashMap );
    int collectRecvId( QMap< int, quint32 > &sendRecvMap );

    void buildDeviceIds( QMap< int, quint32 > &sendHashMap,
                         QMap< int, quint32 > &sendRecvMap );

    int assignIds( const modelSysPref &pref );



protected:
    int mHandle;
    int mDevType;
    int mDevId;
    int mCanId;

    CANApi mApi;

};

}

#endif // CANBUS_H
