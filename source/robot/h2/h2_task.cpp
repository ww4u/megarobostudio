#include "h2.h"

H2ZeroArg::H2ZeroArg()
{
    mAx = 0;

    mZeroDist = 100;
    mZeroTime = 10;
    mZeroEndV = 5;

    mZeroGapDist = 50;
    mZeroGapTime = 5;

    //! z special
    mZeroZDist = 100;
    mZeroZTime = 10;
    mZeroZEndV = 5;

    mZeroGapZDist = 50;
    mZeroGapZTime = 5;
}

H2Task::H2Task(QObject *pParent) : RoboTask( pParent )
{}

void H2Task::procRequest(RoboTaskRequest*pReq)
{
    implement_proc_request( robotH2 );
}
