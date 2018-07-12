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
    ����MPU6050,ÿһ�η���һ�����ݰ�,ÿ�����ݰ��к�����֡����,�ֱ��Ǽ���֡,���ٶ�֡�ͽǶ�֡.
    ÿһ֡�����а���11���ֽڵ�����.
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
        u8 head; //��ͷ��ʶ�� �̶�Ϊ0x55
        MpuFrameTypeEnum type; //���������� ȡֵ:0x51,0x52,0x53
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
    u8  isHaveData;//�����Ƿ���Ч����
    u8  frameIndex; //��ǰ���ڽ����ĸ�֡
    u8  counter;    //��ǰ����֡�е�����ƫ��
    MPU6050_FRAME_UNION  fram[3];  //��֪����һ֡�е�������ʲô,��Ҫ����ʵʱ���յ�����������
}MPU_PACKAGE_STRUCT;

typedef struct {
  u8 whichUsed;//��ǰ����ʹ�õ����ĸ���
  MPU_PACKAGE_STRUCT stDataPackage[2];//ֻ���������ݰ�����.��λ������ѯʱ,ֻ���ظ����µ����ݼ���.����û�б�Ҫ�ڴ洢��ʷ����.
}MPU_DATA_INFO;



extern MPU_DATA_INFO g_stMpu6050Info;
/*********************************************************************************************
�� �� ��: servMpu6050ReadValue;
ʵ�ֹ���: �ӽǶȴ������ж�ȡ����ֵ,�����а��������� 
�������: ��;
�������: ��;
�� �� ֵ: 0:��ȡ�ɹ� �����ȡʧ��
˵    ��: 
���ݸ�ʽ:  pdata ��Ҫ�����ռ� (11*3 �ֽ�)
   ��ͷ ������    X������           Y������        Z������       �¶�����
    0x55  0x51  X_low   X_high   Y_low  Y_high  Z_low  Z_high   T_low  T_high
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
//int servMpu6050ReadValue(u8 * pdata);
/*********************************************************************************************
�� �� ��: servMpu6050ReadValue_code;
ʵ�ֹ���: ��ȡ�Ƕȴ��� ���Ĳ�����ֵ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: 
********************************************************************************************/
int servMpu6050ReadValue_code(SensorCodeStruct * acceleration, SensorCodeStruct * angularVelocity,
                              SensorCodeStruct * angle, u16 * temp);
/*********************************************************************************************
�� �� ��: servMpu6050ReadValue_float;
ʵ�ֹ���: ��ȡ�Ƕȴ�������ֵ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: 
********************************************************************************************/
int servMpu6050ReadValue_float(CoordinateStruct * acceleration, CoordinateStruct * angularVelocity,
                               CoordinateStruct * angle, u16 * temp);
#endif



