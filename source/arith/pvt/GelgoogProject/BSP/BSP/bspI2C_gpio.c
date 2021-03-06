/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  drvI2C_gpio.h
功能描述:  
函数列表:  
当前版本:  1.0.0
版本改动:  
作    者:  xyzheng
完成日期:  2017.06.01
历史版本:  无
*********************************************************************************************/
#include "bspI2C_gpio.h"
//#include "bspTimer.h"
/*
     i2c总线GPIO:
        PH4/I2C2_SCL
        PH5/I2C2_SDA
*/
/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C   GPIOB           /* GPIO端口 */
#define RCC_I2C_PORT    RCC_AHB1Periph_GPIOB        /* GPIO端口时钟 */
#define I2C_SCL_PIN     GPIO_Pin_6          /* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN     GPIO_Pin_7          /* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏 */
#define I2C_SCL_1()  GPIO_PORT_I2C->BSRRL = I2C_SCL_PIN             /* SCL = 1 */
#define I2C_SCL_0()  GPIO_PORT_I2C->BSRRH = I2C_SCL_PIN             /* SCL = 0 */

#define I2C_SDA_1()  GPIO_PORT_I2C->BSRRL = I2C_SDA_PIN             /* SDA = 1 */
#define I2C_SDA_0()  GPIO_PORT_I2C->BSRRH = I2C_SDA_PIN             /* SDA = 0 */

#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)   /* 读SDA口线状态 */
#define I2C_SCL_READ()  ((GPIO_PORT_I2C->IDR & I2C_SCL_PIN) != 0)   /* 读SCL口线状态 */

/*********************************************************************************************
函 数 名: bspI2C_init
实现功能: 配置I2C总线的GPIO，采用模拟IO的方式实现
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspI2C_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_I2C_PORT, ENABLE);   /* 打开GPIO时钟 */

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       /* 设为输出口 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;      /* 设为开漏模式 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    /* 上下拉电阻不使能 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  /* IO口最大速度 */

    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

    /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
    bspI2C_Stop();
}

/*********************************************************************************************
函 数 名: i2c_Delay
实现功能: I2C总线位延迟，最快400KHz
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
static void i2c_Delay(void)
{
    uint8_t i;
    /*　
        CPU主频168MHz时，在内部Flash运行。
        循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
        循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
        循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us
        上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us
        实际应用选择400KHz左右的速率即可
    */
    for (i = 0; i < 40; i++);
}
/*********************************************************************************************
函 数 名: i2c_Start
实现功能: CPU发起I2C总线启动信号
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspI2C_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    I2C_SDA_1();
    I2C_SCL_1();
    i2c_Delay();
    I2C_SDA_0();
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
}

/*********************************************************************************************
函 数 名: i2c_Stop
实现功能: CPU发起I2C总线停止信号
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspI2C_Stop()
{
      /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
      I2C_SDA_0();
      I2C_SCL_1();
      i2c_Delay();
      I2C_SDA_1();
}

/*********************************************************************************************
函 数 名:i2c_SendByte 
实现功能: CPU向I2C总线设备发送8bit数据
输入参数: _ucByte ： 等待发送的字节
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspI2C_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            I2C_SDA_1();
        }
        else
        {
            I2C_SDA_0();
        }
        i2c_Delay();
        I2C_SCL_1();
        i2c_Delay();
        I2C_SCL_0();
        if (i == 7)
        {
             I2C_SDA_1(); // 释放总线
        }
        _ucByte <<= 1;  /* 左移一个bit */
        i2c_Delay();
    }
}
/*********************************************************************************************
函 数 名: i2c_ReadByte
实现功能: CPU从I2C总线设备读取8bit数据
输入参数: 无;
输出参数: 无;
返 回 值: 读到的数据
说    明: 无;
*********************************************************************************************/
uint8_t bspI2C_ReadByte(u8 ack)
{
    uint8_t i;
    uint8_t value;

    
    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        I2C_SCL_1();
        i2c_Delay();
        if (I2C_SDA_READ())
        {
            value++;
        }
        I2C_SCL_0();
        i2c_Delay();
    }
    if (!ack)
    {
        bspI2C_NAck();//发送nACK
    } 
    else
    {
        bspI2C_Ack(); //发送ACK 
    }
    return value;
}

/*********************************************************************************************
函 数 名: i2c_WaitAck
实现功能: CPU产生一个时钟，并读取器件的ACK应答信号
输入参数: 无;
输出参数: 无;
返 回 值: 返回0表示正确应答，1表示无器件响应
说    明: 无;
*********************************************************************************************/
uint8_t bspI2C_WaitAck(void)
{
    uint8_t re;

    I2C_SDA_1();    /* CPU释放SDA总线 */
    i2c_Delay();
    I2C_SCL_1();    /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    i2c_Delay();
    if (I2C_SDA_READ()) /* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    I2C_SCL_0();
    i2c_Delay();
    return re;
}

/*********************************************************************************************
函 数 名:i2c_Ack 
实现功能: CPU产生一个ACK信号
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspI2C_Ack(void)
{
    I2C_SDA_0();    /* CPU驱动SDA = 0 */
    i2c_Delay();
    I2C_SCL_1();    /* CPU产生1个时钟 */
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
    I2C_SDA_1();    /* CPU释放SDA总线 */
}
/*********************************************************************************************
函 数 名: i2c_NAck
实现功能: CPU产生1个NACK信号
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspI2C_NAck(void)
{
    I2C_SDA_1();    /* CPU驱动SDA = 1 */
    i2c_Delay();
    I2C_SCL_1();    /* CPU产生1个时钟 */
    i2c_Delay();
    I2C_SCL_0();
    i2c_Delay();
}

/*********************************************************************************************
函 数 名: i2c_CheckDevice
实现功能: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
输入参数: _Address：设备的I2C总线地址
输出参数: 无;
返 回 值: 返回值 0 表示正确， 返回1表示未探测到
说    明: 无;
*********************************************************************************************/
uint8_t bspI2C_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    if (I2C_SDA_READ() && I2C_SCL_READ())
    {
        bspI2C_Start();     /* 发送启动信号 */

        /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
        bspI2C_SendByte(_Address | I2C_WR);
        ucAck = bspI2C_WaitAck();   /* 检测设备的ACK应答 */

        bspI2C_Stop();          /* 发送停止信号 */

        return ucAck;
    }
    return 1;   /* I2C总线异常 */
}
