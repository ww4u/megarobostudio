#include "delta.h"

#include "../../arith/delta/arith_delta.h"

int robotDelta::verifyTrace( QList<TraceKeyPoint> &curve )
{
    //! check size
    if ( curve.size() < 2 )
    {
        sysError( QObject::tr("Invalid data length: less than 2") );
        return -1;
    }

    //! check t
    float t;
    t = curve.at(0).t;
    for ( int i = 1; i < curve.size(); i++ )
    {
        if ( t >= curve.at(i).t )
        {
            sysError( QObject::tr("Invalid time at line:") + QString::number(i) );
            return -1;
        }

        t = curve.at(i).t;
    }

    return 0;
}

int robotDelta::buildTrace( QList<TraceKeyPoint> &curve,
                QList<deltaPoint> &jointsPlan )
{
    int ret;

    xxxGroup<tracePoint> tracePlan;logDbg()<<curve.size();
    ret = planTrace( curve, tracePlan );
    if ( ret != 0 )
    { return ret; }
logDbg()<<tracePlan.size();
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
    int ret = ns_pathplan::GetPointLen( &endPoints.data()->datas,
                                      curve.size(),
                                      mPlanAttr.mStep,
                                      mPlanAttr.mMode,
                                      &xyzResLen );
    if ( ret != 0 )
    { return ERR_PLAN_FAIL; }
logDbg()<<curve.size()<<xyzResLen<<mPlanAttr.mStep;
    int traceSize;
    traceSize = xyzResLen * sizeof(double) / sizeof(tracePoint);
    if ( traceSize > 1 )
    { }
    else
    { return ERR_PLAN_FAIL; }

    tracePlan.clear();
    if ( 0 != tracePlan.alloc( traceSize ) )
    { return ERR_ALLOC_FAIL; }

    ret = ns_pathplan::GetPointInfo( &tracePlan.data()->datas, xyzResLen );
    if ( ret != 0 )
    { return ERR_PLAN_FAIL; }

    return 0;
}

int robotDelta::splitTrace( xxxGroup<tracePoint> &tracePoints,
                            QList<deltaPoint> &traceJoints )
{
    QList<D4Point> points;

    D4Point d4Pt;
    for ( int i = 0; i < tracePoints.size(); i++ )
    {
        d4Pt.t = tracePoints.data()[i].t;
        d4Pt.x = tracePoints.data()[i].x;
        d4Pt.y = tracePoints.data()[i].y;
        d4Pt.z = tracePoints.data()[i].z;

        d4Pt.vx = tracePoints.data()[i].vx;
        d4Pt.vy = tracePoints.data()[i].vy;
        d4Pt.vz = tracePoints.data()[i].vz;

        points.append( d4Pt );

//        logDbg()<<d4Pt.t<<d4Pt.x<<d4Pt.y<<d4Pt.z<<d4Pt.vx<<d4Pt.vy<<d4Pt.vz;
    }

    int ret;
logDbg();
    ret = arith_delta::ccwSlove( points, traceJoints );
logDbg();
    return ret;
}

int robotDelta::convertTrace(  QList<TraceKeyPoint> &curve,
                               QList<deltaPoint> &jointsPlan,
                               QList< tpvGroup *> &gp,
                               QList< int > &sectionList )
{
    //! 0.check
    if( jointsPlan.size() > 0 )
    {}
    else
    { return ERR_NO_TPV_DATA; }

    int ret;
    delete_all( gp );

    //! 0~1
    ret = buildTpvGroup( curve, jointsPlan, gp );
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

    gp.append( pGroup );

    //! sections
    //! x,z,y,h
    sectionList<<0<<2<<2<<2;

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

int robotDelta::buildTpvGroup( QList<TraceKeyPoint> &curve,
                               QList<deltaPoint> &jointsPlan,
                               QList< tpvGroup *> &jointsGroup )
{
    //! create each joints group
    delete_all( jointsGroup );

    int ret;
    //! for x,z
    for ( int i = 0; i < 2; i++ )
    {
        tpvGroup *pGroup = new tpvGroup();
        Q_ASSERT( NULL != pGroup );
logDbg()<<jointsPlan.size();
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

    //! for y
    tpvGroup *pGroup = new tpvGroup();
    Q_ASSERT( NULL != pGroup );

    for ( int i = 0; i < curve.size(); i++ )
    {
        ret = pGroup->addItem( curve.at(i).t,
                               curve.at(i).y,
                               0 );
        if ( ret != 0 )
        {
            delete_all( jointsGroup );
            return ret;
        }
    }
    jointsGroup.append( pGroup );

    return 0;
}
