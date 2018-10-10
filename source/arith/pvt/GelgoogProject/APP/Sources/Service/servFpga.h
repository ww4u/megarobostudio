/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servFpga.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  无;
完成日期:  无;
历史版本:  无;
*********************************************************************************************/
#ifndef __SERV_FPGA_H__
#define __SERV_FPGA_H__



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comEnum.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/*******************************************宏定义********************************************/
#define    FPGA_REG_QUERY_COUNT_MAX    5


#define CMD_FPGA_R_D_LEN            0x01            //主机要从FPGA读出的数据长度 寄存器
#define CMD_FPGA_W_D_LEN            0x00            //主机要发送给FPGA的数据长度 寄存器  
#define CMD_FPGA_W_D_2_LEN          0x02            //主机要发送给FPGA的数据长度 寄存器 
#define CMD_FPGA_W_D_3_LEN          0x03            //主机要发送给FPGA的数据长度 寄存器 
#define CMD_FPGA_REG_ADDR           0x08            //主机从FPGA的寄存器读出数据命令   该命令后的地址段为 要读取的目的寄存器地址   



/******************************************************************************
                      寄存器地址和位域宏定义
******************************************************************************/
/******************************************************************************
FPGA调试寄存器(可读可写)    CJ 2017.07.12 Add
******************************************************************************/
#define    SERV_FPGA_DEBUG_REG            1


#if 0
//只是做个分界线
#endif


/**************************只读寄存器*****************************************/
/******************************************************************************
FPGA版本号寄存器(只读)    CJ 2017.09.09 Add
******************************************************************************/
#define    SERV_FPGA_VERSION_REG             0

#define    SERV_FPGA_VERSION_MAJOR_MASK      0x1E00     //bit[12:9]
#define    SERV_FPGA_VERSION_MAJOR_RSHIFT    9

#define    SERV_FPGA_VERSION_MINOR_MASK      0x01F0     //bit[8:4]
#define    SERV_FPGA_VERSION_MINOR_RSHIFT    4

#define    SERV_FPGA_VERSION_BUILD_MASK      0x000F     //bit[0:3]


/******************************************************************************
FPGA状态寄存器(只读)    CJ 2017.07.12 Add
******************************************************************************/
#define    SERV_FPGA_STATE_REG          2

#define    SERV_FPGA_DRIVE_TYPE_MASK    0x000E    //驱动板型号掩码 bit[1:3]
#define    SERV_FPGA_DRIVE_TYPE_262     2         //驱动板型号262  bit[1:3]
#define    SERV_FPGA_DRIVE_TYPE_2660    4         //驱动板型号2660 bit[1:3]


/******************************************************************************
FIFO未读波表点数寄存器(只读)    CJ 2018.07.05 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_03    3
#define    SERV_FPGA_RESPONSE_REG_04    4



/******************************************************************************
FPGA版本号寄存器(只读)    CJ 2017.09.09 Add
******************************************************************************/
#define    SERV_FPGA_TYPE_REG             5

#define    SERV_FPGA_TYPE_MAJOR_MASK      0x0F00     //bit[11:8]
#define    SERV_FPGA_TYPE_MAJOR_RSHIFT    8

#define    SERV_FPGA_TYPE_MINOR_MASK      0x00F0     //bit[7:4]
#define    SERV_FPGA_TYPE_MINOR_RSHIFT    4

#define    SERV_FPGA_TYPE_HARD_MASK       0x000F     //bit[0:3]


