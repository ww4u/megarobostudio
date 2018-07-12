/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfMainVerify.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "pvrfMainVerify.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
ParaLimitStruct g_paraLimit;



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfParaLimitInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void pvrfParaLimitInit(void)
{    
    //����ϵͳ�ͺŶԲ��������޽��и�ֵ
    /*switch (cmdMainType)
    {
        case 0:
          break;

        default:
          break;
    }*/
    
    //����
    g_paraLimit.upLimit.otpThr = 150;
    g_paraLimit.upLimit.canGroup = 200;
    g_paraLimit.upLimit.canSendId = 0x1FFFFFFF;
    g_paraLimit.upLimit.canReceiveId = 0x1FFFFFFF;
    g_paraLimit.upLimit.canGroupId = 0x1FFFFFFF;
    g_paraLimit.upLimit.canRadioId = 0x1FFFFFFF;
    g_paraLimit.upLimit.motorGearRatio = 250;
    g_paraLimit.upLimit.motorVolt = 250;
    g_paraLimit.upLimit.motorCurr = 250;
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
    g_paraLimit.upLimit.fpgaPwmClock = FPGA_PWM_CLOCK + 2000;    //�ݶ���200Hz��ƫ�Χ    CJ 2017.04.12 Add

    g_paraLimit.upLimit.snUartFrameLen = 128;
    g_paraLimit.upLimit.snUartSwTime = 3600000;
    g_paraLimit.upLimit.snUartRecvNum  = 5;

    g_paraLimit.upLimit.motorBacklash = 360;
    g_paraLimit.upLimit.encoderLineNum = 100000;
    g_paraLimit.upLimit.feedbackRatio = 255;

    g_paraLimit.upLimit.speedScale = 500;
    
    
    //����
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
    
    g_paraLimit.downLimit.initPostValue = -1000;  //�ݶ�  cp 
    g_paraLimit.downLimit.outOfStepNum = 0;
    g_paraLimit.downLimit.reportPeriod = 10;   
    g_paraLimit.downLimit.asensorThr = 0;
    g_paraLimit.downLimit.motionMinSpeed = 0;
    g_paraLimit.downLimit.motionMinPosn = 0;
    g_paraLimit.downLimit.motionMinTorque = 0;
    g_paraLimit.downLimit.inchingTimeValue = 0.1;
    g_paraLimit.downLimit.inchingDisValue = -1000;
    g_paraLimit.downLimit.trigInPeriod = 0.000025;
    g_paraLimit.downLimit.fpgaPwmClock = FPGA_PWM_CLOCK - 20000;    //�ݶ���200Hz��ƫ�Χ    CJ 2017.04.12 Add

    g_paraLimit.downLimit.snUartFrameLen = 3;    //��������ֽ�(֡ͷ�����ȣ�����)
    g_paraLimit.downLimit.snUartSwTime = 30;
    g_paraLimit.downLimit.snUartRecvNum  = 1;

    g_paraLimit.downLimit.motorBacklash = 0.0;
    g_paraLimit.downLimit.encoderLineNum = 1;
    g_paraLimit.downLimit.feedbackRatio = 0;

    g_paraLimit.downLimit.speedScale = 1;
}



/*******************************************�ļ�β********************************************/
