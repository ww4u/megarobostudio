/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdSensor.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
*********************************************************************************************/
#ifndef __CMD_SENSOR_H
#define __CMD_SENSOR_H



/******************************************包含文件*******************************************/
#include "comDataType.h"
#include "comCmd.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/ 



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void cmdOtpCmdInit(void);
void cmdOtpCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdAnalogInCmdInit(void);
void cmdAnalogInCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdReportCmdInit(void);
void cmdReportCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdReportTorqueTimerCB(void *timeOutPara);
void cmdReportTempTimerCB(void *timeOutPara);
void cmdReportCycleTimerCB(void *timeOutPara);
void cmdReportXAngleTimerCB(void *timeOutPara);
void cmdReportYAngleTimerCB(void *timeOutPara);
void cmdReportZAngleTimerCB(void *timeOutPara);
void cmdReportXVelocityTimerCB(void *timeOutPara);
void cmdReportYVelocityTimerCB(void *timeOutPara);
void cmdReportZVelocityTimerCB(void *timeOutPara);
void cmdReportXAccTimerCB(void *timeOutPara);
void cmdReportYAccTimerCB(void *timeOutPara);
void cmdReportZAccTimerCB(void *timeOutPara);
void cmdReportAngleSensorTimerCB(void *timeOutPara);

void cmdReportSgAllTimerCB(void *timeOutPara);
void cmdReportSgSeTimerCB(void *timeOutPara);
void cmdReportDriveMonitorTimerCB(void *timeOutPara);

void cmdReportDistanceTimerCB(void *timeOutPara);
void cmdReportAbsEncoderTimerCB(void *timeOutPara);
void cmdReportOutNumTimerCB(void *timeOutPara);
void cmdReportStepsTimerCB(void *timeOutPara);
void cmdReportVelorityTimerCB(void *timeOutPara);

void cmdSensorUartCmdInit(void);
void cmdSensorUartCmdProc(CmdParseFrameStruct *pCmdStackFrame);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/