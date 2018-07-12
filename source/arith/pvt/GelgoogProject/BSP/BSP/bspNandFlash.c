/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspNandFlash.c；
功能描述:  提供NAND Flash (S34ML02G2 8bit 128K字节 2048)的底层接口函数。
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  xyzheng
完成日期:  2017.05.19;
历史版本:  遗留问题：NANA总共有2048块，但是只能用1024个块。原因尚不明确！！！！
            现象是，块擦除后，1024之后的块的spare没有被擦除（非0xFF）
**********************************************************************************************/
#include "bsp.h"
#include <string.h>
#include "bspNandFlash.h"

/*
    LUT  -- Look Up Table. s_usLUT中存储的是物理块号。在进行flash的读写操作时，选由逻辑地址，经过
            地址转换表，查找到逻辑地址对应的物理地址，然后在物理地址上进行真正的读写操作。

    如果在IAR或KEIL的编辑器中阅读，请将编辑器的字体设置为新宋体（9号/五号），缩进的TAB设置为4。
    否则，方框处出现不对齐的问题。

    【待完善的地方】
    （1）在操作NAND Flash时，如下语句是一个死循环。如果硬件出现异常，将导致软件死机
        while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 )

    （2）没有增加ECC校验功能。ECC可以检查1个或2个bit错误，如果只有1个bit错误，则可以修复这个bit。如果
        多余2个bit错误，则可能检测不到。

    （3）正常写文件操作时，会导致重建LUT。目前，重建LUT的代码执行效率还不够高，有待完善。

    【硬件说明】
    安富莱STM32F103ZE-EK开发板配置的NAND Flahs为海力士的HY27UF081G2A
    （1）NAND Flash的片选信号连接到CPU的FSMC_NCE2，这决定了NAND Flash的地址空间为 0x70000000（见CPU的数据
        手册的FSMC章节)
    （2）有FSMC总线上有多个总线设备（如TFT、SRAM、CH374T、NOR），因此必须确保其他总线设备的片选处于禁止
        状态，否则将出现总线冲突问题 （参见本文件初始化FSMC GPIO的函数）


    【NAND Flash 结构定义】
     备用区有16x4字节，每page 1024字节，每512字节一个扇区，每个扇区对应16个自己的备用区：

     每个PAGE的逻辑结构，前面512Bx4是主数据区，后面16Bx4是备用区
    ┌──────┐┌──────┐┌──────┐┌──────┐┌──────┐┌──────┐┌──────┐┌──────┐
    │ Main area  ││ Main area  ││ Main area  ││Main area   ││ Spare area ││ Spare area ││ Spare area ││Spare area  │
    │            ││            ││            ││            ││            ││            ││            ││            │
    │   512B     ││    512B    ││    512B    ││    512B    ││    16B     ││     16B    ││     16B    ││    16B     │
    └──────┘└──────┘└──────┘└──────┘└──────┘└──────┘└──────┘└──────┘

     每16B的备用区的逻辑结构如下:(三星推荐标准）
    ┌───┐┌───┐┌──┐┌──┐┌──┐┌───┐┌───┐┌───┐┌──┐┌──┐┌──┐┌──┐┌───┐┌───┐┌───┐┌───┐┌───┐
    │  BI  ││RESER ││LSN0││LSN1││LSN2││RESER ││RESER ││RESER ││ECC0││ECC1││ECC2││ECC0││S-ECC1││S-ECC0││RESER ││RESER ││RESER │
    │      ││ VED  ││    ││    ││    ││ VED  ││ VED  ││ VED  ││    ││    ││    ││    ││      ││      ││ VED  ││ VED  ││ VED  │
    └───┘└───┘└──┘└──┘└──┘└───┘└───┘└───┘└──┘└──┘└──┘└──┘└───┘└───┘└───┘└───┘└───┘

    K9F1G08U0A 和 HY27UF081G2A 是兼容的。芯片出厂时，厂商保证芯片的第1个块是好块。如果是坏块，则在该块的第1个PAGE的第1个字节
    或者第2个PAGE（当第1个PAGE坏了无法标记为0xFF时）的第1个字节写入非0xFF值。坏块标记值是随机的，软件直接判断是否等于0xFF即可。

    注意：网上有些资料说NAND Flash厂商的默认做法是将坏块标记定在第1个PAGE的第6个字节处。这个说法是错误。坏块标记在第6个字节仅针对部分小扇区（512字节）的NAND Flash
    并不是所有的NAND Flash都是这个标准。大家在更换NAND Flash时，请仔细阅读芯片的数据手册。


    为了便于在NAND Flash 上移植Fat文件系统，我们对16B的备用区采用以下分配方案:
    ┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌───┐┌───┐┌──┐┌──┐┌──┐┌──┐
    │ BI ││USED││LBN0││LBN1││ECC0││ECC1││ECC2││ECC3││ECC4││ECC5││S-ECC1││S-ECC0││RSVD││RSVD││RSVD││RSVD│
    │    ││    ││    ││    ││    ││    ││    ││    ││    ││    ││      ││      ││    ││    ││    ││    │
    └──┘└──┘└──┘└──┘└──┘└──┘└──┘└──┘└──┘└──┘└───┘└───┘└──┘└──┘└──┘└──┘
    - BI : 坏块标志(Bad Block Identifier)。每个BLOCK的第1个PAGE或者第2个PAGE的第1个字节指示该块是否坏块。0xFF表示好块，不是0xFF表示坏块。
    - USED : 该块使用标志。0xFF表示空闲块；0xF0表示已用块。
    - LBN0 LBN1 : 逻辑块号(Logic Block No) 。从0开始编码。只在每个BLOCK的第1个PAGE有效，其它PAGE该字段固定为0xFF FF
    - ECC0 ~ ECC6 : 512B主数据区的ECC校验 （按照三星提供ECC算法，256字节对应3个字节的ECC)
    - S-ECC1 S-ECC0 : LSN0和LSN2的ECC校验
    - RSVD : 保留字节，Reserved
     

    xyzheng
    1. 未对NANDFLASH做坏块管理
    2. 暂只支持块写入,和页读取,不支持页写入(因为擦除需要整块擦除).
    3. 未做ECC校验
*/

