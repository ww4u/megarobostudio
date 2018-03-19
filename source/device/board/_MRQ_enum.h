#ifndef _MRQ_ENUM_H_
#define _MRQ_ENUM_H_
enum MRQ_LINK_INTFC{
	MRQ_LINK_INTFC_NONE,
	MRQ_LINK_INTFC_CAN,
	MRQ_LINK_INTFC_RS232,
};
enum MRQ_SYSTEM_WORKMODE{
	MRQ_SYSTEM_WORKMODE_NORMAL,
	MRQ_SYSTEM_WORKMODE_NONE,
};
enum MRQ_SYSTEM_POWERON{
	MRQ_SYSTEM_POWERON_DEFAULT,
	MRQ_SYSTEM_POWERON_LAST,
};
enum MRQ_SYSTEM_TYPE{
	MRQ_SYSTEM_TYPE_MRQ,
	MRQ_SYSTEM_TYPE_MRV,
};
enum MRQ_SYSTEM_TYPE_1{
	MRQ_SYSTEM_TYPE_1_C17D,
	MRQ_SYSTEM_TYPE_1_C23D,
	MRQ_SYSTEM_TYPE_1_M1703,
	MRQ_SYSTEM_TYPE_1_M2303,
	MRQ_SYSTEM_TYPE_1_M1704,
	MRQ_SYSTEM_TYPE_1_M2304,
	MRQ_SYSTEM_TYPE_1_M1706,
	MRQ_SYSTEM_TYPE_1_M2306,
	MRQ_SYSTEM_TYPE_1_M1707,
	MRQ_SYSTEM_TYPE_1_M2307,
	MRQ_SYSTEM_TYPE_1_M1708,
	MRQ_SYSTEM_TYPE_1_M2308,
	MRQ_SYSTEM_TYPE_1_C17S,
	MRQ_SYSTEM_TYPE_1_C23S,
};
enum MRQ_SYSTEM_REPORTSWITCH{
	MRQ_SYSTEM_REPORTSWITCH_OFF,
	MRQ_SYSTEM_REPORTSWITCH_ON,
};
enum MRQ_RS232_BAUD{
	MRQ_RS232_BAUD__4800,
	MRQ_RS232_BAUD__7200,
	MRQ_RS232_BAUD__9600,
	MRQ_RS232_BAUD__14400,
	MRQ_RS232_BAUD__19200,
	MRQ_RS232_BAUD__38400,
	MRQ_RS232_BAUD__57600,
	MRQ_RS232_BAUD__115200,
	MRQ_RS232_BAUD__128000,
};
enum MRQ_RS232_WORDLEN{
	MRQ_RS232_WORDLEN__8,
	MRQ_RS232_WORDLEN__9,
};
enum MRQ_RS232_FLOWCTL{
	MRQ_RS232_FLOWCTL_NONE,
	MRQ_RS232_FLOWCTL_RTS,
	MRQ_RS232_FLOWCTL_CTS,
	MRQ_RS232_FLOWCTL_RTS_CTS,
};
enum MRQ_RS232_PARITY{
	MRQ_RS232_PARITY_NONE,
	MRQ_RS232_PARITY_EVEN,
	MRQ_RS232_PARITY_ODD,
};
enum MRQ_RS232_STOPBIT{
	MRQ_RS232_STOPBIT__1,
	MRQ_RS232_STOPBIT__0_5,
	MRQ_RS232_STOPBIT__2,
	MRQ_RS232_STOPBIT__1_5,
};
enum MRQ_CAN_TYPE{
	MRQ_CAN_TYPE_A,
	MRQ_CAN_TYPE_B,
};
enum MRQ_CAN_BAUD{
	MRQ_CAN_BAUD__1000,
	MRQ_CAN_BAUD__500,
	MRQ_CAN_BAUD__250,
	MRQ_CAN_BAUD__125,
	MRQ_CAN_BAUD__100,
	MRQ_CAN_BAUD__50,
	MRQ_CAN_BAUD__20,
	MRQ_CAN_BAUD__10,
};
enum MRQ_CAN_NETMANAGESTATE{
	MRQ_CAN_NETMANAGESTATE_IDLE,
	MRQ_CAN_NETMANAGESTATE_HASH,
	MRQ_CAN_NETMANAGESTATE_SIGNATURE,
};
enum MRQ_CAN_NETMANAGEID{
	MRQ_CAN_NETMANAGEID_NONE,
	MRQ_CAN_NETMANAGEID_SENDID,
	MRQ_CAN_NETMANAGEID_RECEIVEID,
};
enum MRQ_CLOCK_STARTTYPE{
	MRQ_CLOCK_STARTTYPE_RADIOID,
	MRQ_CLOCK_STARTTYPE_RECEIVEID,
	MRQ_CLOCK_STARTTYPE_GROUPID1,
	MRQ_CLOCK_STARTTYPE_GROUPID2,
};
enum MRQ_CLOCK_SYNCSTATE{
	MRQ_CLOCK_SYNCSTATE_END,
	MRQ_CLOCK_SYNCSTATE_START,
};
enum MRQ_MOTION_SWITCH{
	MRQ_MOTION_SWITCH_RESET,
	MRQ_MOTION_SWITCH_STOP,
	MRQ_MOTION_SWITCH_RUN,
	MRQ_MOTION_SWITCH_PREPARE,
	MRQ_MOTION_SWITCH_EMERGSTOP,
};
enum MRQ_MOTION_STATE{
	MRQ_MOTION_STATE_POWERON,
	MRQ_MOTION_STATE_IDLE,
	MRQ_MOTION_STATE_CALCING,
	MRQ_MOTION_STATE_CALCEND,
	MRQ_MOTION_STATE_STANDBY,
	MRQ_MOTION_STATE_RUNNING,
	MRQ_MOTION_STATE_ERROR,
};
enum MRQ_MOTION_STATEREPORT{
	MRQ_MOTION_STATEREPORT_ACTIVE,
	MRQ_MOTION_STATEREPORT_QUERY,
};
enum MRQ_MOTION_STARTSOURCE{
	MRQ_MOTION_STARTSOURCE_SOFTWARE,
	MRQ_MOTION_STARTSOURCE_DIGITALIO,
	MRQ_MOTION_STARTSOURCE_CAN,
	MRQ_MOTION_STARTSOURCE_ALL,
};
enum MRQ_MOTION_INITPOSITIONUNIT{
	MRQ_MOTION_INITPOSITIONUNIT_ANGLE,
	MRQ_MOTION_INITPOSITIONUNIT_RADIAN,
	MRQ_MOTION_INITPOSITIONUNIT_MILLIMETER,
	MRQ_MOTION_INITPOSITIONUNIT_IOSIGNAL,
};
enum MRQ_MOTION_INITIOSIGNAL{
	MRQ_MOTION_INITIOSIGNAL_TRIG1,
	MRQ_MOTION_INITIOSIGNAL_TRIG2,
	MRQ_MOTION_INITIOSIGNAL_TRIG3,
	MRQ_MOTION_INITIOSIGNAL_TRIG4,
	MRQ_MOTION_INITIOSIGNAL_TRIG5,
};
enum MRQ_MOTION_INCHINGMODE{
	MRQ_MOTION_INCHINGMODE_RELTANGLE,
	MRQ_MOTION_INCHINGMODE_RELTRADIAN,
	MRQ_MOTION_INCHINGMODE_RELTMILLIMETER,
};
enum MRQ_IDENTITY_GROUP{
	MRQ_IDENTITY_GROUP_GROUP1,
	MRQ_IDENTITY_GROUP_GROUP2,
};
enum MRQ_STOPDECEL_MODE{
	MRQ_STOPDECEL_MODE_IMMEDIATE,
	MRQ_STOPDECEL_MODE_DISTANCE,
};
enum MRQ_OUTOFSTEP_LINERESPONSE{
	MRQ_OUTOFSTEP_LINERESPONSE_NONE,
	MRQ_OUTOFSTEP_LINERESPONSE_ALARM,
	MRQ_OUTOFSTEP_LINERESPONSE_STOP,
	MRQ_OUTOFSTEP_LINERESPONSE_ALARM_STOP,
};
enum MRQ_MOTOR_STEPANGLE{
	MRQ_MOTOR_STEPANGLE__1_8,
	MRQ_MOTOR_STEPANGLE__0_9,
	MRQ_MOTOR_STEPANGLE__15,
	MRQ_MOTOR_STEPANGLE__7_5,
};
enum MRQ_MOTOR_TYPE{
	MRQ_MOTOR_TYPE_ROTARY,
	MRQ_MOTOR_TYPE_LINEAR,
};
enum MRQ_MOTOR_POSITIONUNIT{
	MRQ_MOTOR_POSITIONUNIT_ANGLE,
	MRQ_MOTOR_POSITIONUNIT_RADIAN,
	MRQ_MOTOR_POSITIONUNIT_MILLIMETER,
};
enum MRQ_MOTOR_SIZE{
	MRQ_MOTOR_SIZE__8,
	MRQ_MOTOR_SIZE__11,
	MRQ_MOTOR_SIZE__14,
	MRQ_MOTOR_SIZE__17,
	MRQ_MOTOR_SIZE__23,
	MRQ_MOTOR_SIZE__24,
};
enum MRQ_ENCODER_CHANNELNUM{
	MRQ_ENCODER_CHANNELNUM__1,
	MRQ_ENCODER_CHANNELNUM__3,
};
enum MRQ_ENCODER_TYPE{
	MRQ_ENCODER_TYPE_INCREMENTAL,
	MRQ_ENCODER_TYPE_ABSOLUTE,
};
enum MRQ_ENCODER_MULTIPLE{
	MRQ_ENCODER_MULTIPLE_SINGLE,
	MRQ_ENCODER_MULTIPLE_DOUBLE,
	MRQ_ENCODER_MULTIPLE_QUADRUPLE,
};
enum MRQ_ENCODER_STATE{
	MRQ_ENCODER_STATE_NONE,
	MRQ_ENCODER_STATE_OFF,
	MRQ_ENCODER_STATE_ON,
};
enum MRQ_MOTIONPLAN_PVTCONFIG{
	MRQ_MOTIONPLAN_PVTCONFIG_END,
	MRQ_MOTIONPLAN_PVTCONFIG_START,
	MRQ_MOTIONPLAN_PVTCONFIG_STOP,
	MRQ_MOTIONPLAN_PVTCONFIG_CLEAR,
};
enum MRQ_MOTIONPLAN_EXECUTEMODE{
	MRQ_MOTIONPLAN_EXECUTEMODE_CYCLE,
	MRQ_MOTIONPLAN_EXECUTEMODE_INFINITY,
	MRQ_MOTIONPLAN_EXECUTEMODE_FIFO,
};
enum MRQ_MOTIONPLAN_PLANMODE{
	MRQ_MOTIONPLAN_PLANMODE_CUBICPOLY,
	MRQ_MOTIONPLAN_PLANMODE_LINEAR,
	MRQ_MOTIONPLAN_PLANMODE_UNIFORM,
};
enum MRQ_MOTIONPLAN_MOTIONMODE{
	MRQ_MOTIONPLAN_MOTIONMODE_PVT,
	MRQ_MOTIONPLAN_MOTIONMODE_LVT_CORRECT,
	MRQ_MOTIONPLAN_MOTIONMODE_LVT_NOCORRECT,
};
enum MRQ_MOTIONPLAN_MODIFYDUTY{
	MRQ_MOTIONPLAN_MODIFYDUTY__1_4,
	MRQ_MOTIONPLAN_MODIFYDUTY__1_8,
	MRQ_MOTIONPLAN_MODIFYDUTY__1_16,
	MRQ_MOTIONPLAN_MODIFYDUTY__1_32,
};
enum MRQ_MOTIONPLAN_ENDSTATE{
	MRQ_MOTIONPLAN_ENDSTATE_STOP,
	MRQ_MOTIONPLAN_ENDSTATE_HOLD,
};
enum MRQ_REPORT_STATE{
	MRQ_REPORT_STATE_TORQUE,
	MRQ_REPORT_STATE_CYCLE,
	MRQ_REPORT_STATE_XANGLE,
	MRQ_REPORT_STATE_YANGLE,
	MRQ_REPORT_STATE_ZANGLE,
	MRQ_REPORT_STATE_XVEL,
	MRQ_REPORT_STATE_YVEL,
	MRQ_REPORT_STATE_ZVEL,
	MRQ_REPORT_STATE_XACC,
	MRQ_REPORT_STATE_YACC,
	MRQ_REPORT_STATE_ZACC,
	MRQ_REPORT_STATE_SGALL,
	MRQ_REPORT_STATE_SGSE,
	MRQ_REPORT_STATE_DIST,
};
enum MRQ_TRIGGER_MODE{
	MRQ_TRIGGER_MODE_PATTERN,
	MRQ_TRIGGER_MODE_LEVEL,
};
enum MRQ_TRIGGER_PATTERN{
	MRQ_TRIGGER_PATTERN_X,
	MRQ_TRIGGER_PATTERN_L,
	MRQ_TRIGGER_PATTERN_R,
	MRQ_TRIGGER_PATTERN_F,
	MRQ_TRIGGER_PATTERN_H,
};
enum MRQ_TRIGGER_PATTSMODE{
	MRQ_TRIGGER_PATTSMODE_CONTINUED,
	MRQ_TRIGGER_PATTSMODE_INTERVAL,
	MRQ_TRIGGER_PATTSMODE_SINGLE,
};
enum MRQ_TRIGGER_LEVELSTATE{
	MRQ_TRIGGER_LEVELSTATE_TRIGL,
	MRQ_TRIGGER_LEVELSTATE_TRIGR,
};
enum MRQ_TRIGGER_LEVELTYPE_1{
	MRQ_TRIGGER_LEVELTYPE_1_RESERVE,
	MRQ_TRIGGER_LEVELTYPE_1_LOW,
	MRQ_TRIGGER_LEVELTYPE_1_RISE,
	MRQ_TRIGGER_LEVELTYPE_1_FALL,
	MRQ_TRIGGER_LEVELTYPE_1_HIGH,
};
enum MRQ_DRIVER_TYPE{
	MRQ_DRIVER_TYPE_D17,
	MRQ_DRIVER_TYPE_D23,
};
enum MRQ_DRIVER_STATEREG{
	MRQ_DRIVER_STATEREG_R0,
	MRQ_DRIVER_STATEREG_R1,
	MRQ_DRIVER_STATEREG_R2,
};
enum MRQ_DRIVER_MICROSTEPS{
	MRQ_DRIVER_MICROSTEPS__256,
	MRQ_DRIVER_MICROSTEPS__128,
	MRQ_DRIVER_MICROSTEPS__64,
	MRQ_DRIVER_MICROSTEPS__32,
	MRQ_DRIVER_MICROSTEPS__16,
	MRQ_DRIVER_MICROSTEPS__8,
	MRQ_DRIVER_MICROSTEPS__4,
	MRQ_DRIVER_MICROSTEPS__2,
	MRQ_DRIVER_MICROSTEPS__1,
};
enum MRQ_DRIVER_REGCONFIG{
	MRQ_DRIVER_REGCONFIG_DRVCTRL,
	MRQ_DRIVER_REGCONFIG_CHOPCONF,
	MRQ_DRIVER_REGCONFIG_SMARTEN,
	MRQ_DRIVER_REGCONFIG_SGCSCONF,
	MRQ_DRIVER_REGCONFIG_DRVCONF,
};
enum MRQ_DRIVER_SGLIMIT{
	MRQ_DRIVER_SGLIMIT_UPLIMIT,
	MRQ_DRIVER_SGLIMIT_DOWNLIMIT,
};
enum MRQ_DRIVER_SGPARASET{
	MRQ_DRIVER_SGPARASET_SG0,
	MRQ_DRIVER_SGPARASET_SGT,
	MRQ_DRIVER_SGPARASET_SEMAX,
	MRQ_DRIVER_SGPARASET_SEMIN,
};
enum MRQ_DIGITALOUT_STATE{
	MRQ_DIGITALOUT_STATE_DO1,
	MRQ_DIGITALOUT_STATE_DO2,
	MRQ_DIGITALOUT_STATE_DO3,
	MRQ_DIGITALOUT_STATE_DO4,
};
enum MRQ_DIGITALOUT_STATE_1{
	MRQ_DIGITALOUT_STATE_1_DISABLE,
	MRQ_DIGITALOUT_STATE_1_ENABLE,
};
enum MRQ_DIGITALOUT_SIGNAL_1{
	MRQ_DIGITALOUT_SIGNAL_1_LEVEL,
	MRQ_DIGITALOUT_SIGNAL_1_NONE,
};
enum MRQ_DIGITALOUT_POLARITY_1{
	MRQ_DIGITALOUT_POLARITY_1_POSITIVE,
	MRQ_DIGITALOUT_POLARITY_1_NEGATIVE,
};
enum MRQ_DIGITALOUT_CONDITION_1{
	MRQ_DIGITALOUT_CONDITION_1_AUTO,
	MRQ_DIGITALOUT_CONDITION_1_NONE,
};
enum MRQ_ISOLATOROUTPUT_STATE{
	MRQ_ISOLATOROUTPUT_STATE_YO1,
	MRQ_ISOLATOROUTPUT_STATE_YO2,
};
enum MRQ_ISOLATOROUTPUT_RESPONSE_1{
	MRQ_ISOLATOROUTPUT_RESPONSE_1_TURNON,
	MRQ_ISOLATOROUTPUT_RESPONSE_1_CUTOFF,
};
enum MRQ_SENSORUART_BAUD{
	MRQ_SENSORUART_BAUD_UART1,
	MRQ_SENSORUART_BAUD_UART2,
};
enum MRQ_SENSORUART_STATE_1{
	MRQ_SENSORUART_STATE_1_S1,
	MRQ_SENSORUART_STATE_1_S2,
	MRQ_SENSORUART_STATE_1_S3,
	MRQ_SENSORUART_STATE_1_S4,
};
enum MRQ_PRESET_PVTCONFIG{
	MRQ_PRESET_PVTCONFIG_PRESET1,
	MRQ_PRESET_PVTCONFIG__2,
};
enum MRQ_PRESET_EXECUTE_1{
	MRQ_PRESET_EXECUTE_1_CLEAR,
	MRQ_PRESET_EXECUTE_1_RUN,
	MRQ_PRESET_EXECUTE_1_PREPARE,
	MRQ_PRESET_EXECUTE_1_CALC,
	MRQ_PRESET_EXECUTE_1_SAVE,
};

