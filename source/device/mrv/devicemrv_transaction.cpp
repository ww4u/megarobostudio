#include "devicemrv.h"

namespace MegaDevice {


//! msg patterns
static msg_type _msg_patterns[] =
{
    { e_robot_timeout, {QMetaType::Int, QMetaType::Int} },

    { mrv_msg_idle, {TPV_REGEION_TYPE_ID, QMetaType::Int} },
    { mrv_msg_end, {TPV_REGEION_TYPE_ID, QMetaType::Int} },
    { mrv_msg_running, {TPV_REGEION_TYPE_ID, QMetaType::Int} },

    { mrv_msg_run, {TPV_REGEION_TYPE_ID, QMetaType::Int} },
    { mrv_msg_stop, {TPV_REGEION_TYPE_ID, QMetaType::Int} },
    { mrv_msg_reset, {TPV_REGEION_TYPE_ID, QMetaType::Int} },

    { mrv_msg_completed, {TPV_REGEION_TYPE_ID, QMetaType::Int} },

};

void deviceMRV::onMsg( int subAxes, RoboMsg &msg )
{
    Q_ASSERT( subAxes >= 0 && subAxes < axes() );

    //! type match
    if ( !RoboMsg::checkMsg( msg, _msg_patterns, sizeof_array(_msg_patterns)) )
    {
        logDbg()<<(int)msg.at(0).type()<<msg.getMsg()<<msg.size();
        return;
    }

    //! only the set msg
    mAxesList.at( subAxes )->proc( msg );

    logDbg()<<subAxes;
}

void deviceMRV::onTimer( void *pContext, int id )
{
    Q_ASSERT( NULL != pContext );

    //! context is axes
    MrvAxes *pAxes = (MrvAxes*)pContext;
    lpc( pAxes->axesId() )->postMsg( e_robot_timeout,
                                     pAxes->axesId(),
                                     id );
}

//MRV_MOTION_STATE_1_reserve,
//MRV_MOTION_STATE_1_IDLE,
//MRV_MOTION_STATE_1_reserve1,
//MRV_MOTION_STATE_1_CALCEND,
//MRV_MOTION_STATE_1_reserve2,
//MRV_MOTION_STATE_1_RUNNING,
//MRV_MOTION_STATE_1_reserve3,
void deviceMRV::setStatus( int stat, const tpvRegion &region, frameData &data  )
{
    int ch = region.axes();
    MRV::setStatus( stat, region, data );

    //sysLog( __FUNCTION__, QString::number( __LINE__), QString::number( ch ), QString::number( stat ) );
    if ( stat == MRV_MOTION_STATE_1_IDLE )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrv_msg_idle, region, data.timeStamp() ) ;
    }
    else if ( stat == MRV_MOTION_STATE_1_CALCEND )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrv_msg_end, region, data.timeStamp() ) ;
    }
    else if ( stat == MRV_MOTION_STATE_1_RUNNING )
    {
        lpc( ch )->postMsg( (eRoboMsg)mrv_msg_running, region, data.timeStamp() ) ;
    }
    else
    {
        logDbg()<<stat;
        Q_ASSERT(false);
    }
}

int deviceMRV::switchReset( int ax )
{
    int ret;

    assignStatus( MRV_MOTION_STATE_1_IDLE, tpvRegion(ax,0) );

    ret = setMOTION_SWITCH( ax, MRV_MOTION_SWITCH_RESET, MRV_MOTION_SWITCH_1_MAIN );

    lpc( ax )->postMsg( (eRoboMsg)mrv_msg_reset, tpvRegion(ax) );

    return ret;
}
int deviceMRV::switchRun( int ax )
{
    int ret;

    assignStatus( MRV_MOTION_STATE_1_IDLE, tpvRegion(ax,0) );

    ret = setMOTION_SWITCH( ax, MRV_MOTION_SWITCH_RUN, MRV_MOTION_SWITCH_1_MAIN );

    lpc( ax )->postMsg( (eRoboMsg)mrv_msg_run, tpvRegion(ax) );

    return ret;
}
int deviceMRV::switchStop( int ax )
{
    int ret;

    assignStatus( MRV_MOTION_STATE_1_IDLE, tpvRegion(ax,0) );

    ret = setMOTION_SWITCH( ax, MRV_MOTION_SWITCH_STOP, MRV_MOTION_SWITCH_1_MAIN );

    lpc( ax )->postMsg( (eRoboMsg)mrv_msg_stop, tpvRegion(ax) );

    return ret;
}

void deviceMRV::preSet( int ax )
{
    setMOTION_SWITCH( ax, MRV_MOTION_SWITCH_STOP, MRV_MOTION_SWITCH_1_MAIN );
    setMOTION_SWITCH( ax, MRV_MOTION_SWITCH_RESET, MRV_MOTION_SWITCH_1_MAIN );
    setPVT_RESET( ax );

    requestMotionState( ax, MRV_MOTION_SWITCH_1_MAIN );
}

void deviceMRV::postSet( int ax )
{}

int deviceMRV::setVALVECTRL_DEVICE( byte val0, MRV_VALVECTRL_DEVICE val1 )
{
    preSet( val0 );

    return MRV::setVALVECTRL_DEVICE( val0, val1 );
}

}
