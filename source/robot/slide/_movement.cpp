#include "slide1.h"

int robotSlide1::program( QList<D1Point> &curve,
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

int robotSlide1::move( QList<D1Point> &curve,
                         const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    //! program
    ret = program( curve, region );

    return ret;
}

int robotSlide1::moveTest1( const tpvRegion &region )
{
    D1Point pt1,pt2;
    pt1.t = 0;
    pt1.p = 0;

    pt2.t = 1;
    pt2.p = 20;

    QList<D1Point> curve;
    curve.append( pt1 );
    curve.append( pt2 );

    return move( curve, region );
}

int robotSlide1::moveTest2( const tpvRegion &region )
{
    D1Point pt1,pt2;
    pt1.t = 1;
    pt1.p = 0;

    pt2.t = 0;
    pt2.p = 20;

    //! p2 -> p1
    QList<D1Point> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, region );
}
