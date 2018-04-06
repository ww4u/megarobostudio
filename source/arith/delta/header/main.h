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


int solveDeltaKinematics(double structureParams[5], double angleLim[6], double P0[3], double angle0[3],
                         double *pos1, double *pos2, double *pos3,
                         double *vel1, double *vel2, double *vel3,
                         double *acce1, double *acce2, double *acce3,
                         int solveSet[4], double err);

#endif