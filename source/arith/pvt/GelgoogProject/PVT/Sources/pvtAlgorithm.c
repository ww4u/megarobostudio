/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtalgorithm.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.08.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arm_math.h"
#include "pvtAlgorithm.h"
#include "pvtCubicSolution.h"
#include "pvtTrapezSolution.h"
#include "pvtScurveSolution.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    PVT_FIFO_REPORT_CALC_NUM    5    //FIFOģʽ�¿�ʼ�ϱ��������״̬���Ѽ������



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvtPointCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvtPointCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;


    //���ݹ滮��ʽ����Ԥ����
    switch (pPvtCalcData->pvtPlanMode)
    {
        /***************************************************************
        ���ζ���ʽ��ֵ
        ***************************************************************/
        case PLANMODE_CUBIC:
            errorCode = pvtCubicCurveCalc(chanNum, pPvtCalcData);
          break;

        /***************************************************************
        ���β�ֵ
        ***************************************************************/
        case PLANMODE_TRAPEZOID:
            errorCode = pvtTrapezCurveCalc(chanNum, pPvtCalcData);
          break;          
          
        /***************************************************************
        S���߲�ֵ
        ***************************************************************/
        case PLANMODE_SCURVE:
            errorCode = pvtScurveCalc(chanNum, pPvtCalcData);
          break;

        default:
          break;
    }

    return errorCode;
}


