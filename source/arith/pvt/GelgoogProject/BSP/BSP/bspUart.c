/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspUart.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.08.30;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "bspUart.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    UART_CI_UART              USART1

#define    UART_CI_UART_CLK          RCC_APB2Periph_USART1
#define    UART_CI_UART_DR_ADDR      (uint32_t)&USART1->DR
#define    UART_CI_UART_RX_IRQn      USART1_IRQn

#define    UART_CI_UART_RX_PIN       GPIO_Pin_10
#define    UART_CI_UART_TX_PIN       GPIO_Pin_9
#define    UART_CI_UART_GPIO_PORT    GPIOA
#define    UART_CI_UART_GPIO_CLK     RCC_AHB1Periph_GPIOA
#define    UART_CI_UART_AF_PORT      GPIO_AF_USART1
#define    UART_CI_UART_RX_SOURCE    GPIO_PinSource10
#define    UART_CI_UART_TX_SOURCE    GPIO_PinSource9 

#define    UART_CI_UART_RX_DMA_CLK        RCC_AHB1Periph_DMA2
#define    UART_CI_UART_RX_DMA_STREAM     DMA2_Stream2
#define    UART_CI_UART_RX_DMA_CHANNEL    DMA_Channel_4
#define    UART_CI_UART_RX_DMA_IRQn       DMA2_Stream2_IRQn
#define    UART_CI_UART_RX_DMA_FLAG       DMA_FLAG_FEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_TCIF2


#define    UART_SN1_UART              USART3

#define    UART_SN1_UART_CLK          RCC_APB1Periph_USART3
#define    UART_SN1_UART_DR_ADDR      (uint32_t)&USART3->DR
#define    UART_SN1_UART_RX_IRQn      USART3_IRQn

#define    UART_SN1_UART_RX_PIN       GPIO_Pin_9
#define    UART_SN1_UART_TX_PIN       GPIO_Pin_8
#define    UART_SN1_UART_GPIO_PORT    GPIOD
#define    UART_SN1_UART_GPIO_CLK     RCC_AHB1Periph_GPIOD
#define    UART_SN1_UART_AF_PORT      GPIO_AF_USART3
#define    UART_SN1_UART_RX_SOURCE    GPIO_PinSource9
#define    UART_SN1_UART_TX_SOURCE    GPIO_PinSource8 

#define    UART_SN1_UART_RX_DMA_CLK        RCC_AHB1Periph_DMA1
#define    UART_SN1_UART_RX_DMA_STREAM     DMA1_Stream1
#define    UART_SN1_UART_RX_DMA_CHANNEL    DMA_Channel_4
#define    UART_SN1_UART_RX_DMA_IRQn       DMA1_Stream1_IRQn
#define    UART_SN1_UART_RX_DMA_FLAG       DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_TCIF1


#define    UART_SN2_UART              UART4

#define    UART_SN2_UART_CLK          RCC_APB1Periph_UART4
#define    UART_SN2_UART_DR_ADDR      (uint32_t)&UART4->DR
#define    UART_SN2_UART_RX_IRQn      UART4_IRQn

#define    UART_SN2_UART_RX_PIN       GPIO_Pin_0
#define    UART_SN2_UART_TX_PIN       GPIO_Pin_1
#define    UART_SN2_UART_GPIO_PORT    GPIOA
#define    UART_SN2_UART_GPIO_CLK     RCC_AHB1Periph_GPIOA
#define    UART_SN2_UART_AF_PORT      GPIO_AF_UART4
#define    UART_SN2_UART_RX_SOURCE    GPIO_PinSource0
#define    UART_SN2_UART_TX_SOURCE    GPIO_PinSource1

#define    UART_SN2_UART_RX_DMA_CLK        RCC_AHB1Periph_DMA1
#define    UART_SN2_UART_RX_DMA_STREAM     DMA1_Stream2
#define    UART_SN2_UART_RX_DMA_CHANNEL    DMA_Channel_4
#define    UART_SN2_UART_RX_DMA_IRQn       DMA1_Stream2_IRQn
#define    UART_SN2_UART_RX_DMA_FLAG       DMA_FLAG_FEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_TCIF2


