/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  comCmd.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.09.04;
历史版本:  无;
*********************************************************************************************/
#ifndef __COM_CMD_H
#define __COM_CMD_H



/******************************************包含文件*******************************************/
#include "comDataType.h"



/*******************************************宏定义********************************************/



/***************************************常数和类型声明****************************************/



/***************************************常数和类型声明****************************************/
typedef void (*SubCmdProFunc)(u8 cmdDataLen, u8 *pCmdData);



/***************************************常数和类型声明****************************************/
/********************************************枚举*********************************************/
/*-------------命令系统枚举-------------*/
//主命令枚举
typedef enum
{
    CMD_LINK = 1,      //连接设置命令
    CMD_SYSTME,        //系统设置命令
    CMD_UART,          //RS232设置命令
    CMD_CAN,           //CAN设置命令
    CMD_CLOCK,         //时钟命令 
    
    CMD_MOTION = 21,   //运行设置命令
    CMD_IDENTITY,      //ID设置命令
    
    CMD_MOTOR = 51,    //电机设置命令
    CMD_ENCODER,       //编码器设置命令
    
    CMD_PVT,           //轨迹规划命令
    CMD_PVTPOSN,       //PVT位置发送命令
    CMD_PVTPOSNQ,      //PVT位置查询命令
    CMD_PVTVELC,       //PVT速度发送命令
    CMD_PVTVELCQ,      //PVT速度查询命令
    CMD_PVTTIME,       //PVT时间发送命令
    CMD_PVTTIMEQ,      //PVT时间查询命令
    
    CMD_REPORT,        //上报设置命令
    CMD_TRIGIN,        //触发输入设置命令
    CMD_DRIVER,        //驱动设置
    CMD_TRIGOUT,       //触发输出设置命令
    CMD_ISOLOUT,       //隔离设置命令
    CMD_SENSORUART,    //传感器UART设置命令
    
    CMD_OTP = 70,      //OTP设置命令
    CMD_ANALOGIN,      //模拟输入设置命令
    CMD_ISOLIN,        //隔离输入设置命令

    
    CMD_ADVANCED = 151,    //高级命令，不对普通用户开放
    
    CMD_UPDATE = 201,  //升级命令
    CMD_FACTORY,       //工程命令
    CMD_CALIBRATE,     //校准命令
    
    CMD_DEBUG = 249,   //调试电机命令
    CMD_RESERVE        //保留
    
}CmdTypeEnum;

//子命令枚举
typedef enum
{
    LINKCMD_INTFC = 0,
    LINKCMD_INTFCQ,
    LINKCMD_RESERVE
    
}LinkCmdSubTypeEnum;

typedef enum
{
    SYSCMD_WORKMODE = 0,
    SYSCMD_WORKMODEQ,
    SYSCMD_POWERON,
    SYSCMD_POWERONQ,
    SYSCMD_SNQ,
    SYSCMD_TYPEQ,          //SN和TYPE的设置放在工程命令中，多轴的先不支持    CJ 2017.05.13 Modify 
    SYSCMD_SOFTVERQ,
    SYSCMD_FPGAVERQ,
    SYSCMD_HARDVERQ,
    SYSCMD_BOOTVERQ,
    SYSCMD_ERRORQ,
    SYSCMD_REPORTSW,    //自动上报开关
    SYSCMD_REPORTSWQ,   //自动上报开关
    SYSCMD_RESERVE
    
}SystemCmdSubTypeEnum;

typedef enum
{
    UARTCMD_BAUD = 0,
    UARTCMD_BAUDQ,
    UARTCMD_WORDLEN,
    UARTCMD_WORDLENQ,
    UARTCMD_FLOWCTL,
    UARTCMD_FLOWCTLQ,
    UARTCMD_PARITY,
    UARTCMD_PARITYQ,
    UARTCMD_STOPBIT,
    UARTCMD_STOPBITQ,
    UARTCMD_APPLY,
    UARTCMD_RESERVE
    
}UartCmdSubTypeEnum;

