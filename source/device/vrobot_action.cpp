#include "vrobot.h"

#include "../app/systimer.h"

int VRobot::build( MegaTableModel *pModel,
                   xxxGroup< tracePoint > &tracePlan,
                   xxxGroup< jointsTrace > &jointsPlan,
                   QList< tpvGroup *> &gp,
                   QList< int> &sectionList )
{
    return -1;
}

//! action
int VRobot::transform( int axes )
{ return 0; }

int VRobot::download( tpvGroup *pGroup,
                      const tpvRegion &region )
{
    Q_ASSERT( NULL != pGroup );

    return 0;
}

int VRobot::download( QList<tpvGroup*> &groups,
                      QList<int> &joints,
                      const tpvRegion &region )
{
    return 0;
}
int VRobot::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}

int VRobot::run( const tpvRegion &region )
{
    return 0;
}
int VRobot::stop( const tpvRegion &region  )
{
    return 0;
}

int VRobot::goFactory( const tpvRegion &region )
{ return 0; }

int VRobot::goZero( const tpvRegion &region )
{ return 0; }
int VRobot::goZero( const tpvRegion &region,
                    int jointTabId )
{ return 0; }
int VRobot::goZero( const tpvRegion &region,
                    int jointTabId, bool bCcw )
{ return 0; }

int VRobot::goZero( const tpvRegion &region,
                    const QList<int> &jointList,
                    const QList<bool> &ccwList )
{ return 0; }

bool VRobot::checkZeroValid()
{ return false; }
float VRobot::getZero( int jointTabId )
{ return 0; }
int  VRobot::setZero( int jointTabId, float val )
{ return 0; }

int VRobot::align( const tpvRegion &region )
{ return 0; }

int VRobot::getPOSE( float pos[] )
{ return -1; }
float VRobot::toDeltaAngle( int jointId, float angle )
{ return 0; }

int VRobot::setLoop( int n, const tpvRegion &region )
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

void VRobot::attachCondition( const tpvRegion &region,
                              MegaDevice::RoboCondition *pCond )
{

}

bool VRobot::waitCondition( const tpvRegion &region,
                            MegaDevice::RoboCondition *pCond,
                            int tmoms )
{
   return false;
}
