/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfSensorpara.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_SENSOR_H
#define __PARA_VERIFY_SENSOR_H



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



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