/* 定义NAND Flash的物理地址。这个是由硬件决定的 */
#define Bank2_NAND_ADDR    ((uint32_t)0x70000000)
#define Bank_NAND_ADDR     Bank2_NAND_ADDR

/* 定义操作NAND Flash用到3个宏 */
#define NAND_CMD_AREA       *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA)
#define NAND_ADDR_AREA      *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA)
#define NAND_DATA_AREA      *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA)

static uint8_t FSMC_NAND_GetStatus(void);
uint8_t NAND_WriteToNewBlock(uint32_t _ulPhyPageNo, uint8_t *_pWriteBuf, uint16_t _usOffset, uint16_t _usSize);

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_Init
*   功能说明: 配置FSMC和GPIO用于NAND Flash接口。这个函数必须在读写nand flash前被调用一次。
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************
*/
static void FSMC_NAND_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    FSMC_NANDInitTypeDef  FSMC_NANDInitStructure;
    FSMC_NAND_PCCARDTimingInitTypeDef  p;


    /*--NAND Flash GPIOs 配置  ------
        PD0/FSMC_D2
        PD1/FSMC_D3
        PD4/FSMC_NOE
        PD5/FSMC_NWE
        PD6/FSMC_nWAIT
        PD7/FSMC_NCE2
        PD11/FSMC_A16
        PD12/FSMC_A17
        PD14/FSMC_D0
        PD15/FSMC_D1

        PE7/FSMC_D4
        PE8/FSMC_D5
        PE9/FSMC_D6
        PE10/FSMC_D7

        PG6/FSMC_INT2   (本例程用查询方式判忙，此口线作为普通GPIO输入功能使用)
    */

    /* 使能 GPIO 时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

    /* 使能 FSMC 时钟 */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

    /*  配置GPIOD */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_7 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /*  配置GPIOE */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /*  配置GPIOG, PG6作为忙信息，配置为输入 */

    /* INT2 引脚配置为内部上来输入，用于忙信号 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    //FSMC_NANDDeInit(FSMC_Bank2_NAND);
    //FSMC_NANDStructInit(&FSMC_NANDInitStructure);
    /* 配置 FSMC 时序 */
    /*
        Defines the number of HCLK cycles to setup address before the command assertion for NAND-Flash
        read or write access  to common/Attribute or I/O memory space (depending on the memory space
        timing to be configured).This parameter can be a value between 0 and 0xFF.
    */
    //p.FSMC_SetupTime = 0x01;
    p.FSMC_SetupTime = 0x1;

    /*
        Defines the minimum number of HCLK cycles to assert the command for NAND-Flash read or write
        access to common/Attribute or I/O memory space (depending on the memory space timing to be
        configured). This parameter can be a number between 0x00 and 0xFF
    */
    //p.FSMC_WaitSetupTime = 0x03;
    p.FSMC_WaitSetupTime = 0x3;

    /*
        Defines the number of HCLK clock cycles to hold address (and data for write access) after the
        command deassertion for NAND-Flash read or write access to common/Attribute or I/O memory space
         (depending on the memory space timing to be configured).
        This parameter can be a number between 0x00 and 0xFF
    */
    //p.FSMC_HoldSetupTime = 0x02;
    p.FSMC_HoldSetupTime = 0x2;

    /*
        Defines the number of HCLK clock cycles during which the databus is kept in HiZ after the start
        of a NAND-Flash  write access to common/Attribute or I/O memory space (depending on the memory
        space timing to be configured). This parameter can be a number between 0x00 and 0xFF
    */
    //p.FSMC_HiZSetupTime = 0x01;
    p.FSMC_HiZSetupTime = 0x1;

    FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND;                     /* 定义FSMC NAND BANK 号 */
    FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Disable;     /* 插入等待时序使能 */
    FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;  /* 数据宽度 8bit */
    FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Enable;                      /* ECC错误检查和纠正功能使能 */
    FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes;   /* ECC 页面大小 */
    FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00;                       /* CLE低和RE低之间的延迟，HCLK周期数 */
    FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00;                        /* ALE低和RE低之间的延迟，HCLK周期数 */
    FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;               /* FSMC Common Space Timing */
    FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;            /* FSMC Attribute Space Timing */

    FSMC_NANDInit(&FSMC_NANDInitStructure);

    /* FSMC NAND Bank 使能 */
    FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
}

