/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servLed.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.07.29;
历史版本:  无;
*********************************************************************************************/
#include "project.h"
#include "servLed.h"
#include "bspGpio.h"
#include "servSoftTimer.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/
typedef struct
{
    bool bEnableSTimer;    //是否开启软定时器，其实定时器一直开启中，只是设置是否继续执行
    bool bExectSet;        //是否执行设置
    
    u8 validIndex;
    u8 nextActIndex;

    BitAction greenLed[20];
    BitAction redLed[20];
    
}LedStateStreamStruct;


/******************************************局部变量*******************************************/
LedStateStreamStruct ledStateStream;
SoftTimerStruct g_ledTimer;



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servLedSet;
实现功能: LED设置; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servLedSet(LedStatusEnum ledState)
{
    switch (ledState)
    {
        case LED_POWERON:    //绿灯常亮
            ledStateStream.bEnableSTimer = false;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 1;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
          break;

        case LED_IDEL:    //绿灯闪烁
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 2;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_SET;
            ledStateStream.greenLed[1] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_RESET;
            ledStateStream.redLed[1]  = Bit_RESET;
          break;

        case LED_WORKING:    //两绿一红交替闪烁
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

        /*case LED_CALCEND:    //三绿一红交替闪烁
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

        case LED_STANDBY:    //四绿一红交替闪烁
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

        case LED_RUNNING:    //五绿一红交替闪烁
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

        case LED_ERROR:    //红灯常亮(后续加入不同的红的闪烁方式表示不同的错误)
            ledStateStream.bEnableSTimer = false;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 1;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_SET;
          break;

        case LED_ERROR1:    //红灯闪烁
            ledStateStream.bEnableSTimer = true;
            ledStateStream.bExectSet = true;
            
            ledStateStream.validIndex = 2;
            ledStateStream.nextActIndex = 0;
            
            ledStateStream.greenLed[0] = Bit_RESET;
            ledStateStream.greenLed[1] = Bit_RESET;
            
            ledStateStream.redLed[0]  = Bit_SET;
            ledStateStream.redLed[1]  = Bit_RESET;
          break;

        case LED_DEVICEID:    //一绿一红交替闪烁
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
函 数 名: servLedTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: servLedInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servLedInit(LedStatusEnum ledState)
{
    servLedSet(ledState);
    servLedTimerCB(NULL);
}


/*******************************************文件尾********************************************/
