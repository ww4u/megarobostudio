/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspGpio.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.25;
历史版本:  无;
**********************************************************************************************/
#include "bspGpio.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    GPIO_FPGA_PROG_GPIO_CLK      RCC_AHB1Periph_GPIOB
#define    GPIO_FPGA_PROG_GPIO_PORT     GPIOB

#define    GPIO_FPGA_PROG_ANSWER_PIN    GPIO_Pin_11
#define    GPIO_FPGA_PROG_DONE_PIN      GPIO_Pin_12
#define    GPIO_FPGA_PROG_CONFIG_PIN    GPIO_Pin_13

#define    GPIO_DRIVER_CTRL_GPIO_CLK    RCC_AHB1Periph_GPIOF
#define    GPIO_DRIVER_CTRL_GPIO_PORT   GPIOF
#define    GPIO_DRIVER_CTRL_CHANA_PIN   GPIO_Pin_7
#define    GPIO_DRIVER_CTRL_CHANB_PIN   GPIO_Pin_10 

#define    GPIO_LED_GPIO_CLK            RCC_AHB1Periph_GPIOE
#define    GPIO_LED_GPIO_PORT           GPIOE
#define    GPIO_LED_RED_PIN             GPIO_Pin_5
#define    GPIO_LED_GREEN_PIN           GPIO_Pin_6

#define    GPIO_FPGA_IT_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define    GPIO_FPGA_IT_GPIO_PORT       GPIOE
#define    GPIO_FPGA_IT_PIN             GPIO_Pin_0
#define    GPIO_FPGA_IT_EXTILINE        EXTI_Line0
#define    GPIO_FPGA_IT_IRQ             EXTI0_IRQn
#define    GPIO_FPGA_IT_PORTSRC         EXTI_PortSourceGPIOE
#define    GPIO_FPGA_IT_PINSRC          EXTI_PinSource0

#define    GPIO_MODEL_GPIO_CLK          RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOI
#define    GPIO_MODEL_7_GPIO_PORT       GPIOI
#define    GPIO_MODEL_GPIO_PORT         GPIOG
#define    GPIO_MODEL_0_GPIO_PORT       GPIOD
#define    GPIO_MODEL_BIT7_PIN          GPIO_Pin_1
#define    GPIO_MODEL_BIT6_PIN          GPIO_Pin_8
#define    GPIO_MODEL_BIT5_PIN          GPIO_Pin_7
#define    GPIO_MODEL_BIT4_PIN          GPIO_Pin_5
#define    GPIO_MODEL_BIT3_PIN          GPIO_Pin_4
#define    GPIO_MODEL_BIT2_PIN          GPIO_Pin_3
#define    GPIO_MODEL_BIT1_PIN          GPIO_Pin_2
#define    GPIO_MODEL_BIT0_PIN          GPIO_Pin_13



/*************************************局部常量和类型定义**************************************/
typedef struct
{
    u8 bit0 :1;
    u8 bit1 :1;
    u8 bit2 :1;
    u8 bit3 :1;
    u8 bit4 :1;
    u8 bit5 :1;
    u8 bit6 :1;
    u8 bit7 :1;
    
}BitFiledStruct;

typedef union
{
    BitFiledStruct bitFiled;
    u8 byteValue;
    
}BitByteUnion;



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: bspFpgaProgGpioInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspFpgaProgGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

  
    RCC_AHB1PeriphClockCmd(GPIO_FPGA_PROG_GPIO_CLK, ENABLE);    

    GPIO_InitStructure.GPIO_Pin = GPIO_FPGA_PROG_CONFIG_PIN;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_FPGA_PROG_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(GPIO_FPGA_PROG_GPIO_PORT, GPIO_FPGA_PROG_CONFIG_PIN);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_FPGA_PROG_ANSWER_PIN | GPIO_FPGA_PROG_DONE_PIN;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIO_FPGA_PROG_GPIO_PORT, &GPIO_InitStructure);   
}


