/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servSoftTimer.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.11.11;
历史版本:  无;
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



/****************************************外部变量声明*****************************************/
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



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servStimerInit;
实现功能: 软件定时器初始化; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servStimerInit(SoftTimerStruct *pSoftTimer, u16 interval, TimeOutFunc pTimerOutFunc)
{
    pSoftTimer->period = interval / PERIOD_TASK_PERIOD;
    if (0 == pSoftTimer->period)    //最小1个周期
    {
        pSoftTimer->period = 1;
    }
    pSoftTimer->timeout = 0;
    pSoftTimer->pTimerOutFunc = pTimerOutFunc;
    pSoftTimer->bUsed = false;
}


/*********************************************************************************************
函 数 名: servStimerExamine;
实现功能: 检查定时器是否超时; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: servStimerAdd;
实现功能: 启动定时器; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servStimerAdd(SoftTimerStruct *ptSoftTimer)
{
    ptSoftTimer->bUsed = true;
    ptSoftTimer->timeout = 0;
}


/*********************************************************************************************
函 数 名: servStimerDelete;
实现功能: 关闭定时器; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servStimerDelete(SoftTimerStruct *ptSoftTimer)
{
    ptSoftTimer->bUsed = false;
    ptSoftTimer->timeout = 0;
}


/*********************************************************************************************
函 数 名: servStimerAllInit;
实现功能: 初始化所有的软定时器; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servStimerAllInit(void)
{
    u8 i;

    
    //串口DMA接收定时器，超时时间60ms
    servStimerInit(&g_ciUartDmaRecTimer, 60, servCiUartDmaRecTimerCB);

    servStimerInit(&g_senUartDmaRecTimer[UARTNUM_U1], 110,  servSensor1UartDmaRecTimerCB); 
    servStimerInit(&g_senUartSwitchTimer[UARTNUM_U1], 1000, servSensor1UartSwitchTimerCB); 

    servStimerInit(&g_senUartDmaRecTimer[UARTNUM_U2], 110,  servSensor2UartDmaRecTimerCB); 
    servStimerInit(&g_senUartSwitchTimer[UARTNUM_U2], 1000, servSensor2UartSwitchTimerCB); 
    
    //运动状态上报定时器
    for (i = 0;i < CH_TOTAL;i++)
    {
        servStimerInit(&g_motionStateTimer[i], 1000, EventMotionStateTimerCB);

        //运动状态监测定时器
        servStimerInit(&g_motionMonitorTimer[i], 200, EventMotionMonitorTimerCB);

        //5个上报器定时器初始化(循环数不需要定时上报，数值改变后才上报)
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

        //内部，不对外
        servStimerInit(&g_reportTimer[i][REPTTYPE_ANGLESEN], 100,  cmdReportAngleSensorTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVMONT],  1000, cmdReportDriveMonitorTimerCB); 
        servStimerInit(&g_reportTimer[i][REPTTYPE_DISTANCE], 1000, cmdReportDistanceTimerCB);
        servStimerInit(&g_reportTimer[i][REPTTYPE_ABSENC],   1000, cmdReportAbsEncoderTimerCB);

        //丢步告警上报定时器
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



/*******************************************文件尾********************************************/
