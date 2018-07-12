/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servFpga.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  ��;
�������:  ��;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __SERV_FPGA_H__
#define __SERV_FPGA_H__



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comEnum.h"
#include "comDebug.h"



/****************************************�ⲿ��������*****************************************/



/*******************************************�궨��********************************************/
#define    FPGA_REG_QUERY_COUNT_MAX    5

#define FPGA_READ_ACTION    0
#define FPGA_WRITE_ACTION   1


#define CMD_FPGA_R_D_LEN            0x01            //����Ҫ��FPGA���������ݳ��� �Ĵ���
#define CMD_FPGA_W_D_LEN            0x00            //����Ҫ���͸�FPGA�����ݳ��� �Ĵ���  
#define CMD_FPGA_W_D_2_LEN          0x02            //����Ҫ���͸�FPGA�����ݳ��� �Ĵ��� 
#define CMD_FPGA_W_D_3_LEN          0x03            //����Ҫ���͸�FPGA�����ݳ��� �Ĵ��� 
#define CMD_FPGA_REG_ADDR           0x08            //������FPGA�ļĴ���������������   �������ĵ�ַ��Ϊ Ҫ��ȡ��Ŀ�ļĴ�����ַ   


#define ADDR_FPGA_STAT                        2  //״̬�Ĵ���

#define ADDR_FPGA_SD_FRAME                    6  //SD֡���


#define ADDR_FPGA_SD_BURST_TEST               7  //����SD��Burst


#define ADDR_FPGA_Cache_Read                  11  //��ȡcache����


#define ADDR_FPGA_USONIC_DANGER               14   //���������뱨��     Ϊ1��ʾ������� ��Σ��

#define ADDR_FPGA_USONIC_DIS_L                15   //��ȡ ��������������  ��16bit
#define ADDR_FPGA_USONIC_DIS_H                16   //��ȡ ��������������  ��4bit


#define ADDR_FPGA_ENC_ROUND_CNT               17   //������Ȧ��(Zͨ��)����
#define ADDR_FPGA_ENC_AB_CNT_L                18   //������ABͨ������  ��16bit
#define ADDR_FPGA_ENC_AB_CNT_H                19   //������ABͨ������  ��1bit
#define ADDR_FPGA_STEP_CNT_L                  20   //�����������  ��16bit   ��ȡ�������Լ���������ʱ �����ȶ�ȡ�����ĵ�16bit  �ö�����������������
#define ADDR_FPGA_STEP_CNT_H                  21   //�����������  ��8bit


#define ADDR_FPGA_CYC_CNT_L                   22   //�߼���ǰ�����ѭ������ ��16bit
#define ADDR_FPGA_CYC_CNT_H                   23   //�߼���ǰ�����ѭ������ ��16bit


#define ADDR_FPGA_SDIO_DEBUG                  1    //���ڵ��Ե��߼��Ĵ���  ���Խ��ж�д


#define ADDR_FPGA_DRIVER_CTRL                    16 //д   ���������߼�����262 �� 2660

#define ADDR_FPGA_USONIC_CTRL                    19 //д   ����������

#define ADDR_FPGA_USONIC_THRES_L                   20 //����������ֵ  ��16bit
#define ADDR_FPGA_USONIC_THRES_H                   21 //����������ֵ  ��4bit


#define ADDR_FPGA_LED_CTRL                       22 //LED�ƿ���

#define ADDR_FPGA_DDR_STAT                      10 //��   DDR ״̬�Ĵ���

#define ADDR_FPGA_DDR_WAVE_TYPE               23   //ָʾ���õ���������
#define ADDR_FPGA_DDR_WAVE_WRITE_DATA         26   //д��Ĳ������ݼĴ���

#define ADDR_FPGA_DDR_WAVE_WRITE_CTRL         27   //д�벨��ǰ����ƼĴ���

#define ADDR_FPGA_DDR_WAVE_CTRL               32   //�����߼��Ƿ���PWM����

#define ADDR_FPGA_DDR_WAVE_FIFO_OCCUPY_L      3   // FIFO�л�δִ�е����ݸ��� ��16bit
#define ADDR_FPGA_DDR_WAVE_FIFO_OCCUPY_H      4   // FIFO�л�δִ�е����ݸ��� ��16bit



/******************************************************************************
                      �Ĵ�����ַ��λ��궨��
******************************************************************************/
/******************************************************************************
FPGA���ԼĴ���(�ɶ���д)    CJ 2017.07.12 Add
******************************************************************************/
#define    SERV_FPGA_DEBUG_REG            1

#define    SERV_FPGA_DEBUG_LEN            2


#if 0
//ֻ�������ֽ���
#endif

/**************************ֻ���Ĵ���*****************************************/
/******************************************************************************
FPGA�汾�żĴ���(ֻ��)    CJ 2017.09.09 Add
******************************************************************************/
#define    SERV_FPGA_VERSION_REG          0

