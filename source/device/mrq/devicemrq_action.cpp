
#include "deviceMRQ.h"

namespace MegaDevice
{

int deviceMRQ::run(int axes)
{
    //! start lpc
    lpc( axes )->postMsg(
                            (eRoboMsg)mrq_msg_run,
                            axes
                        );
    return 0;
}

int deviceMRQ::stop(int axes)
{
    lpc( axes )->postMsg(
                            (eRoboMsg)mrq_msg_stop,
                            axes
                        );

    return 0;
}

//MRQ_MOTION_STATE_POWERON,
//MRQ_MOTION_STATE_IDLE,
//MRQ_MOTION_STATE_CALCING,
//MRQ_MOTION_STATE_CALCEND,
//MRQ_MOTION_STATE_STANDBY,
//MRQ_MOTION_STATE_RUNNING,
//MRQ_MOTION_STATE_ERROR,
void deviceMRQ::setStatus( int stat, int ch )
{
    MRQ::setStatus( stat, ch );
logDbg()<<stat<<ch;
    if ( stat == MRQ_MOTION_STATE_POWERON )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_IDLE )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_CALCING )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_calcing, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_CALCEND )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_calcend, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_STANDBY )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_standby, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_RUNNING )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_running, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_ERROR )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, ch ) ;
    }
    else
    { Q_ASSERT(false); }
}

}
