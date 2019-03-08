#include "delta.h"

DeltaZeroArg::DeltaZeroArg()
{}

DeltaTask::DeltaTask(QObject *pParent) : RoboTask( pParent )
{}

void DeltaTask::procRequest(RoboTaskRequest*pReq)
{
    implement_proc_request( robotDelta );
}

