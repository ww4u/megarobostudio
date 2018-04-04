//////////////////////////////////////////////////////////////
//                主函数头文件 main.h
//                    Ver 0.1
//
// 功能：包含各函数和变量声明
//
// Created by StarSky at March 28, 2018
//////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

//#ifdef DELTADLLMANAGER_EXPORTS
//#define DELTA_API	__declspec(dllexport)
//#else
//#define DELTA_API	__declspec(dllimport)
//#endif

#define DELTA_API
#ifdef __cplusplus
extern "C" {
#endif

int solveDeltaKinematics(double structureParams[4], double angleLim[4], double P0[2], double angle0[2], double *pos1, double *pos2, double *vel1, double *vel2, double *acce1, double *acce2, int solveSet[4], double err);

void delta_test();

#ifdef __cplusplus
}
#endif

#endif
