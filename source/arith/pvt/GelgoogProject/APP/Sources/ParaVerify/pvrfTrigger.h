/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfTrigger.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PARA_VERIFY_TRIGGER_H
#define __PARA_VERIFY_TRIGGER_H



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
u8 pvrfTrigInModeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfTrigInPattStateVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfTrigInPattTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfTrigInPattResponseVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfTrigInPattSModeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfTrigInPattSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfTrigInLevelStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigInLevelTypeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigInLevelResponseVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigInLevelSModeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigInLevelSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);

u8 pvrfTrigOutStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigOutSourceVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigOutConditionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigOutSignalVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigOutPolarityVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigOutPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfTrigOutDutyVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);

#ifdef PROJECT_GELGOOG
u8 pvrfIsolatorInStateVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfIsolatorInTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfIsolatorInResponseVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfIsolatorInRespChanVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfIsolatorInSModeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfIsolatorInSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue);
#endif

u8 pvrfIsolatorOutStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfIsolatorOutConditionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfIsolatorOutSourceVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);
u8 pvrfIsolatorOutResponseVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
