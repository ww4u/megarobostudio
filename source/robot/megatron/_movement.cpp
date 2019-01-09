#include "megatron.h"

int robotMegatron::call( int n, const tpvRegion &region )
{
    onLine();

    setLoop( n, region );

    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_call), region );

    return 0;
}

int robotMegatron::program( QList<MegatronKeyPoint> &curve,
             const tpvRegion &region )
{
    int ret;

    //! 0. check
    //!

    //! 1.build
    QList<int> secList;
    ret = buildTrace( curve, mJointsGroup, secList );
    if ( ret != 0 )
    { return ret; }

    //! 2.config
    setLoop( 1, region );

    //! 3.download
    ret = downloadTrace( region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotMegatron::move( QList<MegatronKeyPoint> &curve,
                         const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    ret = preMove( curve, region );

    return ret;
}

int robotMegatron::preMove( QList<MegatronKeyPoint> &curve,
                         const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    setLoop( 1, region );

    //! program
    ret = program( curve, region );

    return ret;
}

int robotMegatron::moveTest1()
{
    MegatronKeyPoint pt1,pt2;
    pt1.t = 0;
    pt1.fx = 0;
    pt1.ly = 0;
    pt1.fz = 0;
    pt1.bx = 0;
    pt1.ry = 0;
    pt1.bz = 0;

    pt2.t = 1;
    pt2.fx = 10;
    pt2.ly = 10;
    pt2.fz = 10;
    pt2.bx = 20;
    pt2.ry = 20;
    pt2.bz = 20;

    QList<MegatronKeyPoint> curve;
    curve.append( pt1 );
    curve.append( pt2 );

    return move( curve, tpvRegion(0,0) );
}

int robotMegatron::moveTest2()
{
    MegatronKeyPoint pt1,pt2;
    pt1.t = 1;
    pt1.fx = 0;
    pt1.ly = 0;
    pt1.fz = 0;
    pt1.bx = 0;
    pt1.ry = 0;
    pt1.bz = 0;

    pt2.t = 0;
    pt2.fx = 10;
    pt2.ly = 10;
    pt2.fz = 10;
    pt2.bx = 20;
    pt2.ry = 20;
    pt2.bz = 20;

    QList<MegatronKeyPoint> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, tpvRegion(0,0) );
}

