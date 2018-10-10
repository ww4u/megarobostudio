/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtalgorithm.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.08.22;
历史版本:  无;
*********************************************************************************************/
#ifndef __PVT_ALGORITHM_H
#define __PVT_ALGORITHM_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"



/****************************************外部变量声明*****************************************/



/*******************************************宏定义********************************************/
#define    OUTPUT_PERIOD_MIN              7    //输出周期最小值

#define    SQUARE(num)                    (num * num)
#define    CUBE(num)                      (num * num * num)
#define    PVT_TARGET_REAL_OFFSET_P       0.02f                            //计算PVT每步速度时，实际步数和目标步数之间最大误差
#define    PVT_TARGET_REAL_OFFSET_N       PVT_TARGET_REAL_OFFSET_P * -1    //计算PVT每步速度时，实际步数和目标步数之间最大误差
#define    LVT_TARGET_REAL_OFFSET_P       0.0003f                          //计算LVT每步速度时，实际线数和目标线数之间最大误差
#define    LVT_TARGET_REAL_OFFSET_N       LVT_TARGET_REAL_OFFSET_P * -1    //计算LVT每步速度时，实际线数和目标线数之间最大误差

#define    PVT_ONE_POINT_CALC_TIMES_MAX    1000    //计算一个点的最大计算次数

#define    SCALE_MAGNIFICATION             1000

//PVT计算错误码
#define    PVT_CALC_NO_ERROR               0
#define    PVT_CALC_ERROR_TYPE             1
#define    PVT_CALC_ERROR_LEN              2
#define    PVT_CALC_ERROR_INDEX            3

#define    PVT_CALC_SECTION_1              0
#define    PVT_CALC_SECTION_2              16
#define    PVT_CALC_SECTION_3              32

#define    PVT_CALC_LESS_LOWER_LIMIT       1
#define    PVT_CALC_GREAT_UPER_LIMIT       2
#define    PVT_CALC_TOO_MANY_CALC_TIMES    3
#define    PVT_CALC_SPEED_EQUAL_ZERO       4



/***************************************常数和类型声明****************************************/
/*******************枚举*********************/
/*-----------PVT规划模式枚举-----------*/
typedef enum
{
    PLANMODE_CUBIC = 0,    //三次多项式插值
    PLANMODE_TRAPEZOID,    //梯形插值
    PLANMODE_SCURVE,       //正弦
    
}PlanModeEnum;

/*-------------PVT模式枚举-------------*/
typedef enum
{
    EXECMODE_NCYCLE = 0,
    EXECMODE_FIFO,
    
}ExecModeEnum;

/*-------------运动方式枚举--------------*/
typedef enum
{
    MTNMODE_PVT  = 0,     //PVT模式
    MTNMODE_LVT_CORR,     //LVT带修正模式
    MTNMODE_LVT_NOCORR,   //LVT不带修正模式
    
}MotionModeEnum;

/*------------输出数据类型枚举------------*/
typedef enum
{
    OUTPDATA_FORWARD = 2,    //正向运行数据
    OUTPDATA_REVERSE,        //反向运行数据
    OUTPDATA_WAIT,           //等待时间
    
}OutpDataTypeEnum;

/*------------Buffer操作类型枚举------------*/
typedef enum
{
    BUFFOPERT_WRITE = 0,    //按顺序写入
    BUFFOPERT_FORCESEND,    //强制发送
    BUFFOPERT_SYNCSEND,     //为了同步进行的发送
    
}BufferOperateEnum;
/*******************枚举*********************/


typedef void (*OutpBufferFillFunc)(u8 chanNum, 
                                   OutpDataTypeEnum datatype, 
                                   u32 outpData, 
                                   BufferOperateEnum bufferOperate,
                                   void *pContext);


/******************结构体********************/
/*-------------PVT点结构体-------------*/
typedef struct
{
    f32 position;    //位置
    f32 speed;       //速度
    f32 time;        //时间
    u16 accScale;    //加速占比
    u16 decScale;    //减速占比
      
}PvtPointStruct;

/*-------------位置转换信息结构体-----------*/
typedef struct
{
    f32 posnToStep;      //位置到步数的转换系数
    f32 posnToLine;      //位置到编码器线数的转换系数 
    f32 lineSteps;       //线间步
    u32 lineMult;        //线数的倍乘
    
}PosnConvertInfoStruct;

typedef struct
{
    bool           bQueryReady;    //是否查询FPGA Ready状态
    bool           bReportCalcEnd; //是否上报了计算结束状态，FIFO模式下使用 CJ 2017.03.28 Add
    
    PlanModeEnum   pvtPlanMode;   //PVT规划模式
    ExecModeEnum   pvtExecMode;   //PVT执行模式
    
    MotionModeEnum   motionMode;     //运动模式

    OutpDataTypeEnum lastStepDir;    //上一步的方向

    u16 lastPoint;      //上次解算的PVT点的编号

    u32 fpgaPwmClock;      //FPGA PWM时钟
    f32 fpgaClockOffset;   //FPGA PWM时钟偏差

    u32 startPosn;      //一段PVT的起始位置
    f32 startSpeed;     //一段PVT的起始速度
    f32 startTime;      //一段PVT的起始时间
    u32 endPosn;        //一段PVT的结束位置
    f32 endSpeed;       //一段PVT的结束速度
    f32 motionTime;     //一段PVT的运行时间
    u32 motionStep;     //一段PVT的运行距离(微步数/线数)
        
    f32 invsePosition;  //上次反向时的位置 
    f32 waitPosition;   //上次等待时的位置  
    f32 waitStepError;  //因为移动距离不够一步而变为等待时间的步数误差
    
    u32 targetStep;     //目标步数
    f32 lastStepSpeed;  //上一步的速度，实际记录的是时间
    f32 maxOffset;      //和目标步数的误差最大值
    f32 minOffset;      //和目标步数的误差最大值

    f32 posnToStep;     //位置到步数的转换系数
    f32 lineSteps;      //线间步
    f32 lineStepsInv;   //线间步倒数

    f32 errorTime;      //四舍五入导致的累计误差
    
    PvtPointStruct startPoint;     //本次计算的两个PVT点
    PvtPointStruct endPoint;       //本次计算的两个PVT点

    OutpBufferFillFunc  outpBufferFill;
    
    void *pContext;    //上下文指针，上位机使用

    u64 timeCount;     //统计用

}PvtCalcStruct;
/******************结构体********************/



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
//u8 pvtSegmentCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
