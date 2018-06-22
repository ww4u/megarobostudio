#ifndef _MRV_H_
#define _MRV_H_
#include "MRV_model.h"

namespace MegaDevice
{
class MRV :  public MRV_model
{
public:
MRV();
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
int getVALVECTRL_OPENDLYTIME( byte val0, uint32 * val1, bool bQuery=true );
int setVALVECTRL_OPENTIME( byte val0
,uint16 val1 );
int getVALVECTRL_OPENTIME( byte val0, uint32 * val1, bool bQuery=true );
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
,uint32 val1 );
int getTHRESHOLD_HIGHPRESSURE( byte val0, uint32 * val1, bool bQuery=true );
int setTHRESHOLD_HIPRESSUREACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_HIPRESSUREACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setTHRESHOLD_LOWPRESSURE( byte val0
,uint32 val1 );
int getTHRESHOLD_LOWPRESSURE( byte val0, uint32 * val1, bool bQuery=true );
int setTHRESHOLD_LOWPRESSUREACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_LOWPRESSUREACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setTHRESHOLD_HIGHCURRENT( byte val0
,uint32 val1 );
int getTHRESHOLD_HIGHCURRENT( byte val0, uint32 * val1, bool bQuery=true );
int setTHRESHOLD_HICURRENTACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 );
int getTHRESHOLD_HICURRENTACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery=true );
int setTHRESHOLD_LOWCURRENT( byte val0
,uint32 val1 );
int getTHRESHOLD_LOWCURRENT( byte val0, uint32 * val1, bool bQuery=true );
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
,uint32 val1 );
int getPVT_TDATA( uint16 val0, uint32 * val1, bool bQuery=true );
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
};
}
#endif
