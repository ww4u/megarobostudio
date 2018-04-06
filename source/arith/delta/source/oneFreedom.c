//////////////////////////////////////////////////////////////
//             参数配置模块 oneFreedom.c
//                    Ver 0.1
//
// 功能：运动学求解
//
// Created by StarSky at April 4, 2018
//////////////////////////////////////////////////////////////

#include "configure.h"


/*************************************************************
* 输入：位置正逆运动学求解的输入参数，主动关节/末端解，
*      从动关节解，解的类型（主从杆的构型），求解模式
* 输出：求解标志位
* 
* 功能：进行位置正逆运动学求解
* 
* 说明：当关节角度为实际机构角度
**************************************************************/
int calOneFreedomPosition(double posIn, double *posOut, int mode)
{
    int i;
    double angleRegion[2];

    /* 获取角度限制区间 */
    for(i = 0;i < 2;i ++)
        angleRegion[i] = oneFreedomStructureParams.angleLim[i];

    if(mode == FORWARD_KINEMATICS)     // 正解求解
    {
        /* 判断角度是否超出范围 */
        if(judgeAngle(angleRegion, &posIn, 1) == EXCEED_REGION)
            return EXCEED_REGION;        // 返回： 输入错误

        *posOut = (posIn - oneFreedomCorrectParams.detTheta) * oneFreedomStructureParams.transConst;    // 计算末端位置
    }
    /* 逆解求解 */
    else      
        if(fabs(oneFreedomStructureParams.transConst) < oneFreedomStructureParams.err)
            return INPUT_ERROR;        // 返回： 输入错误
        else
        {
            *posOut = posIn / oneFreedomStructureParams.transConst + oneFreedomCorrectParams.detTheta;      // 计算关节位置
            /* 判断角度是否超出范围 */
            if(judgeAngle(angleRegion, posOut, 1) == EXCEED_REGION)
                return EXCEED_REGION;        // 返回： 输入错误
        }
    return SOLVE_OVER;        // 返回：求解完成
}


/*************************************************************
* 输入：速度正逆运动学求解的输入参数，主动关节/末端解，
*      从动关节解，解的类型（主从杆的构型），求解模式
* 输出：求解标志位
* 
* 功能：进行速度正逆运动学求解
**************************************************************/
int calOneFreedomVelocity(double velIn, double *velOut, int mode)
{

    if(mode == FORWARD_KINEMATICS)     // 正解求解
        *velOut = velIn * oneFreedomStructureParams.transConst;    // 计算末端速度
    else      // 逆解求解
        if(fabs(oneFreedomStructureParams.transConst) < oneFreedomStructureParams.err)
            return INPUT_ERROR;        // 返回： 输入错误
        else
            *velOut = velIn / oneFreedomStructureParams.transConst;      // 计算关节速度

    return SOLVE_OVER;        // 返回：求解完成
}


/*************************************************************
* 输入：加速度正逆运动学求解的输入参数，主动关节/末端解，
*      从动关节解，解的类型（主从杆的构型），求解模式
* 输出：求解标志位
* 
* 功能：进行加速度正逆运动学求解
**************************************************************/
int calOneFreedomAcceleration(double acceIn, double *acceOut, int mode)
{

    if(mode == FORWARD_KINEMATICS)     // 正解求解
        *acceOut = acceIn * oneFreedomStructureParams.transConst;    // 计算末端速度
    else      // 逆解求解
        if(fabs(oneFreedomStructureParams.transConst) < oneFreedomStructureParams.err)
            return INPUT_ERROR;        // 返回： 输入错误
        else
            *acceOut = acceIn / oneFreedomStructureParams.transConst;      // 计算关节速度

    return SOLVE_OVER;        // 返回：求解完成
}




