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

int robotDelta::goZero()
{
    QList<int> jList;
    jList<<0<<1<<2<<3;

    return goZero( jList, mJointZeroCcw );
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

    setLoop( 1 );

    int ret;
    if ( jTabId == 0 || jTabId == 1 ) //! l,r
    {
        //! get v
        float initV;
        initV = jTabId == 0 ? mInitL : mInitR;

        //! zero && init
        pMrq->lightCouplingZero( tpvRegion(subAx,0),
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
        pMrq->lightCouplingZero( tpvRegion(subAx,0),
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
        pMrq->lightCouplingZero( tpvRegion(subAx,0),
                                 mZeroTime,
                                 bCcw ? (-mZeroAngle) : ( mZeroAngle ),
                                 0
                                 );
    }
    else
    { Q_ASSERT(false); }

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
