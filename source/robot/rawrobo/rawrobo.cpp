
#include "rawrobo.h"
#include "../../device/mrq/devicemrq_state.h"
#include "../../device/board/_MRQ_enum.h"

D1Point::D1Point( float pt, float pp, float pv )
{
    t = pt;
    p = pp;
    v = pv;
}

TraceKeyPoint::TraceKeyPoint( float pt, float px, float py, float pz, float phand )
{
    t = pt;
    x = px;
    y = py;
    z = pz;

    hand = phand;
    iMask = 0;
}

void TraceKeyPoint::clear()
{
    t = 0;
    x = 0;
    y = 0;
    z = 0;

    hand = 0;
    iMask = 0;
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
                  float px, float py, float pvx, float pvy )
{
    t = pt;

    x = px;
    y = py;
    vx = pvx;
    vy = pvy;
}

H2ZKeyPoint::H2ZKeyPoint( float pt, float px, float py, float pz,
                                    float pvx, float pvy, float pvz )
{
    t = pt;

    x = px;
    y = py;
    z = pz;

    vx = pvx;
    vy = pvy;
    vz = pvz;
}

IPKeyPoint::IPKeyPoint( float pt,
                        float px, float py,
                        float pvx, float pvy )
{
    t = pt;

    x = px;
    y = py;
    vx = pvx;
    vy = pvy;
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
    m_pBus->write( id, MRQ_mc_MOTION, MRQ_sc_MOTION_STATE_Q,
                   (byte)mSubGroupId,
                   (byte)region.page()
                    );
}

void RawRobo::toState( const tpvRegion &region, int stat )
{
    //! state changed
//    m_pXEventSema->release();
}
int RawRobo::state( const tpvRegion &region, int inTask )
{
    //! not in task
    if ( inTask == 0 )
    {
        //! robo task valid
        if ( m_pRoboTask != NULL  )
        {
            if ( m_pRoboTask->isRunning() )
            { return MegaDevice::mrq_state_running; }
        }

        //! real status
        return fsm( region )->state();
    }
    //! in task
    //! real status
    else
    { return fsm( region )->state(); }
}

void RawRobo::onTimer( void *pContext, int id )
{
    Q_ASSERT( NULL != pContext );

    //! context is fsm
    RawRoboFsm *pFsm = (RawRoboFsm*)pContext;
    Q_ASSERT( NULL != pFsm );

    pFsm->onTimer( id );
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
{
    Q_ASSERT( mFsms.contains( region) );
    Q_ASSERT( NULL != mFsms[region] );

    return mFsms[region];
}

//! joint move
int RawRobo::jMove( const tpvRegion &region,
                         int jTabId, bool bCcw,
                         float t, float p, float ev )
{
    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( jTabId, &subAx );
    if ( NULL == pMrq )
    {
        sysError( QObject::tr("Invalid device") );
        return -1;
    }
    Q_ASSERT( NULL != pMrq );

    //! to stop mode
    pMrq->setLoop( 1, tpvRegion(subAx,region.page()) );

    return pMrq->lightCouplingZero( tpvRegion(subAx,region.page()),
                             t,
                             bCcw ? (-p) : ( p ),
                             bCcw ? (-ev) : ( ev ) );
}

//! \note only for internal task
int RawRobo::waitFsm( pvt_region,
             int dstState,
             int tmous,
             int tick
             )
{
    do
    {
        //! termination
        if ( QThread::currentThread()->isInterruptionRequested() )
        { return -2; }

        Q_ASSERT( tick > 0 );
        QThread::usleep( tick );
//        m_pXEventSema->tryAcquire( 1, tick > 1000 ? (tick/1000) : 1 );

        if ( state( region, 1 ) == dstState )
        { return 0; }

        tmous -= tick;
    }while( tmous > 0 );

    return -1;
}

void RawRobo::sync( const tpvRegion &region )
{
    onLine();

    queryState( region );
}


