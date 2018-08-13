#include "motors.h"

//int robotMotor::program( QList<D1Point> &curve,
//             const tpvRegion &region )
//{
//    int ret;

//    //! 0. check
//    //!

//    //! 1.build
//    ret = buildTrace( curve );
//    if ( ret != 0 )
//    { return ret; }

//    //! 2.config
//    setLoop( 1 );

//    //! 3.download
//    ret = downloadTrace( region );
//    if ( ret != 0 )
//    { return ret; }

//    return 0;
//}

//int robotMotor::move( QList<D1Point> &curve,
//                         const tpvRegion &region )
//{
//    int ret;

//    //! request run
//    run( region );

//    //! program
//    ret = program( curve, region );

//    return ret;
//}

//int robotMotor::preMove( QList<D1Point> &curve,
//                         const tpvRegion &region )
//{
//    int ret;

//    //! program
//    ret = program( curve, region );

//    return ret;
//}

//int robotMotor::move( float dd,
//          float dt,
//          float endV,
//          const tpvRegion &region )
//{
//    D1PointList curve;

//    D1Point kp;

//    //! p0
//    kp.t = 0;
//    kp.p = 0;
//    kp.v = 0;

//    curve.append( kp );

//    //! p1
//    kp.t = dt;
//    kp.p = dd;
//    kp.v = endV;
//    curve.append( kp );

//    return move( curve, region );
//}

