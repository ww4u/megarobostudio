#ifndef _KINEMATIC_H_
#define _KINEMATIC_H_

#include <QtCore>
#include "../megaarith.h"


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


namespace ns_sinanju {

// 正解求末端位置
// armLength 机械臂的臂长
// angles--四个关节的当前角度
// res--正解的末端位置(X,Y,Z)
MEGA_EXTERN MEGA_EXPORT int GetEndPosition(double* armLength, double* deltAngles,double* angles,double* res);

MEGA_EXTERN MEGA_EXPORT  int  GetPvtInfo(double* res);

MEGA_EXTERN MEGA_EXPORT int  GetPvtLen(double* armLength, double* deltAngles, double* posLast,
                                       double* posIn, int skipP,
                                       double* vIn, int skipV,
                                       double* tIn, int skipT,
                                       int len,
                                       int* resLen );

int MEGA_EXPORT GetEndPosition(
    double* armLengthArray,     //! foreach arm
    int armCount,		//! arm count: 6

//    double* deltAngles,		//! delta angle: degree
//    double*

    double *rotAngles,
    double *archAngles,
    double *deltaAngles,

//    double* angles,		//! current angle 0~360
    int angleCount,             //! 4

    double* res)		//! out: x, y, z
;

}

#endif
