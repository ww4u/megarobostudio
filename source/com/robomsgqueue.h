#ifndef ROBONET_H
#define ROBONET_H

#include <QtCore>
#include "robomsg.h"
#include "basetype.h"

class RoboMsgThread;
class RoboMsgQueue
{
public:
    static QSemaphore _msgSema;
    static int waitIdle( int tickms = 10, int tmo = 10000 );
public:
    RoboMsgQueue();

public:
    void lock();
    void unlock();

    void attachMsgThread( RoboMsgThread *pThread );

    void clear();

    bool filter( const RoboMsg & msg );

    void postMsg( const RoboMsg & msg, quint64 t );

    void postMsg( eRoboMsg msg, quint64 t = 0 );
    void postMsg( eRoboMsg msg, const tpvRegion &region, quint64 t = 0 );
    void postMsg( eRoboMsg msg, const tpvRegion &region, int p1, quint64 t = 0 );
    void postMsg( eRoboMsg msg, int subax, const tpvRegion &region, int p1, quint64 t = 0 );

    void postMsg( eRoboMsg msg, const RoboMsg &leafMsg, quint64 t = 0 );

    void postMsg( eRoboMsg msg, int p1, quint64 t = 0 );
    void postMsg( eRoboMsg msg, int p1, int p2, quint64 t = 0 );
    void postMsg( eRoboMsg msg, int p1, int p2, int p3, quint64 t = 0 );


    void postMsg( eRoboMsg msg,
                         const QString &name,
                         const tpvRegion &region,
                         quint64 t = 0
                         );

    void postMsg( eRoboMsg msg,
                         const QString &name,
                         const tpvRegion &region,
                         int p1,
                         quint64 t = 0
                         );

    void postMsg( eRoboMsg msg,
                         const QString &name,
                         const tpvRegion &region,
                         int p1,
                         int p2,
                         quint64 t = 0
                         );

    void postMsg( eRoboMsg msg,
                         const QString &name,
                         int p1,
                         quint64 t = 0
                         );
    void postMsg( eRoboMsg msg,
                         const QString &name,
                         int p1,
                         int p2,
                         quint64 t = 0
                         );

    void postMsg( eRoboMsg msg,
                         const QString &name,
                         int p1,
                         int p2,
                         int p3,
                         quint64 t = 0
                         );

    void postMsg( eRoboMsg msg,
                         int eId,
                         int fId,       //! get name from device
                         const QByteArray &ary,
                         quint64 t = 0
                         );

    void postMsg( eRoboMsg msg,
                         int eId,
                         int devId,
                         int fId,       //! get name from device
                         const QByteArray &ary,
                         quint64 t = 0
                         );

    void postMsg( eRoboMsg msg,
                  int mi, int ma, int n,
                  const QString &str,
                  quint64 t = 0 );
    void postMsg( eRoboMsg msg,
                  bool b,
                  quint64 t = 0 );
    void postMsg( eRoboMsg msg,
                  const QString &str,
                  quint64 t = 0 );


    void process( int intervalus = 100,
                         RoboMsgThread *pThread = NULL );

public:
    QQueue< RoboMsg > mQueue;
    QMutex mQueueMutex;
    QSemaphore mSemaphore;

    RoboMsgThread *m_pMsgThread;

};

#endif // ROBONET_H
