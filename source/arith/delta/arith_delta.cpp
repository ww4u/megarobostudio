
#include "../../arith/megaarith.h"
#include "../../../include/mcdef.h"
#include "arith_delta.h"

#include "deltadatacache.h"

namespace arith_delta {

//! apis
#include "./arith/addpointparamfunctions.cpp"
#include "./arith/functions.cpp"

//! a * pi/180
static void degToRad(double *degs,
    int count,
    double *rads)
{
    for (int i = 0; i < count; i++)
    {
        rads[i] = degs[i] * PI / 180;
    }
}

//! a * 180/pi
static void radToDeg(double *rads,
    int count,
    double *degs)
{
    for (int i = 0; i < count; i++)
    {
        degs[i] = rads[i] * 180 / PI;
    }
}

//! [base,base+2*PI)
static double normalizeRange( double v, double base  )
{
    while( v <= base )
    { v += 2*MATH_PI; }

    while( v > (base+2*MATH_PI) )
    { v -= 2*MATH_PI; }

    return v;
}

static void normalRange( double *pDat, int count,
                        double base = 0 )
{
    for ( int i = 0 ; i < count; i++ )
    {
        pDat[i] = normalizeRange( pDat[i], base );
    }
}

static double vectorPP( double *pIn,
                        int count )
{
    double ma, mi;

    mi = pIn[0];
    ma = mi;

    for ( int i = 1; i < count; i++ )
    {
        if ( pIn[i] > ma )
        { ma = pIn[i]; }
        else if ( pIn[i] < mi )
        { mi = pIn[i]; }
        else
        {}
    }

    return qAbs( mi - ma );
}

static void rollPhase( double *pDat,    //! in out
                       int count )
{
    Q_ASSERT( NULL != pDat );
    double pp;

    //! normal 0~2*pi
    normalRange( pDat, count, 0 );

    //! peak - peak
    pp = vectorPP( pDat, count );

    //! roll
    if ( pp > MATH_PI )
    {
        normalRange( pDat, count, -MATH_PI );

        pp = vectorPP( pDat, count );

        Q_ASSERT( pp < MATH_PI );
    }
    //! do not change
    else
    {}
}

////! ret : 0 -- no error
//int slove( QList<D4Point> & points,
//           QList<deltaPoint> & deltaPoints,
//           arithDeltaConfig &config )
//{
//    int ret;

//    //! config
//    int sloveSet[4] = {INVERSE_KINEMATICS, TYPE1, ANTICLOCKWISE, };
////    int sloveSet[4] = {INVERSE_KINEMATICS, TYPE1, CLOCKWISE, };

//    //! cache
//    int dataLen = points.size();
//    DeltaDataCache dataCache;
//    if ( 0 != dataCache.alloc( dataLen  ) )
//    { return -1; }

//    //! copy data
//    for ( int i = 0; i < dataLen; i++ )
//    {
//        dataCache.m_pPX[i] = points.at(i).x;
//        dataCache.m_pPY[i] = points.at(i).y;
//        dataCache.m_pPZ[i] = points.at(i).z;
//    }

//    //! config length
//    sloveSet[3] = dataLen;

//    ret = solveDeltaKinematics( config.mLengths,
//                                config.mAngleLimits,
//                                config.mP0,
//                                config.mAngle0,

//                                dataCache.m_pPX, dataCache.m_pPY, dataCache.m_pPZ,
//                                dataCache.m_pVX, dataCache.m_pVY, dataCache.m_pVZ,
//                                dataCache.m_pAX, dataCache.m_pAY, dataCache.m_pAZ,

//                                sloveSet,
//                                config.mError
//                                );
//    if ( ret != SOLVE_OVER )
//    { return -1; }
//    else
//    {  }

//    //! range shift
//    rollPhase( dataCache.m_pPX, dataLen );
//    rollPhase( dataCache.m_pPY, dataLen );
//    rollPhase( dataCache.m_pPZ, dataLen );

//    //! normalize phase to degree
//    radToDeg( dataCache.m_pPX, dataLen, dataCache.m_pPX );
//    radToDeg( dataCache.m_pPY, dataLen, dataCache.m_pPY );
//    radToDeg( dataCache.m_pPZ, dataLen, dataCache.m_pPZ );

//    //! export data
//    deltaPoint dPt;
//    for ( int i = 0; i < dataLen; i++ )
//    {
//        //! set data
//        dPt.t = points.at(i).t;

//        dPt.p[0] = dataCache.m_pPX[i];
//        dPt.p[1] = dataCache.m_pPY[i];
//        dPt.p[2] = dataCache.m_pPZ[i];

//        dPt.v[0] = dataCache.m_pVX[i];
//        dPt.v[1] = dataCache.m_pVY[i];
//        dPt.v[2] = dataCache.m_pVZ[i];

//        dPt.a[0] = dataCache.m_pAX[i];
//        dPt.a[1] = dataCache.m_pAY[i];
//        dPt.a[2] = dataCache.m_pAZ[i];

//        //! append
//        deltaPoints.append( dPt );
//    }

//    return 0;
//}


int ccwSlove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints,
           arithDeltaConfig &localConfig )
{
    //! move data
    DeltaDataCache deltaCache;
    if ( 0 != deltaCache.alloc( points.size() ) )
    { return -1; }

    for ( int i = 0; i < points.size(); i++ )
    {
        deltaCache.m_pPXYZ[i*3+0] = points.at(i).x;
        deltaCache.m_pPXYZ[i*3+1] = points.at(i).y;
        deltaCache.m_pPXYZ[i*3+2] = points.at(i).z;

        deltaCache.m_pT[i] = points.at(i).t;

        deltaCache.m_pV[i*3+0] = points.at(i).vx;
        deltaCache.m_pV[i*3+1] = points.at(i).vy;
        deltaCache.m_pV[i*3+2] = points.at(i).vz;

//        qDebug()<<points.at(i).vx;
    }
    //! slove
    int ret;
    int outSize;
    ret = GetDeltArmLen( localConfig.mArmLength,
                        localConfig.mDeltaPosition,
                        localConfig.mCcwDeltaAngles,
                        localConfig.mPosLast,

                        deltaCache.m_pPXYZ,
                        deltaCache.m_pV,
                        deltaCache.m_pT,
                        points.size(),
                        &outSize
                        );
    if ( ret != 0 )
    { return ret; }

    //! check size
    if ( outSize >= 2 )
    {}
    else
    { return -1; }

    deltaCache.clean();

    //! alloc again
    if ( 0 != deltaCache.alloc( outSize ) )
    { return -1; }

    ret = GetDeltArmPosition( deltaCache.m_pOutput );
    if ( ret != 0 )
    { return ret; }

    //! export the x, y
    deltaPoint pt;
    pt.p[2] = 0;
    pt.v[2] = 0;
    int stride  = 5;
    for ( int i = 0; i < outSize; i++ )
    {
        pt.t = deltaCache.m_pOutput[i*stride + 4];

        pt.p[0] = deltaCache.m_pOutput[i*stride + 0];
        pt.p[1] = deltaCache.m_pOutput[i*stride +1];

        pt.v[0] = deltaCache.m_pOutput[i*stride +2];
        pt.v[1] = deltaCache.m_pOutput[i*stride +3];

        deltaPoints.append( pt );

//        qDebug()<<pt.t<<pt.v[0]<<pt.v[1];
    }

    return 0;
}

