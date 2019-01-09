#include "h2.h"
#include "../../device/mrq/deviceMRQ.h"
#include "../../arith/h2_split/h2_split.h"

int robotH2::program( QList<H2KeyPoint> &curve,
                      const tpvRegion &region )
{
    int ret;

    //! 0. check
    ret = verifyTrace( curve );
    if ( ret != 0 )
    { return ret; }

    //! 1. coord
    coordRotate( curve );

    //! 2.build
    QList<int> secList;
    ret = buildTrace( curve, mJointsGroup, secList );
    if ( ret != 0 )
    { return ret; }

    //! 3.config
    setLoop( 1, region );

    //! 4.download
    ret = downloadTrace( region, mJointsGroup );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotH2::move( QList<H2KeyPoint> &curve,
                         const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    ret = preMove( curve, region );

    return ret;
}

int robotH2::move( float x, float y,
                   float dstx, float dsty, float dt, float endVx, float endVy,
                   const tpvRegion &region )
{
    QList<H2KeyPoint> curve;

    H2KeyPoint kp;

    //! p0
    kp.t = 0;
    kp.x = x;
    kp.y = y;
    kp.vx = 0;
    kp.vy = 0;

    curve.append( kp );

    //! p1
    kp.t = dt;
    kp.x = dstx;
    kp.y = dsty;
    kp.vx = endVx;
    kp.vy = endVy;
    curve.append( kp );

    return move( curve, region );
}

int robotH2::preMove( QList<H2KeyPoint> &curve,
          const tpvRegion &region )
{
    int ret;

    setLoop( 1, region );

    //! program
    ret = program( curve, region );

    return ret;
}

bool robotH2::checkZeroValid()
{
    int ax;
    MegaDevice::deviceMRQ *pMrq;

    //! get ax pos
    quint16 dio, subBits;
    for ( int jId = 0; jId < axes(); jId++ )
    {
        pMrq = jointDevice( jId, &ax );
        Q_ASSERT( NULL != pMrq );

        //! get the dio
        if ( 0 == pMrq->getSYSTEM_DIOSTATE( &dio ) )
        {}
        else
        {
            sysError( QObject::tr("Invalid init"), QString::number( dio, 16) );
            return false;
        }

        //! get the pos
        subBits = ( dio >> (4 * ax) ) & 0x03;
        if ( subBits != 0 )
        { return false; }
    }

    return true;
}

int robotH2::goZero( const tpvRegion &region,
                     int jointId, bool bCcw )
{
    if ( 0 != checkRoboTask() )
    {
        sysError( QObject::tr("task not running") );
        return -1;
    }

    setLoop( 1, region );

    int ax;
    MegaDevice::deviceMRQ *pMrq;
    pMrq = jointDevice( jointId, &ax );
    Q_ASSERT( NULL != pMrq );

    H2ZeroArg *pArg;
    pArg = new H2ZeroArg();
    pArg->mRegion = region;

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();

    //! x
    if ( jointId == 0 )
    {//logDbg()<<mZeroDistance<<mZeroTime<<mGapDistance<<mGapTime;
        //! arg
        pArg->mAx = 0;
        pArg->mZeroDist = mZeroDistance;
        pArg->mZeroTime = mZeroTime;
        pArg->mZeroEndV = -mZeroSpeed;

        pArg->mZeroGapDist = mGapDistance;
        pArg->mZeroGapTime = mGapTime;

        //! time
        pArg->mTick = mZeroTick;
        pArg->mTmo = mZeroTmo;

        pReq->request( this,
                       (VRobot::apiTaskRequest)(&robotH2::zeroAxesTask),
                       pArg );

        m_pRoboTask->setRequest( pReq );
        m_pRoboTask->start();

    }
    else if ( jointId == 1 )
    {
        //! arg
        pArg->mAx = 1;
        pArg->mZeroDist = mZeroDistance;
        pArg->mZeroTime = mZeroTime;
        pArg->mZeroEndV = -mZeroSpeed;

        pArg->mZeroGapDist = mGapDistance;
        pArg->mZeroGapTime = mGapTime;

        //! time
        pArg->mTick = mZeroTick;
        pArg->mTmo = mZeroTmo;

        pReq->request( this,
                       (VRobot::apiTaskRequest)(&robotH2::zeroAxesTask),
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

int robotH2::goZero( const tpvRegion &region )
{
    QList<int> jList;
    jList<<0<<1;

    return goZero( region, jList, mJointZeroCcw );
}

int robotH2::goZero( const tpvRegion &region,
                     const QList<int> &jointList,
                     const QList<bool> &ccwList )
{
    if ( 0 != checkRoboTask() )
    {
        sysError( QObject::tr("task not running") );
        return -1;
    }

    setLoop( 1, region );

    H2ZeroArg *pArg;
    pArg = new H2ZeroArg();
    pArg->mRegion = region;

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();

    //! arg
    pArg->mAx = 128;        //! full ax
    pArg->mZeroDist = mZeroDistance;
    pArg->mZeroTime = mZeroTime;
    pArg->mZeroEndV = -mZeroSpeed;

                            //! us
    pArg->mTmo = mZeroTmo;
    pArg->mTick = mZeroTick;

    pArg->mZeroGapDist = mGapDistance;
    pArg->mZeroGapTime = mGapTime;

    pReq->request( this,
                   (VRobot::apiTaskRequest)(&robotH2::zeroAxesTask),
                   pArg );

    m_pRoboTask->setRequest( pReq );
    m_pRoboTask->start();

    return 0;
}

int robotH2::getPOSE( float pos[] )
{
    return pose( pos[0], pos[1] );
}

int robotH2::zeroAxesTask( void *pArg )
{
    Q_ASSERT( NULL != pArg );

    //! check valid
    //! assure the pos is not zero
    if ( checkZeroValid() )
    {}
    else
    {
        sysError( QObject::tr("Invlaid init pos") );
        return -1;
    }

    tpvRegion region;
    int ret;

    H2ZeroArg *pZArg = (H2ZeroArg*)pArg;
    region = pZArg->mRegion;

    //! x
    if ( pZArg->mAx == 0 )
    {
        //! zero
        ret = zeroX( pZArg );
        if ( ret != 0 )
        { return ret; }

        //! gap
        ret = gapX( pZArg );
        if ( ret != 0 )
        { return ret; }

        ret = clrAngle();
    }
    //! y
    else if ( pZArg->mAx == 1 )
    {
        //! zero
        ret = zeroY( pZArg );
        if ( ret != 0 )
        { return ret; }

        //! gap
        ret = gapY( pZArg );
        if ( ret != 0 )
        { return ret; }

        //! rst l,y count
        ret = clrAngle();
    }
    else
    {
        //! zero
        if ( mZeroMovement == movement_x_y )
        {
            //! x
            pZArg->mAx = 0;
            ret = zeroX( pZArg );
            if ( ret != 0 ){ return ret; }

            //! y
            pZArg->mAx = 1;
            ret = zeroY( pZArg );
            if ( ret != 0 ){ return ret; }

            //! gap
            ret = gapXY( pZArg );
            if ( ret != 0 ){ return ret; }
        }
        else
        {
            //! y
            pZArg->mAx = 1;
            ret = zeroY( pZArg );
            if ( ret != 0 ){ return ret; }

            //! x
            pZArg->mAx = 0;
            ret = zeroX( pZArg );
            if ( ret != 0 ){ return ret; }

            //! gap
            ret = gapXY( pZArg );
            if ( ret != 0 ){ return ret; }
        }

        ret = clrAngle();
    }

    return ret;
}

int robotH2::zeroX( H2ZeroArg *pZArg )
{
    tpvRegion region;
    int ret;

    region = pZArg->mRegion;

//    bool bTransfer;
//    bTransfer = transferAble();
//    setTransferAble( false );

    //! m
    move(   pZArg->mZeroDist,0,
            0, 0, pZArg->mZeroTime, pZArg->mZeroEndV, 0, region
         );

//    setTransferAble( bTransfer );

    //! wait
    ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );

    return ret;
}
int robotH2::zeroY( H2ZeroArg *pZArg )
{
    tpvRegion region;
    int ret;

    region = pZArg->mRegion;

//    bool bTransfer;
//    bTransfer = transferAble();
//    setTransferAble( false );

    move( 0, pZArg->mZeroDist,
          0, 0, pZArg->mZeroTime, 0, pZArg->mZeroEndV, region );

//    setTransferAble( bTransfer );

    ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );

    return ret;
}

int robotH2::gapX( H2ZeroArg *pZArg )
{
    tpvRegion region;
    int ret;

    region = pZArg->mRegion;

    //! gap
    move( 0,0,
          pZArg->mZeroGapDist, 0, pZArg->mZeroGapTime, 0, 0, region );

    ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );

    return ret;
}
int robotH2::gapY( H2ZeroArg *pZArg )
{
    tpvRegion region;
    int ret;

    region = pZArg->mRegion;

    move( 0,0,
          0, pZArg->mZeroGapDist, pZArg->mZeroGapTime, 0, 0, region );

    ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
    if ( ret != 0 )
    { return ret; }

    return ret;
}

int robotH2::gapXY( H2ZeroArg *pZArg )
{
    tpvRegion region;
    int ret;

    region = pZArg->mRegion;

    move( 0,0,
          pZArg->mZeroGapDist, pZArg->mZeroGapDist, pZArg->mZeroGapTime, 0, 0, region );

    ret = waitFsm( region, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
    if ( ret != 0 )
    { return ret; }

    return ret;
}

int robotH2::clrAngle()
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

int robotH2::angle( int jId, float &fAng )
{
    Q_ASSERT( jId >=0 && jId < axes() );

    MegaDevice::deviceMRQ *pMrq;
    int ax;

    pMrq = jointDevice( jId, &ax );
    Q_ASSERT( NULL != pMrq );

//    int abcCount;
//    int ret = pMrq->getMOTION_ABCOUNT( ax, &abcCount );
//    if ( ret != 0 )
//    { return -1; }

    //! line -> angle
//    fAng = abcCount * 360.0f/mLines *

    fAng = pMrq->getIncAngle( ax ) * mEncoderDirs.at( jId );

    return 0;
}

int robotH2::pose( float &x, float &y )
{
    int ret;

    //! get angle
    float angleL, angleR;
    ret = angle( 0, angleL );
    if( ret != 0 )
    { return ret; }
    ret = angle( 1, angleR );
    if( ret != 0 )
    { return ret; }

    //! convert
    QList<int> gantry;
    gantry<<mToothType<<mToothGear<<mPDirIndex<<0;      //! \note 0 for angle in

    QList<double> zeroXy;
    zeroXy.append( mZeroX );
    zeroXy.append( mZeroY );
    ret = h2_split::h2Pose( mArmLengths,
                            gantry,
                            zeroXy,
                            angleL, angleR,
                            x, y );           //! \note invert

    //! invert x
//    x = -x;
//    y = -y;
    //! coord
    H2KeyPoint keyP;
    keyP.x = x;logDbg()<<x<<y;
    keyP.y = y;
    coordIRotate( keyP );
    x = keyP.x;logDbg()<<keyP.x<<keyP.y;
    y = keyP.y;

    return ret;
}

void robotH2::coordRotate( QList<H2KeyPoint> &curve )
{
//    if ( mbTransferAble )
    {
        //! for each point
        H2KeyPoint localPt;
        for ( int i = 0; i < curve.size(); i++ )
        {
            localPt = curve.at( i );

            coordRotate( localPt, mTransferR, mTransferS );

            curve[i] = localPt;
        }
    }
//    else
//    { return; }
}
void robotH2::coordRotate( H2KeyPoint &pt, double rot[2*2], double shift[2*1] )
{
    H2KeyPoint local;

    local = pt;

    pt.x = rot[0*2+0]*local.x + rot[0*2+1]*local.y;
    pt.y = rot[1*2+0]*local.x + rot[1*2+1]*local.y;
}

void robotH2::coordIRotate( H2KeyPoint &pt )
{
//    if ( mbTransferAble )
    {
        coordIRotate( pt, mTransferRInv, mTransferS );
    }
}
void robotH2::coordIRotate( H2KeyPoint &pt, double rot[2*2], double shift[2*1] )
{
    H2KeyPoint local;

    local = pt;

    //! -
    local.x = local.x - shift[0*1+0];
    local.y = local.y - shift[1*1+0];

    //! inv(A)*
    pt.x = rot[0*2+0]*local.x + rot[0*2+1]*local.y;
    pt.y = rot[1*2+0]*local.x + rot[1*2+1]*local.y;
}

int robotH2::moveTest1( const tpvRegion &region )
{
    H2KeyPoint pt1,pt2;
    pt1.t = 0;
    pt1.x = 0;
    pt1.y = 0;

    pt2.t = 1;
    pt2.x = 20;
    pt2.y = 20;

    QList<H2KeyPoint> curve;
    curve.append( pt1 );
    curve.append( pt2 );

    return move( curve, region );
}

int robotH2::moveTest2( const tpvRegion &region )
{
    H2KeyPoint pt1,pt2;
    pt1.t = 1;
    pt1.x = 0;
    pt1.y = 0;

    pt2.t = 0;
    pt2.x = 20;
    pt2.y = 20;

    //! p2 -> p1
    QList<H2KeyPoint> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, region );
}

