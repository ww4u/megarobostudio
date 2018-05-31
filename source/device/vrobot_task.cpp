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
//                void *pApi,
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

RoboTask::RoboTask( QObject *pParent ) : QThread( pParent )
{
    m_pReq = NULL;
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