#define    UART_DATA_TIMEOUT    ((uint32_t)0x334500)    //����DELAY_COUNT_MS = 33600���˳�ʱʱ���ԼΪ100ms



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
u32 uartBaudrate[UARTBAUD_RESERVE] = { 4800,  7200,   9600,  14400, 19200, 
                                      38400, 57600, 115200, 128000, 2000000, 2500000};
                                         
u16 uartWordLen[WORDLEN_RESERVE] = {USART_WordLength_8b, USART_WordLength_9b};

u16 uartFlowCtl[FLOWCTL_RESERVE] = {USART_HardwareFlowControl_None, USART_HardwareFlowControl_RTS,
                                    USART_HardwareFlowControl_CTS,  USART_HardwareFlowControl_RTS_CTS};
                                      
u16 uartStopBit[STOPBIT_RESERVE] = {USART_StopBits_1, USART_StopBits_0_5, 
                                    USART_StopBits_2, USART_StopBits_1_5};
                                      
u16 uartParity[PARITY_RESERVE]  = {USART_Parity_No, USART_Parity_Even, USART_Parity_Odd};



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: bspCiUartInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiUartInit(UartIntfcStruct uartIntfc)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;


    /****************UART1 GPIO����*****************************************/
    RCC_AHB1PeriphClockCmd(UART_CI_UART_GPIO_CLK, ENABLE);

    GPIO_PinAFConfig(UART_CI_UART_GPIO_PORT, UART_CI_UART_RX_SOURCE, UART_CI_UART_AF_PORT);
    GPIO_PinAFConfig(UART_CI_UART_GPIO_PORT, UART_CI_UART_TX_SOURCE, UART_CI_UART_AF_PORT); 
    
    GPIO_InitStructure.GPIO_Pin = UART_CI_UART_RX_PIN | UART_CI_UART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(UART_CI_UART_GPIO_PORT, &GPIO_InitStructure);


    /******************UART1 ����*******************************************/
    RCC_APB2PeriphClockCmd(UART_CI_UART_CLK, ENABLE);
    
    USART_InitStructure.USART_BaudRate   = uartBaudrate[uartIntfc.baud];
    USART_InitStructure.USART_WordLength = uartWordLen[uartIntfc.wordLen];
    USART_InitStructure.USART_StopBits   = uartStopBit[uartIntfc.stopBit];
    USART_InitStructure.USART_Parity     = uartParity[uartIntfc.parity];
    USART_InitStructure.USART_HardwareFlowControl = uartFlowCtl[uartIntfc.flowCtl];
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART_CI_UART, &USART_InitStructure);
    USART_ITConfig(UART_CI_UART, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART_CI_UART, ENABLE);


    /******************UART1 DMA����****************************************/
    RCC_AHB1PeriphClockCmd(UART_CI_UART_RX_DMA_CLK, ENABLE);
    
    DMA_DeInit(UART_CI_UART_RX_DMA_STREAM);                       //�ָ�ȱʡֵ
    
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;               //���ý��ջ������׵�ַ
    DMA_InitStructure.DMA_BufferSize = 0;                    //��Ҫ�����ܽ��յ����ֽ���
    
    DMA_InitStructure.DMA_Channel = UART_CI_UART_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = UART_CI_UART_DR_ADDR;           //���ô��ڽ������ݼĴ���
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                    //��������Ϊ����Դ������Ĵ��� -> �ڴ滺����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           //�����ַ�������ӵ�����������������DMA�Զ�ʵ�ֵ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //�ڴ滺������ַ���ӵ���
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    //�������ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;            //�ڴ����ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                              //���δ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                    //���ȼ�����
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(UART_CI_UART_RX_DMA_STREAM, &DMA_InitStructure);                            //д������
    
    DMA_ClearFlag(UART_CI_UART_RX_DMA_STREAM, UART_CI_UART_RX_DMA_FLAG);              //���DMA���б�־
                                         
    DMA_ITConfig(UART_CI_UART_RX_DMA_STREAM, DMA_IT_TC, ENABLE);
    //DMA_Cmd(UART_CI_UART_RX_DMA_STREAM, ENABLE); 
    //USART_DMACmd(UART_CI_UART, USART_DMAReq_Rx, ENABLE);


    /******************UART1 �ж�����****************************************/
    NVIC_InitStructure.NVIC_IRQChannel = UART_CI_UART_RX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART_CI_UART_RX_DMA_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;   
    NVIC_Init(&NVIC_InitStructure);
}


