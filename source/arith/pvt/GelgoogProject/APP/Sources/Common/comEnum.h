/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  comEnum.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.01.10;
历史版本:  无;
*********************************************************************************************/
#ifndef __COM_ENUM_H
#define __COM_ENUM_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"



/*******************************************宏定义********************************************/
#define    U32_VALUE_MAX              4294967296u
#define    PVT_POINT_BUFFER_SIZE      256
#define    SMALL_PVT_BUFFER_SIZE      4
#define    PRESET_PVT_BUFFER_SIZE     32

#define    OUTPUT_DATA_BUFFER_SIZE    1023          //波表Buffer的长度，值要是2^n - 1，最大4095(16384个字节)
#define    OUTPUT_EMPTY_BUFFER_SIZE   3             //波表空点数

#define    FPGA_PWM_CLOCK             20000000      //20MHz
#define    DEGREE_TO_RADIAN           0.0174533f    //度转弧度因子 = 3.1415926/180
#define    RADIAN_TO_DEGREE           57.295780f    //弧度转度因子 = 180/3.1415926

#define    PI_VALUE                   3.1415926

#define    DEVICE_SN_LEN       19    //字节数
#define    ERROR_CODE_LEN      6     
#define    SOFTWARE_VER_LEN    4
#define    FPGA_VER_LEN        4
#define    HARDWARE_VER_LEN    2
#define    BOOT_VER_LEN        2
#define    DRVBOARD_VER_LEN    3

#define    PERIOD_TASK_PERIOD   10      //周期性任务的周期时间(ms)

#define    NCYCLE_INFINITY                0xFFFFFFFF           //当上位机选择无限循环模式时 下位机发送给逻辑的次数   CP        

#define    APP_IMAGE_OK         0xA5    //主程序镜像是否可用
#define    VERIFY_SUCCESSFUL    0       //数据校验成功
#define    VERIFY_FAILED        -1      //数据校验失败

#define    ANGLE_CODE_TO_VALUE_FACTOR           0.005493164f    //角度传感器角度码值到实际值的转换系数(32768.0 * 180)
#define    VELOCITY_CODE_TO_VALUE_FACTOR        0.061035156f    //角度传感器角速度码值到实际值的转换系数(32768.0 * 2000)
#define    ACCELERATION_CODE_TO_VALUE_FACTOR    0.004785156f    //角度传感器加速度码值到实际值的转换系数(32768.0 * 16 * 9.8)

#define    ANALOG_IN_ADC_CH         ADC_Channel_10    //模拟输入对应的ADC通道
#define    ADC_CONVERT_Value_MAX    4095
#define    ADC_CONVERT_Value_MIN    0

#define    FRACTION_FRAME_DATA_LEN_MAX    5    //分数帧每帧最大发送的数据长度
#define    FRACTION(num, den)             (((num << 4) & 0xF0) + (den & 0x0F))

#define    EMERG_STOP_END_PERIOD    20000    //20000/20000000(FPGA_PWM_CLOCK)=0.001，对应的时间是1ms

#define    GROUP_NUM          2
#define    GROUP_NUM_VALUE_MIN    129
#define    GROUP_NUM_VALUE_MAX    255

#define    UNIT_S_TO_MS    1000

#ifdef PROJECT_QUBELEY
#define    ANGLE_SENSOR_NUM       1
#define    DIST_SENSOR_NUM        1
#else
#define    ANGLE_SENSOR_NUM       4
#define    DIST_SENSOR_NUM        4

#define    ABS_ENCODER_NUM        4
#endif

#define    PVT_CALC_NEED_POINT_NUM    2    //PVT计算需要的点数

#define    PVT_CALC_USE_FPGA_CLOCK_ERROR    1    //PVT计算过程中加入时钟误差

#define    PVT_CALC_QUEUE_SIZE    20    //PVT解算队列长度



/***************************************常数和类型声明****************************************/
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
    BUFFOPERT_MODIFYEND,    //修改末尾点
    
}BufferOperateEnum;



/***************************************常数和类型声明****************************************/
typedef void (*TimeOutFunc)(void *timeOutPara);
typedef void (*OutpBufferFillFunc)(uint8_t chanNum, 
                                   OutpDataTypeEnum datatype, 
                                   uint32_t outpData, 
                                   BufferOperateEnum bufferOperate,
                                   void *pContext);



