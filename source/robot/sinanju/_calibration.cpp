#include "sinanju.h"
#include "../../com/comassist.h"

#define SPACE_RESOLUTION    2     //! mm
#define SPACE_VELOCITY      5     //! mm/s

int robotSinanju::goFactory( const tpvRegion &region )
{
    QList<double> aimAngles;

    //! normalize
    for ( int i = 0; i < 4; i++ )
    {
        aimAngles.append( comAssist::normalizeDegree360( mJointFactoryList.at(i) + mInitAngles.at( i ) ) );
    }

    return toAimSession( region, aimAngles, mJointFactorySeperateList );
}

int robotSinanju::goZero( const tpvRegion &region )
{
    return toAimSession( region, mInitAngles, mSeperateAngles );
}

int robotSinanju::goZero( const tpvRegion &region,
                          int jointId, bool bCcw )
{
    Q_ASSERT( jointId >= 0 && jointId < axes() );

    //! device
    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( jointId, &subAx );
    if ( NULL == pMrq )
    { return -1; }

    setLoop( 1, region );

    //! jointid
    if ( jointId >= 0 && jointId <= 3 )
    {
        //! angle zero
        double angleZero = mInitAngles.at( jointId );
        float angleNow = pMrq->getAbsAngle( subAx );

        //! deduce
        float deltaAngle;
        deltaAngle = deduceAngle( angleNow, angleZero,mSeperateAngles.at( jointId),
                                  mAngleDir.at( jointId ) );

        sysLog( QString::number(deltaAngle) );
        Q_ASSERT( mZeroSpeed > 0 );
        pMrq->rotate( tpvRegion(subAx,region.page()),
                      qAbs(deltaAngle) / mZeroSpeed,
                      deltaAngle
                      );
    }
    //! hand
    else
    {
        //! to stop mode
        pMrq->lightCouplingZero( tpvRegion(subAx,region.page()),
                                 mHandZeroTime,
                                 bCcw ? (-mHandZeroAngle) : ( mHandZeroAngle ),
                                 bCcw ? (-mZeroSpeed) : ( mZeroSpeed ),
                                 mHandStopAngle,
                                 mGapTime,
                                 bCcw ? (mGapAngle) : (-mGapAngle),
                                 axes_zero_op_none,
                                 mZeroTmo, mZeroTick );

        sysLog( QString::number( bCcw ? (-mHandZeroAngle) : ( mHandZeroAngle ) ) );
    }

    return 0;
}

int robotSinanju::goZero( const tpvRegion &region,
                    const QList<int> &jointList,
                    const QList<bool> &ccwList )
{
    Q_ASSERT( jointList.size() == ccwList.size() );
    for ( int i = 0; i < jointList.size(); i++ )
    {
        goZero( region, jointList.at(i), ccwList.at(i) );
    }

    return 0;
}

//! en, comment, t, x, y, z, h, attr, comment,speed
int robotSinanju::routeTo(const tpvRegion &region,

                          QVariantList &vars)
{

    if ( vars.size() != 10 )
    { return -1; }

    QVariantList *pVars = new QVariantList();
    *pVars = vars;

    post_set( (VRobot::apiTaskRequest)(&robotSinanju::on_route_to),
              pVars,
              region,
              0,0
              );
    return 0;
}

//! route to
void robotSinanju::on_route_to( void *pArg )
{
    //! check arg
    RoboTaskArgument *pRoboTask = (RoboTaskArgument*)pArg;

    if ( pRoboTask == NULL )
    { return; }

    QVariantList *pVars;
    pVars = pRoboTask->m_pVarList;
    if ( NULL == pVars )
    { return; }

    if ( pVars->size() != (9 + 1) )
    { return; }

    bool bOk;
    double x,y,z;
    x = pVars->at(3).toDouble( &bOk );
    if ( !bOk ){ return; }
    y = pVars->at(4).toDouble( &bOk );
    if ( !bOk ){ return; }
    z = pVars->at(5).toDouble( &bOk );
    if ( !bOk ){ return; }

    double speed;
    speed = pVars->at(9).toDouble( &bOk );
    if ( !bOk ){ return; }

    //! check
    //! guess the time by speed
    TraceKeyPoint pose;
    int ret = nowPose( pose );
    float guessT = comAssist::eulcidenTime( pose.x, pose.y, pose.z,
                                            x,y,z,
                                            speed );

    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint pt2( guessT, x, y, z, 0 );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    move( curve, pRoboTask->mRegion );
}

int robotSinanju::post_set( VRobot::apiTaskRequest api,
                            QVariantList *pVars,
                            const tpvRegion &region,
                            int tmo, int tick )
{
    //! post do
    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();
    if ( NULL == pReq )
    { return -1; }

    RoboTaskArgument *pArg;
    pArg = new RoboTaskArgument();
    if ( NULL == pArg )
    { return -1; }

    //! attach

    //! fill
    pArg->mTmo = tmo;
    pArg->mTick = tick;
    pArg->mRegion = region;

    if ( NULL != pVars )
    { pArg->attachVars( pVars ); }

    //! request
    pReq->request( this,
                   api,
                   pArg );

    //! start
    Q_ASSERT( NULL != m_pRoboTask );
    m_pRoboTask->attachRequest( pReq );

    return 0;
}

