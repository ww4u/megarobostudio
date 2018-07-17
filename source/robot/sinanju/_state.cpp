#include "sinanju.h"

void robotSinanju::toState(int stat)
{
    logDbg()<<stat;

//    if (  stat == MegaDevice::mrq_msg_standby )
//    {
//        if ( mFsm.runReqed() )
//        { run(); }
//    }
}

//int robotSinanju::state( const tpvRegion &region, int inTask )
//{
//    //! for the sub status
//    MegaDevice::deviceMRQ *pMrq;
//    int subAx, mrqState;
//    //! sub joint task -- check again
//    for ( int i = 0; i < axes(); i++ )
//    {
//        pMrq = jointDevice( i, &subAx );
//        if ( NULL == pMrq )
//        {}
//        //! get real state
//        else
//        {
//            mrqState = pMrq->fsmState( region, inTask );
//            if ( mrqState != MegaDevice::mrq_state_idle )
//            { return mrqState; }
//        }
//    }

//    //! real status
//    return fsm( region )->state();
//}
