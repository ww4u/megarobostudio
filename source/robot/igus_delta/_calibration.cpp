#include "igus_delta.h"
#include "../../com/comassist.h"

void robotIgusDelta::setZeroAttr( double zeroTime, double zeroAngle, int zMode )
{
    mZeroTime = zeroTime;
    mZeroAngle = zeroAngle;

    mLimitOpt = zMode;

}
void robotIgusDelta::zeroAttr( double &zeroTime, double &zeroAngle, int &zMode )
{
    zeroTime = mZeroTime;
    zeroAngle = mZeroAngle;

    zMode = mLimitOpt;
}

void robotIgusDelta::setInitAttr( double t, double leg, double h )
{
    mInitT = t;
    mInitLeg = leg;
    mInitH = h;
}
void robotIgusDelta::initAttr( double &t, double &leg, double &h )
{
    t = mInitT;
    leg = mInitLeg;
    h = mInitH;
}

int robotIgusDelta::goZero( const tpvRegion &region )
{
    QList<int> jList;
//    jList<<0<<1<<2<<3;
    jList<<0<<1<<2;

    return goZero( region, jList, mJointZeroCcw );
}

int robotIgusDelta::goZero( const tpvRegion &region,
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
    if ( jTabId == 0 || jTabId == 1 || jTabId == 2 ) //! legs
    {        
        AxesZeroOp zOp;

        if ( mLimitOpt == 0 )
        { zOp = combine_zero_op2( axes_zero_clr_cnt, axes_zero_lose_step );}
        else
        { zOp = ( axes_zero_clr_cnt ); }

        //! zero && init
        ret = pMrq->lightCouplingZero( tpvRegion(subAx,region.page()),
                                 mZeroTime,
                                 bCcw ? (-mZeroAngle) : ( mZeroAngle ),
                                 bCcw ? (-mZeroSpeed) : ( mZeroSpeed ),
                                 mInitT,
                                 bCcw ? ( mInitLeg ) : (-mInitLeg),
                                 zOp,
                                 mZeroTmo,
                                 mZeroTick
                                 );

    }
    else if ( jTabId == 3 )
    {
        //! \note no init op
        ret = pMrq->lightCouplingZero( tpvRegion(subAx,region.page() ),
                                 mZeroTime,
                                 bCcw ? (-mZeroAngle) : ( mZeroAngle ),
                                 0
                                 );
    }
    else
    { Q_ASSERT(false); }

    return ret;
}

int robotIgusDelta::goZero( const tpvRegion &region,
                        const QList<int> &jointList,
                        const QList<bool> &ccwList )
{
    Q_ASSERT( jointList.size() == ccwList.size() );

    RoboTaskRequest *pReq;
    pReq = new RoboTaskRequest();
    Q_ASSERT( NULL != pReq );

    IgusDeltaZeroArg *pArg;
    pArg = new IgusDeltaZeroArg();
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
            pArg->mGapDistList<<mInitLeg;
        }
        else if ( jointList.at(i) == 1 )
        {
            pArg->mZDistList<<mZeroAngle;
            pArg->mZVList<<mZeroSpeed;
            pArg->mGapDistList<<mInitLeg;
        }
        else if ( jointList.at(i) == 2 )
        {
            pArg->mZDistList<<mZeroAngle;
            pArg->mZVList<<mZeroSpeed;
            pArg->mGapDistList<<mInitLeg;
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
    pReq->request( this,
                   (VRobot::apiTaskRequest)(&robotIgusDelta::zeroAxesTask),
                   pArg );

    //! start
    Q_ASSERT( NULL != m_pRoboTask );
    m_pRoboTask->setRequest( pReq );
    m_pRoboTask->start();

    return 0;
}

int robotIgusDelta::getPOSE(float pos[])
{
    return pose( pos );
}

int robotIgusDelta::zeroAxesTask( void *pArg )
{
    Q_ASSERT( NULL != pArg );
    int ret = 0;
    IgusDeltaZeroArg *pZArg = (IgusDeltaZeroArg*)pArg;

    //! to zero
    {
        onLine();

        fsm( pZArg->mRegion )->setState( MegaDevice::mrq_state_calcend );

        MegaDevice::deviceMRQ *pMrq;
        int subAx;

        //! lose step on
        if ( mLimitOpt == 0 )
        {
            for( int i = 0; i < pZArg->mJList.size(); i++ )
            {
                pMrq = jointDevice( pZArg->mJList[i], &subAx );
                if ( NULL == pMrq )
                { return -1; }

                pMrq->setMOTIONPLAN_OOSLINESTATE( subAx,
                                                  (MRQ_MOTION_SWITCH_1)pZArg->mRegion.page(),
                                                  MRQ_SYSTEM_REVMOTION_ON );
            }
        }

        ret = onZArg( pArg );

        //! lose step off
        if ( mLimitOpt == 0 )
        {
            for ( int i = 0; i < pZArg->mJList.size(); i++ )
            {
                pMrq = jointDevice( pZArg->mJList[i], &subAx );
                if ( NULL == pMrq )
                { return -1; }

                pMrq->setMOTIONPLAN_OOSLINESTATE( subAx,
                                                  (MRQ_MOTION_SWITCH_1)pZArg->mRegion.page(),
                                                  MRQ_SYSTEM_REVMOTION_OFF );
            }
        }

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

        //! clear the encoder
        int ax;
        MegaDevice::deviceMRQ *pMrq;
        for ( int i = 0; i < pZArg->mJList.size(); i++ )
        {
            pMrq = jointDevice( i, &ax );
            if ( NULL == pMrq )
            { return -1; }

            ret = pMrq->setMOTION_ABCOUNTCLEAR( ax );
            if ( ret != 0 )
            { return ret; }
        }
    }

    return 0;
}

int robotIgusDelta::onZArg( void *pArg )
{
    Q_ASSERT( NULL != pArg );

    int ret;
    IgusDeltaZeroArg *pZArg = (IgusDeltaZeroArg*)pArg;

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

    return 0;
}