/*
*********************************************************************************************************
*   函 数 名: NAND_ReadID
*   功能说明: 读NAND Flash的ID。ID存储到形参指定的结构体变量中。
*   形    参:  无
*   返 回 值: 32bit的NAND Flash ID
*********************************************************************************************************
*/
uint32_t NAND_ReadID(void)
{
    uint32_t data = 0;
    /* 发送命令 Command to the command area */
    NAND_CMD_AREA = 0x90;
    NAND_ADDR_AREA = 0x00;

    /* 顺序读取NAND Flash的ID */
    data = *(__IO uint32_t *)(Bank_NAND_ADDR | DATA_AREA);
    data =  ((data << 24) & 0xFF000000) |
            ((data << 8 ) & 0x00FF0000) |
            ((data >> 8 ) & 0x0000FF00) |
            ((data >> 24) & 0x000000FF) ;
   
    return data;
}

void FMSC_NAND_ReadID( NAND_IDTypeDef * stid)
{
    /* 发送命令 Command to the command area */
    NAND_CMD_AREA = 0x90;
    NAND_ADDR_AREA = 0x00;
    stid->Maker_ID = NAND_DATA_AREA;
    stid->Device_ID = NAND_DATA_AREA;
    stid->Third_ID = NAND_DATA_AREA;
    stid->Fourth_ID = NAND_DATA_AREA;
    stid->Fifth_ID = NAND_DATA_AREA;
}

