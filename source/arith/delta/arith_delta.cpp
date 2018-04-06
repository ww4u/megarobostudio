
#include "../../arith/megaarith.h"
#include "../../../include/mcdef.h"
#include "arith_delta.h"

#include "./header/dllMain.h"
#include "./header/configure.h"

#include "deltadatacache.h"

namespace arith_delta {

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

//! ret : 0 -- no error
int slove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints,
           arithDeltaConfig &config )
{
    int ret;

    //! config
    int sloveSet[4] = {INVERSE_KINEMATICS, TYPE1, ANTICLOCKWISE, };
//    int sloveSet[4] = {INVERSE_KINEMATICS, TYPE1, CLOCKWISE, };

    //! cache
    int dataLen = points.size();
    DeltaDataCache dataCache;
    if ( 0 != dataCache.alloc( dataLen  ) )
    { return -1; }

    //! copy data
    for ( int i = 0; i < dataLen; i++ )
    {
        dataCache.m_pPX[i] = points.at(i).x;
        dataCache.m_pPY[i] = points.at(i).y;
        dataCache.m_pPZ[i] = points.at(i).z;
    }

    //! config length
    sloveSet[3] = dataLen;

    ret = solveDeltaKinematics( config.mLengths,
                                config.mAngleLimits,
                                config.mP0,
                                config.mAngle0,

                                dataCache.m_pPX, dataCache.m_pPY, dataCache.m_pPZ,
                                dataCache.m_pVX, dataCache.m_pVY, dataCache.m_pVZ,
                                dataCache.m_pAX, dataCache.m_pAY, dataCache.m_pAZ,

                                sloveSet,
                                config.mError
                                );
    if ( ret != SOLVE_OVER )
    { return -1; }
    else
    {  }

    //! range shift
    rollPhase( dataCache.m_pPX, dataLen );
    rollPhase( dataCache.m_pPY, dataLen );
    rollPhase( dataCache.m_pPZ, dataLen );

    //! normalize phase to degree
    radToDeg( dataCache.m_pPX, dataLen, dataCache.m_pPX );
    radToDeg( dataCache.m_pPY, dataLen, dataCache.m_pPY );
    radToDeg( dataCache.m_pPZ, dataLen, dataCache.m_pPZ );

    //! export data
    deltaPoint dPt;
    for ( int i = 0; i < dataLen; i++ )
    {
        //! set data
        dPt.t = points.at(i).t;

        dPt.p[0] = dataCache.m_pPX[i];
        dPt.p[1] = dataCache.m_pPY[i];
        dPt.p[2] = dataCache.m_pPZ[i];

        dPt.v[0] = dataCache.m_pVX[i];
        dPt.v[1] = dataCache.m_pVY[i];
        dPt.v[2] = dataCache.m_pVZ[i];

        dPt.a[0] = dataCache.m_pAX[i];
        dPt.a[1] = dataCache.m_pAY[i];
        dPt.a[2] = dataCache.m_pAZ[i];

        //! append
        deltaPoints.append( dPt );
    }

    return 0;
}

#define ANGLE_L_LIMIT     (0)
#define ANGLE_U_LIMIT     (2*MATH_PI)
int slove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints )
{
    //! default config
    arithDeltaConfig config;

    config.mLengths[0] = 132;
    config.mLengths[1] = 262.5;
    config.mLengths[2] = 90;
    config.mLengths[3] = 64;
    config.mLengths[4] = -10;

    config.mAngleLimits[0] = ANGLE_L_LIMIT;
    config.mAngleLimits[1] = ANGLE_U_LIMIT;
    config.mAngleLimits[2] = ANGLE_L_LIMIT;
    config.mAngleLimits[3] = ANGLE_U_LIMIT;
    config.mAngleLimits[4] = ANGLE_L_LIMIT;
    config.mAngleLimits[5] = ANGLE_U_LIMIT;

    config.mP0[0] = 0;
    config.mP0[1] = -218.8;
    config.mP0[2] = 0;

    config.mAngle0[0] = 0;
    config.mAngle0[1] = 0;
    config.mAngle0[2] = 0.5;

    config.mError = 1.0e-5;

    return slove( points, deltaPoints, config );
}

int calc( QList<deltaPoint> &deltaPoints,
          QList<D4Point> &points,
          arithDeltaConfig &config )
{
    return 0;
}

}
