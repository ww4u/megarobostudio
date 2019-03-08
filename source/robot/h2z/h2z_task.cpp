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

void H2ZTask::procRequest( RoboTaskRequest *pReq )
{
    implement_proc_request( robotH2Z );
}
