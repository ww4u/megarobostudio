/**********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspSdio.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.18;
��ʷ�汾:  ��;
**********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "bspSdio.h"
#include "bspTimer.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    SDIO_CLK_24MHZ         1    //ʹ��24Mhzʱ�ӣ�8bit, ����ʹ��1MHzʱ�ӽ��е���

#if SDIO_CLK_24MHZ
#define    SDIO_CLK_FREQ          24    //ʹ��24Mhzʱ��
#else
#define    SDIO_CLK_FREQ          1     //ʹ��1Mhzʱ��
#endif

#define    SDIO_CRC_LEN           17

#define    SDIO_FIFO_ADDR         (uint32_t)&SDIO->FIFO    //(uint32_t)0x40012C80
#define    SDIO_CLK               RCC_APB2Periph_SDIO
#define    SDIO_GPIO_CLK          RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB
#define    SDIO_AF_PORT           GPIO_AF_SDIO

#define    SDIO_CMD_GPIO_PORT     GPIOD
#define    SDIO_CMD_PIN           GPIO_Pin_2
#define    SDIO_CMD_SOURCE        GPIO_PinSource2

#define    SDIO_CLK_GPIO_PORT     GPIOC
#define    SDIO_CLK_PIN           GPIO_Pin_12
#define    SDIO_CLK_SOURCE        GPIO_PinSource12

#define    SDIO_DATA45_GPIO_PORT  GPIOB
#define    SDIO_DATA4_PIN         GPIO_Pin_8
#define    SDIO_DATA4_SOURCE      GPIO_PinSource8
#define    SDIO_DATA5_PIN         GPIO_Pin_9
#define    SDIO_DATA5_SOURCE      GPIO_PinSource9

#define    SDIO_DATA_GPIO_PORT    GPIOC
#define    SDIO_DATA0_PIN         GPIO_Pin_8
#define    SDIO_DATA0_SOURCE      GPIO_PinSource8
#define    SDIO_DATA1_PIN         GPIO_Pin_9
#define    SDIO_DATA1_SOURCE      GPIO_PinSource9
#define    SDIO_DATA2_PIN         GPIO_Pin_10
#define    SDIO_DATA2_SOURCE      GPIO_PinSource10
#define    SDIO_DATA3_PIN         GPIO_Pin_11
#define    SDIO_DATA3_SOURCE      GPIO_PinSource11
#define    SDIO_DATA6_PIN         GPIO_Pin_6
#define    SDIO_DATA6_SOURCE      GPIO_PinSource6
#define    SDIO_DATA7_PIN         GPIO_Pin_7
#define    SDIO_DATA7_SOURCE      GPIO_PinSource7

#define    SDIO_DMA_CLK           RCC_AHB1Periph_DMA2
#define    SDIO_DMA_IRQ           DMA2_Stream3_IRQn

#define    SDIO_DMA_STREAM        DMA2_Stream3
#define    SDIO_DMA_CHANNEL       DMA_Channel_4
#define    SDIO_DMA_FLAG_FEIF     DMA_FLAG_FEIF3
#define    SDIO_DMA_FLAG_DMEIF    DMA_FLAG_DMEIF3
#define    SDIO_DMA_FLAG_TEIF     DMA_FLAG_TEIF3
#define    SDIO_DMA_FLAG_HTIF     DMA_FLAG_HTIF3
#define    SDIO_DMA_FLAG_TCIF     DMA_FLAG_TCIF3 

#define    SDIO_DATA_TIMEOUT      33600    //����DELAY_COUNT_MS = 33600���˳�ʱʱ���ԼΪ1ms
#define    SDIO_STATIC_FLAGS      ((uint32_t)0x000005FF)
#define    SDIO_DATA_LEN_MASK     0x01FFFFFF
#define    SDIO_DCTRL_CLEAR_MASK  0xFFFFFF08

#define    SDIO_CMD_INDEX         0x3C

#define    SDIO_SEND_BYTES_OFFSET_ENBALE    1
#define    SDIO_SEND_BYTES_OFFSET           12    //����DMA����ʱ��������ݳ��ȴ���8���ֽ������ٷ�12���ֽڣ���������Ϊ�Ĳ�12���ֽ�    CJ 2017.11.27 Add
#define    SDIO_SEND_BYTES_THRESHOLD        8



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
u8 g_sdioDmaTxComplete = 0;



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: bspSdioInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSdioInit(void)
{
    SDIO_InitTypeDef SDIO_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;


    /***************************************SDIO GPIO����******************************************/
    RCC_AHB1PeriphClockCmd(SDIO_GPIO_CLK, ENABLE);
    
    GPIO_PinAFConfig(SDIO_DATA_GPIO_PORT,   SDIO_DATA0_SOURCE,  SDIO_AF_PORT);    
    GPIO_PinAFConfig(SDIO_DATA_GPIO_PORT,   SDIO_DATA1_SOURCE,  SDIO_AF_PORT);      
    GPIO_PinAFConfig(SDIO_DATA_GPIO_PORT,   SDIO_DATA2_SOURCE,  SDIO_AF_PORT); 
    GPIO_PinAFConfig(SDIO_DATA_GPIO_PORT,   SDIO_DATA3_SOURCE,  SDIO_AF_PORT);    
    GPIO_PinAFConfig(SDIO_DATA_GPIO_PORT,   SDIO_DATA6_SOURCE,  SDIO_AF_PORT); 
    GPIO_PinAFConfig(SDIO_DATA_GPIO_PORT,   SDIO_DATA7_SOURCE, SDIO_AF_PORT);
    GPIO_PinAFConfig(SDIO_DATA45_GPIO_PORT, SDIO_DATA4_SOURCE, SDIO_AF_PORT); 
    GPIO_PinAFConfig(SDIO_DATA45_GPIO_PORT, SDIO_DATA5_SOURCE, SDIO_AF_PORT);
    
    GPIO_PinAFConfig(SDIO_CLK_GPIO_PORT, SDIO_CLK_SOURCE, SDIO_AF_PORT);
    GPIO_PinAFConfig(SDIO_CMD_GPIO_PORT, SDIO_CMD_SOURCE, SDIO_AF_PORT);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        
    GPIO_InitStructure.GPIO_Pin = SDIO_DATA0_PIN | SDIO_DATA1_PIN |
                                  SDIO_DATA2_PIN | SDIO_DATA3_PIN | 
                                  SDIO_DATA6_PIN | SDIO_DATA7_PIN;
    GPIO_Init(SDIO_DATA_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SDIO_DATA4_PIN | SDIO_DATA5_PIN;
    GPIO_Init(SDIO_DATA45_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SDIO_CLK_PIN;
    GPIO_Init(SDIO_CLK_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SDIO_CMD_PIN;
    GPIO_Init(SDIO_CMD_GPIO_PORT, &GPIO_InitStructure);


    /***************************************SDIO ����******************************************/
    RCC_APB2PeriphClockCmd(SDIO_CLK, ENABLE);
    
    SDIO_DeInit();
    SDIO_StructInit(&SDIO_InitStructure);
#if SDIO_CLK_24MHZ
    SDIO_InitStructure.SDIO_ClockDiv            = SDIO_CLK_DIV_24MHz;
#else
    SDIO_InitStructure.SDIO_ClockDiv            = SDIO_CLK_DIV_1MHz;
#endif
    SDIO_InitStructure.SDIO_BusWide             = SDIO_BusWide_8b;                     //ʹ��8λ��������ģʽ
    SDIO_InitStructure.SDIO_ClockBypass         = SDIO_ClockBypass_Disable;            //�ر���·�����������·���ʹ��ϵͳʱ��                   
    SDIO_InitStructure.SDIO_ClockEdge           = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockPowerSave      = SDIO_ClockPowerSave_Disable;         //����ʡ��ģʽ�����߿���ʱ�ر�sdclkʱ�ӣ��߼�������ʱ�ӹر�
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;    //Ӳ�����ؿ�����,�ڷ��Ͷ�����ʱ�ᷢ���������,������ʱû������,���Ա���ر�
    SDIO_Init(&SDIO_InitStructure);

    SDIO_SetPowerState(SDIO_PowerState_ON);    //SDIO����������
    SDIO_ClockCmd(ENABLE);                     //SDIOʱ��ʹ��


    /***************************************SDIO DMA����******************************************/
    RCC_AHB1PeriphClockCmd(SDIO_DMA_CLK, ENABLE);

    DMA_Cmd(SDIO_DMA_STREAM, DISABLE);
    DMA_DeInit(SDIO_DMA_STREAM);
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;    //�ڴ��ַ���ֽڳ��Ⱥʹ��䷽��ʹ�õ�ʱ����д
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_PeripheralBaseAddr = SDIO_FIFO_ADDR;
    DMA_InitStructure.DMA_Channel            = SDIO_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode            = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority        = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(SDIO_DMA_STREAM, &DMA_InitStructure);
    DMA_ClearFlag(SDIO_DMA_STREAM, SDIO_DMA_FLAG_FEIF  | 
                                   SDIO_DMA_FLAG_DMEIF | 
                                   SDIO_DMA_FLAG_TEIF  | 
                                   SDIO_DMA_FLAG_HTIF  | 
                                   SDIO_DMA_FLAG_TCIF);
    DMA_ITConfig(SDIO_DMA_STREAM, DMA_IT_TC, ENABLE);
    DMA_FlowControllerConfig(SDIO_DMA_STREAM, DMA_FlowCtrl_Peripheral);
    DMA_Cmd(SDIO_DMA_STREAM, ENABLE);

    /***************************************SDIO �ж�����******************************************/
    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = SDIO_DMA_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}


/*********************************************************************************************
�� �� ��: bspSdioDataSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 bspSdioDataSend(u8 *dataBuff, u32 dataLen, u16 dataBlock)
{
    u32 tmpReg;
    u32 delaySeconds;

    
    SDIO->DCTRL = 0x0;    //�رմ���
    
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);

    DMA_ClearFlag(SDIO_DMA_STREAM, SDIO_DMA_FLAG_FEIF  | 
                                   SDIO_DMA_FLAG_DMEIF | 
                                   SDIO_DMA_FLAG_TEIF  | 
                                   SDIO_DMA_FLAG_HTIF  | 
                                   SDIO_DMA_FLAG_TCIF);

    //ʧ��DMA
    SDIO_DMA_STREAM->CR &= (uint32_t)(~DMA_SxCR_EN);
    
    g_sdioDmaTxComplete = 0;

    //����DMA���䷽�򡢳��Ⱥ��ڴ��ַ
    tmpReg = SDIO_DMA_STREAM->CR;
    tmpReg &= (uint32_t)~DMA_SxCR_DIR;       //���DMA���䷽��
    tmpReg |= DMA_DIR_MemoryToPeripheral;    //��������Ϊ�ڴ浽����
    SDIO_DMA_STREAM->CR   = tmpReg;
    SDIO_DMA_STREAM->M0AR = (uint32_t)dataBuff;
    SDIO_DMA_STREAM->NDTR = dataLen;    

    //ʹ��DMA
    SDIO_DMA_STREAM->CR  |= (uint32_t)DMA_SxCR_EN;
    SDIO_DMACmd(ENABLE);

#if SDIO_SEND_BYTES_OFFSET_ENBALE
    //CJ 2017.11.27 Modify
    if (dataLen <= SDIO_SEND_BYTES_THRESHOLD)
    {
        SDIO->DLEN   = dataLen & SDIO_DATA_LEN_MASK;
    }
    else
    {
        SDIO->DLEN   = (dataLen + SDIO_SEND_BYTES_OFFSET) & SDIO_DATA_LEN_MASK;
    }
    
#else

    SDIO->DLEN   = dataLen & SDIO_DATA_LEN_MASK;
#endif

    SDIO->DTIMER = SDIO_DATA_TIMEOUT;
    tmpReg = SDIO->DCTRL;
    tmpReg &= SDIO_DCTRL_CLEAR_MASK;    //Clear DEN, DTMODE, DTDIR and DBCKSIZE bits
    tmpReg |= dataBlock | SDIO_TransferDir_ToCard | SDIO_TransferMode_Block | SDIO_DPSM_Enable;
    SDIO->DCTRL = tmpReg;

#if 1    //ʹ��״̬λ�жϵķ�ʽ����������ʱ
    //
    delaySeconds = SDIO_DATA_TIMEOUT;

    //while ((!g_sdioDmaTxComplete) && (delaySeconds > 0))    //Ҳ�����ñ��������ж�
    while ((SDIO_FLAG_TXACT == (SDIO->STA & SDIO_FLAG_TXACT)) && (delaySeconds > 0))
    {
        delaySeconds--;  
    }

    if (0 == delaySeconds)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
#else

    //Ŀǰû���ҵ���Ч���ж����ݴ�����ɵı�ʶ������ʹ����ʱ�ķ�ʽ��֤���ݴ������
    //������ʱʱ��: 1Mʱ���£�8bit����ģʽ����һ���ֽ���Ҫʱ��Ϊ1us��������ʱʱ������ֽ�������ʱ��
    delaySeconds = (u32)(((float)dataLen + SDIO_CRC_LEN) / SDIO_CLK_FREQ + 1);    //����ȡ��
    bspDelayUs(delaySeconds);    //�ȴ����ݴ������

    return 0;
#endif
}


/*********************************************************************************************
�� �� ��: bspSdioDataReceiveSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��������ʱ���������DMA����Ҫʹ��bspSdioDataRead��ȡ���ݣ���������ʱ������;
*********************************************************************************************/
void bspSdioDataReceiveSet(u8 *dataBuff, u16 dataLen, u8 dataBlock)
{
    u32 tmpReg;

  
    SDIO->DCTRL = 0x0;    //�رմ���
    
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);

    g_sdioDmaTxComplete = 0;
    
    SDIO_DMACmd(ENABLE);
    
    DMA_ClearFlag(SDIO_DMA_STREAM, SDIO_DMA_FLAG_FEIF  | 
                                   SDIO_DMA_FLAG_DMEIF | 
                                   SDIO_DMA_FLAG_TEIF  | 
                                   SDIO_DMA_FLAG_HTIF  | 
                                   SDIO_DMA_FLAG_TCIF);

    //���뷽�򣬽��յ�ַ�ͳ��ȣ�ʹ��DMA
    SDIO_DMA_STREAM->CR  &= ~(uint32_t)DMA_SxCR_EN;
    tmpReg = SDIO_DMA_STREAM->CR;
    tmpReg &= (uint32_t)~DMA_SxCR_DIR;
    tmpReg |= DMA_DIR_PeripheralToMemory;
    SDIO_DMA_STREAM->CR   = tmpReg;
    SDIO_DMA_STREAM->NDTR = dataLen;
    SDIO_DMA_STREAM->M0AR = (uint32_t)dataBuff;
    SDIO_DMA_STREAM->CR  |= (uint32_t)DMA_SxCR_EN;

#if SDIO_SEND_BYTES_OFFSET_ENBALE
    //����Ҳ������12���ֽڵ�����
    if (dataLen <= SDIO_SEND_BYTES_THRESHOLD)
    {
        SDIO->DLEN = dataLen & SDIO_DATA_LEN_MASK;
    }
    else
    {
        SDIO->DLEN = (dataLen + SDIO_SEND_BYTES_OFFSET) & SDIO_DATA_LEN_MASK;
    }

#else
    SDIO->DLEN = dataLen & SDIO_DATA_LEN_MASK;
#endif

    //SDIO->DLEN   = dataLen & SDIO_DATA_LEN_MASK;
    SDIO->DTIMER = SDIO_DATA_TIMEOUT;
    tmpReg = SDIO->DCTRL;    
    tmpReg &= SDIO_DCTRL_CLEAR_MASK;    //Clear DEN, DTMODE, DTDIR and DBCKSIZE bits
    tmpReg |= dataBlock | SDIO_TransferDir_ToSDIO | SDIO_TransferMode_Stream | SDIO_DPSM_Enable;
    SDIO->DCTRL = tmpReg;
}


/*********************************************************************************************
�� �� ��: bspSdioDataTxComplete;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8  bspSdioDataTxComplete(void)
{
    return g_sdioDmaTxComplete;
}


/*********************************************************************************************
�� �� ��: bspSdioCmdSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 bspSdioCmdSend(u16 addr, u16 length, unsigned char needDelay)
{   
    u32 timeout;
    u32 argument = 0;
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;

    
    argument = ((addr & 0x3F) << 26) | (length & 0xFFFF);
    SDIO_CmdInitStructure.SDIO_Argument = argument;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_CMD_INDEX; 
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_No;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    timeout = SDIO_DATA_TIMEOUT;

    while (!(SDIO->STA & SDIO_FLAG_CMDSENT) && (timeout > 0))
    {
        timeout--;  
    }
    
    SDIO_ClearFlag(SDIO_FLAG_CMDSENT);

    if (0 == timeout)
    {
        return 1;    //���ͳ���
    }

    if (needDelay)    //��ȡ����ʱ����Ҫ��ʱ�ȴ����ݷ������
    {
        //�ȴ�FPGA����������ϣ����ȹ̶�Ϊ4 + SDIO_CRC_LEN = 21���ֽڣ�ʱ��(21/24(24MHzʱ��))+2 = 2us
        bspDelayUs(21 / SDIO_CLK_FREQ + 2);  
    }

    return 0;
}



/*******************************************�ļ�β********************************************/
