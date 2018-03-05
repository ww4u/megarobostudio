#ifndef _KINEMATIC_H_
#define _KINEMATIC_H_

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

// 导出函数

//! for 4 angle
struct jointsAngle
{
    double angles[4];
};

struct jointsTrace
{
    union {
        struct {
        double p[4];
        double v[4];
        double t;
        };

        double datas;
    };
};

namespace ns_kinematic {

// 正解求末端位置
MEGA_EXPORT int  GetEndPosition(double* angles,double* res);
// 逆解求关节PVT
MEGA_EXPORT int  GetArmPosition(double* posLast,
                                double* posIn, int skipP,
                                double* vIn, int skipV,
                                double* tIn, int skipT,
                                int len,
                                double* res);

}

#endif
