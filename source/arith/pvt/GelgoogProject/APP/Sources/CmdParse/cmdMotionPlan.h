/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdMotionPlan.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __CMD_MOTION_PLAN_H
#define __CMD_MOTION_PLAN_H



/******************************************�����ļ�*******************************************/
#include "comDataType.h"
#include "comCmd.h"



/****************************************�ⲿ��������*****************************************/



/*******************************************�궨��********************************************/



/***************************************��������������****************************************/



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void cmdPvtCmdInit(void);
void cmdPvtCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdPvtPositionSetCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtPositionQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtSpeedSetCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtSpeedQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtTimeSetCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtTimeQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/