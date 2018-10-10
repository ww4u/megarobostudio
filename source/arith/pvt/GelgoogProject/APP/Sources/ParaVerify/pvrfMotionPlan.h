/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfMotionPlan.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PARA_VERIFY_MOTION_PLAN_H
#define __PARA_VERIFY_MOTION_PLAN_H



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************�ⲿ��������*****************************************/



/*******************************************�궨��********************************************/



/***************************************��������������****************************************/



/******************************************��������*******************************************/



/******************************************��������*******************************************/
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



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/