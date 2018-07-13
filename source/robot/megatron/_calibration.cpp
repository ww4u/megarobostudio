#include "megatron.h"

int robotMegatron::zeroAxesTask( void *pArg )
{
    Q_ASSERT( NULL != pArg );
    int ret = 0;
    MegatronZeroArg *pZArg = (MegatronZeroArg*)pArg;

    //! to zero
    {
        onLine();

        fsm(pZArg->mRegion)->setState( MegaDevice::mrq_state_calcend );

        //! foreach axes
        for ( int i = 0; i < pZArg->mJList.size(); i++ )
        {
            ret = jMove( pZArg->mRegion,
                   pZArg->mJList[i], pZArg->mCcwList[i],
                   mZeroTime, mZeroAngle, mZeroSpeed );
            if ( ret != 0 )
            { return ret; }
        }

        //! wait idle
        ret = waitFsm( pZArg->mRegion, MegaDevice::mrq_state_idle, mZeroTmo, mZeroTick );
        if ( ret != 0 )
        { return ret; }
    }

    //! inverse
    {
        onLine();

        fsm(pZArg->mRegion)->setState( MegaDevice::mrq_state_calcend );

        for ( int i = 0; i < pZArg->mJList.size(); i++ )
        {
            ret = jMove( pZArg->mRegion,
                   pZArg->mJList[i], !pZArg->mCcwList[i],
                   mGapTime, mGapDistance, 0 );
            if ( ret != 0 )
            { return ret; }
        }

        //! wait idle
        ret = waitFsm( pZArg->mRegion, MegaDevice::mrq_state_idle, mZeroTmo, mZeroTick );
        if ( ret != 0 )
        { return ret; }
    }

    return 0;
}

void robotMegatron::setZeroAttr( double zeroTime, double zeroAngle )
{
    mZeroTime = zeroTime;
    mZeroAngle = zeroAngle;
}
void robotMegatron::zeroAttr( double &zeroTime, double &zeroAngle )
{
    zeroTime = mZeroTime;
    zeroAngle = mZeroAngle;
}

void robotMegatron::setGapAttr( double gapTime, double gapDist )
{
    mGapTime = gapTime;
    mGapDistance = gapDist;

}
void robotMegatron::gapAttr( double &gapTime, double &gapDist )
{
    gapTime = mGapTime;
    gapDist = mGapDistance;
}

int robotMegatron::goZero( const tpvRegion &region )
{
    QList<int> jList;
    QList<bool> ccwList;
    for ( int i = 0; i < axes(); i++ )
    {
        jList<<i;
        ccwList<<true;
    }

    return goZero( region, jList, ccwList );
}

int robotMegatron::goZero( const tpvRegion &region,
                           int jTabId, bool bCcw )
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

    pMrq->lightCouplingZero( tpvRegion(subAx,region.page()),
                             mZeroTime,
                             bCcw ? (-mZeroAngle) : ( mZeroAngle ),
                             bCcw ? (-mZeroSpeed) : ( mZeroSpeed ),
                             mGapTime,
                             bCcw ? (mGapDistance) : (-mGapDistance),
                             mZeroTmo, mZeroTick );

    return 0;
}

int robotMegatron::goZero( const tpvRegion &region,
                           const QList<int> &jointList,
                           const QList<bool> &ccwList )
{
    Q_ASSERT( jointList.size() == ccwList.size() );

//    int ret=0;
//    for ( int i = 0; i < jointList.size(); i++ )
//    {
//        ret = goZero( region, jointList.at(i), ccwList.at(i) );
//        if ( ret != 0 )
//        { return ret; }
//    }

//    return 0;

    //! new
    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();
    Q_ASSERT( NULL != pReq );

    MegatronZeroArg *pArg;
    pArg = new MegatronZeroArg();
    Q_ASSERT( NULL != pArg );

    //! fill
    pArg->mRegion = region;
    pArg->mJList = jointList;
    pArg->mCcwList = ccwList;

    //! request
    //! \todo
    pReq->request( this,
                   (VRobot::apiTaskRequest)(&robotMegatron::zeroAxesTask),
                   pArg );

    //! start
    Q_ASSERT( NULL != m_pRoboTask );
    m_pRoboTask->setRequest( pReq );
    m_pRoboTask->start();

    return 0;
}
