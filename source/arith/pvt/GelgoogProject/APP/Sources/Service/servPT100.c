#include "project.h"
#include "servPT100.h"
#include "bspAdc.h"
#include "servSoftTimer.h"
#include "eventManageTask.h"



#define PT100_ADC_CH     ADC_Channel_11

#define    VOLT_TO_CODE(v)    (u16)(v * 40950 / 105.6)    //code = vi * (10 / 32) * 4095 / 3.3(V)

#ifdef PROJECT_QUBELEY
SoftTimerStruct g_analogInTimer;
#endif



/*********************************************************************************************
函 数 名:servGetPT100Value
实现功能:获取PT100的温度值  
输入参数: 无;
输出参数: 无;
返 回 值: 无;
*********************************************************************************************/
u32 servGetPT100Value()
{
    u16  value = 0;
    float current = 0.0,volt = 0.0,r = 0.0;
    u32  temp = 0;
    float vref = 3.3; /*参考电压*/
    float r1 = 4990.0;/*R1电阻值*/
    float r2=100.0; /*R2电阻值*/
    float g=90.0; /*放大器放大倍数*/
    value = drvAdcGetAverage(PT100_ADC_CH,1);/*获取ADC的采样值*/
    volt = vref*value/4096; /*则采样值换算出电压*/
    current = volt/(g*vref) + r2/(r1+r2)*1.0; /*计算PT100上通过的电流值*/
    
    r = current*r1/(1.0-current);/*计算PT100的阻值*/
    
    //temp = (r-100)*2.5;/*由阻值计算出温度*/
    //printf("temp:%f\n",temp);
    
    temp = (u32)((r - 100) * 250);    /*由阻值计算出温度*///返回值为整数，精度0.01°   CJ 2017.05.19 Modify
    
    return temp;
}


/*********************************************************************************************
函 数 名: servAnalogInSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servAnalogInSet(AnalogInfoStruct asensorInfo)
{
    asensorInfo.adcCodeH = VOLT_TO_CODE(asensorInfo.thresholdH);
    asensorInfo.adcCodeL = VOLT_TO_CODE(asensorInfo.thresholdL);
    
    if (asensorInfo.state)
    {
        bspAdc1ConvStart(asensorInfo.adcCodeH, asensorInfo.adcCodeL);
    }
    else
    {
        bspAdc1ConvStop();
        servStimerDelete(&g_analogInTimer);
    }
}


/*********************************************************************************************
函 数 名: servAdc1IntTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servAdc1IntTimerCB(void *timeOutPara)
{
    servStimerDelete(&g_analogInTimer);
    bspAdc1ConvEnable();    
}


/*********************************************************************************************
函 数 名: irqAdc1AWD;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqAdc1AWD(void)
{
    u16 threshold;

    
    threshold = ADC_GetConversionValue(ADC1);

    //关闭ADC转换，开启定时器，一定时间后再转换
    bspAdc1ConvStop();
    servStimerAdd(&g_analogInTimer);

    irqEventOccur(EVENTSRC_ANALOGIN, threshold);
}



/*******************************************文件尾********************************************/
