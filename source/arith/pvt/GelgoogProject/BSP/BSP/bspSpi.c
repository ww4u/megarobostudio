/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspSpi.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.18;
历史版本:  无;
**********************************************************************************************/
#include "bspSpi.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    SPI_DMA_MODE                 1

#define    SPI_FPGA_PROG_SPI            SPI1

#define    SPI_FP_SPI_CLK               RCC_APB2Periph_SPI1
#define    SPI_FP_SPI_DR_ADDR           (uint32_t)&SPI1->DR

#define    SPI_FP_SPI_GPIO_CLK          RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB
#define    SPI_FP_SPI_AF_PORT           GPIO_AF_SPI1
#define    SPI_FP_SPI_CLK_GPIO_PORT     GPIOA
#define    SPI_FP_SPI_CLK_PIN           GPIO_Pin_5
#define    SPI_FP_SPI_CLK_SOURCE        GPIO_PinSource5
#define    SPI_FP_SPI_MOSI_GPIO_PORT    GPIOB
#define    SPI_FP_SPI_MOSI_PIN          GPIO_Pin_5
#define    SPI_FP_SPI_MOSI_SOURCE       GPIO_PinSource5

#define    SPI_FP_SPI_TX_DMA_CLK        RCC_AHB1Periph_DMA2
#define    SPI_FP_SPI_TX_DMA_STREAM     DMA2_Stream5
#define    SPI_FP_SPI_TX_DMA_CHANNEL    DMA_Channel_3
#define    SPI_FP_SPI_TX_DMA_FLAG_TCIF  DMA_FLAG_TCIF5
#define    SPI_FP_SPI_TX_DMA_FLAG_HTIF  DMA_FLAG_HTIF5

#define    SPI_DATA_TIMEOUT             33600    //根据DELAY_COUNT_MS = 33600，此超时时间大约为1ms



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: bspFpSpiInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspFpSpiInit(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStructure;
#if SPI_DMA_MODE
    DMA_InitTypeDef  DMA_InitStructure;
#endif


    /***************************************SPI1 GPIO配置******************************************/
    RCC_AHB1PeriphClockCmd(SPI_FP_SPI_GPIO_CLK, ENABLE);

    GPIO_PinAFConfig(SPI_FP_SPI_CLK_GPIO_PORT,  SPI_FP_SPI_CLK_SOURCE,  SPI_FP_SPI_AF_PORT);      //指定复用功能
    GPIO_PinAFConfig(SPI_FP_SPI_MOSI_GPIO_PORT, SPI_FP_SPI_MOSI_SOURCE, SPI_FP_SPI_AF_PORT); 
    
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = SPI_FP_SPI_CLK_PIN;
    GPIO_Init(SPI_FP_SPI_CLK_GPIO_PORT,  &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI_FP_SPI_MOSI_PIN;
    GPIO_Init(SPI_FP_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);


    /***************************************SPI1 配置******************************************/
    RCC_APB2PeriphClockCmd(SPI_FP_SPI_CLK, ENABLE);

    SPI_I2S_DeInit(SPI_FPGA_PROG_SPI);

    //对PCLK2进行分频(目前系统中的PCLK2为84MHz)
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI_FPGA_PROG_SPI, &SPI_InitStructure);
    SPI_Cmd(SPI_FPGA_PROG_SPI, ENABLE);
    //SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);


    /***************************************SPI1 DMA配置******************************************/
#if SPI_DMA_MODE
    RCC_AHB1PeriphClockCmd(SPI_FP_SPI_TX_DMA_CLK, ENABLE);
    
    DMA_DeInit(SPI_FP_SPI_TX_DMA_STREAM);

    DMA_InitStructure.DMA_Channel = SPI_FP_SPI_TX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = SPI_FP_SPI_DR_ADDR;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    
    //内存地址、字节长度使用的时候再写
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_Init(SPI_FP_SPI_TX_DMA_STREAM, &DMA_InitStructure);

    //DMA_Cmd(SPI1_TX_DMA_STREAM, ENABLE);
    //SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
#endif

    /***************************************SPI1 中断配置******************************************/
    /*NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);*/
}


/*********************************************************************************************
函 数 名: bspFpSpiSend;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 bspFpSpiSend(u8 *dataBuff, u32 dataLen)
{
#if SPI_DMA_MODE

    SPI_FP_SPI_TX_DMA_STREAM->NDTR = dataLen;
    SPI_FP_SPI_TX_DMA_STREAM->M0AR = (u32)dataBuff;
    
    DMA_Cmd(SPI_FP_SPI_TX_DMA_STREAM, ENABLE);
    SPI_I2S_DMACmd(SPI_FPGA_PROG_SPI, SPI_I2S_DMAReq_Tx, ENABLE);
    
    while (DMA_GetFlagStatus(SPI_FP_SPI_TX_DMA_STREAM, SPI_FP_SPI_TX_DMA_FLAG_TCIF) == RESET);

    DMA_ClearFlag(SPI_FP_SPI_TX_DMA_STREAM, SPI_FP_SPI_TX_DMA_FLAG_TCIF | SPI_FP_SPI_TX_DMA_FLAG_HTIF); 

    DMA_Cmd(SPI_FP_SPI_TX_DMA_STREAM, DISABLE);
    SPI_I2S_DMACmd(SPI_FPGA_PROG_SPI, SPI_I2S_DMAReq_Tx, DISABLE);
    //SPI_Cmd(SPI1, DISABLE); 
    
#else

    u32 timeout = SPI_DATA_TIMEOUT;


    while (dataLen > 0)
    {
        while ((SPI_I2S_GetFlagStatus(SPI_FPGA_PROG_SPI, SPI_I2S_FLAG_TXE) == RESET) && (timeout > 0))
        {
            //等待发送完成
            timeout--;
        }

        if (0 == timeout)
        {
            return 1;    //发送出错
        }

        SPI_I2S_SendData(SPI_FPGA_PROG_SPI, *dataBuff);
        
        dataBuff++;
        dataLen--;
    }
    
#endif

    return 0;
}



/*******************************************文件尾********************************************/
