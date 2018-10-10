/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  paraSaveTask.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2018.06.10;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <os.h>
#include "paraSaveTask.h"
#include "servSystemPara.h"
#include "servSoftTimer.h"



/****************************************�ⲿ��������*****************************************/
extern OS_SEM g_semParaSaveTask;

extern bool g_bPerdPostSemToPara;

extern CommIntfcStruct   g_commIntfc;
extern SystemInfoStruct  g_systemInfo;

extern DeviceInfoStruct  g_deviceInfo;
extern PlanInfoStruct    g_planInfo;

extern MotorInfoStruct   g_motorInfo; 
extern MotionInfoStruct  g_motionInfo;
extern ReportInfoStruct  g_reportInfo;
extern TrigInInfoStruct  g_trigInInfo;
extern DigitalOutStruct  g_digitalOut;
extern IsolatorOutStruct g_isolatorOut;
extern SensorUartStruct  g_sensorUart;
extern DriverInfoStruct  g_driverInfo;

#ifdef PROJECT_QUBELEY
extern AnalogInfoStruct g_analogInfo;
#endif

#ifdef PROJECT_GELGOOG
#if GELGOOG_SINANJU
extern SensorAlarmStruct g_sensorAlarm;

#else

extern IsolatorInStruct  g_isolatorIn;
#endif
#endif



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SoftTimerStruct g_paraSaveTimer;



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: paraSaveTimerCB
ʵ�ֹ���: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
void paraSaveTimerCB(void *timeOutPara)
{
    servStimerDelete(&g_paraSaveTimer);
    
    g_bPerdPostSemToPara = true;
}


/*********************************************************************************************
�� �� ��: ParaSaveTask;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void ParaSaveTask(void *p_arg)
{
    OS_ERR  os_err;

    (void)p_arg;

    while (1)
    {
        //�ȴ��ź���
        OSSemPend(&g_semParaSaveTask, 0, OS_OPT_PEND_BLOCKING, NULL, &os_err);


        servInterfaceInfoWrite(&g_commIntfc);
        
        servSystemInfoWrite(&g_systemInfo);
        
        servMotorInfoWrite(&g_motorInfo);
        
        servMotionInfoWrite(&g_motionInfo);
        
        servReportInfoWrite(&g_reportInfo);
        
        servTrigInInfoWrite(&g_trigInInfo);
        
        servTrigOutInfoWrite(&g_digitalOut);
        
        servDriverInfoStore(&g_driverInfo);

        servIsolatorOutInfoStore(&g_isolatorOut);
        
        servSensorUartInfoStore(&g_sensorUart);

        servPlanInfoStore(&g_planInfo);

#ifdef PROJECT_GELGOOG

#if GELGOOG_SINANJU
        //����������
        servSensorAlarmInfoStore(&g_sensorAlarm);

#else

        //��������
        servIsolatorInInfoStore(&g_isolatorIn);
#endif

#endif

#ifdef PROJECT_QUBELEY
        if (SDMODEL_C23D == g_deviceInfo.sDevcModel)    //-D����AI��OTP
        {
            //ģ������
            servAnalogInInfoWrite(&g_analogInfo);
        }
#endif
        
        /*OSTimeDlyHMSM(0, 0, 0, 1, OS_OPT_TIME_HMSM_STRICT, &os_err);*/
    }
}



/*******************************************�ļ�β********************************************/