#define    SERV_FPGA_VERSION_LEN          2

#define    SERV_FPGA_VERSION_HARD_MASK       0xE000     //bit[15:13]
#define    SERV_FPGA_VERSION_HARD_RSHIFT     13

#define    SERV_FPGA_VERSION_MAJOR_MASK      0x1E00     //bit[12:9]
#define    SERV_FPGA_VERSION_MAJOR_RSHIFT    9

#define    SERV_FPGA_VERSION_MINOR_MASK      0x01F0     //bit[8:4]
#define    SERV_FPGA_VERSION_MINOR_RSHIFT    4

#define    SERV_FPGA_VERSION_BUILD_MASK      0x000F     //bit[0:3]


/******************************************************************************
FPGA״̬�Ĵ���(ֻ��)    CJ 2017.07.12 Add
******************************************************************************/
#define    SERV_FPGA_STATE_REG          2

#define    SERV_FPGA_STATE_LEN          2

#define    SERV_FPGA_DRIVE_TYPE_MASK    0x000E    //�������ͺ����� bit[1:3]
#define    SERV_FPGA_DRIVE_TYPE_262     2         //�������ͺ�262  bit[1:3]
#define    SERV_FPGA_DRIVE_TYPE_2660    4         //�������ͺ�2660 bit[1:3]


/******************************************************************************
DDR״̬/���״̬�Ĵ���(ֻ��)    CJ 2017.07.12 Add
******************************************************************************/
#define    SERV_FPGA_DDR_MOTOR_STATE_REG    10

#define    SERV_FPGA_DDR_MOTOR_STATE_LEN    2

#define    SERV_FPGA_RUN_STATE_MASK         0x0780    //����״̬����  bit[7:10]
#define    SERV_FPGA_RUN_STATE_NORUN1       0x0000    //û�����S/D�ź�
#define    SERV_FPGA_RUN_STATE_RUN1         0x0080    //���S/D�ź�
#define    SERV_FPGA_RUN_STATE_SD           0x0100    //��ͣ����
#define    SERV_FPGA_RUN_STATE_NORUN2       0x0180    //û�����S/D�ź�
#define    SERV_FPGA_RUN_STATE_RUN2         0x0200    //���S/D�ź�
#define    SERV_FPGA_RUN_STATE_HOLD         0x0280    //����

#define    SERV_FPGA_DDR_TEST_ERROR         0x0800    //DDR���Դ���   bit[11]
#define    SERV_FPGA_DDR_INIT_MASK          0x0001    //DDR��ʼ������ bit[0]
#define    SERV_FPGA_DDR_INIT_SUCCESS       1         //DDR��ʼ�����


/******************************************************************************
DRIVER״̬�Ĵ���(ֻ��)    CJ 2017.10.26 Add
******************************************************************************/
#define    SERV_FPGA_DRIVER_STATE_L_REG     12
#define    SERV_FPGA_DRIVER_STATE_H_REG     13

#define    SERV_FPGA_DRIVER_STATE_H_MASK    0x000F    //bit[0:3]
#define    SERV_FPGA_DRIVER_STATE_H_LSHIFT  16

#define    SERV_FPGA_DRIVER_CHANNUM_MASK    0x0070    //bit[4:6]
#define    SERV_FPGA_DRIVER_CHANNUM_RSHIFT  4


/******************************************************************************
�����������Ĵ���(ֻ��)    CJ 2017.07.19 Add
******************************************************************************/
#define    SERV_FPGA_ENC_CIRCLE_NUM_REG    17
#define    SERV_FPGA_ENC_ABCNT_L_REG       18
#define    SERV_FPGA_ENC_ABCNT_H_REG       19
#define    SERV_FPGA_ENC_STEPS_L_REG       20
#define    SERV_FPGA_ENC_STEPS_H_REG       21

#define    SERV_FPGA_ENC_COUNT_LEN         2

#define    SERV_FPGA_ENC_AB_CNT_H_MASK     0x0001    //bit[0]
#define    SERV_FPGA_ENC_STEPS_H_MASK      0x00FF    //bit[0:7]

#define    SERV_FPGA_ENC_COUNT_H_LSHIFT    16

#define    SERV_FPGA_ABCNT_FCOUNT_MAX      0x10000      //����������ֵ
#define    SERV_FPGA_ABCNT_RCOUNT_MIN      0x20000      //���������Сֵ
#define    SERV_FPGA_STEPS_FCOUNT_MAX      0x800000     //����������ֵ
#define    SERV_FPGA_STEPS_RCOUNT_MIN      0xFFFFFF     //���������Сֵ


/******************************************************************************
ʱ��ͬ�������Ĵ���(ֻ��)    CJ 2017.07.19 Add
******************************************************************************/
#define    SERV_FPGA_CLOCK_SYNC_COUNT_L_REG    24
#define    SERV_FPGA_CLOCK_SYNC_COUNT_M_REG    25
#define    SERV_FPGA_CLOCK_SYNC_COUNT_H_REG    26

