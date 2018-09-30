
#include "../../com/basetype.h"
#include "arith_igus.h"
#include "../../../source/sys/sysapi.h"

#include "float.h"  //! FLT_MIN
#include "../../../include/mcdef.h"

namespace arith_igus {

#include "./arith/params.h"
#include "./arith/functions.cpp"

#include "./arith/patch.cpp"


struct ResInfoAlist
{
    double p[3];
    double v[3];
    double t;
};

int cwSlove( igusConfig &cfg,
             float ps[3],
             float xyzs[3] )
{
    return getPos( cfg, ps, xyzs );
}

int ccwSlove( igusConfig &cfg,
              QList<D4Point> & pts,
              QList<deltaPoint> & deltaPoints )
{

    int ret;

    int dataLen = pts.size();

    //! cache data
    xxxGroup<double> ps, vs, ts;
    if ( 0 != ps.alloc( dataLen * 3 ) )
    { return -1; }

    if ( 0 != vs.alloc( dataLen * 3 ) )
    { return -1; }

    if ( 0 != ts.alloc( dataLen) )
    { return -1; }

    //! shift
    for ( int i = 0; i < dataLen; i++ )
    {
        ps.data()[i*3] = pts.at(i).x;
        ps.data()[i*3+1] = pts.at(i).y;
        ps.data()[i*3+2] = pts.at(i).z;

        vs.data()[i*3] = pts.at(i).vx;
        vs.data()[i*3+1] = pts.at(i).vy;
        vs.data()[i*3+2] = pts.at(i).vz;

        ts.data()[i] = pts.at(i).t;
    }

    Igus obj;
//    obj.armLength[0] = 679.2;
//    obj.armLength[1] = 400;
//    obj.armLength[2] = 72.75;
//    obj.armLength[3] = PI/4;

//    obj.offset[0] = 0;
//    obj.offset[1] = 239.5;

//    obj.P0[0] = 0;
//    obj.P0[1] = 0;
//    obj.P0[2] = 526.168;

//    obj.posLim[0] = 239.5;
//    obj.posLim[1] = 385.5;

//    obj.scal = 70;
//    obj.vM = 2*360*2;

    obj.armLength[0] = cfg.armLength[0];
    obj.armLength[1] = cfg.armLength[1];
    obj.armLength[2] = cfg.armLength[2];
    obj.armLength[3] = cfg.armLength[3];

    obj.offset[0] = cfg.offset[0];
    obj.offset[1] = cfg.offset[1];

    obj.P0[0] = cfg.P0[0];
    obj.P0[1] = cfg.P0[1];
    obj.P0[2] = cfg.P0[2];

    obj.posLim[0] = cfg.posLim[0];
    obj.posLim[1] = cfg.posLim[1];

    obj.scal = cfg.scal;
    obj.vM = cfg.vM;

    obj.mode = 1;

    int outLen;
    ret = GetIgusPvtLen( &obj,
                         ps.data(),
                         vs.data(),
                         ts.data(),
                         dataLen,
                         &outLen
                         );
    if ( ret != 0 )
    { return ret*100; }

    //! copy data
    xxxGroup<ResInfoAlist> planeCache;

    if ( 0 != planeCache.alloc( outLen  ) )
    { return -2; }

    if ( 0 != GetIgusPvtInfo( planeCache.data()->p ) )  //! first
    {
        return -3;
    }

    //! output
    ResInfoAlist *pBase = planeCache.data();

    deltaPoint outPt;
    for ( int i = 0; i < outLen / (int)(sizeof(ResInfo)/sizeof(double) ) ; i++ )
    {
        outPt.t = pBase[i].t;
        for ( int j = 0; j < 3; j++ )
        {
            outPt.p[j] = pBase[i].p[j];
            outPt.v[j] = pBase[i].v[j];
        }

        deltaPoints.append( outPt );
    }

    return 0;
}

}
