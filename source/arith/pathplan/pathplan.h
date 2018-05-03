#ifndef _PATH_PLAN_H_
#define _PATH_PLAN_H_

#include <QtCore>
#include "../megaarith.h"


// 导出函数
//PATH_PLAN_EXPORT int  GetPvtInfo(double** pIn, int length, double pStep, int pMode,double* resOut);
//PATH_PLAN_EXPORT int  Test(int length, int pStep, int pMode);

struct endPoint
{
    union
    {
        struct {
            double x,y,z;
            double end;
            double t;
            double flagInterp;
        };

        double datas;
    };
};

struct tracePoint
{
    union
    {
        struct {
        double x,y,z;
        double vx,vy,vz;
        double t;
        };

        double datas;
    };
};

namespace ns_pathplan {

//MEGA_EXPORT int  GetPvtLen(double* pIn, int length, double pStep, int pMode, int* resLength);
//MEGA_EXPORT int  GetPvtInfo(double* outPut,int len);

MEGA_EXTERN MEGA_EXPORT int  GetPointLen(double* pIn, int length, double pStep, int pMode, int* resLength);
MEGA_EXTERN MEGA_EXPORT int  GetPointInfo(double* outPut, int len);

}

#endif
