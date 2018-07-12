/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servStreamBuffer.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.11.14;
历史版本:  无;
*********************************************************************************************/
#ifndef __SERV_STRAM_BUFFER_H
#define __SERV_STRAM_BUFFER_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/*******************************************宏定义********************************************/



/***************************************常数和类型声明****************************************/



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void servStreamBufferInit(StreamBufferStruct *pStreamBuffer, u8* pBuffer, u16 bufferSize);
u8 * servGetHead(StreamBufferStruct *pStreamBuffer);
void servEnqueue(StreamBufferStruct *pStreamBuffer, u16 length);
void servDequeue(StreamBufferStruct *pStreamBuffer, u16 length);
u8 * servReserveMemory(StreamBufferStruct *pStreamBuffer, u16 bufferSize);
void *servFastReserveMemory(StreamBufferStruct *pStreamBuffer, u16 bufferSize);
void *servFastGetTail(StreamBufferStruct pStreamBuffer);
void servFastEnqueue(StreamBufferStruct *pStreamBuffer, u16 bufferSize);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/