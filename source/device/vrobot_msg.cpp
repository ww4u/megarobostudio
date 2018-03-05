#include "vrobot.h"

void VRobot::onMsg( int subAxes, RoboMsg &msg )
{
    logDbg();
}

void VRobot::interruptHandler( eventId eId,
                               frameData &data )
{
    if ( eId == event_status )
    {
        event_motionStatus( data );
    }
}

void VRobot::interruptHandler( eventId eId,
                               frameData &data,
                               VRobot *pRobot
                               )
{
    if ( eId == event_status )
    {
        event_motionStatus( data, pRobot );
    }
}

void VRobot::onLine()
{}
void VRobot::offLine()
{}

void VRobot::event_motionStatus( frameData &data )
{
    if ( data.size() < 4 )
    { return; }

    //! deload data
    char ch = data.at( 2 );
    char stat = data.at( 3 );

    setStatus( stat, ch );
}

void VRobot::event_motionStatus( frameData &data, VRobot *pRobot )
{

}

