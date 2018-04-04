#include "delta.h"

int robotDelta::program( QList<TraceKeyPoint> &curve,
             const tpvRegion &region )
{
//    int ret;

//    //! 0. check
//    //!

//    //! 1.build
//    xxxGroup<jointsTrace> jointsPlan;
//    ret = buildTrace( curve, jointsPlan );
//    if ( ret != 0 )
//    { return ret; }

//    //! 2.convert
//    ret = convertTrace( curve, jointsPlan );
//    if ( ret != 0 )
//    { return ret; }

//    //! 3.config
//    setLoop( 1 );

//    //! 4.download
//    ret = downloadTrace( region );
//    if ( ret != 0 )
//    { return ret; }

    return 0;
}

int robotDelta::move( QList<TraceKeyPoint> &curve,
                        const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    //! program
    ret = program( curve, region );

    return ret;
}

int robotDelta::moveTest1( const tpvRegion &region )
{
    TraceKeyPoint pt1,pt2;
    pt1.t = 0;
    pt1.x = 280;
    pt1.y = 21.5;
    pt1.z = 452.75;
    pt1.hand = 0;

    pt2.t = 1;
    pt2.x = 250;
    pt2.y = 0;
    pt2.z = 502;
    pt2.hand = 270;

    QList<TraceKeyPoint> curve;
    curve.append( pt1 );
    curve.append( pt2 );
//    return moveTest( pt1, pt2, dt );
// MOVE 280,21.5,452.75,0,250,0,502,90,1
// MOVE 250,0,502,90,280,21.5,452.75,0,1
    return move( curve, region );
}

int robotDelta::moveTest2( const tpvRegion &region )
{
    TraceKeyPoint pt1,pt2;
    pt1.t = 1;
    pt1.x = 280;
    pt1.y = 21.5;
    pt1.z = 452.75;
    pt1.hand = 0;

    pt2.t = 0;
    pt2.x = 250;
    pt2.y = 0;
    pt2.z = 502;
    pt2.hand = 270;

    //! p2 -> p1
    QList<TraceKeyPoint> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, region );
}
