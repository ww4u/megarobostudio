#include "delta.h"
#include "../../com/comassist.h"

void robotDelta::setZeroAttr( double zeroTime, double zeroAngle )
{
    mZeroTime = zeroTime;
    mZeroAngle = zeroAngle;

}
void robotDelta::zeroAttr( double &zeroTime, double &zeroAngle )
{
    zeroTime = mZeroTime;
    zeroAngle = mZeroAngle;
}

void robotDelta::setInitAttr( double t, double l, double r, double y, double h )
{
    mInitT = t;
    mInitL = l;
    mInitR = r;
    mInitY = y;
    mInitH = h;
}
void robotDelta::initAttr( double &t, double &l, double &r, double &y, double &h )
{
    t = mInitT;
    l = mInitL;
    r = mInitR;
    y = mInitY;
    h = mInitH;
}

int robotDelta::goZero( const tpvRegion &region )
{
    QList<int> jList;
    jList<<0<<1<<2<<3;

    return goZero( region, jList, mJointZeroCcw );
}

int robotDelta::goZero( const tpvRegion &region,
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

    setLoop( 1 );

    int ret;
    if ( jTabId == 0 || jTabId == 1 ) //! l,r
    {
        //! get v
        float initV;
        initV = jTabId == 0 ? mInitL : mInitR;

        //! zero && init
        pMrq->lightCouplingZero( tpvRegion(subAx,region.page()),
                                 mZeroTime,
                                 bCcw ? (-mZeroAngle) : ( mZeroAngle ),
                                 bCcw ? (-mZeroSpeed) : ( mZeroSpeed ),
                                 mInitT,
                                 bCcw ? ( initV ) : (-initV),
                                 mZeroTmo,
                                 mZeroTick
                                 );

    }
    else if ( jTabId == 2 )
    {
        pMrq->lightCouplingZero( tpvRegion(subAx,region.page() ),
                                 mZeroTime,
                                 bCcw ? (-mZeroAngle) : ( mZeroAngle ),
                                 bCcw ? (-mZeroSpeed) : ( mZeroSpeed ),
                                 mInitT,
                                 bCcw ? (mInitY) : (-mInitY),
                                 mZeroTmo,
                                 mZeroTick
                                 );
    }
    else if ( jTabId == 3 )
    {
        //! \note no init op
        pMrq->lightCouplingZero( tpvRegion(subAx,region.page() ),
                                 mZeroTime,
                                 bCcw ? (-mZeroAngle) : ( mZeroAngle ),
                                 0
                                 );
    }
    else
    { Q_ASSERT(false); }

    return 0;
}

int robotDelta::goZero( const tpvRegion &region,
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

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();
    Q_ASSERT( NULL != pReq );

    DeltaZeroArg *pArg;
    pArg = new DeltaZeroArg();
    Q_ASSERT( NULL != pArg );

    pArg->mRegion = region;
    pArg->mJList = jointList;
    pArg->mCcwList = ccwList;

    //! init para list
    for ( int i = 0; i <jointList.size(); i++ )
    {
        if ( jointList.at(i) == 0 )
        {
            pArg->mZDistList<<mZeroAngle;
            pArg->mZVList<<mZeroSpeed;
            pArg->mGapDistList<<mInitL;
        }
        else if ( jointList.at(i) == 1 )
        {
            pArg->mZDistList<<mZeroAngle;
            pArg->mZVList<<mZeroSpeed;
            pArg->mGapDistList<<mInitR;
        }
        else if ( jointList.at(i) == 2 )
        {
            pArg->mZDistList<<mZeroAngle;
            pArg->mZVList<<mZeroSpeed;
            pArg->mGapDistList<<mInitY;
        }
        else if ( jointList.at(i) == 3 )
        {
            pArg->mZDistList<<mZeroAngle;
            pArg->mZVList<<0;
            pArg->mGapDistList<<0;
        }
        else
        { Q_ASSERT(false); }
    }

    //! request
    //! \todo
//    pReq->request( this, (VRobot::apiTaskRequest)(this->zeroAxesTask), pArg );

    //! start
    Q_ASSERT( NULL != m_pRoboTask );
    m_pRoboTask->setRequest( pReq );
    m_pRoboTask->start();

    return 0;
}

int robotDelta::zeroAxesTask( void *pArg )
{
    Q_ASSERT( NULL != pArg );
    int ret = 0;
    DeltaZeroArg *pZArg = (DeltaZeroArg*)pArg;

    //! to zero
    {
        onLine();

        fsm( pZArg->mRegion )->setState( MegaDevice::mrq_state_calcend );

        //! foreach axes
        for ( int i = 0; i < pZArg->mJList.size(); i++ )
        {
            ret = jMove( pZArg->mRegion,
                   pZArg->mJList[i], pZArg->mCcwList[i],
                   mZeroTime,
                   pZArg->mZDistList[i],
                   pZArg->mZVList[i] );
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

        fsm( pZArg->mRegion )->setState( MegaDevice::mrq_state_calcend );

        //! foreach axes
        for ( int i = 0; i < pZArg->mJList.size(); i++ )
        {
            ret = jMove( pZArg->mRegion,
                   pZArg->mJList[i], !pZArg->mCcwList[i],
                   mInitT,
                   pZArg->mGapDistList[i],
                   0 );
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
