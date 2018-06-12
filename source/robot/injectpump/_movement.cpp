#include "injectpump.h"
#include "../../device/mrq/deviceMRQ.h"


int robotInject::program( IPKeyPointList &curve,
             const tpvRegion &region )
{
    int ret;

    //! 0. check
    ret = verifyTrace( curve );
    if ( ret != 0 )
    { return ret; }

    //! 1.build
    QList<int> secList;
    ret = buildTrace( curve, mJointsGroup, secList );
    if ( ret != 0 )
    { return ret; }

    //! 2.config
    setLoop( 1 );

    //! 3.download
    ret = downloadTrace( region, mJointsGroup );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotInject::move( IPKeyPointList &curve,
                       const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    ret = preMove( curve, region );

    return ret;
}

int robotInject::move( float dx, float dy, float dt, float endVx, float endVy,
                       const tpvRegion &region )
{
    IPKeyPointList curve;

    IPKeyPoint kp;

    //! p0
    kp.t = 0;
    kp.x = 0;
    kp.y = 0;
    kp.vx = 0;
    kp.vy = 0;

    curve.append( kp );

    //! p1
    kp.t = dt;
    kp.x = dx;
    kp.y = dy;
    kp.vx = endVx;
    kp.vy = endVy;

    curve.append( kp );

    return move( curve, region );
}

int robotInject::move ( float dx, float dxt,
           float dy, float dyt,
           const tpvRegion &region )
{
    IPKeyPointList curve;

    IPKeyPoint kp;

    //! p0
    kp.t = 0;
    kp.x = 0;
    kp.y = 0;
    kp.vx = 0;
    kp.vy = 0;

    curve.append( kp );

    //! px
    kp.t = dxt;
    kp.x = dx;
    kp.y = 0;
    kp.vx = 0;
    kp.vy = 0;

    //! py
    kp.t = dyt;
    kp.x = 0;
    kp.y = dy;
    kp.vx = 0;
    kp.vy = 0;

    curve.append( kp );

    return move( curve, region );
}

int robotInject::preMove( IPKeyPointList &curve,
          const tpvRegion &region )
{
    int ret;

    setLoop( 1, region );

    //! program
    ret = program( curve, region );

    return ret;
}


int robotInject::goZero( const tpvRegion &region,
                     int jointId, bool bCcw )
{
    if ( 0 != checkRoboTask() )
    {
        sysError( QObject::tr("task not running") );
        return -1;
    }

    setLoop( 1 );

    int ax;
    MegaDevice::deviceMRQ *pMrq;
    pMrq = jointDevice( jointId, &ax );
    Q_ASSERT( NULL != pMrq );

    IJZeroArg *pArg;
    pArg = new IJZeroArg();
    pArg->mRegion = region;

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();

    //! dir by ccw
    int dir = bCcw ? (-1) : (1);

    //! x
    if ( jointId == 0 )
    {logDbg()<<mZeroDistance<<mZeroTime<<mVGapDistance<<mVGapTime;
        //! arg
        pArg->mAx = 0;
        pArg->mZeroDist = dir * mZeroDistance;
        pArg->mZeroTime = mZeroTime;
        pArg->mZeroEndV = dir * mZeroSpeed;

        pArg->mZeroVGapDist = -dir * mVGapDistance;
        pArg->mZeroVGapTime = mVGapTime;

        pArg->mZeroIGapDist = -dir * mIGapDistance;
        pArg->mZeroIGapTime = mIGapTime;

        //! time
        pArg->mTick = mZeroTick;
        pArg->mTmo = mZeroTmo;

        pReq->request( this, (VRobot::apiTaskRequest)(this->zeroAxesTask), pArg );

        m_pRoboTask->setRequest( pReq );
        m_pRoboTask->start();

    }
    else if ( jointId == 1 )
    {
        //! arg
        pArg->mAx = 1;
        pArg->mZeroDist = dir * mZeroDistance;
        pArg->mZeroTime = mZeroTime;
        pArg->mZeroEndV = dir * mZeroSpeed;

        pArg->mZeroVGapDist = -dir * mVGapDistance;
        pArg->mZeroVGapTime = mVGapTime;

        pArg->mZeroIGapDist = -dir * mIGapDistance;
        pArg->mZeroIGapTime = mIGapTime;

        //! time
        pArg->mTick = mZeroTick;
        pArg->mTmo = mZeroTmo;

        pReq->request( this, (VRobot::apiTaskRequest)(this->zeroAxesTask), pArg );

        m_pRoboTask->setRequest( pReq );
        m_pRoboTask->start();
    }
    else
    {
        Q_ASSERT( false );
    }

    return 0;
}

int robotInject::goZero( const tpvRegion &region )
{
    if ( 0 != checkRoboTask() )
    {
        sysError( QObject::tr("task not running") );
        return -1;
    }

    setLoop( 1 );

    IJZeroArg *pArg;
    pArg = new IJZeroArg();
    pArg->mRegion = region;

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();

    //! arg
    pArg->mAx = 128;        //! full ax
    pArg->mZeroDist = mZeroDistance;
    pArg->mZeroTime = mZeroTime;
    pArg->mZeroEndV = mZeroSpeed;

                            //! us
    pArg->mTmo = mZeroTmo;
    pArg->mTick = mZeroTick;

    pArg->mZeroVGapDist = -mVGapDistance;
    pArg->mZeroVGapTime = mVGapTime;

    pArg->mZeroIGapDist = -mIGapDistance;
    pArg->mZeroIGapTime = mIGapTime;

    //! \todo x, y invert
    pReq->request( this, (VRobot::apiTaskRequest)(this->zeroAxesTask), pArg );

    m_pRoboTask->setRequest( pReq );
    m_pRoboTask->start();

    return 0;
}

int robotInject::zeroAxesTask( void *pArg )
{
    Q_ASSERT( NULL != pArg );

    tpvRegion region;
    int ret;

    IJZeroArg *pZArg = (IJZeroArg*)pArg;
    region = pZArg->mRegion;
logDbg()<<pZArg->mZeroEndV;
    //! x
    if ( pZArg->mAx == 0 )
    {
        //! m
        move( pZArg->mZeroDist, 0, pZArg->mZeroTime, pZArg->mZeroEndV, 0, region );

        //! wait
        //! \todo wait idle
        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

        //! gap
        move( pZArg->mZeroVGapDist, 0, pZArg->mZeroVGapTime, 0, 0, region );

        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }
    }
    //! y
    else if ( pZArg->mAx == 1 )
    {
        move( 0, pZArg->mZeroDist, pZArg->mZeroTime, 0, pZArg->mZeroEndV, region );

        //! \todo
        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

        move( 0, pZArg->mZeroIGapDist, pZArg->mZeroIGapTime, 0, 0, region );

        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

    }
    else
    {
        //! x
        pZArg->mAx = 0;
        ret = zeroAxesTask( pZArg );
        if ( ret != 0 )
        { return ret; }

        //! y
        pZArg->mAx = 1;
        ret = zeroAxesTask( pZArg );
        if ( ret != 0 )
        { return ret; }

        return ret;
    }

    return 0;
}

