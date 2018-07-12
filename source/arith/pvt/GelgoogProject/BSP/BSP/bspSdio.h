/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspSdio.h；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.18;
历史版本:  无;
**********************************************************************************************/
#ifndef __BSP_SDIO_H
#define __BSP_SDIO_H



/******************************************包含文件*******************************************/
#include "stm32f4xx.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/
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



/*******************************************宏定义********************************************/



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void bspSdioInit(void);
u8   bspSdioCmdSend(u16 addr, u16 length, unsigned char needDelay);
void bspSdioDataSend(u8 *dataBuff, u32 dataLen, u16 dataBlock);
void bspSdioDataReceive(u8 *dataBuff, u16 dataLen, u8 dataBlock);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
