#include "vrobot.h"

QString & VRobot::getClass()
{ return mClass; }
VRobot::robotEnum VRobot::getId()
{ return mId; }
VRobot::robotEnum VRobot::robotId()
{ return mId; }

int VRobot::setAxes(int n)
{
    mAxes = n;

    //! gc
    gcWorker();

    //! create the workers
    m_pAxesWorkers = new RoboWorker[ n ];
    Q_ASSERT( NULL != m_pAxesWorkers );
    for( int i = 0; i < n; i++ )
    {
        m_pAxesWorkers[i].attachRobot( this, i );
    }
}
int VRobot::getAxes()
{ return mAxes; }
int VRobot::axes()
{ return mAxes; }
QImage & VRobot::getImage()
{
    return mImage;
}

RoboWorker *VRobot::lpc( int ax )
{
    Q_ASSERT( NULL != m_pAxesWorkers );
    Q_ASSERT( ax >= 0 && ax < axes() );

    return m_pAxesWorkers + ax;
}

RoboWorker *VRobot::lpc( )
{
    return m_pRoboWoker;
}