/******************************************************************************
SD帧检测寄存器(只读)    CJ 2018.07.07 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_06    6

typedef struct
{
    u16  resv_bit00_13 :14;
    
    u16  sdioWrError   :1;
    
    u16  resv_bit15    :1;
    
}FpgaResponseReg06Struct;

typedef union
{
    FpgaResponseReg06Struct regBitFiled;
    
    u16 regValue;
    
}FpgaResponseReg06Union;


/******************************************************************************
限位开关状态寄存器(只读)    CJ 2018.03.10 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_07    7


/******************************************************************************
反向间隙结果寄存器(只读)    CJ 2018.05.18 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_08    8


/******************************************************************************
PDM微步统计数寄存器(只读)    CJ 2018.06.12 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_09    9

#define    SERV_FPGA_MSTEP_COUNT_OFFSET    2


/******************************************************************************
DDR状态/电机状态寄存器(只读)    CJ 2017.07.12 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_10    10

typedef enum
{
    WAVESTATE_IDLE1 = 0,      //停止
    WAVESTATE_OUTP1,
    WAVESTATE_SD,
    WAVESTATE_IDLE2,
    WAVESTATE_OUTP2,
    WAVESTATE_HOLD,
    
    WAVESTATE_ERROR
    
}WaveStateEnum;

typedef struct
{
    u16  ddrInitDone      :1;
    u16  ddrWrAllow       :1;
    u16  waveDataReady    :1;
    u16  periodOverTx     :1;
    u16  periodOverDdr    :1;
    
    u16  resv_bit05_06    :2;
    
    u16  waveState        :4;
    
    u16  ddrCalibError    :1;
    
    u16  periodUnderTx    :1;
    u16  periodUnderDdr   :1;
    
    u16  resv_bit14_15    :4;
    
}FpgaResponseReg10Struct;

typedef union
{
    FpgaResponseReg10Struct regBitFiled;
    
    u16 regValue;
    
}FpgaResponseReg10Union;


/******************************************************************************
PDM微步统计结果寄存器(只读)    CJ 2018.06.12 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_11    11


/******************************************************************************
DRIVER状态寄存器(只读)    CJ 2017.10.26 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_12    12
#define    SERV_FPGA_RESPONSE_REG_13    13

#define    SERV_FPGA_DRIVER_STATE_H_MASK    0x000F    //bit[0:3]
#define    SERV_FPGA_DRIVER_STATE_H_LSHIFT  16

#define    SERV_FPGA_DRIVER_CHANNUM_MASK    0x0070    //bit[4:6]
#define    SERV_FPGA_DRIVER_CHANNUM_RSHIFT  4


#if GELGOOG_AXIS_10

typedef struct
{
    u16  faultState0  :1;
    u16  faultState1  :1;
    u16  faultState2  :1;
    u16  faultState3  :1;
    u16  faultState4  :1;
    u16  faultState5  :1;
    u16  faultState6  :1;
    u16  faultState7  :1;
    u16  faultState8  :1;
    u16  faultState9  :1;
    
    u16  resv_bit10_15   :6;
    
}FpgaResponseReg12Struct;

typedef union
{
    FpgaResponseReg12Struct regBitFiled;
    
    u16 regValue;
    
}FpgaResponseReg12Union;

#endif


/******************************************************************************
编码器计数寄存器(只读)    CJ 2017.07.19 Add
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

#define    SERV_FPGA_ABCNT_FCOUNT_MAX      0x10000      //正向计数最大值
#define    SERV_FPGA_ABCNT_RCOUNT_MIN      0x20000      //反向计数最小值
#define    SERV_FPGA_STEPS_FCOUNT_MAX      0x800000     //正向计数最大值
#define    SERV_FPGA_STEPS_RCOUNT_MIN      0xFFFFFF     //反向计数最小值


/******************************************************************************
循环数寄存器(只读)    CJ 2018.07.05 Add
******************************************************************************/
#define    SERV_FPGA_RESPONSE_REG_22    22
#define    SERV_FPGA_RESPONSE_REG_23    23


/******************************************************************************
时钟同步计数寄存器(只读)    CJ 2017.07.19 Add
******************************************************************************/
#define    SERV_FPGA_CLOCK_SYNC_COUNT_L_REG    24
#define    SERV_FPGA_CLOCK_SYNC_COUNT_M_REG    25
#define    SERV_FPGA_CLOCK_SYNC_COUNT_H_REG    26

#define    SERV_FPGA_CLOCK_SYNC_COUNT_REG      2

#define    SERV_FPGA_CLOCK_SYNC_COUNT_INVAILD  0x8000    //bit[15]


/******************************************************************************
DIO状态寄存器(只读)    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_DIO_STATUS_REG    27

#define    SERV_FPGA_DIO_STATUS_MASK   0x0001


/******************************************************************************
中断源寄存器(只读)    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_INTERRUPT_SRC_REG    28


/******************************************************************************
波表运行时间寄存器(只读)    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_WT_RUN_TIME_L_REG    29
#define    SERV_FPGA_WT_RUN_TIME_M_REG    30
#define    SERV_FPGA_WT_RUN_TIME_H_REG    31


/**************************只读寄存器*****************************************/


#if 0
//只是做个分界线
#endif


typedef enum
{
    BITOPT_DISABLE = 0,
    BITOPT_ENABLE,
    
}BitFiledOperateEnum;