typedef enum
{
    CANCMD_TYPE = 0,
    CANCMD_TYPEQ,
    CANCMD_BAUD,
    CANCMD_BAUDQ,
    CANCMD_GROUP,
    CANCMD_GROUPQ,
    CANCMD_SENDID,
    CANCMD_SENDIDQ,
    CANCMD_RECEIVEID,
    CANCMD_RECEIVEIDQ,
    CANCMD_GROUPID1,
    CANCMD_GROUPID1Q,
    CANCMD_GROUPID2,
    CANCMD_GROUPID2Q,
    CANCMD_RADIOID,
    CANCMD_RADIOIDQ,
    CANCMD_APPLY,
    
    CANCMD_NMTLED,
    CANCMD_NMTSTATE,
    CANCMD_NMTSTATEQ,
    CANCMD_NMTSETID,
    CANCMD_NMTIDQ,
    CANCMD_NMTHASH,
    CANCMD_NMTHASHQ,
    CANCMD_NMTSIGNATURE,
    CANCMD_NMTSIGNATUREQ,
    CANCMD_NMTSIGNATURESIZEQ,//暂留 xyzheng
    
    CANCMD_RESERVE
    
}CanCmdSubTypeEnum;

//时钟和FPGA子命令枚举    CJ 2017.04.11 Add
typedef enum
{
    CLOCK_FREQUENCY = 0,
    CLOCK_FREQUENCYQ,
    CLOCK_SYNCREG,
    CLOCK_SYNCREGQ,
    CLOCK_STARTTYPE,
    CLOCK_STARTTYPEQ,
    CLOCK_SYNCSTATE,
    CLOCK_SYNCSTATEQ,
    CLOCK_COUNTQ,
    CLOCK_RESERVE
    
}ClockCmdSubTypeEnum;

typedef enum
{
    MOTIONCMD_SWITCH = 0,
    MOTIONCMD_STATE,
    MOTIONCMD_STATEREPT,
    MOTIONCMD_STATEREPTQ,
    MOTIONCMD_STARTSRC,
    MOTIONCMD_STARTSRCQ,
    MOTIONCMD_STARTTYPE,
    MOTIONCMD_STARTTYPEQ,
    
    MOTIONCMD_MAXSPEED,    // = 8
    MOTIONCMD_MAXSPEEDQ,
    MOTIONCMD_MINSPEED,
    MOTIONCMD_MINSPEEDQ,
    MOTIONCMD_MAXPOSN,
    MOTIONCMD_MAXPOSNQ,
    MOTIONCMD_MINPOSN,
    MOTIONCMD_MINPOSNQ,
    MOTIONCMD_MAXTORQUE,
    MOTIONCMD_MAXTORQUEQ,
    MOTIONCMD_MINTORQUE,
    MOTIONCMD_MINTORQUEQ,
    MOTIONCMD_MAXACC,    //最大加速度    CJ 2017.06.07 Add
    MOTIONCMD_MAXACCQ,
    MOTIONCMD_MINACC,
    MOTIONCMD_MINACCQ,
    MOTIONCMD_ORIGIN,
    MOTIONCMD_ORIGINQ,
    MOTIONCMD_GOORIGIN,
    
    MOTIONCMD_OFFSETSTATE,    //27
    MOTIONCMD_OFFSETSTATEQ,
    MOTIONCMD_OFFSET,
    
    MOTIONCMD_PVTSTEPSQ,    //30
    MOTIONCMD_COUNTSTEPSQ,
    MOTIONCMD_PVTCIRCLEQ,
    MOTIONCMD_COUNTCIRCLEQ,
    MOTIONCMD_ABCOUNTQ,
    
    MOTIONCMD_REVMOTION,    //35
    MOTIONCMD_REVMOTIONQ,
    
    MOTIONCMD_RESERVE
 
}MotionCmdSubTypeEnum;