int ccwSlove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints )
{
    arithDeltaConfig lConfig;

    return ccwSlove( points, deltaPoints, lConfig );
}


int cwSlove( QList<deltaPoint> &deltaPoints,
          QList<D4Point> &points,
          arithDeltaConfig &localConfig )
{

    DeltaPhaseCache phaseCache, xzCache;

    if ( 0 != phaseCache.alloc( deltaPoints.size() ) )
    { return -1; }

    if ( 0 != xzCache.alloc( deltaPoints.size() ) )
    { return -1; }

    //! load phase
    for ( int i = 0; i < deltaPoints.size(); i++ )
    {
        phaseCache.m_pPhase[ i * 2 + 0 ] = deltaPoints.at(i).pl;
        phaseCache.m_pPhase[ i * 2 + 1 ] = deltaPoints.at(i).pr;
    }

    int ret;
    ret = GetDeltEndPosition(
                       localConfig.mArmLength,
                       localConfig.mDeltaAngles,
                       localConfig.mP,
                       phaseCache.m_pPhase,
                       deltaPoints.size(),
                       xzCache.m_pPhase );
    if ( 0 != ret )
    { return -1; }

    //! export data
    D4Point pt;
    for ( int i = 0; i < deltaPoints.size(); i++ )
    {
        pt.t = deltaPoints.at(i).t;
        pt.z = xzCache.m_pPhase[ i * 2 + 1 ];
        pt.y = 0;
        pt.x = xzCache.m_pPhase[ i * 2 + 0 ];

        pt.vx = 0;
        pt.vy = 0;
        pt.vz = 0;

        points.append( pt );
    }

    return 0;
}

int cwSlove( QList<deltaPoint> &deltaPoints,
          QList<D4Point> &points )
{
    arithDeltaConfig lConfig;

    return cwSlove( deltaPoints, points, lConfig );
}

}
