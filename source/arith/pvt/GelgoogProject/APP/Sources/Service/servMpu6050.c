/**********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servMpu6050.c��
��������:  �Ƕȴ�����
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  xyzheng;
�������:  2017.02.25;
��ʷ�汾:  ��;
**********************************************************************************************/



/**********************************************************************************************
                                ���ڽǶȴ�����Ӧ�õ�˵��   
                                                            --CP
Ŀǰʹ�õĽǶȴ�������x y z������ĽǶ����� ����Ȩ���������x��ĽǶ������ṩ����е�۽��м���
���ڲ�ͬ�İ�װλ�� x�ĽǶȱ仯����Ҳ�᲻ͬ Ŀǰ��ۺ�С�۵ĽǶȴ�������������ͬ�İ�װ��ʽ:����
��û��оƬ��һ��ƽ���ڴ��/С�۱���,�����ߴӱ��������,�����ְ�װ��ʽ�� x��Ƕȱ仯��������ͼ
                    90��
                    |
                    |
                    |
                    |
    180��--------------------0��/360��(ǰ��)
                    |
                    |
                    |
                    |
                    270��
(�涨����������Ϊ  0��-->90��-->180��-->270��)


��ؽڵĴ�����û��оƬ��һ��ƽ������ؽ���ĩ����С��ƽ�洹ֱ�Ľ�������,�����ߴ�С��ƽ��һ������,
�����ְ�װ��ʽ�� x��ĽǶȱ仯��������ͼ

                180��
                |
                |
                |
                |
  90��--------------------270��(ǰ��)
                |
                |
                |
                |
                0��/360��
(�涨����������Ϊ  270��-->180��-->90��--0��)

��λ�����ڽǶȵĴ����ǹ�ʽ�����Դ�/С�۵ĽǶ�����ϵΪ׼,��ؽڵĽǶ�������Ҫ�������ܴ��빫ʽ
�Ƕȴ���ʽ����:
float servWristAngleHandle(float RealX)//RealXΪ��ؽڽǶȴ������ش��ĽǶ�����
{
    return (270+360-RealX)%360; //----��ʽ1
}





ĳ�������������ߵļн�Ϊ0ʱ��ʾ������ֱ��
��˳ʱ��ƫ��Ϊ���Ƕ�  ��ʱ��ƫ��Ϊ���Ƕ� 
����̧��Ϊ���Ƕ�  �½�Ϊ���Ƕ�

servInAngleFromVertical()��������ĳ���������ߵ�ƫ�ƽ�
realX���ڴ�/С��Ϊ�Ƕȴ������ش���X��Ƕ�����
realX������ؽ�Ϊ����servWristAngleHandle()����������

float servInAngleFromVertical(float realX)
{
    float Inangle = 0;
    if(realX < 270)
    {
        Inangle =  realX - 90;
    }
    else //�Ƕ���ֵ��270 -- 360֮��
    {
        Inangle = realX - 450;
    }
}



�ٴ˶���������ڵ������ؽڶ���
Խ���������Ĺؽ�(��)Ϊ����
Զ������Ĺؽ�(��)ΪԶ��


�Դ��(���˱�)С��(Զ�˱�)Ϊ��,����
��۾�ֹ,С��˳ʱ����ת���ڴ���غ�ʱ�ĽǶ�Ϊ
���ۼ�н�
servInAngleBetween�����������������ߵ�
�нǼ������ۼ�н�
float BigArmAngΪ����servInAngleFromVertical������Ľ��˱۵�ƫ��
float LowArmAngΪ����servInAngleFromVertical�������Զ�˱۵�ƫ��
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
�� �� ��: servMpu6050ReadValue;
ʵ�ֹ���: �ӽǶȴ������ж�ȡ����ֵ,�����а��������� 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: 
���ݸ�ʽ:  pdata ��Ҫ�����ռ� (11*3 �ֽ�)
   ��ͷ ������    X������           Y������        Z������       �¶�����     У���
    0x55  0x51  X_low   X_high   Y_low  Y_high  Z_low  Z_high   T_low  T_high  sum
��ͷ����:
    0x51  ���ٶ�
    0x52  ���ٶ�
    0x53  �Ƕ�����
  ���ٶ�ת�� a = u16CODE / 32768 * 16 * 9.8
  ���ٶ�ת����ʽ w = u16CODE / 32768 * 2000   ��λ:��/S
  �Ƕ�ת����ʽ   angle = u16CODE / 32768 * 180   ��λ:��
  �¶ȼ��㹫ʽ  T = u16CODE/340 + 36.53   ��λ:���϶�
   u16CODE = (XXX_high<<8) | XXX_low
*********************************************************************************************/
int servMpu6050ReadValue(u8 * pdata)
{
    u16 ii = 0;
    MPU_PACKAGE_STRUCT *pstPackage;
    
#if REC_ONCE_WITH_IRQ
    pstPackage = &g_stMpu6050Info.stDataPackage[0];
    USART_ITConfig(MPU_UART_USED, USART_IT_RXNE, ENABLE); //���ж�
    while (!pstPackage->isHaveData)//�ȴ�����
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
    USART_ITConfig(MPU_UART_USED, USART_IT_RXNE, DISABLE); //�ر��ж�
    memset(&g_stMpu6050Info,0,sizeof(MPU_DATA_INFO));
#endif

    return 0;
}

/*********************************************************************************************
�� �� ��: servMpu6050ReadValue_code;
ʵ�ֹ���: ��ȡ�Ƕȴ��� ���Ĳ�����ֵ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: 
********************************************************************************************/
int servMpu6050ReadValue_code(SensorCodeStruct * acceleration, SensorCodeStruct * angularVelocity,
                              SensorCodeStruct * angle, u16 * temp)
{
    int i = 0;
    MPU6050_FRAME_UNION unData[3];

    
    if (servMpu6050ReadValue((u8*)unData)< 0)
    {
        return -1; //���ճ���
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
�� �� ��: servMpu6050ReadValue_float;
ʵ�ֹ���: ��ȡ�Ƕȴ�������ֵ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: 
********************************************************************************************/
int servMpu6050ReadValue_float(CoordinateStruct * acceleration, CoordinateStruct * angularVelocity,
                               CoordinateStruct * angle, u16 * temp)
{
    int i = 0;
    MPU6050_FRAME_UNION unData[3];


    if (servMpu6050ReadValue((u8*)unData)< 0)
    {
        return -1; //���ճ���
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



/*******************************************�ļ�β********************************************/
