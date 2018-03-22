#include "robomsgqueue.h"
#include "robomsgthread.h"

//! vars

#define tick_ms( ms )   (ms),
#define tick_s( s )     ((s)*tick_ms(1000))

//! ticks
static RoboMsgTick _robo_msg_ticks[]=
{
    { e_download_processing, tick_ms(500) },
};

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
    for( int i = 0; i < sizeof(_robo_msg_ticks)/sizeof(_robo_msg_ticks[0]); i++ )
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

void RoboMsgQueue::postMsg( const RoboMsg &msg )
{
    mQueueMutex.lock();

    if ( filter(msg) )
    {}
    else
    {
        mQueue.enqueue( msg );

        //! thread not running
        if ( NULL != m_pMsgThread && !m_pMsgThread->isRunning() )
        {
            m_pMsgThread->start();
        }
    }

    mQueueMutex.unlock();
}

void RoboMsgQueue::postMsg( eRoboMsg msg )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );

    postMsg( lMsg );
}

void RoboMsgQueue::postMsg( eRoboMsg msg, int p1 )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(p1) );

    postMsg( lMsg );
}
void RoboMsgQueue::postMsg( eRoboMsg msg, int p1, int p2 )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );

    postMsg( lMsg );
}
void RoboMsgQueue::postMsg( eRoboMsg msg, int p1, int p2, int p3 )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );
    lMsg.append( QVariant(p3) );

    postMsg( lMsg );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     int p1
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant(p1) );

    postMsg( lMsg );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     int p1,
                     int p2
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );

    postMsg( lMsg );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     const QString &name,
                     int p1,
                     int p2,
                     int p3
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(name) );
    lMsg.append( QVariant(p1) );
    lMsg.append( QVariant(p2) );
    lMsg.append( QVariant(p3) );

    postMsg( lMsg );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
                     int eId,
                     int fId,
                     const QByteArray &ary
                     )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(eId) );
    lMsg.append( QVariant(fId) );
    lMsg.append( QVariant(ary) );

    postMsg( lMsg );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
              int mi, int ma, int n,
              const QString &str )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(mi) );
    lMsg.append( QVariant(ma) );
    lMsg.append( QVariant(n) );
    lMsg.append( QVariant(str) );

    postMsg( lMsg );
}
void RoboMsgQueue::postMsg( eRoboMsg msg,
              bool b )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(b) );

    postMsg( lMsg );
}

void RoboMsgQueue::postMsg( eRoboMsg msg,
              const QString &str )
{
    RoboMsg lMsg;

    lMsg.setMsg( msg );
    lMsg.append( QVariant(str) );

    postMsg( lMsg );
}

void RoboMsgQueue::process( int intervalus,
                       RoboMsgThread *pThread )
{
    //! wait the writer
//    mSemaphore.acquire();

    RoboMsg lMsg;
    //! loop do
    while( mQueue.size() > 0 )
    {
        lock();

        lMsg = mQueue.dequeue();

        unlock();

        QThread::usleep( intervalus );

        if ( NULL != pThread )
        {
            pThread->onMsg( lMsg );
        }

//        QVariant var;
//        qDebug()<<lMsg.mMsg<<lMsg.size();
//        for( int i = 0; i < lMsg.size(); i++ )
//        {
//            var = lMsg[i];
//            qDebug()<<var.typeName();
//            if ( var.type() == QMetaType::Int )
//            { qDebug()<<var.toInt(); }
//        }
    }
}