bool robotSinanju::checkZeroValid()
{
    //! device
    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( 0, &subAx );
    if ( NULL == pMrq )
    { return false; }

    return pMrq->getEncoderZeroValid();
}
float robotSinanju::getZero( int jointTabId )
{
    //! device
    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( jointTabId, &subAx );
    Q_ASSERT( NULL != pMrq );

    return pMrq->getEncoderZero( subAx );
}
int  robotSinanju::setZero( int jointTabId, float zero )
{
    //! device
    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( jointTabId, &subAx );
    Q_ASSERT( NULL != pMrq );

    return pMrq->setEncoderZero( subAx, zero );
}

int robotSinanju::getMechanicalVersion()
{
    //! device
    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( 0, &subAx );
    if( NULL == pMrq )
    { return 0; }
    else
    {}

    return pMrq->mSYSTEM_REVMOTION;
}

int robotSinanju::align( const tpvRegion &region )
{
    //! angle1 + angle2 + angle3 = 0
    float angles[4], dAngles[4];

    if ( nowAngle( angles ) != 0 )
    { return -1; }

    //! delta angles
    for ( int i = 1; i < 4; i++ )
    {
        dAngles[i] = toDeltaAngle( i, angles[i] );
    }

    //! dstAngle
    float angle3 = -( dAngles[1] + dAngles[2] );
    float dAngle3 = dAngles[3] - angle3;
    logDbg()<<dAngles[1]<<dAngles[2]<<angle3<<dAngle3;

    dAngle3 = comAssist::normalizeDegreeN180_180( dAngle3 );

    //! device
    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( region.axes(), &subAx );
    Q_ASSERT( NULL != pMrq );

    tpvRegion localRegion;
    localRegion = region;
    localRegion.setAx( subAx );

    //! now rotate
    if ( mAngleDir.at( region.axes() ) )
    { pMrq->rotate( localRegion, qAbs(dAngle3)/mZeroSpeed,dAngle3  ); }
    else
    { pMrq->rotate( localRegion, qAbs(dAngle3)/mZeroSpeed ,-dAngle3  ); }

    return 0;
}

int robotSinanju::getPOSE( float pos[] )
{
    int ret;

    TraceKeyPoint nowP;
    ret = nowPose( nowP );
    if ( ret != 0 )
    { return ret; }

    pos[0] = nowP.x;
    pos[1] = nowP.y;
    pos[2] = nowP.z;

    return 0;
}

float robotSinanju::toDeltaAngle(int jointId, float angle)
{
    Q_ASSERT( jointId >= 0 && jointId < 4 );

    float dAngle = ( angle - mInitAngles[jointId] ) * ( mAngleDir.at( jointId ) ? 1 : -1 );

    dAngle = comAssist::normalizeDegreeN180_180( dAngle );

    return dAngle;
}

float robotSinanju::deduceAngle( float fNow, float fAim, float fSep, bool dir )
{
    //! normalize to [0~360)
    float deltaAngle;
    deltaAngle = comAssist::normalizeDegree360( fAim - fNow );
logDbg()<<deltaAngle<<fAim<<fNow;
    //! select the other
    if ( deltaAngle > fSep )
    { deltaAngle = deltaAngle - 360; }
    else
    {  }

    deltaAngle *= dir ? 1 : -1;
logDbg()<<deltaAngle;
    return deltaAngle;
}

