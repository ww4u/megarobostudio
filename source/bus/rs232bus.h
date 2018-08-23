#ifndef RS232BUS_H
#define RS232BUS_H

#include "visa.h"

#include <QtCore>
typedef QList<quint8> U8List;

#include "ibus.h"
#include "canbus.h"

namespace MegaDevice {

//class Rs232Bus : public IBus
class Rs232Bus : public CANBus
{
public:
    Rs232Bus();
    virtual ~Rs232Bus();

public:
    virtual int open( const modelSysPref &pref,
                      int devType = 6,
                      int devId=0, int seqId = 0, int canId=0,
                      const QString &desc="" );

    int open_( const modelSysPref &pref,
               QString str );
    void close();

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

//    virtual int enumerate( const modelSysPref &pref );

protected:
    virtual int rawEnumerate( const modelSysPref &pref );

public:
    //! no para
    int write( quint8 m, quint8 c );

    //! 1 para
    int write( quint8 m, quint8 c, quint8 v0 );
    int write( quint8 m, quint8 c, quint16 v0 );
    int write( quint8 m, quint8 c, quint32 v0 );

    int write( quint8 m, quint8 c, qint8 v0 );
    int write( quint8 m, quint8 c, qint16 v0 );
    int write( quint8 m, quint8 c, qint32 v0 );

    //! 2 para
    int write( quint8 m, quint8 c, quint8 v0, quint8 v1 );
    int write( quint8 m, quint8 c, quint8 v0, quint16 v1 );
    int write( quint8 m, quint8 c, quint8 v0, quint32 v1 );

    int write( quint8 m, quint8 c, quint8 v0, qint8 v1 );
    int write( quint8 m, quint8 c, quint8 v0, qint16 v1 );
    int write( quint8 m, quint8 c, quint8 v0, qint32 v1 );

    //! 3 para
    int write( quint8 m, quint8 c, quint8 v0, quint8 v1, quint8 v2 );
    int write( quint8 m, quint8 c, quint8 v0, quint8 v1, quint32 v2 );
    int write( quint8 m, quint8 c, quint8 v0, quint8 v1, float v2 );

    int write( quint8 m, quint8 c, quint8 v0, quint16 v1, quint16 v2 );
    int write( quint8 m, quint8 c, quint8 v0, quint32 v1, quint32 v2 );

    int write( quint8 m, quint8 c, quint8 v0, qint8 v1, qint8 v2 );
    int write( quint8 m, quint8 c, quint8 v0, qint16 v1, qint16 v2 );
    int write( quint8 m, quint8 c, quint8 v0, qint32 v1, qint32 v2 );

    //! read
    int read( U8List &rawList );
protected:
    virtual void encode( U8List &rawData, U8List &outAry );
    virtual void decode( U8List &frameData, U8List &rawData );

    //! high api
    int write( U8List &rawList );
    int write( QByteArray &rawList );

    int send( U8List &sendList );
    int recv( U8List &readList, int cnt );

    quint8 xored( U8List &data );

protected:
    ViSession mViDevice;
    ViSession mViDef;
};

}

#endif // RS232BUS_H