/***************************************常数和类型声明****************************************/
/*-------------通道号枚举-----------*/
typedef enum
{
    CH_MIN = 0,     //通道下限
    CH_SYS = 0,     //系统通道
    CH1    = 0,     //通道1

#ifdef PROJECT_QUBELEY  

    CH_MAX = CH1,   //通道上限
    
#endif

#ifdef PROJECT_GELGOOG

    CH2,            //通道2
    CH3,            //通道3
    CH4,            //通道4

#if GELGOOG_AXIS_4 

    CH_MAX = CH4,   //通道上限
    
#elif GELGOOG_AXIS_8

    CH5,            //通道5
    CH6,            //通道6
    CH7,            //通道7
    CH8,            //通道8
    CH_MAX = CH8,   //通道上限
    
#endif

#endif    //#ifdef PROJECT_GELGOOG

    CH_TOTAL,       //总的通道数
    CH_ALL = 128    //所有通道
    
}ChannelEnum;

/*-------------工作模式枚举-------------*/
typedef enum
{
    WORK_NORMAL = 0,    //普通模式
    WORK_UNIFORM,       //匀速模式
    WORK_POSITION,      //位置反馈模式
    WORK_TORQUE         //力矩反馈模式
    
}WorkModeEnum;

/*-------------上电状态枚举-------------*/
typedef enum
{
    POWERON_DEFAULT = 0,    //使用默认值
    POWERON_LAST,           //使用上次值
    
}PowerOnModeEnum;

/*-------------产品类型枚举-------------*/
typedef enum
{
    MDMODEL_MRQ = 0,   //步进电机驱控器系列
    MDMODEL_MRV,       //电磁阀系列
    MDMODEL_MRX,       //机器人系列
    
}MainDeviceModelEnum;

/*-------------产品类型枚举-------------*/
typedef enum
{
    SDMODEL_C17D = 0,   //单轴，03/1.0版，2660
    SDMODEL_C23D,       //单轴，03/1.0版，262
    
    SDMODEL_M1703,      //3轴，2660
    SDMODEL_M2303,      //3轴，262
    
    SDMODEL_M1704,      //4轴，2660
    SDMODEL_M2304,      //4轴，262
    
    SDMODEL_M1706,      //6轴，2660
    SDMODEL_M2306,      //6轴，262
    
    SDMODEL_M1707,      //7轴，2660
    SDMODEL_M2307,      //7轴，262
    
    SDMODEL_M1708,      //8轴，2660
    SDMODEL_M2308,      //8轴，262
    
    SDMODEL_C17S,       //单轴，02/1.1版，2660
    SDMODEL_C23S,       //单轴，02/1.1版，262
    
    SDMODEL_ERROR,      //错误的型号
    
}SubDeviceModelEnum;

/*-------------电机驱动器型号枚举-------------*/
typedef enum
{
    DRIVER_2660 = 0,    // 驱动器为  2660 
    DRIVER_262  = 1,    //
    DRIVER_UNKNOW
    
}DriverTypeEnum;    

/*-------------电机微步枚举-------------*/
typedef enum
{
    MICROSTEP_256 = 0,
    MICROSTEP_128,
    MICROSTEP_64,
    MICROSTEP_32,
    MICROSTEP_16,
    MICROSTEP_8,
    MICROSTEP_4,
    MICROSTEP_2,
    MICROSTEP_1
    
}MicroStepEnum;

/*-------------电机步距角枚举-------------*/
typedef enum
{
    STEPANGLE_18 = 0,    //1.8°
    STEPANGLE_09,        //0.9°
    STEPANGLE_150,       //15°
    STEPANGLE_75         //7.5°
    
}StepAngleEnum;

/*-----------电机运动类型枚举-----------*/
typedef enum
{
    MOTOR_ROTARY = 0,    //旋转运动
    MOTOR_LINEAR         //直线运动
    
}MotorTypeEnum;

/*-------------电机尺寸枚举-------------*/
typedef enum
{
    MOTOR_08 = 0,
    MOTOR_11,
    MOTOR_14,
    MOTOR_17, //42
    MOTOR_23, //57
    MOTOR_24
    
}MotorSizeEnum;

