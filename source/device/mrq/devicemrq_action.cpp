
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
void deviceMRQ::setStatus( int stat, int ch, int page  )
{
    MRQ::setStatus( stat, ch, page );
logDbg()<<stat<<ch<<page;
    if ( stat == MRQ_MOTION_STATE_2_POWERON )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_IDLE )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_CALCING )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_calcing, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_CALCEND )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_calcend, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_STANDBY )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_standby, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_RUNNING )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_running, ch ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_ERROR )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, ch ) ;
    }
    else
    { Q_ASSERT(false); }
}

}
