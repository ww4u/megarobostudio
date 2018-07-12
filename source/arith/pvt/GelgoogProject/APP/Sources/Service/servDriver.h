/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��: servDriver.h��
��������:     ;
�����б�:     ;
��ǰ�汾:     1.0.0;
�汾�Ķ�:     ;
��     ��:  CJ;
�������:     2017.10.25;
��ʷ�汾:     ��;
*********************************************************************************************/
#ifndef __SERV_DRIVER_H__
#define __SERV_DRIVER_H__



#include "comDataType.h"
#include "comStruct.h"
#include "comEnum.h"
#include "comDebug.h"


#define R_SENSE_2660     75   //2660 ʹ��75��ŷķ��Ӧ����   
#define R_SENSE_262      40   //262  40��ŷķ

#define CURR_MAX_262     8  //262֧�ֵ�������
#define CURR_MAX_2660    4  //2660֧�ֵ�������


//��ʹ��STEP/DIR�ź�ʱ TMC262�ļĴ�����ַλ������λ��17/18/19λ
//����ʹ��STEP/DIR�ź�ʱ ADDR_262_DRVCTRL�ĵ�ַΪ 18/19λ --(0x00<<18)

#define ADDR_TMC_DRVCTRL    (0x00)  // DRVCTRL �Ĵ�����ַ
#define ADDR_TMC_CHOPCONF   (0x04)  //
#define ADDR_TMC_SMARTEN    (0x05)  //
#define ADDR_TMC_SGCSCONF   (0x06)  //
#define ADDR_TMC_DRVCONF    (0x07)  //

#define RDSEL_MICRO_POS   0x00    //�ض�λ��
#define RDSEL_SG_LVL      0x01    //�ض�stallguard ����
#define RDSEL_SG_CS_LVL   0x02    //�ض�stallguard��coolstep ����

typedef int8_t   SGTValue ;//SGT����������  ʵ��Ϊ int8 ��ʵȡֵ��Χʱ -64   --- +63
typedef uint8_t  CSValue  ;//����������ֵ
#define NCFG        127        //����Ҫ����ʱ�Ĳ���
typedef enum
{
    INTPOL_ENABLE = 1,   //DRVCTRL ��ֵ����ʹ��
    INTPOL_DISABLE = 0   //DRVCTRL ��ֵ���ܽ���
}euIntpolCtrl;


typedef enum
{
    EDGE_RISEING = 0,  // step�źŵ���������Ч
    EDGE_DOUBLE = 1  // step�źŵ������غ��½��ض���Ч
}euEdgeCtrl;


//SMARTEN


typedef enum
{
    MIN_CS_1_2 = 0,      //smarten ������СֵΪ1/2 CS
    MIN_CS_1_4 = 1       //smarten ������СֵΪ1/4 CS
}euMinCS;


typedef enum
{
    SEDN_32 = 0,  //smarten SGֵ����32�δﵽ����ֵʱ �����½�
    SEDN_8 = 1,  //smarten SGֵ����8�δﵽ����ֵʱ �����½�
    SEDN_2 = 2,
    SEDN_1 = 3
}euDownSpeed;

typedef enum
{
    SEUP_8 = 3,  //smarten SGֵ���ڵ���ֵʱ�������8������
    SEUP_4 = 2,  //smarten SGֵ���ڵ���ֵʱ�������4������
    SEUP_2 = 1,
    SEUP_1 = 0
}euUpSize;

typedef enum
{
    SEMIN_COOL_DISABLE = 0,
    SEMIN_1 = 1,
    SEMIN_2 = 2,
    SEMIN_3 = 3,
    SEMIN_4 = 4,
    SEMIN_5 = 5,
    SEMIN_6 = 6,
    SEMIN_7 = 7,
    SEMIN_8 = 8,
    SEMIN_9 = 9,
    SEMIN_10 = 10,
    SEMIN_11 = 11,
    SEMIN_12 = 12,
    SEMIN_13 = 13,
    SEMIN_14 = 14,
    SEMIN_15 = 15
}euSEMin;

typedef enum
{
    SEMAX_P_1 = 0,
    SEMAX_P_2 = 1,
    SEMAX_P_3 = 2,
    SEMAX_P_4 = 3,
    SEMAX_P_5 = 4,
    SEMAX_P_6 = 5,
    SEMAX_P_7 = 6,
    SEMAX_P_8 = 7,
    SEMAX_P_9 = 8,
    SEMAX_P_10 = 9,
    SEMAX_P_11 = 10,
    SEMAX_P_12 = 11,
    SEMAX_P_13 = 12,
    SEMAX_P_14 = 13,
    SEMAX_P_15 = 14,
    SEMAX_P_16 = 15,
}euSEMax;


