#ifndef __MRV_MODEL_H_
#define __MRV_MODEL_H_
#include "../../../include/mcstd.h"

#include "../devicemodel.h"
#include "_MRV_enum.h"
namespace MegaDevice
{
class _MRV_model : public DeviceModel
{
public:
QString toString( MRV_LINK_INTFC eType );
QString toString( MRV_LINK_DEVICEINFO eType );
QString toString( MRV_LINK_DEVICEINFO_1 eType );
QString toString( MRV_SYSTEM_WORKMODE eType );
QString toString( MRV_SYSTEM_POWERON eType );
QString toString( MRV_SYSTEM_TYPE eType );
QString toString( MRV_RS232_BAUD eType );
QString toString( MRV_RS232_WORDLEN eType );
QString toString( MRV_RS232_FLOWCTL eType );
QString toString( MRV_RS232_PARITY eType );
QString toString( MRV_RS232_STOPBIT eType );
QString toString( MRV_CAN_TYPE eType );
QString toString( MRV_CAN_BAUD eType );
QString toString( MRV_CAN_NETMANAGELED eType );
QString toString( MRV_CAN_NETMANAGESTATE eType );
QString toString( MRV_MOTION_SWITCH eType );
QString toString( MRV_MOTION_SWITCH_1 eType );
QString toString( MRV_MOTION_STATE_1 eType );
QString toString( MRV_MOTION_STATEREPORT eType );
QString toString( MRV_GLOBAL_GROUP eType );
QString toString( MRV_VALVECTRL_DEVICE eType );
QString toString( MRV_VALVECTRL_ACTION eType );
QString toString( MRV_THRESHOLD_HIPRESSUREACTION eType );
QString toString( MRV_PVT_EXECMODE eType );
QString toString( MRV_PVT_HOLD eType );
QString toString( MRV_CALIBRATION_TYPE eType );
QString toString( MRV_CALIBRATION_SAMPLERATE eType );
QString toString( MRV_CALIBRATION_DATAUPCTRL eType );
QString toString( MRV_REPORT_STATE eType );
QString toString( MRV_IOCONFIG_IOFUNCSEL eType );
QString toString( MRV_IOCONFIG_TRIGEDGE eType );
QString toString( MRV_IOCONFIG_FLOWCTL eType );
int toValue( const QString &str, MRV_LINK_INTFC *pEVal );
int toValue( const QString &str, MRV_LINK_DEVICEINFO *pEVal );
int toValue( const QString &str, MRV_LINK_DEVICEINFO_1 *pEVal );
int toValue( const QString &str, MRV_SYSTEM_WORKMODE *pEVal );
int toValue( const QString &str, MRV_SYSTEM_POWERON *pEVal );
int toValue( const QString &str, MRV_SYSTEM_TYPE *pEVal );
int toValue( const QString &str, MRV_RS232_BAUD *pEVal );
int toValue( const QString &str, MRV_RS232_WORDLEN *pEVal );
int toValue( const QString &str, MRV_RS232_FLOWCTL *pEVal );
int toValue( const QString &str, MRV_RS232_PARITY *pEVal );
int toValue( const QString &str, MRV_RS232_STOPBIT *pEVal );
int toValue( const QString &str, MRV_CAN_TYPE *pEVal );
int toValue( const QString &str, MRV_CAN_BAUD *pEVal );
int toValue( const QString &str, MRV_CAN_NETMANAGELED *pEVal );
int toValue( const QString &str, MRV_CAN_NETMANAGESTATE *pEVal );
int toValue( const QString &str, MRV_MOTION_SWITCH *pEVal );
int toValue( const QString &str, MRV_MOTION_SWITCH_1 *pEVal );
int toValue( const QString &str, MRV_MOTION_STATE_1 *pEVal );
int toValue( const QString &str, MRV_MOTION_STATEREPORT *pEVal );
int toValue( const QString &str, MRV_GLOBAL_GROUP *pEVal );
int toValue( const QString &str, MRV_VALVECTRL_DEVICE *pEVal );
int toValue( const QString &str, MRV_VALVECTRL_ACTION *pEVal );
int toValue( const QString &str, MRV_THRESHOLD_HIPRESSUREACTION *pEVal );
int toValue( const QString &str, MRV_PVT_EXECMODE *pEVal );
int toValue( const QString &str, MRV_PVT_HOLD *pEVal );
int toValue( const QString &str, MRV_CALIBRATION_TYPE *pEVal );
int toValue( const QString &str, MRV_CALIBRATION_SAMPLERATE *pEVal );
int toValue( const QString &str, MRV_CALIBRATION_DATAUPCTRL *pEVal );
int toValue( const QString &str, MRV_REPORT_STATE *pEVal );
int toValue( const QString &str, MRV_IOCONFIG_IOFUNCSEL *pEVal );
int toValue( const QString &str, MRV_IOCONFIG_TRIGEDGE *pEVal );
int toValue( const QString &str, MRV_IOCONFIG_FLOWCTL *pEVal );
_MRV_model();
int setLINK_INTFC( MRV_LINK_INTFC val0 );
int getLINK_INTFC(  MRV_LINK_INTFC * val0, bool bQuery=true );
int getLINK_DEVICEINFO(  MRV_LINK_DEVICEINFO * val0, MRV_LINK_DEVICEINFO_1 * val1, uint32 * val2, bool bQuery=true );
int setSYSTEM_WORKMODE( MRV_SYSTEM_WORKMODE val0 );
int getSYSTEM_WORKMODE(  MRV_SYSTEM_WORKMODE * val0, bool bQuery=true );
int setSYSTEM_POWERON( MRV_SYSTEM_POWERON val0 );
int getSYSTEM_POWERON(  MRV_SYSTEM_POWERON * val0, bool bQuery=true );
int getSYSTEM_SN(  byte * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery=true );
int getSYSTEM_TYPE(  MRV_SYSTEM_TYPE * val0, MRV_LINK_DEVICEINFO_1 * val1, bool bQuery=true );
int getSYSTEM_SOFTVER(  char * val0, char * val1, char * val2, bool bQuery=true );
int getSYSTEM_HARDVER(  char * val0, char * val1, bool bQuery=true );
int getSYSTEM_BOOTVER(  char * val0, char * val1, bool bQuery=true );
int getSYSTEM_ERROR(  char * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery=true );
int setSYSTEM_WARNCLEAR( byte val0 );
int setRS232_BAUD( MRV_RS232_BAUD val0 );
int getRS232_BAUD(  MRV_RS232_BAUD * val0, bool bQuery=true );
int setRS232_WORDLEN( MRV_RS232_WORDLEN val0 );
int getRS232_WORDLEN(  MRV_RS232_WORDLEN * val0, bool bQuery=true );
int setRS232_FLOWCTL( MRV_RS232_FLOWCTL val0 );
int getRS232_FLOWCTL(  MRV_RS232_FLOWCTL * val0, bool bQuery=true );
int setRS232_PARITY( MRV_RS232_PARITY val0 );
int getRS232_PARITY(  MRV_RS232_PARITY * val0, bool bQuery=true );
int setRS232_STOPBIT( MRV_RS232_STOPBIT val0 );
int getRS232_STOPBIT(  MRV_RS232_STOPBIT * val0, bool bQuery=true );
int setRS232_APPLYPARA(  );
int setCAN_TYPE( MRV_CAN_TYPE val0 );
int getCAN_TYPE(  MRV_CAN_TYPE * val0, bool bQuery=true );
int setCAN_BAUD( MRV_CAN_BAUD val0 );
int getCAN_BAUD(  MRV_CAN_BAUD * val0, bool bQuery=true );
int setCAN_GROUP( byte val0 );
int getCAN_GROUP(  byte * val0, bool bQuery=true );
int setCAN_SENDID( uint32 val0 );
int getCAN_SENDID(  uint32 * val0, bool bQuery=true );
int setCAN_RECEIVEID( uint32 val0 );
int getCAN_RECEIVEID(  uint32 * val0, bool bQuery=true );
int setCAN_GROUPID1( uint32 val0 );
int getCAN_GROUPID1(  uint32 * val0, bool bQuery=true );
int setCAN_GROUPID2( uint32 val0 );
int getCAN_GROUPID2(  uint32 * val0, bool bQuery=true );
int setCAN_BROADCASTID( uint32 val0 );
int getCAN_BROADCASTID(  uint32 * val0, bool bQuery=true );
int setCAN_APPLYPARA(  );
int setCAN_NETMANAGELED( MRV_CAN_NETMANAGELED val0 );
int setCAN_NETMANAGESTATE( MRV_CAN_NETMANAGESTATE val0 );
int getCAN_NETMANAGESTATE(  MRV_CAN_NETMANAGESTATE * val0, bool bQuery=true );
int setCAN_NETMANAGEID( uint32 val0 );
int getCAN_NETMANAGEID(  uint32 * val0, bool bQuery=true );
int setCAN_NETMANAGEHASH( uint32 val0 );
int getCAN_NETMANAGEHASH(  uint32 * val0, bool bQuery=true );
int setCAN_NETMANAGESIGNATURE( uint32 val0 );
int getCAN_NETMANAGESIGNATURE(  uint32 * val0, bool bQuery=true );
int setMOTION_SWITCH( byte val0
,MRV_MOTION_SWITCH val1
,MRV_MOTION_SWITCH_1 val2 );
int getMOTION_STATE( byte val0
,MRV_MOTION_SWITCH_1 val1, MRV_MOTION_STATE_1 * val2, bool bQuery=true );
int setMOTION_STATEREPORT( byte val0
,MRV_MOTION_STATEREPORT val1 );
int getMOTION_STATEREPORT( byte val0, MRV_MOTION_STATEREPORT * val1, bool bQuery=true );
int setGLOBAL_GROUP( byte val0
,MRV_GLOBAL_GROUP val1
,byte val2 );
int getGLOBAL_GROUP( byte val0
,MRV_GLOBAL_GROUP val1, byte * val2, bool bQuery=true );
int setGLOBAL_DISTINGUISH( byte val0
,MRV_CAN_NETMANAGELED val1 );
int getGLOBAL_DISTINGUISH( byte val0, MRV_CAN_NETMANAGELED * val1, bool bQuery=true );
int setVALVECTRL_DEVICE( byte val0
,MRV_VALVECTRL_DEVICE val1 );
int getVALVECTRL_DEVICE( byte val0, MRV_VALVECTRL_DEVICE * val1, bool bQuery=true );
int setVALVECTRL_PWMFREQ( uint32 val0 );
int getVALVECTRL_PWMFREQ(  uint32 * val0, bool bQuery=true );
int setVALVECTRL_RTDUTY( byte val0
,uint32 val1 );
int getVALVECTRL_RTDUTY( byte val0, uint32 * val1, bool bQuery=true );
int setVALVECTRL_ACTION( byte val0
,MRV_VALVECTRL_ACTION val1 );
int getVALVECTRL_ACTION( byte val0, MRV_VALVECTRL_ACTION * val1, bool bQuery=true );
int setVALVECTRL_IDLEDUTY( byte val0
,uint32 val1 );
int getVALVECTRL_IDLEDUTY( byte val0, uint32 * val1, bool bQuery=true );
int setVALVECTRL_HOLDDUTY( byte val0
,uint32 val1 );
int getVALVECTRL_HOLDDUTY( byte val0, uint32 * val1, bool bQuery=true );
int setVALVECTRL_OPENDUTY( byte val0
,uint32 val1 );
int getVALVECTRL_OPENDUTY( byte val0, uint32 * val1, bool bQuery=true );
int setVALVECTRL_OPENDLYTIME( byte val0
,uint16 val1 );
int getVALVECTRL_OPENDLYTIME( byte val0, uint16 * val1, bool bQuery=true );
int setVALVECTRL_OPENTIME( byte val0
,uint16 val1 );
int getVALVECTRL_OPENTIME( byte val0, uint16 * val1, bool bQuery=true );
int setVALVECTRL_RECORDSTATE( byte val0
,MRV_CAN_NETMANAGELED val1 );
int getVALVECTRL_RECORDSTATE( byte val0, MRV_CAN_NETMANAGELED * val1, bool bQuery=true );
int getVALVECTRL_ONNUMS( byte val0, uint32 * val1, bool bQuery=true );
int setVALVECTRL_ONNUMSCLEAR( byte val0 );
int getVALVECTRL_OFFNUMS( byte val0, uint32 * val1, bool bQuery=true );
int setVALVECTRL_OFFNUMSCLEAR( byte val0 );
int getVALVECTRL_TIME( byte val0, uint32 * val1, bool bQuery=true );
int setVALVECTRL_TIMECLEAR( byte val0 );
int setTHRESHOLD_HIGHPRESSURE( byte val0
,uint16 val1 );
int getTHRESHOLD_HIGHPRESSURE( byte val0, uint16 * val1, bool bQuery=true );
int setTHRESHOLD_HIPRESSUREACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_HIPRESSUREACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setTHRESHOLD_LOWPRESSURE( byte val0
,uint16 val1 );
int getTHRESHOLD_LOWPRESSURE( byte val0, uint16 * val1, bool bQuery=true );
int setTHRESHOLD_LOWPRESSUREACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_LOWPRESSUREACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setTHRESHOLD_HIGHCURRENT( byte val0
,uint16 val1 );
int getTHRESHOLD_HIGHCURRENT( byte val0, uint16 * val1, bool bQuery=true );
int setTHRESHOLD_HICURRENTACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_HICURRENTACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setTHRESHOLD_LOWCURRENT( byte val0
,uint16 val1 );
int getTHRESHOLD_LOWCURRENT( byte val0, uint16 * val1, bool bQuery=true );
int setTHRESHOLD_LOWCURRENTACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_LOWCURRENTACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setTHRESHOLD_ONNUMS( byte val0
,uint32 val1 );
int getTHRESHOLD_ONNUMS( byte val0, uint32 * val1, bool bQuery=true );
int setTHRESHOLD_NUMSACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_NUMSACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setTHRESHOLD_TIME( byte val0
,uint32 val1 );
int getTHRESHOLD_TIME( byte val0, uint32 * val1, bool bQuery=true );
int setTHRESHOLD_TIMEACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_TIMEACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setPVT_EXECMODE( byte val0
,MRV_PVT_EXECMODE val1 );
int getPVT_EXECMODE( byte val0, MRV_PVT_EXECMODE * val1, bool bQuery=true );
int setPVT_RESET( byte val0 );
int setPVT_CYCLES( byte val0
,uint32 val1 );
int getPVT_CYCLES( byte val0, uint32 * val1, bool bQuery=true );
int setPVT_GRADE( byte val0
,byte val1
,uint32 val2 );
int getPVT_GRADE( byte val0
,byte val1, uint32 * val2, bool bQuery=true );
int getPVT_BUFFERCHECK( byte val0, uint32 * val1, bool bQuery=true );
int setPVT_START( byte val0 );
int getPVT_START(  byte * val0, bool bQuery=true );
int setPVT_END( byte val0 );
int getPVT_END(  byte * val0, bool bQuery=true );
int setPVT_PDATA( uint16 val0
,uint32 val1 );
int getPVT_PDATA( uint16 val0, uint32 * val1, bool bQuery=true );
int setPVT_TDATA( uint16 val0
,f32 val1 );
int getPVT_TDATA( uint16 val0, f32 * val1, bool bQuery=true );
int setPVT_HOLD( byte val0
,MRV_PVT_HOLD val1 );
int getPVT_HOLD( byte val0, MRV_PVT_HOLD * val1, bool bQuery=true );
int setPVT_SAVE( byte val0 );
int getPVT_SAVE(  byte * val0, bool bQuery=true );
int setCALIBRATION_TYPE( byte val0
,MRV_CALIBRATION_TYPE val1 );
int getCALIBRATION_TYPE( byte val0, MRV_CALIBRATION_TYPE * val1, bool bQuery=true );
int setCALIBRATION_SAMPLERATE( byte val0
,MRV_CALIBRATION_SAMPLERATE val1 );
int getCALIBRATION_SAMPLERATE( byte val0, MRV_CALIBRATION_SAMPLERATE * val1, bool bQuery=true );
int setCALIBRATION_OPENTIME( byte val0
,uint16 val1 );
int getCALIBRATION_OPENTIME( byte val0, uint16 * val1, bool bQuery=true );
int setCALIBRATION_HOLDTIME( byte val0
,uint16 val1 );
int getCALIBRATION_HOLDTIME( byte val0, uint16 * val1, bool bQuery=true );
int setCALIBRATION_CLOSETIME( byte val0
,uint16 val1 );
int getCALIBRATION_CLOSETIME( byte val0, uint16 * val1, bool bQuery=true );
int setCALIBRATION_OPENDUTY( byte val0
,uint32 val1 );
int getCALIBRATION_OPENDUTY( byte val0, uint32 * val1, bool bQuery=true );
int setCALIBRATION_HOLDDUTY( byte val0
,uint32 val1 );
int getCALIBRATION_HOLDDUTY( byte val0, uint32 * val1, bool bQuery=true );
int getCALIBRATION_DATALENGTH( byte val0, uint16 * val1, bool bQuery=true );
int setCALIBRATION_DATAUPCTRL( byte val0
,MRV_CALIBRATION_DATAUPCTRL val1 );
int setCALIBRATION_DATAUP( uint16 val0
,uint16 val1
,uint16 val2 );
int setREPORT_STATE( byte val0
,MRV_REPORT_STATE val1
,MRV_CAN_NETMANAGELED val2 );
int getREPORT_STATE( byte val0
,MRV_REPORT_STATE val1, MRV_CAN_NETMANAGELED * val2, bool bQuery=true );
int setREPORT_PERIOD( byte val0
,MRV_REPORT_STATE val1
,uint32 val2 );
int getREPORT_PERIOD( byte val0
,MRV_REPORT_STATE val1, uint32 * val2, bool bQuery=true );
int getREPORT_DATA( byte val0
,MRV_REPORT_STATE val1, uint32 * val2, bool bQuery=true );
int setIOCONFIG_IOFUNCSEL( byte val0
,MRV_IOCONFIG_IOFUNCSEL val1 );
int getIOCONFIG_IOFUNCSEL( byte val0, MRV_IOCONFIG_IOFUNCSEL * val1, bool bQuery=true );
int setIOCONFIG_SWFILTERDELAY( byte val0
,uint16 val1 );
int getIOCONFIG_SWFILTERDELAY( byte val0, uint16 * val1, bool bQuery=true );
int setIOCONFIG_SWFILTERCOUNTER( byte val0
,uint16 val1 );
int getIOCONFIG_SWFILTERCOUNTER( byte val0, uint16 * val1, bool bQuery=true );
int setIOCONFIG_TRIGEDGE( byte val0
,MRV_IOCONFIG_TRIGEDGE val1 );
int getIOCONFIG_TRIGEDGE( byte val0, MRV_IOCONFIG_TRIGEDGE * val1, bool bQuery=true );
int setIOCONFIG_BAUD( byte val0
,MRV_RS232_BAUD val1 );
int getIOCONFIG_BAUD( byte val0, MRV_RS232_BAUD * val1, bool bQuery=true );
int setIOCONFIG_WORDLEN( byte val0
,MRV_RS232_WORDLEN val1 );
int getIOCONFIG_WORDLEN( byte val0, MRV_RS232_WORDLEN * val1, bool bQuery=true );
int setIOCONFIG_FLOWCTL( byte val0
,MRV_IOCONFIG_FLOWCTL val1 );
int getIOCONFIG_FLOWCTL( byte val0, MRV_IOCONFIG_FLOWCTL * val1, bool bQuery=true );
int setIOCONFIG_PARITY( byte val0
,MRV_RS232_PARITY val1 );
int getIOCONFIG_PARITY( byte val0, MRV_RS232_PARITY * val1, bool bQuery=true );
int setIOCONFIG_STOPBIT( byte val0
,MRV_RS232_STOPBIT val1 );
int getIOCONFIG_STOPBIT( byte val0, MRV_RS232_STOPBIT * val1, bool bQuery=true );
int setIOCONFIG_APPLYPARA( byte val0 );
void loadOtp();
public:
	MRV_LINK_INTFC mLINK_INTFC;
	MRV_LINK_DEVICEINFO mLINK_DEVICEINFO;
	MRV_LINK_DEVICEINFO_1 mLINK_DEVICEINFO1;
	uint32 mLINK_DEVICEINFO2;

