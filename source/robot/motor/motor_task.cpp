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

void MotorTask::procRequest( RoboTaskRequest *pReq )
{
    implement_proc_request( robotMotor );
}