/*-------------编码器通道数枚举-------------*/
typedef enum
{
    ECCHAN_1 = 0,   //单通道
    ECCHAN_3        //三通道
    
}EncoderChanEnum;

/*-------------编码器类型枚举-------------*/
typedef enum
{
    ENCODER_INC = 0,     //增量型编码器
    ENCODER_ABS          //绝对型编码器
    
}EncoderTypeEnum;

/*-------------PVT模式枚举-------------*/
typedef enum
{
    EXECMODE_NCYCLE = 0,
    EXECMODE_FIFO,
    
}ExecModeEnum;

/*-------------运行方向枚举-------------*/
typedef enum
{
    DIR_FORWARD = OUTPDATA_FORWARD,
    DIR_REVERSE = OUTPDATA_REVERSE
    
}DirectionEnum;

/*-----------PVT规划模式枚举-----------*/
typedef enum
{
    PLANMODE_CUBIC = 0,    //三次多项式插值
    PLANMODE_LINEAR,       //线性插值
    PLANMODE_UNIFORM,      //匀速规划
    PLANMODE_TRAPEZOID,    //梯形插值
    PLANMODE_QUINTIC,      //五次多项式插值
    PLANMODE_SINE,         //正弦
    
}PlanModeEnum;

/*-------------运动切换枚举-------------*/
typedef enum
{
    MTSWITCH_RESET = 0,
    MTSWITCH_STOP,
    MTSWITCH_RUN,
    MTSWITCH_PREPARE,
    MTSWITCH_EMERGSTOP, //紧急停止动作   是强制立即停止 不进行任何减速动作  cp 20170516
    
}MotionSwitchEnum;

/*-------------运动状态枚举-------------*/
/*运动状态分三级: 第一级是系统级，主要用于状态灯的闪烁需求，
                                  有初始化状态(POWERON)、待机状态(IDLE)、
                                  工作状态(CALCEND、CALCEND、STANDBY、RUNNING)和错误状态(ERROR);

                  第二级是通道级，有初始化状态(POWERON)、静止状态(IDLE、CALCEND、STANDBY)、
                                    解算状态(CALCING)、工作状态(OUTPUTING)和错误状态(ERROR);
                                    
                  第三级是波表级，有未配置状态(IDLE)、写入中状态(CALCEND)、
                                    写入完成状态(CALCEND)、预取状态(STANDBY)、
                                    输出状态(RUNNING)和错误状态(ERROR);
*/
typedef enum
{
    MTSTATE_POWERON = 0, //上电中，              对于波表来说不存在此状态
    MTSTATE_IDLE,        //运动系统处于空闲状态，对于波表来说表示未配置
    MTSTATE_CALCING,     //PVT计算中，           对于波表来说表示写入中
    MTSTATE_CALCEND,     //PVT计算完成，         对于波表来说表示写入完成
    MTSTATE_STANDBY,     //运动准备完成，        对于波表来说表示预取完成
    MTSTATE_OUTPUTING,   //正在运动，            对于波表来说表示输出中
    MTSTATE_ERROR,       //运动系统控制流程出现错误   具体错误需要从错误码查询
    
}MotionStateEnum;

/*-------------通道状态枚举--------------*/
typedef enum
{
    CHSTATE_POWERON = 0,
    CHSTATE_IDLE,
    CHSTATE_CALCING,
    CHSTATE_OUTPUTING,
    CHSTATE_ERROR     
    
}ChanStateEnum;

/*-------------系统状态枚举--------------*/
typedef enum
{
    SYSTATE_POWERON = 0,
    SYSTATE_IDLE,
    SYSTATE_WORKING,
    SYSTATE_ERROR     
    
}SysStateEnum;

/*-----------紧急停止类型枚举-----------*/
typedef enum
{
    STOPMODE_IMMED = 0,    //直接停止
    //STOPMODE_HOLD,         //保持当前输出
    STOPMODE_DIST,         //按距离停止
    STOPMODE_TIME,         //按时间停止
    STOPMODE_DIST_TIME,    //按距离和时间停止
    
}StopModeEnum;

