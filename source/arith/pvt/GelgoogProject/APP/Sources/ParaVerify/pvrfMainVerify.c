/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfMainVerify.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#include "pvrfMainVerify.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
ParaLimitStruct g_paraLimit;



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfParaLimitInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void pvrfParaLimitInit(void)
{    
    //根据系统型号对参数上下限进行赋值
    /*switch (cmdMainType)
    {
        case 0:
          break;

        default:
          break;
    }*/
    
    //上限
    g_paraLimit.upLimit.otpThr = 150;
    g_paraLimit.upLimit.canGroup = 200;
    g_paraLimit.upLimit.canSendId = 0x1FFFFFFF;
    g_paraLimit.upLimit.canReceiveId = 0x1FFFFFFF;
    g_paraLimit.upLimit.canGroupId = 0x1FFFFFFF;
    g_paraLimit.upLimit.canRadioId = 0x1FFFFFFF;
    g_paraLimit.upLimit.motorGearRatio = 250;
    g_paraLimit.upLimit.motorVolt = 250;
    
#if GELGOOG_AXIS_10
    g_paraLimit.upLimit.motorCurr = 15;    //最大1.5A
#elif QUBELEY_HARDVER_1
    g_paraLimit.upLimit.motorCurr = 80;    //最大8A
#else
    g_paraLimit.upLimit.motorCurr = 50;    //最大5A    
#endif

    g_paraLimit.upLimit.motorPeakSpeed = 1024000;
    g_paraLimit.upLimit.motorLead = 1024000;
    g_paraLimit.upLimit.pvtStartPoint = PVT_POINT_BUFFER_SIZE - 1;
    g_paraLimit.upLimit.pvtEndPoint = PVT_POINT_BUFFER_SIZE - 1;
    g_paraLimit.upLimit.pvtNcycles = U32_VALUE_MAX - 1;  
    g_paraLimit.upLimit.pvtWarnPoint = PVT_POINT_BUFFER_SIZE - 1;
    g_paraLimit.upLimit.pvtPosn = 500000.0;
    g_paraLimit.upLimit.pvtSpeed = 1024000.0;
    g_paraLimit.upLimit.pvtTime = 500000.0;
    
    g_paraLimit.upLimit.stopTime = 500000.0;
    g_paraLimit.upLimit.stopDistance = 500000.0;
    g_paraLimit.upLimit.endSpeed = 500000.0;
    
    g_paraLimit.upLimit.initPostValue = 500000.0;
    g_paraLimit.upLimit.outOfStepNum = 1024000; 
    g_paraLimit.upLimit.reportPeriod = 3600000;
    g_paraLimit.upLimit.asensorThr = 10.0;
    g_paraLimit.upLimit.motionMaxSpeed = 1024000;
    g_paraLimit.upLimit.motionMaxPosn = 500000;
    g_paraLimit.upLimit.motionMaxTorque = 500000;
    g_paraLimit.upLimit.inchingTimeValue = 1000;
    g_paraLimit.upLimit.inchingDisValue = 1000;
    g_paraLimit.upLimit.trigInPeriod = 6000;
    g_paraLimit.upLimit.fpgaPwmClock = FPGA_PWM_CLOCK + 2000;    //暂定±200Hz的偏差范围    CJ 2017.04.12 Add

    g_paraLimit.upLimit.snUartFrameLen = 128;
    g_paraLimit.upLimit.snUartSwTime = 3600000;
    g_paraLimit.upLimit.snUartRecvNum  = 5;

    g_paraLimit.upLimit.motorBacklash = 360;
    g_paraLimit.upLimit.encoderLineNum = 100000;
    g_paraLimit.upLimit.feedbackRatio = 255;

    g_paraLimit.upLimit.sgLimit = 101;
    
    g_paraLimit.upLimit.lineOutNum = 32768;    //占比不做验证了，丢步的上限需要验证

    g_paraLimit.upLimit.switchTime = 10000;

    g_paraLimit.upLimit.fanDuty = 100;

    g_paraLimit.upLimit.absEncValue = 262143;    //0x3FFFF

    g_paraLimit.upLimit.alarmDist = 1000;     //1000mm
    
    g_paraLimit.upLimit.ledFlickerFreq = 5000;    //5000ms
    
    //下限
    g_paraLimit.downLimit.otpThr = 0;
    g_paraLimit.downLimit.canGroup = 0;
    g_paraLimit.downLimit.canSendId = 0;
    g_paraLimit.downLimit.canReceiveId = 0;
    g_paraLimit.downLimit.canGroupId = 0;
    g_paraLimit.downLimit.canRadioId = 0;
    g_paraLimit.downLimit.motorGearRatio = 1;
    g_paraLimit.downLimit.motorVolt = 0;
    g_paraLimit.downLimit.motorCurr = 0;
    g_paraLimit.downLimit.motorLead = 0;
    g_paraLimit.downLimit.pvtStartPoint = 0;
    g_paraLimit.downLimit.pvtEndPoint = 0;
    g_paraLimit.downLimit.pvtNcycles = 0;
    g_paraLimit.downLimit.pvtWarnPoint = 0;
    g_paraLimit.downLimit.pvtPosn = -500000.0;
    g_paraLimit.downLimit.pvtSpeed = -1024000.0;
    g_paraLimit.downLimit.pvtTime = 0;
    
    g_paraLimit.downLimit.stopTime = 0;
    g_paraLimit.downLimit.stopDistance = 0;
    g_paraLimit.downLimit.endSpeed = 0;
    
    g_paraLimit.downLimit.initPostValue = -1000;  //暂定  cp 
    g_paraLimit.downLimit.outOfStepNum = 0;
    g_paraLimit.downLimit.reportPeriod = 10;   
    g_paraLimit.downLimit.asensorThr = 0;
    g_paraLimit.downLimit.motionMinSpeed = 0;
    g_paraLimit.downLimit.motionMinPosn = 0;
    g_paraLimit.downLimit.motionMinTorque = 0;
    g_paraLimit.downLimit.inchingTimeValue = 0.1;
    g_paraLimit.downLimit.inchingDisValue = -1000;
    g_paraLimit.downLimit.trigInPeriod = 0.000025;
    g_paraLimit.downLimit.fpgaPwmClock = FPGA_PWM_CLOCK - 20000;    //暂定±200Hz的偏差范围    CJ 2017.04.12 Add

    g_paraLimit.downLimit.snUartFrameLen = 3;    //最短三个字节(帧头，长度，数据)
    g_paraLimit.downLimit.snUartSwTime = 30;
    g_paraLimit.downLimit.snUartRecvNum  = 1;

    g_paraLimit.downLimit.motorBacklash = 0.0;
    g_paraLimit.downLimit.encoderLineNum = 1;
    g_paraLimit.downLimit.feedbackRatio = 0;

    g_paraLimit.downLimit.sgLimit = -1;

    g_paraLimit.downLimit.speedScale = 1;
    
    g_paraLimit.downLimit.switchTime = 0;
    
    g_paraLimit.downLimit.absEncValue = 0;

    g_paraLimit.downLimit.alarmDist = 10;     //10mm

#if GELGOOG_SINANJU    
    g_paraLimit.downLimit.ledFlickerFreq = LED_FLICKER_FREQ_MIN;    //100ms
#endif
}



/*******************************************文件尾********************************************/
