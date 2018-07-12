/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名: bspAdc.c；
功能描述:     ;
函数列表:     ;
当前版本:     1.0.0;
版本改动:     ;
作     者:  CJ;
完成日期:     2017.10.30;
历史版本:     无;
**********************************************************************************************/
#include "bspAdc.h"
#include "bspTimer.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    ADC_HARDVER_ACD_PORT_VOLT_MAX     3300    //mV
#define    ADC_HARDVER_ACD_DR_MAX            4095    //12位ADC

#define    ADC_HARDVER_ACD                   ADC1
#define    ADC_DIGITAL_HARDVER_ACD_CHAN      ADC_Channel_13
#define    ADC_DRIVER_HARDVER_ACD_CHAN_DN    ADC_Channel_11
#define    ADC_DRIVER_HARDVER_ACD_CHAN_UP    ADC_Channel_12

#define    ADC_HARDVER_ACD_CLOCK             RCC_APB2Periph_ADC1

#define    ADC_HARDVER_ACD_GPIO_PORT         GPIOC
#define    ADC_HARDVER_ACD_GPIO_CLK          RCC_AHB1Periph_GPIOC

#define    ADC_DATA_TIMEOUT      ((uint32_t)0x200B200)    //根据DELAY_COUNT_MS = 33600，此超时时间大约为1s



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: bspHardVerAdcInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void bspHardVerAdcInit(void)
{
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;


    /****************ADC1 GPIO配置*****************************************/
    RCC_AHB1PeriphClockCmd(ADC_HARDVER_ACD_GPIO_CLK, ENABLE);

    //配置PC.02(ADC Channel12)为模拟输入
    GPIO_InitStructure.GPIO_Pin = ADC_DIGITAL_HARDVER_ACD_CHAN | ADC_DRIVER_HARDVER_ACD_CHAN_DN | ADC_DRIVER_HARDVER_ACD_CHAN_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(ADC_HARDVER_ACD_GPIO_PORT, &GPIO_InitStructure);


    /****************ADC1 配置*****************************************/
    /****************************************************************************
    PCLK2  = 84M
    ADCCLK = PCLK2 / 2 = 42M
    ADC采样频率： Sampling Time + Conversion Time = 480 + 12 cycles = 492cyc
                  Conversion Time = 42MHz / 492cyc = 85.36ksps. 
    *****************************************************************************/
    RCC_APB2PeriphClockCmd(ADC_HARDVER_ACD_CLOCK, ENABLE);
    
    /* ADC Common 配置 ----------------------------------------------------------*/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC_HARDVER_ACD, &ADC_InitStructure);

    /*ADC_RegularChannelConfig(ADC_HARDVER_ACD, ADC_DIGITAL_HARDVER_ACD_CHAN,   1, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC_HARDVER_ACD, ADC_DRIVER_HARDVER_ACD_CHAN_DN, 2, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC_HARDVER_ACD, ADC_DRIVER_HARDVER_ACD_CHAN_UP, 3, ADC_SampleTime_480Cycles);*/

    /* Enable ADC1 **************************************************************/
    ADC_Cmd(ADC_HARDVER_ACD, ENABLE);

    /* Start ADC1 Software Conversion */
    //ADC_SoftwareStartConv(ADC_HARDVER_ACD);
}


