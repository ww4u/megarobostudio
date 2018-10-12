#include "h2z.h"
#include "../../device/mrq/deviceMRQ.h"
#include "../../arith/h2_split/h2_split.h"

int robotH2Z::program( QList<H2ZKeyPoint> &curve,
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

int robotH2Z::move( QList<H2ZKeyPoint> &curve,
                         const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    ret = preMove( curve, region );

    return ret;
}

int robotH2Z::move( float dx, float dy, float dz,
                    float dt,
                    float endVx, float endVy, float endVz,
                    const tpvRegion &region )
{
    QList<H2ZKeyPoint> curve;

    H2ZKeyPoint kp;

    //! p0
    kp.t = 0;
    kp.x = 0;
    kp.y = 0;
    kp.z = 0;
    kp.vx = 0;
    kp.vy = 0;
    kp.vz = 0;

    curve.append( kp );

    //! p1
    kp.t = dt;
    kp.x = dx;
    kp.y = dy;
    kp.z = dz;
    kp.vx = endVx;
    kp.vy = endVy;
    kp.vz = endVz;
    curve.append( kp );

    return move( curve, region );
}

int robotH2Z::preMove( QList<H2ZKeyPoint> &curve,
          const tpvRegion &region )
{
    int ret;

    setLoop( 1, region );

    //! program
    ret = program( curve, region );

    return ret;
}

int robotH2Z::goZero( const tpvRegion &region,
                      int jointId,
                      bool bCcw )
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

    H2ZeroArg *pArg;
    pArg = new H2ZeroArg();
    pArg->mRegion = region;

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();

    int dir;

    //! x
    if ( jointId == 0 )
    {
        dir = bCcw ? (-1) : (1);

        //! arg
        pArg->mAx = 0;
        pArg->mZeroXDist = dir * mZeroDistance;
        pArg->mZeroTime = mZeroTime;
        pArg->mZeroXEndV = dir * mZeroSpeed;

        pArg->mZeroXGapDist = -dir * mGapDistance;
        pArg->mZeroGapTime = mGapTime;

        //! time
        pArg->mTick = mZeroTick;
        pArg->mTmo = mZeroTmo;

        pReq->request( this,
                       (VRobot::apiTaskRequest)(&robotH2Z::zeroAxesTask),
                       pArg );

        m_pRoboTask->setRequest( pReq );
        m_pRoboTask->start();

    }
    else if ( jointId == 1 )
    {
        dir = bCcw ? (-1) : (1);

        //! arg
        pArg->mAx = 1;
        pArg->mZeroYDist = dir * mZeroDistance;
        pArg->mZeroTime = mZeroTime;
        pArg->mZeroYEndV = dir * mZeroSpeed;

        pArg->mZeroYGapDist = -dir * mGapDistance;
        pArg->mZeroGapTime = mGapTime;

        //! time
        pArg->mTick = mZeroTick;
        pArg->mTmo = mZeroTmo;

        pReq->request( this,
                       (VRobot::apiTaskRequest)(&robotH2Z::zeroAxesTask),
                       pArg );

        m_pRoboTask->setRequest( pReq );
        m_pRoboTask->start();
    }
    else if ( jointId == 2 )
    {
        //! arg
        //! same as x,y
        dir = bCcw ? (-1) : (1);

        pArg->mAx = 2;
        pArg->mZeroZDist = dir * mZeroDistance;
        pArg->mZeroZTime = mZeroTime;
        pArg->mZeroZEndV = dir * mZeroSpeed;

        pArg->mZeroGapZDist = -1 * dir * mGapZDistance;
        pArg->mZeroGapZTime = mGapZTime;

        //! time
        pArg->mTick = mZeroTick;
        pArg->mTmo = mZeroTmo;

        pReq->request( this,
                       (VRobot::apiTaskRequest)(&robotH2Z::zeroAxesTask),
                       pArg );

        m_pRoboTask->setRequest( pReq );
        m_pRoboTask->start();
    }
    else
    {
        Q_ASSERT( false );
    }

    return 0;
}

int robotH2Z::goZero( const tpvRegion &region )
{
    QList<int> jList;
    jList<<0<<1<<2;

    return goZero( region, jList, mJointZeroCcw );
}

