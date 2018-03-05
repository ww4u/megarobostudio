#include "robomsgthread.h"
#include "../../include/mcstd.h"
RoboMsgThread::RoboMsgThread( QObject *pObj ) : QThread( pObj )
{
    mIntervalus = 100000;   //! 10ms
}

void RoboMsgThread::onMsg( RoboMsg &msg )
{}

void RoboMsgThread::run()
{
//    Q_FOREVER
    {
        process( mIntervalus, this );
    }
}

void RoboMsgThread::setInterval( int us )
{ mIntervalus = us;  }
int RoboMsgThread::interval()
{ return mIntervalus; }
