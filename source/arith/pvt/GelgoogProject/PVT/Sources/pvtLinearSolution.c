/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtLinearSolution.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.11.23;
历史版本:  无;
*********************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arm_math.h"
#include "pvtLinearSolution.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvtLinearCurveCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvtLinearCurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;
    f32 calcOutput;
    u32 i;
    f64 curTime = 0;    //临时改成64位    NICK MARK
    u32 poseOffset;
    f64 lastTime;       //临时改成64位    NICK MARK

    f32 startSpeed;
    f32 endSpeed;
    f32 peakSpeed;
    f32 midPosition;
    f32 accelCoef;      //A
    f32 decelCoef;      //D
    f32 decelOffset;    //E
    f32 decelPost;      //F
    f32 deltaPost;
    u32 accelCount;
    u32 decelCount;
    f32 squareDecelOffset;
    f32 squareStartSpeed;
    f32 sqrtDeltaPost;

    u32 startPosition = (u32)(pPvtCalcData->startPosn * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);
    u32 endPosition   = (u32)(pPvtCalcData->endPosn   * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);
    

    //转换量纲
    startSpeed = fabs(pPvtCalcData->startSpeed * pPvtCalcData->posnConvertInfo.posnToStep);
    endSpeed   = fabs(pPvtCalcData->endSpeed   * pPvtCalcData->posnConvertInfo.posnToStep);
    poseOffset = abs(endPosition - startPosition);

    //统计下微步数，正向加，反向减
    if (DIR_FORWARD == pPvtCalcData->lastStepDir)
    {
        pPvtCalcData->pvtSteps += poseOffset;
    }
    else
    {
        pPvtCalcData->pvtSteps -= poseOffset;
    }
            
    //初始化时间
    if ((1 == pPvtCalcData->targetStep) ||
        (0.0f == startSpeed))
    {
        pPvtCalcData->lastStepTime = pPvtCalcData->startTime;
        pPvtCalcData->lastStepSpeed = 0;
        lastTime = 0;
    }
    else
    {
        lastTime = pPvtCalcData->lastStepTime - pPvtCalcData->startTime;
    }

    //提前算各项的系数以及判断条件
    peakSpeed = 2.0f * poseOffset / pPvtCalcData->motionTime - 0.5f * (endSpeed + startSpeed);
    midPosition = 0.5f * (endPosition + startPosition) + 
                  0.125f * (startSpeed - endSpeed) * pPvtCalcData->motionTime;
    accelCoef = (peakSpeed - startSpeed) / pPvtCalcData->motionTime;
    decelCoef = (endSpeed - peakSpeed) / pPvtCalcData->motionTime;
    decelOffset = 2.0f * peakSpeed - endSpeed;
    decelPost = startPosition + 0.25f * pPvtCalcData->motionTime * (startSpeed + endSpeed - 2.0f * peakSpeed);

    accelCount = (u32)midPosition - startPosition;
    decelCount = poseOffset - accelCount;

    squareStartSpeed  = SQUARE(startSpeed);
    for (i = 0;i < accelCount;i++)
    {
        //根据位置直接求出时间
        deltaPost = squareStartSpeed - 4.0f * accelCoef * ((f32)startPosition - pPvtCalcData->targetStep);
        if (deltaPost < 0.0f)
        {
            deltaPost = 0;
        }
        arm_sqrt_f32(deltaPost, &sqrtDeltaPost);
        curTime = (sqrtDeltaPost - startSpeed) * 0.5f / accelCoef;
        
        pPvtCalcData->targetStep++;    //开始计算下一个目标
        
        pPvtCalcData->lastStepSpeed = curTime - lastTime;    //记录这一步和上一步的间隔时间，也就是速度
        lastTime = curTime;

        //时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
        calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                     pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

        //做个上下限保护
        if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
        {
            calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
            
            //系统错误码置位
            errorCode = PVT_CALC_GREAT_UPER_LIMIT;
        }
        else if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //系统错误码置位
            errorCode = PVT_CALC_LESS_LOWER_LIMIT;
        }

        calcOutput += pPvtCalcData->errorTime;
        pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

        pPvtCalcData->outpBufferFill(chanNum, (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              pPvtCalcData->pContext);
    }

    squareDecelOffset = SQUARE(decelOffset);
    for (i = 0;i < decelCount;i++)
    {
        //根据位置直接求出时间
        deltaPost = squareDecelOffset - 4.0f * decelCoef * (decelPost - pPvtCalcData->targetStep);
        if (deltaPost < 0.0f)
        {
            deltaPost = 0;
        }
        arm_sqrt_f32(deltaPost, &sqrtDeltaPost);
        curTime = (sqrtDeltaPost - decelOffset) * 0.5f / decelCoef;
        
        pPvtCalcData->targetStep++;    //开始计算下一个目标
        
        pPvtCalcData->lastStepSpeed = curTime - lastTime;    //记录这一步和上一步的间隔时间，也就是速度
        lastTime = curTime;

        //时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
        calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                     pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

        //做个上下限保护
        if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
        {
            calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
            
            //系统错误码置位
            errorCode = PVT_CALC_GREAT_UPER_LIMIT;
        }
        else if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //系统错误码置位
            errorCode = PVT_CALC_LESS_LOWER_LIMIT;
        }

        calcOutput += pPvtCalcData->errorTime;
        pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

        pPvtCalcData->outpBufferFill(chanNum, (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              pPvtCalcData->pContext);
    }

    pPvtCalcData->lastStepTime = curTime + pPvtCalcData->startTime;   

    return errorCode;
}



/*******************************************文件尾********************************************/
