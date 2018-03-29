
#include "deviceMRQ.h"

namespace MegaDevice
{

int deviceMRQ::run( const tpvRegion &region )
{
    //! start lpc
    lpc( region.axes() )->postMsg(
                            (eRoboMsg)mrq_msg_run,
                            region
                        );
    return 0;
}

int deviceMRQ::stop( const tpvRegion &region )
{
    lpc( region.axes() )->postMsg(
                            (eRoboMsg)mrq_msg_stop,
                            region
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
void deviceMRQ::setStatus( int stat, const tpvRegion &region  )
{
    int ch = region.axes();
    MRQ::setStatus( stat, region );

    if ( stat == MRQ_MOTION_STATE_2_POWERON )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, region ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_IDLE )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, region ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_CALCING )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_calcing, region ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_CALCEND )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_calcend, region ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_STANDBY )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_standby, region ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_RUNNING )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_running, region ) ;
    }
    else if ( stat == MRQ_MOTION_STATE_2_ERROR )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrq_msg_idle, region ) ;
    }
    else
    { Q_ASSERT(false); }
}

}
