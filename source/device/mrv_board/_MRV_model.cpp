#include "_MRV_model.h"
namespace MegaDevice
{
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
_MRV_model::_MRV_model()
{
	loadOtp();
}
void _MRV_model::loadOtp()
{
	mLINK_INTFC = (MRV_LINK_INTFC)0;
	mLINK_DEVICEINFO = (MRV_LINK_DEVICEINFO)0;
	mLINK_DEVICEINFO1 = (MRV_LINK_DEVICEINFO_1)0;
	mLINK_DEVICEINFO2 = (uint32)0;

	mSYSTEM_WORKMODE = (MRV_SYSTEM_WORKMODE)0;
	mSYSTEM_POWERON = (MRV_SYSTEM_POWERON)0;
	mSYSTEM_SN = (byte)0;
	mSYSTEM_SN1 = (char)0;

	mSYSTEM_SN2 = (char)0;
	mSYSTEM_SN3 = (char)0;
	mSYSTEM_SN4 = (char)0;
	mSYSTEM_SN5 = (char)0;

	mSYSTEM_TYPE = (MRV_SYSTEM_TYPE)0;
	mSYSTEM_TYPE1 = (MRV_LINK_DEVICEINFO_1)0;
	mSYSTEM_SOFTVER = (char)0;
	mSYSTEM_SOFTVER1 = (char)0;

	mSYSTEM_SOFTVER2 = (char)0;
	mSYSTEM_HARDVER = (char)0;
	mSYSTEM_HARDVER1 = (char)0;
	mSYSTEM_BOOTVER = (char)0;

	mSYSTEM_BOOTVER1 = (char)0;
	mSYSTEM_ERROR = (char)0;
	mSYSTEM_ERROR1 = (char)0;
	mSYSTEM_ERROR2 = (char)0;

	mSYSTEM_ERROR3 = (char)0;
	mSYSTEM_ERROR4 = (char)0;
	mSYSTEM_ERROR5 = (char)0;
	mRS232_BAUD = (MRV_RS232_BAUD)0;

	mRS232_WORDLEN = (MRV_RS232_WORDLEN)0;
	mRS232_FLOWCTL = (MRV_RS232_FLOWCTL)0;
	mRS232_PARITY = (MRV_RS232_PARITY)0;
	mRS232_STOPBIT = (MRV_RS232_STOPBIT)0;

	mCAN_TYPE = (MRV_CAN_TYPE)0;
	mCAN_BAUD = (MRV_CAN_BAUD)0;
	mCAN_GROUP = (byte)0;
	mCAN_SENDID = (uint32)0;

	mCAN_RECEIVEID = (uint32)0;
	mCAN_GROUPID1 = (uint32)0;
	mCAN_GROUPID2 = (uint32)0;
	mCAN_BROADCASTID = (uint32)0;

	mCAN_NETMANAGESTATE = (MRV_CAN_NETMANAGESTATE)0;
	mCAN_NETMANAGEID = (uint32)0;
	mCAN_NETMANAGEHASH = (uint32)0;
	mCAN_NETMANAGESIGNATURE = (uint32)0;

for ( int i0=0; i0 < 10; i0++ )
{
for ( int i1=0; i1 < 2; i1++ )
{
mMOTION_STATE[i0][i1] = (MRV_MOTION_STATE_1)0;
}
}
for ( int i0=0; i0 < 10; i0++ )
{
mMOTION_STATEREPORT[i0] = (MRV_MOTION_STATEREPORT)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
for ( int i1=0; i1 < 2; i1++ )
{
mGLOBAL_GROUP[i0][i1] = (byte)0;
}
}
for ( int i0=0; i0 < 10; i0++ )
{
mGLOBAL_DISTINGUISH[i0] = (MRV_CAN_NETMANAGELED)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_DEVICE[i0] = (MRV_VALVECTRL_DEVICE)0;
}
	mVALVECTRL_PWMFREQ = (uint32)0;
for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_RTDUTY[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_ACTION[i0] = (MRV_VALVECTRL_ACTION)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_IDLEDUTY[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_HOLDDUTY[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_OPENDUTY[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_OPENDLYTIME[i0] = (uint32)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_OPENTIME[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_RECORDSTATE[i0] = (MRV_CAN_NETMANAGELED)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_ONNUMS[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_OFFNUMS[i0] = (uint32)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mVALVECTRL_TIME[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_HIGHPRESSURE[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_HIPRESSUREACTION[i0] = (MRV_THRESHOLD_HIPRESSUREACTION)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_LOWPRESSURE[i0] = (uint32)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_LOWPRESSUREACTION[i0] = (MRV_THRESHOLD_HIPRESSUREACTION)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_HIGHCURRENT[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_HICURRENTACTION[i0] = (MRV_THRESHOLD_HIPRESSUREACTION)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_LOWCURRENT[i0] = (uint32)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_LOWCURRENTACTION[i0] = (MRV_THRESHOLD_HIPRESSUREACTION)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_ONNUMS[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_NUMSACTION[i0] = (MRV_THRESHOLD_HIPRESSUREACTION)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_TIME[i0] = (uint32)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mTHRESHOLD_TIMEACTION[i0] = (MRV_THRESHOLD_HIPRESSUREACTION)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mPVT_EXECMODE[i0] = (MRV_PVT_EXECMODE)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mPVT_CYCLES[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
for ( int i1=0; i1 < 10; i1++ )
{
mPVT_GRADE[i0][i1] = (uint32)0;
}
}

for ( int i0=0; i0 < 10; i0++ )
{
mPVT_BUFFERCHECK[i0] = (uint32)0;
}
	mPVT_START = (byte)0;
	mPVT_END = (byte)0;
for ( int i0=0; i0 < 10; i0++ )
{
mPVT_PDATA[i0] = (uint32)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mPVT_TDATA[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mPVT_HOLD[i0] = (MRV_PVT_HOLD)0;
}
	mPVT_SAVE = (byte)0;
for ( int i0=0; i0 < 10; i0++ )
{
mCALIBRATION_TYPE[i0] = (MRV_CALIBRATION_TYPE)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mCALIBRATION_SAMPLERATE[i0] = (MRV_CALIBRATION_SAMPLERATE)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mCALIBRATION_OPENTIME[i0] = (uint16)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mCALIBRATION_HOLDTIME[i0] = (uint16)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mCALIBRATION_CLOSETIME[i0] = (uint16)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mCALIBRATION_OPENDUTY[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mCALIBRATION_HOLDDUTY[i0] = (uint32)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mCALIBRATION_DATALENGTH[i0] = (uint16)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
for ( int i1=0; i1 < 4; i1++ )
{
mREPORT_STATE[i0][i1] = (MRV_CAN_NETMANAGELED)0;
}
}

for ( int i0=0; i0 < 10; i0++ )
{
for ( int i1=0; i1 < 4; i1++ )
{
mREPORT_PERIOD[i0][i1] = (uint32)0;
}
}
for ( int i0=0; i0 < 10; i0++ )
{
for ( int i1=0; i1 < 4; i1++ )
{
mREPORT_DATA[i0][i1] = (uint32)0;
}
}
for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_IOFUNCSEL[i0] = (MRV_IOCONFIG_IOFUNCSEL)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_SWFILTERDELAY[i0] = (uint16)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_SWFILTERCOUNTER[i0] = (uint16)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_TRIGEDGE[i0] = (MRV_IOCONFIG_TRIGEDGE)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_BAUD[i0] = (MRV_RS232_BAUD)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_WORDLEN[i0] = (MRV_RS232_WORDLEN)0;
}

for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_FLOWCTL[i0] = (MRV_IOCONFIG_FLOWCTL)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_PARITY[i0] = (MRV_RS232_PARITY)0;
}
for ( int i0=0; i0 < 10; i0++ )
{
mIOCONFIG_STOPBIT[i0] = (MRV_RS232_STOPBIT)0;
}
}
QString _MRV_model::toString( MRV_LINK_INTFC eType )
{
	return QString( MRV_LINK_INTFC_toString( eType ) );
}
QString _MRV_model::toString( MRV_LINK_DEVICEINFO eType )
{
	return QString( MRV_LINK_DEVICEINFO_toString( eType ) );
}
QString _MRV_model::toString( MRV_LINK_DEVICEINFO_1 eType )
{
	return QString( MRV_LINK_DEVICEINFO_1_toString( eType ) );
}
QString _MRV_model::toString( MRV_SYSTEM_WORKMODE eType )
{
	return QString( MRV_SYSTEM_WORKMODE_toString( eType ) );
}
QString _MRV_model::toString( MRV_SYSTEM_POWERON eType )
{
	return QString( MRV_SYSTEM_POWERON_toString( eType ) );
}
QString _MRV_model::toString( MRV_SYSTEM_TYPE eType )
{
	return QString( MRV_SYSTEM_TYPE_toString( eType ) );
}
QString _MRV_model::toString( MRV_RS232_BAUD eType )
{
	return QString( MRV_RS232_BAUD_toString( eType ) );
}
QString _MRV_model::toString( MRV_RS232_WORDLEN eType )
{
	return QString( MRV_RS232_WORDLEN_toString( eType ) );
}
QString _MRV_model::toString( MRV_RS232_FLOWCTL eType )
{
	return QString( MRV_RS232_FLOWCTL_toString( eType ) );
}
QString _MRV_model::toString( MRV_RS232_PARITY eType )
{
	return QString( MRV_RS232_PARITY_toString( eType ) );
}
QString _MRV_model::toString( MRV_RS232_STOPBIT eType )
{
	return QString( MRV_RS232_STOPBIT_toString( eType ) );
}
QString _MRV_model::toString( MRV_CAN_TYPE eType )
{
	return QString( MRV_CAN_TYPE_toString( eType ) );
}
QString _MRV_model::toString( MRV_CAN_BAUD eType )
{
	return QString( MRV_CAN_BAUD_toString( eType ) );
}
QString _MRV_model::toString( MRV_CAN_NETMANAGELED eType )
{
	return QString( MRV_CAN_NETMANAGELED_toString( eType ) );
}
QString _MRV_model::toString( MRV_CAN_NETMANAGESTATE eType )
{
	return QString( MRV_CAN_NETMANAGESTATE_toString( eType ) );
}
QString _MRV_model::toString( MRV_MOTION_SWITCH eType )
{
	return QString( MRV_MOTION_SWITCH_toString( eType ) );
}
QString _MRV_model::toString( MRV_MOTION_SWITCH_1 eType )
{
	return QString( MRV_MOTION_SWITCH_1_toString( eType ) );
}
QString _MRV_model::toString( MRV_MOTION_STATE_1 eType )
{
	return QString( MRV_MOTION_STATE_1_toString( eType ) );
}
QString _MRV_model::toString( MRV_MOTION_STATEREPORT eType )
{
	return QString( MRV_MOTION_STATEREPORT_toString( eType ) );
}
QString _MRV_model::toString( MRV_GLOBAL_GROUP eType )
{
	return QString( MRV_GLOBAL_GROUP_toString( eType ) );
}
QString _MRV_model::toString( MRV_VALVECTRL_DEVICE eType )
{
	return QString( MRV_VALVECTRL_DEVICE_toString( eType ) );
}
QString _MRV_model::toString( MRV_VALVECTRL_ACTION eType )
{
	return QString( MRV_VALVECTRL_ACTION_toString( eType ) );
}
QString _MRV_model::toString( MRV_THRESHOLD_HIPRESSUREACTION eType )
{
	return QString( MRV_THRESHOLD_HIPRESSUREACTION_toString( eType ) );
}
QString _MRV_model::toString( MRV_PVT_EXECMODE eType )
{
	return QString( MRV_PVT_EXECMODE_toString( eType ) );
}
QString _MRV_model::toString( MRV_PVT_HOLD eType )
{
	return QString( MRV_PVT_HOLD_toString( eType ) );
}
QString _MRV_model::toString( MRV_CALIBRATION_TYPE eType )
{
	return QString( MRV_CALIBRATION_TYPE_toString( eType ) );
}
QString _MRV_model::toString( MRV_CALIBRATION_SAMPLERATE eType )
{
	return QString( MRV_CALIBRATION_SAMPLERATE_toString( eType ) );
}
QString _MRV_model::toString( MRV_CALIBRATION_DATAUPCTRL eType )
{
	return QString( MRV_CALIBRATION_DATAUPCTRL_toString( eType ) );
}
QString _MRV_model::toString( MRV_REPORT_STATE eType )
{
	return QString( MRV_REPORT_STATE_toString( eType ) );
}
QString _MRV_model::toString( MRV_IOCONFIG_IOFUNCSEL eType )
{
	return QString( MRV_IOCONFIG_IOFUNCSEL_toString( eType ) );
}
QString _MRV_model::toString( MRV_IOCONFIG_TRIGEDGE eType )
{
	return QString( MRV_IOCONFIG_TRIGEDGE_toString( eType ) );
}
QString _MRV_model::toString( MRV_IOCONFIG_FLOWCTL eType )
{
	return QString( MRV_IOCONFIG_FLOWCTL_toString( eType ) );
}
int _MRV_model::toValue( const QString &str, MRV_LINK_INTFC *pEVal )
{
	return ( MRV_LINK_INTFC_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_LINK_DEVICEINFO *pEVal )
{
	return ( MRV_LINK_DEVICEINFO_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_LINK_DEVICEINFO_1 *pEVal )
{
	return ( MRV_LINK_DEVICEINFO_1_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_SYSTEM_WORKMODE *pEVal )
{
	return ( MRV_SYSTEM_WORKMODE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_SYSTEM_POWERON *pEVal )
{
	return ( MRV_SYSTEM_POWERON_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_SYSTEM_TYPE *pEVal )
{
	return ( MRV_SYSTEM_TYPE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_RS232_BAUD *pEVal )
{
	return ( MRV_RS232_BAUD_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_RS232_WORDLEN *pEVal )
{
	return ( MRV_RS232_WORDLEN_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_RS232_FLOWCTL *pEVal )
{
	return ( MRV_RS232_FLOWCTL_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_RS232_PARITY *pEVal )
{
	return ( MRV_RS232_PARITY_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_RS232_STOPBIT *pEVal )
{
	return ( MRV_RS232_STOPBIT_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_CAN_TYPE *pEVal )
{
	return ( MRV_CAN_TYPE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_CAN_BAUD *pEVal )
{
	return ( MRV_CAN_BAUD_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_CAN_NETMANAGELED *pEVal )
{
	return ( MRV_CAN_NETMANAGELED_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_CAN_NETMANAGESTATE *pEVal )
{
	return ( MRV_CAN_NETMANAGESTATE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_MOTION_SWITCH *pEVal )
{
	return ( MRV_MOTION_SWITCH_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_MOTION_SWITCH_1 *pEVal )
{
	return ( MRV_MOTION_SWITCH_1_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_MOTION_STATE_1 *pEVal )
{
	return ( MRV_MOTION_STATE_1_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_MOTION_STATEREPORT *pEVal )
{
	return ( MRV_MOTION_STATEREPORT_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_GLOBAL_GROUP *pEVal )
{
	return ( MRV_GLOBAL_GROUP_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_VALVECTRL_DEVICE *pEVal )
{
	return ( MRV_VALVECTRL_DEVICE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_VALVECTRL_ACTION *pEVal )
{
	return ( MRV_VALVECTRL_ACTION_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_THRESHOLD_HIPRESSUREACTION *pEVal )
{
	return ( MRV_THRESHOLD_HIPRESSUREACTION_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_PVT_EXECMODE *pEVal )
{
	return ( MRV_PVT_EXECMODE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_PVT_HOLD *pEVal )
{
	return ( MRV_PVT_HOLD_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_CALIBRATION_TYPE *pEVal )
{
	return ( MRV_CALIBRATION_TYPE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_CALIBRATION_SAMPLERATE *pEVal )
{
	return ( MRV_CALIBRATION_SAMPLERATE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_CALIBRATION_DATAUPCTRL *pEVal )
{
	return ( MRV_CALIBRATION_DATAUPCTRL_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_REPORT_STATE *pEVal )
{
	return ( MRV_REPORT_STATE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_IOCONFIG_IOFUNCSEL *pEVal )
{
	return ( MRV_IOCONFIG_IOFUNCSEL_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_IOCONFIG_TRIGEDGE *pEVal )
{
	return ( MRV_IOCONFIG_TRIGEDGE_toValue( str.toLatin1().data(), pEVal) );
}
int _MRV_model::toValue( const QString &str, MRV_IOCONFIG_FLOWCTL *pEVal )
{
	return ( MRV_IOCONFIG_FLOWCTL_toValue( str.toLatin1().data(), pEVal) );
}
//! ENUM
//! NONE|CAN|RS232
int _MRV_model::setLINK_INTFC( MRV_LINK_INTFC val0 )
{
	//! 1 0
	int ret=0; 
	mLINK_INTFC = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getLINK_INTFC(  MRV_LINK_INTFC * val0, bool bQuery )
{
	//! 1 1
	int ret = 0;

	byte lval0 = 0;
	*val0 = mLINK_INTFC;
	return 0;
}
//! 
//! 
int _MRV_model::getLINK_DEVICEINFO(  MRV_LINK_DEVICEINFO * val0, MRV_LINK_DEVICEINFO_1 * val1, uint32 * val2, bool bQuery )
{
	//! 1 2
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	uint32 lval2 = 0;
	*val0 = mLINK_DEVICEINFO;
	*val1 = mLINK_DEVICEINFO1;
	*val2 = mLINK_DEVICEINFO2;
	return 0;
}
//! ENUM
//! NORMAL|NONE
int _MRV_model::setSYSTEM_WORKMODE( MRV_SYSTEM_WORKMODE val0 )
{
	//! 2 0
	int ret=0; 
	mSYSTEM_WORKMODE = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getSYSTEM_WORKMODE(  MRV_SYSTEM_WORKMODE * val0, bool bQuery )
{
	//! 2 1
	int ret = 0;

	byte lval0 = 0;
	*val0 = mSYSTEM_WORKMODE;
	return 0;
}
//! ENUM
//! DEFAULT|LAST
int _MRV_model::setSYSTEM_POWERON( MRV_SYSTEM_POWERON val0 )
{
	//! 2 2
	int ret=0; 
	mSYSTEM_POWERON = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getSYSTEM_POWERON(  MRV_SYSTEM_POWERON * val0, bool bQuery )
{
	//! 2 3
	int ret = 0;

	byte lval0 = 0;
	*val0 = mSYSTEM_POWERON;
	return 0;
}
//! 
//! 
int _MRV_model::getSYSTEM_SN(  byte * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery )
{
	//! 2 4
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
	byte lval4 = 0;
	byte lval5 = 0;
	*val0 = mSYSTEM_SN;
	*val1 = mSYSTEM_SN1;
	*val2 = mSYSTEM_SN2;
	*val3 = mSYSTEM_SN3;
	*val4 = mSYSTEM_SN4;
	*val5 = mSYSTEM_SN5;
	return 0;
}
//! 
//! 
int _MRV_model::getSYSTEM_TYPE(  MRV_SYSTEM_TYPE * val0, MRV_LINK_DEVICEINFO_1 * val1, bool bQuery )
{
	//! 2 5
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	*val0 = mSYSTEM_TYPE;
	*val1 = mSYSTEM_TYPE1;
	return 0;
}
//! 
//! 
int _MRV_model::getSYSTEM_SOFTVER(  char * val0, char * val1, char * val2, bool bQuery )
{
	//! 2 6
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	*val0 = mSYSTEM_SOFTVER;
	*val1 = mSYSTEM_SOFTVER1;
	*val2 = mSYSTEM_SOFTVER2;
	return 0;
}
//! 
//! 
int _MRV_model::getSYSTEM_HARDVER(  char * val0, char * val1, bool bQuery )
{
	//! 2 7
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	*val0 = mSYSTEM_HARDVER;
	*val1 = mSYSTEM_HARDVER1;
	return 0;
}
//! 
//! 
int _MRV_model::getSYSTEM_BOOTVER(  char * val0, char * val1, bool bQuery )
{
	//! 2 8
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	*val0 = mSYSTEM_BOOTVER;
	*val1 = mSYSTEM_BOOTVER1;
	return 0;
}
//! 
//! 
int _MRV_model::getSYSTEM_ERROR(  char * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery )
{
	//! 2 9
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
	byte lval4 = 0;
	byte lval5 = 0;
	*val0 = mSYSTEM_ERROR;
	*val1 = mSYSTEM_ERROR1;
	*val2 = mSYSTEM_ERROR2;
	*val3 = mSYSTEM_ERROR3;
	*val4 = mSYSTEM_ERROR4;
	*val5 = mSYSTEM_ERROR5;
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::setSYSTEM_WARNCLEAR( byte val0 )
{
	//! 2 10
	int ret=0; 

	return ret;
}
//! ENUM
//! 4800|7200|9600|14400|19200|38400|57600|115200|128000
int _MRV_model::setRS232_BAUD( MRV_RS232_BAUD val0 )
{
	//! 3 0
	int ret=0; 
	mRS232_BAUD = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getRS232_BAUD(  MRV_RS232_BAUD * val0, bool bQuery )
{
	//! 3 1
	int ret = 0;

	byte lval0 = 0;
	*val0 = mRS232_BAUD;
	return 0;
}
//! ENUM
//! 8|9
int _MRV_model::setRS232_WORDLEN( MRV_RS232_WORDLEN val0 )
{
	//! 3 2
	int ret=0; 
	mRS232_WORDLEN = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getRS232_WORDLEN(  MRV_RS232_WORDLEN * val0, bool bQuery )
{
	//! 3 3
	int ret = 0;

	byte lval0 = 0;
	*val0 = mRS232_WORDLEN;
	return 0;
}
//! ENUM
//! NONE|RTS|CTS|RTS&CTS
int _MRV_model::setRS232_FLOWCTL( MRV_RS232_FLOWCTL val0 )
{
	//! 3 4
	int ret=0; 
	mRS232_FLOWCTL = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getRS232_FLOWCTL(  MRV_RS232_FLOWCTL * val0, bool bQuery )
{
	//! 3 5
	int ret = 0;

	byte lval0 = 0;
	*val0 = mRS232_FLOWCTL;
	return 0;
}
//! ENUM
//! NONE|EVEN|ODD
int _MRV_model::setRS232_PARITY( MRV_RS232_PARITY val0 )
{
	//! 3 6
	int ret=0; 
	mRS232_PARITY = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getRS232_PARITY(  MRV_RS232_PARITY * val0, bool bQuery )
{
	//! 3 7
	int ret = 0;

	byte lval0 = 0;
	*val0 = mRS232_PARITY;
	return 0;
}
//! ENUM
//! 1|0.5|2|1.5
int _MRV_model::setRS232_STOPBIT( MRV_RS232_STOPBIT val0 )
{
	//! 3 8
	int ret=0; 
	mRS232_STOPBIT = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getRS232_STOPBIT(  MRV_RS232_STOPBIT * val0, bool bQuery )
{
	//! 3 9
	int ret = 0;

	byte lval0 = 0;
	*val0 = mRS232_STOPBIT;
	return 0;
}
//! 
//! 
int _MRV_model::setRS232_APPLYPARA(  )
{
	//! 3 10
	int ret=0; 

	return ret;
}
//! ENUM
//! A|B
int _MRV_model::setCAN_TYPE( MRV_CAN_TYPE val0 )
{
	//! 4 0
	int ret=0; 
	mCAN_TYPE = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_TYPE(  MRV_CAN_TYPE * val0, bool bQuery )
{
	//! 4 1
	int ret = 0;

	byte lval0 = 0;
	*val0 = mCAN_TYPE;
	return 0;
}
//! ENUM
//! 1000|500|250|125|100|50|20|10
int _MRV_model::setCAN_BAUD( MRV_CAN_BAUD val0 )
{
	//! 4 2
	int ret=0; 
	mCAN_BAUD = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_BAUD(  MRV_CAN_BAUD * val0, bool bQuery )
{
	//! 4 3
	int ret = 0;

	byte lval0 = 0;
	*val0 = mCAN_BAUD;
	return 0;
}
//! U8
//! 
int _MRV_model::setCAN_GROUP( byte val0 )
{
	//! 4 4
	int ret=0; 
	mCAN_GROUP = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_GROUP(  byte * val0, bool bQuery )
{
	//! 4 5
	int ret = 0;

	byte lval0 = 0;
	*val0 = mCAN_GROUP;
	return 0;
}
//! U32
//! 
int _MRV_model::setCAN_SENDID( uint32 val0 )
{
	//! 4 6
	int ret=0; 
	mCAN_SENDID = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_SENDID(  uint32 * val0, bool bQuery )
{
	//! 4 7
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mCAN_SENDID;
	return 0;
}
//! U32
//! 
int _MRV_model::setCAN_RECEIVEID( uint32 val0 )
{
	//! 4 8
	int ret=0; 
	mCAN_RECEIVEID = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_RECEIVEID(  uint32 * val0, bool bQuery )
{
	//! 4 9
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mCAN_RECEIVEID;
	return 0;
}
//! U32
//! 
int _MRV_model::setCAN_GROUPID1( uint32 val0 )
{
	//! 4 10
	int ret=0; 
	mCAN_GROUPID1 = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_GROUPID1(  uint32 * val0, bool bQuery )
{
	//! 4 11
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mCAN_GROUPID1;
	return 0;
}
//! U32
//! 
int _MRV_model::setCAN_GROUPID2( uint32 val0 )
{
	//! 4 12
	int ret=0; 
	mCAN_GROUPID2 = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_GROUPID2(  uint32 * val0, bool bQuery )
{
	//! 4 13
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mCAN_GROUPID2;
	return 0;
}
//! U32
//! 
int _MRV_model::setCAN_BROADCASTID( uint32 val0 )
{
	//! 4 14
	int ret=0; 
	mCAN_BROADCASTID = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_BROADCASTID(  uint32 * val0, bool bQuery )
{
	//! 4 15
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mCAN_BROADCASTID;
	return 0;
}
//! 
//! 
int _MRV_model::setCAN_APPLYPARA(  )
{
	//! 4 16
	int ret=0; 

	return ret;
}
//! ENUM
//! OFF|ON
int _MRV_model::setCAN_NETMANAGELED( MRV_CAN_NETMANAGELED val0 )
{
	//! 4 17
	int ret=0; 

	return ret;
}
//! ENUM
//! IDLE|HASH|SIGNATURE
int _MRV_model::setCAN_NETMANAGESTATE( MRV_CAN_NETMANAGESTATE val0 )
{
	//! 4 18
	int ret=0; 
	mCAN_NETMANAGESTATE = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_NETMANAGESTATE(  MRV_CAN_NETMANAGESTATE * val0, bool bQuery )
{
	//! 4 19
	int ret = 0;

	byte lval0 = 0;
	*val0 = mCAN_NETMANAGESTATE;
	return 0;
}
//! U32
//! 
int _MRV_model::setCAN_NETMANAGEID( uint32 val0 )
{
	//! 4 20
	int ret=0; 
	mCAN_NETMANAGEID = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_NETMANAGEID(  uint32 * val0, bool bQuery )
{
	//! 4 21
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mCAN_NETMANAGEID;
	return 0;
}
//! U32
//! 
int _MRV_model::setCAN_NETMANAGEHASH( uint32 val0 )
{
	//! 4 22
	int ret=0; 
	mCAN_NETMANAGEHASH = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_NETMANAGEHASH(  uint32 * val0, bool bQuery )
{
	//! 4 23
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mCAN_NETMANAGEHASH;
	return 0;
}
//! U32
//! 
int _MRV_model::setCAN_NETMANAGESIGNATURE( uint32 val0 )
{
	//! 4 24
	int ret=0; 
	mCAN_NETMANAGESIGNATURE = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getCAN_NETMANAGESIGNATURE(  uint32 * val0, bool bQuery )
{
	//! 4 25
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mCAN_NETMANAGESIGNATURE;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],RESET|STOP|RUN,MAIN|RESSERVE1
int _MRV_model::setMOTION_SWITCH( byte val0
,MRV_MOTION_SWITCH val1
,MRV_MOTION_SWITCH_1 val2 )
{
	//! 21 0
	int ret=0; 

	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|RESSERVE1
int _MRV_model::getMOTION_STATE( byte val0
,MRV_MOTION_SWITCH_1 val1, MRV_MOTION_STATE_1 * val2, bool bQuery )
{
	//! 21 1
	int ret = 0;

	byte lval0 = 0;
	*val2 = mMOTION_STATE[ (int)val0 ][ (int)val1 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],ACTIVE|QUERY
int _MRV_model::setMOTION_STATEREPORT( byte val0
,MRV_MOTION_STATEREPORT val1 )
{
	//! 21 2
	int ret=0; 
	mMOTION_STATEREPORT[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getMOTION_STATEREPORT( byte val0, MRV_MOTION_STATEREPORT * val1, bool bQuery )
{
	//! 21 3
	int ret = 0;

	byte lval0 = 0;
	*val1 = mMOTION_STATEREPORT[ (int)val0 ];
	return 0;
}
//! U8,ENUM,U8
//! [CHANNUM],GROUP1|GROUP2
int _MRV_model::setGLOBAL_GROUP( byte val0
,MRV_GLOBAL_GROUP val1
,byte val2 )
{
	//! 22 0
	int ret=0; 
	mGLOBAL_GROUP[ (int)val0 ][ (int)val1 ] = val2;

	return ret;
}
//! U8,ENUM
//! [CHANNUM],GROUP1|GROUP2
int _MRV_model::getGLOBAL_GROUP( byte val0
,MRV_GLOBAL_GROUP val1, byte * val2, bool bQuery )
{
	//! 22 1
	int ret = 0;

	byte lval0 = 0;
	*val2 = mGLOBAL_GROUP[ (int)val0 ][ (int)val1 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int _MRV_model::setGLOBAL_DISTINGUISH( byte val0
,MRV_CAN_NETMANAGELED val1 )
{
	//! 22 2
	int ret=0; 
	mGLOBAL_DISTINGUISH[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! 
int _MRV_model::getGLOBAL_DISTINGUISH( byte val0, MRV_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 22 3
	int ret = 0;

	byte lval0 = 0;
	*val1 = mGLOBAL_DISTINGUISH[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],VALVE|MOTOR
int _MRV_model::setVALVECTRL_DEVICE( byte val0
,MRV_VALVECTRL_DEVICE val1 )
{
	//! 51 0
	int ret=0; 
	mVALVECTRL_DEVICE[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_DEVICE( byte val0, MRV_VALVECTRL_DEVICE * val1, bool bQuery )
{
	//! 51 1
	int ret = 0;

	byte lval0 = 0;
	*val1 = mVALVECTRL_DEVICE[ (int)val0 ];
	return 0;
}
//! U32
//! 
int _MRV_model::setVALVECTRL_PWMFREQ( uint32 val0 )
{
	//! 51 2
	int ret=0; 
	mVALVECTRL_PWMFREQ = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getVALVECTRL_PWMFREQ(  uint32 * val0, bool bQuery )
{
	//! 51 3
	int ret = 0;

	uint32 lval0 = 0;
	*val0 = mVALVECTRL_PWMFREQ;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setVALVECTRL_RTDUTY( byte val0
,uint32 val1 )
{
	//! 51 4
	int ret=0; 
	mVALVECTRL_RTDUTY[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_RTDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 5
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_RTDUTY[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON|IDLE
int _MRV_model::setVALVECTRL_ACTION( byte val0
,MRV_VALVECTRL_ACTION val1 )
{
	//! 51 6
	int ret=0; 
	mVALVECTRL_ACTION[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_ACTION( byte val0, MRV_VALVECTRL_ACTION * val1, bool bQuery )
{
	//! 51 7
	int ret = 0;

	byte lval0 = 0;
	*val1 = mVALVECTRL_ACTION[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setVALVECTRL_IDLEDUTY( byte val0
,uint32 val1 )
{
	//! 51 8
	int ret=0; 
	mVALVECTRL_IDLEDUTY[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_IDLEDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 9
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_IDLEDUTY[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setVALVECTRL_HOLDDUTY( byte val0
,uint32 val1 )
{
	//! 51 10
	int ret=0; 
	mVALVECTRL_HOLDDUTY[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_HOLDDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 11
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_HOLDDUTY[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setVALVECTRL_OPENDUTY( byte val0
,uint32 val1 )
{
	//! 51 12
	int ret=0; 
	mVALVECTRL_OPENDUTY[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_OPENDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 13
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_OPENDUTY[ (int)val0 ];
	return 0;
}
//! U8,U16
//! [CHANNUM]
int _MRV_model::setVALVECTRL_OPENDLYTIME( byte val0
,uint16 val1 )
{
	//! 51 14
	int ret=0; 
	mVALVECTRL_OPENDLYTIME[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_OPENDLYTIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 15
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_OPENDLYTIME[ (int)val0 ];
	return 0;
}
//! U8,U16
//! [CHANNUM]
int _MRV_model::setVALVECTRL_OPENTIME( byte val0
,uint16 val1 )
{
	//! 51 16
	int ret=0; 
	mVALVECTRL_OPENTIME[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_OPENTIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 17
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_OPENTIME[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int _MRV_model::setVALVECTRL_RECORDSTATE( byte val0
,MRV_CAN_NETMANAGELED val1 )
{
	//! 51 18
	int ret=0; 
	mVALVECTRL_RECORDSTATE[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_RECORDSTATE( byte val0, MRV_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 51 19
	int ret = 0;

	byte lval0 = 0;
	*val1 = mVALVECTRL_RECORDSTATE[ (int)val0 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_ONNUMS( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 20
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_ONNUMS[ (int)val0 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::setVALVECTRL_ONNUMSCLEAR( byte val0 )
{
	//! 51 21
	int ret=0; 

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_OFFNUMS( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 22
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_OFFNUMS[ (int)val0 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::setVALVECTRL_OFFNUMSCLEAR( byte val0 )
{
	//! 51 23
	int ret=0; 

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getVALVECTRL_TIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 24
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mVALVECTRL_TIME[ (int)val0 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::setVALVECTRL_TIMECLEAR( byte val0 )
{
	//! 51 25
	int ret=0; 

	return ret;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setTHRESHOLD_HIGHPRESSURE( byte val0
,uint32 val1 )
{
	//! 52 0
	int ret=0; 
	mTHRESHOLD_HIGHPRESSURE[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_HIGHPRESSURE( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 1
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mTHRESHOLD_HIGHPRESSURE[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int _MRV_model::setTHRESHOLD_HIPRESSUREACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 2
	int ret=0; 
	mTHRESHOLD_HIPRESSUREACTION[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_HIPRESSUREACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 3
	int ret = 0;

	byte lval0 = 0;
	*val1 = mTHRESHOLD_HIPRESSUREACTION[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setTHRESHOLD_LOWPRESSURE( byte val0
,uint32 val1 )
{
	//! 52 4
	int ret=0; 
	mTHRESHOLD_LOWPRESSURE[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_LOWPRESSURE( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 5
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mTHRESHOLD_LOWPRESSURE[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int _MRV_model::setTHRESHOLD_LOWPRESSUREACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 6
	int ret=0; 
	mTHRESHOLD_LOWPRESSUREACTION[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_LOWPRESSUREACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 7
	int ret = 0;

	byte lval0 = 0;
	*val1 = mTHRESHOLD_LOWPRESSUREACTION[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setTHRESHOLD_HIGHCURRENT( byte val0
,uint32 val1 )
{
	//! 52 8
	int ret=0; 
	mTHRESHOLD_HIGHCURRENT[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_HIGHCURRENT( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 9
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mTHRESHOLD_HIGHCURRENT[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int _MRV_model::setTHRESHOLD_HICURRENTACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 10
	int ret=0; 
	mTHRESHOLD_HICURRENTACTION[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_HICURRENTACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 11
	int ret = 0;

	byte lval0 = 0;
	*val1 = mTHRESHOLD_HICURRENTACTION[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setTHRESHOLD_LOWCURRENT( byte val0
,uint32 val1 )
{
	//! 52 12
	int ret=0; 
	mTHRESHOLD_LOWCURRENT[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_LOWCURRENT( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 13
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mTHRESHOLD_LOWCURRENT[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int _MRV_model::setTHRESHOLD_LOWCURRENTACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 14
	int ret=0; 
	mTHRESHOLD_LOWCURRENTACTION[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_LOWCURRENTACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 15
	int ret = 0;

	byte lval0 = 0;
	*val1 = mTHRESHOLD_LOWCURRENTACTION[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setTHRESHOLD_ONNUMS( byte val0
,uint32 val1 )
{
	//! 52 16
	int ret=0; 
	mTHRESHOLD_ONNUMS[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_ONNUMS( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 17
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mTHRESHOLD_ONNUMS[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int _MRV_model::setTHRESHOLD_NUMSACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 18
	int ret=0; 
	mTHRESHOLD_NUMSACTION[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_NUMSACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 19
	int ret = 0;

	byte lval0 = 0;
	*val1 = mTHRESHOLD_NUMSACTION[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setTHRESHOLD_TIME( byte val0
,uint32 val1 )
{
	//! 52 20
	int ret=0; 
	mTHRESHOLD_TIME[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_TIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 21
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mTHRESHOLD_TIME[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int _MRV_model::setTHRESHOLD_TIMEACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 22
	int ret=0; 
	mTHRESHOLD_TIMEACTION[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getTHRESHOLD_TIMEACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 23
	int ret = 0;

	byte lval0 = 0;
	*val1 = mTHRESHOLD_TIMEACTION[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],RT|NCYCLE|FIFO
int _MRV_model::setPVT_EXECMODE( byte val0
,MRV_PVT_EXECMODE val1 )
{
	//! 53 0
	int ret=0; 
	mPVT_EXECMODE[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getPVT_EXECMODE( byte val0, MRV_PVT_EXECMODE * val1, bool bQuery )
{
	//! 53 1
	int ret = 0;

	byte lval0 = 0;
	*val1 = mPVT_EXECMODE[ (int)val0 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::setPVT_RESET( byte val0 )
{
	//! 53 2
	int ret=0; 

	return ret;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setPVT_CYCLES( byte val0
,uint32 val1 )
{
	//! 53 3
	int ret=0; 
	mPVT_CYCLES[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getPVT_CYCLES( byte val0, uint32 * val1, bool bQuery )
{
	//! 53 4
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mPVT_CYCLES[ (int)val0 ];
	return 0;
}
//! U8,U8,U32
//! [CHANNUM]
int _MRV_model::setPVT_GRADE( byte val0
,byte val1
,uint32 val2 )
{
	//! 53 5
	int ret=0; 
	mPVT_GRADE[ (int)val0 ][ (int)val1 ] = val2;

	return ret;
}
//! U8,U8
//! [CHANNUM]
int _MRV_model::getPVT_GRADE( byte val0
,byte val1, uint32 * val2, bool bQuery )
{
	//! 53 6
	int ret = 0;

	uint32 lval0 = 0;
	*val2 = mPVT_GRADE[ (int)val0 ][ (int)val1 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::getPVT_BUFFERCHECK( byte val0, uint32 * val1, bool bQuery )
{
	//! 53 7
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mPVT_BUFFERCHECK[ (int)val0 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::setPVT_START( byte val0 )
{
	//! 53 8
	int ret=0; 
	mPVT_START = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getPVT_START(  byte * val0, bool bQuery )
{
	//! 53 9
	int ret = 0;

	byte lval0 = 0;
	*val0 = mPVT_START;
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::setPVT_END( byte val0 )
{
	//! 53 10
	int ret=0; 
	mPVT_END = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getPVT_END(  byte * val0, bool bQuery )
{
	//! 53 11
	int ret = 0;

	byte lval0 = 0;
	*val0 = mPVT_END;
	return 0;
}
//! U16,U32
//! 
int _MRV_model::setPVT_PDATA( uint16 val0
,uint32 val1 )
{
	//! 53 12
	int ret=0; 
	mPVT_PDATA[ (int)val0 ] = val1;

	return ret;
}
//! U16
//! 
int _MRV_model::getPVT_PDATA( uint16 val0, uint32 * val1, bool bQuery )
{
	//! 53 13
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mPVT_PDATA[ (int)val0 ];
	return 0;
}
//! U16,U32
//! 
int _MRV_model::setPVT_TDATA( uint16 val0
,uint32 val1 )
{
	//! 53 14
	int ret=0; 
	mPVT_TDATA[ (int)val0 ] = val1;

	return ret;
}
//! U16
//! 
int _MRV_model::getPVT_TDATA( uint16 val0, uint32 * val1, bool bQuery )
{
	//! 53 15
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mPVT_TDATA[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],TRUE|FALSE
int _MRV_model::setPVT_HOLD( byte val0
,MRV_PVT_HOLD val1 )
{
	//! 53 16
	int ret=0; 
	mPVT_HOLD[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! 
int _MRV_model::getPVT_HOLD( byte val0, MRV_PVT_HOLD * val1, bool bQuery )
{
	//! 53 17
	int ret = 0;

	byte lval0 = 0;
	*val1 = mPVT_HOLD[ (int)val0 ];
	return 0;
}
//! U8
//! 
int _MRV_model::setPVT_SAVE( byte val0 )
{
	//! 53 18
	int ret=0; 
	mPVT_SAVE = val0;

	return ret;
}
//! 
//! 
int _MRV_model::getPVT_SAVE(  byte * val0, bool bQuery )
{
	//! 53 19
	int ret = 0;

	byte lval0 = 0;
	*val0 = mPVT_SAVE;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|OPEN|CLOSE
int _MRV_model::setCALIBRATION_TYPE( byte val0
,MRV_CALIBRATION_TYPE val1 )
{
	//! 54 0
	int ret=0; 
	mCALIBRATION_TYPE[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getCALIBRATION_TYPE( byte val0, MRV_CALIBRATION_TYPE * val1, bool bQuery )
{
	//! 54 1
	int ret = 0;

	byte lval0 = 0;
	*val1 = mCALIBRATION_TYPE[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],222KHz|176KHz|142KHz|47KHz
int _MRV_model::setCALIBRATION_SAMPLERATE( byte val0
,MRV_CALIBRATION_SAMPLERATE val1 )
{
	//! 54 2
	int ret=0; 
	mCALIBRATION_SAMPLERATE[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getCALIBRATION_SAMPLERATE( byte val0, MRV_CALIBRATION_SAMPLERATE * val1, bool bQuery )
{
	//! 54 3
	int ret = 0;

	byte lval0 = 0;
	*val1 = mCALIBRATION_SAMPLERATE[ (int)val0 ];
	return 0;
}
//! U8,U16
//! [CHANNUM]
int _MRV_model::setCALIBRATION_OPENTIME( byte val0
,uint16 val1 )
{
	//! 54 4
	int ret=0; 
	mCALIBRATION_OPENTIME[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getCALIBRATION_OPENTIME( byte val0, uint16 * val1, bool bQuery )
{
	//! 54 5
	int ret = 0;

	uint16 lval0 = 0;
	*val1 = mCALIBRATION_OPENTIME[ (int)val0 ];
	return 0;
}
//! U8,U16
//! [CHANNUM]
int _MRV_model::setCALIBRATION_HOLDTIME( byte val0
,uint16 val1 )
{
	//! 54 6
	int ret=0; 
	mCALIBRATION_HOLDTIME[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getCALIBRATION_HOLDTIME( byte val0, uint16 * val1, bool bQuery )
{
	//! 54 7
	int ret = 0;

	uint16 lval0 = 0;
	*val1 = mCALIBRATION_HOLDTIME[ (int)val0 ];
	return 0;
}
//! U8,U16
//! [CHANNUM]
int _MRV_model::setCALIBRATION_CLOSETIME( byte val0
,uint16 val1 )
{
	//! 54 8
	int ret=0; 
	mCALIBRATION_CLOSETIME[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getCALIBRATION_CLOSETIME( byte val0, uint16 * val1, bool bQuery )
{
	//! 54 9
	int ret = 0;

	uint16 lval0 = 0;
	*val1 = mCALIBRATION_CLOSETIME[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setCALIBRATION_OPENDUTY( byte val0
,uint32 val1 )
{
	//! 54 10
	int ret=0; 
	mCALIBRATION_OPENDUTY[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getCALIBRATION_OPENDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 54 11
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mCALIBRATION_OPENDUTY[ (int)val0 ];
	return 0;
}
//! U8,U32
//! [CHANNUM]
int _MRV_model::setCALIBRATION_HOLDDUTY( byte val0
,uint32 val1 )
{
	//! 54 12
	int ret=0; 
	mCALIBRATION_HOLDDUTY[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getCALIBRATION_HOLDDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 54 13
	int ret = 0;

	uint32 lval0 = 0;
	*val1 = mCALIBRATION_HOLDDUTY[ (int)val0 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::getCALIBRATION_DATALENGTH( byte val0, uint16 * val1, bool bQuery )
{
	//! 54 14
	int ret = 0;

	uint16 lval0 = 0;
	*val1 = mCALIBRATION_DATALENGTH[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],START|STOP
int _MRV_model::setCALIBRATION_DATAUPCTRL( byte val0
,MRV_CALIBRATION_DATAUPCTRL val1 )
{
	//! 54 15
	int ret=0; 

	return ret;
}
//! U16,U16,U16
//! 
int _MRV_model::setCALIBRATION_DATAUP( uint16 val0
,uint16 val1
,uint16 val2 )
{
	//! 54 16
	int ret=0; 

	return ret;
}
//! U8,ENUM,ENUM
//! [CHANNUM],Current|Pressure|Position|Cycle,OFF|ON
int _MRV_model::setREPORT_STATE( byte val0
,MRV_REPORT_STATE val1
,MRV_CAN_NETMANAGELED val2 )
{
	//! 55 0
	int ret=0; 
	mREPORT_STATE[ (int)val0 ][ (int)val1 ] = val2;

	return ret;
}
//! U8,ENUM
//! [CHANNUM],Current|Pressure|Position|Cycle
int _MRV_model::getREPORT_STATE( byte val0
,MRV_REPORT_STATE val1, MRV_CAN_NETMANAGELED * val2, bool bQuery )
{
	//! 55 1
	int ret = 0;

	byte lval0 = 0;
	*val2 = mREPORT_STATE[ (int)val0 ][ (int)val1 ];
	return 0;
}
//! U8,ENUM,U32
//! [CHANNUM],Current|Pressure|Position|Cycle
int _MRV_model::setREPORT_PERIOD( byte val0
,MRV_REPORT_STATE val1
,uint32 val2 )
{
	//! 55 2
	int ret=0; 
	mREPORT_PERIOD[ (int)val0 ][ (int)val1 ] = val2;

	return ret;
}
//! U8,ENUM
//! [CHANNUM],Current|Pressure|Position|Cycle
int _MRV_model::getREPORT_PERIOD( byte val0
,MRV_REPORT_STATE val1, uint32 * val2, bool bQuery )
{
	//! 55 3
	int ret = 0;

	uint32 lval0 = 0;
	*val2 = mREPORT_PERIOD[ (int)val0 ][ (int)val1 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],Current|Pressure|Position|Cycle
int _MRV_model::getREPORT_DATA( byte val0
,MRV_REPORT_STATE val1, uint32 * val2, bool bQuery )
{
	//! 55 4
	int ret = 0;

	uint32 lval0 = 0;
	*val2 = mREPORT_DATA[ (int)val0 ][ (int)val1 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],UART|SWITCH
int _MRV_model::setIOCONFIG_IOFUNCSEL( byte val0
,MRV_IOCONFIG_IOFUNCSEL val1 )
{
	//! 56 0
	int ret=0; 
	mIOCONFIG_IOFUNCSEL[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_IOFUNCSEL( byte val0, MRV_IOCONFIG_IOFUNCSEL * val1, bool bQuery )
{
	//! 56 1
	int ret = 0;

	byte lval0 = 0;
	*val1 = mIOCONFIG_IOFUNCSEL[ (int)val0 ];
	return 0;
}
//! U8,U16
//! [CHANNUM]
int _MRV_model::setIOCONFIG_SWFILTERDELAY( byte val0
,uint16 val1 )
{
	//! 56 2
	int ret=0; 
	mIOCONFIG_SWFILTERDELAY[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_SWFILTERDELAY( byte val0, uint16 * val1, bool bQuery )
{
	//! 56 3
	int ret = 0;

	uint16 lval0 = 0;
	*val1 = mIOCONFIG_SWFILTERDELAY[ (int)val0 ];
	return 0;
}
//! U8,U16
//! [CHANNUM]
int _MRV_model::setIOCONFIG_SWFILTERCOUNTER( byte val0
,uint16 val1 )
{
	//! 56 4
	int ret=0; 
	mIOCONFIG_SWFILTERCOUNTER[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_SWFILTERCOUNTER( byte val0, uint16 * val1, bool bQuery )
{
	//! 56 5
	int ret = 0;

	uint16 lval0 = 0;
	*val1 = mIOCONFIG_SWFILTERCOUNTER[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|RISE|FALL|BOTH
int _MRV_model::setIOCONFIG_TRIGEDGE( byte val0
,MRV_IOCONFIG_TRIGEDGE val1 )
{
	//! 56 6
	int ret=0; 
	mIOCONFIG_TRIGEDGE[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_TRIGEDGE( byte val0, MRV_IOCONFIG_TRIGEDGE * val1, bool bQuery )
{
	//! 56 7
	int ret = 0;

	byte lval0 = 0;
	*val1 = mIOCONFIG_TRIGEDGE[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],4800|7200|9600|14400|19200|38400|57600|115200|128000
int _MRV_model::setIOCONFIG_BAUD( byte val0
,MRV_RS232_BAUD val1 )
{
	//! 56 8
	int ret=0; 
	mIOCONFIG_BAUD[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_BAUD( byte val0, MRV_RS232_BAUD * val1, bool bQuery )
{
	//! 56 9
	int ret = 0;

	byte lval0 = 0;
	*val1 = mIOCONFIG_BAUD[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],8|9
int _MRV_model::setIOCONFIG_WORDLEN( byte val0
,MRV_RS232_WORDLEN val1 )
{
	//! 56 10
	int ret=0; 
	mIOCONFIG_WORDLEN[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_WORDLEN( byte val0, MRV_RS232_WORDLEN * val1, bool bQuery )
{
	//! 56 11
	int ret = 0;

	byte lval0 = 0;
	*val1 = mIOCONFIG_WORDLEN[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|RTS|CTS|RTS_CTS
int _MRV_model::setIOCONFIG_FLOWCTL( byte val0
,MRV_IOCONFIG_FLOWCTL val1 )
{
	//! 56 12
	int ret=0; 
	mIOCONFIG_FLOWCTL[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_FLOWCTL( byte val0, MRV_IOCONFIG_FLOWCTL * val1, bool bQuery )
{
	//! 56 13
	int ret = 0;

	byte lval0 = 0;
	*val1 = mIOCONFIG_FLOWCTL[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|EVEN|ODD
int _MRV_model::setIOCONFIG_PARITY( byte val0
,MRV_RS232_PARITY val1 )
{
	//! 56 14
	int ret=0; 
	mIOCONFIG_PARITY[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_PARITY( byte val0, MRV_RS232_PARITY * val1, bool bQuery )
{
	//! 56 15
	int ret = 0;

	byte lval0 = 0;
	*val1 = mIOCONFIG_PARITY[ (int)val0 ];
	return 0;
}
//! U8,ENUM
//! [CHANNUM],1|0.5|2|1.5
int _MRV_model::setIOCONFIG_STOPBIT( byte val0
,MRV_RS232_STOPBIT val1 )
{
	//! 56 16
	int ret=0; 
	mIOCONFIG_STOPBIT[ (int)val0 ] = val1;

	return ret;
}
//! U8
//! [CHANNUM]
int _MRV_model::getIOCONFIG_STOPBIT( byte val0, MRV_RS232_STOPBIT * val1, bool bQuery )
{
	//! 56 17
	int ret = 0;

	byte lval0 = 0;
	*val1 = mIOCONFIG_STOPBIT[ (int)val0 ];
	return 0;
}
//! U8
//! [CHANNUM]
int _MRV_model::setIOCONFIG_APPLYPARA( byte val0 )
{
	//! 56 18
	int ret=0; 

	return ret;
}
}
