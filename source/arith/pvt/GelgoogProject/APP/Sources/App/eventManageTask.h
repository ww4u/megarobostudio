/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  eventManageTask.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.06.23;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __EVENT_MANAGE_TASK_H
#define __EVENT_MANAGE_TASK_H



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/



/*******************************************�궨��********************************************/ 



/******************************************��������*******************************************/



/******************************************��������*******************************************/
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



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/