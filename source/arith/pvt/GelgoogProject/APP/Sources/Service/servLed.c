/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servLed.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.07.29;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "project.h"
#include "servLed.h"
#include "bspGpio.h"
#include "servSoftTimer.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/
typedef struct
{
    bool bEnableSTimer;    //�Ƿ�����ʱ������ʵ��ʱ��һֱ�����У�ֻ�������Ƿ����ִ��
    bool bExectSet;        //�Ƿ�ִ������
    
    u8 validIndex;
    u8 nextActIndex;

    BitAction greenLed[20];
    BitAction redLed[20];
    
}LedStateStreamStruct;


/******************************************�ֲ�����*******************************************/
LedStateStreamStruct ledStateStream;
SoftTimerStruct g_ledTimer;



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: servLedSet;
ʵ�ֹ���: LED����; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servLedSet(LedStatusEnum ledState)
{
    switch (ledState)
    {
        case LED_POWERON:    //�̵Ƴ���
            ledStateStream.bEnableSTimer = false;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 1;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
          break;

        case LED_IDEL:    //�̵���˸
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 2;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            ledStateStream.greenLed[1] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
            ledStateStream.redLed[1]  = Bit_RESET;
          break;

        case LED_WORKING:    //����һ�콻����˸
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 6;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            ledStateStream.greenLed[1] = Bit_RESET;
            ledStateStream.greenLed[2] = Bit_SET;
            ledStateStream.greenLed[3] = Bit_RESET;
            ledStateStream.greenLed[4] = Bit_RESET;
            ledStateStream.greenLed[5] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
            ledStateStream.redLed[1]  = Bit_RESET;
            ledStateStream.redLed[2]  = Bit_RESET;
            ledStateStream.redLed[3]  = Bit_RESET;
            ledStateStream.redLed[4]  = Bit_SET;
            ledStateStream.redLed[5]  = Bit_RESET;
          break;

        /*case LED_CALCEND:    //����һ�콻����˸
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 8;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            ledStateStream.greenLed[1] = Bit_RESET;
            ledStateStream.greenLed[2] = Bit_SET;
            ledStateStream.greenLed[3] = Bit_RESET;
            ledStateStream.greenLed[4] = Bit_SET;
            ledStateStream.greenLed[5] = Bit_RESET;
            ledStateStream.greenLed[6] = Bit_RESET;
            ledStateStream.greenLed[7] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
            ledStateStream.redLed[1]  = Bit_RESET;
            ledStateStream.redLed[2]  = Bit_RESET;
            ledStateStream.redLed[3]  = Bit_RESET;
            ledStateStream.redLed[4]  = Bit_RESET;
            ledStateStream.redLed[5]  = Bit_RESET;
            ledStateStream.redLed[6]  = Bit_SET;
            ledStateStream.redLed[7]  = Bit_RESET;
          break;

        case LED_STANDBY:    //����һ�콻����˸
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 10;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            ledStateStream.greenLed[1] = Bit_RESET;
            ledStateStream.greenLed[2] = Bit_SET;
            ledStateStream.greenLed[3] = Bit_RESET;
            ledStateStream.greenLed[4] = Bit_SET;
            ledStateStream.greenLed[5] = Bit_RESET;
            ledStateStream.greenLed[6] = Bit_SET;
            ledStateStream.greenLed[7] = Bit_RESET;
            ledStateStream.greenLed[8] = Bit_RESET;
            ledStateStream.greenLed[9] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
            ledStateStream.redLed[1]  = Bit_RESET;
            ledStateStream.redLed[2]  = Bit_RESET;
            ledStateStream.redLed[3]  = Bit_RESET;
            ledStateStream.redLed[4]  = Bit_RESET;
            ledStateStream.redLed[5]  = Bit_RESET;
            ledStateStream.redLed[6]  = Bit_RESET;
            ledStateStream.redLed[7]  = Bit_RESET;
            ledStateStream.redLed[8]  = Bit_SET;
            ledStateStream.redLed[9]  = Bit_RESET;
          break;

        case LED_RUNNING:    //����һ�콻����˸
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 12;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            ledStateStream.greenLed[1] = Bit_RESET;
            ledStateStream.greenLed[2] = Bit_SET;
            ledStateStream.greenLed[3] = Bit_RESET;
            ledStateStream.greenLed[4] = Bit_SET;
            ledStateStream.greenLed[5] = Bit_RESET;
            ledStateStream.greenLed[6] = Bit_SET;
            ledStateStream.greenLed[7] = Bit_RESET;
            ledStateStream.greenLed[8] = Bit_SET;
            ledStateStream.greenLed[9] = Bit_RESET;
            ledStateStream.greenLed[10] = Bit_RESET;
            ledStateStream.greenLed[11] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
            ledStateStream.redLed[1]  = Bit_RESET;
            ledStateStream.redLed[2]  = Bit_RESET;
            ledStateStream.redLed[3]  = Bit_RESET;
            ledStateStream.redLed[4]  = Bit_RESET;
            ledStateStream.redLed[5]  = Bit_RESET;
            ledStateStream.redLed[6]  = Bit_RESET;
            ledStateStream.redLed[7]  = Bit_RESET;
            ledStateStream.redLed[8]  = Bit_RESET;
            ledStateStream.redLed[9]  = Bit_RESET;
            ledStateStream.redLed[10]  = Bit_SET;
            ledStateStream.redLed[11]  = Bit_RESET;
          break;*/

        case LED_ERROR:    //��Ƴ���(�������벻ͬ�ĺ����˸��ʽ��ʾ��ͬ�Ĵ���)
            ledStateStream.bEnableSTimer = false;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 1;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_SET;
          break;

        case LED_ERROR1:    //�����˸
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 2;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_RESET;
            ledStateStream.greenLed[1] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_SET;
            ledStateStream.redLed[1]  = Bit_RESET;
          break;

        case LED_DEVICEID:    //һ��һ�콻����˸
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 4;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            ledStateStream.greenLed[1] = Bit_RESET;
            ledStateStream.greenLed[2] = Bit_RESET;
            ledStateStream.greenLed[3] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
            ledStateStream.redLed[1]  = Bit_RESET;
            ledStateStream.redLed[2]  = Bit_SET;
            ledStateStream.redLed[3]  = Bit_RESET;
          break;

        default:
          break;
    }
}


/*********************************************************************************************
�� �� ��: servLedTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servLedTimerCB(void *timeOutPara)
{
    if (ledStateStream.bExectSet)
    {
#ifdef PROJECT_GELGOOG
        bspGpioGreenLedSet((BitAction)(Bit_SET - ledStateStream.greenLed[ledStateStream.nextActIndex]));
        bspGpioRedLedSet((BitAction)(Bit_SET - ledStateStream.redLed[ledStateStream.nextActIndex]));
#endif
        
#ifdef PROJECT_QUBELEY
        bspGpioGreenLedSet(ledStateStream.greenLed[ledStateStream.nextActIndex]);
        bspGpioRedLedSet(ledStateStream.redLed[ledStateStream.nextActIndex]);
#endif

        ledStateStream.nextActIndex++;
        if (ledStateStream.nextActIndex >= ledStateStream.validIndex)
        {
            ledStateStream.nextActIndex = 0;
        }

        if (ledStateStream.bEnableSTimer)
        {
            ledStateStream.bExectSet = true;
        }
    }
    
    servStimerAdd(&g_ledTimer);
}


/*********************************************************************************************
�� �� ��: servLedInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servLedInit(LedStatusEnum ledState)
{
    servLedSet(ledState);
    servLedTimerCB(NULL);
}


/*******************************************�ļ�β********************************************/
