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
bool VRobot::checkLink( int p1, int p2 )
{ return false; }

void VRobot::event_motionStatus( frameData &data )
{
    if ( data.size() < 5 )
    { return; }

    //! deload data mc,sc,ch,page,state
    char ch   = data.at(2);
    char page = data.at(3);
    char stat = data.at(4);

    setStatus( stat, tpvRegion(ch, page), data );
}

void VRobot::event_motionStatus( frameData &data, VRobot *pRobot )
{

}

