#include "igus_delta.h"

IgusDeltaZeroArg::IgusDeltaZeroArg()
{}

IgusDeltaTask::IgusDeltaTask(QObject *pParent) : RoboTask( pParent )
{}

void IgusDeltaTask::procRequest( RoboTaskRequest *pReq )
{
    implement_proc_request( robotIgusDelta );
}