#define    SERV_FPGA_CLOCK_SYNC_COUNT_REG      2

#define    SERV_FPGA_CLOCK_SYNC_COUNT_INVAILD  0x8000    //bit[15]


/******************************************************************************
DIO״̬�Ĵ���(ֻ��)    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_DIO_STATUS_REG    27
#define    SERV_FPGA_DIO_STATUS_LEN    2

#define    SERV_FPGA_DIO_STATUS_MASK   0x0001


/******************************************************************************
�ж�Դ�Ĵ���(ֻ��)    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_INTERRUPT_SRC_REG    28
#define    SERV_FPGA_INTERRUPT_SRC_LEN    2


/******************************************************************************
��������ʱ��Ĵ���(ֻ��)    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_WT_RUN_TIME_L_REG    29
#define    SERV_FPGA_WT_RUN_TIME_M_REG    30
#define    SERV_FPGA_WT_RUN_TIME_H_REG    31

#define    SERV_FPGA_WT_RUN_TIME_LEN    2

/**************************ֻ���Ĵ���*****************************************/

#if 0
//ֻ�������ֽ���
#endif

/**************************ֻд�Ĵ���*****************************************/
/******************************************************************************
DDR���ԼĴ���    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_DDR_TEST_REG      11

#define    SERV_FPGA_DDR_TEST_LEN      2

#define    SERV_FPGA_DDR_TEST_CLEAR    1    //bit[0]


/******************************************************************************
�߼䲽�������üĴ���    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_LINE_STEPS_INVS_L_REG    12
#define    SERV_FPGA_LINE_STEPS_INVS_M_REG    13
#define    SERV_FPGA_LINE_STEPS_INVS_H_REG    14

#define    SERV_FPGA_LINE_STEPS_INVS_LEN    2

#define    SERV_FPGA_LINE_STEPS_MAX    0x100000000    //ʵ��ֵ��SERV_FPGA_LINE_STEPS_VAILD_BIT_VALUE����


/******************************************************************************
�߼䲽������Чλ��Ĵ���    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_LINE_STEPS_VAILD_BIT_REG    15

#define    SERV_FPGA_LINE_STEPS_VAILD_BIT_LEN    2

#define    SERV_FPGA_LINE_STEPS_VAILD_BIT_MASK     0x003F    //bit[0:5]
#define    SERV_FPGA_LINE_STEPS_VAILD_BIT_VALUE    32        //ʵ��ʹ�õ���32bit


/******************************************************************************
����ѡ��Ĵ���    CJ 2017.08.25 Add
******************************************************************************/
#define    SERV_FPGA_DRIVER_REG           16

#define    SERV_FPGA_DRIVER_LEN           2

#define    SERV_FPGA_DRIVER_SELC_MASK     0x0001    //bit[0]
#define    SERV_FPGA_DRIVER_SELC_2660     0x0000
#define    SERV_FPGA_DRIVER_SELC_262      0x0001

#define    SERV_FPGA_DRIVER_STATE_MASK    0x0006    //bit[1:2]
#define    SERV_FPGA_DRIVER_OFF_2660      0x0002    //bit[1]
#define    SERV_FPGA_DRIVER_OFF_262       0x0004    //bit[2]

#define    SERV_FPGA_DRIVER_CLK_MASK      0x0008    //bit[3]
#define    SERV_FPGA_DRIVER_CLK_ENABLE    0x0008

#define    SERV_FPGA_DRIVER_SRC_MASK      0x0030    //bit[4:5]
#define    SERV_FPGA_DRIVER_SRC_CPU       0x0010

#define    SERV_FPGA_DRIVER_CLK2_MASK     0x0040    //bit[6:12]��ÿ��bit��Ӧһ��ͨ��
#define    SERV_FPGA_DRIVER_CLK2_ENABLE   0x0040

#define    SERV_FPGA_DRIVER_CHAN_MASK     0xE000    //bit[13:15]
#define    SERV_FPGA_DRIVER_CHAN_LSHIFT   13


/******************************************************************************
������д�Ĵ���    CJ 2017.08.25 Add
******************************************************************************/
#define    SERV_FPGA_DRIVER_OPERT_L_REG       17
#define    SERV_FPGA_DRIVER_OPERT_H_REG       18

#define    SERV_FPGA_DRIVER_OPERT_LEN          2

#define    SERV_FPGA_DRV_OPERT_VALUE_H_MASK    0x000F    //bit[0:3]
#define    SERV_FPGA_DRV_OPERT_ACT_MASK        0x0010    //bit[4]
#define    SERV_FPGA_DRV_OPERT_ACT_LSHIFT      4


/******************************************************************************
����д��ַ�Ĵ���    CJ 2018.02.01 Add
******************************************************************************/
#define    SERV_FPGA_WT_WRITE_ADDR_L_REG    24
#define    SERV_FPGA_WT_WRITE_ADDR_H_REG    25


