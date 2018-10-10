/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtTrapezSolution.c；
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
#include "pvtTrapezSolution.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvtTrapezCurveCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvtTrapezCurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;

    f32 calcOutput;
    u32 i;
    
    f32 curTime = 0;
    f32 lastTime;
    
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

    //一元二次方程相关变量
    f32 delta;
    f32 sqrtDelta;
    f32 squarecoeff2B;
    f32 coeff4A;
    f32 coeff2AInv;
    //一元二次方程相关变量

    f32 squareSpeed;
    

    //有等于一步的情况，等于一步时直接进入最后一步计算
    if (pPvtCalcData->motionStep > 1)    
    {         
        //梯形曲线即一次的S曲线，分为三段，I、II和III段，简单点称为加速、匀速和减速段
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
        //加减速段P和T的关系是一个一元二次方程，P是y，T是x，最终是根据P获取T
        //一元二次方程求根公式里面的三个系数分别是: a -- (uniformSpeed - startSpeed)/(2 * accTime)
        //一元二次方程求根公式里面的三个系数分别是: b -- startSpeed
        //一元二次方程求根公式里面的三个系数分别是: c -- pPvtCalcData->targetStep
        if (accPosn > 0)
        {
            squarecoeff2B = pPvtCalcData->startSpeed * pPvtCalcData->startSpeed;
            coeff2AInv = accTime / (uniformSpeed - pPvtCalcData->startSpeed);
            coeff4A = 2.0f / coeff2AInv;
            
            //初始化时间
            lastTime = 0;
            realStep = realAccPosn / accPosn;
            
            //这里少算了一步，I段最后一步直接用差值的方式计算
            for (i = 1;i < accPosn;i++)
            {
                //因为b = v0, a = (vu - v0) / (2 * Tacc), c = -p(t) = -i
                //△= b^2 - 4ac
                //所以变成了delta = squarecoeff2B + coeff4A * i
                delta = squarecoeff2B + coeff4A * i * realStep;
                arm_sqrt_f32(delta, &sqrtDelta);
                curTime = (sqrtDelta - pPvtCalcData->startSpeed) * coeff2AInv;

                //仅做统计用
                pPvtCalcData->targetStep++;

                pPvtCalcData->lastStepSpeed = curTime - lastTime;    
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                lastTime = curTime;
                
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

            pPvtCalcData->lastStepSpeed = accTime - curTime;    
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
        //加减速段P和T的关系是一个一元二次方程，P是y，T是x，最终是根据P获取T
        //一元二次方程求根公式里面的三个系数分别是: a -- (endSpeed - uniformSpeed)/(2 * decTime)
        //一元二次方程求根公式里面的三个系数分别是: b -- uniformSpeed
        //一元二次方程求根公式里面的三个系数分别是: c -- pPvtCalcData->targetStep
        decStep = pPvtCalcData->motionStep - decPosn;
        if (decStep > 1)
        {
            squarecoeff2B = uniformSpeed * uniformSpeed;
            coeff2AInv = decTime / (pPvtCalcData->endSpeed - uniformSpeed);
            coeff4A = 2.0f / coeff2AInv;
            
            lastTime = 0.0;
            realStep = realDecPosn / decStep;
            
            //少算一步，最后一步不在这里算
            for (i = 1;i < decStep;i++)    
            {
                //因为b = v0, a = (vu - v0) / (2 * Tacc), c = -p(t) = -i
                //△= b^2 - 4ac
                //所以变成了delta = squarecoeff2B + coeff4A * i
                delta = squarecoeff2B + coeff4A * i * realStep;
                arm_sqrt_f32(delta, &sqrtDelta);
                curTime = (sqrtDelta - uniformSpeed) * coeff2AInv;

                //仅做统计用
                pPvtCalcData->targetStep++;

                pPvtCalcData->lastStepSpeed = curTime - lastTime;    
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_3;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                lastTime = curTime;
                
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
    //III段最后一步
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
