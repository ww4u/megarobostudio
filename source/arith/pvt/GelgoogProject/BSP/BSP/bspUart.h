/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspUart.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.24;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __BSP_UART_H
#define __BSP_UART_H



/******************************************�����ļ�*******************************************/
#include "stm32f4xx.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/
/*-------------UART������ö��-------------*/
typedef enum
{
    UARTBAUD_4800 = 0,    //4800bps
    UARTBAUD_7200,
    UARTBAUD_9600,
    UARTBAUD_14400,
    UARTBAUD_19200,
    UARTBAUD_38400,
    UARTBAUD_57600,
    UARTBAUD_115200,
    UARTBAUD_128000,
    UARTBAUD_2000000,
    UARTBAUD_2500000,
    UARTBAUD_RESERVE
    
}UartBaudEnum;

/*-------------UART���ݳ���ö��-------------*/
typedef enum
{
    WORDLEN_8 = 0,    //4800bps
    WORDLEN_9,
    WORDLEN_RESERVE
    
}WordLenEnum;

/*-------------UART������ö��-------------*/
typedef enum
{
    STOPBIT_10 = 0,
    STOPBIT_05,
    STOPBIT_20,
    STOPBIT_15,
    STOPBIT_RESERVE
    
}StopBitEnum;

/*-------------UARTУ�鷽ʽö��-------------*/
typedef enum
{
    PARITY_NONE = 0,
    PARITY_EVEN,
    PARITY_ODD,
    PARITY_RESERVE
    
}ParityEnum;

/*-------------UARTֹͣλö��-------------*/
typedef enum
{
    FLOWCTL_NONE = 0,
    FLOWCTL_RTS,
    FLOWCTL_CTS,
    FLOWCTL_RTS_CTS,
    FLOWCTL_RESERVE
    
}FlowCtlEnum;

/*-------------UART�ӿڽṹ��------------*/
typedef struct //5�ֽ�
{
    UartBaudEnum baud;       //������
    WordLenEnum  wordLen;    //���ݳ���
    StopBitEnum  stopBit;    //ֹͣλ
    ParityEnum   parity;     //У�鷽ʽ
    FlowCtlEnum  flowCtl;    //������    
}UartIntfcStruct;


/*******************************************�궨��********************************************/
#define    UART_SN2_UART_USE_DMA     1



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void bspCiUartInit(UartIntfcStruct uartIntfc);
u8   bspCiUartSend(u8 *dataBuff, u8 dataLen);
void bspCiUartReciveOn(void);
void bspCiUartDmaOff(void);
u8   bspCiUartRecive(void);
void bspCiUartDmaRecive(u8 *dataBuff, u32 dataLen);
void bspCiUartITConfig(FunctionalState funcState);

void bspSensor1UartInit(UartIntfcStruct uartIntfc);
u8   bspSensor1UartSend(u8 *dataBuff, u8 dataLen);
void bspSensor1UartReciveOn(void);
void bspSensor1UartReciveOff(void);
void bspSensor1UartDmaOff(void);
u16  bspSensor1UartDmaCounterGet(void);
u8   bspSensor1UartRecive(void);
void bspSensor1UartDmaRecive(u8 *dataBuff, u32 dataLen);
void bspSensor1UartITConfig(FunctionalState funcState);

void bspSensor2UartInit(UartIntfcStruct uartIntfc);
u8   bspSensor2UartSend(u8 *dataBuff, u8 dataLen);
void bspSensor2UartReciveOn(void);
void bspSensor2UartReciveOff(void);
void bspSensor2UartDmaOff(void);
u16  bspSensor2UartDmaCounterGet(void);
u8   bspSensor2UartRecive(void);
void bspSensor2UartDmaRecive(u8 *dataBuff, u32 dataLen);
void bspSensor2UartITConfig(FunctionalState funcState);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/