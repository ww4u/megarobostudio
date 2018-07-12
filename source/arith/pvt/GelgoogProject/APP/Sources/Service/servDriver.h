/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名: servDriver.h；
功能描述:     ;
函数列表:     ;
当前版本:     1.0.0;
版本改动:     ;
作     者:  CJ;
完成日期:     2017.10.25;
历史版本:     无;
*********************************************************************************************/
#ifndef __SERV_DRIVER_H__
#define __SERV_DRIVER_H__



#include "comDataType.h"
#include "comStruct.h"
#include "comEnum.h"
#include "comDebug.h"


#define R_SENSE_2660     75   //2660 使用75毫欧姆感应电阻   
#define R_SENSE_262      40   //262  40毫欧姆

#define CURR_MAX_262     8  //262支持的最大电流
#define CURR_MAX_2660    4  //2660支持的最大电流


//当使用STEP/DIR信号时 TMC262的寄存器地址位于数据位的17/18/19位
//当不使用STEP/DIR信号时 ADDR_262_DRVCTRL的地址为 18/19位 --(0x00<<18)

#define ADDR_TMC_DRVCTRL    (0x00)  // DRVCTRL 寄存器地址
#define ADDR_TMC_CHOPCONF   (0x04)  //
#define ADDR_TMC_SMARTEN    (0x05)  //
#define ADDR_TMC_SGCSCONF   (0x06)  //
#define ADDR_TMC_DRVCONF    (0x07)  //

#define RDSEL_MICRO_POS   0x00    //回读位置
#define RDSEL_SG_LVL      0x01    //回读stallguard 级别
#define RDSEL_SG_CS_LVL   0x02    //回读stallguard和coolstep 电流

typedef int8_t   SGTValue ;//SGT的数据类型  实际为 int8 真实取值范围时 -64   --- +63
typedef uint8_t  CSValue  ;//电流缩放数值
#define NCFG        127        //不需要配置时的参数
typedef enum
{
    INTPOL_ENABLE = 1,   //DRVCTRL 插值功能使能
    INTPOL_DISABLE = 0   //DRVCTRL 插值功能禁用
}euIntpolCtrl;


typedef enum
{
    EDGE_RISEING = 0,  // step信号的上升沿有效
    EDGE_DOUBLE = 1  // step信号的上升沿和下降沿都有效
}euEdgeCtrl;


//SMARTEN


typedef enum
{
    MIN_CS_1_2 = 0,      //smarten 电流最小值为1/2 CS
    MIN_CS_1_4 = 1       //smarten 电流最小值为1/4 CS
}euMinCS;


typedef enum
{
    SEDN_32 = 0,  //smarten SG值至少32次达到高阈值时 电流下降
    SEDN_8 = 1,  //smarten SG值至少8次达到高阈值时 电流下降
    SEDN_2 = 2,
    SEDN_1 = 3
}euDownSpeed;

typedef enum
{
    SEUP_8 = 3,  //smarten SG值低于低阈值时电流提高8步级别
    SEUP_4 = 2,  //smarten SG值低于低阈值时电流提高4步级别
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
    SFILT_ENABLE = 1,   //开启滤波 每4整步进行一次堵转报警
    SFILT_DISABLE = 0   //关闭滤波 
}euSGfiltCtrl;


//CHOPCONF
typedef enum
{
    TBL_16 = 0,   //800ns
    TBL_24 = 1,   //1.2us
    TBL_36 = 2,   //1.8us
    TBL_54 = 3    //2.7us
}euBlankTIme; //消隐时间  以周期为单位

typedef enum  //斩波模式
{
    CHOP_SPREAD = 0, //spreadcycle
    CHOP_CONST = 1   //constant off time with fast decay
}euChopMode; 


typedef enum  //OFF time的模式
{
    OFFTIME_FIXED = 0,  //固定时间模式
    OFFTIME_RANDOM = 1   //随机时间模式
}euRNDTFMode; 


typedef enum
{
    OFFTIM_SHUTOFF = 0,
    OFFTIM_44_CLK =  1,   //只能在TBL为 24clocks时使用          2.2us
    OFFTIM_76_CLK =  2,   //offtime时间为 12+(32*2) = 76clks    3.8us
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
}euChopOffTime;       //off time 设置
 

typedef enum
{
    FD_TERMI_BOTH = 0, //可以被电流比较器和计时器终结
    FD_TERMI_TIME = 1, //只可以被计时器终结
}euFastDecayMode;     //constant Toff模式下,fast decay 可以被终结的选择

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
}euHSTRT;   //spreadcycle模式下,HSTRT的值



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
}euHEND;   //spreadcycle模式下,HEND的值


