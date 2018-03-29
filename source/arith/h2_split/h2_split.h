#ifndef _H2_SPLIT_H_
#define _H2_SPLIT_H_

#include <QtCore>
#include "../megaarith.h"

#ifdef MEGA_EXPORT
#undef MEGA_EXPORT
#endif

#if defined(EXPORT_LIBRARY)
#  define MEGA_EXPORT Q_DECL_EXPORT
#else
#  define MEGA_EXPORT Q_DECL_IMPORT
#endif

#undef MEGA_EXPORT
#define MEGA_EXPORT

#include "../../com/basetype.h"
namespace h2_split {

struct endPoint
{
    union
    {
        struct {
            float t;
            float x1,y1,z1;
        };

        float datas[4];
    };
};

int h2Split(  QList< endPoint *> &points,
                    QList< tpvList*> &splitDataset );
int h2Split(
                    endPoint * pPoints,
                    int n,
                    QList< tpvList*> &splitDataset );

int h2Gc( QList< tpvList*> &splitDataset );
}

#endif
