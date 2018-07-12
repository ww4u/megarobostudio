/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdMotionPlan.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
*********************************************************************************************/
#ifndef __CMD_MOTION_PLAN_H
#define __CMD_MOTION_PLAN_H



/******************************************包含文件*******************************************/
#include "comDataType.h"
#include "comCmd.h"



/****************************************外部变量声明*****************************************/



/*******************************************宏定义********************************************/



/***************************************常数和类型声明****************************************/



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void cmdPvtCmdInit(void);
void cmdPvtCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdPvtPositionSetCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtPositionQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtSpeedSetCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtSpeedQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtTimeSetCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPvtTimeQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/