/*********************************************************************************************
函 数 名: bspDigitalHardVerVoltGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
u16 bspDigitalHardVerVoltGet(void)
{
    u32 timeout = ADC_DATA_TIMEOUT;

    
    ADC_RegularChannelConfig(ADC_HARDVER_ACD, ADC_DIGITAL_HARDVER_ACD_CHAN, 1, ADC_SampleTime_480Cycles);
    ADC_Cmd(ADC_HARDVER_ACD, ENABLE);

    ADC_SoftwareStartConv(ADC_HARDVER_ACD);
    
    while ((!ADC_GetFlagStatus(ADC_HARDVER_ACD, ADC_FLAG_EOC)) && (timeout > 0))    //等待转换结束
    {
        timeout--;  
    }

    ADC_Cmd(ADC_HARDVER_ACD, DISABLE);

    if (0 == timeout)
    {
        return 0;
    }
    else
    {
        return ADC_GetConversionValue(ADC_HARDVER_ACD);    //返回最近一次ADC规则组的转换结果
    }
}


/*********************************************************************************************
函 数 名: bspDigitalHardVerAverVoltGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
u16 bspDigitalHardVerAverVoltGet(u8 ConvTimes)
{
    u16 i;
    u16 portVolt;
    u32 tempValue = 0;


    for (i = 0;i < ConvTimes;i++)
    {
        tempValue += bspDigitalHardVerVoltGet();
    }
    tempValue /= ConvTimes;
    portVolt = tempValue * ADC_HARDVER_ACD_PORT_VOLT_MAX / ADC_HARDVER_ACD_DR_MAX;

    return portVolt;
}


/*********************************************************************************************
函 数 名: bspDrvUpHardVerVoltGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
u16 bspDrvUpHardVerVoltGet(void)
{
    u32 timeout = ADC_DATA_TIMEOUT;

    
    ADC_RegularChannelConfig(ADC_HARDVER_ACD, ADC_DRIVER_HARDVER_ACD_CHAN_UP, 1, ADC_SampleTime_480Cycles);
    ADC_Cmd(ADC_HARDVER_ACD, ENABLE);

    ADC_SoftwareStartConv(ADC_HARDVER_ACD);
    
    while ((!ADC_GetFlagStatus(ADC_HARDVER_ACD, ADC_FLAG_EOC)) && (timeout > 0))    //等待转换结束
    {
        timeout--;  
    }

    ADC_Cmd(ADC_HARDVER_ACD, DISABLE);

    if (0 == timeout)
    {
        return ADC_HARDVER_ACD_PORT_VOLT_MAX;
    }
    else
    {
        return ADC_GetConversionValue(ADC_HARDVER_ACD);    //返回最近一次ADC规则组的转换结果
    }
}


/*********************************************************************************************
函 数 名: bspDrvUpHardVerAverVoltGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
u16 bspDrvUpHardVerAverVoltGet(u8 ConvTimes)
{
    u16 i;
    u16 portVolt;
    u32 tempValue = 0;


    for (i = 0;i < ConvTimes;i++)
    {
        tempValue += bspDrvUpHardVerVoltGet();
    }
    tempValue /= ConvTimes;
    portVolt = tempValue * ADC_HARDVER_ACD_PORT_VOLT_MAX / ADC_HARDVER_ACD_DR_MAX;

    return portVolt;
}


/*********************************************************************************************
函 数 名: bspDrvDnHardVerVoltGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
u16 bspDrvDnHardVerVoltGet(void)
{
    u32 timeout = ADC_DATA_TIMEOUT;


    ADC_RegularChannelConfig(ADC_HARDVER_ACD, ADC_DRIVER_HARDVER_ACD_CHAN_DN, 1, ADC_SampleTime_480Cycles);
    ADC_Cmd(ADC_HARDVER_ACD, ENABLE);

    ADC_SoftwareStartConv(ADC_HARDVER_ACD);
    
    while ((!ADC_GetFlagStatus(ADC_HARDVER_ACD, ADC_FLAG_EOC)) && (timeout > 0))    //等待转换结束
    {
        timeout--;  
    }
    
    ADC_Cmd(ADC_HARDVER_ACD, DISABLE);

    if (0 == timeout)
    {
        return ADC_HARDVER_ACD_PORT_VOLT_MAX;
    }
    else
    {
        return ADC_GetConversionValue(ADC_HARDVER_ACD);    //返回最近一次ADC规则组的转换结果
    }
}


/*********************************************************************************************
函 数 名: bspDrvDnHardVerAverVoltGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
u16 bspDrvDnHardVerAverVoltGet(u8 ConvTimes)
{
    u16 i;
    u16 portVolt;
    u32 tempValue = 0;


    for (i = 0;i < ConvTimes;i++)
    {
        tempValue += bspDrvDnHardVerVoltGet();
    }
    tempValue /= ConvTimes;
    portVolt = tempValue * ADC_HARDVER_ACD_PORT_VOLT_MAX / ADC_HARDVER_ACD_DR_MAX;

    return portVolt;
}



/*******************************************文件尾********************************************/
