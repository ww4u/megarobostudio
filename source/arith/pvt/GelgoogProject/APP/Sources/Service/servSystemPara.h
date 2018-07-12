/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servSysPara.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.15;
历史版本:  无;
*********************************************************************************************/
#ifndef _SYS_PARA_H
#define _SYS_PARA_H



/******************************************包含文件*******************************************/
#include "comDataType.h"
#include "comStruct.h"
#include "comInterface.h"



/****************************************外部变量声明*****************************************/



/*******************************************宏定义********************************************/



/***************************************常数和类型声明****************************************/



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
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



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
