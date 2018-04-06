#include "delta.h"

void robotDelta::setZeroAttr( double zeroTime, double zeroAngle, double zeroSpeed )
{
    mZeroTime = zeroTime;
    mZeroAngle = zeroAngle;
    mZeroSpeed = zeroSpeed;
}
void robotDelta::zeroAttr( double &zeroTime, double &zeroAngle, double &zeroSpeed )
{
    zeroTime = mZeroTime;
    zeroAngle = mZeroAngle;
    zeroSpeed = mZeroSpeed;
}

int robotDelta::goZero( int jTabId, bool bCcw )
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

/*    //! angle
    if ( jTabId == 0 || jTabId == 1 )
    {
        //! angle zero
        double angleZero = mInitAngles.at( jTabId );
        float angleNow = pMrq->getAbsAngle( subAx );

        float deltaAngle = angleZero - angleNow;        //! distance

        if ( bCcw )
        {  }
        else
        { deltaAngle = -deltaAngle; }

        Q_ASSERT( mZeroSpeed > 0 );
        pMrq->rotate( tpvRegion(subAx,0),
                      qAbs(deltaAngle) / mZeroSpeed,
                      deltaAngle
                      );
    }
    else */
    if ( jTabId >= 0 && jTabId < 4 )
    {
        //! to stop mode
        pMrq->setMOTIONPLAN_ENDSTATE( subAx, MRQ_MOTION_SWITCH_1_MAIN, MRQ_MOTIONPLAN_ENDSTATE_1_STOP );
        pMrq->lightCouplingZero( tpvRegion(subAx,0),
                                 mZeroTime,
                                 bCcw ? (-mZeroAngle) : ( mZeroAngle ),
                                 mZeroSpeed );
    }
    else
    {}

    return 0;
}

int robotDelta::goZero( const QList<int> &jointList,
                    const QList<bool> &ccwList )
{
    Q_ASSERT( jointList.size() == ccwList.size() );

    int ret=0;
    for ( int i = 0; i < jointList.size(); i++ )
    {
        ret = goZero( jointList.at(i), ccwList.at(i) );
        if ( ret != 0 )
        { return ret; }
    }

    return 0;
}