typedef enum
{
    HDEC_16 = 0,
    HDEC_32 = 1,
    HDEC_48 = 2,
    HDEC_64 = 3
}euHDEC;   //spreadcycle模式下,HDEC的值  单位为 fclk



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
}euSineOffset;   //constant Toff模式下,给sine波表的偏移值


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
}euFDTime;   //const offtime and fast decay time模式下,fast time 时间





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
    VENSE_305MV = 0,    //设置VENSE的值为305mV   在感应电阻值为75m欧姆的情况下 输出电流峰值为 305/75 = 4.1A     有效值为 2.9A
    VENSE_165MV = 1     //设置VENSE的值为165mV   在感应电阻值为75m欧姆的情况下 输出电流峰值为 165/75 = 2.2A     有效值为 1.55A
}euVsenseCtrl;



#if 0
typedef enum
{
    DRIVER_2660 = 0,  // 驱动器为  2660 
    DRIVER_262 = 1     //
}DriverTypeEnum;   //驱动器型号

#endif 





typedef struct
{
    u8 bStall:1; //  stallguard状态, 为1表示达到堵转阈值 SG_TST会被拉高
    u8 bOverTempDown:1; // 1 表示 温度达到150  发生芯片关闭保护
    u8 bOverTempWarn:1; // 1 表示 温度达到100  发生过温报警
    u8 bS2GA:1;         // 1 表示 A路高端mosfet发生接地短路
    u8 bS2GB:1;         // 1 表示 B路高端mosfet发生接地短路
    u8 bOLA:1;          // 1 表示 A路负载开路  应是电机没接或者线圈断了
    u8 bOLB:1;          // 1 表示 B路负载开路  应是电机没接或者线圈断了
    u8 bSTST:1;         // 1 表示 处于停止状态,在过去的一段时间内没有接收到步进触发信号
}stTMC26xxWarn; //TMC2660和TMC262的报警状态



typedef union
{
    struct {
      u32 CB:8;//B相电流值索引
      u32 PHB:1;//B相电流方向
      u32 CA:8;//A相电流值索引
      u32 PHA:1;//A相电流方向
      u32 addr:2;//本寄存器地址
      u32 resv_bit20_31:12;//保留位
    }stCtrl1;
    u32 u32Ctrl1;
}UN_DRVCTRL_SPI;
typedef union
{
    struct {
        u32 MRES:4;//细分设置
        u32 resv_bit4_7:4;//保留位
        u32 DEDGE:1;//边沿模式
        u32 INTPOL:1;//内部插值控制 0:禁用内部插值,1:使能内部插值
        u32 resv_bit10_17:8;//保留位
        u32 addr:2;//本寄存器地址
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
        u32 CHM:1; //斩波模式设置,0:扩展周期模式;1:传统的恒定快速衰减时间
        u32 TBL:2;//消隐时间
        u32 addr:3;//寄存器地址
        u32 resv_bit20_31:12;
    }stChopConf;
    u32 u32ChopConf;
}UN_CHOPCONF;
typedef union
{
    struct{
        u32 SEMIN:4;//当SG小于SEMIN时,调整电流
        u32 resv_bit4:1;
        u32 SEUP:2;//电流增加比例因子
        u32 resv_bit7:1;
        u32 SEMAX:4;//当SG高于SEMAX时,调整电流
        u32 resv_bit12:1;
        u32 SEDN:2;//电流减小比例因子
        u32 SEIMIN:1;//最低电流缩放比例限制
        u32 resv_bit16:1;
        u32 addr:3;//寄存器地址
        u32 resv_bit20_31:12;
    }stSmarten;
    u32 u32Smarten;
}UN_SMARTEN;
typedef union
{
    struct
    {
        u32 CS:5;//电流缩放比例
        u32 resv_bit5_7:3;
        u32 SGT:7;//电机失速指示阈值 stall guard threshold
        u32 resv_bit15:1;
        u32 SFILT:1;//是否进行滤波
        u32 addr:3;//寄存器地址
        u32 resv_bit20_31:12;
    }stSgcsConf;
    u32 u32SgcsConf;
}UN_SGCSCONF;

typedef union
{
    struct{
        u32 resv_bit0_3:4;
        u32 RDSEL:2;//回读寄存器选择
        u32 VSENSE:1;
        u32 SDOFF:1;//关闭STEP/DIR接口
        u32 TS2G:2;//接地短路检测时间
        u32 DISS2G:1;//接地短路保护使能
        u32 resv_bit11:1;
        u32 SLPL:2;
        u32 SLPH:2;
        u32 TST:1;//测试
        u32 addr:3;//寄存器地址
        u32 resv_bit20_31:12;
    }stDrvConf;
    u32 u32DrvConf;
}UN_DRVCONF;

typedef enum
{
    MOTOR_SPEED_TYPE_LOW = 0,
    MOTOR_SPEED_TYPE_MID = 1,
    MOTOR_SPEED_TYPE_HIGH = 2
}MOTOR_SPEED_TYPE;    //mosfet管开关状态

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
