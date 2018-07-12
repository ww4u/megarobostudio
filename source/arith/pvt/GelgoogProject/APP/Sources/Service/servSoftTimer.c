/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servSoftTimer.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.11.11;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "project.h"
#include "servSoftTimer.h"
#include "servCommIntfc.h"
#include "cmdSensor.h"
#include "servLed.h"
#include "eventManageTask.h"
#include "cmdDriver.h"
#include "cmdTrigger.h"

#ifdef PROJECT_QUBELEY
#include "servPT100.h"
#endif



/****************************************�ⲿ��������*****************************************/
extern SoftTimerStruct g_ciUartDmaRecTimer;
extern SoftTimerStruct g_senUartDmaRecTimer[UARTNUM_RESERVE];
extern SoftTimerStruct g_senUartSwitchTimer[UARTNUM_RESERVE];
extern SoftTimerStruct g_motionStateTimer[CH_TOTAL];
extern SoftTimerStruct g_motionMonitorTimer[CH_TOTAL];
extern SoftTimerStruct g_lineOOStepTimer[CH_TOTAL];

extern SoftTimerStruct g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];

#ifdef PROJECT_QUBELEY
extern SoftTimerStruct g_analogInTimer;
#endif

#ifdef PROJECT_GELGOOG
extern SoftTimerStruct g_isolatorInTimer;
#endif

extern SoftTimerStruct g_ledTimer;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: servStimerInit;
ʵ�ֹ���: �����ʱ����ʼ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servStimerInit(SoftTimerStruct *pSoftTimer, u16 interval, TimeOutFunc pTimerOutFunc)
{
    pSoftTimer->period = interval / PERIOD_TASK_PERIOD;
    if (0 == pSoftTimer->period)    //��С1������
    {
        pSoftTimer->period = 1;
    }
    pSoftTimer->timeout = 0;
    pSoftTimer->pTimerOutFunc = pTimerOutFunc;
    pSoftTimer->bUsed = false;
}


/*********************************************************************************************
�� �� ��: servStimerExamine;
ʵ�ֹ���: ��鶨ʱ���Ƿ�ʱ; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servStimerExamine(SoftTimerStruct * ptSoftTimer, void *timeOutPara)
{
    if (ptSoftTimer->bUsed)
    {
        ptSoftTimer->timeout++;

        if (ptSoftTimer->timeout >= ptSoftTimer->period)
        {  
            if (ptSoftTimer->pTimerOutFunc != NULL)
            {
                ptSoftTimer->pTimerOutFunc(timeOutPara);   
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: servStimerAdd;
ʵ�ֹ���: ������ʱ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servStimerAdd(SoftTimerStruct *ptSoftTimer)
{
    ptSoftTimer->bUsed = true;
    ptSoftTimer->timeout = 0;
}


/*********************************************************************************************
�� �� ��: servStimerDelete;
ʵ�ֹ���: �رն�ʱ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servStimerDelete(SoftTimerStruct *ptSoftTimer)
{
    ptSoftTimer->bUsed = false;
    ptSoftTimer->timeout = 0;
}


/*********************************************************************************************
�� �� ��: servStimerAllInit;
ʵ�ֹ���: ��ʼ�����е���ʱ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servStimerAllInit(void)
{
    u8 i;

    
    //����DMA���ն�ʱ������ʱʱ��60ms
    servStimerInit(&g_ciUartDmaRecTimer, 60, servCiUartDmaRecTimerCB);

    servStimerInit(&g_senUartDmaRecTimer[UARTNUM_U1], 110,  servSensor1UartDmaRecTimerCB); 
    servStimerInit(&g_senUartSwitchTimer[UARTNUM_U1], 1000, servSensor1UartSwitchTimerCB); 

    servStimerInit(&g_senUartDmaRecTimer[UARTNUM_U2], 110,  servSensor2UartDmaRecTimerCB); 
    servStimerInit(&g_senUartSwitchTimer[UARTNUM_U2], 1000, servSensor2UartSwitchTimerCB); 
    
    //�˶�״̬�ϱ���ʱ��
    for (i = 0;i < CH_TOTAL;i++)
    {
        servStimerInit(&g_motionStateTimer[i], 1000, EventMotionStateTimerCB);

        //�˶�״̬��ⶨʱ��
        servStimerInit(&g_motionMonitorTimer[i], 200, EventMotionMonitorTimerCB);

        //5���ϱ�����ʱ����ʼ��(ѭ��������Ҫ��ʱ�ϱ�����ֵ�ı����ϱ�)
        servStimerInit(&g_reportTimer[i][REPTTYPE_TORQUE], 1000, cmdReportTorqueTimerCB);
        
        servStimerInit(&g_reportTimer[i][REPTTYPE_XANGLE], 1000, cmdReportXAngleTimerCB);
        servStimerInit(&g_reportTimer[i][REPTTYPE_YANGLE], 1000, cmdReportYAngleTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_ZANGLE], 1000, cmdReportZAngleTimerCB); 
        
        servStimerInit(&g_reportTimer[i][REPTTYPE_XVELOC], 1000, cmdReportXVelocityTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_YVELOC], 1000, cmdReportYVelocityTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_ZVELOC], 1000, cmdReportZVelocityTimerCB); 
        
        servStimerInit(&g_reportTimer[i][REPTTYPE_XACC], 1000, cmdReportXAccTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_YACC], 1000, cmdReportYAccTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_ZACC], 1000, cmdReportZAccTimerCB); 
        
        servStimerInit(&g_reportTimer[i][REPTTYPE_OUTNUM],   1000, cmdReportOutNumTimerCB);
        servStimerInit(&g_reportTimer[i][REPTTYPE_STEPS],    1000, cmdReportStepsTimerCB);
        servStimerInit(&g_reportTimer[i][REPTTYPE_VELOCITY], 1000, cmdReportVelorityTimerCB);

        //�ڲ���������
        servStimerInit(&g_reportTimer[i][REPTTYPE_ANGLESEN], 100,  cmdReportAngleSensorTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVMONT],  1000, cmdReportDriveMonitorTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_DISTANCE], 1000, cmdReportDistanceTimerCB);
        servStimerInit(&g_reportTimer[i][REPTTYPE_ABSENC],   1000, cmdReportAbsEncoderTimerCB);

        //�����澯�ϱ���ʱ��
        servStimerInit(&g_lineOOStepTimer[i], 200, EventLineOOStepTimerCB);
    }

#ifdef PROJECT_QUBELEY
    servStimerInit(&g_analogInTimer, 100, servAdc1IntTimerCB);
#endif

#ifdef PROJECT_GELGOOG
    servStimerInit(&g_isolatorInTimer, 100, EventIsolInMonitorTimerCB);
#endif


    servStimerInit(&g_ledTimer, 500, servLedTimerCB);
}



/*******************************************�ļ�β********************************************/
