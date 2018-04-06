//////////////////////////////////////////////////////////////
//               配置文件 oneFreedom.h
//                    Ver 0.1
//
// 功能：包含各头文件、宏定义和机构求解参数结构体
//
// Created by StarSky at April 4, 2018
//////////////////////////////////////////////////////////////

#ifndef _ONEFREEDOM_H_
#define _ONEFREEDOM_H_

/* 机构参数结构体 */
struct oneFreedomStructureParamsObj
{
    double transConst;        // 角度转换位移系数

    double angleLim[2];        // 关节角度限制
    double err;        // 误差的定义

    int mode;        // 求解模式
};

/* 机构矫正参数结构体 */
struct oneFreedomCorrectParamsObj
{
    int length;
    double detTheta;        // 实际机构关节在零位时的角度
};


int calOneFreedomPosition(double posIn, double *posOut, int mode);
int calOneFreedomVelocity(double velIn, double *velOut, int mode);
int calOneFreedomAcceleration(double acceIn, double *acceOut, int mode);

#endif