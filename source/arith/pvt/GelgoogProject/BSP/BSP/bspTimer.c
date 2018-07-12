/**********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspTimer.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.25;
��ʷ�汾:  ��;
**********************************************************************************************/
#include "bspTimer.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    LSI_FREQ          32       //KHz

#define    DELAY_COUNT_MS    33600    //1ms
#define    DELAY_COUNT_US    32       //1us

#define    ENC_TIME_PERIOD   65000    //Ŀǰ֧�ֵ��������������

#define    ENC1_TIME_CLK     RCC_APB1Periph_TIM2

#define    ENC1_TIME         TIM2

#define    ENC1_GPIO_CLK            RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB
#define    ENC1_GPIO_AF             GPIO_AF_TIM2

#define    ENC1_CHAN_Z_GPIO_PORT    GPIOB
#define    ENC1_CHAN_Z_PIN          GPIO_Pin_10
#define    ENC1_CHAN_Z_SOURCE       GPIO_PinSource10
#define    ENC1_CHAN_Z_IT_EXTILINE  EXTI_Line10
#define    ENC1_CHAN_Z_IT_IRQ       EXTI15_10_IRQn
#define    ENC1_CHAN_Z_IT_PORTSRC   EXTI_PortSourceGPIOB
#define    ENC1_CHAN_Z_IT_PINSRC    EXTI_PinSource10

#define    ENC1_OF_IT_IRQ           TIM2_IRQn

#define    ENC1_CHAN_A_GPIO_PORT    GPIOA
#define    ENC1_CHAN_A_PIN          GPIO_Pin_15
#define    ENC1_CHAN_A_SOURCE       GPIO_PinSource15

#define    ENC1_CHAN_B_GPIO_PORT    GPIOB
#define    ENC1_CHAN_B_PIN          GPIO_Pin_3
#define    ENC1_CHAN_B_SOURCE       GPIO_PinSource3


#define    ENC2_TIME_CLK    RCC_APB1Periph_TIM3

#define    ENC2_TIME        TIM3

#define    ENC2_GPIO_CLK            RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB
#define    ENC2_GPIO_AF             GPIO_AF_TIM3

#define    ENC2_CHAN_Z_GPIO_PORT    GPIOB
#define    ENC2_CHAN_Z_PIN          GPIO_Pin_1
#define    ENC2_CHAN_Z_SOURCE       GPIO_PinSource1

#define    ENC2_CHAN_Z_IT_EXTILINE  EXTI_Line1
#define    ENC2_CHAN_Z_IT_IRQ       EXTI1_IRQn
#define    ENC2_CHAN_Z_IT_PORTSRC   EXTI_PortSourceGPIOB
#define    ENC2_CHAN_Z_IT_PINSRC    EXTI_PinSource1

#define    ENC2_OF_IT_IRQ           TIM3_IRQn

#define    ENC2_CHAN_A_GPIO_PORT    GPIOB
#define    ENC2_CHAN_A_PIN          GPIO_Pin_4
#define    ENC2_CHAN_A_SOURCE       GPIO_PinSource4

#define    ENC2_CHAN_B_GPIO_PORT    GPIOA
#define    ENC2_CHAN_B_PIN          GPIO_Pin_7
#define    ENC2_CHAN_B_SOURCE       GPIO_PinSource7


#define    ENC3_TIME_CLK    RCC_APB1Periph_TIM5

#define    ENC3_TIME        TIM5

#define    ENC3_GPIO_CLK            RCC_AHB1Periph_GPIOH
#define    ENC3_GPIO_AF             GPIO_AF_TIM5

#define    ENC3_CHAN_Z_GPIO_PORT    GPIOH
#define    ENC3_CHAN_Z_PIN          GPIO_Pin_12
#define    ENC3_CHAN_Z_SOURCE       GPIO_PinSource12

#define    ENC3_CHAN_Z_IT_EXTILINE  EXTI_Line12
#define    ENC3_CHAN_Z_IT_IRQ       EXTI15_10_IRQn
#define    ENC3_CHAN_Z_IT_PORTSRC   EXTI_PortSourceGPIOH
#define    ENC3_CHAN_Z_IT_PINSRC    EXTI_PinSource12

#define    ENC3_OF_IT_IRQ           TIM5_IRQn

