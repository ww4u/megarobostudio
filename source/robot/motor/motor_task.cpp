#include "motor.h"


MotorArg::MotorArg()
{
    mAx = 0;

    mZeroDist = 100;
    mZeroTime = 10;
    mZeroEndV = 5;

    mZeroGapDist = 50;
    mZeroGapTime = 5;
}

MotorTask::MotorTask( QObject *pParent ) : RoboTask( pParent )
{}

void MotorTask::run()
{
    if ( 0 != checkRequest( m_pReq ) )
    { return; }

    robotMotor *pRobo;
    pRobo = (robotMotor*)m_pReq->m_pRobo;

    int ret;
    ret = (pRobo->*( m_pReq->m_pApi ))( m_pReq->m_pArg );
    if ( ret != 0 )
    {
        sysWarn( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
    }

    gc();
}


