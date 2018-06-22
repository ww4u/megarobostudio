static value_desc _value_desc_MRV_LINK_INTFC[] = {
	{0,"NONE",},
	{1,"CAN",},
	{2,"RS232",},
};
static value_desc _value_desc_MRV_LINK_DEVICEINFO[] = {
	{0,"RESERVE",},
	{1,"MRV",},
};
static value_desc _value_desc_MRV_LINK_DEVICEINFO_1[] = {
	{0,"M3604",},
	{1,"OTHER",},
};
static value_desc _value_desc_MRV_SYSTEM_WORKMODE[] = {
	{0,"NORMAL",},
	{1,"NONE",},
};
static value_desc _value_desc_MRV_SYSTEM_POWERON[] = {
	{0,"DEFAULT",},
	{1,"LAST",},
};
static value_desc _value_desc_MRV_SYSTEM_TYPE[] = {
	{0,"MRQ",},
	{1,"MRV",},
};
static value_desc _value_desc_MRV_RS232_BAUD[] = {
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
static value_desc _value_desc_MRV_RS232_WORDLEN[] = {
	{0,"8",},
	{1,"9",},
};
static value_desc _value_desc_MRV_RS232_FLOWCTL[] = {
	{0,"NONE",},
	{1,"RTS",},
	{2,"CTS",},
	{3,"RTS&CTS",},
};
static value_desc _value_desc_MRV_RS232_PARITY[] = {
	{0,"NONE",},
	{1,"EVEN",},
	{2,"ODD",},
};
static value_desc _value_desc_MRV_RS232_STOPBIT[] = {
	{0,"1",},
	{1,"0.5",},
	{2,"2",},
	{3,"1.5",},
};
static value_desc _value_desc_MRV_CAN_TYPE[] = {
	{0,"A",},
	{1,"B",},
};
static value_desc _value_desc_MRV_CAN_BAUD[] = {
	{0,"1000",},
	{1,"500",},
	{2,"250",},
	{3,"125",},
	{4,"100",},
	{5,"50",},
	{6,"20",},
	{7,"10",},
};
static value_desc _value_desc_MRV_CAN_NETMANAGELED[] = {
	{0,"OFF",},
	{1,"ON",},
};
static value_desc _value_desc_MRV_CAN_NETMANAGESTATE[] = {
	{0,"IDLE",},
	{1,"HASH",},
	{2,"SIGNATURE",},
};
static value_desc _value_desc_MRV_MOTION_SWITCH[] = {
	{0,"RESET",},
	{1,"STOP",},
	{2,"RUN",},
};
static value_desc _value_desc_MRV_MOTION_SWITCH_1[] = {
	{0,"MAIN",},
	{1,"RESSERVE1",},
};
static value_desc _value_desc_MRV_MOTION_STATE_1[] = {
	{0,"reserve",},
	{1,"IDLE",},
	{2,"reserve1",},
	{3,"CALCEND",},
	{4,"reserve2",},
	{5,"RUNNING",},
	{6,"reserve3",},
};
static value_desc _value_desc_MRV_MOTION_STATEREPORT[] = {
	{0,"ACTIVE",},
	{1,"QUERY",},
};
static value_desc _value_desc_MRV_GLOBAL_GROUP[] = {
	{0,"GROUP1",},
	{1,"GROUP2",},
};
static value_desc _value_desc_MRV_VALVECTRL_DEVICE[] = {
	{0,"VALVE",},
	{1,"MOTOR",},
};
static value_desc _value_desc_MRV_VALVECTRL_ACTION[] = {
	{0,"OFF",},
	{1,"ON",},
	{2,"IDLE",},
};
static value_desc _value_desc_MRV_THRESHOLD_HIPRESSUREACTION[] = {
	{0,"NONE",},
	{1,"WARN",},
	{2,"STOP",},
};
static value_desc _value_desc_MRV_PVT_EXECMODE[] = {
	{0,"RT",},
	{1,"NCYCLE",},
	{2,"FIFO",},
};
static value_desc _value_desc_MRV_PVT_HOLD[] = {
	{0,"TRUE",},
	{1,"FALSE",},
};
static value_desc _value_desc_MRV_CALIBRATION_TYPE[] = {
	{0,"NONE",},
	{1,"OPEN",},
	{2,"CLOSE",},
};
static value_desc _value_desc_MRV_CALIBRATION_SAMPLERATE[] = {
	{0,"222KHz",},
	{1,"176KHz",},
	{2,"142KHz",},
	{3,"47KHz",},
};
static value_desc _value_desc_MRV_CALIBRATION_DATAUPCTRL[] = {
	{0,"START",},
	{1,"STOP",},
};
static value_desc _value_desc_MRV_REPORT_STATE[] = {
	{0,"Current",},
	{1,"Pressure",},
	{2,"Position",},
	{3,"Cycle",},
};
static value_desc _value_desc_MRV_IOCONFIG_IOFUNCSEL[] = {
	{0,"UART",},
	{1,"SWITCH",},
};
static value_desc _value_desc_MRV_IOCONFIG_TRIGEDGE[] = {
	{0,"NONE",},
	{1,"RISE",},
	{2,"FALL",},
	{3,"BOTH",},
};
static value_desc _value_desc_MRV_IOCONFIG_FLOWCTL[] = {
	{0,"NONE",},
	{1,"RTS",},
	{2,"CTS",},
	{3,"RTS_CTS",},
};
const char* MRV_LINK_INTFC_toString( MRV_LINK_INTFC eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_LINK_INTFC ) ); 
}
const char* MRV_LINK_DEVICEINFO_toString( MRV_LINK_DEVICEINFO eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_LINK_DEVICEINFO ) ); 
}
const char* MRV_LINK_DEVICEINFO_1_toString( MRV_LINK_DEVICEINFO_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_LINK_DEVICEINFO_1 ) ); 
}
const char* MRV_SYSTEM_WORKMODE_toString( MRV_SYSTEM_WORKMODE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_SYSTEM_WORKMODE ) ); 
}
const char* MRV_SYSTEM_POWERON_toString( MRV_SYSTEM_POWERON eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_SYSTEM_POWERON ) ); 
}
const char* MRV_SYSTEM_TYPE_toString( MRV_SYSTEM_TYPE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_SYSTEM_TYPE ) ); 
}
const char* MRV_RS232_BAUD_toString( MRV_RS232_BAUD eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_RS232_BAUD ) ); 
}
const char* MRV_RS232_WORDLEN_toString( MRV_RS232_WORDLEN eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_RS232_WORDLEN ) ); 
}
const char* MRV_RS232_FLOWCTL_toString( MRV_RS232_FLOWCTL eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_RS232_FLOWCTL ) ); 
}
const char* MRV_RS232_PARITY_toString( MRV_RS232_PARITY eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_RS232_PARITY ) ); 
}
const char* MRV_RS232_STOPBIT_toString( MRV_RS232_STOPBIT eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_RS232_STOPBIT ) ); 
}
const char* MRV_CAN_TYPE_toString( MRV_CAN_TYPE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_CAN_TYPE ) ); 
}
const char* MRV_CAN_BAUD_toString( MRV_CAN_BAUD eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_CAN_BAUD ) ); 
}
const char* MRV_CAN_NETMANAGELED_toString( MRV_CAN_NETMANAGELED eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_CAN_NETMANAGELED ) ); 
}
const char* MRV_CAN_NETMANAGESTATE_toString( MRV_CAN_NETMANAGESTATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_CAN_NETMANAGESTATE ) ); 
}
const char* MRV_MOTION_SWITCH_toString( MRV_MOTION_SWITCH eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_MOTION_SWITCH ) ); 
}
const char* MRV_MOTION_SWITCH_1_toString( MRV_MOTION_SWITCH_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_MOTION_SWITCH_1 ) ); 
}
const char* MRV_MOTION_STATE_1_toString( MRV_MOTION_STATE_1 eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_MOTION_STATE_1 ) ); 
}
const char* MRV_MOTION_STATEREPORT_toString( MRV_MOTION_STATEREPORT eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_MOTION_STATEREPORT ) ); 
}
const char* MRV_GLOBAL_GROUP_toString( MRV_GLOBAL_GROUP eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_GLOBAL_GROUP ) ); 
}
const char* MRV_VALVECTRL_DEVICE_toString( MRV_VALVECTRL_DEVICE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_VALVECTRL_DEVICE ) ); 
}
const char* MRV_VALVECTRL_ACTION_toString( MRV_VALVECTRL_ACTION eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_VALVECTRL_ACTION ) ); 
}
const char* MRV_THRESHOLD_HIPRESSUREACTION_toString( MRV_THRESHOLD_HIPRESSUREACTION eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_THRESHOLD_HIPRESSUREACTION ) ); 
}
const char* MRV_PVT_EXECMODE_toString( MRV_PVT_EXECMODE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_PVT_EXECMODE ) ); 
}
const char* MRV_PVT_HOLD_toString( MRV_PVT_HOLD eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_PVT_HOLD ) ); 
}
const char* MRV_CALIBRATION_TYPE_toString( MRV_CALIBRATION_TYPE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_CALIBRATION_TYPE ) ); 
}
const char* MRV_CALIBRATION_SAMPLERATE_toString( MRV_CALIBRATION_SAMPLERATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_CALIBRATION_SAMPLERATE ) ); 
}
const char* MRV_CALIBRATION_DATAUPCTRL_toString( MRV_CALIBRATION_DATAUPCTRL eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_CALIBRATION_DATAUPCTRL ) ); 
}
const char* MRV_REPORT_STATE_toString( MRV_REPORT_STATE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_REPORT_STATE ) ); 
}
const char* MRV_IOCONFIG_IOFUNCSEL_toString( MRV_IOCONFIG_IOFUNCSEL eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_IOCONFIG_IOFUNCSEL ) ); 
}
const char* MRV_IOCONFIG_TRIGEDGE_toString( MRV_IOCONFIG_TRIGEDGE eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_IOCONFIG_TRIGEDGE ) ); 
}
const char* MRV_IOCONFIG_FLOWCTL_toString( MRV_IOCONFIG_FLOWCTL eType )
{
	return enum_toString( (int)eType, desc_table( _value_desc_MRV_IOCONFIG_FLOWCTL ) ); 
}
int MRV_LINK_INTFC_toValue( const char *pStr, MRV_LINK_INTFC *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_LINK_INTFC ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_LINK_INTFC)lval;
	return 0;
}
int MRV_LINK_DEVICEINFO_toValue( const char *pStr, MRV_LINK_DEVICEINFO *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_LINK_DEVICEINFO ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_LINK_DEVICEINFO)lval;
	return 0;
}
int MRV_LINK_DEVICEINFO_1_toValue( const char *pStr, MRV_LINK_DEVICEINFO_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_LINK_DEVICEINFO_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_LINK_DEVICEINFO_1)lval;
	return 0;
}
int MRV_SYSTEM_WORKMODE_toValue( const char *pStr, MRV_SYSTEM_WORKMODE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_SYSTEM_WORKMODE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_SYSTEM_WORKMODE)lval;
	return 0;
}
int MRV_SYSTEM_POWERON_toValue( const char *pStr, MRV_SYSTEM_POWERON *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_SYSTEM_POWERON ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_SYSTEM_POWERON)lval;
	return 0;
}
int MRV_SYSTEM_TYPE_toValue( const char *pStr, MRV_SYSTEM_TYPE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_SYSTEM_TYPE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_SYSTEM_TYPE)lval;
	return 0;
}
int MRV_RS232_BAUD_toValue( const char *pStr, MRV_RS232_BAUD *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_RS232_BAUD ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_RS232_BAUD)lval;
	return 0;
}
int MRV_RS232_WORDLEN_toValue( const char *pStr, MRV_RS232_WORDLEN *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_RS232_WORDLEN ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_RS232_WORDLEN)lval;
	return 0;
}
int MRV_RS232_FLOWCTL_toValue( const char *pStr, MRV_RS232_FLOWCTL *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_RS232_FLOWCTL ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_RS232_FLOWCTL)lval;
	return 0;
}
int MRV_RS232_PARITY_toValue( const char *pStr, MRV_RS232_PARITY *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_RS232_PARITY ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_RS232_PARITY)lval;
	return 0;
}
int MRV_RS232_STOPBIT_toValue( const char *pStr, MRV_RS232_STOPBIT *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_RS232_STOPBIT ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_RS232_STOPBIT)lval;
	return 0;
}
int MRV_CAN_TYPE_toValue( const char *pStr, MRV_CAN_TYPE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_CAN_TYPE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_CAN_TYPE)lval;
	return 0;
}
int MRV_CAN_BAUD_toValue( const char *pStr, MRV_CAN_BAUD *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_CAN_BAUD ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_CAN_BAUD)lval;
	return 0;
}
int MRV_CAN_NETMANAGELED_toValue( const char *pStr, MRV_CAN_NETMANAGELED *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_CAN_NETMANAGELED ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_CAN_NETMANAGELED)lval;
	return 0;
}
int MRV_CAN_NETMANAGESTATE_toValue( const char *pStr, MRV_CAN_NETMANAGESTATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_CAN_NETMANAGESTATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_CAN_NETMANAGESTATE)lval;
	return 0;
}
int MRV_MOTION_SWITCH_toValue( const char *pStr, MRV_MOTION_SWITCH *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_MOTION_SWITCH ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_MOTION_SWITCH)lval;
	return 0;
}
int MRV_MOTION_SWITCH_1_toValue( const char *pStr, MRV_MOTION_SWITCH_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_MOTION_SWITCH_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_MOTION_SWITCH_1)lval;
	return 0;
}
int MRV_MOTION_STATE_1_toValue( const char *pStr, MRV_MOTION_STATE_1 *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_MOTION_STATE_1 ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_MOTION_STATE_1)lval;
	return 0;
}
int MRV_MOTION_STATEREPORT_toValue( const char *pStr, MRV_MOTION_STATEREPORT *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_MOTION_STATEREPORT ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_MOTION_STATEREPORT)lval;
	return 0;
}
int MRV_GLOBAL_GROUP_toValue( const char *pStr, MRV_GLOBAL_GROUP *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_GLOBAL_GROUP ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_GLOBAL_GROUP)lval;
	return 0;
}
int MRV_VALVECTRL_DEVICE_toValue( const char *pStr, MRV_VALVECTRL_DEVICE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_VALVECTRL_DEVICE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_VALVECTRL_DEVICE)lval;
	return 0;
}
int MRV_VALVECTRL_ACTION_toValue( const char *pStr, MRV_VALVECTRL_ACTION *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_VALVECTRL_ACTION ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_VALVECTRL_ACTION)lval;
	return 0;
}
int MRV_THRESHOLD_HIPRESSUREACTION_toValue( const char *pStr, MRV_THRESHOLD_HIPRESSUREACTION *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_THRESHOLD_HIPRESSUREACTION ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_THRESHOLD_HIPRESSUREACTION)lval;
	return 0;
}
int MRV_PVT_EXECMODE_toValue( const char *pStr, MRV_PVT_EXECMODE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_PVT_EXECMODE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_PVT_EXECMODE)lval;
	return 0;
}
int MRV_PVT_HOLD_toValue( const char *pStr, MRV_PVT_HOLD *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_PVT_HOLD ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_PVT_HOLD)lval;
	return 0;
}
int MRV_CALIBRATION_TYPE_toValue( const char *pStr, MRV_CALIBRATION_TYPE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_CALIBRATION_TYPE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_CALIBRATION_TYPE)lval;
	return 0;
}
int MRV_CALIBRATION_SAMPLERATE_toValue( const char *pStr, MRV_CALIBRATION_SAMPLERATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_CALIBRATION_SAMPLERATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_CALIBRATION_SAMPLERATE)lval;
	return 0;
}
int MRV_CALIBRATION_DATAUPCTRL_toValue( const char *pStr, MRV_CALIBRATION_DATAUPCTRL *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_CALIBRATION_DATAUPCTRL ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_CALIBRATION_DATAUPCTRL)lval;
	return 0;
}
int MRV_REPORT_STATE_toValue( const char *pStr, MRV_REPORT_STATE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_REPORT_STATE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_REPORT_STATE)lval;
	return 0;
}
int MRV_IOCONFIG_IOFUNCSEL_toValue( const char *pStr, MRV_IOCONFIG_IOFUNCSEL *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_IOCONFIG_IOFUNCSEL ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_IOCONFIG_IOFUNCSEL)lval;
	return 0;
}
int MRV_IOCONFIG_TRIGEDGE_toValue( const char *pStr, MRV_IOCONFIG_TRIGEDGE *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_IOCONFIG_TRIGEDGE ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_IOCONFIG_TRIGEDGE)lval;
	return 0;
}
int MRV_IOCONFIG_FLOWCTL_toValue( const char *pStr, MRV_IOCONFIG_FLOWCTL *pEVal )
{
	int ret, lval;
	ret = enum_toValue( pStr, desc_table( _value_desc_MRV_IOCONFIG_FLOWCTL ), &lval );
	if ( ret != 0 ) return ret; 
	*pEVal=(MRV_IOCONFIG_FLOWCTL)lval;
	return 0;
}
