//////////////////////////////////////////////////////////////
//             位置计算头文件 configure.h
//                    Ver 0.1
//
// 功能：包含各函数和变量声明
//
// Created by StarSky at March 14, 2018
//////////////////////////////////////////////////////////////

#ifndef _CALPOSITION_H_
#define _CALPOSITION_H_

void angleTrans(double *data1, double *data2, int length, int mode, int direction);
void velAcceTrans(double *data1, double *data2, int length, int mode, int direction);
int positionKinematics(double posIn[2], double posOut[6], int type, int mode);
int selPosition(double *solution, int dataSize[2]);
int binaryQuadratic(double A[8], double b[2], double solution[4], double err);


#endif