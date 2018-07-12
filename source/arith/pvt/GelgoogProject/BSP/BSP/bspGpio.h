/**********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspGpio.h��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.25;
��ʷ�汾:  ��;
**********************************************************************************************/
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H



/******************************************�����ļ�*******************************************/
#include "stm32f4xx.h"



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/



/*******************************************�궨��********************************************/



/******************************************��������*******************************************/



/******************************************��������*******************************************/
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



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