typedef enum
{
    IDCMD_GROUP = 0,
    IDCMD_GROUPQ,
    IDCMD_DISTDEVICE,
    IDCMD_DISTDEVICEQ,
    IDCMD_LABEL,
    IDCMD_LABELQ,
    IDCMD_RESERVE
    
}IdentityCmdSubTypeEnum;    //ID命令    CJ 2017.08.11 Add

typedef enum
{
    MOTORCMD_STEPS = 0,
    MOTORCMD_STEPSQ,
    MOTORCMD_TYPE,
    MOTORCMD_TYPEQ,
    MOTORCMD_POSNUNIT,    // = 6
    MOTORCMD_POSNUNITQ,
    MOTORCMD_GRATIONUM,     //齿轮比分子
    MOTORCMD_GRATIONUMQ,
    MOTORCMD_GRATIODEN,     //齿轮比分母
    MOTORCMD_GRATIODENQ,
    MOTORCMD_LEAD,
    MOTORCMD_LEADQ,
    MOTORCMD_PEAKSPEED,    // = 14
    MOTORCMD_PEAKSPEEDQ,
    MOTORCMD_PEAKACC,
    MOTORCMD_PEAKACCQ,
    MOTORCMD_SIZE,
    MOTORCMD_SIZEQ,
    MOTORCMD_VOLT,
    MOTORCMD_VOLTQ,    // = 21
    MOTORCMD_CURR,
    MOTORCMD_CURRQ,
    MOTORCMD_BACKLASH,
    MOTORCMD_BACKLASHQ,
    MOTORCMD_RESERVE
    
}MotorCmdSubTypeEnum;

typedef enum
{
    ENCODERCMD_LINENUM = 0,
    ENCODERCMD_LINENUMQ,
    ENCODERCMD_CHANNUM,
    ENCODERCMD_CHANNUMQ,
    ENCODERCMD_TYPE,
    ENCODERCMD_TYPEQ,
    ENCODERCMD_MULTIPLE,
    ENCODERCMD_MULTIPLEQ,
    ENCODERCMD_STATE,
    ENCODERCMD_STATEQ,
    ENCODERCMD_FEEDBACKRATIO,
    ENCODERCMD_FEEDBACKRATIOQ,
    ENCODERCMD_RESERVE
    
}EncoderCmdSubTypeEnum;

typedef enum
{
    PVTCMD_CONFIG = 0,
    PVTCMD_CONFIGQ,
    PVTCMD_PRESETSTATE,
    PVTCMD_PRESETSTATEQ,
    
    PVTCMD_EXECMODE,        //=4
    PVTCMD_EXECMODEQ,
    PVTCMD_PLANMODE,
    PVTCMD_PLANMODEQ,
    PVTCMD_MOTIONMODE,
    PVTCMD_MOTIONMODEQ,
    PVTCMD_MODIFYDUTY,
    PVTCMD_MODIFYDUTYQ,
    PVTCMD_REMAINPOINTQ,
    PVTCMD_NCYCLES,         //13
    PVTCMD_NCYCLESQ,
    PVTCMD_WARNPOINT,
    PVTCMD_WARNPOINTQ,         
    PVTCMD_ENDSTATE,
    PVTCMD_ENDSTATEQ,
    PVTCMD_BUFFERSIZEQ,
    
    PVTCMD_ACCSCALE,        //20
    PVTCMD_ACCSCALEQ,
    PVTCMD_DECSCALE,
    PVTCMD_DECSCALEQ,
    PVTCMD_STOPMODE,
    PVTCMD_STOPMODEQ,
    PVTCMD_STOPDIST,
    PVTCMD_STOPDISTQ,
    PVTCMD_STOPTIME,
    PVTCMD_STOPTIMEQ,

    PVTCMD_LINESTATE,       //30
    PVTCMD_LINESTATEQ,
    PVTCMD_LINEOUTNUM,
    PVTCMD_LINEOUTNUMQ,
    PVTCMD_LINERESPONSE,
    PVTCMD_LINERESPONSEQ,
    PVTCMD_TOTALSTATE,
    PVTCMD_TOTALSTATEQ,
    PVTCMD_TOTALOUTNUM,
    PVTCMD_TOTALOUTNUMQ,
    PVTCMD_TOTALRESPONSE,
    PVTCMD_TOTALRESPONSEQ,

    PVTCMD_RESERVE
    
}PvtCmdSubTypeEnum;