/*-----------结束状态枚举-----------*/
typedef enum
{
    ENDSTATE_STOP = 0,    //直接停止
    ENDSTATE_HOLD,        //保持最后一步的速度
    
}EndStateEnum;

/*-------------位置单位枚举-------------*/
typedef enum
{
    POSTUNIT_ANGLE = 0,    //角度
    POSTUNIT_RADIAN,       //弧度
    POSTUNIT_MMETER,       //毫米
    POSTUNIT_IOSIGNAL      //IO信号
    
}PostUnitEnum;

/*-------------微调模式枚举-------------*/
typedef enum
{
    INCHIMODE_RELTANGLE = 0,    //以相对角度进行微调
    INCHIMODE_RELTRADIAN,       //以相对弧度进行微调
    INCHIMODE_RELTMMETER,       //以相对毫米进行微调
    INCHIMODE_ABSANGLE,         //以绝对角度进行微调
    INCHIMODE_ABSRADIAN,        //以绝对弧度进行微调
    INCHIMODE_ABSMMETER         //以绝对毫米进行微调
    
}InchingModeEnum;

/*-------------上报类型枚举-------------*/
typedef enum
{
    REPTTYPE_TORQUE = 0,    //TMC芯片的SG值
    REPTTYPE_CYCLE,         //循环数，循环数的上报周期设置后是无效的，循环数的更新是在数值变化后    CJ 2017.06.07 Add

    REPTTYPE_XANGLE,    //角度传感器X轴角度      //2
    REPTTYPE_YANGLE,    //角度传感器Y轴角度
    REPTTYPE_ZANGLE,    //角度传感器Z轴角度
    
    REPTTYPE_XVELOC,    //角度传感器X轴角速度      //5
    REPTTYPE_YVELOC,    //角度传感器Y轴角速度
    REPTTYPE_ZVELOC,    //角度传感器Z轴角速度
    
    REPTTYPE_XACC,      //角度传感器X轴角加速度      //8
    REPTTYPE_YACC,      //角度传感器Y轴角加速度
    REPTTYPE_ZACC,      //角度传感器Z轴角加速度

    REPTTYPE_DRVSGALL,  //SGALL获取
    REPTTYPE_DRVSGSE,   //SGSE获取
    
    REPTTYPE_DISTANCE,  //测距传感器值
    REPTTYPE_ABSENC,    //绝对值编码器
    
    REPTTYPE_OUTNUM,        //失步数             //11
    REPTTYPE_STEPS,         //输出的步数
    REPTTYPE_VELOCITY,      //速度
    REPTTYPE_ANGLESEN,      //角度传感器
    REPTTYPE_DRVMONT,       //驱动监控
    
    REPTTYPE_RESERVE
    
}ReportTypeEnum;

/*-------------上报状态枚举-------------*///将上报器也看成是一种传感器，和SensorStateEnum枚举一致    CJ 2017.05.24 Delete

/*-----------触发输入类型枚举-----------*/
typedef enum
{
    TRIGINTYPE_NONE = 0,    //不关心
    TRIGINTYPE_LOW,
    TRIGINTYPE_RISE,
    TRIGINTYPE_FALL,
    TRIGINTYPE_HIGH,
    
}TrigInTypeEnum;

/*-----------触发输入管脚枚举-----------*/
typedef enum
{
#ifdef PROJECT_QUBELEY

    TRIGPIN_DI2_X1 = 0,
    TRIGPIN_DI3_X2,
    TRIGPIN_DI4_X3,
    TRIGPIN_DI5_X4,
    TRIGPIN_CHANZ,

#else

    TRIGPIN_DIL = 0,
    TRIGPIN_DIR,
    
#endif

    TRIGPIN_RESERVE
    
}TrigInPinEnum;

/*-----------触发输入管脚枚举-----------*/
typedef enum
{
    SMODE_CONTINUED = 0,    //连续采样
    SMODE_INTERVAL,         //间隔采样
    SMODE_SINGLE            //单次采样
    
}SamplingModeEnum;

/*-----------数字IO信号类型枚举-----------*/
typedef enum
{
    DIOSIGNAL_LEVEL = 0,
    DIOSIGNAL_SQUARE,
    DIOSIGNAL_PULSE
    
}DIOSignalEnum;

