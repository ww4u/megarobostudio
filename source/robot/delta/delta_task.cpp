#include "delta.h"

DeltaZeroArg::DeltaZeroArg()
{}

DeltaTask::DeltaTask(QObject *pParent) : RoboTask( pParent )
{}

void DeltaTask::run()
{
    sysLog( __FUNCTION__, QString::number(__LINE__), "Enter" );

    if ( 0 != checkRequest( m_pReq ) )
    { return; }

    robotDelta *pRobo;
    pRobo = (robotDelta*)m_pReq->m_pRobo;

    int ret;
    ret = (pRobo->*( m_pReq->m_pApi ))( m_pReq->m_pArg );

    gc();

    sysLog( __FUNCTION__,
            QString::number(__LINE__),
            QString::number( ret ),
            "exit" );
}

