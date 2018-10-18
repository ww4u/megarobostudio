#ifndef RECEIVECACHE_H
#define RECEIVECACHE_H

#include <QtCore>

#include "../bus/ibus.h"

enum eventId
{
    event_none,

    event_exception_min, //! exceptions [_min, _max)

    event_over_step = event_exception_min,
    event_over_angle,
    event_over_distance,


    event_exception_max,

    event_stop,

    event_status,       //! status change
//    event_alarm,

};
Q_DECLARE_METATYPE( eventId )

enum exceptionAction
{
    exception_action_stop,
    exception_action_prompt,
    exception_action_prompt_stop,
};

class frameEvent : public QByteArray
{
public:
    frameEvent();

    void setId( eventId id );
    eventId getId();

    void setEnable( bool b );
    bool getEnable();

    void setRepeatAble( bool b );
    bool repeatAble();

    void setAckAble( bool b );
    bool ackAble();

    void setMainSubCode( int mainCode, int subCode );
    void setMainSubCode( int mainCode,
                         int subCode,
                         int patterns[],
                         int pattLen );

    bool match( frameData &ary );

    int ackEvent( MegaDevice::IBus *pBus,
                  MegaDevice::DeviceId &did,
                  frameData &data );
protected:
    eventId mEventId;
    bool mbEn;
    bool mRepeatAble;
    bool mAckAble;

    int mPatterns[6];
    int mPatternLen;

};



//! for each device
class frameHouse : public QQueue< frameData >
{
public:
    frameHouse();

public:
    void append(const frameData &t);
    void append(const frameHouse &t);

public:
    void setId( int sendId, int recvId );
    void setSendId( int sendId );
    void setRecvId( int recvId );
    int sendId();
    int recvId();

    void lock();
    void unlock();

    void clear();

    int bytes();

public:
    int mSendId, mRecvId;       //! in device view
    QMutex mMutex;
    int mBytes;
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

protected:
    static QMutex _threadMutex;
    static quint64 _timeStamp;

    static QList< frameEvent* > _frameEvents;
    static QMutex _frameEventsMutex;

    static QSemaphore _eventSema;
    static bool _eventEn;

    static QMap<int, bool> _mapBypass;

    static QQueue< frameData > _cacheEvents;
    static QMutex _cacheEventsMutex;

public:

//    static void pendBus( bool b );
//    static bool isPendBus();

    //! thread
    static void lock();
    static void unlock();

    static void lockFrameEvents();
    static void unlockFrameEvents();

    static void accEvent();
    static void decEvent();
    static int availableEvent();

    static quint64 timeStamp();

    static void cli();
    static void sti();

    static void setByPass( int devId, bool b );
    static bool getByPass( int devId );

    static bool queueInEvent( bool bRepetable, frameData &frame );
    static bool dequeueEvent();

public:
    receiveCache( QObject *parent = 0 );
    ~receiveCache();

Q_SIGNALS:
    void sig_event( eventId id, frameData );

public:
    void attachBus( MegaDevice::IBus *pBus );
    void detachBus();

    static int  setFrameEventEnable( frameEvent &evt, bool b );
    static bool getFrameEventEnable( frameEvent &evt );
    static frameEvent* findFrameEvent( frameEvent &evt );
    static frameEvent* findFrameEvent( eventId id );

    static void clearFrameEvent();

    void append( frameData &ary );
//    int readAFrame( frameData &ary, int tmous );
    int readAFrame( MegaDevice::DeviceId &nodeId, int *pFrameId, byte *pBuf, int *pLen, int tmous );
    int readFrame( MegaDevice::DeviceId &nodeId, frameHouse &frames );

    void flush( MegaDevice::DeviceId &nodeId );
    void clear();

    int frameCount( int id = -1 );
    int frameBytes( int id = -1 );

    void lockWarehouse();
    void unlockWarehouse();

protected:
    virtual void run();

protected:
    bool detectEvent( frameData &ary );

protected:
    MegaDevice::IBus *m_pBus;

    frameWarehouse mFrameWarehouse;

    QMutex mCacheMutex;

    byte mFrameBuf[8];

};


#endif // RECEIVECACHE_H
