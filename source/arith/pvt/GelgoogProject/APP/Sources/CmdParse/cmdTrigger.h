/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdTrigger.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __CMD_TRIGGER_H
#define __CMD_TRIGGER_H



/******************************************�����ļ�*******************************************/
#include "comDataType.h"
#include "comCmd.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/



/*******************************************�궨��********************************************/ 



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void cmdTrigInCmdInit(void);
void cmdTrigInCmdProc(CmdParseFrameStruct *pCmdStackFrame);

#ifdef PROJECT_GELGOOG
#if !GELGOOG_SINANJU
void cmdIsolInMonitorTimerCB(void *timeOutPara);

void cmdIsolInCmdInit(void);
void cmdIsolInCmdProc(CmdParseFrameStruct *pCmdStackFrame);
#endif
#endif

#if !GELGOOG_SINANJU
void cmdTrigOutCmdInit(void);
void cmdTrigOutCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdIsolatorOutCmdInit(void);
void cmdIsolatorOutCmdProc(CmdParseFrameStruct *pCmdStackFrame);
#endif



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/