/**************************只写寄存器*****************************************/
/******************************************************************************
DDR测试寄存器    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_DDR_TEST_REG      11

#define    SERV_FPGA_DDR_TEST_LEN      2

#define    SERV_FPGA_DDR_TEST_CLEAR    1    //bit[0]


/******************************************************************************
线间步倒数设置寄存器    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_LINE_STEPS_INVS_L_REG    12
#define    SERV_FPGA_LINE_STEPS_INVS_M_REG    13
#define    SERV_FPGA_LINE_STEPS_INVS_H_REG    14

#define    SERV_FPGA_LINE_STEPS_INVS_LEN    2

#define    SERV_FPGA_LINE_STEPS_MAX    0x100000000    //实际值由SERV_FPGA_LINE_STEPS_VAILD_BIT_VALUE决定


/******************************************************************************
线间步倒数有效位宽寄存器    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_LINE_STEPS_VAILD_BIT_REG    15

#define    SERV_FPGA_LINE_STEPS_VAILD_BIT_LEN    2

#define    SERV_FPGA_LINE_STEPS_VAILD_BIT_MASK     0x003F    //bit[0:5]
#define    SERV_FPGA_LINE_STEPS_VAILD_BIT_VALUE    32        //实际使用的是32bit


/******************************************************************************
驱动配置寄存器    CJ 2018.04.08 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_16    16

#if GELGOOG_AXIS_10

typedef enum
{
    DRIVER_STANDBY = 0,
    DRIVER_WORKING,
    
}DriverResetEnum;

//原理图上MODE的管脚焊接顺序里，M3和M2焊错反了，这里先用软件规避掉
//所以细分的顺序是乱的
typedef enum
{
    DRIVER_MODE1 = 0,
    DRIVER_MODE2,
    DRIVER_MODE16,
    DRIVER_MODE32,
    DRIVER_MODE4,
    DRIVER_MODE8,
    DRIVER_MODE128,
    DRIVER_MODE256,
    
}DriverModeEnum;

typedef enum
{
    DRIVER_MIXED = 0,
    DRIVER_SLOW,
    
}DriverDecayEnum;

typedef enum
{
    DRIVER_WAVE = 0,
    DRIVER_ARM,
    
}DriverSourceEnum;

typedef struct
{
    u16  drvReset  :1;
    u16  drvMode   :3;
    u16  drvDecay  :1;
    u16  drvSrc    :1;
    
    u16  resv_bit06_15   :10;
    
}FpgaConfigReg16Struct;


#else    //除10轴之外


typedef enum
{
    DRIVER_WAVE = 0,
    DRIVER_ARM,
    
}DriverSourceEnum;

typedef enum
{
    DRVCLK_CLOSE = 0,
    DRVCLK_OPEN
    
}DriverClkStateEnum;    //送给TMC芯片的时钟开关状态

typedef struct
{
    u16  drvType    :1;
    u16  tmc2660    :1;
    u16  tmc262     :1;
    u16  drvSysClk1 :1;
    
    u16  drvSrc     :2;
    
    u16  drvSysClk2 :1;
    u16  drvSysClk3 :1;
    u16  drvSysClk4 :1;
    u16  drvSysClk5 :1;
    u16  drvSysClk6 :1;
    u16  drvSysClk7 :1;
    u16  drvSysClk8 :1;
    
    u16  drvSpiSelect   :3;
    
}FpgaConfigReg16Struct;

#endif    //#if GELGOOG_AXIS_10

typedef union
{
    FpgaConfigReg16Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg16Union;


/******************************************************************************
驱动使能寄存器    CJ 2018.04.08 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_17    17

#if GELGOOG_AXIS_10
typedef struct
{
    u16  drvEnable0  :1;
    u16  drvEnable1  :1;
    u16  drvEnable2  :1;
    u16  drvEnable3  :1;
    u16  drvEnable4  :1;
    u16  drvEnable5  :1;
    u16  drvEnable6  :1;
    u16  drvEnable7  :1;
    u16  drvEnable8  :1;
    u16  drvEnable9  :1;
    
    u16  resv_bit10_15   :6;
    
}FpgaConfigReg17Struct;

#else

typedef struct
{
    u16  drvData;
    
}FpgaConfigReg17Struct;
#endif

typedef union
{
    FpgaConfigReg17Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg17Union;


/******************************************************************************
驱动读写寄存器    CJ 2017.08.25 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_18    18

typedef enum 
{
    DRVACT_WRITE = 0,
    DRVACT_READ
    
}DrvConfActionEnum;

typedef struct
{
    u16  drvData :4;
    u16  drvRead :1;
    
    u16  resv_bit05_15   :11;
    
}FpgaConfigReg18Struct;

typedef union
{
    FpgaConfigReg18Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg18Union;


/******************************************************************************
波表类型写寄存器    CJ 2018.07.05 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_23    23

typedef enum
{
    DATATYPE_WAVE  = 1,    //写入的是波表
    DATATYPE_ENC   = 2,    //写入的是编码器校准系数
    DATATYPE_CACHE = 3,    //写入的是CACHE
    
}WaveDataTypeEnum;

typedef struct
{
    u16  waveType :4;
    
    u16  resv_bit04_15   :12;
    
}FpgaConfigReg23Struct;

typedef union
{
    FpgaConfigReg23Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg23Union;


/******************************************************************************
波表写地址寄存器    CJ 2018.02.01 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_24    24

typedef struct
{
    u16  regData;
    
}FpgaConfigReg24Struct;

typedef union
{
    FpgaConfigReg24Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg24Union;


/******************************************************************************
波表写地址寄存器    CJ 2018.02.01 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_25    25

typedef struct
{
    u16  regData;
    
}FpgaConfigReg25Struct;

typedef union
{
    FpgaConfigReg25Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg25Union;


/******************************************************************************
波表数据写寄存器    CJ 2018.07.05 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_26    26


/******************************************************************************
波表配置完成写寄存器    CJ 2018.07.05 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_27    27

typedef enum
{
    DDRWR_START = 0,
    DDRWR_FINISH
    
}DDRWriteEnum;

typedef struct
{
    u16  wrDdrFinish  :1;
    
    u16  resv_bit01_15   :15;
    
}FpgaConfigReg27Struct;

typedef union
{
    FpgaConfigReg27Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg27Union;


/******************************************************************************
反向间隙测量选择寄存器    CJ 2018.05.18 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_28    28

typedef enum
{
    BLCH_CH1 = 0,
    BLCH_CH2,
    BLCH_CH3,
    BLCH_CH4,
    
}BackLashChanEnum;

typedef enum
{
    BLRESET_ON = 1,
    
}BackLashResetEnum;

typedef struct
{
    u16  chSelect  :2;
    
    u16  resv_bit02_03   :2;
    
    u16  blReset   :1;
    
    u16  resv_bit05_15   :11;
    
}FpgaConfigReg28Struct;

typedef union
{
    FpgaConfigReg28Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg28Union;


/******************************************************************************
PDM微步统计寄存器    CJ 2018.06.12 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_29    29

typedef enum
{
    SAMPLECH_CH1 = 0,
    SAMPLECH_CH2,
    SAMPLECH_CH3,
    SAMPLECH_CH4,
    
}SampleChanEnum;

typedef enum
{
    SAMPLESTATE_START = 1,
    SAMPLESTATE_END   = 2,
    SAMPLESTATE_RESET = 4,
    
}SampleStateEnum;

typedef struct
{
    u16  chSelect      :2;
    
    u16  resv_bit02_03 :2;
    
    u16  sampleState   :3;
    
    u16  cacheMode     :1;
    
    u16  encDiv        :8;
    
}FpgaConfigReg29Struct;

typedef union
{
    FpgaConfigReg29Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg29Union;


/******************************************************************************
波表RUN/STOP控制寄存器    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_31    31

typedef enum
{
    WSTOPMODE_FIFO  = 1,
    WSTOPMODE_CYCLE = 2,
    
}WaveStopModeEnum;

typedef enum
{
    CSSTATE_RESET = 1,
    CSSTATE_ON    = 2,
    CSSTATE_OFF   = 4,
    
}ClkSynStateEnum;

typedef enum
{
    WMMODE_PVT         = 0,
    WMMODE_LVT_CORR    = 1,
    WMMODE_LVT_NOCORR  = 3,
    
}WaveMotionModeEnum;

typedef struct
{
    u16  waveStopMode :2;

    u16  resv_bit02   :1;

    u16  dirReverse   :1;    //没有通道属性

    u16  startSrcSoft :1;
    u16  startSrcHard :1;
    u16  startSrcCan  :1;

    u16  resv_bit07   :1;

    u16  clkSynState  :3;

    u16  resv_bit11   :1;

    u16  motionMode   :2;

    u16  lvtModify    :2;
    
    
}FpgaConfigReg31Struct;

typedef union
{
    FpgaConfigReg31Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg31Union;


/******************************************************************************
波表RUN/STOP控制寄存器    CJ 2017.07.09 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_32    32

typedef enum
{
    WAVEOPT_DISABLE = 0,
    WAVEOPT_ENABLE,
    
}WaveOperateEnum;

typedef enum
{
    SDTYPE_NONE  = 0,    //
    SDTYPE_IMMED = 1,    //直接停止
    SDTYPE_DIST  = 2,    //按距离停止
    SDTYPE_SPEED = 4,    //按速度停止
    SDTYPE_TIME  = 8,    //按时间停止
    
}StopDecTypeEnum;

typedef enum
{
    WHSRC_NONE = 0,    //不触发
    WHSRC_SOFT = 1,    //软件触发
    WHSRC_HARD = 2,    //硬件触发
    WHSRC_END  = 4,    //波表输出结束触发(目前唯一支持的方式)
    
}WaveHoldSourceEnum;

typedef struct
{
    u16  wavePrepare :1;
    u16  waveOutput  :1;
    u16  waveStop    :1;
    u16  waveReset   :1;
    
    u16  sdSrcSoft   :1;    //软件触发急停
    u16  sdSrcHard   :1;    //数字输入触发急停
    u16  sdSrcEnd    :1;    //波表输出结束触发急停

    u16  resv_bit07  :1;

    u16  stopDecType :4;    //bit[8:11]--[立即停止;按距离停止;按减速度停止;按时间停止]

    u16  waveRunAll  :1;

    u16  waveHoldSrc :3;    //bit[13:15]--[软件触发;硬件触发;波表输出结束触发(目前唯一支持的方式)]
    
    
}FpgaConfigReg32Struct;

typedef union
{
    FpgaConfigReg32Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg32Union;


/******************************************************************************
波表起始地址配置寄存器    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_33    33

typedef struct
{
    u16  regData;
    
}FpgaConfigReg33Struct;

typedef union
{
    FpgaConfigReg33Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg33Union;


/******************************************************************************
波表起始地址配置寄存器    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_34    34

typedef struct
{
    u16  regData;
    
}FpgaConfigReg34Struct;

typedef union
{
    FpgaConfigReg34Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg34Union;


/******************************************************************************
波表长度配置寄存器    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_35    35

typedef struct
{
    u16  regData;
    
}FpgaConfigReg35Struct;

typedef union
{
    FpgaConfigReg35Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg35Union;


/******************************************************************************
波表长度配置寄存器    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_36    36

typedef struct
{
    u16  regData;
    
}FpgaConfigReg36Struct;

typedef union
{
    FpgaConfigReg36Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg36Union;


/******************************************************************************
波表循环数配置寄存器    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_WT_CYCLE_NUM_L_REG    37
#define    SERV_FPGA_CONFIG_REG_37    37

typedef struct
{
    u16  regData;
    
}FpgaConfigReg37Struct;

typedef union
{
    FpgaConfigReg37Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg37Union;


/******************************************************************************
波表循环数配置寄存器    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_WT_CYCLE_NUM_H_REG    38
#define    SERV_FPGA_CONFIG_REG_38    38

typedef struct
{
    u16  regData;
    
}FpgaConfigReg38Struct;

typedef union
{
    FpgaConfigReg38Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg38Union;


/******************************************************************************
编码器计数复位寄存器    CJ 2017.07.19 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_39    39

#define    SERV_FPGA_ENC_LINE_MULT_OFFSET    1

typedef enum
{
    EMSEL_NONE   = 0,
    EMSEL_DOUBLE = 1,
    EMSEL_QUADR  = 3,
    
}EncMultSelEnum;

typedef struct
{
    u16  encCountReset :1;
    
    u16  encMultSelect :2;

    u16  encSingleSel  :1;

    u16  resv_bit04_07 :4;

    u16  encDivSelect  :8;
    
}FpgaConfigReg39Struct;

typedef union
{
    FpgaConfigReg39Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg39Union;


/******************************************************************************
减速周期阈值配置寄存器    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_STOPDECEL_PERIOD_L_REG     40
#define    SERV_FPGA_STOPDECEL_PERIOD_H_REG     41

#define    SERV_FPGA_STOPDECEL_PERIOD_LEN       2

#define    SERV_FPGA_STOPDECEL_PERIOD_H_MASK    0x01FF    //bit[0:8]

#define    SERV_FPGA_STOPDECEL_PERIOD_H_RSHIFT  16


/******************************************************************************
减速斜率配置寄存器    CJ 2017.08.04 Add
******************************************************************************/
#define    SERV_FPGA_STOPDECEL_SLOPE_L_REG    42
#define    SERV_FPGA_STOPDECEL_SLOPE_M_REG    43
#define    SERV_FPGA_STOPDECEL_SLOPE_H_REG    44

