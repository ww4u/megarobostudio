/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  comInterface.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.09.03;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __COM_INTERFACE_H
#define __COM_INTERFACE_H



/******************************************�����ļ�*******************************************/
#include "bspCan.h"
#include "bspUart.h"



/***************************************��������������****************************************/
/*-------------��������ö��-------------*/
typedef enum
{
    LINK_NONE,
    LINK_CAN,
    LINK_UART
    
}LinkTypeEnum;

/*-----------CAN Phy��֡��ʽ�ṹ��-----------*/
typedef struct
{    
    u32 canID;         //CANID
    u8  frameLen;      //֡���ȣ�����ID
    u8  payload[0];    //֡����
    
}CanPhyFrameStruct;

/*-----------UART Phy��֡��ʽ�ṹ��-----------*/
typedef struct
{    
    u8 SOF;           //Start of frame
    u8 frameLen;      //֡���ȣ�����SOF������
    u8 xorValue;      //���У��ֵ
    u8 payload[0];    //֡����
    
}UartPhyFrameStruct;

/*-----------UART Phy��֡��ʽ�ṹ��-----------*/
typedef struct
{
    u8 frameLen;      //֡���ȣ�����SOF������    
    u8 SOF;           //Start of frame
    u8 payload[0];    //֡����
    
}SensorUartPhyFrameStruct;

/*-----------ϵͳ�ӿڽṹ��-------------*/
typedef struct
{
    LinkTypeEnum       linkType;    //�ظ�����ʱʹ��CAN����UART

    CanIntfcStruct     canIntfc;    //24���ֽ�
    UartIntfcStruct    uartIntfc;   //8���ֽ�
    u8  reserve[12];    //ǰ����36���ֽڣ�Ԥ��12���ֽڣ���CRC�ܹ�52���ֽ�
    u32 crc;
    
}CommIntfcStruct;    //��SystemIntfc���׺�SystemInfo�������ĳ�CommIntfc    CJ 2017.05.24 Modify



#endif
/*****************************************END OF FILE****************************************/
