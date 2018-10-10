/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  paraSaveTask.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2018.06.10;
历史版本:  无;
*********************************************************************************************/
#include <os.h>
#include "paraSaveTask.h"
#include "servSystemPara.h"
#include "servSoftTimer.h"



/****************************************外部变量声明*****************************************/
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



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SoftTimerStruct g_paraSaveTimer;



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: paraSaveTimerCB
实现功能: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
void paraSaveTimerCB(void *timeOutPara)
{
    servStimerDelete(&g_paraSaveTimer);
    
    g_bPerdPostSemToPara = true;
}


/*********************************************************************************************
函 数 名: ParaSaveTask;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void ParaSaveTask(void *p_arg)
{
    OS_ERR  os_err;

    (void)p_arg;

    while (1)
    {
        //等待信号量
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
        //传感器警报
        servSensorAlarmInfoStore(&g_sensorAlarm);

#else

        //隔离输入
        servIsolatorInInfoStore(&g_isolatorIn);
#endif

#endif

#ifdef PROJECT_QUBELEY
        if (SDMODEL_C23D == g_deviceInfo.sDevcModel)    //-D才有AI和OTP
        {
            //模拟输入
            servAnalogInInfoWrite(&g_analogInfo);
        }
#endif
        
        /*OSTimeDlyHMSM(0, 0, 0, 1, OS_OPT_TIME_HMSM_STRICT, &os_err);*/
    }
}



/*******************************************文件尾********************************************/