/******************************************************************************
����RUN/STOP���ƼĴ���    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_WT_MODE_REG          31

#define    SERV_FPGA_WT_MODE_LEN          2

#define    SERV_FPGA_WT_MODE_MASK            0x0007    //bit[0:2]
#define    SERV_FPGA_WT_MODE_FIFO_ENABLE     0x0001
#define    SERV_FPGA_WT_MODE_CYCLE_ENABLE    0x0002

#define    SERV_FPGA_REVERSE_MOTION_ENABLE   0x0008    //bit[3]

#define    SERV_FPGA_START_SOURCE_MASK       0x0070    //bit[4:6]
#define    SERV_FPGA_START_SOURCE_SOFT       0x0010    //bit[4]
#define    SERV_FPGA_START_SOURCE_HARD       0x0020    //bit[5]
#define    SERV_FPGA_START_SOURCE_CAN        0x0040    //bit[6]

#define    SERV_FPGA_CLOCK_SYNC_MASK         0x0740    //bit[8:10]��ͬʱ��bit6����
#define    SERV_FPGA_CLOCK_SYNC_RESET        0x0100    //bit[8]
#define    SERV_FPGA_CLOCK_SYNC_START        0x0200    //bit[9]
#define    SERV_FPGA_CLOCK_SYNC_END          0x0400    //bit[10]

#define    SERV_FPGA_WT_MOTION_MODE_MASK     0x3000    //bit[12:13]
#define    SERV_FPGA_WT_MTN_MODE_PVT         0x0000
#define    SERV_FPGA_WT_MTN_MODE_LVT_VEL     0x1000
#define    SERV_FPGA_WT_MTN_MODE_LVT_NOVEL   0x3000

#define    SERV_FPGA_WT_LVT_TIME_MASK        0xC000    //bit[15:14]
#define    SERV_FPGA_WT_LVT_TIME_1000        0x0000
#define    SERV_FPGA_WT_LVT_TIME_500         0x4000
#define    SERV_FPGA_WT_LVT_TIME_250         0x8000
#define    SERV_FPGA_WT_LVT_TIME_125         0xC000


/******************************************************************************
����RUN/STOP���ƼĴ���    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_WT_CTRL_REG          32

#define    SERV_FPGA_WT_CTRL_LEN          2

#define    SERV_FPGA_WAVE_PREFETCH             0x0001    //bit[0]
#define    SERV_FPGA_WAVE_OUTPUT_ENABLE        0x0002    //bit[1]
#define    SERV_FPGA_WAVE_STOP                 0x0004    //bit[2]
#define    SERV_FPGA_WAVE_FIFO_RESET           0x0008    //bit[3]
#define    SERV_FPGA_WAVE_ALL_RUN              0x1000    //bit[12]


#define    SERV_FPGA_SD_SRC_SOFT_ENABLE        0x0010    //bit[4]    ���������ͣ
#define    SERV_FPGA_SD_SRC_DIO_ENABLE         0x0020    //bit[5]    �������봥����ͣ
#define    SERV_FPGA_SD_SRC_WAVE_END_ENABLE    0x0040    //bit[6]    �����������������ͣ

#define    SERV_FPGA_STOP_DECEL_MASK           0x0F00    //bit[8:11]
#define    SERV_FPGA_SD_IMMED_ENABLE           0x0100    //bit[8]    ����ģʽ����ֹͣʹ��
#define    SERV_FPGA_SD_STEP_ENABLE            0x0200    //bit[9]    ����ģʽ������ֹͣ
#define    SERV_FPGA_SD_SLOPE_ENABLE           0x0400    //bit[10]   ����ģʽ�����ٶ�ֹͣ
#define    SERV_FPGA_SD_TIME_ENABLE            0x0800    //bit[11]   ����ģʽ��ʱ��ֹͣ

#define    SERV_FPGA_WAVE_HOLD_MASK            0xE000    //bit[13:15]
#define    SERV_FPGA_WH_SRC_WAVE_END_ENABLE    0x8000    //bit[15]   �������������������


/******************************************************************************
������ʼ��ַ���üĴ���    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_WT_START_ADDR_L_REG    33
#define    SERV_FPGA_WT_START_ADDR_H_REG    34


/******************************************************************************
���������üĴ���    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_WT_LENGTH_L_REG    35
#define    SERV_FPGA_WT_LENGTH_H_REG    36

#define    SERV_FPGA_WT_LENGTH_LEN      2


/******************************************************************************
����ѭ�������üĴ���    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_WT_CYCLE_NUM_L_REG    37
#define    SERV_FPGA_WT_CYCLE_NUM_H_REG    38

#define    SERV_FPGA_WT_CYCLE_NUM_LEN      2


/******************************************************************************
������������λ�Ĵ���    CJ 2017.07.19 Add
******************************************************************************/
#define    SERV_FPGA_ENCODER_RESET_REG    39

