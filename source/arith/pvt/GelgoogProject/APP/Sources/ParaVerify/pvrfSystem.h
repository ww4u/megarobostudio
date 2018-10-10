/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfSystem.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PARA_VERIFY_SYSTEM_H
#define __PARA_VERIFY_SYSTEM_H



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
u8 pvrfSystemModeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfSystemPowerOnVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfSystemDioRefReadVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfSystemReverseMotionVerify(u8 dataLen, u8 *pData, void *pParaValue);

#if GELGOOG_SINANJU
u8 pvrfSystemFanParaVerify(u8 dataLen, u8 *pData, void *pParaValue1, void *pParaValue2);
u8 pvrfSystemArmLedParaVerify(u8 dataLen, u8 *pData, void *pParaValue1, void *pParaValue2, u8 *pIndex);
u8 pvrfSystemArmLedFreqVerify(u8 dataLen, u8 *pData, SensorStateEnum *pLedState, u16 *pLedFreq);
#endif



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
