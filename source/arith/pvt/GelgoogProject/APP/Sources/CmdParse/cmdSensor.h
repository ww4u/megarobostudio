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



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/