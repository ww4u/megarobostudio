
#ifndef _IBUS_H_
#define _IBUS_H_

#include "../../include/datatype.h"
#include <QtCore>

#include "deviceid.h"

#include "../model/modelsyspref.h"

class receiveCache;

#define is_phy_bus( busType )       ( ( (busType) == MegaDevice::IBus::e_bus_can ) \
                                       || ( (busType) == MegaDevice::IBus::e_bus_232 ) )

class frameData : public QByteArray
{
public:
    frameData();

    void setFrameId( int frameid );
    int frameId();

    void setDevId( int devId );
    int devId();

    void setTimeStamp( quint64 t );
    quint64 timeStamp();

    //! same except ts
    bool isLike( frameData &data );

    QString toString( bool tTs = true, const QString &lineSep="\n" );
protected:
    int mFrameId;
    int mDevId;
    quint64 mTimeStamp;
};
Q_DECLARE_METATYPE( frameData )
Q_DECLARE_METATYPE( frameData* )

namespace MegaDevice {

#define FRAME_LEN   8

class IBus
{
public:
    enum eBusType
    {
        e_bus_unk,
        e_bus_can,
        e_bus_232,
        e_bus_file,
    };

protected:
    int mRdTmo;           //! us -- read timeout
    int mRdTick;          //! us -- cache read
    int mRdInterval;      //! us -- bus read
    int mRecvTmo;         //! ms

    int mEnumerateTmo;    //! us -- enumerate timeout

    int mWtInterval;      //! us
    int mFailTry;         //! time

    int mSpeed;           //! speed
    int mPId;             //! productId: 0 -- mega can
                          //! 1 -- usb can ii
                          //!
    unsigned int mDevId;           //! device id -- device handle
    int mDefRM;

    QString mName;

    eBusType mBusType;
    byte mLinkType;         //! can,232
    DeviceId mDeviceId;     //! the attached bus

                            //! receive cache
    receiveCache *m_pRecvCache;

    QMutex mbus_mutext;     //! for each bus
    QMutex mbus_query_mutext;

public:
    QList< DeviceId *> mDevices;
    QList< DeviceId *> mEnumDevices;

public :
    IBus();
    virtual ~IBus()
    {}

    void setName( const QString & name );
    QString name();

    void setBusType( eBusType type );
    eBusType busType();

    void setSpeed( int speed );
    int speed();

    void setPId( int pid );
    int pId();

    void setDevId( int devId );
    int devId();

    void setDeviceId( DeviceId &id );
    DeviceId deviceId();

    virtual int open( const modelSysPref &pref,
                      int devType,
                      int devId,
                      int seqId,
                      int canId,
                      const QString &desc="" );

    virtual void close();

    virtual int size();
    virtual int flush( DeviceId &id );
    virtual int clear();

    virtual int doSend( const QString &buf );

    //! raw write
    virtual int doWrite( byte *pBuf, int len );
    virtual int doRead( byte *pOutBuf, int len, int tmo, int &retLen );

    //! write to bus
    virtual int doWrite( DeviceId &id, byte *pBuf, int len );
    virtual int doWrite( QList<frameData> &canFrames );
    //! read from bus
    virtual int doReceive( int *pFrameId, byte *pBuf, int *pLen );

    virtual int doReceive( QList< frameData > &receiveFrames );

    virtual int doRead( DeviceId &id, byte *pBuf, int cap, int *pLen );

    virtual int doFrameRead( DeviceId &id, int *pFrameId, byte *pBuf, int *pLen );
    virtual int doFrameRead( DeviceId &id, int *pFrameId, byte *pBuf, int eachFrameSize, int n );
    virtual int doSplitRead( DeviceId &id, int packOffset, byte *pBuf, int cap, int *pLen );

    virtual int enumerate( const modelSysPref &pref );

    void lock();    //! write
    void unlock();

    void lockQuery();
    void unlockQuery();

    void wait_ms( int ms )
    {
        if ( ms > 0 )
        { QThread::msleep( ms ); }
    }
    void wait_us( int us )
    {
        if ( us > 0 )
        { QThread::usleep(us); }
    }

    void setRecvTmo( int tmo )
    { mRecvTmo = tmo; }
    int recvTmo()
    { return mRecvTmo; }

    void setRdTmo(int tmo)
    { mRdTmo = tmo; }
    int rdTmo()
    { return mRdTmo; }

    void setRdInterval( int interval )
    { mRdInterval = interval; }
    int rdInterval()
    { return mRdInterval; }

    void setRdTick( int tick )
    { mRdTick = tick; }
    int rdTick()
    { return mRdTick; }

    void setEnumTmo( int tmo )
    { mEnumerateTmo = tmo; }
    int enumTmo()
    { return mEnumerateTmo; }

    void setWtInterval( int interval )
    { mWtInterval = interval; }
    int wtInterval()
    { return mWtInterval; }

