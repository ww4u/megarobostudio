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
//#include <math.h>
#include "arm_math.h"
#include "pvtCubicSolution.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



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
    u16 calcCount;

    f32 calcOutput;
    u32 i;

    //时间使用64位是因为用的是查找法(数值解)，当精度不够时可能导致查找不到在范围内的目标值
    //如果使用解析解的方式则不需要
    f64 curTime;
    f64 lastTime;
    f64 maxTime;
    f64 minTime;
    
    f32 cubeMotionTime;
    f32 squareMotionTime;
    f32 curPost;   
    f32 threeTermCoef;
    f32 twoTermCoef;
    f32 posnOffset;

    
    pPvtCalcData->targetStep -= pPvtCalcData->startPosn;

    //有等于一步的时间，等于一步时直接进入最后一步计算
    if (pPvtCalcData->motionStep > 1)    
    {
        //初始化时间
        minTime  = 0;
        lastTime = 0;
        if ((1 == pPvtCalcData->targetStep) ||     //第一步的时候需要在motionTime范围内查找
            (0.0f == pPvtCalcData->startSpeed))    //或者初始速度为0则认为两段之间速度变化较大则重新在motionTime范围内查找
        {
            curTime = pPvtCalcData->motionTime / pPvtCalcData->motionStep;
            maxTime = pPvtCalcData->motionTime;

            pPvtCalcData->lastStepSpeed = 0;
        }
        else
        {
            curTime = lastTime;
            maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
        }
        
        //提前算好三次项和二次项的系数
        cubeMotionTime   = CUBE(pPvtCalcData->motionTime);
        squareMotionTime = SQUARE(pPvtCalcData->motionTime);
        
        //2 * (p0 - p1) / T^3 + (v0 + v1) / T^2 = (v0 + v1) / T^2 - 2 * (p1 - p0) / T^3
        threeTermCoef = ((pPvtCalcData->startSpeed + pPvtCalcData->endSpeed) / squareMotionTime) 
                        - (pPvtCalcData->motionStep * 2.0f / cubeMotionTime);

        //3 * (p1 - p0) / T^2 - (2v0 + v1) / T
        twoTermCoef   = (pPvtCalcData->motionStep * 3.0f / squareMotionTime) 
                        - (pPvtCalcData->startSpeed * 2.0f + pPvtCalcData->endSpeed) / pPvtCalcData->motionTime;

        //P(t) = threeTermCoef * t^3 + twoTermCoef * t^2 + v0 * t + p0
        //P(t) = pPvtCalcData->targetStep
        //P(t) - p0 = threeTermCoef * t^3 + twoTermCoef * t^2 + v0 * t
        //P(t) - p0 = pPvtCalcData->targetStep - p0
        
        for (i = 0;i < (pPvtCalcData->motionStep - 1);i++)    //最后一步不用算，直接用motionTime - curTime
        { 
            calcCount = 0;       
            curTime += pPvtCalcData->lastStepSpeed;    //在上一步的时间轴上加上上一步的速度作为这一步的计算初始时间
            if (curTime > pPvtCalcData->motionTime)
            {
                curTime = pPvtCalcData->motionTime;
            }

            //P(t) = threeTermCoef * t^3 + twoTermCoef * t^2 + v0 * t
            curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + pPvtCalcData->startSpeed) * curTime;
            
            //跟目标值的误差太大则需要继续计算
            posnOffset = curPost - pPvtCalcData->targetStep;
            while ((posnOffset > pPvtCalcData->maxOffset) ||    //跟目标值得误差太大继续计算
                   (posnOffset < pPvtCalcData->minOffset))
            {
                calcCount++;
                
                if (calcCount >= PVT_ONE_POINT_CALC_TIMES_MAX)
                {
                    //计算次数过多则结束计算，报错
                    errorCode = PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_1;
                    break;
                }
                else if (PVT_ONE_POINT_CALC_TIMES_MAX / 2 == calcCount)  //超过一半限制后先加大maxTime
                {
                    maxTime = pPvtCalcData->motionTime;
                }

                //二分查找，改变时间继续计算
                if (posnOffset > pPvtCalcData->maxOffset)
                {
                    maxTime = curTime;
                    curTime = (curTime + minTime) * 0.5f;
                }
                else
                {
                    minTime = curTime;
                    curTime = (curTime + maxTime) * 0.5f;
                }

                //P(t) = threeTermCoef * t^3 + twoTermCoef * t^2 + v0 * t
                curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + pPvtCalcData->startSpeed) * curTime;
                
                posnOffset = curPost - pPvtCalcData->targetStep;
            }

            //发生计算次数过多错误结束计算，报错
            if ((PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_1) == errorCode)
            {
                break;
            }
            
            //记录这一步和上一步的间隔时间，也就是速度
            pPvtCalcData->lastStepSpeed = curTime - lastTime;    
            if (pPvtCalcData->lastStepSpeed <= 0.0f)
            {
                errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
                pPvtCalcData->lastStepSpeed = 0;
            }
            
            //更新时间，准备下一步的计算
            pPvtCalcData->targetStep++;
            minTime = curTime;
            lastTime = curTime;
            maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
            if (maxTime > pPvtCalcData->motionTime)
            {
                maxTime = pPvtCalcData->motionTime;
            }
            
            //将时间按照线间步平分
            calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                          pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                          pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

            //做个上下限保护
            if (calcOutput > pPvtCalcData->fpgaPwmClock)
            {
                calcOutput -= pPvtCalcData->fpgaPwmClock;
                
                pPvtCalcData->errorTime = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                
                //多出来的部分作为等待时间
                pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                      (u32)calcOutput, 
                                                      BUFFOPERT_WRITE, 
                                                      (void *)pPvtCalcData, 
                                                      pPvtCalcData->pContext);
                                             
                //统计下时间
                pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
                
                calcOutput = pPvtCalcData->fpgaPwmClock;
                
                //系统错误码置位
                errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_1;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //系统错误码置位
                errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_1;
            }
            
            //统计下误差和时间
            pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
            pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

            pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                                  (u32)calcOutput, 
                                                  BUFFOPERT_WRITE, 
                                                  (void *)pPvtCalcData, 
                                                  pPvtCalcData->pContext);
            
        }    //for (i = 0;i < (pPvtCalcData->motionStep - 1);i++)
    }
    else
    {
        curTime = 0;
    } 
    
    /*****************************最后一步*******************************/
    //最后一步的时间直接用motionTime减去上一步的时间位置
    pPvtCalcData->lastStepSpeed = pPvtCalcData->motionTime - curTime;  
    
    if (pPvtCalcData->lastStepSpeed <= 0.0f)
    {
        errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
        pPvtCalcData->lastStepSpeed = 0;
    }
    else
    {
        pPvtCalcData->targetStep++;    //CJ 2018.12.10 Add
        
        //将时间按照线间步平分
        calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                      pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                      pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

        //做个上限保护
        if (calcOutput > pPvtCalcData->fpgaPwmClock)
        {
            calcOutput -= pPvtCalcData->fpgaPwmClock;
            
            pPvtCalcData->errorTime = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
            
            //多出来的部分作为等待时间
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                  (u32)calcOutput, 
                                                  BUFFOPERT_WRITE, 
                                                  (void *)pPvtCalcData, 
                                                  pPvtCalcData->pContext);
                                         
            //统计下时间
            pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
            
            calcOutput = pPvtCalcData->fpgaPwmClock;
            
            //系统错误码置位
            errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_1;
        }
        else if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //系统错误码置位
            errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_1;
        }
        
        //统计下误差和时间
        pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
        pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
        
        pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              (void *)pPvtCalcData, 
                                              pPvtCalcData->pContext);
        /*****************************最后一步*******************************/
    }
    
    //因为之前P(t) - p0 = pPvtCalcData->targetStep - p0，所以需要恢复目标微步
    pPvtCalcData->targetStep += pPvtCalcData->startPosn;

    return errorCode;
}



/*******************************************文件尾********************************************/
