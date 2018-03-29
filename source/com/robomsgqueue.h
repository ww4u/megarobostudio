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

    void postMsg( const RoboMsg & msg );

    void postMsg( eRoboMsg msg );
    void postMsg( eRoboMsg msg, const tpvRegion &region );
    void postMsg( eRoboMsg msg, const tpvRegion &region, int p1 );
    void postMsg( eRoboMsg msg, int subax, const tpvRegion &region, int p1 );

    void postMsg( eRoboMsg msg, const RoboMsg &leafMsg );

    void postMsg( eRoboMsg msg, int p1 );
    void postMsg( eRoboMsg msg, int p1, int p2 );
    void postMsg( eRoboMsg msg, int p1, int p2, int p3 );


    void postMsg( eRoboMsg msg,
                         const QString &name,
                         const tpvRegion &region
                         );

    void postMsg( eRoboMsg msg,
                         const QString &name,
                         const tpvRegion &region,
                         int p1
                         );

    void postMsg( eRoboMsg msg,
                         const QString &name,
                         const tpvRegion &region,
                         int p1,
                         int p2
                         );

    void postMsg( eRoboMsg msg,
                         const QString &name,
                         int p1
                         );
    void postMsg( eRoboMsg msg,
                         const QString &name,
                         int p1,
                         int p2
                         );

    void postMsg( eRoboMsg msg,
                         const QString &name,
                         int p1,
                         int p2,
                         int p3
                         );

    void postMsg( eRoboMsg msg,
                         int eId,
                         int fId,       //! get name from device
                         const QByteArray &ary
                         );

    void postMsg( eRoboMsg msg,
                  int mi, int ma, int n,
                  const QString &str );
    void postMsg( eRoboMsg msg,
                  bool b );
    void postMsg( eRoboMsg msg,
                  const QString &str );


    void process( int intervalus = 100,
                         RoboMsgThread *pThread = NULL );

public:
    QQueue< RoboMsg > mQueue;
    QMutex mQueueMutex;
    QSemaphore mSemaphore;

    RoboMsgThread *m_pMsgThread;

};

#endif // ROBONET_H