#define    SERV_FPGA_ENCODER_RESET_LEN    2

#define    SERV_FPGA_ENC_COUNT_MASK       0x0001    //bit[0]
#define    SERV_FPGA_ENC_COUNT_RESET      1

#define    SERV_FPGA_ENC_MULT_MASK        0x0006    //bit[1:2]
#define    SERV_FPGA_ENC_MULT_SINGLE      0x0000
#define    SERV_FPGA_ENC_MULT_DOUBLE      0x0002
#define    SERV_FPGA_ENC_MULT_QUADR       0x0006

#define    SERV_FPGA_ENC_CHAN_SINGLE      0x0008    //bit[3]

#define    SERV_FPGA_ENC_LINE_MULT_MASK      0xFF00    //bit[8:15]
#define    SERV_FPGA_ENC_LINE_MULT_OFFSET    0
#define    SERV_FPGA_ENC_LINE_MULT_LSHIFT    8


/******************************************************************************
����������ֵ���üĴ���    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_STOPDECEL_PERIOD_L_REG     40
#define    SERV_FPGA_STOPDECEL_PERIOD_H_REG     41

#define    SERV_FPGA_STOPDECEL_PERIOD_LEN       2

#define    SERV_FPGA_STOPDECEL_PERIOD_H_MASK    0x01FF    //bit[0:8]

#define    SERV_FPGA_STOPDECEL_PERIOD_H_RSHIFT  16


/******************************************************************************
����б�����üĴ���    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_STOPDECEL_SLOPE_L_REG    42
#define    SERV_FPGA_STOPDECEL_SLOPE_M_REG    43
#define    SERV_FPGA_STOPDECEL_SLOPE_H_REG    44

#define    SERV_FPGA_STOPDECEL_SLOPE_LEN    2

//��48bit����������Ϊ��25bit��С������Ϊ��23bit
#define    SERV_FPGA_STOPDECEL_SLOPE_DEC_VAILD_BITS    0x800000   //23bit��Ч
#define    SERV_FPGA_STOPDECEL_SLOPE_M_DEC_MASK        0x007F
#define    SERV_FPGA_STOPDECEL_SLOPE_M_INT_MASK        0x01FF

#define    SERV_FPGA_STOPDECEL_SLOPE_M_INT_LSHIFT    7
#define    SERV_FPGA_STOPDECEL_SLOPE_RSHIFT          16


/******************************************************************************
DO���üĴ���    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_DIGITAL_OUTPUT_REG    45

#define    SERV_FPGA_DIGITAL_OUTPUT_LEN    2

#define    SERV_FPGA_DO1_CONFIG_MASK     0x0007    //bit[0:2]
#define    SERV_FPGA_DO1_SELECT_TRIG     0x0000
#define    SERV_FPGA_DO1_SELECT_LEVEL    0x0001
#define    SERV_FPGA_DO1_LEVEL_HIGH      0x0008

#define    SERV_FPGA_DO2_CONFIG_MASK     0x0070    //bit[4:6]
#define    SERV_FPGA_DO2_SELECT_UART     0x0000
#define    SERV_FPGA_DO2_SELECT_LEVEL    0x0010
#define    SERV_FPGA_DO2_SELECT_LED      0x0020
#define    SERV_FPGA_DO2_SELECT_SQUARE   0x0030
#define    SERV_FPGA_DO2_LEVEL_HIGH      0x0080    //bit[7]

#define    SERV_FPGA_YO1_LEVEL_HIGH      0x0100    //bit[8]
#define    SERV_FPGA_YO2_LEVEL_HIGH      0x0200    //bit[9]


/******************************************************************************
UART3���üĴ���    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_ENC_AND_UART3_REG   46

#define    SERV_FPGA_ENC_SELECT_MASK     0x000F    //bit[0:3]
#define    SERV_FPGA_ENC_SELECT_SWZAB    0x0000    //bit[0:3]
#define    SERV_FPGA_ENC_SELECT_DI145    0x0001    //bit[0:3]
#define    SERV_FPGA_ENC_SELECT_DI123    0x0002    //bit[0:3]

#define    SERV_FPGA_UART_SELECT_MASK    0x0070    //bit[4:6]
#define    SERV_FPGA_UART_SELECT_SEN1    0x0010
#define    SERV_FPGA_UART_SELECT_SEN2    0x0020
#define    SERV_FPGA_UART_SELECT_SEN3    0x0030
#define    SERV_FPGA_UART_SELECT_SEN4    0x0040

#define    SERV_FPGA_UART_TX_MASK_SEN1   0x0300    //bit[8:9]
#define    SERV_FPGA_UART_TX_SEN1_HIGH   0x0000
#define    SERV_FPGA_UART_TX_SEN1_TX     0x0100
#define    SERV_FPGA_UART_TX_SEN1_LOW    0x0200

#define    SERV_FPGA_UART_TX_MASK_SEN2   0x0C00    //bit[10:11]
#define    SERV_FPGA_UART_TX_SEN2_HIGH   0x0000
#define    SERV_FPGA_UART_TX_SEN2_TX     0x0400
#define    SERV_FPGA_UART_TX_SEN2_LOW    0x0800

#define    SERV_FPGA_UART_TX_MASK_SEN3   0x3000    //bit[12:13]
#define    SERV_FPGA_UART_TX_SEN3_HIGH   0x0000
#define    SERV_FPGA_UART_TX_SEN3_TX     0x1000
#define    SERV_FPGA_UART_TX_SEN3_LOW    0x2000

#define    SERV_FPGA_UART_TX_MASK_SEN4   0xC000    //bit[14:15]
#define    SERV_FPGA_UART_TX_SEN4_HIGH   0x0000
#define    SERV_FPGA_UART_TX_SEN4_TX     0x4000
#define    SERV_FPGA_UART_TX_SEN4_LOW    0x8000


/******************************************************************************
ֹͣ���ؼĴ����͸澯���ͼĴ����������Ĵ�����λ����һ��    CJ 2017.06.26 Add
******************************************************************************/
#define    SERV_FPGA_STOP_SWITCH_REG        47
#define    SERV_FPGA_ALARM_CODE_REG         48

