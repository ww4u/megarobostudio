//////////////////////////////////////////////////////////////
//            角度处理模块 angleOperate.c
//                    Ver 0.1
//
// 功能：标准化角度及其区间
//
// Created by StarSky at March 15, 2018
//////////////////////////////////////////////////////////////

#include "configure.h"


double angleErr;        // 定义角度误差(rad)

/*************************************************************
* 输入：待标准化的角度值，角度元素的个数
* 输出：无
* 
* 功能：将角度值标准化在[0,2*pi)范围
**************************************************************/
void angleStandard(double *angle, int length)
{
    int i;
    double det;        // 定义误差

    for(i = 0;i < length;i ++)
    {
        if(angle[i] >= 2*PI)        // 角度超出上边界
            angle[i] -= 2.0*PI*(int)(angle[i] / (2*PI));
        else if(angle[i] < 0)        // 角度超出下边界
            angle[i] += 2.0 * PI * ((int)(-angle[i] / (2 * PI)) + 1);

        det = angle[i] - 2 * PI;        // 计算偏差
        if(angle[i] < angleErr && angle[i] > -angleErr)        // 消除误差
            angle[i] = 0;
    }
}


/*************************************************************
* 输入：待标准化的角度区间（单个）
* 输出：无
* 
* 功能：将角度闭区间标准化在[0,2*pi)范围，且为逆时针指向
*      情形1：若左边界 < 右边界，表示正常区间
*      情形2：若左边界 = 右边界，表示满区间，即[0,2*PI)
*      情形3：若左边界 > 右边界，表示补区间，即调换左右边界后的区间为其补值
*            如区间[1.5*PI,PI]表示的为区间(PI,1.5*PI)的补值
**************************************************************/
void angleRegionStandard(double *angleRegion)
{
    double det;

    angleStandard(angleRegion, 2);        // 将区间左右边界标准化
    det = angleRegion[0] - angleRegion[1];        // 计算区间大小

    if(det < angleErr && det > angleErr)        // 忽略误差
    {
        angleRegion[0] = angleRegion[1];
    }
}


/*************************************************************
* 输入：角度区间（单个），角度值，角度数组长度
* 输出：标志位
* 
* 功能：判断角度是否在区间内（不改变输入参数值）
**************************************************************/
int judgeAngle(double angleRegion[2], double *angle, int length)
{
    int i;
    double tmp, angleRegionTmp[2], *angleTmp = &tmp;

    angleTmp = (double *)malloc(length * sizeof(double));        // 申请内存

    /* 数组赋值，以便后续处理不改变输入参数值 */
    for(i = 0;i < 2;i ++)
        angleRegionTmp[i] = angleRegion[i];
    for(i = 0;i < length;i ++)
        angleTmp[i] = angle[i];

    angleStandard(angleTmp, length);            // 角度标准化
    angleRegionStandard(angleRegionTmp);        // 标准化角度区间

    if(angleRegionTmp[0] == angleRegionTmp[1])        // 区间为[0, 2*PI)
    {
        free(angleTmp);        // 释放内存
        return IN_REGION;        // 返回：角度在区间内
    }
    else if(angleRegionTmp[0] < angleRegionTmp[1])        // 正常区间
    {
        for(i = 0;i < length;i ++)
            if(angleTmp[i] > angleRegionTmp[1] || angleTmp[i] < angleRegionTmp[0])
            {
                free(angleTmp);        // 释放内存

                return EXCEED_REGION;        // 返回：角度超出范围
            }

        free(angleTmp);        // 释放内存

        return IN_REGION;        // 返回：角度在区间内
    }
    else        // 补区间
    {
        for(i = 0;i < length;i ++)
            if(angleTmp[i] > angleRegionTmp[1] && angleTmp[i] < angleRegionTmp[0])
            {
                free(angleTmp);        // 释放内存

                return EXCEED_REGION;        // 返回：角度超出范围
            }

        free(angleTmp);        // 释放内存

        return IN_REGION;        // 返回：角度在区间内
    }
}
