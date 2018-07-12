/**********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspNandFlash.c��
��������:  �ṩNAND Flash (S34ML02G2 8bit 128K�ֽ� 2048)�ĵײ�ӿں�����
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  xyzheng
�������:  2017.05.19;
��ʷ�汾:  �������⣺NANA�ܹ���2048�飬����ֻ����1024���顣ԭ���в���ȷ��������
            �����ǣ��������1024֮��Ŀ��spareû�б���������0xFF��
**********************************************************************************************/
#include "bsp.h"
#include <string.h>
#include "bspNandFlash.h"

/*
    LUT  -- Look Up Table. s_usLUT�д洢���������š��ڽ���flash�Ķ�д����ʱ��ѡ���߼���ַ������
            ��ַת�������ҵ��߼���ַ��Ӧ�������ַ��Ȼ���������ַ�Ͻ��������Ķ�д������

    �����IAR��KEIL�ı༭�����Ķ����뽫�༭������������Ϊ�����壨9��/��ţ���������TAB����Ϊ4��
    ���򣬷��򴦳��ֲ���������⡣

    �������Ƶĵط���
    ��1���ڲ���NAND Flashʱ�����������һ����ѭ�������Ӳ�������쳣���������������
        while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 )

    ��2��û������ECCУ�鹦�ܡ�ECC���Լ��1����2��bit�������ֻ��1��bit����������޸����bit�����
        ����2��bit��������ܼ�ⲻ����

    ��3������д�ļ�����ʱ���ᵼ���ؽ�LUT��Ŀǰ���ؽ�LUT�Ĵ���ִ��Ч�ʻ������ߣ��д����ơ�

    ��Ӳ��˵����
    ������STM32F103ZE-EK���������õ�NAND FlahsΪ����ʿ��HY27UF081G2A
    ��1��NAND Flash��Ƭѡ�ź����ӵ�CPU��FSMC_NCE2���������NAND Flash�ĵ�ַ�ռ�Ϊ 0x70000000����CPU������
        �ֲ��FSMC�½�)
    ��2����FSMC�������ж�������豸����TFT��SRAM��CH374T��NOR������˱���ȷ�����������豸��Ƭѡ���ڽ�ֹ
        ״̬�����򽫳������߳�ͻ���� ���μ����ļ���ʼ��FSMC GPIO�ĺ�����


    ��NAND Flash �ṹ���塿
     ��������16x4�ֽڣ�ÿpage 1024�ֽڣ�ÿ512�ֽ�һ��������ÿ��������Ӧ16���Լ��ı�������

     ÿ��PAGE���߼��ṹ��ǰ��512Bx4����������������16Bx4�Ǳ�����
    ��������������������������������������������������������������������������������������������������������������������������������
    �� Main area  ���� Main area  ���� Main area  ����Main area   ���� Spare area ���� Spare area ���� Spare area ����Spare area  ��
    ��            ����            ����            ����            ����            ����            ����            ����            ��
    ��   512B     ����    512B    ����    512B    ����    512B    ����    16B     ����     16B    ����     16B    ����    16B     ��
    ��������������������������������������������������������������������������������������������������������������������������������

     ÿ16B�ı��������߼��ṹ����:(�����Ƽ���׼��
    ������������������������������������������������������������������������������������������������������������������������������������������������������������
    ��  BI  ����RESER ����LSN0����LSN1����LSN2����RESER ����RESER ����RESER ����ECC0����ECC1����ECC2����ECC0����S-ECC1����S-ECC0����RESER ����RESER ����RESER ��
    ��      ���� VED  ����    ����    ����    ���� VED  ���� VED  ���� VED  ����    ����    ����    ����    ����      ����      ���� VED  ���� VED  ���� VED  ��
    ������������������������������������������������������������������������������������������������������������������������������������������������������������

    K9F1G08U0A �� HY27UF081G2A �Ǽ��ݵġ�оƬ����ʱ�����̱�֤оƬ�ĵ�1�����Ǻÿ顣����ǻ��飬���ڸÿ�ĵ�1��PAGE�ĵ�1���ֽ�
    ���ߵ�2��PAGE������1��PAGE�����޷����Ϊ0xFFʱ���ĵ�1���ֽ�д���0xFFֵ��������ֵ������ģ����ֱ���ж��Ƿ����0xFF���ɡ�

    ע�⣺������Щ����˵NAND Flash���̵�Ĭ�������ǽ������Ƕ��ڵ�1��PAGE�ĵ�6���ֽڴ������˵���Ǵ��󡣻������ڵ�6���ֽڽ���Բ���С������512�ֽڣ���NAND Flash
    ���������е�NAND Flash���������׼������ڸ���NAND Flashʱ������ϸ�Ķ�оƬ�������ֲᡣ


    Ϊ�˱�����NAND Flash ����ֲFat�ļ�ϵͳ�����Ƕ�16B�ı������������·��䷽��:
    ������������������������������������������������������������������������������������������������������������������������������������
    �� BI ����USED����LBN0����LBN1����ECC0����ECC1����ECC2����ECC3����ECC4����ECC5����S-ECC1����S-ECC0����RSVD����RSVD����RSVD����RSVD��
    ��    ����    ����    ����    ����    ����    ����    ����    ����    ����    ����      ����      ����    ����    ����    ����    ��
    ������������������������������������������������������������������������������������������������������������������������������������
    - BI : �����־(Bad Block Identifier)��ÿ��BLOCK�ĵ�1��PAGE���ߵ�2��PAGE�ĵ�1���ֽ�ָʾ�ÿ��Ƿ񻵿顣0xFF��ʾ�ÿ飬����0xFF��ʾ���顣
    - USED : �ÿ�ʹ�ñ�־��0xFF��ʾ���п飻0xF0��ʾ���ÿ顣
    - LBN0 LBN1 : �߼����(Logic Block No) ����0��ʼ���롣ֻ��ÿ��BLOCK�ĵ�1��PAGE��Ч������PAGE���ֶι̶�Ϊ0xFF FF
    - ECC0 ~ ECC6 : 512B����������ECCУ�� �����������ṩECC�㷨��256�ֽڶ�Ӧ3���ֽڵ�ECC)
    - S-ECC1 S-ECC0 : LSN0��LSN2��ECCУ��
    - RSVD : �����ֽڣ�Reserved
     

    xyzheng
    1. δ��NANDFLASH���������
    2. ��ֻ֧�ֿ�д��,��ҳ��ȡ,��֧��ҳд��(��Ϊ������Ҫ�������).
    3. δ��ECCУ��
*/

