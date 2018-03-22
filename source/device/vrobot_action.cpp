#include "vrobot.h"

#include "../../app/systimer.h"

//! action
int VRobot::transform( int axes )
{ return 0; }

int VRobot::download( tpvGroup *pGroup, int page, int axes )
{
    Q_ASSERT( NULL != pGroup );

    return 0;
}
int VRobot::download( motionGroup *pGroup, int page, int axes )
{
    Q_ASSERT( NULL != pGroup );

    return 0;
}
int VRobot::download( QList<tpvGroup*> &groups,
                      QList<int> &joints )
{
    return 0;
}
int VRobot::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}

int VRobot::run( int axes )
{
    return 0;
}
int VRobot::stop( int axes  )
{
    return 0;
}

int VRobot::run()
{
    return 0;
}
int VRobot::stop()
{
    return 0;
}

int VRobot::setLoop( int n )
{ return 0;  }
int VRobot::loopNow()
{ return 0; }

void VRobot::startTimer( void *pContext, int id, int tmous )
{
    SysTimerThread::sysStartTimer( this, pContext, id, tmous );
}
void VRobot::killTimer( void *pContext, int id )
{
    SysTimerThread::sysStopTimer( this, pContext, id );
}

void VRobot::onTimer( void *pContext, int id )
{

}

void VRobot::attachCondition( int subAxes,
                              MegaDevice::RoboCondition *pCond )
{

}

bool VRobot::waitCondition( int subAxes,
                            MegaDevice::RoboCondition *pCond,
                            int tmoms )
{
   return false;
}

void VRobot::attachCondition(
                              MegaDevice::RoboCondition *pCond )
{

}

bool VRobot::waitCondition(
                            MegaDevice::RoboCondition *pCond,
                            int tmoms )
{
   return false;
}
