/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfMotion.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PARA_VERIFY_MOTION_H
#define __PARA_VERIFY_MOTION_H



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
u8 pvrfMotionStateReportVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionStartSrcVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionStartTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);

u8 pvrfMotionMaxSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMinSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMaxPosnVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMinPosnVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMaxTorqueVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMinTorqueVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMaxAccVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMinAccVerify(u8 dataLen, u8 *pData, void *pParaValue);

u8 pvrfMotionOffsetStateVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionReverseMotionVerify(u8 dataLen, u8 *pData, void *pParaValue);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
