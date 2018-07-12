/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servMpu6050.c；
功能描述:  角度传感器
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  xyzheng;
完成日期:  2017.02.25;
历史版本:  无;
**********************************************************************************************/



/**********************************************************************************************
                                关于角度传感器应用的说明   
                                                            --CP
目前使用的角度传感器有x y z三个轴的角度数据 经过权衡决定采用x轴的角度数据提供给机械臂进行计算
对于不同的安装位置 x的角度变化规律也会不同 目前大臂和小臂的角度传感器采用了相同的安装方式:传感
器没有芯片的一面平贴在大臂/小臂背部,数据线从臂左侧引出,在这种安装方式下 x轴角度变化规律如下图
                    90度
                    |
                    |
                    |
                    |
    180度--------------------0度/360度(前向)
                    |
                    |
                    |
                    |
                    270度
(规定运行正方向为  0度-->90度-->180度-->270度)


腕关节的传感器没有芯片的一面平贴在腕关节最末端与小臂平面垂直的金属板上,数据线从小臂平面一侧引出,
在这种安装方式下 x轴的角度变化规律如下图

                180度
                |
                |
                |
                |
  90度--------------------270度(前向)
                |
                |
                |
                |
                0度/360度
(规定运行正方向为  270度-->180度-->90度--0度)

上位机对于角度的处理都是公式都是以大/小臂的角度坐标系为准,腕关节的角度数据需要处理后才能带入公式
角度处理方式如下:
float servWristAngleHandle(float RealX)//RealX为腕关节角度传感器回传的角度数据
{
    return (270+360-RealX)%360; //----公式1
}





某臂与向上中轴线的夹角为0时表示臂向上直立
臂顺时针偏移为负角度  逆时针偏移为正角度 
即臂抬高为正角度  下降为负角度

servInAngleFromVertical()函数计算某臂与中轴线的偏移角
realX对于大/小臂为角度传感器回传的X轴角度数据
realX对于腕关节为经过servWristAngleHandle()处理后的数据

float servInAngleFromVertical(float realX)
{
    float Inangle = 0;
    if(realX < 270)
    {
        Inangle =  realX - 90;
    }
    else //角度数值在270 -- 360之间
    {
        Inangle = realX - 450;
    }
}



再此定义对于相邻的两个关节而言
越靠近基座的关节(臂)为近端
远离基座的关节(臂)为远端


以大臂(近端臂)小臂(远端臂)为例,定义
大臂静止,小臂顺时针旋转至于大臂重合时的角度为
两臂间夹角
servInAngleBetween根据两条臂与中轴线的
夹角计算两臂间夹角
float BigArmAng为根据servInAngleFromVertical计算出的近端臂的偏角
float LowArmAng为根据servInAngleFromVertical计算出的远端臂的偏角
float servInAngleBetween(float BigArmAng,float LowArmAng)
{
    return 180 - (BigArmAng - LowArmAng);
}




**********************************************************************************************/





#include "servMpu6050.h"
#include "servSoftTimer.h"

#include "string.h"
MPU_DATA_INFO g_stMpu6050Info=
{
    .whichUsed = 0,
    .stDataPackage = {0,0},
};


extern SensorDataStruct  g_sensorData;



