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
#include <math.h>
#include "arm_math.h"
#include "pvtCubicSolution.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    PVT_CALC_MODIFY_ERROR    1



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
    u16 calcCount = 0;

    f32 calcOutput;
    u32 i;
    f64 curTime = 0;    //��ʱ�ĳ�64λ    NICK MARK
    u32 poseOffset;
    f64 lastTime;       //��ʱ�ĳ�64λ    NICK MARK
    f64 maxTime;        //��ʱ�ĳ�64λ    NICK MARK
    f64 minTime;        //��ʱ�ĳ�64λ    NICK MARK

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
    

    //ת������
    startSpeed = fabs(pPvtCalcData->startSpeed * pPvtCalcData->posnConvertInfo.posnToStep);
    endSpeed   = fabs(pPvtCalcData->endSpeed   * pPvtCalcData->posnConvertInfo.posnToStep);
    poseOffset = abs(endPosition - startPosition);

    //ͳ����΢����������ӣ������
    if (DIR_FORWARD == pPvtCalcData->lastStepDir)
    {
        pPvtCalcData->pvtSteps += poseOffset;
    }
    else
    {
        pPvtCalcData->pvtSteps -= poseOffset;
    }

    //��ʼ��ʱ��
    minTime = 0;
    if ((1 == pPvtCalcData->targetStep) ||   //��һ����ʱ����Ҫ��motionTime��Χ�ڲ���
        (0.0f == startSpeed))                //���߳�ʼ�ٶ�Ϊ0����Ϊ����֮���ٶȱ仯�ϴ���������motionTime��Χ�ڲ���
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
    
    //��ǰ���������Ͷ������ϵ��
    cubeMotionTime   = CUBE(pPvtCalcData->motionTime);
    squareMotionTime = SQUARE(pPvtCalcData->motionTime);
    
    threeTermCoef = ((startSpeed + endSpeed) / squareMotionTime) - (poseOffset * 2.0f / cubeMotionTime);
    twoTermCoef   = (poseOffset * 3.0f / squareMotionTime) - (startSpeed * 2.0f + endSpeed) / pPvtCalcData->motionTime;

    pPvtCalcData->targetStep -= startPosition;

#if PVT_CALC_MODIFY_ERROR
    if (poseOffset > 1)    //С�ڵ���һ��ʱֱ�ӽ������һ������
    {
    
        for (i = 0;i < (poseOffset - 1);i++)    //���һ��������
#else
    for (i = 0;i < poseOffset;i++)
#endif
        { 
            calcCount = 0;       
            curTime += pPvtCalcData->lastStepSpeed;    //����һ����ʱ�����ϼ�����һ�����ٶ���Ϊ��һ���ļ����ʼʱ��
            if (curTime > pPvtCalcData->motionTime)
            {
                curTime = pPvtCalcData->motionTime;
            }

            curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + startSpeed) * curTime;
            
            posnOffset = curPost - pPvtCalcData->targetStep;
            while ((posnOffset > TARGET_REAL_OFFSET_P) ||    //��Ŀ��ֵ�����̫���������
                   (posnOffset < TARGET_REAL_OFFSET_N))
            {
                calcCount++;
                if (calcCount > PVT_ONE_POINT_CALC_TIMES_MAX)
                {
                    //�������������������㣬����
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

            //������������������������㣬����
            if (PVT_CALC_TOO_MANY_CALC_TIMES == errorCode)
            {
                break;
            }
            
            pPvtCalcData->targetStep++;    //��ʼ������һ��Ŀ��
            
            pPvtCalcData->lastStepSpeed = curTime - lastTime;    //��¼��һ������һ���ļ��ʱ�䣬Ҳ�����ٶ�

            minTime = curTime;
            lastTime = curTime;
            maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
            if (maxTime > pPvtCalcData->motionTime)
            {
                maxTime = pPvtCalcData->motionTime;
            }

            //ʱ��Ĳ�������:�����Ͻ���ʱ����Ϊ�ٶ���û�����ģ��������Դ�ڹ�һ�������������
            calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                         pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

            //���������ޱ���
            if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
            {
                calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                
                //ϵͳ��������λ
                errorCode = PVT_CALC_GREAT_UPER_LIMIT;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //ϵͳ��������λ
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
    
    //���һ��
    pPvtCalcData->lastStepSpeed = pPvtCalcData->motionTime - curTime;    //���һ����ʱ��ֱ����motionTime��ȥ��һ����ʱ��λ��
    
    //ʱ��Ĳ�������:�����Ͻ���ʱ����Ϊ�ٶ���û�����ģ��������Դ�ڹ�һ�������������
    calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                 pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

    //�������ޱ���
    if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
    {
        calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
        
        //ϵͳ��������λ
        errorCode = PVT_CALC_GREAT_UPER_LIMIT;
    }
    else if (calcOutput < OUTPUT_PERIOD_MIN)
    {
        calcOutput = OUTPUT_PERIOD_MIN;
        
        //ϵͳ��������λ
        errorCode = PVT_CALC_LESS_LOWER_LIMIT;
    }

    calcOutput += pPvtCalcData->errorTime;
    pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

    pPvtCalcData->outpBufferFill(chanNum, (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                          (u32)calcOutput, 
                                          BUFFOPERT_WRITE, 
                                          pPvtCalcData->pContext);
    //���һ��
    
    pPvtCalcData->targetStep++;    //��ʼ������һ��Ŀ��
    
#endif

    pPvtCalcData->targetStep += startPosition;

#if PVT_CALC_MODIFY_ERROR
    pPvtCalcData->lastStepTime = pPvtCalcData->motionTime + pPvtCalcData->startTime;
#else
    pPvtCalcData->lastStepTime = curTime + pPvtCalcData->startTime;
#endif

    return errorCode;
}



/*******************************************�ļ�β********************************************/
