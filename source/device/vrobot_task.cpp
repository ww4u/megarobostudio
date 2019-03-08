#include "vrobot.h"

//! argument
RoboTaskArgument::RoboTaskArgument()
{
    mTmo = time_s(20);
    mTick = time_ms(500);

    m_pVarList = NULL;
}

RoboTaskArgument::~RoboTaskArgument()
{
    if ( NULL != m_pVarList )
    { delete m_pVarList; }
}

void RoboTaskArgument::attachVars( QVariantList *pVars )
{
    Q_ASSERT( NULL != pVars );

    m_pVarList = pVars;
}

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
    RoboTask::_roboTasks.append( this );
}

RoboTask::~RoboTask()
{
    RoboTask::_roboTasks.removeAll( this );
}

void RoboTask::run()
{
    RoboTaskRequest* pRequest;
    while( !mRequests.isEmpty() )
    {logDbg();
        mMutex.lock();
            pRequest = mRequests.dequeue();
        mMutex.unlock();

        //! check req
        if( checkRequest( pRequest ) )
        {
            //! do it
            procRequest( pRequest );
        }

        //! gc it
        gcRequest( pRequest );logDbg();
    }
}

void RoboTask::procRequest( RoboTaskRequest *pRequest )
{}

void RoboTask::attachRequest( RoboTaskRequest *pRequest )
{
    Q_ASSERT( NULL != pRequest );

    mMutex.lock();
        mRequests.enqueue( pRequest );
    mMutex.unlock();

    //! start again
    start();
}

bool RoboTask::checkRequest( const RoboTaskRequest *pReq )
{
    if( NULL == pReq ){ return false; }
    if( NULL == pReq->m_pRobo ){ return false; }
    if( NULL == pReq->m_pApi ){ return false; }
    if( NULL == pReq->m_pArg ){ return false; }

    return true;
}

void RoboTask::gcRequest( RoboTaskRequest *pReq )
{
    if ( NULL != pReq )
    {
        //! arg
        if ( NULL != pReq->m_pArg )
        { delete pReq->m_pArg; }

        //! request
        delete pReq;
    }
}

RoboService::RoboService(QObject *pObj) : QThread( pObj )
{}

void RoboService::run()
{}
