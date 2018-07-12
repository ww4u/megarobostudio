/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servCommIntfc.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.08.22;
历史版本:  无;
*********************************************************************************************/
#ifndef __SERV_COMM_INTFC_H
#define __SERV_COMM_INTFC_H



/******************************************包含文件*******************************************/
#include "comDataType.h"
#include "comCmd.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "servMsgEscape.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/ 
#define    GET_CAN_PHY_FRMAE_DATA_SIZE    frameLen - sizeof(CanPhyFrameStruct)
#define    CAN_FRAME_BYTES_MAX            8       //CAN一帧发送的字节数，最大为8字节

#define    UART_START_OF_FRAME                FE_BEFORE_ESCAPE 
#define    GET_UART_PHY_FRMAE_PAYLOAD_SIZE    frameLen - sizeof(UartPhyFrameStruct)

#define    UART_FRAME_BYTES_BFESCAPE_MAX    124     //Uart一帧发送的最大字节数，转义前
#define    UART_FRAME_BYTES_AFESCAPE_MAX    0xFC    //Uart一帧发送的最大字节数，转义后



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void servCommStreamBufferInit(void);
void servFrameSend(CmdTypeEnum cmdMainType, u8 cmdSubType, u8 dataLen, u8 *pData, LinkTypeEnum linkType);
void servTxFrameProcess(void);
void servRxFrameProcess(void);

void irqCiCanReceive(void);
void irqCiUartReceive(void);

void servCiUartDmaRecTimerCB(void *timeOutPara);
void irqCiUartDmaComplReceive(void);

void irqSensor1UartDmaComplReceive(void);
void irqSensor1UartReceive(void);
void servSensor1UartDmaRecTimerCB(void *timeOutPara);
void servSensor1UartSwitchTimerCB(void *timeOutPara);
void servSensor1UartReciveOff(void);
void servSensor1UartReciveOn(SensorManageStruct sensor, SensorNumEnum sensorNum);

void irqSensor2UartDmaComplReceive(void);
void irqSensor2UartReceive(void);
void servSensor2UartDmaRecTimerCB(void *timeOutPara);
void servSensor2UartSwitchTimerCB(void *timeOutPara);
void servSensor2UartReciveOff(void);
void servSensor2UartReciveOn(SensorManageStruct sensor, SensorNumEnum sensorNum);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/