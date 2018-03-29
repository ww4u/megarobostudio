
#ifndef _IBUS_H_
#define _IBUS_H_

#include "..\..\include\datatype.h"
#include <QtCore>

#include "deviceid.h"

#include "../model/modelsyspref.h"

class receiveCache;

#define is_phy_bus( busType )       ( (busType) == MegaDevice::IBus::e_bus_can )

namespace MegaDevice {

class IBus
{
public:
    enum eBusType
    {
        e_bus_unk,
        e_bus_can,
        e_bus_file,
    };

protected:
    int mRdTmo;           //! us -- read timeout
    int mRdTick;          //! us -- cache read
    int mRdInterval;      //! us -- bus read

    int mEnumerateTmo;    //! us -- enumerate timeout

    int mWtInterval;      //! us
    int mFailTry;         //! time

    int mSpeed;           //! speed
    int mPId;             //! productId: 0 -- mega can
                          //! 1 -- usb can ii

    QString mName;

    eBusType mBusType;
                        //! receive cache
    receiveCache *m_pRecvCache;

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

    virtual int open(QString dev);
    virtual int open( int devType, int devId, int canId );
    virtual void close();

    virtual int size();
    virtual int flush( DeviceId &id );
    virtual int clear();

    //! write to bus
    virtual int doWrite( DeviceId &id, byte *pBuf, int len );
    //! read from bus
    virtual int doReceive( int *pFrameId, byte *pBuf, int *pLen );

    virtual int doRead( DeviceId &id, byte *pBuf, int cap, int *pLen );

    virtual int doFrameRead( DeviceId &id, int *pFrameId, byte *pBuf, int *pLen );
    virtual int doFrameRead( DeviceId &id, int *pFrameId, byte *pBuf, int eachFrameSize, int n );
    virtual int doSplitRead( DeviceId &id, int packOffset, byte *pBuf, int cap, int *pLen );

    virtual int enumerate( const modelSysPref &pref );

    void lock();
    void unlock();

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

    void attachReceiveCache( receiveCache *pCache );

	//! apis
	public:
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
                    byte * v1,
                    byte * v2,
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

        //! ******** sub apis ********
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
                    byte * v1,
                    byte * v2,
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

};

}

#endif
