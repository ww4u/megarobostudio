/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfMotionPlan.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_MOTION_PLAN_H
#define __PARA_VERIFY_MOTION_PLAN_H



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
u8 pvrfPvtExecModeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtPlanModeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtMotionModeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtModifyDutyVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtNcyclesVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtWarnPointVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtEndStateVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtStopDecelModeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtStopDecelTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtStopDecelDistanceVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtOOSStateVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtOOSLineOutNumVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtOOSTotalOutNumVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtOOSResponseVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex);

u8 pvrfPvtPositionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex, WaveTableTypeEnum *pWaveIndex);
u8 pvrfPvtTimeScaleVerify(u8 dataLen, u8 *pData, u16 *pAccScale, u16 *pDecScale, u8 *pIndex, WaveTableTypeEnum *pWaveIndex);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/