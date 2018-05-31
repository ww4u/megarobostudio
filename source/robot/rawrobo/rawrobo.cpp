
#include "rawrobo.h"
#include "../../device/mrq/devicemrq_state.h"
#include "../../device/board/_MRQ_enum.h"

D1Point::D1Point( float pt, float pp )
{
    t = pt;
    p = pp;
}

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
                  float tfx, float tly, float tfz,
                  float tbx, float tRy, float tbz )
{
    t = pt;

    fx = tfx;
    ly = tly;
    fz = tfz;

    bx = tbx;
    ry = tRy;
    bz = tbz;
}

H2KeyPoint::H2KeyPoint( float pt,
                  float px, float py, float pv )
{
    t = pt;

    x = px;
    y = py;
    v = pv;
}

RawRobo::RawRobo()
{
}

RawRobo::~RawRobo()
{
    foreach( RawRoboFsm*pFsm, mFsms )
    {
        Q_ASSERT( NULL != pFsm );
        delete pFsm;
    }

    mFsms.clear();

    delete_all( mJointsGroup );
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
int RawRobo::state( const tpvRegion &region, int inTask )
{
    if ( inTask != 0 )
    { return fsm( region )->state();}
    else
    {
        //! robo task valid
        if ( m_pRoboTask != NULL  )
        {
            if ( m_pRoboTask->isRunning() )
            { return 1; }
        }

        return fsm( region )->state();
    }

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

RawRoboFsm * RawRobo::fsm( const tpvRegion &region )
{logDbg()<<region.axes()<<region.page()<<mFsms.size();
    Q_ASSERT( mFsms.contains( region) );
    Q_ASSERT( NULL != mFsms[region] );

    return mFsms[region];
}