/* ����NAND Flash�������ַ���������Ӳ�������� */
#define Bank2_NAND_ADDR    ((uint32_t)0x70000000)
#define Bank_NAND_ADDR     Bank2_NAND_ADDR

/* �������NAND Flash�õ�3���� */
#define NAND_CMD_AREA       *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA)
#define NAND_ADDR_AREA      *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA)
#define NAND_DATA_AREA      *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA)

static uint8_t FSMC_NAND_GetStatus(void);
uint8_t NAND_WriteToNewBlock(uint32_t _ulPhyPageNo, uint8_t *_pWriteBuf, uint16_t _usOffset, uint16_t _usSize);

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_Init
*   ����˵��: ����FSMC��GPIO����NAND Flash�ӿڡ�������������ڶ�дnand flashǰ������һ�Ρ�
*   ��    ��:  ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void FSMC_NAND_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    FSMC_NANDInitTypeDef  FSMC_NANDInitStructure;
    FSMC_NAND_PCCARDTimingInitTypeDef  p;


    /*--NAND Flash GPIOs ����  ------
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

        PG6/FSMC_INT2   (�������ò�ѯ��ʽ��æ���˿�����Ϊ��ͨGPIO���빦��ʹ��)
    */

    /* ʹ�� GPIO ʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

    /* ʹ�� FSMC ʱ�� */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

    /*  ����GPIOD */
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

    /*  ����GPIOE */
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

    /*  ����GPIOG, PG6��Ϊæ��Ϣ������Ϊ���� */

    /* INT2 ��������Ϊ�ڲ��������룬����æ�ź� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    //FSMC_NANDDeInit(FSMC_Bank2_NAND);
    //FSMC_NANDStructInit(&FSMC_NANDInitStructure);
    /* ���� FSMC ʱ�� */
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

    FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND;                     /* ����FSMC NAND BANK �� */
    FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Disable;     /* ����ȴ�ʱ��ʹ�� */
    FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;  /* ���ݿ�� 8bit */
    FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Enable;                      /* ECC������;�������ʹ�� */
    FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes;   /* ECC ҳ���С */
    FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00;                       /* CLE�ͺ�RE��֮����ӳ٣�HCLK������ */
    FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00;                        /* ALE�ͺ�RE��֮����ӳ٣�HCLK������ */
    FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;               /* FSMC Common Space Timing */
    FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;            /* FSMC Attribute Space Timing */

    FSMC_NANDInit(&FSMC_NANDInitStructure);

    /* FSMC NAND Bank ʹ�� */
    FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_ReadID
