#ifndef ARITH_DELTA_H
#define ARITH_DELTA_H

#include <QtCore>
#include "../../../include/datatype.h"

namespace arith_delta {

struct deltaPoint
{
    union
    {
        struct
        {
            float p[3];     //! xyz, pos
            float v[3];     //! xyz, vel
            float a[3];     //! xyz, acc
            float t;
        };
        float datas[10];
    };
};

struct arithDeltaConfig
{
    double mLengths[5];     //! depends on arith type
    double mAngleLimits[6];
    double mP0[3];
    double mAngle0[3];
    float mError;
};

//! ret : 0 -- no error
int slove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints,
           arithDeltaConfig &config );

int slove( QList<D4Point> & points,
           QList<deltaPoint> & deltaPoints );

int calc( QList<deltaPoint> &deltaPoints,
          QList<D4Point> &points,
          arithDeltaConfig &config );
}

#endif // ARITH_DELTA_H
