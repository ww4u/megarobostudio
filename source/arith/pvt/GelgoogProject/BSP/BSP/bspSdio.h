/**********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspSdio.h��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.18;
��ʷ�汾:  ��;
**********************************************************************************************/
#ifndef __BSP_SDIO_H
#define __BSP_SDIO_H



/******************************************�����ļ�*******************************************/
#include "stm32f4xx.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/
/*< SDIO_CK = SDIOCLK / (SDIO_TRANSFER_CLK_DIV + 2) */
/*< on STM32F4xx devices, SDIOCLK is fixed to 48MHz */
typedef enum 
{
    SDIO_CLK_DIV_24MHz = 0,
    SDIO_CLK_DIV_18MHz = 1,
    SDIO_CLK_DIV_1MHz = 46,
    SDIO_CLK_DIV_400KHz = 118   
}euSDIO_CLK_DIV;




typedef enum 
{
    SDIO_BLK_1_Byte = SDIO_DataBlockSize_1b,
    SDIO_BLK_2_Byte = SDIO_DataBlockSize_2b,
    SDIO_BLK_4_Byte = SDIO_DataBlockSize_4b,
    SDIO_BLK_8_Byte = SDIO_DataBlockSize_8b,
    SDIO_BLK_16_Byte = SDIO_DataBlockSize_16b,
    SDIO_BLK_32_Byte = SDIO_DataBlockSize_32b,
    SDIO_BLK_64_Byte = SDIO_DataBlockSize_64b,
    SDIO_BLK_128_Byte = SDIO_DataBlockSize_128b,
    SDIO_BLK_256_Byte = SDIO_DataBlockSize_256b,
    SDIO_BLK_512_Byte = SDIO_DataBlockSize_512b,
    SDIO_BLK_1024_Byte = SDIO_DataBlockSize_1024b,
    SDIO_BLK_2048_Byte = SDIO_DataBlockSize_2048b,
    SDIO_BLK_4096_Byte = SDIO_DataBlockSize_4096b,
    SDIO_BLK_8192_Byte = SDIO_DataBlockSize_8192b,
    SDIO_BLK_16384_Byte = SDIO_DataBlockSize_16384b,
}euSDIO_BLK_SIZE;



/*******************************************�궨��********************************************/



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void bspSdioInit(void);
u8   bspSdioCmdSend(u16 addr, u16 length, unsigned char needDelay);
void bspSdioDataSend(u8 *dataBuff, u32 dataLen, u16 dataBlock);
void bspSdioDataReceive(u8 *dataBuff, u16 dataLen, u8 dataBlock);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
