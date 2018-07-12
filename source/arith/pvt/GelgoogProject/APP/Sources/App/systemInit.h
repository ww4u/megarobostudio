/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  systemInit.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.01.11;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comMemoryMap.h"
#include "comDebug.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/
typedef enum
{
    SYSTASK_CMDPARSE = 0,    //�����������
    SYSTASK_PVTCALC,         //PVT��������
    SYSTASK_FUNCTION,        
    SYSTASK_PERIODIC,        
    SYSTASK_EVENMANG,        
    
}SystemTaskEnum;



/*******************************************�궨��********************************************/ 
#define    APP_TASK_PRIO_SYSTEM_INIT      2u
#define    APP_TASK_PRIO_CMD_PARSE        3u
#define    APP_TASK_PRIO_PVT_CALC         7u
#define    APP_TASK_PRIO_FUNCTION         5u
#define    APP_TASK_PRIO_PERIODIC         6u
#define    APP_TASK_PRIO_EVENT_MANAGE     4u

#define    APP_TASK_STK_SIZE_SYSTEM_INIT    256u
#define    APP_TASK_STK_SIZE_CMD_PARSE      512u
#define    APP_TASK_STK_SIZE_PVT_CALC       512u
#define    APP_TASK_STK_SIZE_FUNCTION       512u
#define    APP_TASK_STK_SIZE_PERIODIC       512u
#define    APP_TASK_STK_SIZE_EVENT_MANAGE   512u



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void SystemParaReset(void);
void SystemOSCreate(void * pHCLKFrequency);
void sysTaskCreate(SystemTaskEnum sysTask);
void sysTaskDelete(SystemTaskEnum sysTask);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/