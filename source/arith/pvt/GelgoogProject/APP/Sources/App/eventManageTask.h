/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  eventManageTask.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.06.23;
历史版本:  无;
*********************************************************************************************/
#ifndef __EVENT_MANAGE_TASK_H
#define __EVENT_MANAGE_TASK_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/ 



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void EventManageTask(void *p_arg);
void irqEventOccur(EventSourceEnum eventSrc, u16 eventValue);

void eventMotionStateTimerCB(void *timeOutPara);
void eventMotionMonitorTimerCB(void *timeOutPara);
void eventLineOOStepTimerCB(void *timeOutPara);
void eventDriverCurrTimerCB(void *timeOutPara);
void eventDriverStateReadTimerCB(void *timeOutPara);

void eventDriverCurrProcess(u8 chanNum, SensorStateEnum state);

#ifdef PROJECT_GELGOOG

#if !GELGOOG_SINANJU
void eventIsolInMonitorTimerCB(void *timeOutPara);
#else
void eventDistAlarmLedTimerCB(void *timeOutPara);
void eventDistAlarmLedProcess(bool bRecoverDuty);
#endif

#if GELGOOG_AXIS_10
void eventDriverFaultTimerCB(void *timeOutPara);
#endif

#endif

void eventRunTimeCountTimerCB(void *timeOutPara);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/