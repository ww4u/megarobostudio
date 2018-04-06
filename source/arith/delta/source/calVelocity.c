//////////////////////////////////////////////////////////////
//             速度计算模块 calVelocity.c
//                    Ver 0.1
//
// 功能：速度的正逆解求解
//
// Created by StarSky at March 19, 2018
//////////////////////////////////////////////////////////////

#include "configure.h"

/*************************************************************
* 输入：主从关节和末端的位置参数，主关节/末端速度参数，
*      主关节/末端速度解数组，求解模式
* 输出：求解标志位
* 
* 功能：进行速度正逆运动学求解，返回加速度为主从关节角速度和末端速度
**************************************************************/
int velocityKinematics(double posIn[6], double velIn[2], double velOut[6], int mode)
{
    double s11, c11, s12, c12, s21, c21, s22, c22, s11c21, s12c22;
    double L1, L2, A, B, C;
    double J[16];
    int i, tmp;
    
    /* 常用三角函数 */
    s11 = sin(posIn[0]);    // sin(t11)
    c11 = cos(posIn[0]);    // cos(t11)
    s12 = sin(posIn[1]);    // sin(t12)
    c12 = cos(posIn[1]);    // cos(t12)
    s21 = sin(posIn[2]);    // sin(t21)
    c21 = cos(posIn[2]);    // cos(t21)
    s22 = sin(posIn[3]);    // sin(t22)
    c22 = cos(posIn[3]);    // cos(t22)
    s11c21 = sin(posIn[0] - posIn[2]);    // sin(t11-t21)
    s12c22 = sin(posIn[1] - posIn[3]);    // sin(t12-t22)

    /* 机构参数 */
    L1 = deltaStructureParams.L1;
    L2 = deltaStructureParams.L2;

    /* 初始化数组 */
    for(i = 0;i < 16;i ++)
        J[i] = 0;

    /* 计算速度雅克比矩阵 */
    J[0] = -c21 / (L1 * s11c21);  
    J[1] = -s21 / (L1 * s11c21);
    J[4] = -c22 / (L1 * s12c22);
    J[5] = -s22 / (L1 * s12c22);

    J[10] = c11 / (L2 * s11c21);
    J[11] = s11 / (L2 * s11c21);
    J[14] = c12 / (L2 * s12c22);
    J[15] = s12 / (L2 * s12c22);

    /* 奇异位检测 */
    if(fabs(s11c21) < ANGULAR_LIMIT || fabs(s12c22) < ANGULAR_LIMIT)
        return SINGULAR_POSITION;        // 返回：奇异位置

    /* 正解求解 */
    if(mode == FORWARD_KINEMATICS)
    {
        /* 保存主动关节速度 */
        velOut[0] = velIn[0];
        velOut[1] = velIn[1];

        A = J[5] * velIn[0] - J[1] * velIn[1];
        B = J[4] * velIn[0] - J[0] * velIn[1];
        C = J[0] * J[5] - J[1] * J[4];
        
        velOut[4] = A / C;        // 末端x轴速度
        velOut[5] = -B / C;        // 末端y轴速度
        velOut[2] = J[10] * velOut[4] + J[11] * velOut[5];    // 从动关节1速度
        velOut[3] = J[14] * velOut[4] + J[15] * velOut[5];    // 从动关节2速度
    
    }
    /* 速度逆解求解 */
    else
    {  
        /* 计算主从关节速度值 */
        for(i = 0;i < 4;i ++)
        {
            tmp = 4 * i;
            velOut[i] = (J[tmp] + J[tmp + 2]) * velIn[0] + (J[tmp + 1] + J[tmp + 3]) * velIn[1];
        }

        /* 保存末端速度 */
        velOut[4] = velIn[0];
        velOut[5] = velIn[1];
    }

    return SOLVE_OVER;        // 返回：求解完成
}