#define    SERV_FPGA_STOP_SWITCH_LEN   2
#define    SERV_FPGA_ALARM_CODE_LEN    2

#define    SERV_FPGA_TRIG_COND_LEN     3      //��������λ�򳤶�3bit
#ifdef PROJECT_GELGOOG
#define    SERV_FPGA_TRIG_IN_OFFSET    2      //��������˳��ƫ��
#else
#define    SERV_FPGA_TRIG_IN_OFFSET    0      //��������˳��ƫ��
#endif
#define    SERV_FPGA_TRIG_COND_NONE    0x00
#define    SERV_FPGA_TRIG_COND_LOW     0x04
#define    SERV_FPGA_TRIG_COND_RISE    0x05
#define    SERV_FPGA_TRIG_COND_FALL    0x06
#define    SERV_FPGA_TRIG_COND_HIGH    0x07

#define    SERV_FPGA_TRIG_ENABLE       0x07
#define    SERV_FPGA_TRIG_DISABLE      0x00


/******************************************************************************
���������߼���ϵ�Ĵ���    CJ 2017.06.26 Add
******************************************************************************/
#define    SERV_FPGA_TRIG_REL_REG         49

#define    SERV_FPGA_TRIG_REL_LEN         2

#define    SERV_FPGA_STOP_OPR_ENABLE      0x03    //bit[0:1]
#define    SERV_FPGA_STOP_OPR_AND         0x02
#define    SERV_FPGA_STOP_OPR_OR          0x03

#define    SERV_FPGA_ALARM_OPR_ENABLE     0x0C    //bit[2:3]
#define    SERV_FPGA_ALARM_OPR_AND        0x08
#define    SERV_FPGA_ALARM_OPR_OR         0x0C

#define    SERV_FPGA_REF_SELECT_MASK      0x0030    //bit[5:4]
#define    SERV_FPGA_REF_CH8_NO_ENC       0x0000
#define    SERV_FPGA_REF_CH4_ENC_SINGLE   0x0010    //�ӵ��߱�����
#define    SERV_FPGA_REF_CH4_ENC_TRIPLE   0x0020    //�����߱�����
#define    SERV_FPGA_REF_CH4_ENC_SENSOR   0x0030    //4ͨ���������ӿ����ڽӴ�����


/******************************************************************************
�ж�Դ���üĴ���    CJ 2017.06.26 Add
******************************************************************************/
#define    SERV_FPGA_INT_SRC_REG          50

#define    SERV_FPGA_INT_SRC_LEN          2

#define    SERV_FPGA_STOP_INT_ENABLE        0x01
#define    SERV_FPGA_ALARM_INT_ENABLE       0x02
#define    SERV_FPGA_WAVE_END_INT_ENABLE    0x04
#define    SERV_FPGA_OOSTEP_INT_ENABLE      0x08


/******************************************************************************
UART4����ѡ��Ĵ���    CJ 2017.11.02 Add
******************************************************************************/
#define    SERV_FPGA_UART4_RX_REG          51

#define    SERV_FPGA_UART4_SELECT_MASK    0x0007    //bit[0:2]
#define    SERV_FPGA_UART4_SELECT_SEN4    0x0001    //POS13
#define    SERV_FPGA_UART4_SELECT_SEN1    0x0002    //POS14
#define    SERV_FPGA_UART4_SELECT_SEN2    0x0003    //POS15
#define    SERV_FPGA_UART4_SELECT_SEN3    0x0004    //POS16