#define    SERV_FPGA_STOPDECEL_SLOPE_LEN    2

//共48bit，整数部分为高25bit，小数部分为低23bit
#define    SERV_FPGA_STOPDECEL_SLOPE_DEC_VAILD_BITS    0x800000   //23bit有效
#define    SERV_FPGA_STOPDECEL_SLOPE_M_DEC_MASK        0x007F
#define    SERV_FPGA_STOPDECEL_SLOPE_M_INT_MASK        0x01FF

#define    SERV_FPGA_STOPDECEL_SLOPE_M_INT_LSHIFT    7
#define    SERV_FPGA_STOPDECEL_SLOPE_RSHIFT          16


/******************************************************************************
DO配置寄存器    CJ 2017.08.04 Add
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
UART3配置寄存器    CJ 2017.08.04 Add
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
停止开关寄存器和告警码型寄存器，两个寄存器的位域定义一样    CJ 2017.06.26 Add
******************************************************************************/
#define    SERV_FPGA_STOP_SWITCH_REG     47
#define    SERV_FPGA_ALARM_CODE_REG      48

#define    SERV_FPGA_TRIG_COND_LEN     3      //触发条件位域长度3bit
#ifdef PROJECT_GELGOOG
#define    SERV_FPGA_TRIG_IN_OFFSET    2      //触发输入顺序偏移
#else
#define    SERV_FPGA_TRIG_IN_OFFSET    0      //触发输入顺序偏移
#endif
#define    SERV_FPGA_TRIG_COND_NONE    0x00
#define    SERV_FPGA_TRIG_COND_LOW     0x04
#define    SERV_FPGA_TRIG_COND_RISE    0x05
#define    SERV_FPGA_TRIG_COND_FALL    0x06
#define    SERV_FPGA_TRIG_COND_HIGH    0x07