	MRV_SYSTEM_WORKMODE mSYSTEM_WORKMODE;
	MRV_SYSTEM_POWERON mSYSTEM_POWERON;
	byte mSYSTEM_SN;
	char mSYSTEM_SN1;

	char mSYSTEM_SN2;
	char mSYSTEM_SN3;
	char mSYSTEM_SN4;
	char mSYSTEM_SN5;

	MRV_SYSTEM_TYPE mSYSTEM_TYPE;
	MRV_LINK_DEVICEINFO_1 mSYSTEM_TYPE1;
	char mSYSTEM_SOFTVER;
	char mSYSTEM_SOFTVER1;

	char mSYSTEM_SOFTVER2;
	char mSYSTEM_HARDVER;
	char mSYSTEM_HARDVER1;
	char mSYSTEM_BOOTVER;

	char mSYSTEM_BOOTVER1;
	char mSYSTEM_ERROR;
	char mSYSTEM_ERROR1;
	char mSYSTEM_ERROR2;

	char mSYSTEM_ERROR3;
	char mSYSTEM_ERROR4;
	char mSYSTEM_ERROR5;
	MRV_RS232_BAUD mRS232_BAUD;

	MRV_RS232_WORDLEN mRS232_WORDLEN;
	MRV_RS232_FLOWCTL mRS232_FLOWCTL;
	MRV_RS232_PARITY mRS232_PARITY;
	MRV_RS232_STOPBIT mRS232_STOPBIT;

