
#include "rawrobo.h"

static msg_type _msg_patterns[]={

    //! member_state
    { e_robot_member_state,
      { QMetaType::Int, TPV_REGEION_TYPE_ID, QMetaType::Int },   //! subax, region, ax
    },

    //! msg
    { MegaDevice::mrq_msg_run,
      { TPV_REGEION_TYPE_ID },  //! region
    },

    { MegaDevice::mrq_msg_stop,
      { TPV_REGEION_TYPE_ID },  //! region
    },

    { MegaDevice::mrq_msg_call,
      { TPV_REGEION_TYPE_ID },  //! region
    },
};

//! \note subax invalid
void RawRobo::onMsg( int subAxes, RoboMsg &msg )
{
    if ( !RoboMsg::checkMsg( msg, _msg_patterns, sizeof_array(_msg_patterns)) )
    {
        logDbg()<<msg.Msg()<<msg.size();
        return;
    }

    //! for some region
//    tpvRegion region = msg.at(0).value<tpvRegion>();
//    region.set( subAxes, region.page() );
//    tpvRegion region;
    tpvRegion region;
    if ( msg.getMsg() == e_robot_member_state )     //! subax, region, ax
    {
        int tabAx = msg.at(0).toInt();
        region = msg.at(1).value<tpvRegion>();
        Q_ASSERT( tabAx >= 0 && tabAx < axes() );
        region.setAx( 0 );
    }
    else
    {
        region = msg.at(0).value<tpvRegion>();
    }

    logDbg()<<region.axes()<<region.page();
    Q_ASSERT( region.page() >= 0 && region.page() < regions() );

    //! to some region
    fsm( region )->proc( msg.Msg(), msg );
}