#define    SERV_FPGA_TRIG_ENABLE       0x07
#define    SERV_FPGA_TRIG_DISABLE      0x00


/******************************************************************************
触发条件逻辑关系寄存器    CJ 2017.06.26 Add
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
#define    SERV_FPGA_REF_CH4_ENC_SINGLE   0x0010    //接单线编码器
#define    SERV_FPGA_REF_CH4_ENC_TRIPLE   0x0020    //接三线编码器
#define    SERV_FPGA_REF_CH4_ENC_SENSOR   0x0030    //4通道编码器接口用于接传感器


/******************************************************************************
中断源设置寄存器    CJ 2017.06.26 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_50    50

typedef enum
{
    INT_DISABLE = 0,
    INT_ENABLE,
    
}IntStateEnum;

typedef struct
{
    u16  trigger      :1;
    u16  alarm        :1;
    u16  waveEnd      :1;
    u16  outOfStep    :1;
    u16  sdioCrcErr   :1;
    
    u16  resv_bit05_11   :7;
    
    u16  waveEndClear    :1;
    u16  pOvrTxClear     :1;    //无通道属性
    u16  pOvrDdrClear    :1;
    
    u16  resv_bit15      :1;
    
}FpgaConfigReg50Struct;

typedef union
{
    FpgaConfigReg50Struct regBitFiled;
    
    u16 regValue;
    
}FpgaConfigReg50Union;


/******************************************************************************
UART4接收选择寄存器    CJ 2017.11.02 Add
******************************************************************************/
#define    SERV_FPGA_UART4_RX_REG          51