	MRV_CAN_TYPE mCAN_TYPE;
	MRV_CAN_BAUD mCAN_BAUD;
	byte mCAN_GROUP;
	uint32 mCAN_SENDID;

	uint32 mCAN_RECEIVEID;
	uint32 mCAN_GROUPID1;
	uint32 mCAN_GROUPID2;
	uint32 mCAN_BROADCASTID;

	MRV_CAN_NETMANAGESTATE mCAN_NETMANAGESTATE;
	uint32 mCAN_NETMANAGEID;
	uint32 mCAN_NETMANAGEHASH;
	uint32 mCAN_NETMANAGESIGNATURE;

	MRV_MOTION_STATE_1 mMOTION_STATE[10][2];
	MRV_MOTION_STATEREPORT mMOTION_STATEREPORT[10];
	byte mGLOBAL_GROUP[10][2];
	MRV_CAN_NETMANAGELED mGLOBAL_DISTINGUISH[10];

	MRV_VALVECTRL_DEVICE mVALVECTRL_DEVICE[10];
	uint32 mVALVECTRL_PWMFREQ;
	uint32 mVALVECTRL_RTDUTY[10];
	MRV_VALVECTRL_ACTION mVALVECTRL_ACTION[10];

	uint32 mVALVECTRL_IDLEDUTY[10];
	uint32 mVALVECTRL_HOLDDUTY[10];
	uint32 mVALVECTRL_OPENDUTY[10];
	uint16 mVALVECTRL_OPENDLYTIME[10];