const char* MRQ_LINK_INTFC_toString( MRQ_LINK_INTFC eType );
const char* MRQ_SYSTEM_WORKMODE_toString( MRQ_SYSTEM_WORKMODE eType );
const char* MRQ_SYSTEM_POWERON_toString( MRQ_SYSTEM_POWERON eType );
const char* MRQ_SYSTEM_TYPE_toString( MRQ_SYSTEM_TYPE eType );
const char* MRQ_SYSTEM_TYPE_1_toString( MRQ_SYSTEM_TYPE_1 eType );
const char* MRQ_SYSTEM_REPORTSWITCH_toString( MRQ_SYSTEM_REPORTSWITCH eType );
const char* MRQ_RS232_BAUD_toString( MRQ_RS232_BAUD eType );
const char* MRQ_RS232_WORDLEN_toString( MRQ_RS232_WORDLEN eType );
const char* MRQ_RS232_FLOWCTL_toString( MRQ_RS232_FLOWCTL eType );
const char* MRQ_RS232_PARITY_toString( MRQ_RS232_PARITY eType );
const char* MRQ_RS232_STOPBIT_toString( MRQ_RS232_STOPBIT eType );
const char* MRQ_CAN_TYPE_toString( MRQ_CAN_TYPE eType );
const char* MRQ_CAN_BAUD_toString( MRQ_CAN_BAUD eType );
const char* MRQ_CAN_NETMANAGESTATE_toString( MRQ_CAN_NETMANAGESTATE eType );
const char* MRQ_CAN_NETMANAGEID_toString( MRQ_CAN_NETMANAGEID eType );
const char* MRQ_CLOCK_STARTTYPE_toString( MRQ_CLOCK_STARTTYPE eType );
const char* MRQ_CLOCK_SYNCSTATE_toString( MRQ_CLOCK_SYNCSTATE eType );
const char* MRQ_MOTION_SWITCH_toString( MRQ_MOTION_SWITCH eType );
const char* MRQ_MOTION_STATE_toString( MRQ_MOTION_STATE eType );
const char* MRQ_MOTION_STATEREPORT_toString( MRQ_MOTION_STATEREPORT eType );
const char* MRQ_MOTION_STARTSOURCE_toString( MRQ_MOTION_STARTSOURCE eType );
const char* MRQ_MOTION_INITPOSITIONUNIT_toString( MRQ_MOTION_INITPOSITIONUNIT eType );
const char* MRQ_MOTION_INITIOSIGNAL_toString( MRQ_MOTION_INITIOSIGNAL eType );
const char* MRQ_MOTION_INCHINGMODE_toString( MRQ_MOTION_INCHINGMODE eType );
const char* MRQ_IDENTITY_GROUP_toString( MRQ_IDENTITY_GROUP eType );
const char* MRQ_STOPDECEL_MODE_toString( MRQ_STOPDECEL_MODE eType );
const char* MRQ_OUTOFSTEP_LINERESPONSE_toString( MRQ_OUTOFSTEP_LINERESPONSE eType );
const char* MRQ_MOTOR_STEPANGLE_toString( MRQ_MOTOR_STEPANGLE eType );
const char* MRQ_MOTOR_TYPE_toString( MRQ_MOTOR_TYPE eType );
const char* MRQ_MOTOR_POSITIONUNIT_toString( MRQ_MOTOR_POSITIONUNIT eType );
const char* MRQ_MOTOR_SIZE_toString( MRQ_MOTOR_SIZE eType );
const char* MRQ_ENCODER_CHANNELNUM_toString( MRQ_ENCODER_CHANNELNUM eType );
const char* MRQ_ENCODER_TYPE_toString( MRQ_ENCODER_TYPE eType );
const char* MRQ_ENCODER_MULTIPLE_toString( MRQ_ENCODER_MULTIPLE eType );
const char* MRQ_ENCODER_STATE_toString( MRQ_ENCODER_STATE eType );
const char* MRQ_MOTIONPLAN_PVTCONFIG_toString( MRQ_MOTIONPLAN_PVTCONFIG eType );
const char* MRQ_MOTIONPLAN_EXECUTEMODE_toString( MRQ_MOTIONPLAN_EXECUTEMODE eType );
const char* MRQ_MOTIONPLAN_PLANMODE_toString( MRQ_MOTIONPLAN_PLANMODE eType );
const char* MRQ_MOTIONPLAN_MOTIONMODE_toString( MRQ_MOTIONPLAN_MOTIONMODE eType );
const char* MRQ_MOTIONPLAN_MODIFYDUTY_toString( MRQ_MOTIONPLAN_MODIFYDUTY eType );
const char* MRQ_MOTIONPLAN_ENDSTATE_toString( MRQ_MOTIONPLAN_ENDSTATE eType );
const char* MRQ_REPORT_STATE_toString( MRQ_REPORT_STATE eType );
const char* MRQ_TRIGGER_MODE_toString( MRQ_TRIGGER_MODE eType );
const char* MRQ_TRIGGER_PATTERN_toString( MRQ_TRIGGER_PATTERN eType );
const char* MRQ_TRIGGER_PATTSMODE_toString( MRQ_TRIGGER_PATTSMODE eType );
const char* MRQ_TRIGGER_LEVELSTATE_toString( MRQ_TRIGGER_LEVELSTATE eType );
const char* MRQ_TRIGGER_LEVELTYPE_1_toString( MRQ_TRIGGER_LEVELTYPE_1 eType );
const char* MRQ_DRIVER_TYPE_toString( MRQ_DRIVER_TYPE eType );
const char* MRQ_DRIVER_STATEREG_toString( MRQ_DRIVER_STATEREG eType );
const char* MRQ_DRIVER_MICROSTEPS_toString( MRQ_DRIVER_MICROSTEPS eType );
const char* MRQ_DRIVER_REGCONFIG_toString( MRQ_DRIVER_REGCONFIG eType );
const char* MRQ_DRIVER_SGLIMIT_toString( MRQ_DRIVER_SGLIMIT eType );
const char* MRQ_DRIVER_SGPARASET_toString( MRQ_DRIVER_SGPARASET eType );
const char* MRQ_DIGITALOUT_STATE_toString( MRQ_DIGITALOUT_STATE eType );
const char* MRQ_DIGITALOUT_STATE_1_toString( MRQ_DIGITALOUT_STATE_1 eType );
const char* MRQ_DIGITALOUT_SIGNAL_1_toString( MRQ_DIGITALOUT_SIGNAL_1 eType );
const char* MRQ_DIGITALOUT_POLARITY_1_toString( MRQ_DIGITALOUT_POLARITY_1 eType );
const char* MRQ_DIGITALOUT_CONDITION_1_toString( MRQ_DIGITALOUT_CONDITION_1 eType );
const char* MRQ_ISOLATOROUTPUT_STATE_toString( MRQ_ISOLATOROUTPUT_STATE eType );
const char* MRQ_ISOLATOROUTPUT_RESPONSE_1_toString( MRQ_ISOLATOROUTPUT_RESPONSE_1 eType );
const char* MRQ_SENSORUART_BAUD_toString( MRQ_SENSORUART_BAUD eType );
const char* MRQ_SENSORUART_STATE_1_toString( MRQ_SENSORUART_STATE_1 eType );
const char* MRQ_PRESET_PVTCONFIG_toString( MRQ_PRESET_PVTCONFIG eType );
const char* MRQ_PRESET_EXECUTE_1_toString( MRQ_PRESET_EXECUTE_1 eType );