#define    SERV_FPGA_UART4_SELECT_MASK    0x0007    //bit[0:2]
#define    SERV_FPGA_UART4_SELECT_SEN1    0x0002    //POS14
#define    SERV_FPGA_UART4_SELECT_SEN2    0x0003    //POS15
#define    SERV_FPGA_UART4_SELECT_SEN3    0x0004    //POS16
#define    SERV_FPGA_UART4_SELECT_SEN4    0x0001    //POS13


/******************************************************************************
DI滤波设置寄存器    CJ 2017.07.12 Add
******************************************************************************/
#define    SERV_FPGA_DI2_X1_FILTER_REG         52
#define    SERV_FPGA_DI3_X2_FILTER_REG         53
#define    SERV_FPGA_DI4_X3_FILTER_REG         54
#define    SERV_FPGA_DI5_X4_FILTER_REG         55
#define    SERV_FPGA_CHANZ_FILTER_REG          56

#define    SERV_FPGA_DI_FILTER_LEN             2

#define    SERV_FPGA_SPERIOD_US_ENABLE         0x8000        //bit[15]    采样周期200us，disable时为12.5ns
#define    SERV_FPGA_FILTER_TIME_MASK          0x7FFF        //bit[0:14]

#define    SERV_FPGA_SPERIOD_GEAR_VALUE        0.4f

#define    SERV_FPGA_SPERIOD_CODE_MIN          1

