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
u8 pvrfDriverSGLimitVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfDriverSGParaVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/