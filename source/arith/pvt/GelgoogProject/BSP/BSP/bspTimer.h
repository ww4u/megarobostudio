/**********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspTimer.h��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.25;
��ʷ�汾:  ��;
**********************************************************************************************/
#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H



/******************************************�����ļ�*******************************************/
#include "stm32f4xx.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/
/*--------------����������ö��----------------*/
typedef enum
{
    ENCMULT_SINGLE = 0,    //û�б���
    ENCMULT_DOUBLE,        //X2
    ENCMULT_QUADR          //X4
    
}EncoderMultiEnum;

/*--------------����������ö��----------------*/
typedef enum
{
    ENCDIR_P = 0,    //������ת
    ENCDIR_N,        //����
    
}EncoderDirEnum;

/*--------------ARM LEDö��----------------*/
typedef enum
{
    ARMLED_BASE = 0,    //����LED
    ARMLED_BIGARM,      //���
    ARMLED_SMALLARM,    //С��
    ARMLED_WRIST,       //��
    
    ARMLED_RESERVE
    
}ArmLedEnum;



/*******************************************�궨��********************************************/
#define    ENCODER_SUPPORT_NUM       4

#define    FAN_CTRL_TIME_FREQUENCY   21000000    //84000000Hz 4��Ƶ

#define    DRIVER_CTRL_TIME_FREQUENCY   2100000    //168000000Hz 80��Ƶ

#define    BASE_LED_CTRL_TIME_FREQUENCY        2100000
#define    BIGARM_LED_CTRL_TIME_FREQUENCY      2100000    //168000000Hz 80��Ƶ
#define    SMALLARM_LED_CTRL_TIME_FREQUENCY    2100000    //84000000Hz  40��Ƶ
#define    WRIST_LED_CTRL_TIME_FREQUENCY       2100000



/******************************************��������*******************************************/



/******************************************��������*******************************************/
void bspIWDGInit(u16 timeout);
void bspIWDGFeed(void);
void bspSoftReset(void);
void bspDelayMs(u32 milliSeconds);
void bspDelayUs(u32 microSeconds);

void bspEncoderTimerInit(u8 chanNum, EncoderMultiEnum encMult, FunctionalState NewState);
void bspEncoderCountSet(u8 chanNum, u16 count);
s32  bspEncoderABCountGet(u8 chanNum);
s16  bspEncoderCircleCountGet(u8 chanNum);
void irqChanZProcess(u8 chanNum);
void irqChanABProcess(u8 chanNum, EncoderDirEnum encoderDir);

void bspFanTimerInit(void);
void bspFanTimerSet(u32 frequency, u8 duty);

void bspArmLedTimerInit(void);
void bspArmLedTimerSet(ArmLedEnum armLed, u32 frequency, u8 duty);

void bspDriverTimerInit(void);
void bspDriverTimerSet(u32 frequency, u8 duty);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