int MRQ_LINK_INTFC_toValue( const char *pStr, MRQ_LINK_INTFC *pEVal );
int MRQ_SYSTEM_WORKMODE_toValue( const char *pStr, MRQ_SYSTEM_WORKMODE *pEVal );
int MRQ_SYSTEM_POWERON_toValue( const char *pStr, MRQ_SYSTEM_POWERON *pEVal );
int MRQ_SYSTEM_TYPE_toValue( const char *pStr, MRQ_SYSTEM_TYPE *pEVal );
int MRQ_SYSTEM_TYPE_1_toValue( const char *pStr, MRQ_SYSTEM_TYPE_1 *pEVal );
int MRQ_SYSTEM_REPORTSWITCH_toValue( const char *pStr, MRQ_SYSTEM_REPORTSWITCH *pEVal );
int MRQ_RS232_BAUD_toValue( const char *pStr, MRQ_RS232_BAUD *pEVal );
int MRQ_RS232_WORDLEN_toValue( const char *pStr, MRQ_RS232_WORDLEN *pEVal );
int MRQ_RS232_FLOWCTL_toValue( const char *pStr, MRQ_RS232_FLOWCTL *pEVal );
int MRQ_RS232_PARITY_toValue( const char *pStr, MRQ_RS232_PARITY *pEVal );
int MRQ_RS232_STOPBIT_toValue( const char *pStr, MRQ_RS232_STOPBIT *pEVal );
int MRQ_CAN_TYPE_toValue( const char *pStr, MRQ_CAN_TYPE *pEVal );
int MRQ_CAN_BAUD_toValue( const char *pStr, MRQ_CAN_BAUD *pEVal );
int MRQ_CAN_NETMANAGESTATE_toValue( const char *pStr, MRQ_CAN_NETMANAGESTATE *pEVal );
int MRQ_CAN_NETMANAGEID_toValue( const char *pStr, MRQ_CAN_NETMANAGEID *pEVal );
int MRQ_CLOCK_STARTTYPE_toValue( const char *pStr, MRQ_CLOCK_STARTTYPE *pEVal );
int MRQ_CLOCK_SYNCSTATE_toValue( const char *pStr, MRQ_CLOCK_SYNCSTATE *pEVal );
int MRQ_MOTION_SWITCH_toValue( const char *pStr, MRQ_MOTION_SWITCH *pEVal );
int MRQ_MOTION_STATE_toValue( const char *pStr, MRQ_MOTION_STATE *pEVal );
int MRQ_MOTION_STATEREPORT_toValue( const char *pStr, MRQ_MOTION_STATEREPORT *pEVal );
int MRQ_MOTION_STARTSOURCE_toValue( const char *pStr, MRQ_MOTION_STARTSOURCE *pEVal );
int MRQ_MOTION_INITPOSITIONUNIT_toValue( const char *pStr, MRQ_MOTION_INITPOSITIONUNIT *pEVal );
int MRQ_MOTION_INITIOSIGNAL_toValue( const char *pStr, MRQ_MOTION_INITIOSIGNAL *pEVal );
int MRQ_MOTION_INCHINGMODE_toValue( const char *pStr, MRQ_MOTION_INCHINGMODE *pEVal );
int MRQ_IDENTITY_GROUP_toValue( const char *pStr, MRQ_IDENTITY_GROUP *pEVal );
int MRQ_STOPDECEL_MODE_toValue( const char *pStr, MRQ_STOPDECEL_MODE *pEVal );
int MRQ_OUTOFSTEP_LINERESPONSE_toValue( const char *pStr, MRQ_OUTOFSTEP_LINERESPONSE *pEVal );
int MRQ_MOTOR_STEPANGLE_toValue( const char *pStr, MRQ_MOTOR_STEPANGLE *pEVal );
int MRQ_MOTOR_TYPE_toValue( const char *pStr, MRQ_MOTOR_TYPE *pEVal );
int MRQ_MOTOR_POSITIONUNIT_toValue( const char *pStr, MRQ_MOTOR_POSITIONUNIT *pEVal );
int MRQ_MOTOR_SIZE_toValue( const char *pStr, MRQ_MOTOR_SIZE *pEVal );
int MRQ_ENCODER_CHANNELNUM_toValue( const char *pStr, MRQ_ENCODER_CHANNELNUM *pEVal );
int MRQ_ENCODER_TYPE_toValue( const char *pStr, MRQ_ENCODER_TYPE *pEVal );
int MRQ_ENCODER_MULTIPLE_toValue( const char *pStr, MRQ_ENCODER_MULTIPLE *pEVal );
int MRQ_ENCODER_STATE_toValue( const char *pStr, MRQ_ENCODER_STATE *pEVal );
int MRQ_MOTIONPLAN_PVTCONFIG_toValue( const char *pStr, MRQ_MOTIONPLAN_PVTCONFIG *pEVal );
int MRQ_MOTIONPLAN_EXECUTEMODE_toValue( const char *pStr, MRQ_MOTIONPLAN_EXECUTEMODE *pEVal );
int MRQ_MOTIONPLAN_PLANMODE_toValue( const char *pStr, MRQ_MOTIONPLAN_PLANMODE *pEVal );
int MRQ_MOTIONPLAN_MOTIONMODE_toValue( const char *pStr, MRQ_MOTIONPLAN_MOTIONMODE *pEVal );
int MRQ_MOTIONPLAN_MODIFYDUTY_toValue( const char *pStr, MRQ_MOTIONPLAN_MODIFYDUTY *pEVal );
int MRQ_MOTIONPLAN_ENDSTATE_toValue( const char *pStr, MRQ_MOTIONPLAN_ENDSTATE *pEVal );
int MRQ_REPORT_STATE_toValue( const char *pStr, MRQ_REPORT_STATE *pEVal );
int MRQ_TRIGGER_MODE_toValue( const char *pStr, MRQ_TRIGGER_MODE *pEVal );
int MRQ_TRIGGER_PATTERN_toValue( const char *pStr, MRQ_TRIGGER_PATTERN *pEVal );
int MRQ_TRIGGER_PATTSMODE_toValue( const char *pStr, MRQ_TRIGGER_PATTSMODE *pEVal );
int MRQ_TRIGGER_LEVELSTATE_toValue( const char *pStr, MRQ_TRIGGER_LEVELSTATE *pEVal );
int MRQ_TRIGGER_LEVELTYPE_1_toValue( const char *pStr, MRQ_TRIGGER_LEVELTYPE_1 *pEVal );
int MRQ_DRIVER_TYPE_toValue( const char *pStr, MRQ_DRIVER_TYPE *pEVal );
int MRQ_DRIVER_STATEREG_toValue( const char *pStr, MRQ_DRIVER_STATEREG *pEVal );
int MRQ_DRIVER_MICROSTEPS_toValue( const char *pStr, MRQ_DRIVER_MICROSTEPS *pEVal );
int MRQ_DRIVER_REGCONFIG_toValue( const char *pStr, MRQ_DRIVER_REGCONFIG *pEVal );
int MRQ_DRIVER_SGLIMIT_toValue( const char *pStr, MRQ_DRIVER_SGLIMIT *pEVal );
int MRQ_DRIVER_SGPARASET_toValue( const char *pStr, MRQ_DRIVER_SGPARASET *pEVal );
int MRQ_DIGITALOUT_STATE_toValue( const char *pStr, MRQ_DIGITALOUT_STATE *pEVal );
int MRQ_DIGITALOUT_STATE_1_toValue( const char *pStr, MRQ_DIGITALOUT_STATE_1 *pEVal );
int MRQ_DIGITALOUT_SIGNAL_1_toValue( const char *pStr, MRQ_DIGITALOUT_SIGNAL_1 *pEVal );
int MRQ_DIGITALOUT_POLARITY_1_toValue( const char *pStr, MRQ_DIGITALOUT_POLARITY_1 *pEVal );
int MRQ_DIGITALOUT_CONDITION_1_toValue( const char *pStr, MRQ_DIGITALOUT_CONDITION_1 *pEVal );
int MRQ_ISOLATOROUTPUT_STATE_toValue( const char *pStr, MRQ_ISOLATOROUTPUT_STATE *pEVal );
int MRQ_ISOLATOROUTPUT_RESPONSE_1_toValue( const char *pStr, MRQ_ISOLATOROUTPUT_RESPONSE_1 *pEVal );
int MRQ_SENSORUART_BAUD_toValue( const char *pStr, MRQ_SENSORUART_BAUD *pEVal );
int MRQ_SENSORUART_STATE_1_toValue( const char *pStr, MRQ_SENSORUART_STATE_1 *pEVal );
int MRQ_PRESET_PVTCONFIG_toValue( const char *pStr, MRQ_PRESET_PVTCONFIG *pEVal );
int MRQ_PRESET_EXECUTE_1_toValue( const char *pStr, MRQ_PRESET_EXECUTE_1 *pEVal );

