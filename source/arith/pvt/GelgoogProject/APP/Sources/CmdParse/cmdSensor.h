/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdSensor.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __CMD_SENSOR_H
#define __CMD_SENSOR_H



/******************************************�����ļ�*******************************************/
#include "comDataType.h"
#include "comCmd.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/



/*******************************************�궨��********************************************/ 



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void cmdOtpCmdInit(void);
void cmdOtpCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdAnalogInCmdInit(void);
void cmdAnalogInCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdReportCmdInit(void);
void cmdReportCmdProc(CmdParseFrameStruct *pCmdStackFrame);

void cmdReportTorqueTimerCB(void *timeOutPara);
void cmdReportTempTimerCB(void *timeOutPara);
void cmdReportCycleTimerCB(void *timeOutPara);

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

#if GELGOOG_SINANJU
void cmdAbsEncAlarmCmdInit(void);
void cmdAbsEncAlarmCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdDistAlarmCmdInit(void);
void cmdDistAlarmCmdProc(CmdParseFrameStruct *pCmdStackFrame);
void cmdPdmCmdInit(void);
void cmdPdmCmdProc(CmdParseFrameStruct *pCmdStackFrame);
#endif



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/