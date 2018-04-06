//////////////////////////////////////////////////////////////
//          加速度计算模块 calAcceleration.c
//                    Ver 0.1
//
// 功能：加速度的正逆解求解
//
// Created by StarSky at March 29, 2018
//////////////////////////////////////////////////////////////

#include "configure.h"

/*************************************************************
* 输入：主从关节和末端的位置和速度参数，主关节/末端加速度参数，
*      主关节/末端加速度解数组，求解模式
* 输出：求解标志位
* 
* 功能：进行加速度正逆运动学求解，返回加速度为主从关节角加速度和末端加速度
**************************************************************/
int accelerationKinematics(double posIn[6], double velIn[6], double acceIn[2], double acceOut[6], int mode)
{
    double s11, c11, s12, c12, s21, c21, s22, c22, s11c21, s12c22, c11c21, c12c22;
    double dt11, dt12, dt21, dt22, v1, v2, L1, L2, ddt1, ddt2;
    double dJ[4], b[4];
    double tmp;

    /* 常用三角函数 */
    s11 = sin(posIn[0]);        // sin(t11)
    c11 = cos(posIn[0]);        // cos(t11)
    s12 = sin(posIn[1]);        // sin(t12)
    c12 = cos(posIn[1]);        // cos(t12)
    s21 = sin(posIn[2]);        // sin(t21)
    c21 = cos(posIn[2]);        // cos(t21)
    s22 = sin(posIn[3]);        // sin(t22)
    c22 = cos(posIn[3]);        // cos(t22)
    s11c21 = sin(posIn[0] - posIn[2]);        // sin(t11-t21)
    s12c22 = sin(posIn[1] - posIn[3]);        // sin(t12-t22)
    c11c21 = cos(posIn[0] - posIn[2]);        // cos(t11-t21)
    c12c22 = cos(posIn[1] - posIn[3]);        // cos(t12-t22)
    
    /* 提取速度 */
    dt11 = velIn[0];
    dt12 = velIn[1];
    dt21 = velIn[2];
    dt22 = velIn[3];
    v1 = velIn[4];
    v2 = velIn[5];

    L1 = deltaStructureParams.L1;
    L2 = deltaStructureParams.L2;

    ddt1 = acceIn[0];
    ddt2 = acceIn[1];
    
    /* 奇异位置检测 */
    if(fabs(s11c21) < ANGULAR_LIMIT || fabs(s12c22) < ANGULAR_LIMIT)
        return SINGULAR_POSITION;        // 返回：奇异位置

    /* 加速度雅克比矩阵 */
    dJ[0] = c21;     
    dJ[1] = s21;
    dJ[2] = c22;
    dJ[3] = s22;

    /* 右端常数项 */
    b[0] = L1 * c11c21 * (dt21 - dt11) * dt11 - dt21 * (-v1 * s21 + v2 * c21);
    b[1] = L1 * c12c22 * (dt22 - dt12) * dt12 - dt22 * (-v1 * s22 + v2 * c22);
    b[2] = L2 * c11c21 * (dt11 - dt21) * dt21 - dt11 * (-v1 * s11 + v2 * c11);
    b[3] = L2 * c12c22 * (dt12 - dt22) * dt22 - dt12 * (-v1 * s12 + v2 * c12);

    /* 正解 */
    if(mode == FORWARD_KINEMATICS)
    {
        acceOut[0] = acceIn[0];        // 保存主动关机1角加速度
        acceOut[1] = acceIn[1];        // 保存主动关机2角加速度

        b[0] = b[0] - L1 * s11c21 * ddt1;
        b[1] = b[1] - L1 * s12c22 * ddt2;
        tmp = (dJ[0] * dJ[3] - dJ[1] * dJ[2]);
        acceOut[4] = (dJ[3] * b[0] - dJ[1] * b[1]) / tmp;     // 末端x轴加速度
        acceOut[5] = -(dJ[2] * b[0] - dJ[0] * b[1]) / tmp;     // 末端y轴加速度
        
        b[2] =  b[2] - acceOut[4] * c11 - acceOut[5] * s11;
        b[3] =  b[3] - acceOut[4] * c12 - acceOut[5] * s12;
        acceOut[2] = -b[2] / (L2 * s11c21);       // 从动杆关节1角加速度
        acceOut[3] = -b[3] / (L2 * s12c22);       // 从动杆关节2角加速度    
    }
    /* 逆解 */
    else        
    {
        acceOut[4] = acceIn[0];        // 保存末端x加速度
        acceOut[5] = acceIn[1];        // 保存末端y加速度

        b[0] = b[0] - ddt1 * c21 - ddt2 * s21;
        b[1] = b[1] - ddt1 * c22 - ddt2 * s22;
        acceOut[0] = b[0] / (L1 * s11c21);       // 主动杆关节1角加速度
        acceOut[1] = b[1] / (L1 * s12c22);       // 主动杆关节2角加速度
        
        b[2] =  b[2] - ddt1 * c11 - ddt2 * s11;
        b[3] =  b[3] - ddt1 * c12 - ddt2 * s12;
        acceOut[2] = -b[2] / (L2 * s11c21);       // 从动杆关节1角加速度
        acceOut[3] = -b[3] / (L2 * s12c22);       // 从动杆关节2角加速度    
    }

    return SOLVE_OVER;        // 返回：求解完成

}