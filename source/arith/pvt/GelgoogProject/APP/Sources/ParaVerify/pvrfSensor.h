/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfSensorpara.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PARA_VERIFY_SENSOR_H
#define __PARA_VERIFY_SENSOR_H



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"
#include "comEnum.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/



/*******************************************�궨��********************************************/ 



/******************************************��������*******************************************/



/******************************************��������*******************************************/
u8 pvrfOtpStateVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfOtpThresholdVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfOtpResponseVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfOtpPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue);

u8 pvrfAnalogInStateVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfAnalogInThresholdVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfAnalogInResponseVerify(u8 dataLen, u8 *pData, void *pParaValue);

u8 pvrfReportStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfReportPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);

u8 pvrfSensorUartStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex);
u8 pvrfSensorUartSofVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex);
u8 pvrfSensorUartFrameLenVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex);
u8 pvrfSensorUartRecvNumVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex);
u8 pvrfSensorUartSwTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