typedef enum
{
    REPORTCMD_STATE = 0,
    REPORTCMD_STATEQ,
    REPORTCMD_PERIOD,
    REPORTCMD_PERIODQ,
    REPORTCMD_DATAQ,
    REPORTCMD_RESERVE
    
}ReportCmdSubTypeEnum;

typedef enum
{
    OTPCMD_STATE = 0,
    OTPCMD_STATEQ,
    OTPCMD_THRESHOLD,
    OTPCMD_THRESHOLDQ,
    OTPCMD_RESPONSE,
    OTPCMD_RESPONSEQ,
    OTPCMD_PERIOD,
    OTPCMD_PERIODQ,
    OTPCMD_DATAQ,
    OTPCMD_RESERVE
    
}OtpCmdSubTypeEnum;

typedef enum
{
    TRIGINCMD_MODE = 0,
    TRIGINCMD_MODEQ,

    TRIGINCMD_PATTSTATE,
    TRIGINCMD_PATTSTATEQ,
    TRIGINCMD_PATTERN,
    TRIGINCMD_PATTERNQ,
    TRIGINCMD_PATTRESP,
    TRIGINCMD_PATTRESPQ,
    TRIGINCMD_PATTSMODE,
    TRIGINCMD_PATTSMODEQ,
    TRIGINCMD_PATTPERIOD,
    TRIGINCMD_PATTPERIODQ,

    TRIGINCMD_LEVELSTATE,
    TRIGINCMD_LEVELSTATEQ,
    TRIGINCMD_LEVELTYPE,
    TRIGINCMD_LEVELTYPEQ,
    TRIGINCMD_LEVELRESP,
    TRIGINCMD_LEVELRESPQ,
    TRIGINCMD_LEVELSMODE,
    TRIGINCMD_LEVELSMODEQ,
    TRIGINCMD_LEVELPERIOD,
    TRIGINCMD_LEVELPERIODQ,
    
    TRIGINCMD_RESERVE
    
}TrigInCmdSubTypeEnum;

typedef enum
{
    TRIGOUTCMD_STATE = 0,
    TRIGOUTCMD_STATEQ,
    TRIGOUTCMD_SIGNAL,
    TRIGOUTCMD_SIGNALQ,
    TRIGOUTCMD_POLARITY,
    TRIGOUTCMD_POLARITYQ,
    TRIGOUTCMD_SOURCE,
    TRIGOUTCMD_SOURCEQ,
    TRIGOUTCMD_CONDITION,
    TRIGOUTCMD_CONDITIONQ,
    TRIGOUTCMD_PERIOD,
    TRIGOUTCMD_PERIODQ,
    TRIGOUTCMD_DUTY,
    TRIGOUTCMD_DUTYQ,
    TRIGOUTCMD_RESERVE
    
}TrigOutCmdSubTypeEnum;

typedef enum
{
    ANALOGINCMD_STATE = 0,
    ANALOGINCMD_STATEQ,
    ANALOGINCMD_THRESHOLDH,
    ANALOGINCMD_THRESHOLDHQ,
    ANALOGINCMD_THRESHOLDL,
    ANALOGINCMD_THRESHOLDLQ,
    ANALOGINCMD_RESPONSEH,
    ANALOGINCMD_RESPONSEHQ,
    ANALOGINCMD_RESPONSEL,
    ANALOGINCMD_RESPONSELQ,
    ANALOGINCMD_RESERVE
    
}AnalogInCmdSubTypeEnum;