*   ����˵��: ��NAND Flash��ID��ID�洢���β�ָ���Ľṹ������С�
*   ��    ��:  ��
*   �� �� ֵ: 32bit��NAND Flash ID
*********************************************************************************************************
*/
uint32_t NAND_ReadID(void)
{
    uint32_t data = 0;
    /* �������� Command to the command area */
    NAND_CMD_AREA = 0x90;
    NAND_ADDR_AREA = 0x00;

    /* ˳���ȡNAND Flash��ID */
    data = *(__IO uint32_t *)(Bank_NAND_ADDR | DATA_AREA);
    data =  ((data << 24) & 0xFF000000) |
            ((data << 8 ) & 0x00FF0000) |
            ((data >> 8 ) & 0x0000FF00) |
            ((data >> 24) & 0x000000FF) ;
   
    return data;
}

void FMSC_NAND_ReadID( NAND_IDTypeDef * stid)
{
    /* �������� Command to the command area */
    NAND_CMD_AREA = 0x90;
    NAND_ADDR_AREA = 0x00;
    stid->Maker_ID = NAND_DATA_AREA;
    stid->Device_ID = NAND_DATA_AREA;
    stid->Third_ID = NAND_DATA_AREA;
    stid->Fourth_ID = NAND_DATA_AREA;
    stid->Fifth_ID = NAND_DATA_AREA;
}

/**********************************************************************************************************
*   �� �� ��: FSMC_NAND_WritePage
*   ����˵��: дһ��������NandFlashָ��ҳ���ָ��λ�ã�д������ݳ��Ȳ�����һҳ�Ĵ�С��
*   ��    ��:  - _pBuffer: ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ�ڵ�ַ����ΧΪ��0-2111
*             - _usByteCount: д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
**********************************************************************************************************/
static uint8_t FSMC_NAND_WritePage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    uint16_t i;
    NAND_ROW_ADDR row_addr;
    /* ����ҳд���� */
    NAND_CMD_AREA = NAND_CMD_WRITE0;

    /* ����ҳ��ַ 
                  Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
        ��1�ֽڣ� A7   A6   A5   A4   A3   A2   A1   A0     (_usPageAddr ��bit7 - bit0)
        ��2�ֽڣ� 0    0    0    0    A11  A10  A9   A8     (_usPageAddr ��bit11 - bit8, ��4bit������0)
        ��3�ֽڣ� B19  PLA18  P17  P16  P15  P14  P13  P12
        ��4�ֽڣ� B27  B26  B25  B24  B23  B22  B21  B20
        ��5�ֽڣ� B28
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
    /* д���� */
    for(i = 0; i < _usByteCount; i++)
    {
        NAND_DATA_AREA = _pBuffer[i];
    }
    NAND_CMD_AREA = NAND_CMD_WRITE_TRUE1;

    /* ������״̬ */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_ReadPage
