#ifndef RECEIVECACHE_H
#define RECEIVECACHE_H

#include <QtCore>

#include "../bus/ibus.h"

enum eventId
{
    event_none,

    event_exception_min, //! exceptions [_min, _max)

    event_lose_step = event_exception_min,
    event_over_pe,

    event_exception_max,

    event_stop,

    event_status,       //! status change

};
Q_DECLARE_METATYPE( eventId )

enum exceptionAction
{
    exception_action_none,
    exception_action_stop,
    exception_action_prompt,
};

class frameEvent : public QByteArray
{
public:
    frameEvent();

    void setId( eventId id );
    eventId getId();

    void setEnable( bool b );
    bool getEnable();

    void setMainSubCode( int mainCode, int subCode );

protected:
    eventId mEventId;
    bool mbEn;
};

class frameData : public QByteArray
{
public:
    frameData();

    void setFrameId( int frameid );
    int getFrameId();

protected:
    int mFrameId;
};
Q_DECLARE_METATYPE( frameData )
Q_DECLARE_METATYPE( frameData* )

//! for each device
class frameHouse : public QQueue< frameData >
{
public:
    frameHouse();

public:
    void setId( int sendId, int recvId );
    void setSendId( int sendId );
    void setRecvId( int recvId );
    int sendId();
    int recvId();

    void lock();
    void unlock();

    void clear();

public:
    int mSendId, mRecvId;       //! in device view
    QMutex mMutex;
};

class frameWarehouse
{
public:
    frameWarehouse();
    ~frameWarehouse();

public:
    void inFrame( frameData & data );

    frameHouse *findHouseByRecvId( int recvId );
    frameHouse *findHouseBySendId( int sendId );

    QList< frameHouse *> *houses();

protected:
    QList< frameHouse *> mHouses;
};


class receiveCache : public QThread
{
    Q_OBJECT
public:
    receiveCache( QObject *parent = 0 );
    ~receiveCache();

Q_SIGNALS:
    void sig_event( eventId id, frameData );

public:
    void attachBus( MegaDevice::IBus *pBus );
    void detachBus();

    int  setFrameEventEnable( frameEvent &evt, bool b );
    bool getFrameEventEnable( frameEvent &evt );
    frameEvent* findFrameEvent( frameEvent &evt );
    frameEvent* findFrameEvent( eventId id );

    void append( frameData &ary );
//    int readAFrame( frameData &ary, int tmous );
    int readAFrame( MegaDevice::DeviceId &nodeId, int *pFrameId, byte *pBuf, int *pLen, int tmous );
    int readFrame( MegaDevice::DeviceId &nodeId, frameHouse &frames );

    void flush( MegaDevice::DeviceId &nodeId );
    void clear();

    int frameCount();

    //! thread
    void lock();
    void unlock();

    void lockWarehouse();
    void unlockWarehouse();

protected:
    virtual void run();

protected:
    bool detectEvent( frameData &ary );

protected:
    MegaDevice::IBus *m_pBus;
//    int mNodeId;

//    QQueue< frameData > mRecvCache;

    frameWarehouse mFrameWarehouse;

    QMutex mCacheMutex;
    QMutex mThreadMutex;
    QList< frameEvent* > mEvents;

    byte mFrameBuf[8];
};


#endif // RECEIVECACHE_H