#define    ENC3_CHAN_A_GPIO_PORT    GPIOH
#define    ENC3_CHAN_A_PIN          GPIO_Pin_10
#define    ENC3_CHAN_A_SOURCE       GPIO_PinSource10

#define    ENC3_CHAN_B_GPIO_PORT    GPIOH
#define    ENC3_CHAN_B_PIN          GPIO_Pin_11
#define    ENC3_CHAN_B_SOURCE       GPIO_PinSource11


#define    ENC4_TIME_CLK    RCC_APB2Periph_TIM8

#define    ENC4_TIME        TIM8

#define    ENC4_GPIO_CLK            RCC_AHB1Periph_GPIOI
#define    ENC4_GPIO_AF             GPIO_AF_TIM8

#define    ENC4_CHAN_Z_GPIO_PORT    GPIOI
#define    ENC4_CHAN_Z_PIN          GPIO_Pin_7
#define    ENC4_CHAN_Z_SOURCE       GPIO_PinSource7

#define    ENC4_CHAN_Z_IT_EXTILINE  EXTI_Line7
#define    ENC4_CHAN_Z_IT_IRQ       EXTI9_5_IRQn
#define    ENC4_CHAN_Z_IT_PORTSRC   EXTI_PortSourceGPIOI
#define    ENC4_CHAN_Z_IT_PINSRC    EXTI_PinSource7

#define    ENC4_OF_IT_IRQ           TIM8_UP_TIM13_IRQn

#define    ENC4_CHAN_A_GPIO_PORT    GPIOI
#define    ENC4_CHAN_A_PIN          GPIO_Pin_5
#define    ENC4_CHAN_A_SOURCE       GPIO_PinSource5

#define    ENC4_CHAN_B_GPIO_PORT    GPIOI
#define    ENC4_CHAN_B_PIN          GPIO_Pin_6
#define    ENC4_CHAN_B_SOURCE       GPIO_PinSource6



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
s32 encoderZCount[ENCODER_SUPPORT_NUM];
s32 encoderABCount[ENCODER_SUPPORT_NUM];
EncoderMultiEnum encoderMult[ENCODER_SUPPORT_NUM];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: bspIWDGInit;
ʵ�ֹ���: ��; 
�������: timeout, ��λʱ�䣬��λms;
�������: ��;
�� �� ֵ: ��;
˵    ��: �������Ź�ʱ��32KHz����λʱ�����Prescaler * Reload / LSI_FREQ;
*********************************************************************************************/
void bspIWDGInit(u16 timeout)
{
    //Enables write access to IWDG_PR and IWDG_RLR registers    
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);        
    //Set prescaler value    
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    //Set reload value    
    //Reload Value = timeout(ms) * LSI_FREQ / Prescaler    
    IWDG_SetReload(timeout * LSI_FREQ / 32);

    //Download reload value to register    
    IWDG_ReloadCounter(); 

    //Enable IWDG    
    IWDG_Enable();
}


/*********************************************************************************************
�� �� ��: bspIWDGFeed;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspIWDGFeed(void)  
{      
    IWDG_ReloadCounter();
}


/*********************************************************************************************
�� �� ��: bspSoftReset;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��λ��ʹ�ÿ��Ź����и�λ;
*********************************************************************************************/
void bspSoftReset(void) 
{
#if 0
    bspIWDGInit(25);    //25ms���Ź���λ
    do
    {
        //ѭ�������Ź���λ��Ϊ���ڿ��Ź���λ���ʱ���ڲ���Ӧ�κβ���
    }while(1);
    
#else

    __set_FAULTMASK(1);    //�ر������ж�
    NVIC_SystemReset();    //��λ
#endif
}


