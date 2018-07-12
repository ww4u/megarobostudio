/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfPreset.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.06.29;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_PRESET_H
#define __PARA_VERIFY_PRESET_H



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
u8 pvrfPresetPvtPositionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfPresetPvtSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfPresetPvtTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfPresetStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/