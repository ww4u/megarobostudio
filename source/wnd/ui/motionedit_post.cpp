#include "motionedit.h"

//! start
int motionEdit::postStart( appMsg msg, void *pPara )
{
    return 0;
}
void motionEdit::beginStart( void *pPara )
{}
void motionEdit::endStart( int ret, void *pPara )
{
    if ( ret != 0 )
    { sysError( tr("start fail") ); }
    else
    { }
}
