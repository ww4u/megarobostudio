#include "sinanju.h"
#include "../../com/comassist.h"

#define SPACE_RESOLUTION    2     //! mm
#define SPACE_VELOCITY      5     //! mm/s

int robotSinanju::goZero()
{
    int ret;

    TraceKeyPoint nowP;
    ret = nowPose( nowP );
    if ( ret != 0 )
    { return ret; }

    TraceKeyPoint dstP( 0,
                        mInitPos.at(0),
                        mInitPos.at(1),
                        mInitPos.at(2),
                        0 );

    //! tune the t by distance
    float dist = comAssist::eulcidenDistance( nowP.x, nowP.y, nowP.z,
                                              dstP.x, dstP.y, dstP.z );

    if ( dist < SPACE_RESOLUTION )
    { return 0; }

    QList<TraceKeyPoint> curve;

    Q_ASSERT( mZeroSpeed > 0 );
    dstP.t = dist / mZeroSpeed;

    curve.append( nowP );
    curve.append( dstP );

    move( curve, tpvRegion(0,0) );
    logDbg()<<nowP.x<<nowP.y<<nowP.z<<dist;

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

    //! jointid
    if ( jointId >= 0 && jointId <= 3 )
    {
        //! angle zero
        double angleZero = mInitAngles.at( jointId );
        float angleNow = pMrq->getAbsAngle( subAx );

        float deltaAngle = angleZero - angleNow;        //! distance

        //! +
        if ( mAngleDir.at(jointId) )
        {}
        else
        { deltaAngle = -deltaAngle; }

        if ( bCcw )
        { deltaAngle = comAssist::normalizeDegreeN360(deltaAngle); }
        else
        { deltaAngle = comAssist::normalizeDegree360(deltaAngle); }

        sysLog( QString::number(deltaAngle) );
        pMrq->rotate( tpvRegion(subAx,0),
                      qAbs(deltaAngle) / mZeroSpeed,
                      deltaAngle
                      );

    }
    //! hand
    else
    {
        //! to stop mode
        pMrq->setMOTIONPLAN_ENDSTATE( subAx, MRQ_MOTION_SWITCH_1_MAIN, MRQ_MOTIONPLAN_ENDSTATE_1_STOP );
        pMrq->lightCouplingZero( tpvRegion(subAx,0),
                                 mHandZeroTime,
                                 bCcw ? (-mHandZeroAngle) : ( mHandZeroAngle ),
                                 mHandZeroSpeed );

        sysLog( QString::number( bCcw ? (-mHandZeroAngle) : ( mHandZeroAngle ) ) );
    }

    return 0;
}