/*********************************************************************************************
函 数 名: servMpu6050ReadValue;
实现功能: 从角度传感器中读取采样值,数据中包含三个包 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 
数据格式:  pdata 需要留够空间 (11*3 字节)
   包头 包类型    X轴数据           Y轴数据        Z轴数据       温度数据     校验和
    0x55  0x51  X_low   X_high   Y_low  Y_high  Z_low  Z_high   T_low  T_high  sum
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
int servMpu6050ReadValue(u8 * pdata)
{
    u16 ii = 0;
    MPU_PACKAGE_STRUCT *pstPackage;
    
#if REC_ONCE_WITH_IRQ
    pstPackage = &g_stMpu6050Info.stDataPackage[0];
    USART_ITConfig(MPU_UART_USED, USART_IT_RXNE, ENABLE); //打开中断
    while (!pstPackage->isHaveData)//等待数据
    {
        ii++;
        bspDelayMs(1);
        if (ii>=15)
        {
            //error
            return -1;
        }
    }; 
#else
    pstPackage = &g_stMpu6050Info.stDataPackage[1 - g_stMpu6050Info.whichUsed];
#endif

    if (pstPackage->isHaveData)
    {
        memcpy(pdata, pstPackage->fram, sizeof(MPU6050_FRAME_UNION) * 3);
        pstPackage->isHaveData = 0;
    }
    else
    {
        return -1;
    }
    
#if REC_ONCE_WITH_IRQ 
    USART_ITConfig(MPU_UART_USED, USART_IT_RXNE, DISABLE); //关闭中断
    memset(&g_stMpu6050Info,0,sizeof(MPU_DATA_INFO));
#endif

    return 0;
}

/*********************************************************************************************
函 数 名: servMpu6050ReadValue_code;
实现功能: 读取角度传感 器的采样码值
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 
********************************************************************************************/
int servMpu6050ReadValue_code(SensorCodeStruct * acceleration, SensorCodeStruct * angularVelocity,
                              SensorCodeStruct * angle, u16 * temp)
{
    int i = 0;
    MPU6050_FRAME_UNION unData[3];

    
    if (servMpu6050ReadValue((u8*)unData)< 0)
    {
        return -1; //接收出错
    }
    
    for (i = 0;i < 3;i++)
    {
        switch (unData[i].stData.type)
        {
            case 0x51:
                if (acceleration != NULL)
                {
                    acceleration->xCode = unData[i].stData.x;
                    acceleration->yCode = unData[i].stData.y;
                    acceleration->zCode = unData[i].stData.z;
                }
              break;
              
            case 0x52:
                if (angularVelocity != NULL)
                {
                    angularVelocity->xCode = unData[i].stData.x;
                    angularVelocity->yCode = unData[i].stData.y;
                    angularVelocity->zCode = unData[i].stData.z;
                }
              break; 
              
            case 0x53:
                if (angle != NULL)
                {
                    angle->xCode = unData[i].stData.x;
                    angle->yCode = unData[i].stData.y;
                    angle->zCode = unData[i].stData.z;
                }
              break;

            default:
              break;
        }
    }

    if (temp != NULL)
    {
        *temp = unData[i].stData.temp;
    }
      
    return 0;  
}

/*********************************************************************************************
函 数 名: servMpu6050ReadValue_float;
实现功能: 读取角度传感器的值
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 
********************************************************************************************/
int servMpu6050ReadValue_float(CoordinateStruct * acceleration, CoordinateStruct * angularVelocity,
                               CoordinateStruct * angle, u16 * temp)
{
    int i = 0;
    MPU6050_FRAME_UNION unData[3];


    if (servMpu6050ReadValue((u8*)unData)< 0)
    {
        return -1; //接收出错
    }

    for (i = 0;i < 3;i++)
    {
        switch(unData[i].stData.type)
        {
            case 0x51:
                if (acceleration != NULL)
                {
                    acceleration->xAxis = unData[i].stData.x * ACCELERATION_CODE_TO_VALUE_FACTOR;    //CJ 2017.06.07 Mdofiy
                    acceleration->yAxis = unData[i].stData.y * ACCELERATION_CODE_TO_VALUE_FACTOR;
                    acceleration->zAxis = unData[i].stData.z * ACCELERATION_CODE_TO_VALUE_FACTOR;

                    //FOR MODIFY NICK
                }
              break;
            
            case 0x52:
                if (angularVelocity != NULL)
                {
                    angularVelocity->xAxis = unData[i].stData.x * VELOCITY_CODE_TO_VALUE_FACTOR;    //CJ 2017.06.07 Mdofiy
                    angularVelocity->yAxis = unData[i].stData.y * VELOCITY_CODE_TO_VALUE_FACTOR;
                    angularVelocity->zAxis = unData[i].stData.z * VELOCITY_CODE_TO_VALUE_FACTOR;

                    //FOR MODIFY NICK
                }
              break;
            
            case 0x53:
                if (angle != NULL)
                {
                    angle->xAxis = unData[i].stData.x * ANGLE_CODE_TO_VALUE_FACTOR;    //CJ 2017.06.07 Mdofiy
                    angle->yAxis = unData[i].stData.y * ANGLE_CODE_TO_VALUE_FACTOR;
                    angle->zAxis = unData[i].stData.z * ANGLE_CODE_TO_VALUE_FACTOR;

                    //FOR MODIFY NICK
                }
              break;

            default:
              break;
        }
    }

    if (temp != NULL)
    {
        *temp = unData[i].stData.temp/340+36.53;
    }
      
    return 0;  
}



/*******************************************文件尾********************************************/
