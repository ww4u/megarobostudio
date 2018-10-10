#include "injectpump.h"

IJZeroArg::IJZeroArg()
{
    mAx = 0;
    mZeroDist = 100;
    mZeroTime = 10;

    mZeroVGapDist = 50;
    mZeroVGapTime = 5;

    mZeroIGapDist = 50;
    mZeroIGapTime = 5;
}

IJTask::IJTask(QObject *pParent) : RoboTask( pParent )
{}

void IJTask::run()
{
    if ( 0 != checkRequest( m_pReq ) )
    { return; }

    robotInject *pRobo;
    pRobo = (robotInject*)m_pReq->m_pRobo;

    int ret;
    ret = (pRobo->*( m_pReq->m_pApi ))( m_pReq->m_pArg );
    if ( ret != 0 )
    {
        sysWarn( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
    }
    gc();
}