/*-----------数字IO条件枚举-----------*/
typedef enum
{
    DIOCONDT_WTSTART = 0,    //波表输出开始
    DIOCONDT_WTEND,          //波表输出结束
    DIOCONDT_AUTO            //自动，使能之后就输出电平
    
}DIOCondtEnum;

/*-----------数字IO信号极性枚举-----------*/
typedef enum
{
    DIOPOLARITY_N = 0,    //负极性
    DIOPOLARITY_P
    
}DIOPolarityEnum;

/*-------------传感器状态枚举-------------*/
typedef enum
{
    SENSOR_OFF = 0,
    SENSOR_ON
    
}SensorStateEnum;

/*-------------时钟同步寄存器枚举-----------*/
typedef enum
{
    CLOCKREG_OFF = 0,
    CLOCKREG_ON 
    
}ClockSyncRegEnum;

/*-------------时钟同步状态枚举-----------*/    //CJ 2017.04.11 Add
typedef enum
{
    CLOCKSYNC_END = 0,
    CLOCKSYNC_START 
    
}ClockSyncStateEnum;

/*-------------寄存器功能枚举-------------*/    //CJ 2017.04.11 Add
typedef enum
{
    REGISTER_NONE = 0,     //关闭
    REGISTER_SOFTWARE,     //软件启动
    REGISTER_DIOSTART,     //数字IO启动
    REGISTER_CANSTART,     //CAN总线启动
    REGISTER_SYNCSTART,    //开始时间同步
    REGISTER_SYNCEND       //结束时间同步
    
}RegisterFuncEnum;

/*-------------接收类型枚举-------------*/    //CJ 2017.04.11 Add
typedef enum
{
    RECEIVE_RADIOID = 0,     //解析的是广播ID
    RECEIVE_RECEIVEID,       //解析的是接收ID
    RECEIVE_GROUPID1,          //解析的是组ID
    RECEIVE_GROUPID2,          //解析的是组ID
    
}ReceiveTypeEnum;

/*-------------传感器响应枚举-------------*/
typedef enum
{
    RESPONSE_NONE = 0,
    RESPONSE_ALARM,
    RESPONSE_STOP,
    RESPONSE_ALARMSTOP,
    RESPONSE_RUN
    
}ResponseTypeEnum;

/*-----------FPGA启动触发源枚举(因这几个枚举,操作的是逻辑的同一个寄存器,且有互斥关系,所以放在一起)---------------*/
typedef enum
{
    MULTI_SYNC_TRIG_SRC_OFF,
    MULTI_SYNC_TRIG_SRC_SW,
    MULTI_SYNC_TRIG_SRC_HW,
    MULTI_SYNC_TRIG_SRC_CAN,
    MULTI_SYNC_RST,
    MULTI_SYNC_START,
    MULTI_SYNC_END,
}MultiSyncFuncEnum;

typedef enum
{
  NMT_CONF_IDEL,
  NMT_C0NF_HASH,
  NMT_CONF_SIGNATURE,
  
}NMTConfigState;

/*-------------工程模式枚举-------------*/
typedef enum
{
    ENGINMODE_NONE = 0,        //非工程模式
    ENGINMODE_DDRTEST,         //DDR测试模式
    ENGINMODE_DRIVETEST,       //驱动测试模式
    ENGINMODE_TRIGINTEST,      //触发输入测试模式
    ENGINMODE_TRIGOUTTEST,     //触发输出测试模式
    ENGINMODE_ENCODERTEST,     //编码器测试模式
    ENGINMODE_ISOLINTEST,      //隔离输入测试模式
    ENGINMODE_ISOLOUTTEST,     //隔离输出测试模式
    ENGINMODE_ANALOGINTEST,    //模拟输入模式
    ENGINMODE_SUARTTEST,       //传感器UART测试模式
    
}EnginModeEnum;

/*-------------电平状态枚举-------------*/
typedef enum
{
    LEVEL_LOW = 0,      //低电平
    LEVEL_HIGH,         //高电平
    LEVEL_ERROR = 5,    //错误电平
    
}LevelStatusEnum;

