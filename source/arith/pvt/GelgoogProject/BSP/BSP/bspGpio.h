/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspGpio.h；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.25;
历史版本:  无;
**********************************************************************************************/
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H



/******************************************包含文件*******************************************/
#include "stm32f4xx.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
void bspFpgaProgGpioInit(void);
void bspGpioFpgaProgConfigSet(BitAction pinAction);
u8   bspGpioFpgaProgAnswerRead(void);
u8   bspGpioFpgaProgDoneRead(void);

void bspDriverGpioInit(void);
void bspGpioDriverCtrlChanASet(BitAction pinAction);
void bspGpioDriverCtrlChanBSet(BitAction pinAction);

void bspLedGpioInit(void);
void bspGpioGreenLedSet(BitAction pinAction);
void bspGpioRedLedSet(BitAction pinAction);

void bspFpgaITGpioInit(void);

void bspModelGpioInit(void);
u8   bspModelGpioRead(void);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