enum cmd_MRQ
{

	mc_LINK = 1,
		sc_LINK_INTFC = 0,
		sc_LINK_INTFC_Q = 1,

	mc_SYSTEM = 2,
		sc_SYSTEM_WORKMODE = 0,
		sc_SYSTEM_WORKMODE_Q = 1,
		sc_SYSTEM_POWERON = 2,
		sc_SYSTEM_POWERON_Q = 3,
		sc_SYSTEM_SN_Q = 4,
		sc_SYSTEM_TYPE_Q = 5,
		sc_SYSTEM_SOFTVER_Q = 6,
		sc_SYSTEM_FPGAVER_Q = 7,
		sc_SYSTEM_HARDVER_Q = 8,
		sc_SYSTEM_BOOTVER_Q = 9,
		sc_SYSTEM_ERROR_Q = 10,
		sc_SYSTEM_REPORTSWITCH = 11,
		sc_SYSTEM_REPORTSWITCH_Q = 12,

	mc_RS232 = 3,
		sc_RS232_BAUD = 0,
		sc_RS232_BAUD_Q = 1,
		sc_RS232_WORDLEN = 2,
		sc_RS232_WORDLEN_Q = 3,
		sc_RS232_FLOWCTL = 4,
		sc_RS232_FLOWCTL_Q = 5,
		sc_RS232_PARITY = 6,
		sc_RS232_PARITY_Q = 7,
		sc_RS232_STOPBIT = 8,
		sc_RS232_STOPBIT_Q = 9,
		sc_RS232_APPLYPARA = 10,

