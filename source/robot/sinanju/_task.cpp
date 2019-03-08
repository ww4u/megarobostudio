#include "sinanjutask.h"

#include "sinanju.h"

SinanjuTask::SinanjuTask( QObject *pParent ) : RoboTask( pParent )
{
}

//void SinanjuTask::run()
//{
//    if ( 0 != checkRequest( m_pReq ) )
//    { return; }

//    robotSinanju *pRobo;
//    pRobo = (robotSinanju*)m_pReq->m_pRobo;

//    int ret;
//    ret = (pRobo->*( m_pReq->m_pApi ))( m_pReq->m_pArg );
//    if ( ret != 0 )
//    {
//        sysLog( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
//    }

//    gc();
//}

void SinanjuTask::procRequest( RoboTaskRequest *pRequest )
{logDbg();
    //! convert robo
    robotSinanju *pRobo;
    pRobo = (robotSinanju*)pRequest->m_pRobo;

    //! proc
    int ret;
    ret = (pRobo->*( pRequest->m_pApi ))( pRequest->m_pArg );
    if ( ret != 0 )
    {
        sysLog( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
    }
}
