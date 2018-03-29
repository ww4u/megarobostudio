#include "vrobot.h"

QString & VRobot::getClass()
{ return mClass; }
QString & VRobot::getDetail()
{ return mDetail; }
VRobot::robotEnum VRobot::getId()
{ return mId; }
VRobot::robotEnum VRobot::robotId()
{ return mId; }

int VRobot::setAxes(int n)
{
    mAxes = n;

    //! gc
    gcWorker();

    //! create the workers for each axes
    m_pAxesWorkers = new RoboWorker[ n ];
    Q_ASSERT( NULL != m_pAxesWorkers );
    for( int i = 0; i < n; i++ )
    { m_pAxesWorkers[i].attachRobot( this, i ); }

    //! init the angle
    for ( int i = 0; i <n; i++ )
    { mJointAngleMask.append( false ); }

    return 0;
}
int VRobot::axes()
{ return mAxes; }
QImage & VRobot::getImage()
{ return mImage; }

void VRobot::setRegions( int regions )
{ mRegions = regions; }
int VRobot::regions()
{ return mRegions; }

void VRobot::setDcAxes( int n )
{ mDCAxes = n; }
int VRobot::dcAxes()
{ return mDCAxes; }

void VRobot::setDOs( int n )
{ mDOs = n; }
int VRobot::dos()
{ return mDOs; }

void VRobot::setDIs( int n )
{ mDIs = n; }
int VRobot::dis()
{ return mDIs; }

void VRobot::setIsos( int n )
{ mISOs = n; }
int VRobot::isos()
{ return mISOs; }

void VRobot::setIsis( int n )
{ mISIs = n; }
int VRobot::isis()
{ return mISIs; }

void VRobot::setAins( int n )
{ mAINs = n; }
int VRobot::ains()
{ return mAINs; }

void VRobot::setMosos( int n )
{ mMosos = n; }
int VRobot::mosos()
{ return mMosos; }

void VRobot::setEncoders( int n )
{ mEncoders = n; }
int VRobot::encoders()
{ return mEncoders; }

void VRobot::setTemperatures( int n)
{ mTemperatures = n; }
int VRobot::temperatures()
{ return mTemperatures; }

void VRobot::setUarts( int n )
{ mUARTs = n; }
int VRobot::uarts()
{ return mUARTs; }

void VRobot::setUartSensors( int n )
{ mUART_Sensors = n; }
int VRobot::uartSensors()
{ return mUART_Sensors; }

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