	mc_CAN = 4,
		sc_CAN_TYPE = 0,
		sc_CAN_TYPE_Q = 1,
		sc_CAN_BAUD = 2,
		sc_CAN_BAUD_Q = 3,
		sc_CAN_GROUP = 4,
		sc_CAN_GROUP_Q = 5,
		sc_CAN_SENDID = 6,
		sc_CAN_SENDID_Q = 7,
		sc_CAN_RECEIVEID = 8,
		sc_CAN_RECEIVEID_Q = 9,
		sc_CAN_GROUPID1 = 10,
		sc_CAN_GROUPID1_Q = 11,
		sc_CAN_GROUPID2 = 12,
		sc_CAN_GROUPID2_Q = 13,
		sc_CAN_BROADCASTID = 14,
		sc_CAN_BROADCASTID_Q = 15,
		sc_CAN_APPLYPARA = 16,
		sc_CAN_NETMANAGELED = 17,
		sc_CAN_NETMANAGESTATE = 18,
		sc_CAN_NETMANAGESTATE_Q = 19,
		sc_CAN_NETMANAGEID = 20,
		sc_CAN_NETMANAGEID_Q = 21,
		sc_CAN_NETMANAGEHASH = 22,
		sc_CAN_NETMANAGEHASH_Q = 23,

