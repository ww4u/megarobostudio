
#include "../../arith/megaarith.h"
#include "../../../include/mcdef.h"
#include "arith_delta.h"

#include "deltadatacache.h"

namespace arith_delta {

//! apis
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

struct deltaConfig
{
    double mArmLength[3];
    double mDeltaPosition[2];
    double mDeltaAngles[2];
    double mP[2];

    double mPosLast[2];

    deltaConfig()
    {
        mArmLength[0] = 132;
        mArmLength[1] = 262.5;
        mArmLength[2] = 13;

        mDeltaPosition[0] = 0;
        mDeltaPosition[1] = 218.818;

        mDeltaAngles[0] = 450;
        mDeltaAngles[1] = 90;

        mP[0] = 0;
        mP[1] = 218.818;

        mPosLast[0] = 0;
        mPosLast[1] = 180;
    }
};

#define ANGLE_L_LIMIT     (0)
#define ANGLE_U_LIMIT     (2*MATH_PI)
int slove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints )
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

        deltaCache.m_pV[i] = points.at(i).v;
    }

    //! slove
    deltaConfig localConfig;
    int ret;
    ret = GetDeltArmPosition( localConfig.mArmLength,
                        localConfig.mDeltaPosition,
                        localConfig.mDeltaAngles,
                        localConfig.mPosLast,

                        deltaCache.m_pPXYZ,
                        deltaCache.m_pV,
                        deltaCache.m_pT,
                        points.size(),
                        deltaCache.m_pOutput
                        );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

}
