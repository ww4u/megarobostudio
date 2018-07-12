/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名: servDeviceInfo.h；
功能描述:     ;
函数列表:     ;
当前版本:     1.0.0;
版本改动:     ;
作     者:  CJ;
完成日期:     2017.10.30;
历史版本:     无;
*********************************************************************************************/
#ifndef __SERV_DEVICE_INFO_H
#define __SERV_DEVICE_INFO_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comEnum.h"
#include "comStruct.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/*******************************************宏定义********************************************/



/***************************************常数和类型声明****************************************/



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void servDeviceInfoInit(DeviceInfoStruct *deviceInfo, u8 *bootVer);
#ifdef PROJECT_GELGOOG
FpgaTypeEnum servFpgaTypeRead(void);
#endif
void servSystemInfoInit(SubDeviceModelEnum sDevcModel);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/