/*********************************************************************************************
�� �� ��: bspEncoderTimerInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspEncoderTimerInit(u8 chanNum, EncoderMultiEnum encMult, FunctionalState NewState)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_ICInitTypeDef        TIM_ICInitStructure;
    GPIO_InitTypeDef         GPIO_InitStructure;
    NVIC_InitTypeDef         NVIC_InitStructure; 
    EXTI_InitTypeDef         EXTI_InitStructure;

    u8  timerGpioAf;
    u8  timerExtiPortSource;
    u8  timerExtiPinSource;
    u8  timerOFIrqChannel;
    u8  timerIrqChannel;
    u32 timerExtiLine;
    
    u32 timerGpioClk;
    u32 timerClock;
    
    u32 timerChanAPin;
    u32 timerChanBPin;
    u32 timerChanZPin;
    
    u16 timerChanASource;
    u16 timerChanBSource;

    TIM_TypeDef * timerType;
    
    GPIO_TypeDef* timerChanAPort;
    GPIO_TypeDef* timerChanBPort;
    GPIO_TypeDef* timerChanZPort;
    

    //����ͨ��ָ����Ӧ�Ĺܽš�ʱ�ӵȲ���
    switch (chanNum)
    {
        case 0:    //TIM2
            timerGpioAf  = ENC1_GPIO_AF;
            timerGpioClk = ENC1_GPIO_CLK;
            timerClock   = ENC1_TIME_CLK;
            timerType    = ENC1_TIME;
            
            timerChanAPin = ENC1_CHAN_A_PIN;
            timerChanBPin = ENC1_CHAN_B_PIN;
            timerChanZPin = ENC1_CHAN_Z_PIN;

            timerChanASource = ENC1_CHAN_A_SOURCE;
            timerChanBSource = ENC1_CHAN_B_SOURCE;

            timerChanAPort = ENC1_CHAN_A_GPIO_PORT;
            timerChanBPort = ENC1_CHAN_B_GPIO_PORT;
            timerChanZPort = ENC1_CHAN_Z_GPIO_PORT;

            timerOFIrqChannel = ENC1_OF_IT_IRQ;

            timerExtiPortSource = ENC1_CHAN_Z_IT_PORTSRC;
            timerExtiPinSource  = ENC1_CHAN_Z_IT_PINSRC;
            timerIrqChannel     = ENC1_CHAN_Z_IT_IRQ;
            timerExtiLine       = ENC1_CHAN_Z_IT_EXTILINE;
          break;

        case 1:    //TIM3
            timerGpioAf  = ENC2_GPIO_AF;
            timerGpioClk = ENC2_GPIO_CLK;
            timerClock   = ENC2_TIME_CLK;
            timerType    = ENC2_TIME;
            
            timerChanAPin = ENC2_CHAN_A_PIN;
            timerChanBPin = ENC2_CHAN_B_PIN;
            timerChanZPin = ENC2_CHAN_Z_PIN;

            timerChanASource = ENC2_CHAN_A_SOURCE;
            timerChanBSource = ENC2_CHAN_B_SOURCE;

            timerChanAPort = ENC2_CHAN_A_GPIO_PORT;
            timerChanBPort = ENC2_CHAN_B_GPIO_PORT;
            timerChanZPort = ENC2_CHAN_Z_GPIO_PORT;

            timerOFIrqChannel = ENC2_OF_IT_IRQ;

            timerExtiPortSource = ENC2_CHAN_Z_IT_PORTSRC;
            timerExtiPinSource  = ENC2_CHAN_Z_IT_PINSRC;
            timerIrqChannel     = ENC2_CHAN_Z_IT_IRQ;
            timerExtiLine       = ENC2_CHAN_Z_IT_EXTILINE;
          break;

        case 2:    //TIM5
            timerGpioAf  = ENC3_GPIO_AF;
            timerGpioClk = ENC3_GPIO_CLK;
            timerClock   = ENC3_TIME_CLK;
            timerType    = ENC3_TIME;
            
            timerChanAPin = ENC3_CHAN_A_PIN;
            timerChanBPin = ENC3_CHAN_B_PIN;
            timerChanZPin = ENC3_CHAN_Z_PIN;

            timerChanASource = ENC3_CHAN_A_SOURCE;
            timerChanBSource = ENC3_CHAN_B_SOURCE;

            timerChanAPort = ENC3_CHAN_A_GPIO_PORT;
            timerChanBPort = ENC3_CHAN_B_GPIO_PORT;
            timerChanZPort = ENC3_CHAN_Z_GPIO_PORT;

            timerOFIrqChannel = ENC3_OF_IT_IRQ;

            timerExtiPortSource = ENC3_CHAN_Z_IT_PORTSRC;
            timerExtiPinSource  = ENC3_CHAN_Z_IT_PINSRC;
            timerIrqChannel     = ENC3_CHAN_Z_IT_IRQ;
            timerExtiLine       = ENC3_CHAN_Z_IT_EXTILINE;
          break;

        case 3:    //TIM8
            timerGpioAf  = ENC4_GPIO_AF;
            timerGpioClk = ENC4_GPIO_CLK;
            timerClock   = ENC4_TIME_CLK;
            timerType    = ENC4_TIME;
            
            timerChanAPin = ENC4_CHAN_A_PIN;
            timerChanBPin = ENC4_CHAN_B_PIN;
            timerChanZPin = ENC4_CHAN_Z_PIN;

            timerChanASource = ENC4_CHAN_A_SOURCE;
            timerChanBSource = ENC4_CHAN_B_SOURCE;

            timerChanAPort = ENC4_CHAN_A_GPIO_PORT;
            timerChanBPort = ENC4_CHAN_B_GPIO_PORT;
            timerChanZPort = ENC4_CHAN_Z_GPIO_PORT;

            timerOFIrqChannel = ENC4_OF_IT_IRQ;

            timerExtiPortSource = ENC4_CHAN_Z_IT_PORTSRC;
            timerExtiPinSource  = ENC4_CHAN_Z_IT_PINSRC;
            timerIrqChannel     = ENC4_CHAN_Z_IT_IRQ;
            timerExtiLine       = ENC4_CHAN_Z_IT_EXTILINE;
          break;

        default:
          break;
    }

    /***************************************TIMX GPIO����******************************************/
    RCC_AHB1PeriphClockCmd(timerGpioClk, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = timerChanAPin;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(timerChanAPort, &GPIO_InitStructure);
    GPIO_PinAFConfig(timerChanAPort, timerChanASource, timerGpioAf); 
    
    GPIO_InitStructure.GPIO_Pin   = timerChanBPin;
    GPIO_Init(timerChanBPort, &GPIO_InitStructure);
    GPIO_PinAFConfig(timerChanBPort, timerChanBSource, timerGpioAf);   

    GPIO_InitStructure.GPIO_Pin   = timerChanZPin;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(timerChanZPort, &GPIO_InitStructure);


    /***************************************TIMX ����******************************************/
    if (3 == chanNum)    //TIM8��APB2
    {
        RCC_APB2PeriphClockCmd(timerClock, NewState);    //ʹ��TIMXʱ��
    }
    else
    {
        RCC_APB1PeriphClockCmd(timerClock, NewState);    //ʹ��TIMXʱ��  
    }

    TIM_DeInit(timerType);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_Period    = ENC_TIME_PERIOD - 1;  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                           
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                
    TIM_TimeBaseInit(timerType, &TIM_TimeBaseStructure); 

    //���ݱ������ö�ʱ��������ģʽ
    switch (encMult)
    {
        //������ģʽֻ��X2��X4����ģʽ
        case ENCMULT_SINGLE:
        case ENCMULT_DOUBLE:
            encoderMult[chanNum] = encMult;
            TIM_EncoderInterfaceConfig(timerType, 
                                       TIM_EncoderMode_TI1, 
                                       TIM_ICPolarity_BothEdge,
                                       TIM_ICPolarity_BothEdge);
          break;
          
        case ENCMULT_QUADR:
            encoderMult[chanNum] = encMult;
            TIM_EncoderInterfaceConfig(timerType, 
                                       TIM_EncoderMode_TI12, 
                                       TIM_ICPolarity_BothEdge,
                                       TIM_ICPolarity_BothEdge);
          break;

        default:
            encoderMult[chanNum] = ENCMULT_SINGLE;
          break;
    }

    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(timerType, &TIM_ICInitStructure);

    TIM_ITConfig(timerType, TIM_IT_Update, NewState); 
    TIM_ClearFlag(timerType, TIM_FLAG_Update);
    timerType->CNT = 0;  //�������
    TIM_Cmd(timerType, NewState);

    NVIC_InitStructure.NVIC_IRQChannel = timerOFIrqChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
    NVIC_Init(&NVIC_InitStructure);


    /***************************************Zͨ�� ����******************************************/
    SYSCFG_EXTILineConfig(timerExtiPortSource, timerExtiPinSource);
    
    NVIC_InitStructure.NVIC_IRQChannel = timerIrqChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
    NVIC_Init(&NVIC_InitStructure); 

    EXTI_InitStructure.EXTI_Line = timerExtiLine;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = NewState;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearFlag(timerExtiLine);
}


