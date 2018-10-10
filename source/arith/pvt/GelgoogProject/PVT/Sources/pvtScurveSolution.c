/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtScurveSolution.c；
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
#include "pvtScurveSolution.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvtScurveCalc
实现功能: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
u8 pvtScurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;
    u16 calcCount = 0;

    f32 calcOutput;
    u32 i;

    //时间使用64位是因为用的是查找法(数值解)，当精度不够时可能导致查找不到在范围内的目标值
    //如果使用解析解的方式则不需要
    f64 curTime = 0;
    f64 lastTime;
    f64 maxTime;
    f64 minTime;
    
    f32 cubeMotionTime;
    f32 squareMotionTime;
    f32 curPost;   
    f32 fourTermCoef;
    f32 threeTermCoef;
    f32 posnOffset;
    
    f32 uniformSpeed;
    f32 accTime;
    f32 decTime;
    f32 accTimeHalf;
    f32 decTimeHalf;
    f32 uniformTime;
     
    u32 accPosn;
    u32 decPosn;
    u32 uniformStep;
    u32 decStep;
     
    f32 realAccPosn;
    f32 realDecPosn;
    f32 realStep;
    f32 targetStep;

    f32 squareSpeed;
    

    //有一步的情况，等于一步时直接进入最后一步计算
    if (pPvtCalcData->motionStep > 1)    
    {         
        //S曲线，分为三段，I、II和III段，简单点称为加速、匀速和减速段
        //实际上I和III段可能的情况是加速减速、加速加速、减速加速、减速减速
        
        //先根据PVT0、PVT1、Sacc(加速(I)段占比)、Sdec(减速(II)段占比)计算出匀速(II)段的速度Vunf
        accTime = pPvtCalcData->motionTime * pPvtCalcData->endPoint.accScale / SCALE_MAGNIFICATION;
        decTime = pPvtCalcData->motionTime * pPvtCalcData->endPoint.decScale / SCALE_MAGNIFICATION;
        accTimeHalf = accTime * 0.5f;
        decTimeHalf = decTime * 0.5f;

        //vu = ((p1 - p0) - (v0 * sacc / 2 + v1 * sdec / 2) * T) / T * (1 - sacc / 2 - sdec / 2)
        //vu = ((p1 - p0) - (v0 * Tacc / 2 + v1 * Tdec / 2)) / (T - Tacc / 2 - Tdec / 2)
        uniformSpeed = (pPvtCalcData->motionStep 
                        - (pPvtCalcData->startSpeed * accTimeHalf + pPvtCalcData->endSpeed * decTimeHalf)) / 
                       (pPvtCalcData->motionTime - accTimeHalf - decTimeHalf);

        //根据平均速度判断是否需要重新计算加减速时间
        if (0.0f == uniformSpeed)
        {
            uniformSpeed = 0;

            squareSpeed = 2.0f / (pPvtCalcData->startSpeed * pPvtCalcData->startSpeed + 
                                  pPvtCalcData->endSpeed   * pPvtCalcData->endSpeed);

            accTime = pPvtCalcData->motionStep * pPvtCalcData->startSpeed * squareSpeed;
            decTime = pPvtCalcData->motionStep * pPvtCalcData->endSpeed * squareSpeed;
            
            accTimeHalf = accTime * 0.5f;
            decTimeHalf = decTime * 0.5f;
        }

        //计算I段的结束位置和III段的起始位置
        realAccPosn = accTimeHalf * (uniformSpeed + pPvtCalcData->startSpeed);
        realDecPosn = decTimeHalf * (uniformSpeed + pPvtCalcData->endSpeed);
        accPosn = (u32)realAccPosn;
        decPosn = pPvtCalcData->motionStep - (u32)realDecPosn;


        //加速段(I)开始点的PVT分别为:   startPosn、startSpeed、  0
        //加速段(I)结束点的PVT分别为:   accPosn、  uniformSpeed、accTime
        //减速段(III)开始点的PVT分别为: decPosn、  uniformSpeed、motionTime - decTime
        //减速段(III)结束点的PVT分别为: endPosn、  endSpeed、    motionTime

        /*************************************解算加速(I)段************************************************************/
        if (accPosn > 0)
        {
            //提前算好三次项和二次项的系数
            cubeMotionTime   = CUBE(accTime);
            squareMotionTime = SQUARE(accTime);
            
            //(v0 - vu) / (2 * Tacc^3)
            fourTermCoef = (pPvtCalcData->startSpeed - uniformSpeed) / (2.0f * cubeMotionTime);

            //(vu - v0) / Tacc^2
            threeTermCoef = (uniformSpeed - pPvtCalcData->startSpeed) / squareMotionTime;

            //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t + p0
            //P(t) = pPvtCalcData->targetStep
            //P(t) - p0 = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
            //P(t) - p0 = pPvtCalcData->targetStep - p0  
            
            //初始化时间
            minTime  = 0;
            lastTime = 0;
            if ((1 == pPvtCalcData->targetStep) ||     //第一步的时候需要在motionTime范围内查找
                (0.0f == pPvtCalcData->startSpeed))    //或者初始速度为0则认为两段之间速度变化较大则重新在motionTime范围内查找
            {
                curTime = accTime / accPosn;
                maxTime = accTime;

                pPvtCalcData->lastStepSpeed = 0;
            }
            else
            {
                curTime = lastTime;
                maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
            }
            realStep = realAccPosn / accPosn;
            
            for (i = 1;i < accPosn;i++)
            {
                calcCount  = 0;
                targetStep = realStep * i;
                
                //在上一步的时间轴上加上上一步的速度作为这一步的计算初始时间
                curTime += pPvtCalcData->lastStepSpeed;    
                if (curTime > accTime)
                {
                    curTime = accTime;
                }

                //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
                curPost = ((fourTermCoef * curTime  + threeTermCoef) * curTime * curTime + pPvtCalcData->startSpeed) * curTime;
                
                //跟目标值的误差太大则需要继续计算
                posnOffset = curPost - targetStep;
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
                        maxTime = accTime;
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

                    //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
                    curPost = ((fourTermCoef * curTime  + threeTermCoef) * curTime * curTime + pPvtCalcData->startSpeed) * curTime;
                    
                    posnOffset = curPost - targetStep;
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
                if (maxTime > accTime)
                {
                    maxTime = accTime;
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
                                                      pPvtCalcData->pContext);
            }

            //I段最后一步
            pPvtCalcData->targetStep++;
            
            pPvtCalcData->lastStepSpeed = accTime - lastTime;    
            if (pPvtCalcData->lastStepSpeed <= 0.0f)
            {
                errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
                pPvtCalcData->lastStepSpeed = 0;
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
                                                  pPvtCalcData->pContext);
        }
        /*************************************解算加速(I)段************************************************************/
        
        
        /*************************************解算匀速(II)段***********************************************************/
        uniformTime = pPvtCalcData->motionTime - accTime - decTime;
        uniformStep = decPosn - accPosn;

        //匀速段有时间才处理
        if (uniformTime > 0.0f)
        {
            //有时间没距离就是等待时间
            if (0 == uniformStep)
            {
                calcOutput = pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                             pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                             pPvtCalcData->errorTime;

                pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;
                
                //多出来的部分作为等待时间
                pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                      (u32)calcOutput, 
                                                      BUFFOPERT_WRITE, 
                                                      pPvtCalcData->pContext);
                                             
                //统计下时间
                pPvtCalcData->timeCount += (u32)calcOutput;
            }
            else
            {
                pPvtCalcData->lastStepSpeed = uniformTime / uniformStep;  
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_2;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                for (i = 0;i < (decPosn - accPosn);i++)
                {
                    //仅做统计用
                    pPvtCalcData->targetStep++;
                    
                    //将时间按照线间步平分
                    calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                                  pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                                  pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

                    //做个上下限保护
                    if (calcOutput > pPvtCalcData->fpgaPwmClock)
                    {
                        /*calcOutput -= pPvtCalcData->fpgaPwmClock;
                        
                        calcOutput += pPvtCalcData->errorTime;
                        pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;
                        
                        //多出来的部分作为等待时间
                        pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                              (u32)calcOutput, 
                                                              BUFFOPERT_WRITE, 
                                                              pPvtCalcData->pContext);
                                                     
                        //统计下时间
                        pPvtCalcData->timeCount += (u32)calcOutput;
                        
                        calcOutput = pPvtCalcData->fpgaPwmClock;*/
                        
                        //系统错误码置位
                        errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_2;

                        //此种模式下匀速段不应出现等待时间，所以退出计算
                        return errorCode;
                    }
                    else if (calcOutput < OUTPUT_PERIOD_MIN)
                    {
                        calcOutput = OUTPUT_PERIOD_MIN;
                        
                        //系统错误码置位
                        errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_2;
                    }
                    
                    //统计下误差和时间
                    pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                    pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

                    pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                                          (u32)calcOutput, 
                                                          BUFFOPERT_WRITE, 
                                                          pPvtCalcData->pContext);
                }
            }
        }
        /*************************************解算匀速(II)段***********************************************************/

        
        /*************************************解算减速(III)段**********************************************************/
        decStep = pPvtCalcData->motionStep - decPosn;
        if (decStep > 1)
        {
            //提前算好三次项和二次项的系数
            cubeMotionTime   = CUBE(decTime);
            squareMotionTime = SQUARE(decTime);
            
            //(v0 - vu) / (2 * Tacc^3)
            fourTermCoef = (uniformSpeed - pPvtCalcData->endSpeed) / (2.0f * cubeMotionTime);

            //(vu - v0) / Tacc^2
            threeTermCoef = (pPvtCalcData->endSpeed - uniformSpeed) / squareMotionTime;
            
            //初始化时间
            minTime  = 0;
            lastTime = 0;
            curTime  = decTime / decStep;
            maxTime  = decTime;
            pPvtCalcData->lastStepSpeed = 0;

            realStep = realDecPosn / decStep;
            
            //少算一步，最后一步不在这里算
            for (i = 1;i < decStep;i++)    
            {
                calcCount = 0; 
                targetStep = realStep * i;

                //在上一步的时间轴上加上上一步的速度作为这一步的计算初始时间
                curTime += pPvtCalcData->lastStepSpeed;    
                if (curTime > decTime)
                {
                    curTime = decTime;
                }

                //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
                curPost = ((fourTermCoef * curTime  + threeTermCoef) * curTime * curTime + uniformSpeed) * curTime;
                
                //跟目标值的误差太大则需要继续计算
                posnOffset = curPost - targetStep;
                while ((posnOffset > pPvtCalcData->maxOffset) ||    //跟目标值得误差太大继续计算
                       (posnOffset < pPvtCalcData->minOffset))
                {
                    calcCount++;
                    
                    if (calcCount >= PVT_ONE_POINT_CALC_TIMES_MAX)
                    {
                        //计算次数过多则结束计算，报错
                        errorCode = PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_3;
                        break;
                    }
                    else if (PVT_ONE_POINT_CALC_TIMES_MAX / 2 == calcCount)  //超过一半限制后先加大maxTime
                    {
                        maxTime = decTime;
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

                    //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
                    curPost = ((fourTermCoef * curTime  + threeTermCoef) * curTime * curTime + uniformSpeed) * curTime;
                    
                    posnOffset = curPost - targetStep;
                }

                //发生计算次数过多错误结束计算，报错
                if ((PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_3) == errorCode)
                {
                    break;
                }
                
                //记录这一步和上一步的间隔时间，也就是速度
                pPvtCalcData->lastStepSpeed = curTime - lastTime;    
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_3;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                
                //更新时间，准备下一步的计算
                pPvtCalcData->targetStep++;
                minTime = curTime;
                lastTime = curTime;
                maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
                if (maxTime > decTime)
                {
                    maxTime = decTime;
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
                                                          pPvtCalcData->pContext);
                                                 
                    //统计下时间
                    pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
                    
                    calcOutput = pPvtCalcData->fpgaPwmClock;
                    
                    //系统错误码置位
                    errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_3;
                }
                else if (calcOutput < OUTPUT_PERIOD_MIN)
                {
                    calcOutput = OUTPUT_PERIOD_MIN;
                    
                    //系统错误码置位
                    errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_3;
                }
                
                //统计下误差和时间
                pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

                pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                                      (u32)calcOutput, 
                                                      BUFFOPERT_WRITE, 
                                                      pPvtCalcData->pContext);
            }
        }
        else
        {
            curTime = 0;
        }
        /*************************************解算减速(III)段**********************************************************/
    }
    else
    {
        decTime = pPvtCalcData->motionTime;
    }

    /****************************************最后一步**************************************************************/
    //最后一步的时间直接用motionTime减去上一步的时间位置
    //pPvtCalcData->lastStepSpeed = pPvtCalcData->motionTime - curTime);
    pPvtCalcData->lastStepSpeed = decTime - curTime;    
    if (pPvtCalcData->lastStepSpeed <= 0.0f)
    {
        errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_3;
        pPvtCalcData->lastStepSpeed = 0;
    }
    else
    {
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
                                                  pPvtCalcData->pContext);
                                         
            //统计下时间
            pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
            
            calcOutput = pPvtCalcData->fpgaPwmClock;
            
            //系统错误码置位
            errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_3;
        }
        else if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //系统错误码置位
            errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_3;
        }
        
        //统计下误差和时间
        pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
        pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

        pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              pPvtCalcData->pContext);
        /****************************************最后一步**************************************************************/

        //仅做统计用
        pPvtCalcData->targetStep++;
    }
    
    return errorCode;
}




/*******************************************文件尾********************************************/
