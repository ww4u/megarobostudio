/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspI2C_gpio.h
功能描述:  
函数列表:  
当前版本:  1.0.0
版本改动:  
作    者:  xyzheng
完成日期:  2017.06.01
历史版本:  无
*********************************************************************************************/
#include "stm32f4xx.h" 

#ifndef _DRV_I2C_GPIO_H
#define _DRV_I2C_GPIO_H

#define I2C_WR  0       /* 写控制bit */
#define I2C_RD  1       /* 读控制bit */

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