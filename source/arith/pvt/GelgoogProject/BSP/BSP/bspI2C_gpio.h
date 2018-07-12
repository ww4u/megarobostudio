/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspI2C_gpio.h
��������:  
�����б�:  
��ǰ�汾:  1.0.0
�汾�Ķ�:  
��    ��:  xyzheng
�������:  2017.06.01
��ʷ�汾:  ��
*********************************************************************************************/
#include "stm32f4xx.h" 

#ifndef _DRV_I2C_GPIO_H
#define _DRV_I2C_GPIO_H

#define I2C_WR  0       /* д����bit */
#define I2C_RD  1       /* ������bit */

void bspI2C_init(void);
void bspI2C_Start(void);
void bspI2C_Stop(void);
void bspI2C_SendByte(u8 _ucByte);
u8 bspI2C_ReadByte(u8);
u8 bspI2C_WaitAck(void);
void bspI2C_Ack(void);
void bspI2C_NAck(void);
u8 bspI2C_CheckDevice(u8 _Address);

#endif