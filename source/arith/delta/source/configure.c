//////////////////////////////////////////////////////////////
//             参数配置模块 configure.c
//                    Ver 0.1
//
// 功能：位置的正逆解求解
//
// Created by StarSky at March 28, 2018
//////////////////////////////////////////////////////////////

#include "configure.h"


/*************************************************************
* 输入：机构结构参数{L1,L2,e,d},
*      角度限制{t1min,t1max,t2min,t2max}
*      末端初始位置，实际机构零位关节角度值
*      求解模式参数{mode,type,direction,length}
*      误差
* 输出：无
* 
* 功能：将机构结构参数和矫正参数结构参数结构体（完成角度限制区间转换）
**************************************************************/
void GetStructureInfo(double structureParams[4], double angleLim[4], double P0[2], double angle0[2], int solveSet[4], double err)
{
    int i;

    /* 机构参数 */
    deltaStructureParams.L1 = structureParams[0];
    deltaStructureParams.L2 = structureParams[1];
    deltaStructureParams.e = structureParams[2];
    deltaStructureParams.d = structureParams[3];
    deltaStructureParams.err = err;
    deltaStructureParams.mode = solveSet[0];
    deltaStructureParams.type = solveSet[1];

    oneFreedomStructureParams.transConst = structureParams[4];
    oneFreedomStructureParams.err = err;
    oneFreedomStructureParams.mode = solveSet[0];

    /* 初始位置和角度 */
    deltaCorrectParams.P0[0] = P0[0];    // 机构末端初始位置x
    deltaCorrectParams.P0[1] = P0[1];    // 机构末端初始位置y
    deltaCorrectParams.detTheta[0] = angle0[0];        // 实际机构在零位时关节1角度
    deltaCorrectParams.detTheta[1] = angle0[1];     // 实际机构在零位时关节2角度
    deltaCorrectParams.direction = solveSet[2];        // 实际机构正向旋转方向
    deltaCorrectParams.length = solveSet[3];        // 求解点个数

    oneFreedomCorrectParams.detTheta = angle0[2];
    oneFreedomCorrectParams.length = solveSet[3];        // 求解点个数


    /* 角度限制 */
    for(i = 0;i < 2; i ++)
    {
        /* 角度限制区间转换（实际机构->模型结构） */
        angleTrans(&angleLim[i], &angleLim[2 + i], 1, FORWARD_KINEMATICS, deltaCorrectParams.direction);

        deltaStructureParams.angleLim[i] = angleLim[i];
        deltaStructureParams.angleLim[i + 2] = angleLim[i + 2];

        oneFreedomStructureParams.angleLim[i] = angleLim[i + 4];
    }


    angleErr = err;        // 误差
}