	mc_CLOCK = 5,
		sc_CLOCK_FREQUENCY = 0,
		sc_CLOCK_FREQUENCY_Q = 1,
		sc_CLOCK_SYNCREGISTER = 2,
		sc_CLOCK_SYNCREGISTER_Q = 3,
		sc_CLOCK_STARTTYPE = 4,
		sc_CLOCK_STARTTYPE_Q = 5,
		sc_CLOCK_SYNCSTATE = 6,
		sc_CLOCK_SYNCSTATE_Q = 7,
		sc_CLOCK_COUNT_Q = 8,

	mc_MOTION = 21,
		sc_MOTION_SWITCH = 0,
		sc_MOTION_STATE_Q = 1,
		sc_MOTION_STATEREPORT = 2,
		sc_MOTION_STATEREPORT_Q = 3,
		sc_MOTION_STARTSOURCE = 4,
		sc_MOTION_STARTSOURCE_Q = 5,
		sc_MOTION_STARTTYPE = 6,
		sc_MOTION_STARTTYPE_Q = 7,
		sc_MOTION_MAXSPEED = 8,
		sc_MOTION_MAXSPEED_Q = 9,
		sc_MOTION_MINSPEED = 10,
		sc_MOTION_MINSPEED_Q = 11,
		sc_MOTION_MAXPOSITION = 12,
		sc_MOTION_MAXPOSITION_Q = 13,
		sc_MOTION_MINPOSITION = 14,
		sc_MOTION_MINPOSITION_Q = 15,
		sc_MOTION_MAXTORQUE = 16,
		sc_MOTION_MAXTORQUE_Q = 17,
		sc_MOTION_MINTORQUE = 18,
		sc_MOTION_MINTORQUE_Q = 19,
		sc_MOTION_MAXACCELERATION = 20,
		sc_MOTION_MAXACCELERATION_Q = 21,
		sc_MOTION_MINACCELERATION = 22,
		sc_MOTION_MINACCELERATION_Q = 23,
		sc_MOTION_ORIGIN = 24,
		sc_MOTION_ORIGIN_Q = 25,
		sc_MOTION_GOORIGIN = 26,
		sc_MOTION_INITPOSITIONUNIT = 27,
		sc_MOTION_INITPOSITIONUNIT_Q = 28,
		sc_MOTION_INITPOSITION = 29,
		sc_MOTION_INITPOSITION_Q = 30,
		sc_MOTION_INITIOSIGNAL = 31,
		sc_MOTION_INITIOSIGNAL_Q = 32,
		sc_MOTION_GOINITPOSITION = 33,
		sc_MOTION_INCHINGMODE = 34,
		sc_MOTION_INCHINGMODE_Q = 35,
		sc_MOTION_INCHINGTIME = 36,
		sc_MOTION_INCHINGTIME_Q = 37,
		sc_MOTION_INCHING = 38,
		sc_MOTION_OFFSETSTATE = 39,
		sc_MOTION_OFFSETSTATE_Q = 40,
		sc_MOTION_OFFSET = 41,
		sc_MOTION_PVTSTEPS_Q = 42,
		sc_MOTION_COUNTSTEPS_Q = 43,
		sc_MOTION_PVTCIRCLE_Q = 44,
		sc_MOTION_COUNTCIRCLE_Q = 45,
		sc_MOTION_ABCOUNT_Q = 46,
		sc_MOTION_REVMOTION = 47,
		sc_MOTION_REVMOTION_Q = 48,

