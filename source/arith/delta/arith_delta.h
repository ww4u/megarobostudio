#ifndef ARITH_DELTA_H
#define ARITH_DELTA_H

#include <QtCore>
#include "../../../include/datatype.h"
#include "../../com/basetype.h"

namespace arith_delta {

//struct D4Point
//{
//    float t;
//    float x, y, z;
//    float vx, vy, vz;
//};

//struct deltaPoint
//{
//    union
//    {
//        struct
//        {
//            float p[3];     //! xzy, pos
//            float v[3];     //! xzy, vel
//            float t;
//        };
//        struct
//        {
//            float pl,pr,py;
//            float vl,vr,vy;
//            float tt;
//        };
//        float datas[7];
//    };
//};

struct deltaPhase
{
    union
    {
        struct
        {
            float p[2];
        };
        float datas[2];
    };
};

struct arithDeltaConfig
{
//    double mLengths[5];     //! depends on arith type
//    double mAngleLimits[6];
//    double mP0[3];
//    double mAngle0[3];
//    float mError;

    double mArmLength[3];
    double mDeltaPosition[2];
    double mDeltaAngles[2];
    double mP[2];

    double mCcwDeltaAngles[2];

    double mPosLast[2];

    arithDeltaConfig()
    {
        mArmLength[0] = 132;
        mArmLength[1] = 262.5;
        mArmLength[2] = 13;

        mDeltaPosition[0] = 0;
        mDeltaPosition[1] = 218.818;

        mDeltaAngles[0] = 450;
        mDeltaAngles[1] = 90;

        mCcwDeltaAngles[0] = 270;
        mCcwDeltaAngles[1] = 90;

        mP[0] = 0;
        mP[1] = -218.818;

        mPosLast[0] = 0;
        mPosLast[1] = 180;
    }
};

//! ret : 0 -- no error
int ccwSlove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints,
           arithDeltaConfig &config );

int ccwSlove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints );

int cwSlove( QList<deltaPoint> &deltaPoints,
          QList<D4Point> &points,
          arithDeltaConfig &config );

int cwSlove( QList<deltaPoint> &deltaPoints,
          QList<D4Point> &points );

}

#endif // ARITH_DELTA_H
