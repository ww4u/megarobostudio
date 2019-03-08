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

void IJTask::procRequest( RoboTaskRequest *pReq )
{
    implement_proc_request( robotInject );
}