	mc_IDENTITY = 22,
		sc_IDENTITY_GROUP = 0,
		sc_IDENTITY_GROUP_Q = 1,
		sc_IDENTITY_DISTDEVICE = 2,
		sc_IDENTITY_DISTDEVICE_Q = 3,

	mc_STOPDECEL = 51,
		sc_STOPDECEL_MODE = 0,
		sc_STOPDECEL_MODE_Q = 1,
		sc_STOPDECEL_DISTANCE = 2,
		sc_STOPDECEL_DISTANCE_Q = 3,
		sc_STOPDECEL_TIME = 4,
		sc_STOPDECEL_TIME_Q = 5,

	mc_OUTOFSTEP = 52,
		sc_OUTOFSTEP_LINESTATE = 0,
		sc_OUTOFSTEP_LINESTATE_Q = 1,
		sc_OUTOFSTEP_LINEOUTNUM = 2,
		sc_OUTOFSTEP_LINEOUTNUM_Q = 3,
		sc_OUTOFSTEP_LINERESPONSE = 4,
		sc_OUTOFSTEP_LINERESPONSE_Q = 5,
		sc_OUTOFSTEP_LINEWARN = 6,
		sc_OUTOFSTEP_TOTALSTATE = 7,
		sc_OUTOFSTEP_TOTALSTATE_Q = 8,
		sc_OUTOFSTEP_TOTALOUTNUM = 9,
		sc_OUTOFSTEP_TOTALOUTNUM_Q = 10,
		sc_OUTOFSTEP_TOTALRESPONSE = 11,
		sc_OUTOFSTEP_TOTALRESPONSE_Q = 12,
		sc_OUTOFSTEP_TOTALWARN = 13,

	mc_MOTOR = 53,
		sc_MOTOR_STEPANGLE = 0,
		sc_MOTOR_STEPANGLE_Q = 1,
		sc_MOTOR_TYPE = 2,
		sc_MOTOR_TYPE_Q = 3,
		sc_MOTOR_POSITIONUNIT = 4,
		sc_MOTOR_POSITIONUNIT_Q = 5,
		sc_MOTOR_GEARRATIONUM = 6,
		sc_MOTOR_GEARRATIONUM_Q = 7,
		sc_MOTOR_GEARRATIODEN = 8,
		sc_MOTOR_GEARRATIODEN_Q = 9,
		sc_MOTOR_LEAD = 10,
		sc_MOTOR_LEAD_Q = 11,
		sc_MOTOR_PEAKSPEED = 12,
		sc_MOTOR_PEAKSPEED_Q = 13,
		sc_MOTOR_PEAKACCELERATION = 14,
		sc_MOTOR_PEAKACCELERATION_Q = 15,
		sc_MOTOR_SIZE = 16,
		sc_MOTOR_SIZE_Q = 17,
		sc_MOTOR_VOLTAGE = 18,
		sc_MOTOR_VOLTAGE_Q = 19,
		sc_MOTOR_CURRENT = 20,
		sc_MOTOR_CURRENT_Q = 21,
		sc_MOTOR_BACKLASH = 22,
		sc_MOTOR_BACKLASH_Q = 23,

	mc_ENCODER = 54,
		sc_ENCODER_LINENUM = 0,
		sc_ENCODER_LINENUM_Q = 1,
		sc_ENCODER_CHANNELNUM = 2,
		sc_ENCODER_CHANNELNUM_Q = 3,
		sc_ENCODER_TYPE = 4,
		sc_ENCODER_TYPE_Q = 5,
		sc_ENCODER_MULTIPLE = 6,
		sc_ENCODER_MULTIPLE_Q = 7,
		sc_ENCODER_STATE = 8,
		sc_ENCODER_STATE_Q = 9,

	mc_MOTIONPLAN = 55,
		sc_MOTIONPLAN_PVTCONFIG = 0,
		sc_MOTIONPLAN_PVTCONFIG_Q = 1,
		sc_MOTIONPLAN_POSITION = 2,
		sc_MOTIONPLAN_POSITION_Q = 3,
		sc_MOTIONPLAN_VELOCITY = 4,
		sc_MOTIONPLAN_VELOCITY_Q = 5,
		sc_MOTIONPLAN_TIME = 6,
		sc_MOTIONPLAN_TIME_Q = 7,
		sc_MOTIONPLAN_EXECUTEMODE = 8,
		sc_MOTIONPLAN_EXECUTEMODE_Q = 9,
		sc_MOTIONPLAN_PLANMODE = 10,
		sc_MOTIONPLAN_PLANMODE_Q = 11,
		sc_MOTIONPLAN_MOTIONMODE = 12,
		sc_MOTIONPLAN_MOTIONMODE_Q = 13,
		sc_MOTIONPLAN_MODIFYDUTY = 14,
		sc_MOTIONPLAN_MODIFYDUTY_Q = 15,
		sc_MOTIONPLAN_REMAINPOINT_Q = 16,
		sc_MOTIONPLAN_OUTPUTPOINT_Q = 17,
		sc_MOTIONPLAN_STARTPOINT = 18,
		sc_MOTIONPLAN_STARTPOINT_Q = 19,
		sc_MOTIONPLAN_ENDPOINT = 20,
		sc_MOTIONPLAN_ENDPOINT_Q = 21,
		sc_MOTIONPLAN_CYCLENUM = 22,
		sc_MOTIONPLAN_CYCLENUM_Q = 23,
		sc_MOTIONPLAN_WARNPOINT = 24,
		sc_MOTIONPLAN_WARNPOINT_Q = 25,
		sc_MOTIONPLAN_WARN = 26,
		sc_MOTIONPLAN_ENDSTATE = 27,
		sc_MOTIONPLAN_ENDSTATE_Q = 28,
		sc_MOTIONPLAN_FEEDBACKRATIO = 29,
		sc_MOTIONPLAN_FEEDBACKRATIO_Q = 30,

	mc_REPORT = 56,
		sc_REPORT_STATE = 0,
		sc_REPORT_STATE_Q = 1,
		sc_REPORT_PERIOD = 2,
		sc_REPORT_PERIOD_Q = 3,
		sc_REPORT_DATA_Q = 4,

