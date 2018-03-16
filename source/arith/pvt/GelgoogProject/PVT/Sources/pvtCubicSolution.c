/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtCubicSolution.c；
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
#include "pvtCubicSolution.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    PVT_CALC_MODIFY_ERROR    1



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvtCubicCurveCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvtCubicCurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;
    u16 calcCount = 0;

    f32 calcOutput;
    u32 i;
    f64 curTime = 0;    //临时改成64位    NICK MARK
    u32 poseOffset;
    f64 lastTime;       //临时改成64位    NICK MARK
    f64 maxTime;        //临时改成64位    NICK MARK
    f64 minTime;        //临时改成64位    NICK MARK

    f32 startSpeed;
    f32 endSpeed;
    
    f32 cubeMotionTime;
    f32 squareMotionTime;
    f32 curPost;   
    f32 threeTermCoef;
    f32 twoTermCoef;
    f32 posnOffset;


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
    minTime = 0;
    if ((1 == pPvtCalcData->targetStep) ||   //第一步的时候需要在motionTime范围内查找
        (0.0f == startSpeed))                //或者初始速度为0则认为两段之间速度变化较大则重新在motionTime范围内查找
    {
        if (poseOffset != 0)
        {
            curTime = pPvtCalcData->motionTime / poseOffset;
            maxTime = pPvtCalcData->motionTime;
        }

        pPvtCalcData->lastStepTime  = pPvtCalcData->startTime;
        pPvtCalcData->lastStepSpeed = 0;
        lastTime = 0;
    }
    else
    {
        lastTime = pPvtCalcData->lastStepTime - pPvtCalcData->startTime;
        curTime = lastTime;
        maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
    }
    
    //提前算好三次项和二次项的系数
    cubeMotionTime   = CUBE(pPvtCalcData->motionTime);
    squareMotionTime = SQUARE(pPvtCalcData->motionTime);
    
    threeTermCoef = ((startSpeed + endSpeed) / squareMotionTime) - (poseOffset * 2.0f / cubeMotionTime);
    twoTermCoef   = (poseOffset * 3.0f / squareMotionTime) - (startSpeed * 2.0f + endSpeed) / pPvtCalcData->motionTime;

    pPvtCalcData->targetStep -= startPosition;

#if PVT_CALC_MODIFY_ERROR
    if (poseOffset > 1)    //小于等于一步时直接进入最后一步计算
    {
    
        for (i = 0;i < (poseOffset - 1);i++)    //最后一步不用算
#else
    for (i = 0;i < poseOffset;i++)
#endif
        { 
            calcCount = 0;       
            curTime += pPvtCalcData->lastStepSpeed;    //在上一步的时间轴上加上上一步的速度作为这一步的计算初始时间
            if (curTime > pPvtCalcData->motionTime)
            {
                curTime = pPvtCalcData->motionTime;
            }

            curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + startSpeed) * curTime;
            
            posnOffset = curPost - pPvtCalcData->targetStep;
            while ((posnOffset > TARGET_REAL_OFFSET_P) ||    //跟目标值得误差太大继续计算
                   (posnOffset < TARGET_REAL_OFFSET_N))
            {
                calcCount++;
                if (calcCount > PVT_ONE_POINT_CALC_TIMES_MAX)
                {
                    //计算次数过多则结束计算，报错
                    errorCode = PVT_CALC_TOO_MANY_CALC_TIMES;
                    break;
                }
                
                if (posnOffset > TARGET_REAL_OFFSET_P)
                {
                    maxTime = curTime;
                    curTime = (curTime + minTime) * 0.5f;
                }
                else
                {
                    minTime = curTime;
                    curTime = (curTime + maxTime) * 0.5f;
                }
                curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + startSpeed) * curTime;
                
                posnOffset = curPost - pPvtCalcData->targetStep;
            }

            //发生计算次数过多错误结束计算，报错
            if (PVT_CALC_TOO_MANY_CALC_TIMES == errorCode)
            {
                break;
            }
            
            pPvtCalcData->targetStep++;    //开始计算下一个目标
            
            pPvtCalcData->lastStepSpeed = curTime - lastTime;    //记录这一步和上一步的间隔时间，也就是速度

            minTime = curTime;
            lastTime = curTime;
            maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
            if (maxTime > pPvtCalcData->motionTime)
            {
                maxTime = pPvtCalcData->motionTime;
            }

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

#if PVT_CALC_MODIFY_ERROR
    }
    
    //最后一步
    pPvtCalcData->lastStepSpeed = pPvtCalcData->motionTime - curTime;    //最后一步的时间直接用motionTime减去上一步的时间位置
    
    //时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
    calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                 pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

    //做个上限保护
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
    //最后一步
    
    pPvtCalcData->targetStep++;    //开始计算下一个目标
    
#endif

    pPvtCalcData->targetStep += startPosition;

#if PVT_CALC_MODIFY_ERROR
    pPvtCalcData->lastStepTime = pPvtCalcData->motionTime + pPvtCalcData->startTime;
#else
    pPvtCalcData->lastStepTime = curTime + pPvtCalcData->startTime;
#endif

    return errorCode;
}



/*******************************************文件尾********************************************/
