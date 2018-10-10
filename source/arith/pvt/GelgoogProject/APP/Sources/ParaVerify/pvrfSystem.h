/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfSystem.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_SYSTEM_H
#define __PARA_VERIFY_SYSTEM_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/*******************************************宏定义********************************************/ 



/***************************************常数和类型声明****************************************/



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
u8 pvrfSystemModeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfSystemPowerOnVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfSystemDioRefReadVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfSystemReverseMotionVerify(u8 dataLen, u8 *pData, void *pParaValue);

#if GELGOOG_SINANJU
u8 pvrfSystemFanParaVerify(u8 dataLen, u8 *pData, void *pParaValue1, void *pParaValue2);
u8 pvrfSystemArmLedParaVerify(u8 dataLen, u8 *pData, void *pParaValue1, void *pParaValue2, u8 *pIndex);
u8 pvrfSystemArmLedFreqVerify(u8 dataLen, u8 *pData, SensorStateEnum *pLedState, u16 *pLedFreq);
#endif



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