/*********************************************************************************************
函 数 名: bspGpioFpgaProgConfigSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspGpioFpgaProgConfigSet(BitAction pinAction)
{
    GPIO_WriteBit(GPIO_FPGA_PROG_GPIO_PORT, GPIO_FPGA_PROG_CONFIG_PIN, pinAction);
}


/*********************************************************************************************
函 数 名: bspGpioFpgaProgAnswerRead;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 bspGpioFpgaProgAnswerRead(void)
{
    return GPIO_ReadInputDataBit(GPIO_FPGA_PROG_GPIO_PORT, GPIO_FPGA_PROG_ANSWER_PIN);
}


/*********************************************************************************************
函 数 名: bspGpioFpgaProgDoneRead;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 bspGpioFpgaProgDoneRead(void)
{
    return GPIO_ReadInputDataBit(GPIO_FPGA_PROG_GPIO_PORT, GPIO_FPGA_PROG_DONE_PIN);
}


/*********************************************************************************************
函 数 名: bspDriverGpioInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspDriverGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

  
    RCC_AHB1PeriphClockCmd(GPIO_DRIVER_CTRL_GPIO_CLK, ENABLE);    

    GPIO_InitStructure.GPIO_Pin = GPIO_DRIVER_CTRL_CHANA_PIN | GPIO_DRIVER_CTRL_CHANB_PIN;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_DRIVER_CTRL_GPIO_PORT, &GPIO_InitStructure);   

    GPIO_WriteBit(GPIO_DRIVER_CTRL_GPIO_PORT, GPIO_DRIVER_CTRL_CHANA_PIN, Bit_SET);
    GPIO_WriteBit(GPIO_DRIVER_CTRL_GPIO_PORT, GPIO_DRIVER_CTRL_CHANB_PIN, Bit_SET);
}


/*********************************************************************************************
函 数 名: bspGpioDriverCtrlChanASet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspGpioDriverCtrlChanASet(BitAction pinAction)
{
    GPIO_WriteBit(GPIO_DRIVER_CTRL_GPIO_PORT, GPIO_DRIVER_CTRL_CHANA_PIN, pinAction);
}


/*********************************************************************************************
函 数 名: bspGpioDriverCtrlChanBSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspGpioDriverCtrlChanBSet(BitAction pinAction)
{
    GPIO_WriteBit(GPIO_DRIVER_CTRL_GPIO_PORT, GPIO_DRIVER_CTRL_CHANB_PIN, pinAction);
}


/*********************************************************************************************
函 数 名: bspLedGpioInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspLedGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

  
    //使能GPIOE时钟
    RCC_AHB1PeriphClockCmd(GPIO_LED_GPIO_CLK, ENABLE);    

    //配置管脚
    GPIO_InitStructure.GPIO_Pin = GPIO_LED_GREEN_PIN | GPIO_LED_RED_PIN;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_LED_GPIO_PORT, &GPIO_InitStructure); 
    
    //GPIO_SetBits(GPIO_LED_GPIO_PORT, GPIO_LED_GREEN_PIN);
    //GPIO_SetBits(GPIO_LED_GPIO_PORT, GPIO_LED_RED_PIN);
}


/*********************************************************************************************
函 数 名: bspGpioGreenLedSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspGpioGreenLedSet(BitAction pinAction)
{
    GPIO_WriteBit(GPIO_LED_GPIO_PORT, GPIO_LED_GREEN_PIN, pinAction);
}


/*********************************************************************************************
函 数 名: bspGpioRedLedSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspGpioRedLedSet(BitAction pinAction)
{
    GPIO_WriteBit(GPIO_LED_GPIO_PORT, GPIO_LED_RED_PIN, pinAction);
}


/*********************************************************************************************
函 数 名: bspFpgaITGpioInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspFpgaITGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    EXTI_InitTypeDef EXTI_InitStructure;

 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(GPIO_FPGA_IT_GPIO_CLK, ENABLE);    //使能时钟

    //配置管脚
    GPIO_InitStructure.GPIO_Pin   = GPIO_FPGA_IT_PIN;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIO_FPGA_IT_GPIO_PORT, &GPIO_InitStructure);       
    SYSCFG_EXTILineConfig(GPIO_FPGA_IT_PORTSRC, GPIO_FPGA_IT_PINSRC);
    
    NVIC_InitStructure.NVIC_IRQChannel = GPIO_FPGA_IT_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    EXTI_InitStructure.EXTI_Line = GPIO_FPGA_IT_EXTILINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearFlag(GPIO_FPGA_IT_EXTILINE);
}


/*********************************************************************************************
函 数 名: bspModelGpioInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void bspModelGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

  
    //使能GPIOE时钟
    RCC_APB2PeriphClockCmd(GPIO_MODEL_GPIO_CLK, ENABLE);    

    //配置管脚
    GPIO_InitStructure.GPIO_Pin  = GPIO_MODEL_BIT1_PIN | GPIO_MODEL_BIT2_PIN | GPIO_MODEL_BIT3_PIN |
                                   GPIO_MODEL_BIT4_PIN | GPIO_MODEL_BIT5_PIN | GPIO_MODEL_BIT6_PIN;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    
    GPIO_Init(GPIO_MODEL_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_MODEL_BIT0_PIN; 
    GPIO_Init(GPIO_MODEL_0_GPIO_PORT, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_MODEL_BIT7_PIN; 
    GPIO_Init(GPIO_MODEL_7_GPIO_PORT, &GPIO_InitStructure);   
}


/*********************************************************************************************
函 数 名: bspModelGpioRead;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 bspModelGpioRead(void)
{
    BitByteUnion typeValue;


    typeValue.byteValue = 0;

    typeValue.bitFiled.bit7 = GPIO_ReadInputDataBit(GPIO_MODEL_7_GPIO_PORT, GPIO_MODEL_BIT7_PIN);
    
    typeValue.bitFiled.bit6 = GPIO_ReadInputDataBit(GPIO_MODEL_GPIO_PORT, GPIO_MODEL_BIT6_PIN);
    typeValue.bitFiled.bit5 = GPIO_ReadInputDataBit(GPIO_MODEL_GPIO_PORT, GPIO_MODEL_BIT5_PIN);
    typeValue.bitFiled.bit4 = GPIO_ReadInputDataBit(GPIO_MODEL_GPIO_PORT, GPIO_MODEL_BIT4_PIN);
    typeValue.bitFiled.bit3 = GPIO_ReadInputDataBit(GPIO_MODEL_GPIO_PORT, GPIO_MODEL_BIT3_PIN);
    typeValue.bitFiled.bit2 = GPIO_ReadInputDataBit(GPIO_MODEL_GPIO_PORT, GPIO_MODEL_BIT2_PIN);
    typeValue.bitFiled.bit1 = GPIO_ReadInputDataBit(GPIO_MODEL_GPIO_PORT, GPIO_MODEL_BIT1_PIN);
    
    typeValue.bitFiled.bit0 = GPIO_ReadInputDataBit(GPIO_MODEL_0_GPIO_PORT, GPIO_MODEL_BIT0_PIN);

    return typeValue.byteValue;
}



/*******************************************文件尾********************************************/
