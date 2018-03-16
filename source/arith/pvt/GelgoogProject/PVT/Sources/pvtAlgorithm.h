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
#include "comStruct.h"
#include "comEnum.h"



/****************************************外部变量声明*****************************************/



/*******************************************宏定义********************************************/
#define    OUTPUT_PERIOD_MIN              7    //输出周期最小值

#define    SQUARE(num)                    (num * num)
#define    CUBE(num)                      (num * num * num)
#define    TARGET_REAL_OFFSET_P           0.02f                        //计算PVT每步速度时，实际步数和目标步数之间最大误差
#define    TARGET_REAL_OFFSET_N           TARGET_REAL_OFFSET_P * -1    //计算PVT每步速度时，实际步数和目标步数之间最大误差

#define    PVT_ONE_POINT_CALC_TIMES_MAX    1000    //计算一个点的最大计算次数

//PVT计算错误码
#define    PVT_CALC_NO_ERROR               0
#define    PVT_CALC_ERROR_TYPE             1
#define    PVT_CALC_ERROR_LEN              2
#define    PVT_CALC_ERROR_INDEX            3
#define    PVT_CALC_LESS_LOWER_LIMIT       4
#define    PVT_CALC_GREAT_UPER_LIMIT       5
#define    PVT_CALC_TOO_MANY_CALC_TIMES    6
#define    PVT_CALC_WRONG_ACC_PLAN         7



/***************************************常数和类型声明****************************************/
typedef struct
{
#if 0
    bool           bQueryReady;    //是否查询FPGA Ready状态
    bool           bReportCalcEnd; //是否上报了计算结束状态，FIFO模式下使用 CJ 2017.03.28 Add
    
    WaveTableTypeEnum calcWaveTable;    //当前使用的波表
    
    PlanModeEnum   pvtPlanMode;   //PVT规划模式
    ExecModeEnum   pvtExecMode;   //PVT执行模式
    
    MotionModeEnum motionMode;     //运动模式

    u16 lastPoint;      //上次计算的点的编号
    
    u32 fpgaPwmClock;   //FPGA PWM时钟

    s32 pvtSteps;
    
    PvtPointStruct startPoint;     //本次计算的两个PVT点
    PvtPointStruct endPoint;       //本次计算的两个PVT点

    PosnConvertInfoStruct posnConvertInfo;

    OutpBufferFillFunc  outpBufferFill;
    
#else

    bool           bQueryReady;    //是否查询FPGA Ready状态
    bool           bReportCalcEnd; //是否上报了计算结束状态，FIFO模式下使用 CJ 2017.03.28 Add
    
    WaveTableTypeEnum calcWaveTable;    //当前使用的波表
    
    PlanModeEnum   pvtPlanMode;   //PVT规划模式
    ExecModeEnum   pvtExecMode;   //PVT执行模式
    
    MotionModeEnum  motionMode;     //运动模式

    DirectionEnum   lastStepDir;    //上一步的方向

    u16 lastPoint;      //上次计算的点的编号

    u16 accScale;       //S曲线加速段时间占比
    u16 decScale;       //S曲线减速段时间占比

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
    f32 fpgaPwmClock;   //FPGA PWM时钟偏差
    
#else

    u32 fpgaPwmClock;   //FPGA PWM时钟
#endif

    s32 pvtSteps;

    f32 startPosn;      //一段PVT的起始位置
    f32 startSpeed;     //一段PVT的起始速度
    f32 startTime;      //一段PVT的起始时间
    f32 endPosn;        //一段PVT的结束位置
    f32 endSpeed;       //一段PVT的结束速度
    f32 motionTime;     //一段PVT的运行时间
        
    f32 initPosition;   //PVT初始点的位置
    f32 invsePosition;  //上次反向时的位置 
    f32 waitPosition;   //上次等待时的位置  
    f32 waitStepError;  //因为移动距离不够一步而变为等待时间的步数误差
    
    u32 targetStep;     //目标步数
    f32 lastStepSpeed;  //上一步的速度，实际记录的是时间
    f32 lastStepTime;   //上一步相对于整个PVT表0时刻的时间(PVT表的第一个点时间总是为0)
    
    u32 targetLine;     //目标线数
    f32 lastLineSpeed;  //上一线的速度，实际记录的是时间
    f32 lastLineTime;   //上一线相对于整个PVT表0时刻的时间(PVT表的第一个点时间总是为0)
    f32 remainLine;     //当前线计算步数后剩下的线数
    f32 lineErrorP;     //目标值与实际值的正误差
    f32 lineErrorN;     //目标值与实际值的负误差
    
    f32 errorTime;      //四舍五入导致的累计误差
    
    PvtPointStruct startPoint;     //本次计算的两个PVT点
    PvtPointStruct endPoint;       //本次计算的两个PVT点

    PosnConvertInfoStruct posnConvertInfo;

    OutpBufferFillFunc  outpBufferFill;
    
    void *pContext;    //上下文指针，上位机使用
    
#endif

}PvtCalcStruct;

/*-------------梯形插值的模式枚举-------------*/
typedef enum
{
    TRAPEZOID_ERROR = 0,    //错误模式
    TRAPEZOID_INTERP,       //插值模式
    TRAPEZOID_UNIFORM       //匀速模式
    
}TrapezoidModeEnum;



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
u8   pvtSegmentCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum);
void pvtPosnConvCoeffCalc(MotorManageStruct motorInfo, u8 drvMres, PosnConvertInfoStruct *pPosnConvertInfo);
s16  pvtStepToCircleCalc(s32 totalSteps, f32 convertCoeff, PostUnitEnum  posnUnit);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