typedef enum
{
    UPDATECMD_START = 0,
    UPDATECMD_ERASE,
    UPDATECMD_DATA,
    UPDATECMD_END,
    UPDATECMD_JUMP,
    UPDATECMD_APPADDR,
    UPDATECMD_APPADDRQ,
    UPDATECMD_FPGAADDR,
    UPDATECMD_FPGAADDRQ,
    UPDATECMD_RESERVE
    
}UpdateCmdSubTypeEnum;

typedef enum
{
    FACTORYCMD_SN = 0,
    FACTORYCMD_TYPE,
    FACTORYCMD_SOFTVERQ,
    FACTORYCMD_FPGAVERQ,
    FACTORYCMD_RESET,
    
    FACTORYCMD_SIGNATUREQ,     //5
    FACTORYCMD_DDRTEST,
    FACTORYCMD_DDRTESTQ,
    FACTORYCMD_DRIVERTEST,
    FACTORYCMD_DRIVERTESTQ,
    
    FACTORYCMD_TRIGINTEST,    //10
    FACTORYCMD_TRIGINTESTQ,
    FACTORYCMD_TRIGOUTTEST,
    FACTORYCMD_TRIGOUTTESTQ,
    FACTORYCMD_ENCODERTEST,
    FACTORYCMD_ENCODERTESTQ,
    
    FACTORYCMD_ISOLINTEST,    //16
    FACTORYCMD_ISOLINTESTQ,
    FACTORYCMD_ISOLOUTTEST,
    FACTORYCMD_ISOLOUTTESTQ,
    
    FACTORYCMD_ANALOGINTEST,    //20
    FACTORYCMD_ANALOGINTESTQ,
    FACTORYCMD_SUARTTEST,
    FACTORYCMD_SUARTTESTQ,
    
    //下面这部分命令是待测设备和测试设备之间进行通信用的，不和上位机进行通信
    FACTORYCMD_DRIVERSTART,    //24
    FACTORYCMD_DRIVERENDQ,
    FACTORYCMD_TRIGINSTART,
    FACTORYCMD_TRIGINENDQ,
    FACTORYCMD_TRIGOUTENDQ,
    FACTORYCMD_ENCODERSTART,
    FACTORYCMD_ENCODERENDQ,
    FACTORYCMD_ISOLINENDQ,
    FACTORYCMD_ISOLOUTENDQ,
    FACTORYCMD_ANALOGINENDQ,
    FACTORYCMD_SUARTSTART,
    FACTORYCMD_SUARTENDQ,
    //上面这部分命令是待测设备和测试设备之间进行通信用的，不和上位机进行通信

    FACTORYCMD_EEPROMCLEAN,    //清除EEPROM所有数据  //36
    FACTORYCMD_FPGAREG,
    
    FACTORYCMD_RESERVE
    
}FactoryCmdSubTypeEnum;

typedef enum
{
    CALBRCMD_BACKLASH = 0,
    CALBRCMD_BLEND,
    CALBRCMD_MECHERROR,
    CALBRCMD_MEEND,
    CALBRCMD_RESERVE
    
}CalbrCmdSubTypeEnum;

typedef enum
{
    DEBUGCMD_SET_DRIVER_TYPE = 0,
    DEBUGCMD_DRIVER_TYPE_Query,
    DEBUGCMD_READ_STATUS_REG,
    DEBUGCMD_START_MOTOR,
    DEBUGCMD_STOP_MOTOR,
    DEBUGCMD_SET_DRVCTRL_REG,
    DEBUGCMD_SET_DRVCONF_REG,
    DEBUGCMD_SET_CHOPCONF_REG,
    DEBUGCMD_SET_SGCSCONF_REG,
    DEBUGCMD_SET_SMARTEN_REG,
    DEBUGCMD_TMC26XX_REG_Query,
    DEBUGCMD_CAN_TEST,
    DEBUGCMD_RESERVE   
}DebugCmdSubTypeEnum;