/**********************************************************************************************************
*   函 数 名: FSMC_NAND_WritePage
*   功能说明: 写一组数据至NandFlash指定页面的指定位置，写入的数据长度不大于一页的大小。
*   形    参:  - _pBuffer: 指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内地址，范围为：0-2111
*             - _usByteCount: 写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
**********************************************************************************************************/
static uint8_t FSMC_NAND_WritePage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    uint16_t i;
    NAND_ROW_ADDR row_addr;
    /* 发送页写命令 */
    NAND_CMD_AREA = NAND_CMD_WRITE0;

    /* 发送页地址 
                  Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
        第1字节： A7   A6   A5   A4   A3   A2   A1   A0     (_usPageAddr 的bit7 - bit0)
        第2字节： 0    0    0    0    A11  A10  A9   A8     (_usPageAddr 的bit11 - bit8, 高4bit必须是0)
        第3字节： B19  PLA18  P17  P16  P15  P14  P13  P12
        第4字节： B27  B26  B25  B24  B23  B22  B21  B20
        第5字节： B28
    */
    row_addr.stRowAddr.page = _ulPageNo & 0x003F;
    row_addr.stRowAddr.plane = (_ulPageNo>>16) & 0x01;
    row_addr.stRowAddr.block = (_ulPageNo>>6) & 0x03FF;
    NAND_ADDR_AREA = _usAddrInPage;
    NAND_ADDR_AREA = _usAddrInPage >> 8;
    NAND_ADDR_AREA = ADDR_1ST_CYCLE(row_addr.u32RowAddr);
    NAND_ADDR_AREA = ADDR_2ND_CYCLE(row_addr.u32RowAddr);
#if (NAND_TYPE == S34ML02G2)
    NAND_ADDR_AREA = ADDR_3RD_CYCLE(row_addr.u32RowAddr);
#endif        
    /* 写数据 */
    for(i = 0; i < _usByteCount; i++)
    {
        NAND_DATA_AREA = _pBuffer[i];
    }
    NAND_CMD_AREA = NAND_CMD_WRITE_TRUE1;

    /* 检查操作状态 */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadPage
*   功能说明: 从NandFlash指定页面的指定位置读一组数据，读出的数据长度不大于一页的大小。
*   形    参:  - _pBuffer: 指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内地址，范围为：0-2111
*             - _usByteCount: 字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadPage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    uint16_t i;
    NAND_ROW_ADDR row_addr;
    /* 发送页面读命令 */
    NAND_CMD_AREA = NAND_CMD_AREA_A;

    /* 发送页地址 
                  Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
        第1字节： A7   A6   A5   A4   A3   A2   A1   A0     (_usPageAddr 的bit7 - bit0)
        第2字节： 0    0    0    0    A11  A10  A9   A8     (_usPageAddr 的bit11 - bit8, 高4bit必须是0)
        第3字节： B19  PLA18  P17  P16  P15  P14  P13  P12
        第4字节： B27  B26  B25  B24  B23  B22  B21  B20
        第5字节： B28
    */
    row_addr.stRowAddr.page = _ulPageNo & 0x0000003F;
    row_addr.stRowAddr.plane = (_ulPageNo>>16) & 0x01;
    row_addr.stRowAddr.block = (_ulPageNo>>6) & 0x03FF;
    NAND_ADDR_AREA = _usAddrInPage;
    NAND_ADDR_AREA = _usAddrInPage >> 8;
    NAND_ADDR_AREA = ADDR_1ST_CYCLE(row_addr.u32RowAddr);
    NAND_ADDR_AREA = ADDR_2ND_CYCLE(row_addr.u32RowAddr);
#if (NAND_TYPE == S34ML02G2)
    NAND_ADDR_AREA = ADDR_3RD_CYCLE(row_addr.u32RowAddr);
#endif        
    NAND_CMD_AREA = NAND_CMD_AREA_TRUE1;

     /* 必须等待，否则读出数据异常, 此处应该判断超时 */
    for (i = 0; i < 20; i++);
    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0);

    /* 读数据到缓冲区pBuffer */
#if 1
    for(i = 0; i < _usByteCount; i++)
    {
        _pBuffer[i] = NAND_DATA_AREA;
    }