/*********************************************************************************************
�� �� ��: pvtSegmentCalc
ʵ�ֹ���: ����һ��PVT(����PVT��֮�������΢����)���˺���������Ҫ�ǶԼ������Ԥ����֮����
          pvtPointCalc�������PVT��֮������΢�����ļ���
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
u8 pvtSegmentCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum)
{
    u8   errorCode = PVT_CALC_NO_ERROR;
    OutpDataTypeEnum direction;    
    f32 motionStep;
    f32 posnConvertCoeff;
    
    f32 startPosn;
    f32 startSpeed;
    f32 endPosn;
    f32 endSpeed;
    f32 startTime;
    f32 motionTime;
    

    //�������һ����ż���
    if (pPvtCalcData->endPoint.time != -1.0f)
    {
        //��ȡ��ǰҪ�������һ��PVT���������
        startSpeed = pPvtCalcData->startPoint.speed;
        startTime  = pPvtCalcData->startPoint.time;
        
        endSpeed = pPvtCalcData->endPoint.speed;
        

        //�жϷ���
        if (pPvtCalcData->endPoint.position < pPvtCalcData->startPoint.position)
        {
            direction = OUTPDATA_REVERSE;
        }
        else
        {
            direction = OUTPDATA_FORWARD;
        }
        
        //������
        if (direction != pPvtCalcData->lastStepDir)
        {
            pPvtCalcData->lastStepDir = direction;
            
            //����֮��Ŀ�경����Ϊ1�����·���
            pPvtCalcData->targetStep = 1;
            pPvtCalcData->waitStepError = 0;

            //����֮���¼�·����λ��
            pPvtCalcData->invsePosition = pPvtCalcData->startPoint.position;
        }

        //���ڷ���λ�ü��������ʼλ��
        startPosn = fabs(pPvtCalcData->startPoint.position - pPvtCalcData->invsePosition);
        endPosn   = fabs(pPvtCalcData->endPoint.position - pPvtCalcData->invsePosition);
        
        motionTime = pPvtCalcData->endPoint.time - startTime;

        posnConvertCoeff = pPvtCalcData->posnToStep;

        motionStep = (endPosn - startPosn) * posnConvertCoeff + pPvtCalcData->waitStepError;   //�ۼ��²���һ���Ĳ���           

        //λ�ñ仯С��1����Ϊ�ȴ�ʱ��
        if (motionStep < 1.0f)
        {
            //�����һ�ο�ʼ�ȴ���λ��
            if (0.0f == pPvtCalcData->waitStepError)
            {
                pPvtCalcData->waitPosition = startPosn;
            }
            pPvtCalcData->waitStepError += motionStep;

            //ʱ���һ��
            motionTime = motionTime * pPvtCalcData->fpgaPwmClock + motionTime * pPvtCalcData->fpgaClockOffset;

            //��Ϊ�ȴ�ʱ����벨��Buffer
            motionTime += pPvtCalcData->errorTime;
            pPvtCalcData->errorTime = motionTime - (u32)motionTime;
            
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, (u32)motionTime, BUFFOPERT_WRITE, pPvtCalcData->pContext);

            //ͳ��ʱ��
            pPvtCalcData->timeCount += (u32)motionTime;
        }
        else
        {
            //��ʼλ�ôӵȴ�λ������
            if (pPvtCalcData->waitStepError > 0.0f)
            {
                pPvtCalcData->waitStepError = 0;
                startPosn = pPvtCalcData->waitPosition;
            }
            
            //����һ��PVT
            pPvtCalcData->startPosn  = (u32)(startPosn * posnConvertCoeff + 0.5f);
            pPvtCalcData->startSpeed = fabs(startSpeed * posnConvertCoeff);
            pPvtCalcData->startTime  = startTime;
            pPvtCalcData->endPosn    = (u32)(endPosn * posnConvertCoeff + 0.5f);
            pPvtCalcData->endSpeed   = fabs(endSpeed * posnConvertCoeff);
            pPvtCalcData->motionTime = motionTime;

            //��ʱ���ò�ͬ�ļ��㷽ʽ    NICK MARK
            if (MTNMODE_PVT == pPvtCalcData->motionMode)
            {
                pPvtCalcData->motionStep = (u32)((endPosn - pPvtCalcData->startPosn / posnConvertCoeff) * posnConvertCoeff + 0.5f);
            }
            else
            {
                pPvtCalcData->motionStep = (u32)((endPosn - startPosn) * posnConvertCoeff + 0.5f);
            }

            errorCode = pvtPointCalc(chanNum, pPvtCalcData);
        }

        //FIFOģʽ����ÿһ�ζ�ǿ�Ʒ��������ݣ��������Შ��ͬ��    CJ 2018.04.21
        if (EXECMODE_FIFO == pPvtCalcData->pvtExecMode)
        {
            //ǿ�Ʒ���
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_FORWARD, 0, BUFFOPERT_SYNCSEND, pPvtCalcData->pContext);
        }
    }
    
    //���һ������ǿ�Ʒ������������ݣ������FIFOģʽ�㹻�˵�����ֻ֪ͨFUNC�ϱ��������״̬
    if ((-1.0f == pPvtCalcData->endPoint.time) ||
        ((EXECMODE_FIFO == pPvtCalcData->pvtExecMode) &&        //FIFOģʽ���㹻�˵����һ�δ�ϱ�    CJ 2017.03.28 Add
         (false == pPvtCalcData->bReportCalcEnd) &&
         (pPvtCalcData->lastPoint >= PVT_FIFO_REPORT_CALC_NUM)))
    {
        //֪ͨFUNC��ѯFPGA��Ready״̬
        if ((EXECMODE_FIFO != pPvtCalcData->pvtExecMode) ||    //FIFOģʽ���Ѿ��ϱ�������Ҫ�ϱ�    CJ 2017.03.28 Modify
            (false == pPvtCalcData->bReportCalcEnd))   
        {
            pPvtCalcData->bQueryReady = true;
            pPvtCalcData->bReportCalcEnd = true;
        }

        //���һ�������΢��ģʽ����Ҫǿ�Ʒ���Buffer�е����ݺ���չ�������
        if (-1.0f == pPvtCalcData->endPoint.time)    //CJ 2017.03.28 Modify
        {
            //ǿ�Ʒ���Buffer�е����ݣ��ܵĲ������ݵ���m_stProcessData.targetStep - 1
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_FORWARD, 0, BUFFOPERT_FORCESEND, pPvtCalcData->pContext);
        }
    }

    return errorCode;
}



/*******************************************�ļ�β********************************************/
