/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdInterface.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __CMD_INTERFACE_H
#define __CMD_INTERFACE_H



/******************************************�����ļ�*******************************************/
#include "comDataType.h"
#include "comCmd.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/



/*******************************************�궨��********************************************/ 



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void cmdLinkCmdInit(void);
void cmdLinkCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdUartIntfcCmdInit(void);
void cmdUartIntfcCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdCanIntfcCmdInit(void);
void cmdCanIntfcCmdProc(CmdParseFrameStruct *pCmdStackFrame);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/