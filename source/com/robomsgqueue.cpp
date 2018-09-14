#include "robomsgqueue.h"
#include "robomsgthread.h"
#include "../../include/mcdef.h"
//! vars

#define tick_ms( ms )   (ms),
#define tick_s( s )     ((s)*tick_ms(1000))

//! ticks
static RoboMsgTick _robo_msg_ticks[]=
{
    { e_download_processing, tick_ms(1000) },
};

QSemaphore RoboMsgQueue::_msgSema;

int RoboMsgQueue::waitIdle( int tickms, int tmo )
{
    while( _msgSema.available() > 0 && tmo > 0 )
    {
        tmo -= tickms;
        QThread::msleep( tickms );
    }

    //! tmo
    if ( tmo < 0 )
    { return -1; }
    else
    { return 0; }
}

RoboMsgQueue::RoboMsgQueue()
{
    m_pMsgThread = NULL;
}

void RoboMsgQueue::attachMsgThread( RoboMsgThread *pThread )
{
    Q_ASSERT( NULL != pThread );

    m_pMsgThread = pThread;
}

void RoboMsgQueue::clear()
{
    lock();

    mQueue.clear();

    unlock();
}

void RoboMsgQueue::lock()
{
    mQueueMutex.lock();
}
void RoboMsgQueue::unlock()
{
    mQueueMutex.unlock();
}

//! called in mutex lock
bool RoboMsgQueue::filter( const RoboMsg & msg )
{
    qint64 tick;

    //! find tick
    tick = 0;
    for( int i = 0; i < sizeof_array(_robo_msg_ticks); i++ )
    {
        if ( msg.mMsg == _robo_msg_ticks[i].msg )
        {
            tick = _robo_msg_ticks[i].tick;
            break;
        }
    }

    //! no tick
    if ( tick <= 0 )
    { return false; }

    //! no time
    if ( msg.timeStamp() == 0 )
    { return false; }

    //! find the queue
    for ( int i = mQueue.size() - 1; i >= 0 ; i-- )
    {
        if ( mQueue[i].mMsg == msg.mMsg )
        {
            //! long time
            if ( ( msg - mQueue[i] ) > tick )
            { return false; }
            else
            { return true; }
        }
    }

    return false;
}

void RoboMsgQueue::postMsg( const RoboMsg &msg, quint64 t )
{

    if ( filter(msg) )
    {}
    else
    {
        RoboMsg lMsg = msg;
        lMsg.setTimeStamp( t );

        lock();
        mQueue.enqueue( lMsg );
        unlock();

        mSemaphore.release();
        _msgSema.release();     //! total sema

        //! thread not running
        if ( NULL != m_pMsgThread && !m_pMsgThread->isRunning() )
        {
            m_pMsgThread->start();
        }
    }

}

void RoboMsgQueue::postMsg( eRoboMsg msg, quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg, const tpvRegion &region, quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant::fromValue(region) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg, const tpvRegion &region, int p1, quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant::fromValue(region) );
    lMsg.append( p1 );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                            int subax,
                            const tpvRegion &region,
                            int p1,
                            quint64 t)
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( subax );
    lMsg.append( QVariant::fromValue(region) );
    lMsg.append( p1 );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg, const RoboMsg &leafMsg, quint64 t  )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant::fromValue(leafMsg) );

    postMsg( lMsg, t );
}


void RoboMsgQueue::postMsg( eRoboMsg msg, int p1, quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(p1) );

    postMsg( lMsg, t );
}
void RoboMsgQueue::postMsg( eRoboMsg msg, int p1, int p2, quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );

    postMsg( lMsg, t );
}
void RoboMsgQueue::postMsg( eRoboMsg msg, int p1, int p2, int p3, quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );
    lMsg.append( QVariant(p3) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     const tpvRegion &region,
                     quint64 t
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant::fromValue(region) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     const tpvRegion &region,
                     int p1,
                     quint64 t
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant::fromValue(region) );
    lMsg.append( QVariant(p1) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     const tpvRegion &region,
                     int p1,
                     int p2,
                     quint64 t
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant::fromValue(region) );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     int p1,
                     quint64 t
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant(p1) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     int p1,
                     int p2,
                     quint64 t
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     int p1,
                     int p2,
                     int p3,
                     quint64 t
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );
    lMsg.append( QVariant(p3) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     int eId,
                     int fId,
                     const QByteArray &ary,
                     quint64 t
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(eId) );
    lMsg.append( QVariant(fId) );
    lMsg.append( QVariant(ary) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     int eId,
                     int devId,
                     int fId,
                     const QByteArray &ary,
                     quint64 t
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(eId) );
    lMsg.append( QVariant(devId) );
    lMsg.append( QVariant(fId) );
    lMsg.append( QVariant(ary) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
              int mi, int ma, int n,
              const QString &str,
              quint64 t        )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(mi) );
    lMsg.append( QVariant(ma) );
    lMsg.append( QVariant(n) );
    lMsg.append( QVariant(str) );

    postMsg( lMsg, t );
}
void RoboMsgQueue::postMsg( eRoboMsg msg,
              bool b,
              quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(b) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                            const QString &str,
                            quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(str) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                            const RpcRequest &rpc,
                            quint64 t )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant::fromValue(rpc) );

    postMsg( lMsg, t );
}

void RoboMsgQueue::process( int intervalus,
                       RoboMsgThread *pThread )
{
    //! wait the writer
//    mSemaphore.acquire();

    RoboMsg lMsg;
    //! loop do
//    while( mQueue.size() > 0 )
//    while( 1 )
    {   
        if( mSemaphore.tryAcquire(1, intervalus/1000 ) )
        {}
        else
        { return; }

        _msgSema.acquire();

        lock();

        lMsg = mQueue.dequeue();

        unlock();

        //! no delay for speed
//        QThread::usleep( intervalus );

        if ( NULL != pThread )
        { pThread->onMsg( lMsg ); }

    }
}
