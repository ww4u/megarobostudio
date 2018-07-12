/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfMotor.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PARA_VERIFY_MOTOR_H
#define __PARA_VERIFY_MOTOR_H



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
u8 pvrfMotorGearRatioVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorStepAngleVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorPeakSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorPeakAccVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorPosnUnitVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorSizeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorLeadVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorVoltVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorCurrVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorBacklashVerify(u8 dataLen, u8 *pData, void *pParaValue);

u8 pvrfEncoderLineNumVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfEncoderChanNumVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfEncoderTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfEncoderMultipleVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfEncoderStateVerify(u8 dataLen, u8 *pData, void *pParaValue);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/