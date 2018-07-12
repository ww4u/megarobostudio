#ifndef _SERV_MPU6050_H_
#define _SERV_MPU6050_H_
#include "bspUart.h"
#include "comStruct.h"

#if HARDWARE_VER_1
#define MPU_UART_USED     USART2
#else
#define MPU_UART_USED     USART3
#endif

/*
    对于MPU6050,每一次发送一个数据包,每个数据包中含有三帧数据,分别是加速帧,角速度帧和角度帧.
    每一帧数据中包含11个字节的内容.
*/
    
typedef enum
{
    MPUFRAME_ACC = 0x51,
    MPUFRAME_VEL = 0x52,
    MPUFRAME_ANGLE = 0x53
    
}MpuFrameTypeEnum;


typedef union 
{
    struct _mpu6050_data_struct
    {
        u8 head; //包头标识符 固定为0x55
        MpuFrameTypeEnum type; //包数据类型 取值:0x51,0x52,0x53
        u16 x;   
        u16 y;
        u16 z;
        u16 temp;
        u8  sum;
    }stData;
    
    u8  au8Data[10];
    
}MPU6050_FRAME_UNION;

typedef struct
{
    u8  isHaveData;//本包是否有效数据
    u8  frameIndex; //当前正在接收哪个帧
    u8  counter;    //当前接收帧中的数据偏移
    MPU6050_FRAME_UNION  fram[3];  //不知道这一帧中的数据是什么,需要根据实时接收到的类型来定
}MPU_PACKAGE_STRUCT;

typedef struct {
  u8 whichUsed;//当前正在使用的是哪个包
  MPU_PACKAGE_STRUCT stDataPackage[2];//只用两个数据包缓冲.上位机来查询时,只返回给最新的数据即可.所以没有必要在存储历史数据.
}MPU_DATA_INFO;



extern MPU_DATA_INFO g_stMpu6050Info;
/*********************************************************************************************
函 数 名: servMpu6050ReadValue;
实现功能: 从角度传感器中读取采样值,数据中包含三个包 
输入参数: 无;
输出参数: 无;
返 回 值: 0:读取成功 否则读取失败
说    明: 
数据格式:  pdata 需要留够空间 (11*3 字节)
   包头 包类型    X轴数据           Y轴数据        Z轴数据       温度数据
    0x55  0x51  X_low   X_high   Y_low  Y_high  Z_low  Z_high   T_low  T_high
包头类型:
    0x51  加速度
    0x52  角速度
    0x53  角度数据
  加速度转换 a = u16CODE / 32768 * 16 * 9.8
  角速度转换公式 w = u16CODE / 32768 * 2000   单位:度/S
  角度转换公式   angle = u16CODE / 32768 * 180   单位:度
  温度计算公式  T = u16CODE/340 + 36.53   单位:摄氏度
   u16CODE = (XXX_high<<8) | XXX_low
*********************************************************************************************/
//int servMpu6050ReadValue(u8 * pdata);
/*********************************************************************************************
函 数 名: servMpu6050ReadValue_code;
实现功能: 读取角度传感 器的采样码值
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 
********************************************************************************************/
int servMpu6050ReadValue_code(SensorCodeStruct * acceleration, SensorCodeStruct * angularVelocity,
                              SensorCodeStruct * angle, u16 * temp);
/*********************************************************************************************
函 数 名: servMpu6050ReadValue_float;
实现功能: 读取角度传感器的值
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 
********************************************************************************************/
int servMpu6050ReadValue_float(CoordinateStruct * acceleration, CoordinateStruct * angularVelocity,
                               CoordinateStruct * angle, u16 * temp);
#endif