#define    SERV_FPGA_SPERIOD_US_VALUE          0.2f          //单位ms，实际值200us
#define    SERV_FPGA_SPERIOD_NS_VALUE          0.0000125f    //单位ms，实际值12.5ns
#define    SERV_FPGA_SPERIOD_OFFSET            1             //FPGA要求配置的时候减1


/******************************************************************************
失步告警阈值设置寄存器    CJ 2017.08.12 Add
******************************************************************************/
#define    SERV_FPGA_CONFIG_REG_57    57
#define    SERV_FPGA_CONFIG_REG_58    58

#define    SERV_FPGA_LINE_OUT_OF_STEP_LEN      2


/******************************************************************************
绝对值编码器读取寄存器    CJ 2018.02.28 Add
******************************************************************************/
#define    SERV_FPGA_ABS_ENC_REDA_REG          59

#define    SERV_FPGA_ABS_ENC_REDA_CMD          0x011A


/**************************只写寄存器*****************************************/



/***************************************常数和类型声明****************************************/
typedef enum 
{
    FPGAACT_READ = 0,
    FPGAACT_WRITE
    
}FpgaActionEnum;

typedef enum
{
    RUNSTETE_NORUN = 0,      //停止
    RUNSTETE_RUN,
    RUNSTETE_SD,
    RUNSTETE_HOLD,
    RUNSTETE_ERROR,
    
}RunStatenum;    //逻辑波表输出状态  


typedef enum
{
    SONIC_ARM_PROTECT = 0,    //逻辑检测到碰撞危险后不做任何动作
    SONIC_FPGA_PROTECT = 1    //逻辑检测到碰撞危险后主动进行保护动作
}euSonicProtMode;



typedef enum
{
    RUN_CTRL = 0,  //写入的是波表
    STOP_CTRL = 1   //写入的是编码器校准系数
}euRunStopCtrl;
 
 typedef enum
 {
     FIFO_MODE = 1,     //FIFO运行模式
     CYCLE_MODE = 2     //循环运行模式
 }euWaveRunMode;


typedef enum
{
    WAVE_RUNNING = 0,      //电机处于运行状态
    WAVE_STOPPING = 1      //电机处于停止状态
}euWaveRunState;

typedef enum
{
    DATA_SENDING = 0,      //数据正在发送
    DATA_FINISHING = 1     //数据发送停止
}euWaveDataState;

typedef enum
{
    DRVSTATE_OFF = 0,
    DRVSTATE_ON
    
}DriverStateEnum;    //mosfet管开关状态

typedef enum
{
    DDR_NOERROR = 0,
    DDR_TESTERROR,    //自测有错
    DDR_NOINIT        //初始化未完成
    
}DdrStateEnum;    //开始输出波表的触发源


typedef struct
{        
    u16 digitalOutputReg;              //DO配置寄存器 45#
    u16 encandUartReg;                 //编码器和UART配置寄存器 46#
    
    u16 trigRelatRefCfgReg[CH_TOTAL];  //触发逻辑关系和REF配置寄存器 49#
    
    u16 sensorUart2SelectReg;          //Uart选择寄存器 51#

    FpgaConfigReg16Union    register16;
    
    FpgaConfigReg17Union    register17;
    
    FpgaConfigReg18Union    register18;

    FpgaConfigReg23Union    register23[CH_TOTAL];

    FpgaConfigReg24Union    register24;

    FpgaConfigReg25Union    register25;

    FpgaConfigReg27Union    register27;

    FpgaConfigReg28Union    register28;

    FpgaConfigReg29Union    register29;

    FpgaConfigReg31Union    register31[CH_TOTAL];

    FpgaConfigReg32Union    register32[CH_TOTAL];

    FpgaConfigReg33Union    register33[CH_TOTAL];

    FpgaConfigReg34Union    register34[CH_TOTAL];

    FpgaConfigReg35Union    register35[CH_TOTAL];

    FpgaConfigReg36Union    register36[CH_TOTAL];

    FpgaConfigReg37Union    register37[CH_TOTAL];

    FpgaConfigReg38Union    register38[CH_TOTAL];

    FpgaConfigReg39Union    register39[CH_TOTAL];

    FpgaConfigReg50Union    register50[CH_TOTAL];
    
}FpgaWriteableRegValueStruct;



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
u8    servFpgaRegisterWrite(u8 chanNum, u16 regAddr, u16 regValue);
u8    servFpgaRegisterRead(u8 chanNum, u16 regAddr, u16 *pRegValue);

u8    servFpgaLoaclFileWrite(u32 writeAddr, char const* filePath);
u8    servFpgaLoaclFileLoad(char const* filePath);
u8    servFpgaFlashFileLoad(u32 loadAddr);