#else
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA1->IFCR = 0x000FUL;        
    DMA1_Channel1->CCR &= ~DMA_CCR1_EN;

    DMA1->IFCR = 0x000FUL;   /*Clear Flag*/
    DMA1_Channel1->CCR &= ~DMA_CCR1_EN;
    DMA1_Channel1->CNDTR = size;  /*Config LCD Data Transfer*/
    DMA1_Channel1->CPAR  = (u32)(NAND_DATA_AREA);
    DMA1_Channel1->CMAR  = (u32)(&_pBuffer[0]);
    DMA1_Channel1->CCR   = DMA_CCR2_PL | DMA_CCR1_MEM2MEM | DMA_CCR1_MINC | DMA_CCR1_EN;
    while((DMA1->ISR&DMA_ISR_TCIF1) != DMA_ISR_TCIF1);                
    DMA1->IFCR = 0x000FUL;    /*Clear Flag*/
    DMA1_Channel1->CCR &= ~DMA_CCR1_EN;
#endif
    return NAND_OK;
}
/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_EraseBlock
*   功能说明: 擦除NAND Flash一个块（block）
*   形    参:  - _ulBlockNo: 块号，范围为：0 - 2047
*   返 回 值: NAND操作状态，有如下几种值：
*             - NAND_TIMEOUT_ERROR  : 超时错误
*             - NAND_READY          : 操作成功
**********************************************************************************************************/
static uint8_t FSMC_NAND_EraseBlock(uint32_t _ulBlockNo)
{
    NAND_ROW_ADDR row_addr;
    /* 发送擦除命令 */
    /*                Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
        第1字节： A7   A6   A5   A4   A3   A2   A1   A0     (_usPageAddr 的bit7 - bit0)
        第2字节： 0    0    0    0    A11  A10  A9   A8     (_usPageAddr 的bit11 - bit8, 高4bit必须是0)
        第3字节： A19  A18  A17  A16  A15  A14  A13  A12
        第4字节： A27  A26  A25  A24  A23  A22  A21  A20
        第5字节： A28
    */
    
    NAND_CMD_AREA = NAND_CMD_ERASE0;
#if (NAND_TYPE == S34ML02G2)
    row_addr.stRowAddr.page = 0;
    row_addr.stRowAddr.plane = (_ulBlockNo>>10) & 0x01;
    row_addr.stRowAddr.block = (_ulBlockNo) & 0x03FF;
    NAND_ADDR_AREA = ADDR_1ST_CYCLE(row_addr.u32RowAddr);
    NAND_ADDR_AREA = ADDR_2ND_CYCLE(row_addr.u32RowAddr);
    NAND_ADDR_AREA = ADDR_3RD_CYCLE(row_addr.u32RowAddr);
#else
    _ulBlockNo <<= 6;   /* 块号转换为页编号 */
    NAND_ADDR_AREA = _ulBlockNo;
    NAND_ADDR_AREA = _ulBlockNo >> 8;
#endif

    NAND_CMD_AREA = NAND_CMD_ERASE1;

    return (FSMC_NAND_GetStatus());
}
/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_WriteSpare
*   功能说明: 向1个PAGE的Spare区写入数据
*   形    参:  - _pBuffer: 指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInSpare : 页内备用区的偏移地址，范围为：0-63
*             - _usByteCount: 写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_WriteSpare(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInSpare, uint16_t _usByteCount)
{
    if (_usByteCount > NAND_SPARE_AREA_SIZE)
    {
        return NAND_FAIL;
    }

    return FSMC_NAND_WritePage(_pBuffer, _ulPageNo, NAND_PAGE_SIZE + _usAddrInSpare, _usByteCount);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadSpare
*   功能说明: 读1个PAGE的Spare区的数据
*   形    参:  - _pBuffer: 指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInSpare : 页内备用区的偏移地址，范围为：0-63
*             - _usByteCount: 写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadSpare(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInSpare, uint16_t _usByteCount)
{
    if (_usByteCount > NAND_SPARE_AREA_SIZE)
    {
        return NAND_FAIL;
    }

    return FSMC_NAND_ReadPage(_pBuffer, _ulPageNo, NAND_PAGE_SIZE + _usAddrInSpare, _usByteCount);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_WriteData
*   功能说明: 向1个PAGE的主数据区写入数据
*   形    参:  - _pBuffer: 指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内数据区的偏移地址，范围为：0-2047
*             - _usByteCount: 写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_WriteData(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    if (_usByteCount > NAND_PAGE_SIZE)
    {
        return NAND_FAIL;
    }

    return FSMC_NAND_WritePage(_pBuffer, _ulPageNo, _usAddrInPage, _usByteCount);
}

/**********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadData
*   功能说明: 读1个PAGE的主数据的数据
*   形    参:  - _pBuffer: 指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内数据区的偏移地址，范围为：0-2047
*             - _usByteCount: 写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
**********************************************************************************************************/
static uint8_t FSMC_NAND_ReadData(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    if (_usByteCount > NAND_PAGE_SIZE)
    {
        return NAND_FAIL;
    }

    return FSMC_NAND_ReadPage(_pBuffer, _ulPageNo, _usAddrInPage, _usByteCount);
}



/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_Reset
*   功能说明: 复位NAND Flash
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_Reset(void)
{
    NAND_CMD_AREA = NAND_CMD_RESET;

        /* 检查操作状态 */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }

    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadStatus
*   功能说明: 使用Read statuc 命令读NAND Flash内部状态
*   形    参:  - Address: 被擦除的快内任意地址
*   返 回 值: NAND操作状态，有如下几种值：
*             - NAND_BUSY: 内部正忙
*             - NAND_READY: 内部空闲，可以进行下步操作
*             - NAND_ERROR: 先前的命令执行失败
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadStatus(void)
{
    uint8_t ucData;
    uint8_t ucStatus = NAND_BUSY;

    /* 读状态操作 */
    NAND_CMD_AREA = NAND_CMD_STATUS;
    ucData = *(__IO uint8_t *)(Bank_NAND_ADDR);

    if((ucData & NAND_ERROR) == NAND_ERROR)
    {
        ucStatus = NAND_ERROR;
    }
    else if((ucData & NAND_READY) == NAND_READY)
    {
        ucStatus = NAND_READY;
    }
    else
    {
        ucStatus = NAND_BUSY;
    }

    return (ucStatus);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_GetStatus
*   功能说明: 获取NAND Flash操作状态
*   形    参:  - Address: 被擦除的快内任意地址
*   返 回 值: NAND操作状态，有如下几种值：
*             - NAND_TIMEOUT_ERROR  : 超时错误
*             - NAND_READY          : 操作成功
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_GetStatus(void)
{
    uint32_t ulTimeout = 0x10000;
    uint8_t ucStatus = NAND_READY;

    ucStatus = FSMC_NAND_ReadStatus();

    /* 等待NAND操作结束，超时后会退出 */
    while ((ucStatus != NAND_READY) &&( ulTimeout != 0x00))
    {
        ucStatus = FSMC_NAND_ReadStatus();
        ulTimeout--;
    }

    if(ulTimeout == 0x00)
    {
        ucStatus =  NAND_TIMEOUT_ERROR;
    }

    /* 返回操作状态 */
    return (ucStatus);
}
/*
*********************************************************************************************************
*   函 数 名: bspNandInit()
*   功能说明: 初始化NAND Flash接口
*   形    参:  无
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
*********************************************************************************************************
*/
uint8_t bspNandInit(void)
{
    FSMC_NAND_Init();           /* 配置FSMC和GPIO用于NAND Flash接口 */
    FSMC_NAND_Reset();          /* 通过复位命令复位NAND Flash到读状态 */
    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_Write
*   功能说明: 写一个扇区
*   形    参:   _MemAddr : 内存单元偏移地址
*               _pReadbuff ：存放待写数据的缓冲区的指针
*               _usSize ：数据长度，必须是4字节的整数倍
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
*********************************************************************************************************
*/
int bspNandWrite(uint32_t _ulMemAddr, uint32_t *_pWriteBuf, uint32_t _usSize)
{
    uint32_t ulPhyPageNo;   /* 物理页号 */
    uint16_t usAddrInPage;  /* 页内偏移地址 */

    /* 数据长度必须是4字节整数倍 */
    if ((_usSize % 4) != 0 )
    {
        return NAND_FAIL;
    }
    
    ulPhyPageNo = _ulMemAddr / NAND_PAGE_SIZE;  /* 计算物理页号 */
    usAddrInPage = _ulMemAddr % NAND_PAGE_SIZE; /* 计算页内偏移地址 */
    if (FSMC_NAND_WriteData((uint8_t *)_pWriteBuf, ulPhyPageNo, usAddrInPage, _usSize) == NAND_FAIL)
    {
        return NAND_FAIL;
    }
    return NAND_OK; /* 写入成功 */
}

/*
*********************************************************************************************************
*   函 数 名: NAND_Read
*   功能说明: 读一个扇区
*   形    参:   _MemAddr : 内存单元偏移地址
*               _pReadbuff ：存放读出数据的缓冲区的指针
*               _usSize ：数据长度，必须是4字节的整数倍
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
**********************************************************************************************************/
int bspNandRead(uint32_t _ulMemAddr, uint32_t *_pReadBuf, uint32_t _usSize)
{
    uint32_t ulPhyPageNo;   /* 物理页号 */
    uint16_t usAddrInPage;  /* 页内偏移地址 */

    /* 数据长度必须是4字节整数倍 */
    if ((_usSize % 4) != 0)
    {
        return NAND_FAIL;
    }
    ulPhyPageNo = _ulMemAddr / NAND_PAGE_SIZE;  /* 计算物理页号 */
    usAddrInPage = _ulMemAddr % NAND_PAGE_SIZE; /* 计算页内偏移地址 */

    if (FSMC_NAND_ReadData((uint8_t *)_pReadBuf, ulPhyPageNo, usAddrInPage, _usSize) == NAND_FAIL)
    {
        return NAND_FAIL;   /* 读NAND Flash失败 */
    }

    /* 成功 */
    return NAND_OK;
}
/*
*********************************************************************************************************
*   函 数 名: bspNandBlockErase
*   功能说明: 擦除块
*   形    参:   start : 块起始地址(128K对齐)
*               end   ：块结束地址(128K对齐)
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK 表示成功
**********************************************************************************************************/
int bspNandBlockErase(uint32_t start,uint32_t end)
{
    int i = 0;
    uint32_t block_start,block_end;
    block_start = start/(NAND_BLOCK_SIZE*NAND_PAGE_SIZE);
    block_end   = end/(NAND_BLOCK_SIZE*NAND_PAGE_SIZE);
    for(i = block_start; i <= block_end; i++)
    {
        if(FSMC_NAND_EraseBlock(i) == NAND_FAIL)
        {
            return NAND_FAIL;
        }
    }
    return NAND_OK;
}
/*
*********************************************************************************************************
*   函 数 名: NAND_IsBadBlock
*   功能说明: 根据坏块标记检测NAND Flash指定的块是否坏块
*   形    参: _ulBlockNo ：块号 0 - 1023 （对于128M字节，2K Page的NAND Flash，有1024个块）
*   返 回 值: 0 ：该块可用； 1 ：该块是坏块
*********************************************************************************************************
*/
 uint8_t NAND_IsBadBlock(uint32_t _ulBlockNo)
{
    uint8_t ucFlag;

    /* 如果NAND Flash出厂前已经标注为坏块了，则就认为是坏块 */
    FSMC_NAND_ReadSpare(&ucFlag, _ulBlockNo * NAND_BLOCK_SIZE, BI_OFFSET, 1);
    if (ucFlag != 0xFF)
    {
        return 1;
    }

    FSMC_NAND_ReadSpare(&ucFlag, _ulBlockNo * NAND_BLOCK_SIZE + 1, BI_OFFSET, 1);
    if (ucFlag != 0xFF)
    {
        return 1;
    }
    return 0;   /* 是好块 */
}

/*
*********************************************************************************************************
*   函 数 名: NAND_DispBadBlockInfo
*   功能说明: 通过串口打印出NAND Flash的坏块信息
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************
*/
void NAND_DispBadBlockInfo(void)
{
    uint32_t id;
    uint32_t i;
    uint32_t n;

    FSMC_NAND_Init();   /* 初始化FSMC */

    id = NAND_ReadID();

    printf("NAND Flash ID = 0x%04X, Type = ", id);
    if (id == HY27UF081G2A)
    {
        printf("HY27UF081G2A\n  1024 Blocks, 64 pages per block, 2048 + 64 bytes per page\n");
    }
    else if (id == K9F1G08U0A)
    {
        printf("K9F1G08U0A\n  1024 Blocks, 64 pages per block, 2048 + 64 bytes per page\n");
    }
    else if (id == K9F1G08U0B)
    {
        printf("K9F1G08U0B\n  1024 Blocks, 64 pages per block, 2048 + 64 bytes per page\n");
    }
    else if (id == S34ML02G2)
    {
        printf("S34ML02G2\n  2048 Blocks, 64 pages per block, 2048 + 128 bytes per page\n");
    }
    else
    {
        printf("unkonow\r\n");
        return;
    }

    printf("Block Info : 0 is OK, * is Bad\r\n");
    n = 0;  /* 坏块统计 */
    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        if (NAND_IsBadBlock(i))
        {
            printf("*");
            n++;
        }
        else
        {
            printf("0");
        }

        if (((i + 1) % 8) == 0)
        {
            printf(" ");
        }

        if (((i + 1) % 64) == 0)
        {
            printf("\r\n");
        }
    }
    printf("Bad Block Count = %d\r\n", n);
}

#if 0 //test
void bspNandTest()
{
    int i = 0;
    char * filePath = "F:\\Gelgoog\\FPGA\\Gelgoog(FPGA)_00.01\\gelgoog_fpga_top_wrapper.bin";
    char * fpga_test = "fpga_test.bin";
    int fileSize = 0,left = 0;
    int read_len = 0,load_addr = 0;
    u8  buff[2048] = {0xFF}; 
    u8  buff_test[2048];
    FILE * pfile_read = fopen((char const*)filePath, "rb");
    FILE * pfile_write = fopen((char const*)fpga_test, "wb");
    
    if(pfile_read == NULL || pfile_write == NULL)
    {
        printf("file open failed!\n");
        return;
    }
    fseek(pfile_read, 0L, SEEK_END);    //定位到文件末尾
    fileSize = ftell(pfile_read);       //得到文件大小
    fseek(pfile_read, 0L, SEEK_SET);    //定位到文件头

    /* 写文件操作 */
    bspNandBlockErase(load_addr,fileSize);
    left = fileSize;
    while(left > 0)
    {
        read_len = (left>2048)?2048:left;
        fread(buff, 1, read_len, pfile_read); //读取一块数据

        read_len = (read_len + 3) & 0xFFFFFFFC;
        bspNandWrite(load_addr, (u32*)buff, read_len);
        load_addr += read_len;
        left -= read_len;
    }
    /* 读文件操作 */
    left = fileSize;
    load_addr = 0;
    fseek(pfile_read, 0L, SEEK_SET);    //定位到文件头
    while(left > 0)
    {
        read_len = (left>2048)?2048:left;
        fread(buff_test, 1, read_len, pfile_read); //读取一块数据
        read_len = (read_len + 3) & 0xFFFFFFFC;
        bspNandRead(load_addr,(u32*)buff, read_len);
        if(memcmp(buff,buff_test,read_len)!=0)
        {
            printf("data is not match!\n");
        }
        else
        {
            fwrite(buff,1,read_len,pfile_write);
        }
        load_addr += read_len;
        left -= read_len;
    }
    fclose(pfile_read);
    fclose(pfile_write);
}

#endif
