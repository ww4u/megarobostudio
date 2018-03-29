#include "roboworker.h"
#include "vrobot.h"

RoboWorker::RoboWorker( VRobot *pRobo, QObject *pObj ) : RoboMsgThread( pObj )
{
    m_pRobot = pRobo;
    mSubAxes = -1;                  //! no axes

    attachMsgThread( this );        //! msg process thread
}

void RoboWorker::onMsg( RoboMsg &msg )
{
    if ( NULL != m_pRobot )
    {
        m_pRobot->onMsg( mSubAxes, msg );
    }
}

void RoboWorker::attachRobot( VRobot *pRobot,
                              int subAxes )
{
    Q_ASSERT( NULL != pRobot );

    m_pRobot = pRobot;
    mSubAxes = subAxes;
}

void RoboWorker::run()
{
    RoboMsgThread::run();
}