	uint16 mVALVECTRL_OPENTIME[10];
	MRV_CAN_NETMANAGELED mVALVECTRL_RECORDSTATE[10];
	uint32 mVALVECTRL_ONNUMS[10];
	uint32 mVALVECTRL_OFFNUMS[10];

	uint32 mVALVECTRL_TIME[10];
	uint16 mTHRESHOLD_HIGHPRESSURE[10];
	MRV_THRESHOLD_HIPRESSUREACTION mTHRESHOLD_HIPRESSUREACTION[10];
	uint16 mTHRESHOLD_LOWPRESSURE[10];

	MRV_THRESHOLD_HIPRESSUREACTION mTHRESHOLD_LOWPRESSUREACTION[10];
	uint16 mTHRESHOLD_HIGHCURRENT[10];
	MRV_THRESHOLD_HIPRESSUREACTION mTHRESHOLD_HICURRENTACTION[10];
	uint16 mTHRESHOLD_LOWCURRENT[10];

	MRV_THRESHOLD_HIPRESSUREACTION mTHRESHOLD_LOWCURRENTACTION[10];
	uint32 mTHRESHOLD_ONNUMS[10];
	MRV_THRESHOLD_HIPRESSUREACTION mTHRESHOLD_NUMSACTION[10];
	uint32 mTHRESHOLD_TIME[10];

