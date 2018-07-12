/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfInterface.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PARA_VERIFY_INTFC_H
#define __PARA_VERIFY_INTFC_H



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



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/