*   ����˵��: ��NandFlashָ��ҳ���ָ��λ�ö�һ�����ݣ����������ݳ��Ȳ�����һҳ�Ĵ�С��
*   ��    ��:  - _pBuffer: ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ�ڵ�ַ����ΧΪ��0-2111
*             - _usByteCount: �ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadPage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    uint16_t i;
    NAND_ROW_ADDR row_addr;
    /* ����ҳ������� */
    NAND_CMD_AREA = NAND_CMD_AREA_A;

    /* ����ҳ��ַ 
                  Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
        ��1�ֽڣ� A7   A6   A5   A4   A3   A2   A1   A0     (_usPageAddr ��bit7 - bit0)
        ��2�ֽڣ� 0    0    0    0    A11  A10  A9   A8     (_usPageAddr ��bit11 - bit8, ��4bit������0)
        ��3�ֽڣ� B19  PLA18  P17  P16  P15  P14  P13  P12
        ��4�ֽڣ� B27  B26  B25  B24  B23  B22  B21  B20
        ��5�ֽڣ� B28
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

     /* ����ȴ���������������쳣, �˴�Ӧ���жϳ�ʱ */
    for (i = 0; i < 20; i++);
    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0);

    /* �����ݵ�������pBuffer */
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
*   �� �� ��: FSMC_NAND_EraseBlock
*   ����˵��: ����NAND Flashһ���飨block��
*   ��    ��:  - _ulBlockNo: ��ţ���ΧΪ��0 - 2047
*   �� �� ֵ: NAND����״̬�������¼���ֵ��
*             - NAND_TIMEOUT_ERROR  : ��ʱ����
*             - NAND_READY          : �����ɹ�
**********************************************************************************************************/
static uint8_t FSMC_NAND_EraseBlock(uint32_t _ulBlockNo)
{
    NAND_ROW_ADDR row_addr;
    /* ���Ͳ������� */
    /*                Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
        ��1�ֽڣ� A7   A6   A5   A4   A3   A2   A1   A0     (_usPageAddr ��bit7 - bit0)
        ��2�ֽڣ� 0    0    0    0    A11  A10  A9   A8     (_usPageAddr ��bit11 - bit8, ��4bit������0)
        ��3�ֽڣ� A19  A18  A17  A16  A15  A14  A13  A12
        ��4�ֽڣ� A27  A26  A25  A24  A23  A22  A21  A20
        ��5�ֽڣ� A28
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
    _ulBlockNo <<= 6;   /* ���ת��Ϊҳ��� */
    NAND_ADDR_AREA = _ulBlockNo;
    NAND_ADDR_AREA = _ulBlockNo >> 8;
#endif

    NAND_CMD_AREA = NAND_CMD_ERASE1;

    return (FSMC_NAND_GetStatus());
}
/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_WriteSpare
*   ����˵��: ��1��PAGE��Spare��д������
*   ��    ��:  - _pBuffer: ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInSpare : ҳ�ڱ�������ƫ�Ƶ�ַ����ΧΪ��0-63
*             - _usByteCount: д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
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
*   �� �� ��: FSMC_NAND_ReadSpare
*   ����˵��: ��1��PAGE��Spare��������
*   ��    ��:  - _pBuffer: ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInSpare : ҳ�ڱ�������ƫ�Ƶ�ַ����ΧΪ��0-63
*             - _usByteCount: д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
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
*   �� �� ��: FSMC_NAND_WriteData
*   ����˵��: ��1��PAGE����������д������
*   ��    ��:  - _pBuffer: ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ����������ƫ�Ƶ�ַ����ΧΪ��0-2047
*             - _usByteCount: д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
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
*   �� �� ��: FSMC_NAND_ReadData
*   ����˵��: ��1��PAGE�������ݵ�����
*   ��    ��:  - _pBuffer: ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ����������ƫ�Ƶ�ַ����ΧΪ��0-2047
*             - _usByteCount: д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
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
*   �� �� ��: FSMC_NAND_Reset
*   ����˵��: ��λNAND Flash
*   ��    ��:  ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_Reset(void)
{
    NAND_CMD_AREA = NAND_CMD_RESET;

        /* ������״̬ */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }

    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_ReadStatus
