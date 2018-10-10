
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

//        qDebug()<<__LINE__<<points.at(i).t<<points.at(i).vx<<points.at(i).vz;
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

//        qDebug()<<__LINE__<<pt.t<<pt.v[0]<<pt.v[1];
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
