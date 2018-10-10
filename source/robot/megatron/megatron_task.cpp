#include "megatron.h"

MegatronZeroArg::MegatronZeroArg()
{}

MegatronTask::MegatronTask(QObject *pParent) : RoboTask( pParent )
{}

void MegatronTask::run()
{
    if ( 0 != checkRequest( m_pReq ) )
    { return; }

    robotMegatron *pRobo;
    pRobo = (robotMegatron*)m_pReq->m_pRobo;

    int ret;
    ret = (pRobo->*( m_pReq->m_pApi ))( m_pReq->m_pArg );
    if( ret != 0 )
    {
        sysWarn( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
    }

    gc();
}