//SGCSCONFG



typedef enum
{
    SFILT_ENABLE = 1,   //�����˲� ÿ4��������һ�ζ�ת����
    SFILT_DISABLE = 0   //�ر��˲� 
}euSGfiltCtrl;


//CHOPCONF
typedef enum
{
    TBL_16 = 0,   //800ns
    TBL_24 = 1,   //1.2us
    TBL_36 = 2,   //1.8us
    TBL_54 = 3    //2.7us
}euBlankTIme; //����ʱ��  ������Ϊ��λ

typedef enum  //ն��ģʽ
{
    CHOP_SPREAD = 0, //spreadcycle
    CHOP_CONST = 1   //constant off time with fast decay
}euChopMode; 


typedef enum  //OFF time��ģʽ
{
    OFFTIME_FIXED = 0,  //�̶�ʱ��ģʽ
    OFFTIME_RANDOM = 1   //���ʱ��ģʽ
}euRNDTFMode; 


typedef enum
{
    OFFTIM_SHUTOFF = 0,
    OFFTIM_44_CLK =  1,   //ֻ����TBLΪ 24clocksʱʹ��          2.2us
    OFFTIM_76_CLK =  2,   //offtimeʱ��Ϊ 12+(32*2) = 76clks    3.8us
    OFFTIM_108_CLK = 3,   //5.4us
    OFFTIM_140_CLK = 4,
    OFFTIM_172_CLK = 5,  
    OFFTIM_204_CLK = 6,    //10.2us
    OFFTIM_236_CLK = 7,    //11.8us
    OFFTIM_268_CLK = 8,    //13.4us
    OFFTIM_300_CLK = 9,    //15us
    OFFTIM_332_CLK = 10,   //16.6us
    OFFTIM_364_CLK = 11,   //18.2us
    OFFTIM_396_CLK = 12,   //19.8us
    OFFTIM_428_CLK = 13,   //21.4us
    OFFTIM_460_CLK = 14,
    OFFTIM_492_CLK = 15   //24.6us
}euChopOffTime;       //off time ����
 

typedef enum
{
    FD_TERMI_BOTH = 0, //���Ա������Ƚ����ͼ�ʱ���ս�
    FD_TERMI_TIME = 1, //ֻ���Ա���ʱ���ս�
}euFastDecayMode;     //constant Toffģʽ��,fast decay ���Ա��ս��ѡ��

typedef enum
{
    HSTRT_1 = 0,
    HSTRT_2 = 1,
    HSTRT_3 = 2,
    HSTRT_4 = 3,
    HSTRT_5 = 4,
    HSTRT_6 = 5,
    HSTRT_7 = 6,
    HSTRT_8 = 7
}euHSTRT;   //spreadcycleģʽ��,HSTRT��ֵ



typedef enum
{
    HEND_N3 = 0,
    HEND_N2 = 1,
    HEND_N1 = 2,
    HEND_0 =  3,
    HEND_1 =  4,
    HEND_2 =  5,
    HEND_3 =  6,
    HEND_4 =  7,
    HEND_5 =  8,
    HEND_6 =  9,
    HEND_7 =  10,
    HEND_8 =  11,
    HEND_9 =  12,
    HEND_10 = 13,
    HEND_11 = 14,
    HEND_12 = 15
}euHEND;   //spreadcycleģʽ��,HEND��ֵ


typedef enum
{
    HDEC_16 = 0,
    HDEC_32 = 1,
    HDEC_48 = 2,
    HDEC_64 = 3
}euHDEC;   //spreadcycleģʽ��,HDEC��ֵ  ��λΪ fclk



typedef enum
{
    SINEOFFSET_N3 = 0,
    SINEOFFSET_N2 = 1,
    SINEOFFSET_N1 = 2,
    SINEOFFSET_0 =  3,
    SINEOFFSET_1 =  4,
    SINEOFFSET_2 =  5,
    SINEOFFSET_3 =  6,
    SINEOFFSET_4 =  7,
    SINEOFFSET_5 =  8,
    SINEOFFSET_6 =  9,
    SINEOFFSET_7 =  10,
    SINEOFFSET_8 =  11,
    SINEOFFSET_9 =  12,
    SINEOFFSET_10 = 13,
    SINEOFFSET_11 = 14,
    SINEOFFSET_12 = 15
}euSineOffset;   //constant Toffģʽ��,��sine�����ƫ��ֵ