void  servFPGA_Cycle_Cnt_Get(u8 chanNum, u32 *times);

FpgaVersionStruct servFpgaVersionGet(void);
RunStatenum       servFpgaRunStateGet(u8 chanNum);
LevelStatusEnum   servFpgaDioStatusRead(DigitalIONumEnum digitalIONum);
DriverTypeEnum    servFpgaDrvBoardTypeGet(u8 chanNum);
DdrStateEnum      servFpgaDDRStateGet(void);

u32  servFpgaWaveWritableSizeRead(u8 chanNum, u32 totalSize);
u8   servFpgaSdioWrErrorRead(void);
void servFPGAStepGet(s32 *s32Round,s32 *s32ABCnt,s32 *s32StepCnt);
u16  servFPGA_DRAM_SELFTEST(void);
u8   servFpgaIntrSourceGet(u8 chanNum, u16 *intrSrcValue);
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
void servFpgaWaveTableInit(WaveTableStruct *pWaveTable, WaveWorkModeEnum waveWorkMode);
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
void servFpgaDriverSelect(u8 chanNum);
u8   servFpgaDriverConfig(u8 regNum, u32 regVaule, DrvConfActionEnum drvConfAct);
void servFpgaStartSourceSet(u8 chanNum, StartSourceEnum startSource);
void servFpgaClockSyncSet(ClockSyncRegEnum ClockSyncReg, ReceiveTypeEnum receiveType, CanIntfcStruct canIntfc);
u32  servFpgaClockSyncCountRead(void);
void servFpgaDDRErrorClear(void);
void servFpgaSensorUart1RxSelect(SensorNumEnum sensorNum);
void servFpgaSensorUart1TxSelect(SensorNumEnum sensorNum);
void servFpgaSensorUart2RxEnable(bool enable);
void servFpgaSensorUart2RxSelect(SensorNumEnum sensorNum);
void servFpgaReverseMotionSet(SensorStateEnum  revMotion);
void servFpgaWavePrepareConfig(u8 chanNum, WaveTableStruct *pOutpWaveTable, PlanManageStruct wavePlanInfo, PosnConvertInfoStruct posnConvertInfo);

u16  servFpgaDioRefStatusRead(void);

void servFpgaSdioCrcErrorIntSet(SensorStateEnum state);

#if GELGOOG_AXIS_10
void servFpgaDriverResetSet(DriverResetEnum drvReset);
void servFpgaDriverModeSet(MicroStepEnum microStep);
void servFpgaDriverDecaySet(DriverDecayEnum drvDecay);
void servFpgaDriverSourceSet(DriverSourceEnum drvSrc);
void servFpgaDriverEnable(u8 chanNum, SensorStateEnum  state);
u16  servFpgaDriverFaultRead(void);

#else

void servFpgaDriverClkCofing(u8 chanNum, DriverClkStateEnum driverClkState);
#endif

void servFpgaBackLashReset(BackLashChanEnum blChan);
u16  servFpgaBackLashRead(BackLashChanEnum blChan);

#if GELGOOG_SINANJU
void servFpgaAbsEncReadTrig(SensorNumEnum sensorNum);
void servFpgaPdmSampleStateSet(SampleChanEnum sampleChan, PdmInfoStruct pdmInfo);
u16  servFpgaPdmMstepCountRead(SampleChanEnum sampleChan);
void servFpgaPdmMstepDataProcess(SampleChanEnum sampleChan, PdmInfoStruct pdmInfo);
void servFpgaPdmCacheTestSet(void);
#endif

void servFpgaWaveEndIntClear(u8 chanNum);
void servFpgaPeriodOverTxClear(void);
void servFpgaPeriodOverDdrClear(u8 chanNum);
u8   servFpgaPeriodStateRead(u8 chanNum);

#if SDIO_TX_RX_TEST
void servFpgaDataLenSet(FpgaActionEnum fpgaAction, u16 realLen, u16 dataLen);
void servFpgaDDRDataTypeSet(u8 chanNum, WaveDataTypeEnum dateType);
void servFpgaPdmSampleReset(SampleChanEnum sampleChan);
void servFpgaPdmSampleStart(SampleChanEnum sampleChan);
void servFpgaPdmSampleEnd(SampleChanEnum sampleChan);
u16  servFpgaPdmMstepCountRead(SampleChanEnum sampleChan);
u8   servFpgaPdmMstepDataRead(SampleChanEnum sampleChan, u8 *pRegValue, u16 bufferSize);
#endif



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
