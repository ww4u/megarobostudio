/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  comInterface.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.09.03;
历史版本:  无;
*********************************************************************************************/
#ifndef __COM_INTERFACE_H
#define __COM_INTERFACE_H



/******************************************包含文件*******************************************/
#include "bspCan.h"
#include "bspUart.h"



/***************************************常数和类型声明****************************************/
/*-------------连接类型枚举-------------*/
typedef enum
{
    LINK_NONE,
    LINK_CAN,
    LINK_UART
    
}LinkTypeEnum;

/*-----------CAN Phy层帧格式结构体-----------*/
typedef struct
{    
    u32 canID;         //CANID
    u8  frameLen;      //帧长度，包含ID
    u8  payload[0];    //帧数据
    
}CanPhyFrameStruct;

/*-----------UART Phy层帧格式结构体-----------*/
typedef struct
{    
    u8 SOF;           //Start of frame
    u8 frameLen;      //帧长度，包含SOF和自身
    u8 xorValue;      //异或校验值
    u8 payload[0];    //帧数据
    
}UartPhyFrameStruct;

/*-----------UART Phy层帧格式结构体-----------*/
typedef struct
{
    u8 frameLen;      //帧长度，包含SOF和自身    
    u8 SOF;           //Start of frame
    u8 payload[0];    //帧数据
    
}SensorUartPhyFrameStruct;

/*-----------系统接口结构体-------------*/
typedef struct
{
    LinkTypeEnum       linkType;    //回复命令时使用CAN还是UART

    CanIntfcStruct     canIntfc;    //24个字节
    UartIntfcStruct    uartIntfc;   //8个字节
    u8  reserve[12];    //前面是36个字节，预留12个字节，加CRC总共52个字节
    u32 crc;
    
}CommIntfcStruct;    //用SystemIntfc容易和SystemInfo混淆，改成CommIntfc    CJ 2017.05.24 Modify



#endif
/*****************************************END OF FILE****************************************/
