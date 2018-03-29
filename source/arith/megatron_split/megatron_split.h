#ifndef _MEGATRON_SPLIT_H_
#define _MEGATRON_SPLIT_H_

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
namespace megatron_split {

struct endPoint
{
    union
    {
        struct {
            float t;
            float x1,y1,z1;
            float x2,y2,z2;
        };

        float datas[7];
    };
};

int megatronSplit(  QList< endPoint *> &points,
                    QList< tpvList*> &splitDataset );
int megatronSplit(
                    endPoint * pPoints,
                    int n,
                    QList< tpvList*> &splitDataset );

int megatronGc( QList< tpvList*> &splitDataset );
}

#endif
