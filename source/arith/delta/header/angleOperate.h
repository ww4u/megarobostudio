//////////////////////////////////////////////////////////////
//               配置文件 configure.h
//                    Ver 0.1
//
// 功能：包含各头文件、宏定义和机构求解参数结构体
//
// Created by StarSky at March 14, 2018
//////////////////////////////////////////////////////////////

#ifndef _ANGLEOPERATE_H_
#define _ANGLEOPERATE_H_

extern double angleErr;

#define PI    (double)3.141592654        // 定义pi常量
#define EXCEED_REGION    0    
#define IN_REGION        1    

void angleStandard(double *angle, int length);
void angleRegionStandard(double *angleRegion);
int judgeAngle(double angleRegion[2], double *angle, int length);

#endif