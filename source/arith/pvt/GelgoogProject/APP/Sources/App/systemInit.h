/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  systemInit.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.01.11;
历史版本:  无;
*********************************************************************************************/
#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comMemoryMap.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/
typedef enum
{
    SYSTASK_CMDPARSE = 0,    //命令解析任务
    SYSTASK_PVTCALC,         //PVT结算任务
    SYSTASK_FUNCTION,        
    SYSTASK_PERIODIC,        
    SYSTASK_EVENMANG,        
    
}SystemTaskEnum;



/*******************************************宏定义********************************************/ 
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



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void SystemParaReset(void);
void SystemOSCreate(void * pHCLKFrequency);
void sysTaskCreate(SystemTaskEnum sysTask);
void sysTaskDelete(SystemTaskEnum sysTask);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/