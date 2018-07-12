
#include "devicemrv.h"
#include "mrvaxes.h"

#define mrv_timer_id   10
#define mrv_timeout time_s( 1 )

namespace MegaDevice {

MrvAxes::MrvAxes()
{
    m_pMrv = NULL;

    mAxesId = 0;
}

void MrvAxes::proc( RoboMsg &msg )
{logDbg()<<(int)msg.mMsg;
    if ( msg.mMsg == (eRoboMsg)mrv_msg_idle )
    { killTimer( mrv_timer_id ); }
    else if ( msg.mMsg == (eRoboMsg)mrv_msg_end )
    { killTimer( mrv_timer_id ); }
    else if ( msg.mMsg == (eRoboMsg)mrv_msg_running )
    {  }

    //! op
    else if ( msg.mMsg == (eRoboMsg)mrv_msg_run )
    { startTimer( mrv_timer_id, mrv_timeout ); }
    else if ( msg.mMsg == (eRoboMsg)mrv_msg_stop )
    { startTimer( mrv_timer_id, mrv_timeout ); }
    else if ( msg.mMsg == (eRoboMsg)mrv_msg_reset )
    { startTimer( mrv_timer_id, mrv_timeout ); }

    else if ( msg.mMsg == e_robot_timeout )
    {
        Q_ASSERT( NULL != m_pMrv );
        m_pMrv->requestMotionState( mAxesId, MRV_MOTION_SWITCH_1_MAIN );
    }
    else
    {}
}

void MrvAxes::setParent( deviceMRV *pMrv )
{
    Q_ASSERT( NULL != pMrv );
    m_pMrv = pMrv;
}
deviceMRV *MrvAxes::parent()
{ return m_pMrv; }

void MrvAxes::setAxesId( int id )
{ mAxesId = id; }
int MrvAxes::axesId()
{ return mAxesId; }


void MrvAxes::startTimer( int id, int tmous )
{
    Q_ASSERT( NULL != m_pMrv );

    m_pMrv->startTimer( this, id, tmous );
}

void MrvAxes::killTimer( int id )
{
    Q_ASSERT( NULL != m_pMrv );

    m_pMrv->killTimer( this, id );
}

}