/*-----------数字IO编号枚举-------------*/
typedef enum
{
    DIGITALIO_DI1 = 0,
    DIGITALIO_DI2,
    DIGITALIO_XI1,
    
    DIGITALIO_SEN1RX = 0,
    DIGITALIO_SEN2RX,
    DIGITALIO_SEN3RX,
    DIGITALIO_SEN4RX,

    DIGITALIO_DI3 = 3,
    DIGITALIO_DI4,
    DIGITALIO_DI5,
    DIGITALIO_DI6,
    DIGITALIO_DI7,

    DIGITALIO_XI2 = 3,
    DIGITALIO_XI3,
    DIGITALIO_XI4,
    DIGITALIO_AI1,



    DIGITALIO_DO1 = 8,
    DIGITALIO_DO2,

    DIGITALIO_XIN = 9,
    
    DIGITALIO_YO1,
    DIGITALIO_YO2,
    
}DigitalIONumEnum;

/*-----------数字输出端口编号枚举-------------*/
typedef enum
{
    DIO_DO1 = 0,
    DIO_DO2,

#ifdef PROJECT_GELGOOG
    DIO_DO3,
    DIO_DO4,
#endif

    DIO_RESERVE
    
}DoutNumEnum;

typedef enum
{
    YOUT_YO1 = 0,
    YOUT_YO2,
    YOUT_RESERVE
    
}YoutNumEnum;

/*---------------波表类型枚举-----------------*/
typedef enum
{
    WTTYPE_MAIN = 0,
    WTTYPE_SMALL,
    WTTYPE_PRESET1,
    WTTYPE_PRESET2,
    WTTYPE_PRESET3,
    WTTYPE_PRESET4,
    WTTYPE_PRESET5,
    WTTYPE_PRESET6,
    WTTYPE_PRESET7,
    WTTYPE_PRESET8,
    WTTYPE_RESERVE
    
}WaveTableTypeEnum;

/*-------------波表使用方式枚举--------------*/
typedef enum
{
    WTWORKMODE_FIFO  = 0,    //波表以FIFO模式使用
    WTWORKMODE_CYCLE,        //波表以循环模式使用
    
}WaveWorkModeEnum;

/*-------------运动方式枚举--------------*/
typedef enum
{
    MTNMODE_PVT  = 0,     //PVT模式
    MTNMODE_LVT_CORR,     //LVT带修正模式
    MTNMODE_LVT_NOCORR,   //LVT不带修正模式
    
}MotionModeEnum;

/*-------------Preset用户枚举---------------*/
typedef enum
{
    PRESET_USER1 = 0,
    PRESET_USER2,
    PRESET_USER3,
    PRESET_USER4,
    PRESET_USER5,
    PRESET_USER6,
    PRESET_USER7,
    PRESET_USER8,
    PRESET_RESERVE
    
}PresetUserEnum;

/*-------------事件源枚举---------------*/
typedef enum
{
    EVENTSRC_ENCCHANZ = 0,    //编码器Z通道
    EVENTSRC_FPGA,            //FPGA中断
    EVENTSRC_STATSWIT,        //状态切换
    EVENTSRC_ANALOGIN         //模拟输入
    
}EventSourceEnum;

/*-------------TRIGGER方式枚举--------------*/
typedef enum
{
    TRIGMODE_PATTERN = 0,    //触发方式为码型触发
    TRIGMODE_LEVEL,          //电平触发
    
}TriggerModeEnum;

/*-----------状态上报方式枚举-----------*/
typedef enum
{
    STATEQRY_ACTIVE = 0,    //主动上报
    STATEQRY_QUERY          //查询上报
    
}StateReportEnum;

/*-----------LED闪烁方式枚举-----------*/
typedef enum
{
    LED_POWERON = 0,    //上电中
    LED_IDEL,           //待机
    LED_WORKING,        //正在运动
    LED_ERROR,          //运动系统控制流程出现错误
    LED_ERROR1,         //运动系统控制流程出现错误
    LED_DEVICEID,       //设备识别
    
}LedStatusEnum;

/*-------------识别状态枚举-----------*/    //CJ 2017.08.11 Add
typedef enum
{
    DISTING_OFF = 0,
    DISTING_ON 
    
}DistingStateEnum;

/*-------------调整占比枚举-----------*/    //CJ 2017.08.11 Add
typedef enum
{
    MDUTY_1000 = 0,    //1
    MDUTY_500,         //0.5
    MDUTY_250,         //0.25
    MDUTY_125,         //0.125
    
}ModifyDutyEnum;

