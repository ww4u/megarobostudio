#include "h2z.h"

H2ZZeroArg::H2ZZeroArg()
{
    mAx = 0;
    mZeroDist = 100;
    mZeroTime = 10;

    mZeroGapDist = 50;
    mZeroGapTime = 5;
}

H2ZTask::H2ZTask(QObject *pParent) : RoboTask( pParent )
{}

void H2ZTask::run()
{
    if ( 0 != checkRequest( m_pReq ) )
    { return; }

    robotH2Z *pH2;
    pH2 = (robotH2Z*)m_pReq->m_pRobo;

    int ret;
    ret = (pH2->*( m_pReq->m_pApi ))( m_pReq->m_pArg );
    if ( ret != 0 )
    {
        sysWarn( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
    }
    gc();
}
