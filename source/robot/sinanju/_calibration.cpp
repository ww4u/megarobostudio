#include "sinanju.h"
#include "../../com/comassist.h"

#define SPACE_RESOLUTION    2     //! mm
#define SPACE_VELOCITY      5     //! mm/s

int robotSinanju::goZero()
{
    //! get
    float angles[4];
    if ( nowAngle( angles ) != 0 )
    { return -1; }

    float distAngles[4];

    for ( int i = 0; i < 4; i++ )
    {
        distAngles[i] = comAssist::diffAngle(
                                              angles[i],
                                              mInitAngles.at(i),
                                              mAngleDir.at(i) );

        logDbg()<<distAngles[i];
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
    pGroup->addItem( dT,0,0 );
    mJointsGroup.append( pGroup );

    //! for download
    tpvRegion region( 0, 0 );
    run( region );

    setLoop( 1 );

    //! download
    int ret = downloadTrace( region, mJointsGroup );
    if ( ret != 0 )
    { return ret; }

    TraceKeyPoint nowP;
    ret = nowPose( nowP );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotSinanju::goZero( int jointId, bool bCcw )
{
    Q_ASSERT( jointId >= 0 && jointId < axes() );

    //! device
    MegaDevice::deviceMRQ *pMrq;
    int subAx;

    pMrq = jointDevice( jointId, &subAx );
    if ( NULL == pMrq )
    { return -1; }

    setLoop( 1 );

    //! jointid
    if ( jointId >= 0 && jointId <= 3 )
    {
        //! angle zero
        double angleZero = mInitAngles.at( jointId );
        float angleNow = pMrq->getAbsAngle( subAx );

//        float deltaAngle = angleZero - angleNow;        //! distance

//        //! +
//        if ( mAngleDir.at(jointId) )
//        {}
//        else
//        { deltaAngle = -deltaAngle; }

        float deltaAngle;
        deltaAngle = comAssist::diffAngle( angleNow, angleZero, mAngleDir.at(jointId) );

//        if ( bCcw )
//        { deltaAngle = comAssist::normalizeDegreeN360(deltaAngle); }
//        else
//        { deltaAngle = comAssist::normalizeDegree360(deltaAngle); }

        sysLog( QString::number(deltaAngle) );
        Q_ASSERT( mZeroSpeed > 0 );
        pMrq->rotate( tpvRegion(subAx,0),
                      qAbs(deltaAngle) / mZeroSpeed,
                      deltaAngle
                      );
    }
    //! hand
    else
    {
        //! to stop mode
        pMrq->lightCouplingZero( tpvRegion(subAx,0),
                                 mHandZeroTime,
                                 bCcw ? (-mHandZeroAngle) : ( mHandZeroAngle ),
                                 bCcw ? (-mZeroSpeed) : ( mZeroSpeed )
                                  );

        sysLog( QString::number( bCcw ? (-mHandZeroAngle) : ( mHandZeroAngle ) ) );
    }

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