typedef enum
{
    FDT_0_CLK = 0,
    FDT_32_CLK = 1,
    FDT_64_CLK = 2,
    FDT_96_CLK = 3,
    FDT_128_CLK = 4,
    FDT_160_CLK = 5,
    FDT_192_CLK = 6,
    FDT_224_CLK = 7,
    FDT_256_CLK = 8,
    FDT_288_CLK = 9,
    FDT_320_CLK = 10,
    FDT_352_CLK = 11,
    FDT_384_CLK = 12,
    FDT_416_CLK = 13,
    FDT_448_CLK = 14,
    FDT_480_CLK = 15
}euFDTime;   //const offtime and fast decay timeģʽ��,fast time ʱ��





//DRVCONF
typedef enum
{
    SLPH_MIN = 0,
    SLPH_MIN_TC = 1,
    SLPH_MID = 2,
    SLPH_MAX = 3,
}euSlopeH;

typedef enum
{
    SLPL_MIN = 0,
    SLPL_MIN_TC = 1,
    SLPL_MID = 2,
    SLPL_MAX = 3,
}euSlopeL;


typedef enum
{
    S2G_ENABLE = 0,
    S2G_DISABLE = 0,    
}euS2GCtrl;

typedef enum
{
    S2G_T_3_2 = 0,    //3.2us
    S2G_T_1_6 = 1,    //1.6us
    S2G_T_1_2 = 2,
    S2G_T_0_8 = 3
}euS2GTime;


typedef enum
{
    VENSE_305MV = 0,    //����VENSE��ֵΪ305mV   �ڸ�Ӧ����ֵΪ75mŷķ������� ���������ֵΪ 305/75 = 4.1A     ��ЧֵΪ 2.9A
    VENSE_165MV = 1     //����VENSE��ֵΪ165mV   �ڸ�Ӧ����ֵΪ75mŷķ������� ���������ֵΪ 165/75 = 2.2A     ��ЧֵΪ 1.55A
}euVsenseCtrl;



#if 0
typedef enum
{
    DRIVER_2660 = 0,  // ������Ϊ  2660 
    DRIVER_262 = 1     //
}DriverTypeEnum;   //�������ͺ�

#endif 





typedef struct
{
    u8 bStall:1; //  stallguard״̬, Ϊ1��ʾ�ﵽ��ת��ֵ SG_TST�ᱻ����
    u8 bOverTempDown:1; // 1 ��ʾ �¶ȴﵽ150  ����оƬ�رձ���
    u8 bOverTempWarn:1; // 1 ��ʾ �¶ȴﵽ100  �������±���
    u8 bS2GA:1;         // 1 ��ʾ A·�߶�mosfet�����ӵض�·
    u8 bS2GB:1;         // 1 ��ʾ B·�߶�mosfet�����ӵض�·
    u8 bOLA:1;          // 1 ��ʾ A·���ؿ�·  Ӧ�ǵ��û�ӻ�����Ȧ����
    u8 bOLB:1;          // 1 ��ʾ B·���ؿ�·  Ӧ�ǵ��û�ӻ�����Ȧ����
    u8 bSTST:1;         // 1 ��ʾ ����ֹͣ״̬,�ڹ�ȥ��һ��ʱ����û�н��յ����������ź�
}stTMC26xxWarn; //TMC2660��TMC262�ı���״̬



