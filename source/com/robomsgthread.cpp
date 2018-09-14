#include "robomsgthread.h"
#include "../../include/mcstd.h"
RoboMsgThread::RoboMsgThread( QObject *pObj ) : QThread( pObj )
{
    mIntervalus = 100000;   //! 100ms
}

void RoboMsgThread::onMsg( RoboMsg &msg )
{}

void RoboMsgThread::run()
{
    Q_FOREVER
    {
        if ( isInterruptionRequested() )
        { break; }

        process( mIntervalus, this );
    }
}

void RoboMsgThread::_setInterval( int us )
{ mIntervalus = us;  }
int RoboMsgThread::interval()
{ return mIntervalus; }
