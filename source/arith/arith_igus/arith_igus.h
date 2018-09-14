#ifndef _ARITH_IGUS_H_
#define _ARITH_IGUS_H_

#include "../../com/basetype.h"

namespace arith_igus {

struct igusConfig
{
    double armLength[4];

    double offset[2];

    double P0[3];

    double posLim[2];

    double scal;

    double vM;
};

int ccwSlove( igusConfig &cfg,
              QList<D4Point> & points,
              QList<deltaPoint> & deltaPoints );

}


//QList< endPoint *> &points,
//QList< tpvList*> &splitDataset

#endif