/*********************************************************************************************
�� �� ��: bspCiUartReciveOn;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiUartReciveOn(void)
{
    DMA_ClearFlag(UART_CI_UART_RX_DMA_STREAM, UART_CI_UART_RX_DMA_FLAG);
                                         
    DMA_Cmd(UART_CI_UART_RX_DMA_STREAM, DISABLE);

    //����USART1�����ж�
    USART_ClearFlag(UART_CI_UART, USART_FLAG_RXNE);
    USART_ITConfig(UART_CI_UART,  USART_IT_RXNE, ENABLE);
}


/*********************************************************************************************
�� �� ��: bspCiUartDmaOff;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiUartDmaOff(void)
{ 
    //���жϱ�ʶ����Usart�����ж�
    USART_ClearFlag(UART_CI_UART, USART_FLAG_RXNE);
    USART_ITConfig(UART_CI_UART, USART_IT_RXNE, ENABLE);

    //��DMA����ʱ��ʱ��
    DMA_Cmd(UART_CI_UART_RX_DMA_STREAM, DISABLE);
}


/*********************************************************************************************
�� �� ��: bspCiUartRecive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 bspCiUartRecive(void)
{
    USART_ClearFlag(UART_CI_UART, USART_FLAG_RXNE); 
    
    return USART_ReceiveData(UART_CI_UART);
}


/*********************************************************************************************
�� �� ��: bspCiUartDmaRecive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiUartDmaRecive(u8 *dataBuff, u32 dataLen)
{
    UART_CI_UART_RX_DMA_STREAM->M0AR = (u32)dataBuff;

    //���Ȱ�����SOF�ͳ��ȱ�������Ӧ�ü�2����Ϊ�Ѿ����յ���SOF��LEN
    //����DMA�ڽ���ʱ���ٴν���LEN�������ȼ�һ����ȥSOF  (NICK MARK)
    UART_CI_UART_RX_DMA_STREAM->NDTR = dataLen - 1;

    DMA_ClearFlag(UART_CI_UART_RX_DMA_STREAM, UART_CI_UART_RX_DMA_FLAG);
                                         
    DMA_Cmd(UART_CI_UART_RX_DMA_STREAM, ENABLE);
    USART_DMACmd(UART_CI_UART, USART_DMAReq_Rx, ENABLE);
}


/*********************************************************************************************
�� �� ��: bspCiUartSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 bspCiUartSend(u8 *dataBuff, u8 dataLen)
{
    u32 index;
    u32 timeout = UART_DATA_TIMEOUT;
    
    
    for (index = 0;index < dataLen;index++)
    {
        USART_SendData(UART_CI_UART, *dataBuff++);
        while ((USART_GetFlagStatus(UART_CI_UART, USART_FLAG_TXE) == RESET) && (timeout > 0))
        {
            //�ȴ��������
            timeout--;
        }

        if (0 == timeout)
        {
            return 1;    //���ͳ���
        }
    }

    return 0;
}


/*********************************************************************************************
�� �� ��: bspCiUartITConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiUartITConfig(FunctionalState funcState)
{
    USART_ITConfig(UART_CI_UART, USART_IT_RXNE, funcState);
}


#if 0
#endif
/*********************************************************************************************
�� �� ��: bspSensor1UartInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor1UartInit(UartIntfcStruct uartIntfc)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;


    /****************UART3 GPIO����*****************************************/
    RCC_AHB1PeriphClockCmd(UART_SN1_UART_GPIO_CLK, ENABLE);

    GPIO_PinAFConfig(UART_SN1_UART_GPIO_PORT, UART_SN1_UART_RX_SOURCE, UART_SN1_UART_AF_PORT);
    GPIO_PinAFConfig(UART_SN1_UART_GPIO_PORT, UART_SN1_UART_TX_SOURCE, UART_SN1_UART_AF_PORT); 
    
    GPIO_InitStructure.GPIO_Pin = UART_SN1_UART_RX_PIN | UART_SN1_UART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(UART_SN1_UART_GPIO_PORT, &GPIO_InitStructure);


    /******************UART3 ����*******************************************/
    RCC_APB1PeriphClockCmd(UART_SN1_UART_CLK, ENABLE);
    
    USART_InitStructure.USART_BaudRate   = uartBaudrate[uartIntfc.baud];
    USART_InitStructure.USART_WordLength = uartWordLen[uartIntfc.wordLen];
    USART_InitStructure.USART_StopBits   = uartStopBit[uartIntfc.stopBit];
    USART_InitStructure.USART_Parity     = uartParity[uartIntfc.parity];
    USART_InitStructure.USART_HardwareFlowControl = uartFlowCtl[uartIntfc.flowCtl];
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART_SN1_UART, &USART_InitStructure);
    USART_ITConfig(UART_SN1_UART, USART_IT_RXNE, ENABLE);

    //USART_Cmd(UART_SN1_UART, ENABLE);

    /******************UART3 DMA����****************************************/
    RCC_AHB1PeriphClockCmd(UART_SN1_UART_RX_DMA_CLK, ENABLE);
    
    DMA_DeInit(UART_SN1_UART_RX_DMA_STREAM);                       //�ָ�ȱʡֵ
    
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;               //���ý��ջ������׵�ַ
    DMA_InitStructure.DMA_BufferSize = 0;                    //��Ҫ�����ܽ��յ����ֽ���
    
    DMA_InitStructure.DMA_Channel = UART_SN1_UART_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = UART_SN1_UART_DR_ADDR;           //���ô��ڽ������ݼĴ���
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                    //��������Ϊ����Դ������Ĵ��� -> �ڴ滺����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           //�����ַ�������ӵ�����������������DMA�Զ�ʵ�ֵ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //�ڴ滺������ַ���ӵ���
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    //�������ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;            //�ڴ����ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                              //���δ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                    //���ȼ�����
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(UART_SN1_UART_RX_DMA_STREAM, &DMA_InitStructure);                            //д������
    
    DMA_ClearFlag(UART_SN1_UART_RX_DMA_STREAM, UART_SN1_UART_RX_DMA_FLAG);              //���DMA���б�־
                                         
    DMA_ITConfig(UART_SN1_UART_RX_DMA_STREAM, DMA_IT_TC, ENABLE);
    //DMA_Cmd(UART_SN1_UART_RX_DMA_STREAM, ENABLE); 
    //USART_DMACmd(UART_SN1_UART, USART_DMAReq_Rx, ENABLE);

    /******************UART3 �ж�����****************************************/
    NVIC_InitStructure.NVIC_IRQChannel = UART_SN1_UART_RX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART_SN1_UART_RX_DMA_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;   
    NVIC_Init(&NVIC_InitStructure);
}


