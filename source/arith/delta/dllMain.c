//////////////////////////////////////////////////////////////
//                  主函数 main.c
//                    Ver 0.1
//
// 函数功能：对模块程序进行测试
//
// 注：1. 程序中所有角度标准化为[0, 2*pi)，角度区间以逆时针标准化，且
//    角度以rad为单位
//    2. 输入的关节角度值及其限制区间为实际机构的值
//
// Created by StarSky at March 28, 2018
//////////////////////////////////////////////////////////////

#include "configure.h"

/* 结构参数声明 */
struct deltaStructureParamsObj  deltaStructureParams = {0};
/* 结构矫正参数声明 */
struct deltaCorrectParamsObj deltaCorrectParams = {0};

/*************************************************************
****************************接口函数****************************
**************************************************************
* 输入：结构参数，角度限制，初始位置，位置参数1和2，速度参数1和2，
*      加速度参数1和2，角度限制标志，求解模式
* 输出：求解标志位
* 
* 功能：进行机构的正逆运动学求解（利用输入数组保存解）
**************************************************************/
DELTA_API
int solveDeltaKinematics(double structureParams[4], double angleLim[4],
                         double P0[2], double angle0[2],
                         double *pos1, double *pos2,
                         double *vel1, double *vel2,
                         double *acce1, double *acce2, int solveSet[4],
                         double err )
{
    double posOut[6], velOut[6], acceOut[6];
    double posIn[2], velIn[2], acceIn[2];
    int i, flag;

    /* 机构参数设置 */
    GetStructureInfo(structureParams, angleLim, P0, angle0, solveSet, err);        // 机构参数设置

    /* 输入：角度转换（实际机构->模型结构） */
    if(solveSet[0] == FORWARD_KINEMATICS)
    {
        angleTrans(pos1, pos2, deltaCorrectParams.length, deltaStructureParams.mode, deltaCorrectParams.direction);
        velAcceTrans(vel1, vel2, deltaCorrectParams.length, deltaStructureParams.mode, deltaCorrectParams.direction);
        velAcceTrans(acce1, acce2, deltaCorrectParams.length, deltaStructureParams.mode, deltaCorrectParams.direction);
    }


    for(i = 0;i < deltaCorrectParams.length;i ++)
    {
        /* 提取输入参数 */
        posIn[0] = pos1[i];
        posIn[1] = pos2[i];
        velIn[0] = vel1[i];
        velIn[1] = vel2[i];
        acceIn[0] = acce1[i];
        acceIn[1] = acce2[i];

        /* 机构位置正逆解求解 */
        flag = positionKinematics(posIn, posOut, deltaStructureParams.type, deltaStructureParams.mode);
        if(flag != SOLVE_OVER)
            return flag;        // 返回：求解失败

        /* 机构速度正逆解求解 */
        flag = velocityKinematics(posOut, velIn, velOut, deltaStructureParams.mode);
        if(flag != SOLVE_OVER)
            return flag;        // 返回：求解失败

        /* 机构加速度正逆解求解 */
        flag = accelerationKinematics(posOut, velOut, acceIn, acceOut, deltaStructureParams.mode);
        if(flag != SOLVE_OVER)
            return flag;        // 返回：求解失败

        /* 提取末端信息 */
        if(deltaStructureParams.mode == FORWARD_KINEMATICS)
        {
            pos1[i] = posOut[4];
            pos2[i] = posOut[5];
            vel1[i] = velOut[4];
            vel2[i] = velOut[5];
            acce1[i] = acceOut[4];
            acce2[i] = acceOut[5];

            /* 更新上一时刻末端位置 */
            deltaCorrectParams.P0[0] = posOut[4];
            deltaCorrectParams.P0[1] = posOut[5];
        }
        /* 提取主动关节信息 */
        else
        {
            pos1[i] = posOut[0];
            pos2[i] = posOut[1];
            vel1[i] = velOut[0];
            vel2[i] = velOut[1];
            acce1[i] = acceOut[0];
            acce2[i] = acceOut[1];
        }
    }

    /* 输出：角度转换（模型结构->实际机构） */
    if(deltaStructureParams.mode == INVERSE_KINEMATICS)
    {
        angleTrans(pos1, pos2, deltaCorrectParams.length, deltaStructureParams.mode, deltaCorrectParams.direction);
        velAcceTrans(vel1, vel2, deltaCorrectParams.length, deltaStructureParams.mode, deltaCorrectParams.direction);
        velAcceTrans(acce1, acce2, deltaCorrectParams.length, deltaStructureParams.mode, deltaCorrectParams.direction);
    }

    return SOLVE_OVER;        // 返回：求解错误
}