/******************************************************************************
DI�˲����üĴ���    CJ 2017.07.12 Add
******************************************************************************/
#define    SERV_FPGA_DI2_X1_FILTER_REG         52
#define    SERV_FPGA_DI3_X2_FILTER_REG         53
#define    SERV_FPGA_DI4_X3_FILTER_REG         54
#define    SERV_FPGA_DI5_X4_FILTER_REG         55
#define    SERV_FPGA_CHANZ_FILTER_REG          56

#define    SERV_FPGA_DI_FILTER_LEN             2

#define    SERV_FPGA_SPERIOD_US_ENABLE         0x8000        //bit[15]    ��������200us��disableʱΪ12.5ns
#define    SERV_FPGA_FILTER_TIME_MASK          0x7FFF        //bit[0:14]

#define    SERV_FPGA_SPERIOD_GEAR_VALUE        0.4f

#define    SERV_FPGA_SPERIOD_CODE_MIN          1

#define    SERV_FPGA_SPERIOD_US_VALUE          0.2f          //��λms��ʵ��ֵ200us
#define    SERV_FPGA_SPERIOD_NS_VALUE          0.0000125f    //��λms��ʵ��ֵ12.5ns
#define    SERV_FPGA_SPERIOD_OFFSET            1             //FPGAҪ�����õ�ʱ���1


/******************************************************************************
ʧ���澯��ֵ���üĴ���    CJ 2017.08.12 Add
******************************************************************************/
#define    SERV_FPGA_LINE_OUT_OF_STEP_L_REG    57
#define    SERV_FPGA_LINE_OUT_OF_STEP_H_REG    58

#define    SERV_FPGA_LINE_OUT_OF_STEP_LEN      2


/******************************************************************************
����ֵ��������ȡ�Ĵ���    CJ 2018.02.28 Add
******************************************************************************/
#define    SERV_FPGA_ABS_ENC_REDA_REG          59


/**************************ֻд�Ĵ���*****************************************/



/***************************************��������������****************************************/
typedef enum
{
    RUNSTETE_NORUN = 0,      //ֹͣ
    RUNSTETE_RUN,
    RUNSTETE_SD,
    RUNSTETE_HOLD,
    RUNSTETE_ERROR,
    
}RunStatenum;    //�߼��������״̬  


typedef enum
{
    SONIC_ARM_PROTECT = 0,    //�߼���⵽��ײΣ�պ����κζ���
    SONIC_FPGA_PROTECT = 1    //�߼���⵽��ײΣ�պ��������б�������
}euSonicProtMode;



typedef enum
{
    RUN_CTRL = 0,  //д����ǲ���
    STOP_CTRL = 1   //д����Ǳ�����У׼ϵ��
}euRunStopCtrl;


typedef enum
{
    WAVE_DATA = 0,  //д����ǲ���
    ENCODER_DATA = 1    //д����Ǳ�����У׼ϵ��
}euWaveDataType;
 
 typedef enum
 {
     FIFO_MODE = 1,     //FIFO����ģʽ
     CYCLE_MODE = 2     //ѭ������ģʽ
 }euWaveRunMode;


typedef enum
{
    WAVE_RUNNING = 0,      //�����������״̬
    WAVE_STOPPING = 1      //�������ֹͣ״̬
}euWaveRunState;

typedef enum
{
    DATA_SENDING = 0,      //�������ڷ���
    DATA_FINISHING = 1     //���ݷ���ֹͣ
}euWaveDataState;

typedef enum
{
    DRVSTATE_OFF = 0,
    DRVSTATE_ON
    
}DriverStateEnum;    //mosfet�ܿ���״̬

typedef enum
{
    DRVCLK_CLOSE = 0,
    DRVCLK_OPEN
    
}DriverClkStateEnum;    //�͸�TMCоƬ��ʱ�ӿ���״̬

typedef enum
{
    DDR_NOERROR = 0,
    DDR_TESTERROR,    //�Բ��д�
    DDR_NOINIT        //��ʼ��δ���
    
}DdrStateEnum;    //��ʼ�������Ĵ���Դ



/******************************************��������*******************************************/



/******************************************��������*******************************************/
u8    servFpgaRegisterWrite(u8 chanNum, u16 regAddr, u16 regValue);
u8    servFpgaRegisterRead(u8 chanNum, u16 regAddr, u16 *pRegValue);
u8    servFpgaLoaclFileWrite(u32 writeAddr, char const* filePath);
u8    servFpgaLoaclFileLoad(char const* filePath);
u8    servFpgaFlashFileLoad(u32 loadAddr);
s32   servFPGA_SNOIC_Threshold_Set(float f32ThresDis,float f32tmeperature);
void  servFPGA_Sonic_Protect_Mode(euSonicProtMode mode);
float servFPGA_Sonic_Distance_Get(float  f32Temperature);
s32   serv_FIFO_Data_Write(OutputDataStruct stoutputdata);
void  servFPGA_PWM_Start(u8 chanNum);   
void  servFPGA_PWM_Cycle_Set(u8 chanNum, u32 times);
void  servFPGA_Cycle_Cnt_Get(u8 chanNum, u32 *times);

