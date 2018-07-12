/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfTrigger.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_TRIGGER_H
#define __PARA_VERIFY_TRIGGER_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comEnum.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/ 



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
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



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
