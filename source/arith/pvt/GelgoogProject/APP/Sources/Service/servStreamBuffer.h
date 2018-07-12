/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servStreamBuffer.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.11.14;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __SERV_STRAM_BUFFER_H
#define __SERV_STRAM_BUFFER_H



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comDebug.h"



/****************************************�ⲿ��������*****************************************/



/*******************************************�궨��********************************************/



/***************************************��������������****************************************/



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void servStreamBufferInit(StreamBufferStruct *pStreamBuffer, u8* pBuffer, u16 bufferSize);
u8 * servGetHead(StreamBufferStruct *pStreamBuffer);
void servEnqueue(StreamBufferStruct *pStreamBuffer, u16 length);
void servDequeue(StreamBufferStruct *pStreamBuffer, u16 length);
u8 * servReserveMemory(StreamBufferStruct *pStreamBuffer, u16 bufferSize);
void *servFastReserveMemory(StreamBufferStruct *pStreamBuffer, u16 bufferSize);
void *servFastGetTail(StreamBufferStruct pStreamBuffer);
void servFastEnqueue(StreamBufferStruct *pStreamBuffer, u16 bufferSize);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/