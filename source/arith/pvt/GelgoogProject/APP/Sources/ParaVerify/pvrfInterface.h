/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfInterface.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_INTFC_H
#define __PARA_VERIFY_INTFC_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/ 



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
u8 pvrfUartBaudVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfUartWordLenVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfUartFlowCtlVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfUartParityVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfUartStopBitVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfCanTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfCanSendIdVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfCanBaudVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfCanGroupVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfCanReceiveIdVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfCanGroupIdVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfCanRadioIdVerify(u8 dataLen, u8 *pData, void *pParaValue);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/