/*********************************************************************************************
�� �� ��: bspSensor1UartReciveOn;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor1UartReciveOn(void)
{
    DMA_ClearFlag(UART_SN1_UART_RX_DMA_STREAM, UART_SN1_UART_RX_DMA_FLAG);
                                         
    DMA_Cmd(UART_SN1_UART_RX_DMA_STREAM, DISABLE);

    //����USART3�����ж�
    USART_ClearFlag(UART_SN1_UART, USART_FLAG_RXNE);
    USART_ITConfig(UART_SN1_UART,  USART_IT_RXNE, ENABLE);
    USART_Cmd(UART_SN1_UART, ENABLE);
}


/*********************************************************************************************
�� �� ��: bspSensor1UartReciveOff;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor1UartReciveOff(void)
{
    DMA_ClearFlag(UART_SN1_UART_RX_DMA_STREAM, UART_SN1_UART_RX_DMA_FLAG);
                                         
    DMA_Cmd(UART_SN1_UART_RX_DMA_STREAM, DISABLE);

    //�ر�USART����
    USART_ClearFlag(UART_SN1_UART, USART_FLAG_RXNE);
    USART_Cmd(UART_SN1_UART, DISABLE);
}


/*********************************************************************************************
�� �� ��: bspSensor1UartDmaOff;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor1UartDmaOff(void)
{
    //���жϱ�ʶ����Usart�����ж�
    USART_ClearFlag(UART_SN1_UART, USART_FLAG_RXNE);
    USART_ITConfig(UART_SN1_UART, USART_IT_RXNE, ENABLE);

    //��DMA����ʱ��ʱ��
    DMA_ClearFlag(UART_SN1_UART_RX_DMA_STREAM, UART_SN1_UART_RX_DMA_FLAG);
    DMA_Cmd(UART_SN1_UART_RX_DMA_STREAM, DISABLE);
}


/*********************************************************************************************
�� �� ��: bspSensor1UartDmaCounterGet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u16 bspSensor1UartDmaCounterGet(void)
{
    return DMA_GetCurrDataCounter(UART_SN1_UART_RX_DMA_STREAM);
}


/*********************************************************************************************
�� �� ��: bspSensor1UartRecive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 bspSensor1UartRecive(void)
{
    USART_ClearFlag(UART_SN1_UART, USART_FLAG_RXNE); 
    
    return USART_ReceiveData(UART_SN1_UART);
}


/*********************************************************************************************
�� �� ��: bspSensor1UartDmaRecive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor1UartDmaRecive(u8 *dataBuff, u32 dataLen)
{
    UART_SN1_UART_RX_DMA_STREAM->M0AR = (u32)dataBuff;

    //���Ȱ�����SOF�ͳ��ȱ�������Ӧ�ü�2����Ϊ�Ѿ����յ���SOF��LEN
    //����DMA�ڽ���ʱ���ٴν���LEN�������ȼ�һ����ȥSOF  (NICK MARK)
    UART_SN1_UART_RX_DMA_STREAM->NDTR = dataLen - 1;

    DMA_ClearFlag(UART_SN1_UART_RX_DMA_STREAM, UART_SN1_UART_RX_DMA_FLAG);
                                         
    DMA_Cmd(UART_SN1_UART_RX_DMA_STREAM, ENABLE);
    USART_DMACmd(UART_SN1_UART, USART_DMAReq_Rx, ENABLE);
}


/*********************************************************************************************
�� �� ��: bspSensor1UartSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 bspSensor1UartSend(u8 *dataBuff, u8 dataLen)
{
    u32 index;
    u32 timeout = UART_DATA_TIMEOUT;
    
    
    for (index = 0;index < dataLen;index++)
    {
        USART_SendData(UART_SN1_UART, *dataBuff++);
        while ((USART_GetFlagStatus(UART_SN1_UART, USART_FLAG_TXE) == RESET) && (timeout > 0))
        {
            //�ȴ��������
            timeout--;
        }

        if (0 == timeout)
        {
            return 1;    //���ͳ���
        }
    }

    return 0;
}


/*********************************************************************************************
�� �� ��: bspSensor1UartITConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor1UartITConfig(FunctionalState funcState)
{
    USART_ITConfig(UART_SN1_UART, USART_IT_RXNE, funcState);
}


#if 0
#endif
/*********************************************************************************************
�� �� ��: bspSensor2UartInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor2UartInit(UartIntfcStruct uartIntfc)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;


    /****************UART4 GPIO����*****************************************/
    RCC_AHB1PeriphClockCmd(UART_SN2_UART_GPIO_CLK, ENABLE);

    GPIO_PinAFConfig(UART_SN2_UART_GPIO_PORT, UART_SN2_UART_RX_SOURCE, UART_SN2_UART_AF_PORT);
    GPIO_PinAFConfig(UART_SN2_UART_GPIO_PORT, UART_SN2_UART_TX_SOURCE, UART_SN2_UART_AF_PORT); 
    
    GPIO_InitStructure.GPIO_Pin = UART_SN2_UART_RX_PIN | UART_SN2_UART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(UART_SN2_UART_GPIO_PORT, &GPIO_InitStructure);


    /******************UART4 ����*******************************************/
    RCC_APB1PeriphClockCmd(UART_SN2_UART_CLK, ENABLE);
    
    USART_InitStructure.USART_BaudRate   = uartBaudrate[uartIntfc.baud];
    USART_InitStructure.USART_WordLength = uartWordLen[uartIntfc.wordLen];
    USART_InitStructure.USART_StopBits   = uartStopBit[uartIntfc.stopBit];
    USART_InitStructure.USART_Parity     = uartParity[uartIntfc.parity];
    USART_InitStructure.USART_HardwareFlowControl = uartFlowCtl[uartIntfc.flowCtl];
    USART_InitStructure.USART_Mode = USART_Mode_Rx;
    USART_Init(UART_SN2_UART, &USART_InitStructure);
    USART_ITConfig(UART_SN2_UART, USART_IT_RXNE, ENABLE);

    //USART_Cmd(UART_SN2_UART, ENABLE);

    /******************UART4 DMA����****************************************/
    RCC_AHB1PeriphClockCmd(UART_SN2_UART_RX_DMA_CLK, ENABLE);
    
    DMA_DeInit(UART_SN2_UART_RX_DMA_STREAM);                       //�ָ�ȱʡֵ
    
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;               //���ý��ջ������׵�ַ
    DMA_InitStructure.DMA_BufferSize = 0;                    //��Ҫ�����ܽ��յ����ֽ���
    
    DMA_InitStructure.DMA_Channel = UART_SN2_UART_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = UART_SN2_UART_DR_ADDR;           //���ô��ڽ������ݼĴ���
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                    //��������Ϊ����Դ������Ĵ��� -> �ڴ滺����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           //�����ַ�������ӵ�����������������DMA�Զ�ʵ�ֵ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //�ڴ滺������ַ���ӵ���
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    //�������ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;            //�ڴ����ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                              //���δ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                    //���ȼ�����
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(UART_SN2_UART_RX_DMA_STREAM, &DMA_InitStructure);                            //д������
    
    DMA_ClearFlag(UART_SN2_UART_RX_DMA_STREAM, UART_SN2_UART_RX_DMA_FLAG);              //���DMA���б�־
                                         
    DMA_ITConfig(UART_SN2_UART_RX_DMA_STREAM, DMA_IT_TC, ENABLE);
    //DMA_Cmd(UART_SN2_UART_RX_DMA_STREAM, ENABLE); 
    //USART_DMACmd(UART_SN2_UART, USART_DMAReq_Rx, ENABLE);

    /******************UART4 �ж�����****************************************/
    NVIC_InitStructure.NVIC_IRQChannel = UART_SN2_UART_RX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART_SN2_UART_RX_DMA_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;   
    NVIC_Init(&NVIC_InitStructure);
}


