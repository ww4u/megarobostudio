//////////////////////////////////////////////////////////////
//               配置文件 configure.h
//                    Ver 0.1
//
// 功能：包含各头文件、宏定义和机构求解参数结构体
//
// Created by StarSky at March 28, 2018
//////////////////////////////////////////////////////////////

#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dllMain.h"
#include "calPosition.h"
#include "calVelocity.h"
#include "calAcceleration.h"
#include "angleOperate.h"


/* 机构参数结构体 */
struct deltaStructureParamsObj
{
    double L1;        // 主动杆长度
    double L2;        // 从主动杆长度
    double e;        // 挡板左右滑轮内边缘间距
    double d;        // 两边带轮内边缘间距

    double angleLim[4];        // 关节角度限制
    double err;        // 误差的定义

    int mode;        // 求解模式
    int type;        // 解的类型
};

/* 机构矫正参数结构体 */
struct deltaCorrectParamsObj
{
    int direction;
    int length;

    double P0[2];        // 上一时刻的位置x
    double detTheta[2];        // 实际机构关节在零位时的角度
};

/* 全局变量 */
extern struct deltaStructureParamsObj  deltaStructureParams;
extern struct deltaCorrectParamsObj deltaCorrectParams;

/* 宏定义 */
#define N    3

#define FORWARD_KINEMATICS    0    // 正解求解
#define INVERSE_KINEMATICS    1    // 逆解求解

#define TRUE    2
#define FALSE    3

#define ANGULAR_LIMIT    (double)0.05            // 奇异位限制边界

#define SOLVE_OVER        4        // 求解完成
#define SINGULAR_POSITION    0    // 机构位置奇异
#define EXCEED_LIMIT    -1        // 超出限制
#define INPUT_ERROR        -2        // 输入参数错误
#define SOLVE_ERROR        -3        // 解错误

#define CLOCKWISE        5        // 顺时针
#define ANTICLOCKWISE    6        // 逆时针

/* 四种构型 */
#define TYPE1    7        // 外外
#define TYPE2    8        // 内内
#define TYPE3    9        // 内外
#define TYPE4    10        // 外内
 

void GetStructureInfo(double structureParams[4], double angleLim[4], double P0[2], double angle0[2], int solveSet[4], double err);


#endif