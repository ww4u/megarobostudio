/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspTimer.h；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.25;
历史版本:  无;
**********************************************************************************************/
#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H



/******************************************包含文件*******************************************/
#include "stm32f4xx.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/
/*--------------编码器倍乘枚举----------------*/
typedef enum
{
    ENCMULT_SINGLE = 0,    //没有倍乘
    ENCMULT_DOUBLE,        //X2
    ENCMULT_QUADR          //X4
    
}EncoderMultiEnum;

/*--------------编码器倍乘枚举----------------*/
typedef enum
{
    ENCDIR_P = 0,    //正向旋转
    ENCDIR_N,        //负向
    
}EncoderDirEnum;



/*******************************************宏定义********************************************/
#define    ENCODER_SUPPORT_NUM    4



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
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



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
