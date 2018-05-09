
#include "sinanju_split.h"


namespace ns_sinanju {

#define PI 3.1415926
//! a * pi/180
static void degToRad(double *degs,
    int count,
    double *rads)
{
    for (int i = 0; i < count; i++)
    {
        rads[i] = degs[i] * PI / 180;
    }
}

//! a * 180/pi
static void radToDeg(double *rads,
    int count,
    double *degs)
{
    for (int i = 0; i < count; i++)
    {
        degs[i] = rads[i] * 180 / PI;
    }
}

//! vector op
static void vectorAdd(double *pIn1,
    double *pIn2,
    int count,
    double *pOut)
{
    for (int i = 0; i < count; i++)
    {
        pOut[i] = pIn1[i] + pIn2[i];
    }
}

static void vectorSub(double *pIn1,
    double *pIn2,
    int count,
    double *pOut)
{
    for (int i = 0; i < count; i++)
    {
        pOut[i] = pIn1[i] - pIn2[i];
    }
}

//! normalize to [ -PI, PI ]
static double normalize_pi(double a)
{
    while (a > PI)
    {
        a -= PI * 2;
    }

    while (a < -PI)
    {
        a += PI * 2;
    }

    return a;
}

static void normalize_pis(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        p[i] = normalize_pi(p[i]);
    }
}

// 正解求末端位置
// armLength 机械臂的臂长
// angles--四个关节的当前角度
// res--正解的末端位置(X,Y,Z)

//! arm count: 6
//!
//!               2
//!             ____
//!            /     | 3
//!         1 /      |
//!          /    4------
//!         ||          | 5
//!         || 0        |
//!         ||
//! return: 0 -- no error
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
{
    if (NULL == armLengthArray
        || NULL == rotAngles
        || NULL == archAngles
        || NULL == deltaAngles
        || NULL == res)
    {
        return -1;
    }

    double localRads[4];

    //! delta + arch
    vectorAdd( deltaAngles, archAngles, angleCount, localRads );

    //! - rot Angle
    vectorSub( localRads, rotAngles, angleCount, localRads );
                                                        //! Ms Builder can not variable array
                                                        //! compensation

    degToRad(localRads, angleCount, localRads);       //! convert angle to rad

    normalize_pis( localRads, angleCount );
                                                        //! const vars
    double s1, c1, s2, c2, s2a3, c2a3, s2a3a4, c2a3a4;

    s1 = sin(localRads[0]);     						//! sin(theta1)
    c1 = cos(localRads[0]);     						//! cos(theta1)
    s2 = sin(localRads[1]);     						//! sin(theta2)
    c2 = cos(localRads[1]);     						//! cos(theta2)
    s2a3 = sin(localRads[1] + localRads[2]);      		//! sin(theta2 + theta3)
    c2a3 = cos(localRads[1] + localRads[2]);      		//! cos(theta2 + theta3)
    s2a3a4 = sin(localRads[1] + localRads[2] + localRads[3]);      //! sin(theta2 + theta3 + theta4)
    c2a3a4 = cos(localRads[1] + localRads[2] + localRads[3]);      //! cos(theta2 + theta3 + theta4)

                                                        //! end pos
    double endOut[4][4];
    //	double endX, endY, endZ;

    //	endOut[0][0] = c1 * c2a3a4;   						// 计算位姿矩阵
    //	endOut[0][1] = -c1 * s2a3a4;
    //	endOut[0][2] = s1;
    endOut[0][3] = armLengthArray[4] * s1 - armLengthArray[5] * c1 * s2a3a4 + c1 * (armLengthArray[1] * c2 + armLengthArray[2] * c2a3 + armLengthArray[3] * c2a3a4);

    //	endOut[1][0] = s1* c2a3a4;
    //	endOut[1][1] = -s1* s2a3a4;
    //	endOut[1][2] = -c1;
    endOut[1][3] = -c1 * armLengthArray[4] - armLengthArray[5] * s1 * s2a3a4 + s1 * (armLengthArray[1] * c2 + armLengthArray[2] * c2a3 + armLengthArray[3] * c2a3a4);

    //	endOut[2][0] = s2a3a4;
    //	endOut[2][1] = c2a3a4;
    endOut[2][3] = armLengthArray[5] * c2a3a4 + armLengthArray[0] + armLengthArray[1] * s2 + armLengthArray[2] * s2a3 + armLengthArray[3] * s2a3a4;

    //	endOut[3][3] = 1;

    res[0] = endOut[0][3];
    res[1] = endOut[1][3];
    res[2] = endOut[2][3];

    return 0;
}


#include "../megaarith.h"

#include "./arith/AddPointParams.h"
#include "./arith/T4params.h"

#include "./arith/T4Functions.cpp"
#include "./arith/AddPointParamFunctions.cpp"

}