    void setFailTry( int failTry )
    { mFailTry = failTry; }
    int failTry()
    { return mFailTry; }

//    void attachReceiveCache( receiveCache *pCache );
    receiveCache *receiveProxy()
    { return m_pRecvCache; }

	//! apis
	public:
        //! raw write
        int write( byte *pData, int len );
        int read( byte *pOut, int len, int tmo, int &retLen );

        //! write buffer
        int write( DeviceId &nodeId, byte *pData, int len );

		//! write
        //! no para
        int write( DeviceId &id, byte mainCode,
                    byte subCode);

        //! 1 para
        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0 );

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt16 v0);

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt32 v0);

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    float v0 );

        //! 2 para
        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1);

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    UInt16 v1);

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    UInt32 v1);

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    Int32 v1);

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    float v1);

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt16 v0,
                    float v1);

        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt16 v0,
                    UInt32 v1);

        int write( DeviceId &id, byte mainCode, byte subCode,
                   byte ary[], int len );

        //! 3 para
        int write( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1,
                    byte v2 );

        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   sbyte v2);

        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   Int16 v2); 

        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   UInt16 v2);

        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   UInt16 v1,
                   UInt16 v2);

        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   UInt32 v2);

        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   float v2);

        //! 4 para
        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   byte v2,
                   float f3 );

        //! 5 para
        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   uint16 v2,
                   uint16 v3
                    );


        //! 6 para
        int write( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   byte v2,
                   byte v3,
                   byte v4,
                   byte v5
                   );

        //! no para
        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    quint32 * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    byte * v2,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    byte * v2,
                    byte * v3,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    quint32 * v2,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    byte * v5,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt16 * v0,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt32 * v0,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    f32 * v0,
                    bool bQuery = true );

        //! 1 para
        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    byte * v2,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    int8 * v2,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    int32 * v2,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    quint32 * v2,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    byte * v5,
                    bool bQuery = true
                    );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    UInt16 * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    UInt32 * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    Int16 * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    Int32 * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    f32 * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt16 v0,
                    f32 * v1,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    uint16 * v1,
                    uint16 * v2,
                    bool bQuery = true );

        //! 2 para
        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1,
                    byte * v2,
                    bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    bool bQuery = true
                    );

        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    byte * v5,
                    bool bQuery = true
                    );

        int read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   sbyte * v2,
                   bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   Int16 * v2,
                   bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   UInt16 * v2,
                   bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   UInt32 * v2,
                   bool bQuery = true );

        int read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   f32 * v2,
                   bool bQuery = true );

        //! 2 para
        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1,
                    UInt16 *v2,
                    UInt16 *v3,

                    bool bQuery = true );

        //! 3 para
        int read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    UInt16 v1,
                    UInt16 v2,
                    byte * v3,
                    bool bQuery = true );


        //! ******** sub apis ********
        bool verifyCode( byte mainCode,
                         byte subCode,
                         byte *pBuf );
        bool verifyCode( byte mainCode,
                         byte subCode,
                         byte v0,
                         byte *pBuf );
        bool verifyCode( byte mainCode,
                         byte subCode,
                         UInt16 v0,
                         byte *pBuf );

        bool verifyCode( byte mainCode,
                         byte subCode,
                         byte v0,
                         byte v1,
                         byte *pBuf );
        bool verifyCode( byte mainCode,
                         byte subCode,
                         byte v0,
                         UInt16 v1,
                         UInt16 v2,
                         byte *pBuf );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    quint32 * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    byte * v2,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    byte * v2,
                    byte * v3,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    quint32 * v2,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte * v0,
                    byte * v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    byte * v5,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt16 * v0,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt32 * v0,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    f32 * v0,
                    bool bQuery = true );

        //! 1 para
        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    byte * v2,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    int8 * v2,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    int32 * v2,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    quint32 * v2,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte * v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    byte * v5,
                    bool bQuery = true
                    );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    UInt16 * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    UInt32 * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    Int16 * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    Int32 * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    f32 * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    UInt16 v0,
                    f32 * v1,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    uint16 * v1,
                    uint16 * v2,
                    bool bQuery = true );

        //! 2 para
        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1,
                    byte * v2,
                    bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    bool bQuery = true
                    );

        int _read( DeviceId &id, byte mainCode,
                    byte subCode,
                    byte v0,
                    byte v1,
                    byte * v2,
                    byte * v3,
                    byte * v4,
                    byte * v5,
                    bool bQuery = true
                    );

        int _read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   sbyte * v2,
                   bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   Int16 * v2,
                   bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   UInt16 * v2,
                   bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   UInt32 * v2,
                   bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   f32 * v2,
                   bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   UInt16 v1,
                   UInt16 v2,
                   byte * v3,
                   bool bQuery = true );

        int _read( DeviceId &id, byte mainCode,
                   byte subCode,
                   byte v0,
                   byte v1,
                   UInt16 *v2,
                   UInt16 * v3,
                   bool bQuery = true );

};

}

#endif
