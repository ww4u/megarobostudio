/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdDriver.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  无;
作    者:  CJ;
完成日期:  2017.08.11;
历史版本:  无;
*********************************************************************************************/
#ifndef __CMD_DRIVER_H
#define __CMD_DRIVER_H



/******************************************包含文件*******************************************/
#include "comDataType.h"
#include "comCmd.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/ 



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void cmdDriverCmdInit(void);
void cmdDriverCmdProc(CmdParseFrameStruct *pCmdStackFrame);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/