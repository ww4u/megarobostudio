/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servSysPara.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef _SYS_PARA_H
#define _SYS_PARA_H



/******************************************�����ļ�*******************************************/
#include "comDataType.h"
#include "comStruct.h"
#include "comInterface.h"



/****************************************�ⲿ��������*****************************************/



/*******************************************�궨��********************************************/



/***************************************��������������****************************************/



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void servBootInfoWrite(BootInfoStruct * info);
int  servBootInfoRead(BootInfoStruct * info);
void servInterfaceInfoWrite(CommIntfcStruct * info);
int  servInterfaceInfoRead(CommIntfcStruct * info);
void servSystemInfoWrite(SystemInfoStruct * info);
int  servSystemInfoRead(SystemInfoStruct * info);
void servMotorInfoWrite(MotorInfoStruct * info);
int  servMotorInfoRead(MotorInfoStruct * info);
int  servMotionInfoWrite(MotionInfoStruct *info);
int  servMotionInfoRead(MotionInfoStruct *info);
int  servReportInfoWrite(ReportInfoStruct *info);
int  servReportInfoRead(ReportInfoStruct *info);
int  servPlanInfoWrite(PlanInfoStruct *info);
int  servPlanInfoRead(PlanInfoStruct *info);

#ifdef PROJECT_QUBELEY
int servAnalogInInfoWrite(AnalogInfoStruct *info);
int servAnalogInInfoRead(AnalogInfoStruct *info);
#endif

#ifdef PROJECT_GELGOOG
int servIsolatorInInfoStore(IsolatorInStruct *info);
int servIsolatorInInfoRead(IsolatorInStruct *info);
#endif

int  servTrigInInfoWrite(TrigInInfoStruct *info);
int  servTrigInInfoRead(TrigInInfoStruct *info);
int  servTrigOutInfoWrite(DigitalOutStruct *info);
int  servTrigOutInfoRead(DigitalOutStruct *info);
int  servDeviceInfoStore(DeviceInfoStorageStruct *info);
int  servDeviceInfoRead(DeviceInfoStorageStruct *info);
int  servIsolatorOutInfoStore(IsolatorOutStruct *info);
int  servIsolatorOutInfoRead(IsolatorOutStruct *info);
int  servSensorUartInfoStore(SensorUartStruct *info);
int  servSensorUartInfoRead(SensorUartStruct *info);
int  servDriverInfoStore(DriverInfoStruct *info);
int  servDriverInfoRead(DriverInfoStruct *info);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
