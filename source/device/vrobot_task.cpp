#include "vrobot.h"

//! argument
RoboTaskArgument::RoboTaskArgument()
{
    mTmo = time_s(20);
    mTick = time_ms(500);
}

RoboTaskArgument::~RoboTaskArgument()
{}

RoboTaskRequest::RoboTaskRequest()
{
    m_pRobo = NULL;
    m_pApi = NULL;
    m_pArg = NULL;
}

void RoboTaskRequest::request( VRobot *pRobo,
              VRobot::apiTaskRequest pApi,
              RoboTaskArgument *pArg )
{
    Q_ASSERT( NULL != pRobo );
    Q_ASSERT( NULL != pApi );
    Q_ASSERT( NULL != pArg );

    m_pRobo = pRobo;
    m_pApi = pApi;
    m_pArg = pArg;
}

RoboTaskRequest::~RoboTaskRequest()
{
    if ( NULL != m_pArg )       //! only delete the argment
    { delete m_pArg; }
}

QList<RoboTask*> RoboTask::_roboTasks;

void RoboTask::killAll()
{
    QList<bool> waitList;
    foreach( RoboTask *pTask, RoboTask::_roboTasks )
    {
        Q_ASSERT( NULL != pTask );
        if ( pTask->isRunning() )
        {
            pTask->requestInterruption();
            waitList.append( true );
        }
        else
        {
            waitList.append( false );
        }
    }

    for ( int i = 0; i < RoboTask::_roboTasks.size(); i++ )
    {
        if ( waitList.at(i) && RoboTask::_roboTasks.at(i)->isRunning() )
        {
            Q_ASSERT( RoboTask::_roboTasks.at(i) != NULL );
            RoboTask::_roboTasks[i]->wait();
        }
    }
}

RoboTask::RoboTask( QObject *pParent ) : QThread( pParent )
{
    m_pReq = NULL;

    RoboTask::_roboTasks.append( this );
}

RoboTask::~RoboTask()
{
    RoboTask::_roboTasks.removeAll( this );
}

void RoboTask::run()
{}

void RoboTask::setRequest( RoboTaskRequest * pReq)
{
    Q_ASSERT( NULL != pReq );
    m_pReq = pReq;
}

int RoboTask::checkRequest( const RoboTaskRequest *pReq )
{
    Q_ASSERT( NULL != pReq );
    Q_ASSERT( NULL != pReq->m_pRobo );
    Q_ASSERT( NULL != pReq->m_pApi );
    Q_ASSERT( NULL != pReq->m_pArg );

    return 0;
}

void RoboTask::gc()
{
    if ( NULL != m_pReq )
    {
        delete m_pReq;
        m_pReq =NULL;
    }
}

RoboService::RoboService(QObject *pObj) : QThread( pObj )
{}

void RoboService::run()
{}
