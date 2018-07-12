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
�� �� ��:servGetPT100Value
ʵ�ֹ���:��ȡPT100���¶�ֵ  
�������: ��;
�������: ��;
�� �� ֵ: ��;
*********************************************************************************************/
u32 servGetPT100Value()
{
    u16  value = 0;
    float current = 0.0,volt = 0.0,r = 0.0;
    u32  temp = 0;
    float vref = 3.3; /*�ο���ѹ*/
    float r1 = 4990.0;/*R1����ֵ*/
    float r2=100.0; /*R2����ֵ*/
    float g=90.0; /*�Ŵ����Ŵ���*/
    value = drvAdcGetAverage(PT100_ADC_CH,1);/*��ȡADC�Ĳ���ֵ*/
    volt = vref*value/4096; /*�����ֵ�������ѹ*/
    current = volt/(g*vref) + r2/(r1+r2)*1.0; /*����PT100��ͨ���ĵ���ֵ*/
    
    r = current*r1/(1.0-current);/*����PT100����ֵ*/
    
    //temp = (r-100)*2.5;/*����ֵ������¶�*/
    //printf("temp:%f\n",temp);
    
    temp = (u32)((r - 100) * 250);    /*����ֵ������¶�*///����ֵΪ����������0.01��   CJ 2017.05.19 Modify
    
    return temp;
}


/*********************************************************************************************
�� �� ��: servAnalogInSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: servAdc1IntTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servAdc1IntTimerCB(void *timeOutPara)
{
    servStimerDelete(&g_analogInTimer);
    bspAdc1ConvEnable();    
}


/*********************************************************************************************
�� �� ��: irqAdc1AWD;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void irqAdc1AWD(void)
{
    u16 threshold;

    
    threshold = ADC_GetConversionValue(ADC1);

    //�ر�ADCת����������ʱ����һ��ʱ�����ת��
    bspAdc1ConvStop();
    servStimerAdd(&g_analogInTimer);

    irqEventOccur(EVENTSRC_ANALOGIN, threshold);
}



/*******************************************�ļ�β********************************************/