FpgaVersionStruct servFpgaVersionGet(void);
RunStatenum       servFpgaRunStateGet(u8 chanNum);
LevelStatusEnum   servFpgaDioStatusRead(DigitalIONumEnum digitalIONum);
DriverTypeEnum    servFpgaDrvBoardTypeGet(u8 chanNum);
DdrStateEnum      servFpgaDDRStateGet(void);

u32  servFPGA_PWM_FIFO_Left(u8 chanNum, u32 u32Total);
void servSetFpgaMultiSyncFunc(MultiSyncFuncEnum enFunc);
void servFPGAStepGet(s32 *s32Round,s32 *s32ABCnt,s32 *s32StepCnt);
u16  servFPGA_DRAM_SELFTEST(void);
s32  servFPGA_STAT_Get(u16* STAT);


u16  servFpgaIntrSourceGet(u8 chanNum);
void servFpgaDigitalOutLevelSet(DoutNumEnum dIONum, LevelStatusEnum levelStatus);
void servFpgaDigitalOutSet(DoutNumEnum dIONum, DigitalOutManageStruct digitalIO);
void servFpgaIsolatorOutLevelSet(YoutNumEnum youtNum, LevelStatusEnum levelStatus);
void servFpgaIsolatorOutSet(YoutNumEnum youtNum, IsolatorOutManageStruct isolOut);

bool servFpgaReadyQuery(u8 chanNum, WaveTableStruct waveTableAddr);
void servFpgaTriggerSet(u8 chanNum, TrigInManageStruct trigInInfo, EncoderChanEnum encChan);
void servFpgaEndStateSet(u8 chanNum, EndStateEnum endState, StopConfigStruct stopConfigInfo);

void servFpgaWaveEndIntSet(u8 chanNum, bool bEnable);
void servFpgaWaveInit(u8 chanNum, WaveTableStruct waveTable);
void servFpgaWaveModifyDutySet(u8 chanNum, ModifyDutyEnum modifyDuty);
void servFpgaWaveLineMultSet(u8 chanNum, u32 lineMult);
void servFpgaWaveCalcModeSet(u8 chanNum, MotionModeEnum motionMode);
void servFpgaWaveMotionModeSet(u8 chanNum, MotionModeEnum motionMode, f32 lineSteps, u32 lineMult, ModifyDutyEnum modifyDuty);
s32  servFpgaWaveDataWrite(u8 chanNum, OutputDataStruct *pOutputData, WaveTableStruct *pWaveTable);
void servFpgaWaveReset(WaveTableStruct *pWaveTable, WaveWorkModeEnum waveWorkMode);
void servFpgaWavePrefetch(u8 chanNum);
void servFpgaWaveEnable(u8 chanNum, bool outputEnable);
void servFpgaWaveStart(void);                      
void servFpgaWaveStop(u8 chanNum, StopConfigStruct stopConfigInfo);
void servFpgaWaveEmergStop(u8 chanNum);

bool servFpgaReadyStateQuery(void);
void servFpgaEncoderSet(u8 chanNum, IntfcStateEnum encoderState, EncoderMultiEnum encMult, EncoderChanEnum  encChan);
void servFpgaEncoderCountReset(u8 chanNum);
void servFpgaEncoderCountRead(u8 chanNum, s16 *pCircleNum, s32 *pABCount, s32 *pSteps);
void servFpgaLineOutOfStepSet(u8 chanNum, SensorStateEnum  state, f32 lineSteps, u16 threshold);
void servFpgaDriverFromArm(u8 chanNum);
s32  servFpgaDriverCofing(u8 chanNum, DriverTypeEnum deviceType, DriverStateEnum driverState, DriverClkStateEnum driverClkState);
void servFpgaDriverSelect(u8 chanNum);
void servFpgaStartSourceSet(u8 chanNum, StartSourceEnum startSource);
void servFpgaClockSyncSet(ClockSyncRegEnum ClockSyncReg, ReceiveTypeEnum receiveType, CanIntfcStruct canIntfc);
u32  servFpgaClockSyncCountRead(void);
void servFpgaDDRErrorClear(void);
void servFpgaSensorUart1RxSelect(SensorNumEnum sensorNum);
void servFpgaSensorUart2RxEnable(bool enable);
void servFpgaSensorUart2RxSelect(SensorNumEnum sensorNum);
void servFpgaReverseMotionSet(u8 chanNum, SensorStateEnum  revMotion);
void servFpgaWavePrepareConfig(u8 chanNum, WaveTableStruct *pOutpWaveTable, PlanManageStruct wavePlanInfo, PosnConvertInfoStruct posnConvertInfo);

void servFpgaAbsEncReadTrig(SensorNumEnum sensorNum);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