typedef union
{
    struct {
      u32 CB:8;//B�����ֵ����
      u32 PHB:1;//B���������
      u32 CA:8;//A�����ֵ����
      u32 PHA:1;//A���������
      u32 addr:2;//���Ĵ�����ַ
      u32 resv_bit20_31:12;//����λ
    }stCtrl1;
    u32 u32Ctrl1;
}UN_DRVCTRL_SPI;
typedef union
{
    struct {
        u32 MRES:4;//ϸ������
        u32 resv_bit4_7:4;//����λ
        u32 DEDGE:1;//����ģʽ
        u32 INTPOL:1;//�ڲ���ֵ���� 0:�����ڲ���ֵ,1:ʹ���ڲ���ֵ
        u32 resv_bit10_17:8;//����λ
        u32 addr:2;//���Ĵ�����ַ
        u32 resv_bit20_31:12;
    }stCtrlStepDir;
    u32 u32CtrlStepDir;
}UN_DRVCTRL_STEP_DIR;
typedef union
{
    struct{
        u32 TOFF:4;
        u32 HSTRT:3;
        u32 HEND:4;
        u32 HDEC:2;
        u32 RNDTF:1;
        u32 CHM:1; //ն��ģʽ����,0:��չ����ģʽ;1:��ͳ�ĺ㶨����˥��ʱ��
        u32 TBL:2;//����ʱ��
        u32 addr:3;//�Ĵ�����ַ
        u32 resv_bit20_31:12;
    }stChopConf;
    u32 u32ChopConf;
}UN_CHOPCONF;
typedef union
{
    struct{
        u32 SEMIN:4;//��SGС��SEMINʱ,��������
        u32 resv_bit4:1;
        u32 SEUP:2;//�������ӱ�������
        u32 resv_bit7:1;
        u32 SEMAX:4;//��SG����SEMAXʱ,��������
        u32 resv_bit12:1;
        u32 SEDN:2;//������С��������
        u32 SEIMIN:1;//��͵������ű�������
        u32 resv_bit16:1;
        u32 addr:3;//�Ĵ�����ַ
        u32 resv_bit20_31:12;
    }stSmarten;
    u32 u32Smarten;
}UN_SMARTEN;
typedef union
{
    struct
    {
        u32 CS:5;//�������ű���
        u32 resv_bit5_7:3;
        u32 SGT:7;//���ʧ��ָʾ��ֵ stall guard threshold
        u32 resv_bit15:1;
        u32 SFILT:1;//�Ƿ�����˲�
        u32 addr:3;//�Ĵ�����ַ
        u32 resv_bit20_31:12;
    }stSgcsConf;
    u32 u32SgcsConf;
}UN_SGCSCONF;

typedef union
{
    struct{
        u32 resv_bit0_3:4;
        u32 RDSEL:2;//�ض��Ĵ���ѡ��
        u32 VSENSE:1;
        u32 SDOFF:1;//�ر�STEP/DIR�ӿ�
        u32 TS2G:2;//�ӵض�·���ʱ��
        u32 DISS2G:1;//�ӵض�·����ʹ��
        u32 resv_bit11:1;
        u32 SLPL:2;
        u32 SLPH:2;
        u32 TST:1;//����
        u32 addr:3;//�Ĵ�����ַ
        u32 resv_bit20_31:12;
    }stDrvConf;
    u32 u32DrvConf;
}UN_DRVCONF;

typedef enum
{
    MOTOR_SPEED_TYPE_LOW = 0,
    MOTOR_SPEED_TYPE_MID = 1,
    MOTOR_SPEED_TYPE_HIGH = 2
}MOTOR_SPEED_TYPE;    //mosfet�ܿ���״̬

typedef struct {
 
    DriverTypeEnum   driver_type;
    u8  resv[3];
    u32  speed;
    UN_DRVCTRL_STEP_DIR unCtrlStepDir;
    UN_DRVCTRL_SPI unCtrlSpi;
    UN_CHOPCONF  unChopConf;
    UN_SMARTEN   unSmarten;
    UN_SGCSCONF  unSgcsConf;
    UN_DRVCONF   unDrvConf;
}STRU_MOTOR_CTRL;
extern  STRU_MOTOR_CTRL g_stTMC26xxDrvConf;



void servDriverCtrlEnable(bool enableDriver);
void servTMC_Ctrl_Cfg(u8 chanNum, DriverTypeEnum euDevice, MicroStepEnum microSteps, u8  motorCurr);
s32  servTMC_Reg_Read(u8 u8Reg,u32* ReadData);
s32 servTmcReadSelectedReg(u32* ReadData); //xyzheng add 
s32 servTMC_Reg_Write(u8 u8Reg);
void servTMC_MS_Set(MicroStepEnum euMS);
void serv_TMC_DRVCTRL_cfg(euIntpolCtrl intpol,euEdgeCtrl edge,MicroStepEnum MicroStep);
void servTMC_CurrentSet(u8 u8curr);

u16  servTMCStatusMonitor(void);

s32  servDriverRegWrite(u8 chanNum, u8 regNum, u32 regValue);
void servDriverStateMonitor(u8 chanNum, DriverMonitorStruct driverMonitor);
u8   servDriverStateRegRead(u8 chanNum, DriverReadRespEnum readReg, u32 *regValue);
void servDriverCurrRegParaCalc(DriverTypeEnum type, u8 curr, u8 *vsense, u8 *csValue);
void servDriverCurrentSet(u8 chanNum, DriverManageStruct driverManage);
u8   servDriverCurrCalc(DriverTypeEnum type, u8 vsense, u8 csValue);
void servDriverMicroStepSet(u8 chanNum, DriverManageStruct driverManage);
void servDriverConfig(u8 chanNum, DriverManageStruct driverManage);
void servDriverSwitch(u8 chanNum, DriverManageStruct driverManage);



#endif
/*****************************************END OF FILE****************************************/