	mc_TRIGGER = 57,
		sc_TRIGGER_MODE = 0,
		sc_TRIGGER_MODE_Q = 1,
		sc_TRIGGER_PATTSTATE = 2,
		sc_TRIGGER_PATTSTATE_Q = 3,
		sc_TRIGGER_PATTERN = 4,
		sc_TRIGGER_PATTERN_Q = 5,
		sc_TRIGGER_PATTRESP = 6,
		sc_TRIGGER_PATTRESP_Q = 7,
		sc_TRIGGER_PATTSMODE = 8,
		sc_TRIGGER_PATTSMODE_Q = 9,
		sc_TRIGGER_PATTSPERIOD = 10,
		sc_TRIGGER_PATTSPERIOD_Q = 11,
		sc_TRIGGER_LEVELSTATE = 12,
		sc_TRIGGER_LEVELSTATE_Q = 13,
		sc_TRIGGER_LEVELTYPE = 14,
		sc_TRIGGER_LEVELTYPE_Q = 15,
		sc_TRIGGER_LEVELRESP = 16,
		sc_TRIGGER_LEVELRESP_Q = 17,
		sc_TRIGGER_LEVELSMODE = 18,
		sc_TRIGGER_LEVELSMODE_Q = 19,
		sc_TRIGGER_LEVELSPERIOD = 20,
		sc_TRIGGER_LEVELSPERIOD_Q = 21,

	mc_DRIVER = 58,
		sc_DRIVER_TYPE_Q = 0,
		sc_DRIVER_STATEREG_Q = 1,
		sc_DRIVER_CURRENT = 2,
		sc_DRIVER_CURRENT_Q = 3,
		sc_DRIVER_MICROSTEPS = 4,
		sc_DRIVER_MICROSTEPS_Q = 5,
		sc_DRIVER_SWITCH = 6,
		sc_DRIVER_SWITCH_Q = 7,
		sc_DRIVER_REGCONFIG = 8,
		sc_DRIVER_REGCONFIG_Q = 9,
		sc_DRIVER_SGLIMIT = 10,
		sc_DRIVER_SGLIMIT_Q = 11,
		sc_DRIVER_SGPARASET = 12,
		sc_DRIVER_SGPARASET_Q = 13,

	mc_DIGITALOUT = 59,
		sc_DIGITALOUT_STATE = 0,
		sc_DIGITALOUT_STATE_Q = 1,
		sc_DIGITALOUT_SIGNAL = 2,
		sc_DIGITALOUT_SIGNAL_Q = 3,
		sc_DIGITALOUT_POLARITY = 4,
		sc_DIGITALOUT_POLARITY_Q = 5,
		sc_DIGITALOUT_SOURCE = 6,
		sc_DIGITALOUT_SOURCE_Q = 7,
		sc_DIGITALOUT_CONDITION = 8,
		sc_DIGITALOUT_CONDITION_Q = 9,
		sc_DIGITALOUT_PERIOD = 10,
		sc_DIGITALOUT_PERIOD_Q = 11,
		sc_DIGITALOUT_DUTY = 12,
		sc_DIGITALOUT_DUTY_Q = 13,

	mc_ISOLATOROUTPUT = 60,
		sc_ISOLATOROUTPUT_STATE = 0,
		sc_ISOLATOROUTPUT_STATE_Q = 1,
		sc_ISOLATOROUTPUT_SOURCE = 2,
		sc_ISOLATOROUTPUT_SOURCE_Q = 3,
		sc_ISOLATOROUTPUT_CONDITION = 4,
		sc_ISOLATOROUTPUT_CONDITION_Q = 5,
		sc_ISOLATOROUTPUT_RESPONSE = 6,
		sc_ISOLATOROUTPUT_RESPONSE_Q = 7,

	mc_SENSORUART = 61,
		sc_SENSORUART_BAUD = 0,
		sc_SENSORUART_BAUD_Q = 1,
		sc_SENSORUART_WORDLEN = 2,
		sc_SENSORUART_WORDLEN_Q = 3,
		sc_SENSORUART_FLOWCTL = 4,
		sc_SENSORUART_FLOWCTL_Q = 5,
		sc_SENSORUART_PARITY = 6,
		sc_SENSORUART_PARITY_Q = 7,
		sc_SENSORUART_STOPBIT = 8,
		sc_SENSORUART_STOPBIT_Q = 9,
		sc_SENSORUART_APPLYPARA = 10,
		sc_SENSORUART_STATE = 11,
		sc_SENSORUART_STATE_Q = 12,
		sc_SENSORUART_SOF = 13,
		sc_SENSORUART_SOF_Q = 14,
		sc_SENSORUART_FRAMELEN = 15,
		sc_SENSORUART_FRAMELEN_Q = 16,
		sc_SENSORUART_RECEIVENUM = 17,
		sc_SENSORUART_RECEIVENUM_Q = 18,
		sc_SENSORUART_SWITCHTIME = 19,
		sc_SENSORUART_SWITCHTIME_Q = 20,
		sc_SENSORUART_DATA_Q = 21,

	mc_PRESET = 62,
		sc_PRESET_PVTCONFIG = 0,
		sc_PRESET_PVTCONFIG_Q = 1,
		sc_PRESET_POSITION = 2,
		sc_PRESET_POSITION_Q = 3,
		sc_PRESET_SPEED = 4,
		sc_PRESET_SPEED_Q = 5,
		sc_PRESET_TIME = 6,
		sc_PRESET_TIME_Q = 7,
		sc_PRESET_REMAINPOINT_Q = 8,
		sc_PRESET_EXECUTE = 9,
		sc_PRESET_STATE_Q = 10,
		sc_PRESET_ENDSTATE = 11,
		sc_PRESET_ENDSTATE_Q = 12,
		sc_PRESET_STOPMODE = 13,
		sc_PRESET_STOPMODE_Q = 14,
		sc_PRESET_STOPDISTANCE = 15,
		sc_PRESET_STOPDISTANCE_Q = 16,

	mc_ISOLATORIN = 72,
		sc_ISOLATORIN_STATE = 0,
		sc_ISOLATORIN_STATE_Q = 1,
		sc_ISOLATORIN_TYPE = 2,
		sc_ISOLATORIN_TYPE_Q = 3,
		sc_ISOLATORIN_RESPONSE = 4,
		sc_ISOLATORIN_RESPONSE_Q = 5,
		sc_ISOLATORIN_RESPCHAN = 6,
		sc_ISOLATORIN_RESPCHAN_Q = 7,
		sc_ISOLATORIN_SMODE = 8,
		sc_ISOLATORIN_SMODE_Q = 9,
		sc_ISOLATORIN_SPERIOD = 10,
		sc_ISOLATORIN_SPERIOD_Q = 11,
};
#endif
