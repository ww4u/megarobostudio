/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfDriver.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.10.25;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_DRIVER_H
#define __PARA_VERIFY_DRIVER_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/ 



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
u8 pvrfDriverCurrVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfDriverMicroStepsVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfDriverStateVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfDriverMonitorPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfDriverRegisterVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfDriverSwitchTimeVerify(u8 dataLen, u8 *pData, void *pParaValue);

#if 0
#endif

u8 pvrfTuningCurrRatioVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfTuningEnergyEfficVerify(u8 dataLen, u8 *pData, u16 *pEnergEfficMax, u16 *pEnergEfficOffset);
u8 pvrfTuningCurrRegulateVerify(u8 dataLen, u8 *pData, CurrIncreEnum *pCurrIncre, CurrDecreEnum *pCurrDecre);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/