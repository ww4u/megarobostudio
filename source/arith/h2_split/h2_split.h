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
            double t;
            double x,y;
            double v;
        };

        double datas[4];
    };
};

struct SlovePoint
{
    union
    {
        struct
        {
            double Pxy[2];
            double Vxy[2];
            double t;
        };
        double datas[5];
    };
};

int h2Split( const QList<double> &armLengthes,
             const QList<double> &zeroXy,
             QList< endPoint *> &points,
              QList< tpvList*> &splitDataset );
int h2Split(
                    const QList<double> &armLengthes,
                    const QList<double> &zeroXy,
                    endPoint * pPoints,
                    int n,
                    QList< tpvList*> &splitDataset );

int h2Gc( QList< tpvList*> &splitDataset );

int h2Pose( const QList<double> &armLengthes,
            const QList<double> &zeroXy,
            float angleL, float angleR,
            float &x, float &y );
}

#endif