*   ����˵��: ʹ��Read statuc �����NAND Flash�ڲ�״̬
*   ��    ��:  - Address: �������Ŀ��������ַ
*   �� �� ֵ: NAND����״̬�������¼���ֵ��
*             - NAND_BUSY: �ڲ���æ
*             - NAND_READY: �ڲ����У����Խ����²�����
*             - NAND_ERROR: ��ǰ������ִ��ʧ��
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadStatus(void)
{
    uint8_t ucData;
    uint8_t ucStatus = NAND_BUSY;

    /* ��״̬���� */
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
*   �� �� ��: FSMC_NAND_GetStatus
*   ����˵��: ��ȡNAND Flash����״̬
*   ��    ��:  - Address: �������Ŀ��������ַ
*   �� �� ֵ: NAND����״̬�������¼���ֵ��
*             - NAND_TIMEOUT_ERROR  : ��ʱ����
*             - NAND_READY          : �����ɹ�
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_GetStatus(void)
{
    uint32_t ulTimeout = 0x10000;
    uint8_t ucStatus = NAND_READY;

    ucStatus = FSMC_NAND_ReadStatus();

    /* �ȴ�NAND������������ʱ����˳� */
    while ((ucStatus != NAND_READY) &&( ulTimeout != 0x00))
    {
        ucStatus = FSMC_NAND_ReadStatus();
        ulTimeout--;
    }

    if(ulTimeout == 0x00)
    {
        ucStatus =  NAND_TIMEOUT_ERROR;
    }

    /* ���ز���״̬ */
    return (ucStatus);
}
/*
*********************************************************************************************************
*   �� �� ��: bspNandInit()
*   ����˵��: ��ʼ��NAND Flash�ӿ�
*   ��    ��:  ��
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t bspNandInit(void)
{
    FSMC_NAND_Init();           /* ����FSMC��GPIO����NAND Flash�ӿ� */
    FSMC_NAND_Reset();          /* ͨ����λ���λNAND Flash����״̬ */
    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_Write
*   ����˵��: дһ������
*   ��    ��:   _MemAddr : �ڴ浥Ԫƫ�Ƶ�ַ
*               _pReadbuff ����Ŵ�д���ݵĻ�������ָ��
*               _usSize �����ݳ��ȣ�������4�ֽڵ�������
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
*********************************************************************************************************
*/
int bspNandWrite(uint32_t _ulMemAddr, uint32_t *_pWriteBuf, uint32_t _usSize)
{
    uint32_t ulPhyPageNo;   /* ����ҳ�� */
    uint16_t usAddrInPage;  /* ҳ��ƫ�Ƶ�ַ */

    /* ���ݳ��ȱ�����4�ֽ������� */
    if ((_usSize % 4) != 0 )
    {
        return NAND_FAIL;
    }
    
    ulPhyPageNo = _ulMemAddr / NAND_PAGE_SIZE;  /* ��������ҳ�� */
    usAddrInPage = _ulMemAddr % NAND_PAGE_SIZE; /* ����ҳ��ƫ�Ƶ�ַ */
    if (FSMC_NAND_WriteData((uint8_t *)_pWriteBuf, ulPhyPageNo, usAddrInPage, _usSize) == NAND_FAIL)
    {
        return NAND_FAIL;
    }
    return NAND_OK; /* д��ɹ� */
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_Read
*   ����˵��: ��һ������
*   ��    ��:   _MemAddr : �ڴ浥Ԫƫ�Ƶ�ַ
*               _pReadbuff ����Ŷ������ݵĻ�������ָ��
*               _usSize �����ݳ��ȣ�������4�ֽڵ�������
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
**********************************************************************************************************/
int bspNandRead(uint32_t _ulMemAddr, uint32_t *_pReadBuf, uint32_t _usSize)
{
    uint32_t ulPhyPageNo;   /* ����ҳ�� */
    uint16_t usAddrInPage;  /* ҳ��ƫ�Ƶ�ַ */

    /* ���ݳ��ȱ�����4�ֽ������� */
    if ((_usSize % 4) != 0)
    {
        return NAND_FAIL;
    }
    ulPhyPageNo = _ulMemAddr / NAND_PAGE_SIZE;  /* ��������ҳ�� */
    usAddrInPage = _ulMemAddr % NAND_PAGE_SIZE; /* ����ҳ��ƫ�Ƶ�ַ */

    if (FSMC_NAND_ReadData((uint8_t *)_pReadBuf, ulPhyPageNo, usAddrInPage, _usSize) == NAND_FAIL)
    {
        return NAND_FAIL;   /* ��NAND Flashʧ�� */
    }

    /* �ɹ� */
    return NAND_OK;
}
/*
*********************************************************************************************************
*   �� �� ��: bspNandBlockErase
*   ����˵��: ������
*   ��    ��:   start : ����ʼ��ַ(128K����)
*               end   ���������ַ(128K����)
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK ��ʾ�ɹ�
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
*   �� �� ��: NAND_IsBadBlock
*   ����˵��: ���ݻ����Ǽ��NAND Flashָ���Ŀ��Ƿ񻵿�
*   ��    ��: _ulBlockNo ����� 0 - 1023 ������128M�ֽڣ�2K Page��NAND Flash����1024���飩
*   �� �� ֵ: 0 ���ÿ���ã� 1 ���ÿ��ǻ���
*********************************************************************************************************
*/
 uint8_t NAND_IsBadBlock(uint32_t _ulBlockNo)
{
    uint8_t ucFlag;

    /* ���NAND Flash����ǰ�Ѿ���עΪ�����ˣ������Ϊ�ǻ��� */
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
    return 0;   /* �Ǻÿ� */
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_DispBadBlockInfo
*   ����˵��: ͨ�����ڴ�ӡ��NAND Flash�Ļ�����Ϣ
*   ��    ��:  ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void NAND_DispBadBlockInfo(void)
{
    uint32_t id;
    uint32_t i;
    uint32_t n;

    FSMC_NAND_Init();   /* ��ʼ��FSMC */

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
    n = 0;  /* ����ͳ�� */
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
    fseek(pfile_read, 0L, SEEK_END);    //��λ���ļ�ĩβ
    fileSize = ftell(pfile_read);       //�õ��ļ���С
    fseek(pfile_read, 0L, SEEK_SET);    //��λ���ļ�ͷ

    /* д�ļ����� */
    bspNandBlockErase(load_addr,fileSize);
    left = fileSize;
    while(left > 0)
    {
        read_len = (left>2048)?2048:left;
        fread(buff, 1, read_len, pfile_read); //��ȡһ������

        read_len = (read_len + 3) & 0xFFFFFFFC;
        bspNandWrite(load_addr, (u32*)buff, read_len);
        load_addr += read_len;
        left -= read_len;
    }
    /* ���ļ����� */
    left = fileSize;
    load_addr = 0;
    fseek(pfile_read, 0L, SEEK_SET);    //��λ���ļ�ͷ
    while(left > 0)
    {
        read_len = (left>2048)?2048:left;
        fread(buff_test, 1, read_len, pfile_read); //��ȡһ������
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