	MRV_THRESHOLD_HIPRESSUREACTION mTHRESHOLD_TIMEACTION[10];
	MRV_PVT_EXECMODE mPVT_EXECMODE[10];
	uint32 mPVT_CYCLES[10];
	uint32 mPVT_GRADE[10][10];

	uint32 mPVT_BUFFERCHECK[10];
	byte mPVT_START;
	byte mPVT_END;
	uint32 mPVT_PDATA[10];

	f32 mPVT_TDATA[10];
	MRV_PVT_HOLD mPVT_HOLD[10];
	byte mPVT_SAVE;
	MRV_CALIBRATION_TYPE mCALIBRATION_TYPE[10];

	MRV_CALIBRATION_SAMPLERATE mCALIBRATION_SAMPLERATE[10];
	uint16 mCALIBRATION_OPENTIME[10];
	uint16 mCALIBRATION_HOLDTIME[10];
	uint16 mCALIBRATION_CLOSETIME[10];

	uint32 mCALIBRATION_OPENDUTY[10];
	uint32 mCALIBRATION_HOLDDUTY[10];
	uint16 mCALIBRATION_DATALENGTH[10];
	MRV_CAN_NETMANAGELED mREPORT_STATE[10][4];

	uint32 mREPORT_PERIOD[10][4];
	uint32 mREPORT_DATA[10][4];
	MRV_IOCONFIG_IOFUNCSEL mIOCONFIG_IOFUNCSEL[10];
	uint16 mIOCONFIG_SWFILTERDELAY[10];

	uint16 mIOCONFIG_SWFILTERCOUNTER[10];
	MRV_IOCONFIG_TRIGEDGE mIOCONFIG_TRIGEDGE[10];
	MRV_RS232_BAUD mIOCONFIG_BAUD[10];
	MRV_RS232_WORDLEN mIOCONFIG_WORDLEN[10];

	MRV_IOCONFIG_FLOWCTL mIOCONFIG_FLOWCTL[10];
	MRV_RS232_PARITY mIOCONFIG_PARITY[10];
	MRV_RS232_STOPBIT mIOCONFIG_STOPBIT[10];
};
}
#endif