int robotSinanju::zeroAxesTask( void *pArg )
{
    int ret;

    roboSinanjuTaskArgument *pZArg = (roboSinanjuTaskArgument*)pArg;

    //! to zero
    onLine();

    beginZero( pZArg->mRegion );
    do
    {
        ret = toAim( pZArg->mRegion, pZArg->mAimAngles, pZArg->mSepAngles );
        if ( ret != 0 )
        { break; }

        ret = waitFsm( pZArg->mRegion, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( 0 != ret )
        { break; }
    }while( 0 );
    endZero( pZArg->mRegion );
    if ( ret != 0 )
    { return ret; }

    //! to init
    if ( mbHandAble )
    {
        onLine();

        ret = toAimd( pZArg->mRegion, pZArg->mAimAngles, pZArg->mSepAngles );
        if ( ret != 0 )
        { return ret; }

        ret = waitFsm( pZArg->mRegion, MegaDevice::mrq_state_idle, pZArg->mTmo, pZArg->mTick );
        if ( ret != 0 )
        { return ret; }
    }

    return ret;
}

int robotSinanju::toAimSession( const tpvRegion &region,
                                const QList<double> &aimAngles,
                                const QList<double> &sepAngles )
{
    //! new
    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();
    Q_ASSERT( NULL != pReq );

    roboSinanjuTaskArgument *pArg;
    pArg = new roboSinanjuTaskArgument();
    Q_ASSERT( NULL != pArg );

    //! fill
    pArg->mTmo = mZeroTmo;
    pArg->mTick = mZeroTick;
    pArg->mRegion = region;

    pArg->mAimAngles = aimAngles;
    pArg->mSepAngles = sepAngles;


    //! request
    pReq->request( this,
                   (VRobot::apiTaskRequest)(&robotSinanju::zeroAxesTask),
                   pArg );

    //! start
    Q_ASSERT( NULL != m_pRoboTask );
    m_pRoboTask->attachRequest( pReq );

    return 0;
}

int robotSinanju::goX( const tpvRegion &region,
                       const QList<double> &aimAngles,
                       const QList<double> &sepAngles,
                       float handT, float handP, float handV )
{
    //! get
    float angles[4];
    if ( nowAngle( angles ) != 0 )
    { return -1; }

    float distAngles[4];

    Q_ASSERT( aimAngles.size() >= 4 );

    //! deduce the angle
    for ( int i = 0; i < 4; i++ )
    {
        distAngles[i] = deduceAngle( angles[i], aimAngles.at(i), sepAngles.at(i), mAngleDir.at( i ) );
    }

    float maxAngle;
    maxAngle = qAbs( distAngles[0] );
    for ( int i = 1; i < 4; i++ )
    {
        if ( qAbs(distAngles[i]) > maxAngle )
        { maxAngle = qAbs(distAngles[i]); }
    }

    //! guess the speed
    if ( maxAngle < 0.001 )
    { return 0; }

    float dT = maxAngle / mZeroSpeed;

    delete_all( mJointsGroup );
    tpvGroup *pGroup;
    for ( int i = 0; i < 4; i++ )
    {
        pGroup = new tpvGroup();
        if ( NULL == pGroup )
        { return -1; }

        pGroup->addItem( 0, 0, 0 );
        pGroup->addItem( dT, distAngles[i], 0 );

        mJointsGroup.append( pGroup );
    }

    //! for hand
    pGroup = new tpvGroup();
    if ( NULL == pGroup )
    { return -1; }
    pGroup->addItem( 0,0,0 );
    pGroup->addItem( handT, handP, handV );

    mJointsGroup.append( pGroup );

    //! for download
    run( region );

    setLoop( 1, region );

    //! download
    int ret = downloadTrace( region, mJointsGroup );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotSinanju::toAim( const tpvRegion &region,
                         const QList<double> &aimAngles,
                         const QList<double> &sepAngles )
{
    int ret;

    float handT, handP, handV;

    if ( mbHandAble )
    {
        if ( mJointZeroCcw.at( 4) )
        {
            handT = mHandZeroTime;
            handP = -mHandZeroAngle;
            handV = -mZeroSpeed;
        }
        else
        {
            handT = mHandZeroTime;
            handP = mHandZeroAngle;
            handV = mZeroSpeed;
        }
    }
    else
    {
        handT = mGapTime;
        handP = 0;
        handV = 0;
    }

    ret = goX( region,
               aimAngles,
               sepAngles,
               handT, handP, handV );

    return ret;
}

int robotSinanju::toAimd( const tpvRegion &region,
                          const QList<double> &aimAngles,
                          const QList<double> &sepAngles
                           )
{
    int ret;

    float handT, handP, handV;

    if ( mbHandAble )
    {
        if ( mJointZeroCcw.at( 4) )
        {
            handT = mGapTime;
            handP = mGapAngle;
            handV = 0;
        }
        else
        {
            handT = mGapTime;
            handP = -mGapAngle;
            handV = 0;
        }
    }
    else
    {
        handT = mGapTime;
        handP = 0;
        handV = 0;
    }

    ret = goX( region,
               aimAngles,
               sepAngles,
               handT, handP, handV );

    return ret;
}

//! region stop distance
void robotSinanju::beginZero( tpvRegion &region )
{
    if ( mHandStopAngle != 0 )
    {}
    else
    { return; }

    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( 4, &subAx );
    if ( NULL == pMrq )
    { return; }

    if ( mbHandAble )
    {
        pMrq->setMOTIONPLAN_STOPMODE( 4,
                                      (MRQ_MOTION_SWITCH_1)region.page(),
                                      MRQ_MOTIONPLAN_STOPMODE_1_DISTANCE );
        pMrq->setMOTIONPLAN_STOPDISTANCE( 4,
                                          (MRQ_MOTION_SWITCH_1)region.page(),
                                          mHandStopAngle );
    }
}
void robotSinanju::endZero( tpvRegion &region )
{
    if ( mHandStopAngle != 0 )
    {}
    else
    { return; }

    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( 4, &subAx );
    if ( NULL == pMrq )
    { return; }

    pMrq->setMOTIONPLAN_STOPMODE( 4,
                                  (MRQ_MOTION_SWITCH_1)region.page(),
                                  MRQ_MOTIONPLAN_STOPMODE_1_IMMEDIATE );
}

