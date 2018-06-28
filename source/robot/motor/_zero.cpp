
#include "motor.h"

int robotMotor::goZero( const tpvRegion &region,
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

    MotorArg *pArg;
    pArg = new MotorArg();
    pArg->mRegion = region;

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();

    int zDir = bCcw ? (-1) : (1);

    //! only one axes
    pArg->mAx = 0;
    pArg->mZeroDist = zDir * mZeroDistance;
    pArg->mZeroTime = mZeroTime;
    pArg->mZeroEndV = zDir *mZeroSpeed;

    pArg->mZeroGapDist = -1 * zDir * mGapDistance;
    pArg->mZeroGapTime = mGapTime;

    //! time
    pArg->mTick = mZeroTick;
    pArg->mTmo = mZeroTmo;

    pReq->request( this, (VRobot::apiTaskRequest)(this->zeroAxesTask), pArg );

    m_pRoboTask->setRequest( pReq );
    m_pRoboTask->start();

    return 0;
}

int robotMotor::goZero( const tpvRegion &region )
{
    QList<int> jList;
    jList<<0;

    return goZero( region, jList, mJointZeroCcw );
}

int robotMotor::goZero( const tpvRegion &region,
                    const QList<int> &jointList,
                    const QList<bool> &ccwList )
{
    return goZero( region, 0, ccwList.at(0) );
}

int robotMotor::zeroAxesTask( void *pZeroArg )
{
    Q_ASSERT( NULL != pZeroArg );

    tpvRegion region;
    int ret;

    MotorArg *pArg = (MotorArg*)pZeroArg;
    region = pArg->mRegion;

    MegaDevice::deviceMRQ *pMrq;
    int ax;
    pMrq = jointDevice( 0, &ax );
    Q_ASSERT( NULL != pMrq );

    //! mrq
    pMrq->setTRIGGER_LEVELSTATE( ax,
                                 (MRQ_TRIGGER_LEVELSTATE)mPosIndex,
                                 (MRQ_CAN_NETMANAGELED)mbZeroPosEn );

    ret = move( pArg->mZeroDist, pArg->mZeroTime, pArg->mZeroEndV, region  );

    //! wait for idle
    ret = waitFsm( region, MegaDevice::mrq_state_idle, pArg->mTmo, pArg->mTick );
    if ( ret != 0 )
    { return ret; }

    //! invert
    pMrq->setTRIGGER_LEVELSTATE( ax,
                                 (MRQ_TRIGGER_LEVELSTATE)mPosIndex,
                                 (MRQ_CAN_NETMANAGELED)mbGapPosEn );

    ret = move( pArg->mZeroGapDist, pArg->mZeroGapTime, 0, region  );

    ret = pMrq->waitFsm( region, MegaDevice::mrq_state_idle, pArg->mTmo, pArg->mTick );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

void robotMotor::setZeroAttr( double zeroTime,
                              double zeroDist,
                              int posIndex,
                              bool bPosEn )
{
    mZeroTime = zeroTime;
    mZeroDistance = zeroDist;

    mPosIndex = posIndex;
    mbZeroPosEn = bPosEn;
}
void robotMotor::zeroAttr( double &zeroTime,
                           double &zeroDist,
                           int &posIndex,
                           bool &bPosEn )
{
    zeroTime = mZeroTime;
    zeroDist = mZeroDistance;

    posIndex = mPosIndex;
    bPosEn = mbZeroPosEn;
}

void robotMotor::setGap( double time,
                         double dist,
                         bool bPosEn )
{
    mGapTime = time;
    mGapDistance = dist;

    mbGapPosEn = bPosEn;
}
void robotMotor::gap( double &time,
                      double &dist,
                      bool &bPosEn )
{
    time = mGapTime;
    dist = mGapDistance;

    bPosEn = mbGapPosEn;


}
