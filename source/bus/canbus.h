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
    int open( int devType = 6, int devId=0, int canId=0 );
    void close();

protected:
    int initBus();
    int matchSpeed( int speed );

public:
    virtual int size();
    virtual int flush( DeviceId &nodeId );
    virtual int clear();

    virtual int doWrite( DeviceId &nodeId, byte *pBuf, int len );
    //! read from bus
    virtual int doReceive( int *pFrameId, byte *pBuf, int *pLen );

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

    int collectHash();
    int assignIds( const modelSysPref &pref );

    void endEnumerate();

protected:
    int mHandle;
    int mDevType;
    int mDevId;
    int mCanId;

    CANApi mApi;

};

}

#endif // CANBUS_H