typedef enum
{
    ISOLOUTCMD_STATE = 0,
    ISOLOUTCMD_STATEQ,
    ISOLOUTCMD_SOURCE,
    ISOLOUTCMD_SOURCEQ,
    ISOLOUTCMD_CONDITION,
    ISOLOUTCMD_CONDITIONQ,
    ISOLOUTCMD_RESPONSE,
    ISOLOUTCMD_RESPONSEQ,
    ISOLOUTCMD_RESERVE
    
}IsolatorOutCmdSubTypeEnum;

typedef enum
{
    SNUARTCMD_BAUD = 0,
    SNUARTCMD_BAUDQ,
    SNUARTCMD_WORDLEN,
    SNUARTCMD_WORDLENQ,
    SNUARTCMD_FLOWCTL,
    SNUARTCMD_FLOWCTLQ,
    SNUARTCMD_PARITY,
    SNUARTCMD_PARITYQ,
    SNUARTCMD_STOPBIT,
    SNUARTCMD_STOPBITQ,
    SNUARTCMD_APPLY,
    
    SNUARTCMD_STATE,    //=11
    SNUARTCMD_STATEQ,
    SNUARTCMD_SOF,
    SNUARTCMD_SOFQ,
    SNUARTCMD_FRAMELEN,
    SNUARTCMD_FRAMELENQ,
    SNUARTCMD_RECVNUM,
    SNUARTCMD_RECVNUMQ,
    SNUARTCMD_SWTIME,
    SNUARTCMD_SWTIMEQ,
    SNUARTCMD_DATAQ,
    
    SNUARTCMD_RESERVE
    
}SensorUartCmdSubTypeEnum;

typedef enum
{
    DRIVERCMD_TYPEQ = 0,
    DRIVERCMD_STATEREGQ,      //回读状态寄存器
    DRIVERCMD_CURRENT,
    DRIVERCMD_CURRENTQ,
    DRIVERCMD_MICROSTEPS,
    DRIVERCMD_MICROSTEPSQ,
    DRIVERCMD_SWITCH,
    DRIVERCMD_SWITCHQ,
    
    DRIVERCMD_REGCONF,        //配置寄存器         //8
    DRIVERCMD_REGCONFQ,
    
    DRIVERCMD_SGLIMIT,        //能效曲线报警阈值   
    DRIVERCMD_SGLIMITQ,

    DRIVERCMD_SGPARA,         //12
    DRIVERCMD_SGPARAQ,

    DRIVERCMD_RESERVE
    
}DriverCmdSubTypeEnum;

typedef enum
{
    ISOLINCMD_STATE,
    ISOLINCMD_STATEQ,
    ISOLINCMD_TYPE,
    ISOLINCMD_TYPEQ,
    ISOLINCMD_RESP,
    ISOLINCMD_RESPQ,
    ISOLINCMD_RESPCH,
    ISOLINCMD_RESPCHQ,
    ISOLINCMD_SMODE,
    ISOLINCMD_SMODEQ,
    ISOLINCMD_SPERIOD,
    ISOLINCMD_SPERIODQ,
    
    ISOLINCMD_RESERVE
    
}IsolatorInCmdSubTypeEnum;


#if 0
#endif


/*******************************************结构体********************************************/
/*-------------命令帧格式结构体-------------*/
typedef struct
{
    CmdTypeEnum  mainType;      //命令主类型
    u8           subType;       //命令子类型
    u8           dataLen;       //数据长度
    u8           payload[0];    //帧数据
    
}CmdParseFrameStruct;



#endif
/*****************************************END OF FILE****************************************/
