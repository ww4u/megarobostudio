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



/*******************************************�궨��********************************************/
#define    ENCODER_SUPPORT_NUM    4



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



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
