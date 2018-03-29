#include "megatron.h"

int robotMegatron::call( const tpvRegion &region )
{
    onLine();

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

int robotMegatron::move( QList<MegatronKeyPoint> &curve,
                         const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    //! program
    ret = program( curve, region );

    return ret;
}

int robotMegatron::moveTest1()
{
    MegatronKeyPoint pt1,pt2;
    pt1.t = 0;
    pt1.x1 = 0;
    pt1.y1 = 0;
    pt1.z1 = 0;
    pt1.x2 = 0;
    pt1.y2 = 0;
    pt1.z2 = 0;

    pt2.t = 1;
    pt2.x1 = 10;
    pt2.y1 = 10;
    pt2.z1 = 10;
    pt2.x2 = 20;
    pt2.y2 = 20;
    pt2.z2 = 20;

    QList<MegatronKeyPoint> curve;
    curve.append( pt1 );
    curve.append( pt2 );

    return move( curve, tpvRegion(0,0) );
}

int robotMegatron::moveTest2()
{
    MegatronKeyPoint pt1,pt2;
    pt1.t = 1;
    pt1.x1 = 0;
    pt1.y1 = 0;
    pt1.z1 = 0;
    pt1.x2 = 0;
    pt1.y2 = 0;
    pt1.z2 = 0;

    pt2.t = 0;
    pt2.x1 = 10;
    pt2.y1 = 10;
    pt2.z1 = 10;
    pt2.x2 = 20;
    pt2.y2 = 20;
    pt2.z2 = 20;

    QList<MegatronKeyPoint> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, tpvRegion(0,0) );
}

