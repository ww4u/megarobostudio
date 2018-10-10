#include "igus_delta.h"

IgusDeltaZeroArg::IgusDeltaZeroArg()
{}

IgusDeltaTask::IgusDeltaTask(QObject *pParent) : RoboTask( pParent )
{}

void IgusDeltaTask::run()
{
    if ( 0 != checkRequest( m_pReq ) )
    { return; }

    robotIgusDelta *pRobo;
    pRobo = (robotIgusDelta*)m_pReq->m_pRobo;

    int ret;
    ret = (pRobo->*( m_pReq->m_pApi ))( m_pReq->m_pArg );
    if ( ret != 0 )
    {
        sysWarn( __FUNCTION__,
                QString::number(__LINE__),
                QString::number( ret ),
                "exit" );
    }

    gc();
}

