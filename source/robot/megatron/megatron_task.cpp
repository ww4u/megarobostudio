#include "megatron.h"

MegatronZeroArg::MegatronZeroArg()
{}

MegatronTask::MegatronTask(QObject *pParent) : RoboTask( pParent )
{}

void MegatronTask::procRequest(RoboTaskRequest *pReq)
{
    implement_proc_request( robotMegatron );
}
