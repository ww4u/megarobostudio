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
#include <os.h>
#include "project.h"
#include "servSoftTimer.h"
#include "servCommIntfc.h"
#include "cmdSensor.h"
#include "servLed.h"
#include "eventManageTask.h"
#include "cmdDriver.h"
#include "cmdTrigger.h"
#include "cmdSystem.h"
#include "paraSaveTask.h"

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
extern SoftTimerStruct g_driverCurrTimer[CH_TOTAL];

extern SoftTimerStruct g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];

#ifdef PROJECT_QUBELEY
extern SoftTimerStruct g_analogInTimer;
#endif

#ifdef PROJECT_GELGOOG

#if !GELGOOG_SINANJU
extern SoftTimerStruct g_isolatorInTimer;

#else

extern SoftTimerStruct g_distAlarmLedTimer;
#endif

#if GELGOOG_AXIS_10
extern SoftTimerStruct g_driverFaultTimer;
#else
extern SoftTimerStruct g_drvStateReadTimer[CH_TOTAL];
#endif

#endif

extern SoftTimerStruct g_ledTimer;

extern SoftTimerStruct g_paraSaveTimer;

extern SoftTimerStruct g_runTimeCountTimer[CH_TOTAL];



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
    
    servStimerInit(&g_senUartDmaRecTimer[UARTNUM_U1], 20,   servSensor1UartDmaRecTimerCB); 
    servStimerInit(&g_senUartSwitchTimer[UARTNUM_U1], 1000, servSensor1UartSwitchTimerCB); 

#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4���10��ֻ֧��1·
    servStimerInit(&g_senUartDmaRecTimer[UARTNUM_U2], 10,   servSensor2UartDmaRecTimerCB); 
    servStimerInit(&g_senUartSwitchTimer[UARTNUM_U2], 1000, servSensor2UartSwitchTimerCB); 
#endif
    
    //�˶�״̬�ϱ���ʱ��
    for (i = 0;i < CH_TOTAL;i++)
    {
        servStimerInit(&g_motionStateTimer[i], 1000, eventMotionStateTimerCB);

        //�˶�״̬��ⶨʱ��
        servStimerInit(&g_motionMonitorTimer[i], 200, eventMotionMonitorTimerCB);

#if !GELGOOG_AXIS_10
#if TUNING_SUPPORT
        //����״̬��ȡ��ʱ��
        servStimerInit(&g_drvStateReadTimer[i], 200, eventDriverStateReadTimerCB);
#endif
#endif

        //5���ϱ�����ʱ����ʼ��(ѭ��������Ҫ��ʱ�ϱ�����ֵ�ı����ϱ�)
        servStimerInit(&g_reportTimer[i][REPTTYPE_TORQUE], 1000, cmdReportTorqueTimerCB);
        
        servStimerInit(&g_reportTimer[i][REPTTYPE_OUTNUM],   1000, cmdReportOutNumTimerCB);
        servStimerInit(&g_reportTimer[i][REPTTYPE_STEPS],    1000, cmdReportStepsTimerCB);
        servStimerInit(&g_reportTimer[i][REPTTYPE_VELOCITY], 1000, cmdReportVelorityTimerCB);

        //�ڲ���������
        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVMONT],  1000, cmdReportDriveMonitorTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_DISTANCE], 1000, cmdReportDistanceTimerCB);
        servStimerInit(&g_reportTimer[i][REPTTYPE_ABSENC],   1000, cmdReportAbsEncoderTimerCB);

        //�����澯�ϱ���ʱ��
        servStimerInit(&g_lineOOStepTimer[i], 200, eventLineOOStepTimerCB);

        //����/���е����л�
        servStimerInit(&g_driverCurrTimer[i], 100, eventDriverCurrTimerCB);

        servStimerInit(&g_runTimeCountTimer[i], 10, eventRunTimeCountTimerCB);
    }

#ifdef PROJECT_QUBELEY
    servStimerInit(&g_analogInTimer, 100, servAdc1IntTimerCB);
#endif

#ifdef PROJECT_GELGOOG

#if !GELGOOG_SINANJU
    servStimerInit(&g_isolatorInTimer, 100, eventIsolInMonitorTimerCB);
#else

    servStimerInit(&g_distAlarmLedTimer, LED_FLICKER_FREQ_MIN, eventDistAlarmLedTimerCB);
#endif

#if GELGOOG_AXIS_10
    servStimerInit(&g_driverFaultTimer, 100, eventDriverFaultTimerCB);
#endif

#endif

    servStimerInit(&g_ledTimer, 500, servLedTimerCB);

    servStimerInit(&g_paraSaveTimer, 2000, paraSaveTimerCB);
}


/*********************************************************************************************
�� �� ��: servStimerManage;
ʵ�ֹ���: �������е���ʱ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servStimerManage(SoftTimerStateStruct softTimer)
{
    /*u8 i;

    
    for (i = 0;i < CH_TOTAL;i++)
    {
        if (SENSOR_ON == softTimer.motionState[i])
        {
            servStimerAdd(&g_motionStateTimer[i]);
        }
    } */         

    //LED��˸��ʱ��
    servStimerAdd(&g_ledTimer);
}


/*********************************************************************************************
�� �� ��: servStimerManage;
ʵ�ֹ���: �������е���ʱ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servSoftDelayMs(u32 delayMs)
{
    OSTimeDlyHMSM(0, 0, 0, delayMs, OS_OPT_TIME_HMSM_STRICT, NULL);
}



/*******************************************�ļ�β********************************************/
