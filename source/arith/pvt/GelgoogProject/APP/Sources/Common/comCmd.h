/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  comCmd.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.09.04;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __COM_CMD_H
#define __COM_CMD_H



/******************************************�����ļ�*******************************************/
#include "comDataType.h"



/*******************************************�궨��********************************************/



/***************************************��������������****************************************/



/***************************************��������������****************************************/
typedef void (*SubCmdProFunc)(u8 cmdDataLen, u8 *pCmdData);



/***************************************��������������****************************************/
/********************************************ö��*********************************************/
/*-------------����ϵͳö��-------------*/
//������ö��
typedef enum
{
    CMD_LINK = 1,      //������������
    CMD_SYSTME,        //ϵͳ��������
    CMD_UART,          //RS232��������
    CMD_CAN,           //CAN��������
    CMD_CLOCK,         //ʱ������ 
    
    CMD_MOTION = 21,   //������������
    CMD_IDENTITY,      //ID��������
    
    CMD_MOTOR = 51,    //�����������
    CMD_ENCODER,       //��������������
    
    CMD_PVT,           //�켣�滮����
    CMD_PVTPOSN,       //PVTλ�÷�������
    CMD_PVTPOSNQ,      //PVTλ�ò�ѯ����
    CMD_PVTVELC,       //PVT�ٶȷ�������
    CMD_PVTVELCQ,      //PVT�ٶȲ�ѯ����
    CMD_PVTTIME,       //PVTʱ�䷢������
    CMD_PVTTIMEQ,      //PVTʱ���ѯ����
    
    CMD_REPORT,        //�ϱ���������
    CMD_TRIGIN,        //����������������
    CMD_DRIVER,        //��������
    CMD_TRIGOUT,       //���������������
    CMD_ISOLOUT,       //������������
    CMD_SENSORUART,    //������UART��������
    
    CMD_OTP = 70,      //OTP��������
    CMD_ANALOGIN,      //ģ��������������
    CMD_ISOLIN,        //����������������

    
    CMD_ADVANCED = 151,    //�߼����������ͨ�û�����
    
    CMD_UPDATE = 201,  //��������
    CMD_FACTORY,       //��������
    CMD_CALIBRATE,     //У׼����
    
    CMD_DEBUG = 249,   //���Ե������
    CMD_RESERVE        //����
    
}CmdTypeEnum;

//������ö��
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
    SYSCMD_TYPEQ,          //SN��TYPE�����÷��ڹ��������У�������Ȳ�֧��    CJ 2017.05.13 Modify 
    SYSCMD_SOFTVERQ,
    SYSCMD_FPGAVERQ,
    SYSCMD_HARDVERQ,
    SYSCMD_BOOTVERQ,
    SYSCMD_ERRORQ,
    SYSCMD_REPORTSW,    //�Զ��ϱ�����
    SYSCMD_REPORTSWQ,   //�Զ��ϱ�����
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
    CANCMD_NMTSIGNATURESIZEQ,//���� xyzheng
    
    CANCMD_RESERVE
    
}CanCmdSubTypeEnum;

//ʱ�Ӻ�FPGA������ö��    CJ 2017.04.11 Add
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
    MOTIONCMD_MAXACC,    //�����ٶ�    CJ 2017.06.07 Add
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
    
}IdentityCmdSubTypeEnum;    //ID����    CJ 2017.08.11 Add

typedef enum
{
    MOTORCMD_STEPS = 0,
    MOTORCMD_STEPSQ,
    MOTORCMD_TYPE,
    MOTORCMD_TYPEQ,
    MOTORCMD_POSNUNIT,    // = 6
    MOTORCMD_POSNUNITQ,
    MOTORCMD_GRATIONUM,     //���ֱȷ���
    MOTORCMD_GRATIONUMQ,
    MOTORCMD_GRATIODEN,     //���ֱȷ�ĸ
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
    
    //�����ⲿ�������Ǵ����豸�Ͳ����豸֮�����ͨ���õģ�������λ������ͨ��
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
    //�����ⲿ�������Ǵ����豸�Ͳ����豸֮�����ͨ���õģ�������λ������ͨ��

    FACTORYCMD_EEPROMCLEAN,    //���EEPROM��������  //36
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
    DRIVERCMD_STATEREGQ,      //�ض�״̬�Ĵ���
    DRIVERCMD_CURRENT,
    DRIVERCMD_CURRENTQ,
    DRIVERCMD_MICROSTEPS,
    DRIVERCMD_MICROSTEPSQ,
    DRIVERCMD_SWITCH,
    DRIVERCMD_SWITCHQ,
    
    DRIVERCMD_REGCONF,        //���üĴ���         //8
    DRIVERCMD_REGCONFQ,
    
    DRIVERCMD_SGLIMIT,        //��Ч���߱�����ֵ   
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


/*******************************************�ṹ��********************************************/
/*-------------����֡��ʽ�ṹ��-------------*/
typedef struct
{
    CmdTypeEnum  mainType;      //����������
    u8           subType;       //����������
    u8           dataLen;       //���ݳ���
    u8           payload[0];    //֡����
    
}CmdParseFrameStruct;



#endif
/*****************************************END OF FILE****************************************/
