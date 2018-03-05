#ifndef ROBONET_H
#define ROBONET_H

#include <QtCore>
#include "robomsg.h"

class RoboMsgThread;
class RoboMsgQueue
{
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
    void postMsg( eRoboMsg msg, int p1 );
    void postMsg( eRoboMsg msg, int p1, int p2 );
    void postMsg( eRoboMsg msg, int p1, int p2, int p3 );

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

    void process( int intervalus = 100,
                         RoboMsgThread *pThread = NULL );

public:
    QQueue< RoboMsg > mQueue;
    QMutex mQueueMutex;
    QSemaphore mSemaphore;

    RoboMsgThread *m_pMsgThread;

};

#endif // ROBONET_H
