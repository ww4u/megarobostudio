/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfDriver.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.10.25;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PARA_VERIFY_DRIVER_H
#define __PARA_VERIFY_DRIVER_H



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/



/*******************************************�궨��********************************************/ 



/******************************************��������*******************************************/



/******************************************��������*******************************************/
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



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/