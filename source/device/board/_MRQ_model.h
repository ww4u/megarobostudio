#ifndef __MRQ_MODEL_H_
#define __MRQ_MODEL_H_
#include "../../../include/mcstd.h"

#include "../devicemodel.h"
#include "_MRQ_enum.h"
namespace MegaDevice
{
class _MRQ_model : public DeviceModel
{
public:
QString toString( MRQ_LINK_INTFC eType );
QString toString( MRQ_SYSTEM_WORKMODE eType );
QString toString( MRQ_SYSTEM_POWERON eType );
QString toString( MRQ_SYSTEM_TYPE eType );
QString toString( MRQ_SYSTEM_TYPE_1 eType );
QString toString( MRQ_SYSTEM_REPORTSWITCH eType );
QString toString( MRQ_RS232_BAUD eType );
QString toString( MRQ_RS232_WORDLEN eType );
QString toString( MRQ_RS232_FLOWCTL eType );
QString toString( MRQ_RS232_PARITY eType );
QString toString( MRQ_RS232_STOPBIT eType );
QString toString( MRQ_CAN_TYPE eType );
QString toString( MRQ_CAN_BAUD eType );
QString toString( MRQ_CAN_NETMANAGESTATE eType );
QString toString( MRQ_CAN_NETMANAGEID eType );
QString toString( MRQ_CLOCK_STARTTYPE eType );
QString toString( MRQ_CLOCK_SYNCSTATE eType );
QString toString( MRQ_MOTION_SWITCH eType );
QString toString( MRQ_MOTION_STATE eType );
QString toString( MRQ_MOTION_STATEREPORT eType );
QString toString( MRQ_MOTION_STARTSOURCE eType );
QString toString( MRQ_MOTION_INITPOSITIONUNIT eType );
QString toString( MRQ_MOTION_INITIOSIGNAL eType );
QString toString( MRQ_MOTION_INCHINGMODE eType );
QString toString( MRQ_IDENTITY_GROUP eType );
QString toString( MRQ_STOPDECEL_MODE eType );
QString toString( MRQ_OUTOFSTEP_LINERESPONSE eType );
QString toString( MRQ_MOTOR_STEPANGLE eType );
QString toString( MRQ_MOTOR_TYPE eType );
QString toString( MRQ_MOTOR_POSITIONUNIT eType );
QString toString( MRQ_MOTOR_SIZE eType );
QString toString( MRQ_ENCODER_CHANNELNUM eType );
QString toString( MRQ_ENCODER_TYPE eType );
QString toString( MRQ_ENCODER_MULTIPLE eType );
QString toString( MRQ_ENCODER_STATE eType );
QString toString( MRQ_MOTIONPLAN_PVTCONFIG eType );
QString toString( MRQ_MOTIONPLAN_EXECUTEMODE eType );
QString toString( MRQ_MOTIONPLAN_PLANMODE eType );
QString toString( MRQ_MOTIONPLAN_MOTIONMODE eType );
QString toString( MRQ_MOTIONPLAN_MODIFYDUTY eType );
QString toString( MRQ_MOTIONPLAN_ENDSTATE eType );
QString toString( MRQ_REPORT_STATE eType );
QString toString( MRQ_TRIGGER_MODE eType );
QString toString( MRQ_TRIGGER_PATTERN eType );
QString toString( MRQ_TRIGGER_PATTSMODE eType );
QString toString( MRQ_TRIGGER_LEVELTYPE_1 eType );
QString toString( MRQ_DRIVER_TYPE eType );
QString toString( MRQ_DRIVER_STATEREG eType );
QString toString( MRQ_DRIVER_MICROSTEPS eType );
QString toString( MRQ_DRIVER_REGCONFIG eType );
QString toString( MRQ_DRIVER_SGLIMIT eType );
QString toString( MRQ_DRIVER_SGPARASET eType );
QString toString( MRQ_DIGITALOUT_STATE eType );
QString toString( MRQ_DIGITALOUT_STATE_1 eType );
QString toString( MRQ_DIGITALOUT_SIGNAL_1 eType );
QString toString( MRQ_DIGITALOUT_POLARITY_1 eType );
QString toString( MRQ_DIGITALOUT_CONDITION_1 eType );
QString toString( MRQ_ISOLATOROUTPUT_STATE eType );
QString toString( MRQ_ISOLATOROUTPUT_RESPONSE_1 eType );
QString toString( MRQ_SENSORUART_BAUD eType );
QString toString( MRQ_SENSORUART_STATE_1 eType );
QString toString( MRQ_PRESET_PVTCONFIG eType );
QString toString( MRQ_PRESET_EXECUTE_1 eType );
int toValue( const QString &str, MRQ_LINK_INTFC *pEVal );
int toValue( const QString &str, MRQ_SYSTEM_WORKMODE *pEVal );
int toValue( const QString &str, MRQ_SYSTEM_POWERON *pEVal );
int toValue( const QString &str, MRQ_SYSTEM_TYPE *pEVal );
int toValue( const QString &str, MRQ_SYSTEM_TYPE_1 *pEVal );
int toValue( const QString &str, MRQ_SYSTEM_REPORTSWITCH *pEVal );
int toValue( const QString &str, MRQ_RS232_BAUD *pEVal );
int toValue( const QString &str, MRQ_RS232_WORDLEN *pEVal );
int toValue( const QString &str, MRQ_RS232_FLOWCTL *pEVal );
int toValue( const QString &str, MRQ_RS232_PARITY *pEVal );
int toValue( const QString &str, MRQ_RS232_STOPBIT *pEVal );
int toValue( const QString &str, MRQ_CAN_TYPE *pEVal );
int toValue( const QString &str, MRQ_CAN_BAUD *pEVal );
int toValue( const QString &str, MRQ_CAN_NETMANAGESTATE *pEVal );
int toValue( const QString &str, MRQ_CAN_NETMANAGEID *pEVal );
int toValue( const QString &str, MRQ_CLOCK_STARTTYPE *pEVal );
int toValue( const QString &str, MRQ_CLOCK_SYNCSTATE *pEVal );
int toValue( const QString &str, MRQ_MOTION_SWITCH *pEVal );
int toValue( const QString &str, MRQ_MOTION_STATE *pEVal );
int toValue( const QString &str, MRQ_MOTION_STATEREPORT *pEVal );
int toValue( const QString &str, MRQ_MOTION_STARTSOURCE *pEVal );
int toValue( const QString &str, MRQ_MOTION_INITPOSITIONUNIT *pEVal );
int toValue( const QString &str, MRQ_MOTION_INITIOSIGNAL *pEVal );
int toValue( const QString &str, MRQ_MOTION_INCHINGMODE *pEVal );
int toValue( const QString &str, MRQ_IDENTITY_GROUP *pEVal );
int toValue( const QString &str, MRQ_STOPDECEL_MODE *pEVal );
int toValue( const QString &str, MRQ_OUTOFSTEP_LINERESPONSE *pEVal );
int toValue( const QString &str, MRQ_MOTOR_STEPANGLE *pEVal );
int toValue( const QString &str, MRQ_MOTOR_TYPE *pEVal );
int toValue( const QString &str, MRQ_MOTOR_POSITIONUNIT *pEVal );
int toValue( const QString &str, MRQ_MOTOR_SIZE *pEVal );
int toValue( const QString &str, MRQ_ENCODER_CHANNELNUM *pEVal );
int toValue( const QString &str, MRQ_ENCODER_TYPE *pEVal );
int toValue( const QString &str, MRQ_ENCODER_MULTIPLE *pEVal );
int toValue( const QString &str, MRQ_ENCODER_STATE *pEVal );
int toValue( const QString &str, MRQ_MOTIONPLAN_PVTCONFIG *pEVal );
int toValue( const QString &str, MRQ_MOTIONPLAN_EXECUTEMODE *pEVal );
int toValue( const QString &str, MRQ_MOTIONPLAN_PLANMODE *pEVal );
int toValue( const QString &str, MRQ_MOTIONPLAN_MOTIONMODE *pEVal );
int toValue( const QString &str, MRQ_MOTIONPLAN_MODIFYDUTY *pEVal );
int toValue( const QString &str, MRQ_MOTIONPLAN_ENDSTATE *pEVal );
int toValue( const QString &str, MRQ_REPORT_STATE *pEVal );
int toValue( const QString &str, MRQ_TRIGGER_MODE *pEVal );
int toValue( const QString &str, MRQ_TRIGGER_PATTERN *pEVal );
int toValue( const QString &str, MRQ_TRIGGER_PATTSMODE *pEVal );
int toValue( const QString &str, MRQ_TRIGGER_LEVELTYPE_1 *pEVal );
int toValue( const QString &str, MRQ_DRIVER_TYPE *pEVal );
int toValue( const QString &str, MRQ_DRIVER_STATEREG *pEVal );
int toValue( const QString &str, MRQ_DRIVER_MICROSTEPS *pEVal );
int toValue( const QString &str, MRQ_DRIVER_REGCONFIG *pEVal );
int toValue( const QString &str, MRQ_DRIVER_SGLIMIT *pEVal );
int toValue( const QString &str, MRQ_DRIVER_SGPARASET *pEVal );
int toValue( const QString &str, MRQ_DIGITALOUT_STATE *pEVal );
int toValue( const QString &str, MRQ_DIGITALOUT_STATE_1 *pEVal );
int toValue( const QString &str, MRQ_DIGITALOUT_SIGNAL_1 *pEVal );
int toValue( const QString &str, MRQ_DIGITALOUT_POLARITY_1 *pEVal );
int toValue( const QString &str, MRQ_DIGITALOUT_CONDITION_1 *pEVal );
int toValue( const QString &str, MRQ_ISOLATOROUTPUT_STATE *pEVal );
int toValue( const QString &str, MRQ_ISOLATOROUTPUT_RESPONSE_1 *pEVal );
int toValue( const QString &str, MRQ_SENSORUART_BAUD *pEVal );
int toValue( const QString &str, MRQ_SENSORUART_STATE_1 *pEVal );
int toValue( const QString &str, MRQ_PRESET_PVTCONFIG *pEVal );
int toValue( const QString &str, MRQ_PRESET_EXECUTE_1 *pEVal );
_MRQ_model();
int setLINK_INTFC( MRQ_LINK_INTFC val0 );
int getLINK_INTFC(  MRQ_LINK_INTFC * val0, bool bQuery=true );
int setSYSTEM_WORKMODE( MRQ_SYSTEM_WORKMODE val0 );
int getSYSTEM_WORKMODE(  MRQ_SYSTEM_WORKMODE * val0, bool bQuery=true );
int setSYSTEM_POWERON( MRQ_SYSTEM_POWERON val0 );
int getSYSTEM_POWERON(  MRQ_SYSTEM_POWERON * val0, bool bQuery=true );
int getSYSTEM_SN(  byte * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery=true );
int getSYSTEM_TYPE(  MRQ_SYSTEM_TYPE * val0, MRQ_SYSTEM_TYPE_1 * val1, bool bQuery=true );
int getSYSTEM_SOFTVER(  char * val0, char * val1, char * val2, bool bQuery=true );
int getSYSTEM_FPGAVER(  char * val0, char * val1, char * val2, bool bQuery=true );
int getSYSTEM_HARDVER(  char * val0, char * val1, bool bQuery=true );
int getSYSTEM_BOOTVER(  char * val0, char * val1, bool bQuery=true );
int getSYSTEM_ERROR(  char * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery=true );
int setSYSTEM_REPORTSWITCH( MRQ_SYSTEM_REPORTSWITCH val0 );
int getSYSTEM_REPORTSWITCH(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery=true );
int setRS232_BAUD( MRQ_RS232_BAUD val0 );
int getRS232_BAUD(  MRQ_RS232_BAUD * val0, bool bQuery=true );
int setRS232_WORDLEN( MRQ_RS232_WORDLEN val0 );
int getRS232_WORDLEN(  MRQ_RS232_WORDLEN * val0, bool bQuery=true );
int setRS232_FLOWCTL( MRQ_RS232_FLOWCTL val0 );
int getRS232_FLOWCTL(  MRQ_RS232_FLOWCTL * val0, bool bQuery=true );
int setRS232_PARITY( MRQ_RS232_PARITY val0 );
int getRS232_PARITY(  MRQ_RS232_PARITY * val0, bool bQuery=true );
int setRS232_STOPBIT( MRQ_RS232_STOPBIT val0 );
int getRS232_STOPBIT(  MRQ_RS232_STOPBIT * val0, bool bQuery=true );
int setRS232_APPLYPARA(  );
int setCAN_TYPE( MRQ_CAN_TYPE val0 );
int getCAN_TYPE(  MRQ_CAN_TYPE * val0, bool bQuery=true );
int setCAN_BAUD( MRQ_CAN_BAUD val0 );
int getCAN_BAUD(  MRQ_CAN_BAUD * val0, bool bQuery=true );
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
int setCAN_NETMANAGELED( MRQ_SYSTEM_REPORTSWITCH val0 );
int setCAN_NETMANAGESTATE( MRQ_CAN_NETMANAGESTATE val0 );
int getCAN_NETMANAGESTATE(  MRQ_CAN_NETMANAGESTATE * val0, bool bQuery=true );
int setCAN_NETMANAGEID( MRQ_CAN_NETMANAGEID val0
,uint32 val1 );
int getCAN_NETMANAGEID( MRQ_CAN_NETMANAGEID val0, uint32 * val1, bool bQuery=true );
int setCAN_NETMANAGEHASH( uint32 val0 );
int getCAN_NETMANAGEHASH(  uint32 * val0, bool bQuery=true );
int setCLOCK_FREQUENCY( uint32 val0 );
int getCLOCK_FREQUENCY(  uint32 * val0, bool bQuery=true );
int setCLOCK_SYNCREGISTER( MRQ_SYSTEM_REPORTSWITCH val0 );
int getCLOCK_SYNCREGISTER(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery=true );
int setCLOCK_STARTTYPE( MRQ_CLOCK_STARTTYPE val0 );
int getCLOCK_STARTTYPE(  MRQ_CLOCK_STARTTYPE * val0, bool bQuery=true );
int setCLOCK_SYNCSTATE( MRQ_CLOCK_SYNCSTATE val0 );
int getCLOCK_SYNCSTATE(  MRQ_CLOCK_SYNCSTATE * val0, bool bQuery=true );
int getCLOCK_COUNT(  uint32 * val0, bool bQuery=true );
int setMOTION_SWITCH( byte val0
,MRQ_MOTION_SWITCH val1 );
int getMOTION_STATE( byte val0, MRQ_MOTION_STATE * val1, bool bQuery=true );
int setMOTION_STATEREPORT( byte val0
,MRQ_MOTION_STATEREPORT val1 );
int getMOTION_STATEREPORT( byte val0, MRQ_MOTION_STATEREPORT * val1, bool bQuery=true );
int setMOTION_STARTSOURCE( byte val0
,MRQ_MOTION_STARTSOURCE val1 );
int getMOTION_STARTSOURCE( byte val0, MRQ_MOTION_STARTSOURCE * val1, bool bQuery=true );
int setMOTION_STARTTYPE( byte val0
,MRQ_CLOCK_STARTTYPE val1 );
int getMOTION_STARTTYPE( byte val0, MRQ_CLOCK_STARTTYPE * val1, bool bQuery=true );
int setMOTION_MAXSPEED( byte val0
,f32 val1 );
int getMOTION_MAXSPEED( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_MINSPEED( byte val0
,f32 val1 );
int getMOTION_MINSPEED( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_MAXPOSITION( byte val0
,f32 val1 );
int getMOTION_MAXPOSITION( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_MINPOSITION( byte val0
,f32 val1 );
int getMOTION_MINPOSITION( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_MAXTORQUE( byte val0
,f32 val1 );
int getMOTION_MAXTORQUE( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_MINTORQUE( byte val0
,f32 val1 );
int getMOTION_MINTORQUE( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_MAXACCELERATION( byte val0
,f32 val1 );
int getMOTION_MAXACCELERATION( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_MINACCELERATION( byte val0
,f32 val1 );
int getMOTION_MINACCELERATION( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_ORIGIN( byte val0
,uint32 val1 );
int getMOTION_ORIGIN( byte val0, uint32 * val1, bool bQuery=true );
int setMOTION_GOORIGIN( byte val0 );
int setMOTION_INITPOSITIONUNIT( byte val0
,MRQ_MOTION_INITPOSITIONUNIT val1 );
int getMOTION_INITPOSITIONUNIT( byte val0, MRQ_MOTION_INITPOSITIONUNIT * val1, bool bQuery=true );
int setMOTION_INITPOSITION( byte val0
,f32 val1 );
int getMOTION_INITPOSITION( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_INITIOSIGNAL( byte val0
,MRQ_MOTION_INITIOSIGNAL val1 );
int getMOTION_INITIOSIGNAL( byte val0, MRQ_MOTION_INITIOSIGNAL * val1, bool bQuery=true );
int setMOTION_GOINITPOSITION( byte val0 );
int setMOTION_INCHINGMODE( byte val0
,MRQ_MOTION_INCHINGMODE val1 );
int getMOTION_INCHINGMODE( byte val0, MRQ_MOTION_INCHINGMODE * val1, bool bQuery=true );
int setMOTION_INCHINGTIME( byte val0
,f32 val1 );
int getMOTION_INCHINGTIME( byte val0, f32 * val1, bool bQuery=true );
int setMOTION_INCHING( byte val0
,f32 val1 );
int setMOTION_OFFSETSTATE( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 );
int getMOTION_OFFSETSTATE( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery=true );
int setMOTION_OFFSET( byte val0 );
int getMOTION_PVTSTEPS( byte val0, int32 * val1, bool bQuery=true );
int getMOTION_COUNTSTEPS( byte val0, int32 * val1, bool bQuery=true );
int getMOTION_PVTCIRCLE( byte val0, int16 * val1, bool bQuery=true );
int getMOTION_COUNTCIRCLE( byte val0, int16 * val1, bool bQuery=true );
int getMOTION_ABCOUNT( byte val0, int32 * val1, bool bQuery=true );
int setMOTION_REVMOTION( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 );
int getMOTION_REVMOTION( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery=true );
int setIDENTITY_GROUP( byte val0
,MRQ_IDENTITY_GROUP val1
,byte val2 );
int getIDENTITY_GROUP( byte val0
,MRQ_IDENTITY_GROUP val1, byte * val2, bool bQuery=true );
int setIDENTITY_DISTDEVICE( MRQ_SYSTEM_REPORTSWITCH val0 );
int getIDENTITY_DISTDEVICE(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery=true );
int setSTOPDECEL_MODE( byte val0
,MRQ_STOPDECEL_MODE val1 );
int getSTOPDECEL_MODE( byte val0, MRQ_STOPDECEL_MODE * val1, bool bQuery=true );
int setSTOPDECEL_DISTANCE( byte val0
,f32 val1 );
int getSTOPDECEL_DISTANCE( byte val0, f32 * val1, bool bQuery=true );
int setSTOPDECEL_TIME( byte val0
,f32 val1 );
int getSTOPDECEL_TIME( byte val0, f32 * val1, bool bQuery=true );
int setOUTOFSTEP_LINESTATE( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 );
int getOUTOFSTEP_LINESTATE( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery=true );
int setOUTOFSTEP_LINEOUTNUM( byte val0
,uint32 val1 );
int getOUTOFSTEP_LINEOUTNUM( byte val0, uint32 * val1, bool bQuery=true );
int setOUTOFSTEP_LINERESPONSE( byte val0
,MRQ_OUTOFSTEP_LINERESPONSE val1 );
int getOUTOFSTEP_LINERESPONSE( byte val0, MRQ_OUTOFSTEP_LINERESPONSE * val1, bool bQuery=true );
int setOUTOFSTEP_LINEWARN( byte val0 );
int setOUTOFSTEP_TOTALSTATE( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 );
int getOUTOFSTEP_TOTALSTATE( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery=true );
int setOUTOFSTEP_TOTALOUTNUM( byte val0
,uint32 val1 );
int getOUTOFSTEP_TOTALOUTNUM( byte val0, uint32 * val1, bool bQuery=true );
int setOUTOFSTEP_TOTALRESPONSE( byte val0
,MRQ_OUTOFSTEP_LINERESPONSE val1 );
int getOUTOFSTEP_TOTALRESPONSE( byte val0, MRQ_OUTOFSTEP_LINERESPONSE * val1, bool bQuery=true );
int setOUTOFSTEP_TOTALWARN( byte val0 );
int setMOTOR_STEPANGLE( byte val0
,MRQ_MOTOR_STEPANGLE val1 );
int getMOTOR_STEPANGLE( byte val0, MRQ_MOTOR_STEPANGLE * val1, bool bQuery=true );
int setMOTOR_TYPE( byte val0
,MRQ_MOTOR_TYPE val1 );
int getMOTOR_TYPE( byte val0, MRQ_MOTOR_TYPE * val1, bool bQuery=true );
int setMOTOR_POSITIONUNIT( byte val0
,MRQ_MOTOR_POSITIONUNIT val1 );
int getMOTOR_POSITIONUNIT( byte val0, MRQ_MOTOR_POSITIONUNIT * val1, bool bQuery=true );
int setMOTOR_GEARRATIONUM( byte val0
,uint16 val1 );
int getMOTOR_GEARRATIONUM( byte val0, uint16 * val1, bool bQuery=true );
int setMOTOR_GEARRATIODEN( byte val0
,uint16 val1 );
int getMOTOR_GEARRATIODEN( byte val0, uint16 * val1, bool bQuery=true );
int setMOTOR_LEAD( byte val0
,f32 val1 );
int getMOTOR_LEAD( byte val0, f32 * val1, bool bQuery=true );
int setMOTOR_PEAKSPEED( byte val0
,f32 val1 );
int getMOTOR_PEAKSPEED( byte val0, f32 * val1, bool bQuery=true );
int setMOTOR_PEAKACCELERATION( byte val0
,f32 val1 );
int getMOTOR_PEAKACCELERATION( byte val0, f32 * val1, bool bQuery=true );
int setMOTOR_SIZE( byte val0
,MRQ_MOTOR_SIZE val1 );
int getMOTOR_SIZE( byte val0, MRQ_MOTOR_SIZE * val1, bool bQuery=true );
int setMOTOR_VOLTAGE( byte val0
,byte val1 );
int getMOTOR_VOLTAGE( byte val0, byte * val1, bool bQuery=true );
int setMOTOR_CURRENT( byte val0
,byte val1 );
int getMOTOR_CURRENT( byte val0, byte * val1, bool bQuery=true );
int setMOTOR_BACKLASH( byte val0
,f32 val1 );
int getMOTOR_BACKLASH( byte val0, f32 * val1, bool bQuery=true );
int setENCODER_LINENUM( byte val0
,uint32 val1 );
int getENCODER_LINENUM( byte val0, uint32 * val1, bool bQuery=true );
int setENCODER_CHANNELNUM( byte val0
,MRQ_ENCODER_CHANNELNUM val1 );
int getENCODER_CHANNELNUM( byte val0, MRQ_ENCODER_CHANNELNUM * val1, bool bQuery=true );
int setENCODER_TYPE( byte val0
,MRQ_ENCODER_TYPE val1 );
int getENCODER_TYPE( byte val0, MRQ_ENCODER_TYPE * val1, bool bQuery=true );
int setENCODER_MULTIPLE( byte val0
,MRQ_ENCODER_MULTIPLE val1 );
int getENCODER_MULTIPLE( byte val0, MRQ_ENCODER_MULTIPLE * val1, bool bQuery=true );
int setENCODER_STATE( byte val0
,MRQ_ENCODER_STATE val1 );
int getENCODER_STATE( byte val0, MRQ_ENCODER_STATE * val1, bool bQuery=true );
int setMOTIONPLAN_PVTCONFIG( byte val0
,MRQ_MOTIONPLAN_PVTCONFIG val1 );
int getMOTIONPLAN_PVTCONFIG( byte val0, MRQ_MOTIONPLAN_PVTCONFIG * val1, bool bQuery=true );
int setMOTIONPLAN_POSITION( uint16 val0
,f32 val1 );
int getMOTIONPLAN_POSITION( uint16 val0, f32 * val1, bool bQuery=true );
int setMOTIONPLAN_VELOCITY( uint16 val0
,f32 val1 );
int getMOTIONPLAN_VELOCITY( uint16 val0, f32 * val1, bool bQuery=true );
int setMOTIONPLAN_TIME( uint16 val0
,f32 val1 );
int getMOTIONPLAN_TIME( uint16 val0, f32 * val1, bool bQuery=true );
int setMOTIONPLAN_EXECUTEMODE( byte val0
,MRQ_MOTIONPLAN_EXECUTEMODE val1 );
int getMOTIONPLAN_EXECUTEMODE( byte val0, MRQ_MOTIONPLAN_EXECUTEMODE * val1, bool bQuery=true );
int setMOTIONPLAN_PLANMODE( byte val0
,MRQ_MOTIONPLAN_PLANMODE val1 );
int getMOTIONPLAN_PLANMODE( byte val0, MRQ_MOTIONPLAN_PLANMODE * val1, bool bQuery=true );
int setMOTIONPLAN_MOTIONMODE( byte val0
,MRQ_MOTIONPLAN_MOTIONMODE val1 );
int getMOTIONPLAN_MOTIONMODE( byte val0, MRQ_MOTIONPLAN_MOTIONMODE * val1, bool bQuery=true );
int setMOTIONPLAN_MODIFYDUTY( byte val0
,MRQ_MOTIONPLAN_MODIFYDUTY val1 );
int getMOTIONPLAN_MODIFYDUTY( byte val0, MRQ_MOTIONPLAN_MODIFYDUTY * val1, bool bQuery=true );
int getMOTIONPLAN_REMAINPOINT( byte val0, uint16 * val1, bool bQuery=true );
int getMOTIONPLAN_OUTPUTPOINT( byte val0, uint16 * val1, bool bQuery=true );
int setMOTIONPLAN_STARTPOINT( byte val0
,uint16 val1 );
int getMOTIONPLAN_STARTPOINT( byte val0, uint16 * val1, bool bQuery=true );
int setMOTIONPLAN_ENDPOINT( byte val0
,uint16 val1 );
int getMOTIONPLAN_ENDPOINT( byte val0, uint16 * val1, bool bQuery=true );
int setMOTIONPLAN_CYCLENUM( byte val0
,uint32 val1 );
int getMOTIONPLAN_CYCLENUM( byte val0, uint32 * val1, bool bQuery=true );
int setMOTIONPLAN_WARNPOINT( byte val0
,uint16 val1 );
int getMOTIONPLAN_WARNPOINT( byte val0, uint16 * val1, bool bQuery=true );
int setMOTIONPLAN_WARN( byte val0 );
int setMOTIONPLAN_ENDSTATE( byte val0
,MRQ_MOTIONPLAN_ENDSTATE val1 );
int getMOTIONPLAN_ENDSTATE( byte val0, MRQ_MOTIONPLAN_ENDSTATE * val1, bool bQuery=true );
int setMOTIONPLAN_FEEDBACKRATIO( byte val0
,byte val1 );
int getMOTIONPLAN_FEEDBACKRATIO( byte val0, byte * val1, bool bQuery=true );
int setREPORT_STATE( byte val0
,MRQ_REPORT_STATE val1
,MRQ_SYSTEM_REPORTSWITCH val2 );
int getREPORT_STATE( byte val0
,MRQ_REPORT_STATE val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery=true );
int setREPORT_PERIOD( byte val0
,MRQ_REPORT_STATE val1
,uint32 val2 );
int getREPORT_PERIOD( byte val0
,MRQ_REPORT_STATE val1, uint32 * val2, bool bQuery=true );
int getREPORT_DATA( byte val0
,MRQ_REPORT_STATE val1, uint32 * val2, bool bQuery=true );
int setTRIGGER_MODE( byte val0
,MRQ_TRIGGER_MODE val1 );
int getTRIGGER_MODE( byte val0, MRQ_TRIGGER_MODE * val1, bool bQuery=true );
int setTRIGGER_PATTSTATE( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 );
int getTRIGGER_PATTSTATE( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery=true );
int setTRIGGER_PATTERN( byte val0
,MRQ_TRIGGER_PATTERN val1
,MRQ_TRIGGER_PATTERN val2
,MRQ_TRIGGER_PATTERN val3
,MRQ_TRIGGER_PATTERN val4
,MRQ_TRIGGER_PATTERN val5 );
int getTRIGGER_PATTERN( byte val0, MRQ_TRIGGER_PATTERN * val1, MRQ_TRIGGER_PATTERN * val2, MRQ_TRIGGER_PATTERN * val3, MRQ_TRIGGER_PATTERN * val4, MRQ_TRIGGER_PATTERN * val5, bool bQuery=true );
int setTRIGGER_PATTRESP( byte val0
,MRQ_OUTOFSTEP_LINERESPONSE val1 );
int getTRIGGER_PATTRESP( byte val0, MRQ_OUTOFSTEP_LINERESPONSE * val1, bool bQuery=true );
int setTRIGGER_PATTSMODE( byte val0
,MRQ_TRIGGER_PATTSMODE val1 );
int getTRIGGER_PATTSMODE( byte val0, MRQ_TRIGGER_PATTSMODE * val1, bool bQuery=true );
int setTRIGGER_PATTSPERIOD( byte val0
,uint32 val1 );
int getTRIGGER_PATTSPERIOD( byte val0, uint32 * val1, bool bQuery=true );
int setTRIGGER_LEVELSTATE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,MRQ_SYSTEM_REPORTSWITCH val2 );
int getTRIGGER_LEVELSTATE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery=true );
int setTRIGGER_LEVELTYPE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,MRQ_TRIGGER_LEVELTYPE_1 val2 );
int getTRIGGER_LEVELTYPE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, MRQ_TRIGGER_LEVELTYPE_1 * val2, bool bQuery=true );
int setTRIGGER_LEVELRESP( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,MRQ_OUTOFSTEP_LINERESPONSE val2 );
int getTRIGGER_LEVELRESP( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, MRQ_OUTOFSTEP_LINERESPONSE * val2, bool bQuery=true );
int setTRIGGER_LEVELSMODE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,MRQ_TRIGGER_PATTSMODE val2 );
int getTRIGGER_LEVELSMODE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, MRQ_TRIGGER_PATTSMODE * val2, bool bQuery=true );
int setTRIGGER_LEVELSPERIOD( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,f32 val2 );
int getTRIGGER_LEVELSPERIOD( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, f32 * val2, bool bQuery=true );
int getDRIVER_TYPE( byte val0, MRQ_DRIVER_TYPE * val1, bool bQuery=true );
int getDRIVER_STATEREG( byte val0
,MRQ_DRIVER_STATEREG val1, uint32 * val2, bool bQuery=true );
int setDRIVER_CURRENT( byte val0
,byte val1 );
int getDRIVER_CURRENT( byte val0, byte * val1, bool bQuery=true );
int setDRIVER_MICROSTEPS( byte val0
,MRQ_DRIVER_MICROSTEPS val1 );
int getDRIVER_MICROSTEPS( byte val0, MRQ_DRIVER_MICROSTEPS * val1, bool bQuery=true );
int setDRIVER_SWITCH( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 );
int getDRIVER_SWITCH( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery=true );
int setDRIVER_REGCONFIG( byte val0
,MRQ_DRIVER_REGCONFIG val1
,uint32 val2 );
int getDRIVER_REGCONFIG( byte val0
,MRQ_DRIVER_REGCONFIG val1, uint32 * val2, bool bQuery=true );
int setDRIVER_SGLIMIT( byte val0
,MRQ_DRIVER_SGLIMIT val1
,int8 val2 );
int getDRIVER_SGLIMIT( byte val0
,MRQ_DRIVER_SGLIMIT val1, int8 * val2, bool bQuery=true );
int setDRIVER_SGPARASET( byte val0
,MRQ_DRIVER_SGPARASET val1
,int16 val2 );
int getDRIVER_SGPARASET( byte val0
,MRQ_DRIVER_SGPARASET val1, int16 * val2, bool bQuery=true );
int setDIGITALOUT_STATE( MRQ_DIGITALOUT_STATE val0
,MRQ_DIGITALOUT_STATE_1 val1 );
int getDIGITALOUT_STATE( MRQ_DIGITALOUT_STATE val0, MRQ_DIGITALOUT_STATE_1 * val1, bool bQuery=true );
int setDIGITALOUT_SIGNAL( MRQ_DIGITALOUT_STATE val0
,MRQ_DIGITALOUT_SIGNAL_1 val1 );
int getDIGITALOUT_SIGNAL( MRQ_DIGITALOUT_STATE val0, MRQ_DIGITALOUT_SIGNAL_1 * val1, bool bQuery=true );
int setDIGITALOUT_POLARITY( MRQ_DIGITALOUT_STATE val0
,MRQ_DIGITALOUT_POLARITY_1 val1 );
int getDIGITALOUT_POLARITY( MRQ_DIGITALOUT_STATE val0, MRQ_DIGITALOUT_POLARITY_1 * val1, bool bQuery=true );
int setDIGITALOUT_SOURCE( MRQ_DIGITALOUT_STATE val0
,byte val1 );
int getDIGITALOUT_SOURCE( MRQ_DIGITALOUT_STATE val0, byte * val1, bool bQuery=true );
int setDIGITALOUT_CONDITION( MRQ_DIGITALOUT_STATE val0
,MRQ_DIGITALOUT_CONDITION_1 val1 );
int getDIGITALOUT_CONDITION( MRQ_DIGITALOUT_STATE val0, MRQ_DIGITALOUT_CONDITION_1 * val1, bool bQuery=true );
int setDIGITALOUT_PERIOD( MRQ_DIGITALOUT_STATE val0
,uint32 val1 );
int getDIGITALOUT_PERIOD( MRQ_DIGITALOUT_STATE val0, uint32 * val1, bool bQuery=true );
int setDIGITALOUT_DUTY( MRQ_DIGITALOUT_STATE val0
,uint16 val1 );
int getDIGITALOUT_DUTY( MRQ_DIGITALOUT_STATE val0, uint16 * val1, bool bQuery=true );
int setISOLATOROUTPUT_STATE( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_DIGITALOUT_STATE_1 val1 );
int getISOLATOROUTPUT_STATE( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_DIGITALOUT_STATE_1 * val1, bool bQuery=true );
int setISOLATOROUTPUT_SOURCE( MRQ_ISOLATOROUTPUT_STATE val0
,byte val1 );
int getISOLATOROUTPUT_SOURCE( MRQ_ISOLATOROUTPUT_STATE val0, byte * val1, bool bQuery=true );
int setISOLATOROUTPUT_CONDITION( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_DIGITALOUT_CONDITION_1 val1 );
int getISOLATOROUTPUT_CONDITION( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_DIGITALOUT_CONDITION_1 * val1, bool bQuery=true );
int setISOLATOROUTPUT_RESPONSE( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_ISOLATOROUTPUT_RESPONSE_1 val1 );
int getISOLATOROUTPUT_RESPONSE( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_ISOLATOROUTPUT_RESPONSE_1 * val1, bool bQuery=true );
int setSENSORUART_BAUD( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_BAUD val1 );
int getSENSORUART_BAUD( MRQ_SENSORUART_BAUD val0, MRQ_RS232_BAUD * val1, bool bQuery=true );
int setSENSORUART_WORDLEN( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_WORDLEN val1 );
int getSENSORUART_WORDLEN( MRQ_SENSORUART_BAUD val0, MRQ_RS232_WORDLEN * val1, bool bQuery=true );
int setSENSORUART_FLOWCTL( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_FLOWCTL val1 );
int getSENSORUART_FLOWCTL( MRQ_SENSORUART_BAUD val0, MRQ_RS232_FLOWCTL * val1, bool bQuery=true );
int setSENSORUART_PARITY( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_PARITY val1 );
int getSENSORUART_PARITY( MRQ_SENSORUART_BAUD val0, MRQ_RS232_PARITY * val1, bool bQuery=true );
int setSENSORUART_STOPBIT( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_STOPBIT val1 );
int getSENSORUART_STOPBIT( MRQ_SENSORUART_BAUD val0, MRQ_RS232_STOPBIT * val1, bool bQuery=true );
int setSENSORUART_APPLYPARA( MRQ_SENSORUART_BAUD val0 );
int setSENSORUART_STATE( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,MRQ_SYSTEM_REPORTSWITCH val2 );
int getSENSORUART_STATE( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery=true );
int setSENSORUART_SOF( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,byte val2 );
int getSENSORUART_SOF( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, byte * val2, bool bQuery=true );
int setSENSORUART_FRAMELEN( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,byte val2 );
int getSENSORUART_FRAMELEN( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, byte * val2, bool bQuery=true );
int setSENSORUART_RECEIVENUM( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,byte val2 );
int getSENSORUART_RECEIVENUM( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, byte * val2, bool bQuery=true );
int setSENSORUART_SWITCHTIME( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,uint32 val2 );
int getSENSORUART_SWITCHTIME( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, uint32 * val2, bool bQuery=true );
int getSENSORUART_DATA( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, byte * val2, char * val3, char * val4, char * val5, bool bQuery=true );
int setPRESET_PVTCONFIG( byte val0
,MRQ_PRESET_PVTCONFIG val1
,MRQ_MOTIONPLAN_PVTCONFIG val2 );
int getPRESET_PVTCONFIG( byte val0
,MRQ_PRESET_PVTCONFIG val1, MRQ_MOTIONPLAN_PVTCONFIG * val2, bool bQuery=true );
int setPRESET_POSITION( byte val0
,byte val1
,f32 val2 );
int getPRESET_POSITION( byte val0
,byte val1, f32 * val2, bool bQuery=true );
int setPRESET_SPEED( byte val0
,byte val1
,f32 val2 );
int getPRESET_SPEED( byte val0
,byte val1, f32 * val2, bool bQuery=true );
int setPRESET_TIME( byte val0
,byte val1
,f32 val2 );
int getPRESET_TIME( byte val0
,byte val1, f32 * val2, bool bQuery=true );
int getPRESET_REMAINPOINT( byte val0
,MRQ_PRESET_PVTCONFIG val1, byte * val2, bool bQuery=true );
int setPRESET_EXECUTE( byte val0
,MRQ_PRESET_PVTCONFIG val1
,MRQ_PRESET_EXECUTE_1 val2 );
int getPRESET_STATE( byte val0
,MRQ_PRESET_PVTCONFIG val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery=true );
int setPRESET_ENDSTATE( byte val0
,MRQ_PRESET_PVTCONFIG val1
,MRQ_MOTIONPLAN_ENDSTATE val2 );
int getPRESET_ENDSTATE( byte val0
,MRQ_PRESET_PVTCONFIG val1, MRQ_MOTIONPLAN_ENDSTATE * val2, bool bQuery=true );
int setPRESET_STOPMODE( byte val0
,MRQ_PRESET_PVTCONFIG val1
,MRQ_STOPDECEL_MODE val2 );
int getPRESET_STOPMODE( byte val0
,MRQ_PRESET_PVTCONFIG val1, MRQ_STOPDECEL_MODE * val2, bool bQuery=true );
int setPRESET_STOPDISTANCE( byte val0
,MRQ_PRESET_PVTCONFIG val1
,f32 val2 );
int getPRESET_STOPDISTANCE( byte val0
,MRQ_PRESET_PVTCONFIG val1, f32 * val2, bool bQuery=true );
int setOTP_STATE( MRQ_SYSTEM_REPORTSWITCH val0 );
int getOTP_STATE(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery=true );
int setOTP_THRESHOLD( uint16 val0 );
int getOTP_THRESHOLD(  uint16 * val0, bool bQuery=true );
int setOTP_RESPONSE( MRQ_OUTOFSTEP_LINERESPONSE val0 );
int getOTP_RESPONSE(  MRQ_OUTOFSTEP_LINERESPONSE * val0, bool bQuery=true );
int setOTP_PERIOD( uint32 val0 );
int getOTP_PERIOD(  uint32 * val0, bool bQuery=true );
int getOTP_DATA(  uint32 * val0, bool bQuery=true );
int setANALOGIN_STATE( MRQ_SYSTEM_REPORTSWITCH val0 );
int getANALOGIN_STATE(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery=true );
int setANALOGIN_THRESHOLDH( f32 val0 );
int getANALOGIN_THRESHOLDH(  f32 * val0, bool bQuery=true );
int setANALOGIN_THRESHOLDL( f32 val0 );
int getANALOGIN_THRESHOLDL(  f32 * val0, bool bQuery=true );
int setANALOGIN_RESPONSEH( MRQ_OUTOFSTEP_LINERESPONSE val0 );
int getANALOGIN_RESPONSEH(  MRQ_OUTOFSTEP_LINERESPONSE * val0, bool bQuery=true );
int setANALOGIN_RESPONSEL( MRQ_OUTOFSTEP_LINERESPONSE val0 );
int getANALOGIN_RESPONSEL(  MRQ_OUTOFSTEP_LINERESPONSE * val0, bool bQuery=true );
void loadOtp();
public:
	MRQ_LINK_INTFC mLINK_INTFC;
	MRQ_SYSTEM_WORKMODE mSYSTEM_WORKMODE;
	MRQ_SYSTEM_POWERON mSYSTEM_POWERON;
	byte mSYSTEM_SN;

	char mSYSTEM_SN1;
	char mSYSTEM_SN2;
	char mSYSTEM_SN3;
	char mSYSTEM_SN4;

	char mSYSTEM_SN5;
	MRQ_SYSTEM_TYPE mSYSTEM_TYPE;
	MRQ_SYSTEM_TYPE_1 mSYSTEM_TYPE1;
	char mSYSTEM_SOFTVER;

	char mSYSTEM_SOFTVER1;
	char mSYSTEM_SOFTVER2;
	char mSYSTEM_FPGAVER;
	char mSYSTEM_FPGAVER1;

	char mSYSTEM_FPGAVER2;
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
	MRQ_SYSTEM_REPORTSWITCH mSYSTEM_REPORTSWITCH;

	MRQ_RS232_BAUD mRS232_BAUD;
	MRQ_RS232_WORDLEN mRS232_WORDLEN;
	MRQ_RS232_FLOWCTL mRS232_FLOWCTL;
	MRQ_RS232_PARITY mRS232_PARITY;

	MRQ_RS232_STOPBIT mRS232_STOPBIT;
	MRQ_CAN_TYPE mCAN_TYPE;
	MRQ_CAN_BAUD mCAN_BAUD;
	byte mCAN_GROUP;

	uint32 mCAN_SENDID;
	uint32 mCAN_RECEIVEID;
	uint32 mCAN_GROUPID1;
	uint32 mCAN_GROUPID2;

	uint32 mCAN_BROADCASTID;
	MRQ_CAN_NETMANAGESTATE mCAN_NETMANAGESTATE;
	uint32 mCAN_NETMANAGEID[3];
	uint32 mCAN_NETMANAGEHASH;

	uint32 mCLOCK_FREQUENCY;
	MRQ_SYSTEM_REPORTSWITCH mCLOCK_SYNCREGISTER;
	MRQ_CLOCK_STARTTYPE mCLOCK_STARTTYPE;
	MRQ_CLOCK_SYNCSTATE mCLOCK_SYNCSTATE;

	uint32 mCLOCK_COUNT;
	MRQ_MOTION_STATE mMOTION_STATE[4];
	MRQ_MOTION_STATEREPORT mMOTION_STATEREPORT[4];
	MRQ_MOTION_STARTSOURCE mMOTION_STARTSOURCE[4];

	MRQ_CLOCK_STARTTYPE mMOTION_STARTTYPE[4];
	f32 mMOTION_MAXSPEED[4];
	f32 mMOTION_MINSPEED[4];
	f32 mMOTION_MAXPOSITION[4];

	f32 mMOTION_MINPOSITION[4];
	f32 mMOTION_MAXTORQUE[4];
	f32 mMOTION_MINTORQUE[4];
	f32 mMOTION_MAXACCELERATION[4];

	f32 mMOTION_MINACCELERATION[4];
	uint32 mMOTION_ORIGIN[4];
	MRQ_MOTION_INITPOSITIONUNIT mMOTION_INITPOSITIONUNIT[4];
	f32 mMOTION_INITPOSITION[4];

	MRQ_MOTION_INITIOSIGNAL mMOTION_INITIOSIGNAL[4];
	MRQ_MOTION_INCHINGMODE mMOTION_INCHINGMODE[4];
	f32 mMOTION_INCHINGTIME[4];
	MRQ_SYSTEM_REPORTSWITCH mMOTION_OFFSETSTATE[4];

	int32 mMOTION_PVTSTEPS[4];
	int32 mMOTION_COUNTSTEPS[4];
	int16 mMOTION_PVTCIRCLE[4];
	int16 mMOTION_COUNTCIRCLE[4];

	int32 mMOTION_ABCOUNT[4];
	MRQ_SYSTEM_REPORTSWITCH mMOTION_REVMOTION[4];
	byte mIDENTITY_GROUP[4][2];
	MRQ_SYSTEM_REPORTSWITCH mIDENTITY_DISTDEVICE;

	MRQ_STOPDECEL_MODE mSTOPDECEL_MODE[4];
	f32 mSTOPDECEL_DISTANCE[4];
	f32 mSTOPDECEL_TIME[4];
	MRQ_SYSTEM_REPORTSWITCH mOUTOFSTEP_LINESTATE[4];

	uint32 mOUTOFSTEP_LINEOUTNUM[4];
	MRQ_OUTOFSTEP_LINERESPONSE mOUTOFSTEP_LINERESPONSE[4];
	MRQ_SYSTEM_REPORTSWITCH mOUTOFSTEP_TOTALSTATE[4];
	uint32 mOUTOFSTEP_TOTALOUTNUM[4];

	MRQ_OUTOFSTEP_LINERESPONSE mOUTOFSTEP_TOTALRESPONSE[4];
	MRQ_MOTOR_STEPANGLE mMOTOR_STEPANGLE[4];
	MRQ_MOTOR_TYPE mMOTOR_TYPE[4];
	MRQ_MOTOR_POSITIONUNIT mMOTOR_POSITIONUNIT[4];

	uint16 mMOTOR_GEARRATIONUM[4];
	uint16 mMOTOR_GEARRATIODEN[4];
	f32 mMOTOR_LEAD[4];
	f32 mMOTOR_PEAKSPEED[4];

	f32 mMOTOR_PEAKACCELERATION[4];
	MRQ_MOTOR_SIZE mMOTOR_SIZE[4];
	byte mMOTOR_VOLTAGE[4];
	byte mMOTOR_CURRENT[4];

	f32 mMOTOR_BACKLASH[4];
	uint32 mENCODER_LINENUM[4];
	MRQ_ENCODER_CHANNELNUM mENCODER_CHANNELNUM[4];
	MRQ_ENCODER_TYPE mENCODER_TYPE[4];

	MRQ_ENCODER_MULTIPLE mENCODER_MULTIPLE[4];
	MRQ_ENCODER_STATE mENCODER_STATE[4];
	MRQ_MOTIONPLAN_PVTCONFIG mMOTIONPLAN_PVTCONFIG[4];
	f32 mMOTIONPLAN_POSITION[4];

	f32 mMOTIONPLAN_VELOCITY[4];
	f32 mMOTIONPLAN_TIME[4];
	MRQ_MOTIONPLAN_EXECUTEMODE mMOTIONPLAN_EXECUTEMODE[4];
	MRQ_MOTIONPLAN_PLANMODE mMOTIONPLAN_PLANMODE[4];

	MRQ_MOTIONPLAN_MOTIONMODE mMOTIONPLAN_MOTIONMODE[4];
	MRQ_MOTIONPLAN_MODIFYDUTY mMOTIONPLAN_MODIFYDUTY[4];
	uint16 mMOTIONPLAN_REMAINPOINT[4];
	uint16 mMOTIONPLAN_OUTPUTPOINT[4];

	uint16 mMOTIONPLAN_STARTPOINT[4];
	uint16 mMOTIONPLAN_ENDPOINT[4];
	uint32 mMOTIONPLAN_CYCLENUM[4];
	uint16 mMOTIONPLAN_WARNPOINT[4];

	MRQ_MOTIONPLAN_ENDSTATE mMOTIONPLAN_ENDSTATE[4];
	byte mMOTIONPLAN_FEEDBACKRATIO[4];
	MRQ_SYSTEM_REPORTSWITCH mREPORT_STATE[4][14];
	uint32 mREPORT_PERIOD[4][14];

	uint32 mREPORT_DATA[4][14];
	MRQ_TRIGGER_MODE mTRIGGER_MODE[4];
	MRQ_SYSTEM_REPORTSWITCH mTRIGGER_PATTSTATE[4];
	MRQ_TRIGGER_PATTERN mTRIGGER_PATTERN[4];

	MRQ_TRIGGER_PATTERN mTRIGGER_PATTERN1[4];
	MRQ_TRIGGER_PATTERN mTRIGGER_PATTERN2[4];
	MRQ_TRIGGER_PATTERN mTRIGGER_PATTERN3[4];
	MRQ_TRIGGER_PATTERN mTRIGGER_PATTERN4[4];

	MRQ_OUTOFSTEP_LINERESPONSE mTRIGGER_PATTRESP[4];
	MRQ_TRIGGER_PATTSMODE mTRIGGER_PATTSMODE[4];
	uint32 mTRIGGER_PATTSPERIOD[4];
	MRQ_SYSTEM_REPORTSWITCH mTRIGGER_LEVELSTATE[4][5];

	MRQ_TRIGGER_LEVELTYPE_1 mTRIGGER_LEVELTYPE[4][5];
	MRQ_OUTOFSTEP_LINERESPONSE mTRIGGER_LEVELRESP[4][5];
	MRQ_TRIGGER_PATTSMODE mTRIGGER_LEVELSMODE[4][5];
	f32 mTRIGGER_LEVELSPERIOD[4][5];

	MRQ_DRIVER_TYPE mDRIVER_TYPE[4];
	uint32 mDRIVER_STATEREG[4][3];
	byte mDRIVER_CURRENT[4];
	MRQ_DRIVER_MICROSTEPS mDRIVER_MICROSTEPS[4];

	MRQ_SYSTEM_REPORTSWITCH mDRIVER_SWITCH[4];
	uint32 mDRIVER_REGCONFIG[4][5];
	int8 mDRIVER_SGLIMIT[4][2];
	int16 mDRIVER_SGPARASET[4][4];

	MRQ_DIGITALOUT_STATE_1 mDIGITALOUT_STATE[2];
	MRQ_DIGITALOUT_SIGNAL_1 mDIGITALOUT_SIGNAL[2];
	MRQ_DIGITALOUT_POLARITY_1 mDIGITALOUT_POLARITY[2];
	byte mDIGITALOUT_SOURCE[2];

	MRQ_DIGITALOUT_CONDITION_1 mDIGITALOUT_CONDITION[2];
	uint32 mDIGITALOUT_PERIOD[2];
	uint16 mDIGITALOUT_DUTY[2];
	MRQ_DIGITALOUT_STATE_1 mISOLATOROUTPUT_STATE[2];

	byte mISOLATOROUTPUT_SOURCE[2];
	MRQ_DIGITALOUT_CONDITION_1 mISOLATOROUTPUT_CONDITION[2];
	MRQ_ISOLATOROUTPUT_RESPONSE_1 mISOLATOROUTPUT_RESPONSE[2];
	MRQ_RS232_BAUD mSENSORUART_BAUD[2];

	MRQ_RS232_WORDLEN mSENSORUART_WORDLEN[2];
	MRQ_RS232_FLOWCTL mSENSORUART_FLOWCTL[2];
	MRQ_RS232_PARITY mSENSORUART_PARITY[2];
	MRQ_RS232_STOPBIT mSENSORUART_STOPBIT[2];

	MRQ_SYSTEM_REPORTSWITCH mSENSORUART_STATE[2][2];
	byte mSENSORUART_SOF[2][2];
	byte mSENSORUART_FRAMELEN[2][2];
	byte mSENSORUART_RECEIVENUM[2][2];

	uint32 mSENSORUART_SWITCHTIME[2][2];
	byte mSENSORUART_DATA[2][2];
	char mSENSORUART_DATA1[2][2];
	char mSENSORUART_DATA2[2][2];

	char mSENSORUART_DATA3[2][2];
	MRQ_MOTIONPLAN_PVTCONFIG mPRESET_PVTCONFIG[4][2];
	f32 mPRESET_POSITION[4][4];
	f32 mPRESET_SPEED[4][4];

	f32 mPRESET_TIME[4][4];
	byte mPRESET_REMAINPOINT[4][2];
	MRQ_SYSTEM_REPORTSWITCH mPRESET_STATE[4][2];
	MRQ_MOTIONPLAN_ENDSTATE mPRESET_ENDSTATE[4][2];

	MRQ_STOPDECEL_MODE mPRESET_STOPMODE[4][2];
	f32 mPRESET_STOPDISTANCE[4][2];
	MRQ_SYSTEM_REPORTSWITCH mOTP_STATE;
	uint16 mOTP_THRESHOLD;

	MRQ_OUTOFSTEP_LINERESPONSE mOTP_RESPONSE;
	uint32 mOTP_PERIOD;
	uint32 mOTP_DATA;
	MRQ_SYSTEM_REPORTSWITCH mANALOGIN_STATE;

	f32 mANALOGIN_THRESHOLDH;
	f32 mANALOGIN_THRESHOLDL;
	MRQ_OUTOFSTEP_LINERESPONSE mANALOGIN_RESPONSEH;
	MRQ_OUTOFSTEP_LINERESPONSE mANALOGIN_RESPONSEL;

};
}
#endif
