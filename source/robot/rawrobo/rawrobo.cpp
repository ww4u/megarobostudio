
#include "rawrobo.h"
#include "../../device/mrq/devicemrq_state.h"
#include "../../device/board/_MRQ_enum.h"


TraceKeyPoint::TraceKeyPoint( float pt, float px, float py, float pz, float phand )
{
    t = pt;
    x = px;
    y = py;
    z = pz;

    hand = phand;
    iMask = 0;
    set_bit( iMask, BIT_INTERP );
}

MegatronKeyPoint::MegatronKeyPoint( float pt,
                  float px1, float py1, float pz1,
                  float px2, float py2, float pz2 )
{
    t = pt;

    x1 = px1;
    y1 = py1;
    z1 = pz1;

    x1 = px2;
    y2 = py2;
    z2 = pz2;
}

RawRobo::RawRobo()
{
    mPlanStep = 5.0;  //! mm
    mPlanMode = plan_linear;
}

RawRobo::~RawRobo()
{
    foreach( RawRoboFsm*pFsm, mFsms )
    {
        Q_ASSERT( NULL != pFsm );
        delete pFsm;
    }

    mFsms.clear();
}

void RawRobo::postCtor()
{
    //! base call
    VRobot::postCtor();

    //! sub call
    Q_ASSERT( regions() > 0 );

    //! create the fsms
    tpvRegion region;
    RawRoboFsm *pFsm;
//    for ( int i = 0; i < axes(); i++ )
    for ( int i = 0; i < regions(); i++ )
    {
        region.setRegion( 0, i );
        pFsm = new RawRoboFsm();
        Q_ASSERT( NULL != pFsm );

        pFsm->setRegion( 0, i );    //! only one unit

        pFsm->attachRobot( this );
        pFsm->build();

        mFsms.insert( region, pFsm );
        Q_ASSERT( mFsms[region]!=NULL );
    }
}

void RawRobo::switchReset( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_RESET,
                   (byte)region.page() );
}
void RawRobo::switchStop( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_STOP,
                   (byte)region.page() );
}
void RawRobo::switchRun( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_RUN,
                   (byte)region.page() );
    logDbg();
}
void RawRobo::switchPrepare( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_PREPARE,
                   (byte)region.page() );
}
void RawRobo::switchEmergStop( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_EMERGSTOP,
                   (byte)region.page() );
}

void RawRobo::queryState( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_STATE_Q,
                   (byte)x_channel,
//                   (byte)MRQ_MOTION_SWITCH_1_MAIN
                   (byte)region.page()
                    );
}

void RawRobo::toState( const tpvRegion &region, int stat )
{}
int RawRobo::state( const tpvRegion &region )
{
    return fsm( region )->state();
}

void RawRobo::onTimer( void *pContext, int id )
{
    Q_ASSERT( NULL != pContext );

    //! context is fsm
    RawRoboFsm *pFsm = (RawRoboFsm*)pContext;
    Q_ASSERT( NULL != pFsm );

    pFsm->onTimer( id );
//    if ( pFsm == &mFsm )
//    { mFsm.onTimer( id ); }

    logDbg()<<id;
}

void RawRobo::attachCondition(
                              const tpvRegion &region,
                              MegaDevice::RoboCondition *pCond )
{
    fsm( region )->attachCondition( pCond );
}

bool RawRobo::waitCondition(
                            const tpvRegion &region,
                            MegaDevice::RoboCondition *pCond,
                            int tmoms )
{
    return fsm( region )->waitCondition( pCond, tmoms );
}

void RawRobo::setPlanStep( float step )
{ mPlanStep = step; }
float RawRobo::planStep()
{ return mPlanStep; }

void RawRobo::setPlanMode( eRoboPlanMode mode )
{ mPlanMode = mode; }
eRoboPlanMode RawRobo::getPlanMode()
{ return mPlanMode; }

RawRoboFsm * RawRobo::fsm( const tpvRegion &region )
{logDbg()<<region.axes()<<region.page()<<mFsms.size();
    Q_ASSERT( mFsms.contains( region) );
    Q_ASSERT( NULL != mFsms[region] );

    return mFsms[region];
}