int robotH2Z::goZero( const tpvRegion &region,
                      const QList<int> &jointList,
                      const QList<bool> &ccwList )
{
    if ( 0 != checkRoboTask() )
    {
        sysError( QObject::tr("task not running") );
        return -1;
    }

    setLoop( 1 );

    H2ZeroArg *pArg;
    pArg = new H2ZeroArg();
    pArg->mRegion = region;

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();

    int dir;

    //! arg
    pArg->mAx = 128;        //! full ax

    dir = ccwList.at(0) ? (-1) : (1);
    pArg->mZeroXDist = dir * mZeroDistance;
    pArg->mZeroTime = mZeroTime;
    pArg->mZeroXEndV = dir * mZeroSpeed;

    pArg->mTmo = mZeroTmo;
    pArg->mTick = mZeroTick;

    pArg->mZeroXGapDist = -dir * mGapDistance;
    pArg->mZeroGapTime = mGapTime;

                            //! bx
    dir = ccwList.at(1) ? (-1) : (1);
    pArg->mZeroYDist = dir * mZeroDistance;
    pArg->mZeroTime = mZeroTime;
    pArg->mZeroYEndV = dir * mZeroSpeed;

    pArg->mZeroYGapDist = -dir * mGapDistance;
    pArg->mZeroGapTime = mGapTime;

    pArg->mTmo = mZeroTmo;
    pArg->mTick = mZeroTick;

                            //! special for z
    dir = ccwList.at(2) ? (-1) : (1);
    pArg->mZeroZDist = dir * mZeroDistance ;
    pArg->mZeroZTime = mZeroTime;
    pArg->mZeroZEndV = dir * mZeroSpeed;

    pArg->mZeroGapZDist = -1 * dir * mGapZDistance;
    pArg->mZeroGapZTime = mGapZTime;

    pReq->request( this,
                   (VRobot::apiTaskRequest)(&robotH2Z::zeroAxesTask),
                   pArg );

    m_pRoboTask->setRequest( pReq );
    m_pRoboTask->start();

    return 0;
}

int robotH2Z::getPOSE(float pos[])
{
    return pose( pos[0], pos[1] );
}

int robotH2Z::zeroAxesTask( void *pArg )
{
    Q_ASSERT( NULL != pArg );

    tpvRegion region;
    int ret;

    H2ZeroArg *pZArg = (H2ZeroArg*)pArg;
    region = pZArg->mRegion;

    //! x
    if ( pZArg->mAx == 0 )
    {
        //! m
        move( pZArg->mZeroXDist, 0, 0, pZArg->mZeroTime, pZArg->mZeroXEndV,0,0,region );

        //! wait
        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

        //! gap
        move( pZArg->mZeroXGapDist, 0, 0, pZArg->mZeroGapTime, 0,0,0, region );

        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

        //! rst l,y count
        rstZeroAngle();
    }
    //! y
    else if ( pZArg->mAx == 1 )
    {
        move( 0, pZArg->mZeroYDist, 0, pZArg->mZeroTime, 0,pZArg->mZeroYEndV,0, region );

        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

        move( 0, pZArg->mZeroYGapDist, 0, pZArg->mZeroGapTime, 0,0,0, region );

        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

        //! rst l,y count
        rstZeroAngle();
    }
    //! z
    else if ( pZArg->mAx == 2 )
    {
        move( 0, 0, pZArg->mZeroZDist, pZArg->mZeroTime, 0,0,pZArg->mZeroZEndV, region );

        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

        //! invert z
        move( 0, 0, pZArg->mZeroGapZDist, pZArg->mZeroGapZTime, 0,0,0, region );

        ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }

        //! rst l,y count, z
        rstZeroAngle();
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

        //! z
        pZArg->mAx = 2;
        ret = zeroAxesTask( pZArg );
        if ( ret != 0 )
        { return ret; }

        return ret;
    }

    return 0;
}

int robotH2Z::rstZeroAngle()
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;
    int ret;
    //! rst l,y count
    for ( int jId = 0; jId < axes(); jId++ )
    {
        pMrq = jointDevice( jId, &ax );
        Q_ASSERT( NULL != pMrq );
        ret = pMrq->setMOTION_ABCOUNTCLEAR( ax );
        if ( ret != 0 )
        { return ret; }
    }

    return 0;
}

int robotH2Z::pose( float &x, float &y )
{
    int ret;
    ret = robotH2::pose( x, y );

    y = -y;
    return ret;
}

int robotH2Z::moveTest1( const tpvRegion &region )
{
    H2ZKeyPoint pt1,pt2;
    pt1.t = 0;
    pt1.x = 0;
    pt1.y = 0;

    pt2.t = 1;
    pt2.x = 20;
    pt2.y = 20;

    QList<H2ZKeyPoint> curve;
    curve.append( pt1 );
    curve.append( pt2 );

    return move( curve, region );
}

int robotH2Z::moveTest2( const tpvRegion &region )
{
    H2ZKeyPoint pt1,pt2;
    pt1.t = 1;
    pt1.x = 0;
    pt1.y = 0;

    pt2.t = 0;
    pt2.x = 20;
    pt2.y = 20;

    //! p2 -> p1
    QList<H2ZKeyPoint> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, region );
}

