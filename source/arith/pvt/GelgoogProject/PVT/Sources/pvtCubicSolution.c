/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtCubicSolution.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.11.23;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <stdlib.h>
#include <string.h>
//#include <math.h>
#include "arm_math.h"
#include "pvtCubicSolution.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvtCubicCurveCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvtCubicCurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;
    u16 calcCount;

    f32 calcOutput;
    u32 i;

    //ʱ��ʹ��64λ����Ϊ�õ��ǲ��ҷ�(��ֵ��)�������Ȳ���ʱ���ܵ��²��Ҳ����ڷ�Χ�ڵ�Ŀ��ֵ
    //���ʹ�ý�����ķ�ʽ����Ҫ
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

    //�е���һ����ʱ�䣬����һ��ʱֱ�ӽ������һ������
    if (pPvtCalcData->motionStep > 1)    
    {
        //��ʼ��ʱ��
        minTime  = 0;
        lastTime = 0;
        if ((1 == pPvtCalcData->targetStep) ||     //��һ����ʱ����Ҫ��motionTime��Χ�ڲ���
            (0.0f == pPvtCalcData->startSpeed))    //���߳�ʼ�ٶ�Ϊ0����Ϊ����֮���ٶȱ仯�ϴ���������motionTime��Χ�ڲ���
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
        
        //��ǰ���������Ͷ������ϵ��
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
        
        for (i = 0;i < (pPvtCalcData->motionStep - 1);i++)    //���һ�������㣬ֱ����motionTime - curTime
        { 
            calcCount = 0;       
            curTime += pPvtCalcData->lastStepSpeed;    //����һ����ʱ�����ϼ�����һ�����ٶ���Ϊ��һ���ļ����ʼʱ��
            if (curTime > pPvtCalcData->motionTime)
            {
                curTime = pPvtCalcData->motionTime;
            }

            //P(t) = threeTermCoef * t^3 + twoTermCoef * t^2 + v0 * t
            curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + pPvtCalcData->startSpeed) * curTime;
            
            //��Ŀ��ֵ�����̫������Ҫ��������
            posnOffset = curPost - pPvtCalcData->targetStep;
            while ((posnOffset > pPvtCalcData->maxOffset) ||    //��Ŀ��ֵ�����̫���������
                   (posnOffset < pPvtCalcData->minOffset))
            {
                calcCount++;
                
                if (calcCount >= PVT_ONE_POINT_CALC_TIMES_MAX)
                {
                    //�������������������㣬����
                    errorCode = PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_1;
                    break;
                }
                else if (PVT_ONE_POINT_CALC_TIMES_MAX / 2 == calcCount)  //����һ�����ƺ��ȼӴ�maxTime
                {
                    maxTime = pPvtCalcData->motionTime;
                }

                //���ֲ��ң��ı�ʱ���������
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

            //������������������������㣬����
            if ((PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_1) == errorCode)
            {
                break;
            }
            
            //��¼��һ������һ���ļ��ʱ�䣬Ҳ�����ٶ�
            pPvtCalcData->lastStepSpeed = curTime - lastTime;    
            if (pPvtCalcData->lastStepSpeed <= 0.0f)
            {
                errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
                pPvtCalcData->lastStepSpeed = 0;
            }
            
            //����ʱ�䣬׼����һ���ļ���
            pPvtCalcData->targetStep++;
            minTime = curTime;
            lastTime = curTime;
            maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
            if (maxTime > pPvtCalcData->motionTime)
            {
                maxTime = pPvtCalcData->motionTime;
            }
            
            //��ʱ�䰴���߼䲽ƽ��
            calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                          pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                          pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

            //���������ޱ���
            if (calcOutput > pPvtCalcData->fpgaPwmClock)
            {
                calcOutput -= pPvtCalcData->fpgaPwmClock;
                
                pPvtCalcData->errorTime = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                
                //������Ĳ�����Ϊ�ȴ�ʱ��
                pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                      (u32)calcOutput, 
                                                      BUFFOPERT_WRITE, 
                                                      (void *)pPvtCalcData, 
                                                      pPvtCalcData->pContext);
                                             
                //ͳ����ʱ��
                pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
                
                calcOutput = pPvtCalcData->fpgaPwmClock;
                
                //ϵͳ��������λ
                errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_1;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //ϵͳ��������λ
                errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_1;
            }
            
            //ͳ��������ʱ��
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
    
    /*****************************���һ��*******************************/
    //���һ����ʱ��ֱ����motionTime��ȥ��һ����ʱ��λ��
    pPvtCalcData->lastStepSpeed = pPvtCalcData->motionTime - curTime;  
    
    if (pPvtCalcData->lastStepSpeed <= 0.0f)
    {
        errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
        pPvtCalcData->lastStepSpeed = 0;
    }
    else
    {
        pPvtCalcData->targetStep++;    //CJ 2018.12.10 Add
        
        //��ʱ�䰴���߼䲽ƽ��
        calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                      pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                      pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

        //�������ޱ���
        if (calcOutput > pPvtCalcData->fpgaPwmClock)
        {
            calcOutput -= pPvtCalcData->fpgaPwmClock;
            
            pPvtCalcData->errorTime = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
            
            //������Ĳ�����Ϊ�ȴ�ʱ��
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                  (u32)calcOutput, 
                                                  BUFFOPERT_WRITE, 
                                                  (void *)pPvtCalcData, 
                                                  pPvtCalcData->pContext);
                                         
            //ͳ����ʱ��
            pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
            
            calcOutput = pPvtCalcData->fpgaPwmClock;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_1;
        }
        else if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_1;
        }
        
        //ͳ��������ʱ��
        pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
        pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
        
        pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              (void *)pPvtCalcData, 
                                              pPvtCalcData->pContext);
        /*****************************���һ��*******************************/
    }
    
    //��Ϊ֮ǰP(t) - p0 = pPvtCalcData->targetStep - p0��������Ҫ�ָ�Ŀ��΢��
    pPvtCalcData->targetStep += pPvtCalcData->startPosn;

    return errorCode;
}



/*******************************************�ļ�β********************************************/