/*********************************************************************************************
�� �� ��: bspEncoderCountSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspEncoderCountSet(u8 chanNum, u16 count)
{
    //����ͨ��ָ����Ӧ��TIM
    switch (chanNum)
    {
        case 0:    //TIM2
            ENC1_TIME->CNT = count;
            encoderZCount[0] = count;
            encoderABCount[0] = count;
          break;

        case 1:    //TIM3
            ENC2_TIME->CNT = count;
            encoderZCount[1] = count;
            encoderABCount[1] = count;
          break;

        case 2:    //TIM5
            ENC3_TIME->CNT = count;
            encoderZCount[2] = count;
            encoderABCount[2] = count;
          break;

        case 3:    //TIM8
            ENC4_TIME->CNT = count;
            encoderZCount[3] = count;
            encoderABCount[3] = count;
          break;

        default:
          break;
    }
}


/*********************************************************************************************
�� �� ��: bspEncoderABCountGet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
s32 bspEncoderABCountGet(u8 chanNum)
{
    u32 timerCount = 0;
    

    //����ͨ��ָ����Ӧ��TIM
    if (chanNum < ENCODER_SUPPORT_NUM)
    {

        switch (chanNum)
        {
            case 0:    //TIM2
                timerCount  = ENC1_TIME->CNT;    //TIM2��TIM5�ļĴ�����32λ��3��8��16λ
              break;
    
            case 1:    //TIM3
                timerCount = ENC2_TIME->CNT;
              break;
    
            case 2:    //TIM5
                timerCount = ENC3_TIME->CNT;
              break;
    
            case 3:    //TIM8
                timerCount = ENC4_TIME->CNT;
              break;
    
            default:
              break;
        }

        encoderABCount[chanNum] += timerCount;
            
        if (ENCMULT_SINGLE == encoderMult[chanNum])
        {
            encoderABCount[chanNum] /= 2;    //������ֻ֧��X2��X4ģʽ�������ڵ�����ģʽ����Ҫ�ֶ�����2
        }
    }

    return encoderABCount[chanNum];
}


/*********************************************************************************************
�� �� ��: bspEncoderCircleCountGet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
s16 bspEncoderCircleCountGet(u8 chanNum)
{
    s16 circleCount = 0;
    

    //����ͨ��ָ����Ӧ��TIM
    switch (chanNum)
    {
        case 0:    //TIM2
        case 1:    //TIM3
        case 2:    //TIM5
        case 3:    //TIM8
            circleCount = encoderZCount[chanNum];
          break;

        default:
          break;
    } 

    return circleCount;
}


/*********************************************************************************************
�� �� ��: irqChanZProcess;
ʵ�ֹ���: ������Zͨ����������
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void irqChanZProcess(u8 chanNum)
{
    if (chanNum < ENCODER_SUPPORT_NUM)
    {
        encoderZCount[chanNum]++;
    }
}


/*********************************************************************************************
�� �� ��: irqChanABProcess;
ʵ�ֹ���: ������ABͨ����������
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void irqChanABProcess(u8 chanNum, EncoderDirEnum encoderDir)
{
    if (chanNum < ENCODER_SUPPORT_NUM)
    {
        if (ENCDIR_P == encoderDir)
        {
            encoderABCount[chanNum] += ENC_TIME_PERIOD;
        }
        else
        {
            encoderABCount[chanNum] -= ENC_TIME_PERIOD;
        }
    }
}


/*********************************************************************************************
�� �� ��: bspDelayMs;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ͨ��ִ��ѭ����ʵ����ʱʱ�䣬��ȷ�Ȳ���;
*********************************************************************************************/
#pragma optimize=none
void bspDelayMs(u32 milliSeconds)  
{
    u32 msCount;

    for (;milliSeconds != 0;milliSeconds--)
    {
        msCount = DELAY_COUNT_MS;
        for(; msCount!= 0;msCount--);
    }
}


/*********************************************************************************************
�� �� ��: bspDelayUs;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ͨ��ִ��ѭ����ʵ����ʱʱ�䣬��ȷ�Ȳ���;
*********************************************************************************************/
#pragma optimize=none
void bspDelayUs(u32 microSeconds)  
{
    u32 msCount;

    for (;microSeconds != 0;microSeconds--)
    {
        msCount = DELAY_COUNT_US;
        for(; msCount!= 0;msCount--);
    }
}



/*******************************************�ļ�β********************************************/
