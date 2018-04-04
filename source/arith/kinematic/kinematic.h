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


int MEGA_EXPORT GetEndPosition(
                                double* armLengthArray, //! foreach arm
                                int armCount,			//! arm count: 6

                                double *rotAngles,
                                double *archAngles,
                                double *deltaAngles,    //! arch + delta - rot

                                int angleCount,

                                double* res )			//! out: x, y, z
;


//! return: 0 -- no error
int  MEGA_EXPORT getArmPosition_Size(
    double* armLength, int armCount,

//    double* deltAngles, int angleCount,    //! angles: [0~3]
//    double* posRef,                        //!

    double *rotAngles,
    double *archAngles,

    int angleCount,

    double* posIn, int skipP,
    double* vIn, int skipV,
    double* tIn, int skipT,
    int len,                               //! points len in
    int *pOutSize
);

//! return: 0 -- no error
int  MEGA_EXPORT getArmPosition_Data(
    double* armLength, int armCount,

    double *rotAngles,
    double *archAngles,

    int angleCount,

//    double* deltAngles, int angleCount,    //! angles: [0~3]
//    double* posRef,                        //!

    double* posIn, int skipP,
    double* vIn, int skipV,
    double* tIn, int skipT,
    int len,                               //! points len in

    double *outRes,
    int nCap
    );

int zeroCrossTest();

}

#endif
