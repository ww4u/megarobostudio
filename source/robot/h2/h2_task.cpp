#include "h2.h"

H2ZeroArg::H2ZeroArg()
{
    mAx = 0;
    mZeroDist = 100;
    mZeroTime = 10;

    mZeroGapDist = 50;
    mZeroGapTime = 5;
}

H2Task::H2Task(QObject *pParent) : RoboTask( pParent )
{}

void H2Task::run()
{
    sysLog( __FUNCTION__, QString::number(__LINE__), "Enter" );

    if ( 0 != checkRequest( m_pReq ) )
    { return; }

    robotH2 *pH2;
    pH2 = (robotH2*)m_pReq->m_pRobo;

    int ret;
    ret = (pH2->*( m_pReq->m_pApi ))( m_pReq->m_pArg );

    gc();

    sysLog( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
}
