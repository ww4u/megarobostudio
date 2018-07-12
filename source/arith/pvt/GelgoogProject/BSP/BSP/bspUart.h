/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspUart.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.24;
历史版本:  无;
*********************************************************************************************/
#ifndef __BSP_UART_H
#define __BSP_UART_H



/******************************************包含文件*******************************************/
#include "stm32f4xx.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/
/*-------------UART波特率枚举-------------*/
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

/*-------------UART数据长度枚举-------------*/
typedef enum
{
    WORDLEN_8 = 0,    //4800bps
    WORDLEN_9,
    WORDLEN_RESERVE
    
}WordLenEnum;

/*-------------UART流控制枚举-------------*/
typedef enum
{
    STOPBIT_10 = 0,
    STOPBIT_05,
    STOPBIT_20,
    STOPBIT_15,
    STOPBIT_RESERVE
    
}StopBitEnum;

/*-------------UART校验方式枚举-------------*/
typedef enum
{
    PARITY_NONE = 0,
    PARITY_EVEN,
    PARITY_ODD,
    PARITY_RESERVE
    
}ParityEnum;

/*-------------UART停止位枚举-------------*/
typedef enum
{
    FLOWCTL_NONE = 0,
    FLOWCTL_RTS,
    FLOWCTL_CTS,
    FLOWCTL_RTS_CTS,
    FLOWCTL_RESERVE
    
}FlowCtlEnum;

/*-------------UART接口结构体------------*/
typedef struct //5字节
{
    UartBaudEnum baud;       //波特率
    WordLenEnum  wordLen;    //数据长度
    StopBitEnum  stopBit;    //停止位
    ParityEnum   parity;     //校验方式
    FlowCtlEnum  flowCtl;    //流控制    
}UartIntfcStruct;


/*******************************************宏定义********************************************/
#define    UART_SN2_UART_USE_DMA     1



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
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



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/