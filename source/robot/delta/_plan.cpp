#include "delta.h"

#include "../../arith/delta/arith_delta.h"

int robotDelta::buildTrace( QList<TraceKeyPoint> &curve,
                QList<arith_delta::deltaPoint> &jointsPlan )
{
    int ret;

    xxxGroup<tracePoint> tracePlan;
    ret = planTrace( curve, tracePlan );
    if ( ret != 0 )
    { return ret; }

    ret = splitTrace( tracePlan, jointsPlan );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

//! interp
int robotDelta::planTrace( QList<TraceKeyPoint> &curve,
                           xxxGroup<tracePoint> &tracePlan )
{
    xxxGroup<endPoint> endPoints;

    //! alloc
    if ( 0 != endPoints.alloc( curve.size() ) )
    { return ERR_ALLOC_FAIL; }

    //! fill 0
    memset( endPoints.data(), 0, sizeof(endPoints)*curve.size() );

    //! interp fill
    for ( int i = 0; i < curve.size(); i++ )
    {
        //! need interp
        if ( is_bit1( curve.at(i).iMask, BIT_INTERP) )
        { endPoints.data()[i].flagInterp = 1; }
        else
        { endPoints.data()[i].flagInterp = 0; }
    }

    //! xyzt
    for( int i = 0; i < curve.size(); i++ )
    {
        endPoints.data()[i].x = curve.at( i ).x;
        endPoints.data()[i].y = curve.at( i ).y;
        endPoints.data()[i].z = curve.at( i ).z;
        endPoints.data()[i].t = curve.at( i ).t;
    }

    int xyzResLen;
    int ret = ns_pathplan::GetPvtLen( &endPoints.data()->datas,
                                      curve.size(),
                                      mPlanStep,
                                      mPlanMode,
                                      &xyzResLen );
    if ( ret != 0 )
    { return ERR_PLAN_FAIL; }

    int traceSize;
    traceSize = xyzResLen * sizeof(double) / sizeof(tracePoint);
    if ( traceSize > 1 )
    { }
    else
    { return ERR_PLAN_FAIL; }

    tracePlan.clear();
    if ( 0 != tracePlan.alloc( traceSize ) )
    { return ERR_ALLOC_FAIL; }

    ret = ns_pathplan::GetPvtInfo( &tracePlan.data()->datas, xyzResLen );
    if ( ret != 0 )
    { return ERR_PLAN_FAIL; }

    return 0;
}

int robotDelta::splitTrace( xxxGroup<tracePoint> &tracePoints,
                            QList<arith_delta::deltaPoint> &traceJoints )
{
    QList<D4Point> points;

    D4Point d4Pt;
    for ( int i = 0; i < tracePoints.size(); i++ )
    {
        d4Pt.t = tracePoints.data()[i].t;
        d4Pt.x = tracePoints.data()[i].x;
        d4Pt.y = tracePoints.data()[i].y;
        d4Pt.z = tracePoints.data()[i].z;

        points.append( d4Pt );

        logDbg()<<d4Pt.t<<d4Pt.x<<d4Pt.y<<d4Pt.z;
    }

    int ret;
    ret = arith_delta::slove( points, traceJoints );
    return ret;
}

int robotDelta::convertTrace(  QList<TraceKeyPoint> &curve,
                               QList<arith_delta::deltaPoint> &jointsPlan )
{
    //! 0.check
    if( jointsPlan.size() > 0 )
    {}
    else
    { return ERR_NO_TPV_DATA; }

    int ret;
    delete_all( mJointsGroup );

    //! 0~2
    ret = buildTpvGroup( jointsPlan, mJointsGroup );
    if ( ret != 0 )
    { return ret; }

    //! tail
    //! for hand
    tpvGroup *pGroup = new tpvGroup();
    if ( NULL == pGroup )
    { return ERR_ALLOC_FAIL; }

    for ( int i = 0; i < curve.size(); i++ )
    {
        ret = pGroup->addItem( curve.at(i).t, curve.at(i).hand, 0 );
        if ( ret != 0 )
        {
            delete pGroup;
            return ERR_FAIL_ADD_TPV;
        }
    }

    mJointsGroup.append( pGroup );

//    //! log joint group
//    foreach ( tpvGroup *pGp, mJointsGroup )
//    {
//        logDbg()<<"*******";
//        foreach(  tpvItem *pItem, pGp->mItems )
//        {
//            logDbg()<<pItem->mT<<pItem->mP<<pItem->mV;
//        }
//    }

    return 0;
}

int robotDelta::buildTpvGroup( QList<arith_delta::deltaPoint> &jointsPlan,
                               QList< tpvGroup *> &jointsGroup )
{
    //! create each joints group
    delete_all( jointsGroup );

    int ret;
    //! for each axis
    for ( int i = 0; i < 3; i++ )
    {
        tpvGroup *pGroup = new tpvGroup();
        Q_ASSERT( NULL != pGroup );

        //! for each data
        for ( int j = 0; j < jointsPlan.size(); j++ )
        {
            ret = pGroup->addItem(
                                jointsPlan.at(j).t,
                                jointsPlan.at(j).p[i],
                                jointsPlan.at(j).v[i]
                             );
            if ( ret != 0 )
            {
                delete_all( jointsGroup );
                return ret;
            }
        }

        jointsGroup.append( pGroup );
    }

    return 0;
}