/*-------------配置状态枚举-----------*/    //CJ 2017.08.11 Add
typedef enum
{
    CONFIG_END = 0,
    CONFIG_CLEAR
    
}ConfigStateEnum;

/*-------------接口状态枚举-------------*/
typedef enum
{
    INTFC_NONE = 0,
    INTFC_OFF,
    INTFC_ON
    
}IntfcStateEnum;

/*-------------传感器Uart接口数量枚举-------------*///指的是ARM提供给传感器用的接口数量
typedef enum
{
    UARTNUM_U1 = 0,
    UARTNUM_U2,
    UARTNUM_RESERVE
    
}SenUartNumEnum;

/*-------------Sensor数量枚举-------------*/
typedef enum
{
    SENSOR_S1 = 0,

#ifdef PROJECT_GELGOOG
    SENSOR_S2,
    SENSOR_S3,
    SENSOR_S4,
#endif

    SENSOR_RESERVE
    
}SensorNumEnum;

typedef enum
{
    SSRC_SOFT = 0,
    SSRC_HARD,
    SSRC_CAN,
    SSRC_ALL
    
}StartSourceEnum;    //开始输出波表的触发源

typedef enum
{
    UPDATE_TYPE_BOOT,
    UPDATE_TYPE_APP,
    UPDATE_TYPE_FPGA,
    
}UpdateTypeEnum;

/*-------------Driver回读寄存器枚举-------------*/
typedef enum
{
    READREG_R0 = 0,
    READREG_R1,
    READREG_R2,
    READREG_RESERVE
    
}DriverReadRespEnum;

/*-------------Driver寄存器枚举-------------*/
typedef enum
{
    DRVREG_DRVCTRL = 0,
    DRVREG_CHOPCONF,
    DRVREG_SMARTEN,
    DRVREG_SGCSCONF,
    DRVREG_DRVCONF,
    DRVREG_RESERVE
    
}DriverRegisterEnum;

/*-------------Driver寄存器枚举-------------*/
typedef enum
{
    LIMIT_UP = 0,
    LIMIT_DOWN,
    LIMIT_RESERVE
    
}LimitEnum;

/*-------------SG参数枚举-------------*/
typedef enum
{
    SGPARA_SG0 = 0,
    SGPARA_SGT,
    SGPARA_SEMAX,
    SGPARA_SEMIN,
    SGPARA_RESERVE
    
}SGParaEnum;

/*-------------FPGA型号枚举-------------*/
typedef enum
{
    FPGATYPE_SMALLM = 0,    //MRQ-M使用的小FPGA
    FPGATYPE_BIGONE,
    FPGATYPE_BIGTWO,
    FPGATYPE_SMALLD,        //MRQ-D使用的小FPGA
    FPGATYPE_RESERVE
    
}FpgaTypeEnum;

/*-------------驱动板型号枚举-------------*/
typedef enum
{
    DRVBOARD_CTRL = 0,    //驱控器使用
    DRVBOARD_ARM,         //
    DRVBOARD_ERROR
    
}DriverBoardTypeEnum;

/*-------------角度数据类型枚举-------------*/
typedef enum
{
    MPUFRAME_ACC   = 0x51,
    MPUFRAME_VEL   = 0x52,
    MPUFRAME_ANGLE = 0x53
    
}AngleSensorDataTypeEnum;

/*-------------PRESET状态枚举-----------*/    //CJ 2017.08.11 Add
typedef enum
{
    PRESET_OFF = 0,
    PRESET_ON,
    PRESET_CALCABLE
    
}PresetStateEnum;

/*-------------LABEL枚举-----------*/
typedef enum
{
    MLABEL_SINANJU = 0,
    MLABEL_MEGATRON,
    MLABEL_GOUF,
    MLABEL_RESERVE
    
}MainLabelEnum;

/*-------------LABEL枚举-----------*/
typedef enum
{
    SLABEL_S1 = 0,
    SLABEL_S2,
    SLABEL_S3,
    SLABEL_S4,
    SLABEL_RESERVE
    
}SubLabelEnum;



#endif
/*****************************************END OF FILE****************************************/
