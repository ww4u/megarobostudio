#include "sinanjutask.h"

#include "sinanju.h"

SinanjuTask::SinanjuTask( QObject *pParent ) : RoboTask( pParent )
{
}

void SinanjuTask::run()
{
    sysLog( __FUNCTION__, QString::number(__LINE__), "Enter" );

    if ( 0 != checkRequest( m_pReq ) )
    { return; }

    robotSinanju *pRobo;
    pRobo = (robotSinanju*)m_pReq->m_pRobo;

    int ret;
    ret = (pRobo->*( m_pReq->m_pApi ))( m_pReq->m_pArg );

    gc();

    sysLog( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
}
