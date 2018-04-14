static value_desc _value_desc_MRQ_LINK_INTFC[] = {
	{0,"NONE",},
	{1,"CAN",},
	{2,"RS232",},
};
static value_desc _value_desc_MRQ_LINK_DEVICEINFO[] = {
	{0,"MRQ",},
	{1,"MRV",},
};
static value_desc _value_desc_MRQ_LINK_DEVICEINFO_1[] = {
	{0,"C23D",},
	{1,"C23S",},
	{2,"M2304",},
	{3,"M2305",},
};
static value_desc _value_desc_MRQ_SYSTEM_WORKMODE[] = {
	{0,"NORMAL",},
	{1,"NONE",},
};
static value_desc _value_desc_MRQ_SYSTEM_POWERON[] = {
	{0,"DEFAULT",},
	{1,"LAST",},
};
static value_desc _value_desc_MRQ_SYSTEM_ARMLEDPARA[] = {
	{0,"BASE",},
	{1,"BIGARM",},
	{2,"SMALLARM",},
	{3,"WRIST",},
};
static value_desc _value_desc_MRQ_RS232_BAUD[] = {
	{0,"4800",},
	{1,"7200",},
	{2,"9600",},
	{3,"14400",},
	{4,"19200",},
	{5,"38400",},
	{6,"57600",},
	{7,"115200",},
	{8,"128000",},
};
static value_desc _value_desc_MRQ_RS232_WORDLEN[] = {
	{0,"8",},
	{1,"9",},
};
static value_desc _value_desc_MRQ_RS232_FLOWCTL[] = {
	{0,"NONE",},
	{1,"RTS",},
	{2,"CTS",},
	{3,"RTS&CTS",},
};
static value_desc _value_desc_MRQ_RS232_PARITY[] = {
	{0,"NONE",},
	{1,"EVEN",},
	{2,"ODD",},
};
static value_desc _value_desc_MRQ_RS232_STOPBIT[] = {
	{0,"1",},
	{1,"0.5",},
	{2,"2",},
	{3,"1.5",},
};
static value_desc _value_desc_MRQ_CAN_TYPE[] = {
	{0,"A",},
	{1,"B",},
};
static value_desc _value_desc_MRQ_CAN_BAUD[] = {
	{0,"1000",},
	{1,"500",},
	{2,"250",},
	{3,"125",},
	{4,"100",},
	{5,"50",},
	{6,"20",},
	{7,"10",},
};
static value_desc _value_desc_MRQ_CAN_NETMANAGELED[] = {
	{0,"OFF",},
	{1,"ON",},
};
static value_desc _value_desc_MRQ_CAN_NETMANAGESTATE[] = {
	{0,"IDLE",},
	{1,"HASH",},
	{2,"SIGNATURE",},
};
static value_desc _value_desc_MRQ_CAN_NETMANAGEID[] = {
	{0,"NONE",},
	{1,"SENDID",},
	{2,"RECEIVEID",},
};
static value_desc _value_desc_MRQ_CLOCK_STARTTYPE[] = {
	{0,"RADIOID",},
	{1,"RECEIVEID",},
	{2,"GROUPID1",},
	{3,"GROUPID2",},
};
static value_desc _value_desc_MRQ_CLOCK_SYNCSTATE[] = {
	{0,"END",},
	{1,"START",},
};
static value_desc _value_desc_MRQ_MOTION_SWITCH[] = {
	{0,"RESET",},
	{1,"STOP",},
	{2,"RUN",},
	{3,"PREPARE",},
	{4,"EMERGSTOP",},
};
static value_desc _value_desc_MRQ_MOTION_SWITCH_1[] = {
	{0,"MAIN",},
	{1,"SMALL",},
	{2,"P1",},
	{3,"P2",},
	{4,"P3",},
	{5,"P4",},
	{6,"P5",},
	{7,"P6",},
	{8,"P7",},
	{9,"P8",},
};
static value_desc _value_desc_MRQ_MOTION_STATE_2[] = {
	{0,"POWERON",},
	{1,"IDLE",},
	{2,"CALCING",},
	{3,"CALCEND",},
	{4,"STANDBY",},
	{5,"RUNNING",},
	{6,"ERROR",},
};
static value_desc _value_desc_MRQ_MOTION_STATEREPORT[] = {
	{0,"ACTIVE",},
	{1,"QUERY",},
};
static value_desc _value_desc_MRQ_MOTION_STARTSOURCE[] = {
	{0,"SOFTWARE",},
	{1,"DIGITALIO",},
	{2,"CAN",},
	{3,"ALL",},
};
static value_desc _value_desc_MRQ_IDENTITY_GROUP[] = {
	{0,"GROUP1",},
	{1,"GROUP2",},
};
static value_desc _value_desc_MRQ_IDENTITY_LABEL[] = {
	{0,"SINANJU",},
	{1,"MEGATRON",},
	{2,"GOUF",},
};
static value_desc _value_desc_MRQ_IDENTITY_LABEL_1[] = {
	{0,"S1",},
	{1,"S2",},
	{2,"S3",},
	{3,"S4",},
};
static value_desc _value_desc_MRQ_MOTOR_STEPANGLE[] = {
	{0,"1.8",},
	{1,"0.9",},
	{2,"15",},
	{3,"7.5",},
};
static value_desc _value_desc_MRQ_MOTOR_TYPE[] = {
	{0,"ROTARY",},
	{1,"LINEAR",},
};
static value_desc _value_desc_MRQ_MOTOR_POSITIONUNIT[] = {
	{0,"ANGLE",},
	{1,"RADIAN",},
	{2,"MILLIMETER",},
};
static value_desc _value_desc_MRQ_MOTOR_SIZE[] = {
	{0,"8",},
	{1,"11",},
	{2,"14",},
	{3,"17",},
	{4,"23",},
	{5,"24",},
};
static value_desc _value_desc_MRQ_ENCODER_CHANNELNUM[] = {
	{0,"1",},
	{1,"3",},
};
static value_desc _value_desc_MRQ_ENCODER_TYPE[] = {
	{0,"INCREMENTAL",},
	{1,"ABSOLUTE",},
};
static value_desc _value_desc_MRQ_ENCODER_MULTIPLE[] = {
	{0,"SINGLE",},
	{1,"DOUBLE",},
	{2,"QUADRUPLE",},
};
static value_desc _value_desc_MRQ_ENCODER_STATE[] = {
	{0,"NONE",},
	{1,"OFF",},
	{2,"ON",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_PVTCONFIG_1[] = {
	{0,"END",},
	{1,"CLEAR",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_PRESETSTATE_1[] = {
	{0,"OFF",},
	{1,"ON",},
	{2,"CALC",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_EXECUTEMODE_1[] = {
	{0,"CYCLE",},
	{1,"FIFO",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_PLANMODE_1[] = {
	{0,"CUBICPOLY",},
	{1,"LINEAR",},
	{2,"UNIFORM",},
	{3,"TRAPEZOID",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_MOTIONMODE_1[] = {
	{0,"PVT",},
	{1,"LVT_CORRECT",},
	{2,"LVT_NOCORRECT",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_MODIFYDUTY_1[] = {
	{0,"1/4",},
	{1,"1/8",},
	{2,"1/16",},
	{3,"1/32",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_ENDSTATE_1[] = {
	{0,"STOP",},
	{1,"HOLD",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_STOPMODE_1[] = {
	{0,"IMMEDIATE",},
	{1,"DISTANCE",},
};
static value_desc _value_desc_MRQ_MOTIONPLAN_OOSLINERESPONSE_1[] = {
	{0,"NONE",},
	{1,"ALARM",},
	{2,"STOP",},
	{3,"ALARM&STOP",},
};
static value_desc _value_desc_MRQ_REPORT_STATE[] = {
	{0,"TORQUE",},
	{1,"CYCLE",},
	{2,"XANGLE",},
	{3,"YANGLE",},
	{4,"ZANGLE",},
	{5,"XVEL",},
	{6,"YVEL",},
	{7,"ZVEL",},
	{8,"XACC",},
	{9,"YACC",},
	{10,"ZACC",},
	{11,"SGALL",},
	{12,"SGSE",},
	{13,"DIST",},
	{14,"ABSENC",},
};
static value_desc _value_desc_MRQ_TRIGGER_MODE[] = {
	{0,"PATTERN",},
	{1,"LEVEL",},
};
static value_desc _value_desc_MRQ_TRIGGER_PATTERN[] = {
	{0,"X",},
	{1,"L",},
	{2,"R",},
	{3,"F",},
	{4,"H",},
};
static value_desc _value_desc_MRQ_TRIGGER_PATTSMODE[] = {
	{0,"CONTINUED",},
	{1,"INTERVAL",},
	{2,"SINGLE",},
};
static value_desc _value_desc_MRQ_TRIGGER_LEVELSTATE[] = {
	{0,"TRIGL",},
	{1,"TRIGR",},
};
static value_desc _value_desc_MRQ_TRIGGER_LEVELTYPE_1[] = {
	{0,"RESERVE",},
	{1,"LOW",},
	{2,"RISE",},
	{3,"FALL",},
	{4,"HIGH",},
};
static value_desc _value_desc_MRQ_DRIVER_TYPE[] = {
	{0,"D17",},
	{1,"D23",},
};
static value_desc _value_desc_MRQ_DRIVER_STATEREG[] = {
	{0,"R0",},
	{1,"R1",},
	{2,"R2",},
};
static value_desc _value_desc_MRQ_DRIVER_MICROSTEPS[] = {
	{0,"256",},
	{1,"128",},
	{2,"64",},
	{3,"32",},
	{4,"16",},
	{5,"8",},
	{6,"4",},
	{7,"2",},
	{8,"1",},
};
static value_desc _value_desc_MRQ_DRIVER_SGLIMIT[] = {
	{0,"UPLIMIT",},
	{1,"DOWNLIMIT",},
};
static value_desc _value_desc_MRQ_DRIVER_SGPARASET[] = {
	{0,"SG0",},
	{1,"SGT",},
	{2,"SEMAX",},
	{3,"SEMIN",},
};
static value_desc _value_desc_MRQ_DRIVER_MINICURRRATIO[] = {
	{0,"1/2",},
	{1,"1/4",},
};
static value_desc _value_desc_MRQ_DRIVER_REGCONFIG[] = {
	{0,"DRVCTRL",},
	{1,"CHOPCONF",},
	{2,"SMARTEN",},
	{3,"SGCSCONF",},
	{4,"DRVCONF",},
};
static value_desc _value_desc_MRQ_DIGITALOUTPUT_STATE[] = {
	{0,"DO1",},
	{1,"DO2",},
	{2,"DO3",},
	{3,"DO4",},
};
static value_desc _value_desc_MRQ_DIGITALOUTPUT_STATE_1[] = {
	{0,"DISABLE",},
	{1,"ENABLE",},
};
static value_desc _value_desc_MRQ_DIGITALOUTPUT_SIGNAL_1[] = {
	{0,"LEVEL",},
	{1,"NONE",},
};
static value_desc _value_desc_MRQ_DIGITALOUTPUT_POLARITY_1[] = {
	{0,"POSITIVE",},
	{1,"NEGATIVE",},
};
static value_desc _value_desc_MRQ_DIGITALOUTPUT_CONDITION_1[] = {
	{0,"AUTO",},
	{1,"NONE",},
};
static value_desc _value_desc_MRQ_ISOLATOROUTPUT_STATE[] = {
	{0,"YO1",},
	{1,"YO2",},
};
static value_desc _value_desc_MRQ_ISOLATOROUTPUT_RESPONSE_1[] = {
	{0,"TURNON",},
	{1,"CUTOFF",},
};
static value_desc _value_desc_MRQ_SENSORUART_BAUD[] = {
	{0,"UART1",},
	{1,"UART2",},
};
static value_desc _value_desc_MRQ_SENSORUART_BAUD_1[] = {
	{0,"4800",},
	{1,"7200",},
	{2,"9600",},
	{3,"14400",},
	{4,"19200",},
	{5,"38400",},
	{6,"57600",},
	{7,"115200",},
	{8,"128000",},
	{9,"2000000",},
	{10,"2500000",},
};
static value_desc _value_desc_MRQ_ABSENCALARM_ZEROPOSITION_1[] = {
	{0,"NONE",},
	{1,"EXIS",},
};
const char* MRQ_LINK_INTFC_toString( MRQ_LINK_INTFC eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_LINK_INTFC ) ); 
}
const char* MRQ_LINK_DEVICEINFO_toString( MRQ_LINK_DEVICEINFO eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_LINK_DEVICEINFO ) ); 
}
const char* MRQ_LINK_DEVICEINFO_1_toString( MRQ_LINK_DEVICEINFO_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_LINK_DEVICEINFO_1 ) ); 
}
const char* MRQ_SYSTEM_WORKMODE_toString( MRQ_SYSTEM_WORKMODE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_SYSTEM_WORKMODE ) ); 
}
const char* MRQ_SYSTEM_POWERON_toString( MRQ_SYSTEM_POWERON eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_SYSTEM_POWERON ) ); 
}
const char* MRQ_SYSTEM_ARMLEDPARA_toString( MRQ_SYSTEM_ARMLEDPARA eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_SYSTEM_ARMLEDPARA ) ); 
}
const char* MRQ_RS232_BAUD_toString( MRQ_RS232_BAUD eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_RS232_BAUD ) ); 
}
const char* MRQ_RS232_WORDLEN_toString( MRQ_RS232_WORDLEN eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_RS232_WORDLEN ) ); 
}
const char* MRQ_RS232_FLOWCTL_toString( MRQ_RS232_FLOWCTL eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_RS232_FLOWCTL ) ); 
}
const char* MRQ_RS232_PARITY_toString( MRQ_RS232_PARITY eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_RS232_PARITY ) ); 
}
const char* MRQ_RS232_STOPBIT_toString( MRQ_RS232_STOPBIT eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_RS232_STOPBIT ) ); 
}
const char* MRQ_CAN_TYPE_toString( MRQ_CAN_TYPE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_CAN_TYPE ) ); 
}
const char* MRQ_CAN_BAUD_toString( MRQ_CAN_BAUD eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_CAN_BAUD ) ); 
}
const char* MRQ_CAN_NETMANAGELED_toString( MRQ_CAN_NETMANAGELED eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_CAN_NETMANAGELED ) ); 
}
const char* MRQ_CAN_NETMANAGESTATE_toString( MRQ_CAN_NETMANAGESTATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_CAN_NETMANAGESTATE ) ); 
}
const char* MRQ_CAN_NETMANAGEID_toString( MRQ_CAN_NETMANAGEID eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_CAN_NETMANAGEID ) ); 
}
const char* MRQ_CLOCK_STARTTYPE_toString( MRQ_CLOCK_STARTTYPE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_CLOCK_STARTTYPE ) ); 
}
const char* MRQ_CLOCK_SYNCSTATE_toString( MRQ_CLOCK_SYNCSTATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_CLOCK_SYNCSTATE ) ); 
}
const char* MRQ_MOTION_SWITCH_toString( MRQ_MOTION_SWITCH eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTION_SWITCH ) ); 
}
const char* MRQ_MOTION_SWITCH_1_toString( MRQ_MOTION_SWITCH_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTION_SWITCH_1 ) ); 
}
const char* MRQ_MOTION_STATE_2_toString( MRQ_MOTION_STATE_2 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTION_STATE_2 ) ); 
}
const char* MRQ_MOTION_STATEREPORT_toString( MRQ_MOTION_STATEREPORT eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTION_STATEREPORT ) ); 
}
const char* MRQ_MOTION_STARTSOURCE_toString( MRQ_MOTION_STARTSOURCE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTION_STARTSOURCE ) ); 
}
const char* MRQ_IDENTITY_GROUP_toString( MRQ_IDENTITY_GROUP eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_IDENTITY_GROUP ) ); 
}
const char* MRQ_IDENTITY_LABEL_toString( MRQ_IDENTITY_LABEL eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_IDENTITY_LABEL ) ); 
}
const char* MRQ_IDENTITY_LABEL_1_toString( MRQ_IDENTITY_LABEL_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_IDENTITY_LABEL_1 ) ); 
}
const char* MRQ_MOTOR_STEPANGLE_toString( MRQ_MOTOR_STEPANGLE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTOR_STEPANGLE ) ); 
}
const char* MRQ_MOTOR_TYPE_toString( MRQ_MOTOR_TYPE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTOR_TYPE ) ); 
}
const char* MRQ_MOTOR_POSITIONUNIT_toString( MRQ_MOTOR_POSITIONUNIT eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTOR_POSITIONUNIT ) ); 
}
const char* MRQ_MOTOR_SIZE_toString( MRQ_MOTOR_SIZE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTOR_SIZE ) ); 
}
const char* MRQ_ENCODER_CHANNELNUM_toString( MRQ_ENCODER_CHANNELNUM eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_ENCODER_CHANNELNUM ) ); 
}
const char* MRQ_ENCODER_TYPE_toString( MRQ_ENCODER_TYPE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_ENCODER_TYPE ) ); 
}
const char* MRQ_ENCODER_MULTIPLE_toString( MRQ_ENCODER_MULTIPLE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_ENCODER_MULTIPLE ) ); 
}
const char* MRQ_ENCODER_STATE_toString( MRQ_ENCODER_STATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_ENCODER_STATE ) ); 
}
const char* MRQ_MOTIONPLAN_PVTCONFIG_1_toString( MRQ_MOTIONPLAN_PVTCONFIG_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_PVTCONFIG_1 ) ); 
}
const char* MRQ_MOTIONPLAN_PRESETSTATE_1_toString( MRQ_MOTIONPLAN_PRESETSTATE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_PRESETSTATE_1 ) ); 
}
const char* MRQ_MOTIONPLAN_EXECUTEMODE_1_toString( MRQ_MOTIONPLAN_EXECUTEMODE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_EXECUTEMODE_1 ) ); 
}
const char* MRQ_MOTIONPLAN_PLANMODE_1_toString( MRQ_MOTIONPLAN_PLANMODE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_PLANMODE_1 ) ); 
}
const char* MRQ_MOTIONPLAN_MOTIONMODE_1_toString( MRQ_MOTIONPLAN_MOTIONMODE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_MOTIONMODE_1 ) ); 
}
const char* MRQ_MOTIONPLAN_MODIFYDUTY_1_toString( MRQ_MOTIONPLAN_MODIFYDUTY_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_MODIFYDUTY_1 ) ); 
}
const char* MRQ_MOTIONPLAN_ENDSTATE_1_toString( MRQ_MOTIONPLAN_ENDSTATE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_ENDSTATE_1 ) ); 
}
const char* MRQ_MOTIONPLAN_STOPMODE_1_toString( MRQ_MOTIONPLAN_STOPMODE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_STOPMODE_1 ) ); 
}
const char* MRQ_MOTIONPLAN_OOSLINERESPONSE_1_toString( MRQ_MOTIONPLAN_OOSLINERESPONSE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_MOTIONPLAN_OOSLINERESPONSE_1 ) ); 
}
const char* MRQ_REPORT_STATE_toString( MRQ_REPORT_STATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_REPORT_STATE ) ); 
}
const char* MRQ_TRIGGER_MODE_toString( MRQ_TRIGGER_MODE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_TRIGGER_MODE ) ); 
}
const char* MRQ_TRIGGER_PATTERN_toString( MRQ_TRIGGER_PATTERN eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_TRIGGER_PATTERN ) ); 
}
const char* MRQ_TRIGGER_PATTSMODE_toString( MRQ_TRIGGER_PATTSMODE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_TRIGGER_PATTSMODE ) ); 
}
const char* MRQ_TRIGGER_LEVELSTATE_toString( MRQ_TRIGGER_LEVELSTATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_TRIGGER_LEVELSTATE ) ); 
}
const char* MRQ_TRIGGER_LEVELTYPE_1_toString( MRQ_TRIGGER_LEVELTYPE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_TRIGGER_LEVELTYPE_1 ) ); 
}
const char* MRQ_DRIVER_TYPE_toString( MRQ_DRIVER_TYPE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DRIVER_TYPE ) ); 
}
const char* MRQ_DRIVER_STATEREG_toString( MRQ_DRIVER_STATEREG eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DRIVER_STATEREG ) ); 
}
const char* MRQ_DRIVER_MICROSTEPS_toString( MRQ_DRIVER_MICROSTEPS eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DRIVER_MICROSTEPS ) ); 
}
const char* MRQ_DRIVER_SGLIMIT_toString( MRQ_DRIVER_SGLIMIT eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DRIVER_SGLIMIT ) ); 
}
const char* MRQ_DRIVER_SGPARASET_toString( MRQ_DRIVER_SGPARASET eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DRIVER_SGPARASET ) ); 
}
const char* MRQ_DRIVER_MINICURRRATIO_toString( MRQ_DRIVER_MINICURRRATIO eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DRIVER_MINICURRRATIO ) ); 
}
const char* MRQ_DRIVER_REGCONFIG_toString( MRQ_DRIVER_REGCONFIG eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DRIVER_REGCONFIG ) ); 
}
const char* MRQ_DIGITALOUTPUT_STATE_toString( MRQ_DIGITALOUTPUT_STATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DIGITALOUTPUT_STATE ) ); 
}
const char* MRQ_DIGITALOUTPUT_STATE_1_toString( MRQ_DIGITALOUTPUT_STATE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DIGITALOUTPUT_STATE_1 ) ); 
}
const char* MRQ_DIGITALOUTPUT_SIGNAL_1_toString( MRQ_DIGITALOUTPUT_SIGNAL_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DIGITALOUTPUT_SIGNAL_1 ) ); 
}
const char* MRQ_DIGITALOUTPUT_POLARITY_1_toString( MRQ_DIGITALOUTPUT_POLARITY_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DIGITALOUTPUT_POLARITY_1 ) ); 
}
const char* MRQ_DIGITALOUTPUT_CONDITION_1_toString( MRQ_DIGITALOUTPUT_CONDITION_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_DIGITALOUTPUT_CONDITION_1 ) ); 
}
const char* MRQ_ISOLATOROUTPUT_STATE_toString( MRQ_ISOLATOROUTPUT_STATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_ISOLATOROUTPUT_STATE ) ); 
}
const char* MRQ_ISOLATOROUTPUT_RESPONSE_1_toString( MRQ_ISOLATOROUTPUT_RESPONSE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_ISOLATOROUTPUT_RESPONSE_1 ) ); 
}
const char* MRQ_SENSORUART_BAUD_toString( MRQ_SENSORUART_BAUD eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_SENSORUART_BAUD ) ); 
}
const char* MRQ_SENSORUART_BAUD_1_toString( MRQ_SENSORUART_BAUD_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_SENSORUART_BAUD_1 ) ); 
}
const char* MRQ_ABSENCALARM_ZEROPOSITION_1_toString( MRQ_ABSENCALARM_ZEROPOSITION_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRQ_ABSENCALARM_ZEROPOSITION_1 ) ); 
}
int MRQ_LINK_INTFC_toValue( const char *pStr, MRQ_LINK_INTFC *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_LINK_INTFC ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_LINK_INTFC)lval;
	return 0;
}
int MRQ_LINK_DEVICEINFO_toValue( const char *pStr, MRQ_LINK_DEVICEINFO *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_LINK_DEVICEINFO ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_LINK_DEVICEINFO)lval;
	return 0;
}
int MRQ_LINK_DEVICEINFO_1_toValue( const char *pStr, MRQ_LINK_DEVICEINFO_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_LINK_DEVICEINFO_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_LINK_DEVICEINFO_1)lval;
	return 0;
}
int MRQ_SYSTEM_WORKMODE_toValue( const char *pStr, MRQ_SYSTEM_WORKMODE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_SYSTEM_WORKMODE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_SYSTEM_WORKMODE)lval;
	return 0;
}
int MRQ_SYSTEM_POWERON_toValue( const char *pStr, MRQ_SYSTEM_POWERON *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_SYSTEM_POWERON ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_SYSTEM_POWERON)lval;
	return 0;
}
int MRQ_SYSTEM_ARMLEDPARA_toValue( const char *pStr, MRQ_SYSTEM_ARMLEDPARA *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_SYSTEM_ARMLEDPARA ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_SYSTEM_ARMLEDPARA)lval;
	return 0;
}
int MRQ_RS232_BAUD_toValue( const char *pStr, MRQ_RS232_BAUD *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_RS232_BAUD ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_RS232_BAUD)lval;
	return 0;
}
int MRQ_RS232_WORDLEN_toValue( const char *pStr, MRQ_RS232_WORDLEN *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_RS232_WORDLEN ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_RS232_WORDLEN)lval;
	return 0;
}
int MRQ_RS232_FLOWCTL_toValue( const char *pStr, MRQ_RS232_FLOWCTL *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_RS232_FLOWCTL ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_RS232_FLOWCTL)lval;
	return 0;
}
int MRQ_RS232_PARITY_toValue( const char *pStr, MRQ_RS232_PARITY *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_RS232_PARITY ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_RS232_PARITY)lval;
	return 0;
}
int MRQ_RS232_STOPBIT_toValue( const char *pStr, MRQ_RS232_STOPBIT *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_RS232_STOPBIT ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_RS232_STOPBIT)lval;
	return 0;
}
int MRQ_CAN_TYPE_toValue( const char *pStr, MRQ_CAN_TYPE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_CAN_TYPE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_CAN_TYPE)lval;
	return 0;
}
int MRQ_CAN_BAUD_toValue( const char *pStr, MRQ_CAN_BAUD *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_CAN_BAUD ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_CAN_BAUD)lval;
	return 0;
}
int MRQ_CAN_NETMANAGELED_toValue( const char *pStr, MRQ_CAN_NETMANAGELED *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_CAN_NETMANAGELED ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_CAN_NETMANAGELED)lval;
	return 0;
}
int MRQ_CAN_NETMANAGESTATE_toValue( const char *pStr, MRQ_CAN_NETMANAGESTATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_CAN_NETMANAGESTATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_CAN_NETMANAGESTATE)lval;
	return 0;
}
int MRQ_CAN_NETMANAGEID_toValue( const char *pStr, MRQ_CAN_NETMANAGEID *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_CAN_NETMANAGEID ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_CAN_NETMANAGEID)lval;
	return 0;
}
int MRQ_CLOCK_STARTTYPE_toValue( const char *pStr, MRQ_CLOCK_STARTTYPE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_CLOCK_STARTTYPE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_CLOCK_STARTTYPE)lval;
	return 0;
}
int MRQ_CLOCK_SYNCSTATE_toValue( const char *pStr, MRQ_CLOCK_SYNCSTATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_CLOCK_SYNCSTATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_CLOCK_SYNCSTATE)lval;
	return 0;
}
int MRQ_MOTION_SWITCH_toValue( const char *pStr, MRQ_MOTION_SWITCH *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTION_SWITCH ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTION_SWITCH)lval;
	return 0;
}
int MRQ_MOTION_SWITCH_1_toValue( const char *pStr, MRQ_MOTION_SWITCH_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTION_SWITCH_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTION_SWITCH_1)lval;
	return 0;
}
int MRQ_MOTION_STATE_2_toValue( const char *pStr, MRQ_MOTION_STATE_2 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTION_STATE_2 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTION_STATE_2)lval;
	return 0;
}
int MRQ_MOTION_STATEREPORT_toValue( const char *pStr, MRQ_MOTION_STATEREPORT *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTION_STATEREPORT ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTION_STATEREPORT)lval;
	return 0;
}
int MRQ_MOTION_STARTSOURCE_toValue( const char *pStr, MRQ_MOTION_STARTSOURCE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTION_STARTSOURCE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTION_STARTSOURCE)lval;
	return 0;
}
int MRQ_IDENTITY_GROUP_toValue( const char *pStr, MRQ_IDENTITY_GROUP *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_IDENTITY_GROUP ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_IDENTITY_GROUP)lval;
	return 0;
}
int MRQ_IDENTITY_LABEL_toValue( const char *pStr, MRQ_IDENTITY_LABEL *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_IDENTITY_LABEL ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_IDENTITY_LABEL)lval;
	return 0;
}
int MRQ_IDENTITY_LABEL_1_toValue( const char *pStr, MRQ_IDENTITY_LABEL_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_IDENTITY_LABEL_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_IDENTITY_LABEL_1)lval;
	return 0;
}
int MRQ_MOTOR_STEPANGLE_toValue( const char *pStr, MRQ_MOTOR_STEPANGLE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTOR_STEPANGLE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTOR_STEPANGLE)lval;
	return 0;
}
int MRQ_MOTOR_TYPE_toValue( const char *pStr, MRQ_MOTOR_TYPE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTOR_TYPE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTOR_TYPE)lval;
	return 0;
}
int MRQ_MOTOR_POSITIONUNIT_toValue( const char *pStr, MRQ_MOTOR_POSITIONUNIT *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTOR_POSITIONUNIT ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTOR_POSITIONUNIT)lval;
	return 0;
}
int MRQ_MOTOR_SIZE_toValue( const char *pStr, MRQ_MOTOR_SIZE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTOR_SIZE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTOR_SIZE)lval;
	return 0;
}
int MRQ_ENCODER_CHANNELNUM_toValue( const char *pStr, MRQ_ENCODER_CHANNELNUM *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_ENCODER_CHANNELNUM ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_ENCODER_CHANNELNUM)lval;
	return 0;
}
int MRQ_ENCODER_TYPE_toValue( const char *pStr, MRQ_ENCODER_TYPE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_ENCODER_TYPE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_ENCODER_TYPE)lval;
	return 0;
}
int MRQ_ENCODER_MULTIPLE_toValue( const char *pStr, MRQ_ENCODER_MULTIPLE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_ENCODER_MULTIPLE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_ENCODER_MULTIPLE)lval;
	return 0;
}
int MRQ_ENCODER_STATE_toValue( const char *pStr, MRQ_ENCODER_STATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_ENCODER_STATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_ENCODER_STATE)lval;
	return 0;
}
int MRQ_MOTIONPLAN_PVTCONFIG_1_toValue( const char *pStr, MRQ_MOTIONPLAN_PVTCONFIG_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_PVTCONFIG_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_PVTCONFIG_1)lval;
	return 0;
}
int MRQ_MOTIONPLAN_PRESETSTATE_1_toValue( const char *pStr, MRQ_MOTIONPLAN_PRESETSTATE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_PRESETSTATE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_PRESETSTATE_1)lval;
	return 0;
}
int MRQ_MOTIONPLAN_EXECUTEMODE_1_toValue( const char *pStr, MRQ_MOTIONPLAN_EXECUTEMODE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_EXECUTEMODE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_EXECUTEMODE_1)lval;
	return 0;
}
int MRQ_MOTIONPLAN_PLANMODE_1_toValue( const char *pStr, MRQ_MOTIONPLAN_PLANMODE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_PLANMODE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_PLANMODE_1)lval;
	return 0;
}
int MRQ_MOTIONPLAN_MOTIONMODE_1_toValue( const char *pStr, MRQ_MOTIONPLAN_MOTIONMODE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_MOTIONMODE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_MOTIONMODE_1)lval;
	return 0;
}
int MRQ_MOTIONPLAN_MODIFYDUTY_1_toValue( const char *pStr, MRQ_MOTIONPLAN_MODIFYDUTY_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_MODIFYDUTY_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_MODIFYDUTY_1)lval;
	return 0;
}
int MRQ_MOTIONPLAN_ENDSTATE_1_toValue( const char *pStr, MRQ_MOTIONPLAN_ENDSTATE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_ENDSTATE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_ENDSTATE_1)lval;
	return 0;
}
int MRQ_MOTIONPLAN_STOPMODE_1_toValue( const char *pStr, MRQ_MOTIONPLAN_STOPMODE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_STOPMODE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_STOPMODE_1)lval;
	return 0;
}
int MRQ_MOTIONPLAN_OOSLINERESPONSE_1_toValue( const char *pStr, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_MOTIONPLAN_OOSLINERESPONSE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_MOTIONPLAN_OOSLINERESPONSE_1)lval;
	return 0;
}
int MRQ_REPORT_STATE_toValue( const char *pStr, MRQ_REPORT_STATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_REPORT_STATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_REPORT_STATE)lval;
	return 0;
}
int MRQ_TRIGGER_MODE_toValue( const char *pStr, MRQ_TRIGGER_MODE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_TRIGGER_MODE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_TRIGGER_MODE)lval;
	return 0;
}
int MRQ_TRIGGER_PATTERN_toValue( const char *pStr, MRQ_TRIGGER_PATTERN *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_TRIGGER_PATTERN ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_TRIGGER_PATTERN)lval;
	return 0;
}
int MRQ_TRIGGER_PATTSMODE_toValue( const char *pStr, MRQ_TRIGGER_PATTSMODE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_TRIGGER_PATTSMODE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_TRIGGER_PATTSMODE)lval;
	return 0;
}
int MRQ_TRIGGER_LEVELSTATE_toValue( const char *pStr, MRQ_TRIGGER_LEVELSTATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_TRIGGER_LEVELSTATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_TRIGGER_LEVELSTATE)lval;
	return 0;
}
int MRQ_TRIGGER_LEVELTYPE_1_toValue( const char *pStr, MRQ_TRIGGER_LEVELTYPE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_TRIGGER_LEVELTYPE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_TRIGGER_LEVELTYPE_1)lval;
	return 0;
}
int MRQ_DRIVER_TYPE_toValue( const char *pStr, MRQ_DRIVER_TYPE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DRIVER_TYPE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DRIVER_TYPE)lval;
	return 0;
}
int MRQ_DRIVER_STATEREG_toValue( const char *pStr, MRQ_DRIVER_STATEREG *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DRIVER_STATEREG ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DRIVER_STATEREG)lval;
	return 0;
}
int MRQ_DRIVER_MICROSTEPS_toValue( const char *pStr, MRQ_DRIVER_MICROSTEPS *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DRIVER_MICROSTEPS ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DRIVER_MICROSTEPS)lval;
	return 0;
}
int MRQ_DRIVER_SGLIMIT_toValue( const char *pStr, MRQ_DRIVER_SGLIMIT *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DRIVER_SGLIMIT ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DRIVER_SGLIMIT)lval;
	return 0;
}
int MRQ_DRIVER_SGPARASET_toValue( const char *pStr, MRQ_DRIVER_SGPARASET *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DRIVER_SGPARASET ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DRIVER_SGPARASET)lval;
	return 0;
}
int MRQ_DRIVER_MINICURRRATIO_toValue( const char *pStr, MRQ_DRIVER_MINICURRRATIO *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DRIVER_MINICURRRATIO ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DRIVER_MINICURRRATIO)lval;
	return 0;
}
int MRQ_DRIVER_REGCONFIG_toValue( const char *pStr, MRQ_DRIVER_REGCONFIG *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DRIVER_REGCONFIG ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DRIVER_REGCONFIG)lval;
	return 0;
}
int MRQ_DIGITALOUTPUT_STATE_toValue( const char *pStr, MRQ_DIGITALOUTPUT_STATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DIGITALOUTPUT_STATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DIGITALOUTPUT_STATE)lval;
	return 0;
}
int MRQ_DIGITALOUTPUT_STATE_1_toValue( const char *pStr, MRQ_DIGITALOUTPUT_STATE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DIGITALOUTPUT_STATE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DIGITALOUTPUT_STATE_1)lval;
	return 0;
}
int MRQ_DIGITALOUTPUT_SIGNAL_1_toValue( const char *pStr, MRQ_DIGITALOUTPUT_SIGNAL_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DIGITALOUTPUT_SIGNAL_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DIGITALOUTPUT_SIGNAL_1)lval;
	return 0;
}
int MRQ_DIGITALOUTPUT_POLARITY_1_toValue( const char *pStr, MRQ_DIGITALOUTPUT_POLARITY_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DIGITALOUTPUT_POLARITY_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DIGITALOUTPUT_POLARITY_1)lval;
	return 0;
}
int MRQ_DIGITALOUTPUT_CONDITION_1_toValue( const char *pStr, MRQ_DIGITALOUTPUT_CONDITION_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_DIGITALOUTPUT_CONDITION_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_DIGITALOUTPUT_CONDITION_1)lval;
	return 0;
}
int MRQ_ISOLATOROUTPUT_STATE_toValue( const char *pStr, MRQ_ISOLATOROUTPUT_STATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_ISOLATOROUTPUT_STATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_ISOLATOROUTPUT_STATE)lval;
	return 0;
}
int MRQ_ISOLATOROUTPUT_RESPONSE_1_toValue( const char *pStr, MRQ_ISOLATOROUTPUT_RESPONSE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_ISOLATOROUTPUT_RESPONSE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_ISOLATOROUTPUT_RESPONSE_1)lval;
	return 0;
}
int MRQ_SENSORUART_BAUD_toValue( const char *pStr, MRQ_SENSORUART_BAUD *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_SENSORUART_BAUD ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_SENSORUART_BAUD)lval;
	return 0;
}
int MRQ_SENSORUART_BAUD_1_toValue( const char *pStr, MRQ_SENSORUART_BAUD_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_SENSORUART_BAUD_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_SENSORUART_BAUD_1)lval;
	return 0;
}
int MRQ_ABSENCALARM_ZEROPOSITION_1_toValue( const char *pStr, MRQ_ABSENCALARM_ZEROPOSITION_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRQ_ABSENCALARM_ZEROPOSITION_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRQ_ABSENCALARM_ZEROPOSITION_1)lval;
	return 0;
}