/*********************************************************************************************
�� �� ��: bspSensor2UartReciveOn;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor2UartReciveOn(void)
{
    DMA_ClearFlag(UART_SN2_UART_RX_DMA_STREAM, UART_SN2_UART_RX_DMA_FLAG);
                                         
    DMA_Cmd(UART_SN2_UART_RX_DMA_STREAM, DISABLE);

    //����UART4�����ж�
    USART_ClearFlag(UART_SN2_UART, USART_FLAG_RXNE);
    USART_ITConfig(UART_SN2_UART,  USART_IT_RXNE, ENABLE);
    USART_Cmd(UART_SN2_UART, ENABLE);
}


/*********************************************************************************************
�� �� ��: bspSensor2UartReciveOff;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor2UartReciveOff(void)
{
    DMA_ClearFlag(UART_SN2_UART_RX_DMA_STREAM, UART_SN2_UART_RX_DMA_FLAG);
                                         
    DMA_Cmd(UART_SN2_UART_RX_DMA_STREAM, DISABLE);

    //����UART4�����ж�
    USART_ClearFlag(UART_SN2_UART, USART_FLAG_RXNE);
    USART_Cmd(UART_SN2_UART, DISABLE);
}


/*********************************************************************************************
�� �� ��: bspSensor2UartDmaOff;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor2UartDmaOff(void)
{ 
    //���жϱ�ʶ����Usart�����ж�
    USART_ClearFlag(UART_SN2_UART, USART_FLAG_RXNE);
    USART_ITConfig(UART_SN2_UART, USART_IT_RXNE, ENABLE);

    //��DMA����ʱ��ʱ��
    DMA_ClearFlag(UART_SN2_UART_RX_DMA_STREAM, UART_SN2_UART_RX_DMA_FLAG);
    DMA_Cmd(UART_SN2_UART_RX_DMA_STREAM, DISABLE);
}


/*********************************************************************************************
�� �� ��: bspSensor2UartDmaCounterGet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u16 bspSensor2UartDmaCounterGet(void)
{
    return DMA_GetCurrDataCounter(UART_SN2_UART_RX_DMA_STREAM);
}


/*********************************************************************************************
�� �� ��: bspSensor2UartRecive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 bspSensor2UartRecive(void)
{
    USART_ClearFlag(UART_SN2_UART, USART_FLAG_RXNE); 
    
    return USART_ReceiveData(UART_SN2_UART);
}


/*********************************************************************************************
�� �� ��: bspSensor2UartDmaRecive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor2UartDmaRecive(u8 *dataBuff, u32 dataLen)
{
    UART_SN2_UART_RX_DMA_STREAM->M0AR = (u32)dataBuff;

    //���Ȱ�����SOF�ͳ��ȱ�������Ӧ�ü�2����Ϊ�Ѿ����յ���SOF��LEN
    //����DMA�ڽ���ʱ���ٴν���LEN�������ȼ�һ����ȥSOF  (NICK MARK)
    UART_SN2_UART_RX_DMA_STREAM->NDTR = dataLen - 1;

    DMA_ClearFlag(UART_SN2_UART_RX_DMA_STREAM, UART_SN2_UART_RX_DMA_FLAG);
                                         
    DMA_Cmd(UART_SN2_UART_RX_DMA_STREAM, ENABLE);
    USART_DMACmd(UART_SN2_UART, USART_DMAReq_Rx, ENABLE);
}


/*********************************************************************************************
�� �� ��: bspSensor2UartSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 bspSensor2UartSend(u8 *dataBuff, u8 dataLen)
{
    u32 index;
    u32 timeout = UART_DATA_TIMEOUT;
    
    
    for (index = 0;index < dataLen;index++)
    {
        USART_SendData(UART_SN2_UART, *dataBuff++);
        while ((USART_GetFlagStatus(UART_SN2_UART, USART_FLAG_TXE) == RESET) && (timeout > 0))
        {
            //�ȴ��������
            timeout--;
        }

        if (0 == timeout)
        {
            return 1;    //���ͳ���
        }
    }

    return 0;
}


/*********************************************************************************************
�� �� ��: bspSensor2UartITConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspSensor2UartITConfig(FunctionalState funcState)
{
    USART_ITConfig(UART_SN2_UART, USART_IT_RXNE, funcState);
}



/*******************************************�ļ�β********************************************/
