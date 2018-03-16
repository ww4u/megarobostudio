/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtLinearSolution.c��
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
#include "pvtLinearSolution.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvtLinearCurveCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvtLinearCurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;
    f32 calcOutput;
    u32 i;
    f64 curTime = 0;    //��ʱ�ĳ�64λ    NICK MARK
    u32 poseOffset;
    f64 lastTime;       //��ʱ�ĳ�64λ    NICK MARK

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

    //��ǰ������ϵ���Լ��ж�����
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
        //����λ��ֱ�����ʱ��
        deltaPost = squareStartSpeed - 4.0f * accelCoef * ((f32)startPosition - pPvtCalcData->targetStep);
        if (deltaPost < 0.0f)
        {
            deltaPost = 0;
        }
        arm_sqrt_f32(deltaPost, &sqrtDeltaPost);
        curTime = (sqrtDeltaPost - startSpeed) * 0.5f / accelCoef;
        
        pPvtCalcData->targetStep++;    //��ʼ������һ��Ŀ��
        
        pPvtCalcData->lastStepSpeed = curTime - lastTime;    //��¼��һ������һ���ļ��ʱ�䣬Ҳ�����ٶ�
        lastTime = curTime;

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

    squareDecelOffset = SQUARE(decelOffset);
    for (i = 0;i < decelCount;i++)
    {
        //����λ��ֱ�����ʱ��
        deltaPost = squareDecelOffset - 4.0f * decelCoef * (decelPost - pPvtCalcData->targetStep);
        if (deltaPost < 0.0f)
        {
            deltaPost = 0;
        }
        arm_sqrt_f32(deltaPost, &sqrtDeltaPost);
        curTime = (sqrtDeltaPost - decelOffset) * 0.5f / decelCoef;
        
        pPvtCalcData->targetStep++;    //��ʼ������һ��Ŀ��
        
        pPvtCalcData->lastStepSpeed = curTime - lastTime;    //��¼��һ������һ���ļ��ʱ�䣬Ҳ�����ٶ�
        lastTime = curTime;

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

    pPvtCalcData->lastStepTime = curTime + pPvtCalcData->startTime;   

    return errorCode;
}



/*******************************************�ļ�β********************************************/
