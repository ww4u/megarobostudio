#include "h2.h"

int robotH2::program( QList<H2KeyPoint> &curve,
             const tpvRegion &region )
{
    int ret;

    //! 0. check
    //!

    //! 1.build
    ret = buildTrace( curve );
    if ( ret != 0 )
    { return ret; }

    //! 2.config
    setLoop( 1 );

    //! 3.download
    ret = downloadTrace( region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotH2::move( QList<H2KeyPoint> &curve,
                         const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    //! program
    ret = program( curve, region );

    return ret;
}

int robotH2::moveTest1( const tpvRegion &region )
{
    H2KeyPoint pt1,pt2;
    pt1.t = 0;
    pt1.x = 0;
    pt1.y = 0;
    pt1.z = 0;

    pt2.t = 1;
    pt2.x = 20;
    pt2.y = 20;
    pt2.z = 20;

    QList<H2KeyPoint> curve;
    curve.append( pt1 );
    curve.append( pt2 );

    return move( curve, region );
}

int robotH2::moveTest2( const tpvRegion &region )
{
    H2KeyPoint pt1,pt2;
    pt1.t = 1;
    pt1.x = 0;
    pt1.y = 0;
    pt1.z = 0;

    pt2.t = 0;
    pt2.x = 20;
    pt2.y = 20;
    pt2.z = 20;

    //! p2 -> p1
    QList<H2KeyPoint> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, region );
}

