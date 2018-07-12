/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servCommIntfc.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.08.22;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __SERV_COMM_INTFC_H
#define __SERV_COMM_INTFC_H



/******************************************�����ļ�*******************************************/
#include "comDataType.h"
#include "comCmd.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "servMsgEscape.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/



/*******************************************�궨��********************************************/ 
#define    GET_CAN_PHY_FRMAE_DATA_SIZE    frameLen - sizeof(CanPhyFrameStruct)
#define    CAN_FRAME_BYTES_MAX            8       //CANһ֡���͵��ֽ��������Ϊ8�ֽ�

#define    UART_START_OF_FRAME                FE_BEFORE_ESCAPE 
#define    GET_UART_PHY_FRMAE_PAYLOAD_SIZE    frameLen - sizeof(UartPhyFrameStruct)

#define    UART_FRAME_BYTES_BFESCAPE_MAX    124     //Uartһ֡���͵�����ֽ�����ת��ǰ
#define    UART_FRAME_BYTES_AFESCAPE_MAX    0xFC    //Uartһ֡���͵�����ֽ�����ת���



/******************************************��������*******************************************/



/******************************************��������*******************************************/
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



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/