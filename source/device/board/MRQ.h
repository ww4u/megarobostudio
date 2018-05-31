#ifndef _MRQ_H_
#define _MRQ_H_
#include "MRQ_model.h"

namespace MegaDevice
{
class MRQ :  public MRQ_model
{
public:
MRQ();
int setLINK_INTFC( MRQ_LINK_INTFC val0 );
int getLINK_INTFC(  MRQ_LINK_INTFC * val0, bool bQuery=true );
int getLINK_DEVICEINFO(  MRQ_LINK_DEVICEINFO * val0, MRQ_LINK_DEVICEINFO_1 * val1, uint32 * val2, bool bQuery=true );
int setSYSTEM_WORKMODE( MRQ_SYSTEM_WORKMODE val0 );
int getSYSTEM_WORKMODE(  MRQ_SYSTEM_WORKMODE * val0, bool bQuery=true );
int setSYSTEM_POWERON( MRQ_SYSTEM_POWERON val0 );
int getSYSTEM_POWERON(  MRQ_SYSTEM_POWERON * val0, bool bQuery=true );
int getSYSTEM_SN(  byte * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery=true );
int getSYSTEM_TYPE(  MRQ_LINK_DEVICEINFO * val0, MRQ_LINK_DEVICEINFO_1 * val1, bool bQuery=true );
int getSYSTEM_SOFTVER(  char * val0, char * val1, char * val2, bool bQuery=true );
int getSYSTEM_FPGAVER(  char * val0, char * val1, char * val2, bool bQuery=true );
int getSYSTEM_HARDVER(  char * val0, char * val1, bool bQuery=true );
int getSYSTEM_BOOTVER(  char * val0, char * val1, bool bQuery=true );
int getSYSTEM_EVENTCODE(  char * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery=true );
int getSYSTEM_DIOSTATE(  uint16 * val0, bool bQuery=true );
int setSYSTEM_FANPARA( byte val0 );
int getSYSTEM_FANPARA(  byte * val0, bool bQuery=true );
int setSYSTEM_ARMLEDPARA( MRQ_SYSTEM_ARMLEDPARA val0
,byte val1 );
int getSYSTEM_ARMLEDPARA( MRQ_SYSTEM_ARMLEDPARA val0, byte * val1, bool bQuery=true );
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
int setCAN_NETMANAGELED( MRQ_CAN_NETMANAGELED val0 );
int setCAN_NETMANAGESTATE( MRQ_CAN_NETMANAGESTATE val0 );
int getCAN_NETMANAGESTATE(  MRQ_CAN_NETMANAGESTATE * val0, bool bQuery=true );
int setCAN_NETMANAGEID( MRQ_CAN_NETMANAGEID val0
,uint32 val1 );
int getCAN_NETMANAGEID( MRQ_CAN_NETMANAGEID val0, uint32 * val1, bool bQuery=true );
int setCAN_NETMANAGEHASH( uint32 val0 );
int getCAN_NETMANAGEHASH(  uint32 * val0, bool bQuery=true );
int setCLOCK_FREQUENCY( uint32 val0 );
int getCLOCK_FREQUENCY(  uint32 * val0, bool bQuery=true );
int setCLOCK_SYNCREGISTER( MRQ_CAN_NETMANAGELED val0 );
int getCLOCK_SYNCREGISTER(  MRQ_CAN_NETMANAGELED * val0, bool bQuery=true );
int setCLOCK_STARTTYPE( MRQ_CLOCK_STARTTYPE val0 );
int getCLOCK_STARTTYPE(  MRQ_CLOCK_STARTTYPE * val0, bool bQuery=true );
int setCLOCK_SYNCSTATE( MRQ_CLOCK_SYNCSTATE val0 );
int getCLOCK_SYNCSTATE(  MRQ_CLOCK_SYNCSTATE * val0, bool bQuery=true );
int getCLOCK_COUNT(  uint32 * val0, bool bQuery=true );
int setMOTION_SWITCH( byte val0
,MRQ_MOTION_SWITCH val1
,MRQ_MOTION_SWITCH_1 val2 );
int getMOTION_STATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTION_STATE_2 * val2, bool bQuery=true );
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
int setMOTION_OFFSETSTATE( byte val0
,MRQ_CAN_NETMANAGELED val1 );
int getMOTION_OFFSETSTATE( byte val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery=true );
int setMOTION_OFFSET( byte val0
,int32 val1 );
int getMOTION_COUNTCIRCLE( byte val0, int16 * val1, bool bQuery=true );
int getMOTION_ABCOUNT( byte val0, int32 * val1, bool bQuery=true );
int setMOTION_REVMOTION( byte val0
,MRQ_CAN_NETMANAGELED val1 );
int getMOTION_REVMOTION( byte val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery=true );
int setMOTION_ABCOUNTRESET( byte val0 );
int setIDENTITY_GROUP( byte val0
,MRQ_IDENTITY_GROUP val1
,byte val2 );
int getIDENTITY_GROUP( byte val0
,MRQ_IDENTITY_GROUP val1, byte * val2, bool bQuery=true );
int setIDENTITY_DISTDEVICE( MRQ_CAN_NETMANAGELED val0 );
int getIDENTITY_DISTDEVICE(  MRQ_CAN_NETMANAGELED * val0, bool bQuery=true );
int setIDENTITY_LABEL( byte val0
,MRQ_IDENTITY_LABEL val1
,MRQ_IDENTITY_LABEL_1 val2 );
int getIDENTITY_LABEL( byte val0, MRQ_IDENTITY_LABEL * val1, MRQ_IDENTITY_LABEL_1 * val2, bool bQuery=true );
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
int setENCODER_FEEDBACKRATIO( byte val0
,byte val1 );
int getENCODER_FEEDBACKRATIO( byte val0, byte * val1, bool bQuery=true );
int setMOTIONPLAN_PVTCONFIG( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_PVTCONFIG_1 val2 );
int getMOTIONPLAN_PVTCONFIG( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_PVTCONFIG_1 * val2, bool bQuery=true );
int setMOTIONPLAN_PRESETSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_PRESETSTATE_1 val2 );
int getMOTIONPLAN_PRESETSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_PRESETSTATE_1 * val2, bool bQuery=true );
int setMOTIONPLAN_EXECUTEMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_EXECUTEMODE_1 val2 );
int getMOTIONPLAN_EXECUTEMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_EXECUTEMODE_1 * val2, bool bQuery=true );
int setMOTIONPLAN_PLANMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_PLANMODE_1 val2 );
int getMOTIONPLAN_PLANMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_PLANMODE_1 * val2, bool bQuery=true );
int setMOTIONPLAN_MOTIONMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_MOTIONMODE_1 val2 );
int getMOTIONPLAN_MOTIONMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_MOTIONMODE_1 * val2, bool bQuery=true );
int setMOTIONPLAN_MODIFYDUTY( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_MODIFYDUTY_1 val2 );
int getMOTIONPLAN_MODIFYDUTY( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_MODIFYDUTY_1 * val2, bool bQuery=true );
int getMOTIONPLAN_REMAINPOINT( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint16 * val2, bool bQuery=true );
int setMOTIONPLAN_CYCLENUM( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint32 val2 );
int getMOTIONPLAN_CYCLENUM( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint32 * val2, bool bQuery=true );
int setMOTIONPLAN_WARNPOINT( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2 );
int getMOTIONPLAN_WARNPOINT( byte val0
,MRQ_MOTION_SWITCH_1 val1, byte * val2, bool bQuery=true );
int setMOTIONPLAN_ENDSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_ENDSTATE_1 val2 );
int getMOTIONPLAN_ENDSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_ENDSTATE_1 * val2, bool bQuery=true );
int getMOTIONPLAN_BUFFERSIZE( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint32 * val2, bool bQuery=true );
int setMOTIONPLAN_ACCSCALE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint16 val2 );
int getMOTIONPLAN_ACCSCALE( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint16 * val2, bool bQuery=true );
int setMOTIONPLAN_DECSCALE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint16 val2 );
int getMOTIONPLAN_DECSCALE( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint16 * val2, bool bQuery=true );
int setMOTIONPLAN_STOPMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_STOPMODE_1 val2 );
int getMOTIONPLAN_STOPMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_STOPMODE_1 * val2, bool bQuery=true );
int setMOTIONPLAN_STOPDISTANCE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,f32 val2 );
int getMOTIONPLAN_STOPDISTANCE( byte val0
,MRQ_MOTION_SWITCH_1 val1, f32 * val2, bool bQuery=true );
int setMOTIONPLAN_STOPTIME( byte val0
,MRQ_MOTION_SWITCH_1 val1
,f32 val2 );
int getMOTIONPLAN_STOPTIME( byte val0
,MRQ_MOTION_SWITCH_1 val1, f32 * val2, bool bQuery=true );
int setMOTIONPLAN_OOSLINESTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_CAN_NETMANAGELED val2 );
int getMOTIONPLAN_OOSLINESTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery=true );
int setMOTIONPLAN_OOSLINEOUTNUM( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint16 val2 );
int getMOTIONPLAN_OOSLINEOUTNUM( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint16 * val2, bool bQuery=true );
int setMOTIONPLAN_OOSLINERESPONSE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val2 );
int getMOTIONPLAN_OOSLINERESPONSE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val2, bool bQuery=true );
int setMOTIONPLAN_OOSTOTALSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_CAN_NETMANAGELED val2 );
int getMOTIONPLAN_OOSTOTALSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery=true );
int setMOTIONPLAN_OOSTOTALOUTNUM( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint32 val2 );
int getMOTIONPLAN_OOSTOTALOUTNUM( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint32 * val2, bool bQuery=true );
int setMOTIONPLAN_OOSTOTALRESPONSE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val2 );
int getMOTIONPLAN_OOSTOTALRESPONSE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val2, bool bQuery=true );
int setPOSITION( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2
,f32 val3 );
int getPOSITION( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2, f32 * val3, bool bQuery=true );
int setVELOCITY( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2
,f32 val3 );
int getVELOCITY( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2, f32 * val3, bool bQuery=true );
int setTIME( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2
,f32 val3 );
int getTIME( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2, f32 * val3, bool bQuery=true );
int setREPORT_STATE( byte val0
,MRQ_REPORT_STATE val1
,MRQ_CAN_NETMANAGELED val2 );
int getREPORT_STATE( byte val0
,MRQ_REPORT_STATE val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery=true );
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
,MRQ_CAN_NETMANAGELED val1 );
int getTRIGGER_PATTSTATE( byte val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery=true );
int setTRIGGER_PATTERN( byte val0
,MRQ_TRIGGER_PATTERN val1
,MRQ_TRIGGER_PATTERN val2 );
int getTRIGGER_PATTERN( byte val0, MRQ_TRIGGER_PATTERN * val1, MRQ_TRIGGER_PATTERN * val2, bool bQuery=true );
int setTRIGGER_PATTRESP( byte val0
,MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val1 );
int getTRIGGER_PATTRESP( byte val0, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val1, bool bQuery=true );
int setTRIGGER_PATTSMODE( byte val0
,MRQ_TRIGGER_PATTSMODE val1 );
int getTRIGGER_PATTSMODE( byte val0, MRQ_TRIGGER_PATTSMODE * val1, bool bQuery=true );
int setTRIGGER_PATTSPERIOD( byte val0
,uint32 val1 );
int getTRIGGER_PATTSPERIOD( byte val0, uint32 * val1, bool bQuery=true );
int setTRIGGER_LEVELSTATE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_CAN_NETMANAGELED val2 );
int getTRIGGER_LEVELSTATE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery=true );
int setTRIGGER_LEVELTYPE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_TRIGGER_LEVELTYPE_1 val2 );
int getTRIGGER_LEVELTYPE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_TRIGGER_LEVELTYPE_1 * val2, bool bQuery=true );
int setTRIGGER_LEVELRESP( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val2 );
int getTRIGGER_LEVELRESP( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val2, bool bQuery=true );
int setTRIGGER_LEVELSMODE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_TRIGGER_PATTSMODE val2 );
int getTRIGGER_LEVELSMODE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_TRIGGER_PATTSMODE * val2, bool bQuery=true );
int setTRIGGER_LEVELSPERIOD( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,f32 val2 );
int getTRIGGER_LEVELSPERIOD( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, f32 * val2, bool bQuery=true );
int getDRIVER_TYPE( byte val0, MRQ_DRIVER_TYPE * val1, bool bQuery=true );
int getDRIVER_STATEREG( byte val0
,MRQ_DRIVER_STATEREG val1, uint32 * val2, bool bQuery=true );
int setDRIVER_CURRENT( byte val0
,byte val1 );
int getDRIVER_CURRENT( byte val0, byte * val1, bool bQuery=true );
int setDRIVER_MICROSTEPS( byte val0
,MRQ_DRIVER_MICROSTEPS val1 );
int getDRIVER_MICROSTEPS( byte val0, MRQ_DRIVER_MICROSTEPS * val1, bool bQuery=true );
int setDRIVER_STATE( byte val0
,MRQ_CAN_NETMANAGELED val1 );
int getDRIVER_STATE( byte val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery=true );
int setDRIVER_SGLIMIT( byte val0
,MRQ_DRIVER_SGLIMIT val1
,int8 val2 );
int getDRIVER_SGLIMIT( byte val0, MRQ_DRIVER_SGLIMIT * val1, int8 * val2, bool bQuery=true );
int setDRIVER_SGPARASET( byte val0
,MRQ_DRIVER_SGPARASET val1
,int16 val2 );
int getDRIVER_SGPARASET( byte val0
,MRQ_DRIVER_SGPARASET val1, int16 * val2, bool bQuery=true );
int setDRIVER_IDLECURRENT( byte val0
,byte val1 );
int getDRIVER_IDLECURRENT( byte val0, byte * val1, bool bQuery=true );
int setDRIVER_SWITCHTIME( byte val0
,uint32 val1 );
int getDRIVER_SWITCHTIME( byte val0, uint32 * val1, bool bQuery=true );
int setDRIVER_MINICURRRATIO( byte val0
,MRQ_DRIVER_MINICURRRATIO val1 );
int getDRIVER_MINICURRRATIO( byte val0, MRQ_DRIVER_MINICURRRATIO * val1, bool bQuery=true );
int setDRIVER_REGCONFIG( byte val0
,MRQ_DRIVER_REGCONFIG val1
,uint32 val2 );
int getDRIVER_REGCONFIG( byte val0
,MRQ_DRIVER_REGCONFIG val1, uint32 * val2, bool bQuery=true );
int setDIGITALOUTPUT_STATE( MRQ_DIGITALOUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_STATE_1 val1 );
int getDIGITALOUTPUT_STATE( MRQ_DIGITALOUTPUT_STATE val0, MRQ_DIGITALOUTPUT_STATE_1 * val1, bool bQuery=true );
int setDIGITALOUTPUT_SIGNAL( MRQ_DIGITALOUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_SIGNAL_1 val1 );
int getDIGITALOUTPUT_SIGNAL( MRQ_DIGITALOUTPUT_STATE val0, MRQ_DIGITALOUTPUT_SIGNAL_1 * val1, bool bQuery=true );
int setDIGITALOUTPUT_POLARITY( MRQ_DIGITALOUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_POLARITY_1 val1 );
int getDIGITALOUTPUT_POLARITY( MRQ_DIGITALOUTPUT_STATE val0, MRQ_DIGITALOUTPUT_POLARITY_1 * val1, bool bQuery=true );
int setDIGITALOUTPUT_SOURCE( MRQ_DIGITALOUTPUT_STATE val0
,byte val1 );
int getDIGITALOUTPUT_SOURCE( MRQ_DIGITALOUTPUT_STATE val0, byte * val1, bool bQuery=true );
int setDIGITALOUTPUT_CONDITION( MRQ_DIGITALOUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_CONDITION_1 val1 );
int getDIGITALOUTPUT_CONDITION( MRQ_DIGITALOUTPUT_STATE val0, MRQ_DIGITALOUTPUT_CONDITION_1 * val1, bool bQuery=true );
int setDIGITALOUTPUT_PERIOD( MRQ_DIGITALOUTPUT_STATE val0
,uint32 val1 );
int getDIGITALOUTPUT_PERIOD( MRQ_DIGITALOUTPUT_STATE val0, uint32 * val1, bool bQuery=true );
int setDIGITALOUTPUT_DUTY( MRQ_DIGITALOUTPUT_STATE val0
,uint16 val1 );
int getDIGITALOUTPUT_DUTY( MRQ_DIGITALOUTPUT_STATE val0, uint16 * val1, bool bQuery=true );
int setISOLATOROUTPUT_STATE( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_STATE_1 val1 );
int getISOLATOROUTPUT_STATE( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_DIGITALOUTPUT_STATE_1 * val1, bool bQuery=true );
int setISOLATOROUTPUT_SOURCE( MRQ_ISOLATOROUTPUT_STATE val0
,byte val1 );
int getISOLATOROUTPUT_SOURCE( MRQ_ISOLATOROUTPUT_STATE val0, byte * val1, bool bQuery=true );
int setISOLATOROUTPUT_CONDITION( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_CONDITION_1 val1 );
int getISOLATOROUTPUT_CONDITION( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_DIGITALOUTPUT_CONDITION_1 * val1, bool bQuery=true );
int setISOLATOROUTPUT_RESPONSE( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_ISOLATOROUTPUT_RESPONSE_1 val1 );
int getISOLATOROUTPUT_RESPONSE( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_ISOLATOROUTPUT_RESPONSE_1 * val1, bool bQuery=true );
int setSENSORUART_BAUD( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_BAUD_1 val1 );
int getSENSORUART_BAUD( MRQ_SENSORUART_BAUD val0, MRQ_SENSORUART_BAUD_1 * val1, bool bQuery=true );
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
,MRQ_IDENTITY_LABEL_1 val1
,MRQ_CAN_NETMANAGELED val2 );
int getSENSORUART_STATE( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery=true );
int setSENSORUART_SOF( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,byte val2 );
int getSENSORUART_SOF( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, byte * val2, bool bQuery=true );
int setSENSORUART_FRAMELEN( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,byte val2 );
int getSENSORUART_FRAMELEN( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, byte * val2, bool bQuery=true );
int setSENSORUART_RECEIVENUM( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,byte val2 );
int getSENSORUART_RECEIVENUM( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, byte * val2, bool bQuery=true );
int setSENSORUART_SWITCHTIME( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,uint32 val2 );
int getSENSORUART_SWITCHTIME( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, uint32 * val2, bool bQuery=true );
int getSENSORUART_DATA( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, byte * val2, char * val3, char * val4, char * val5, bool bQuery=true );
int setISOLATORIN_STATE( MRQ_CAN_NETMANAGELED val0 );
int getISOLATORIN_STATE(  MRQ_CAN_NETMANAGELED * val0, bool bQuery=true );
int setISOLATORIN_TYPE( MRQ_TRIGGER_LEVELTYPE_1 val0 );
int getISOLATORIN_TYPE(  MRQ_TRIGGER_LEVELTYPE_1 * val0, bool bQuery=true );
int setISOLATORIN_RESPONSE( MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val0 );
int getISOLATORIN_RESPONSE(  MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val0, bool bQuery=true );
int setISOLATORIN_RESPCHAN( byte val0 );
int getISOLATORIN_RESPCHAN(  byte * val0, bool bQuery=true );
int setISOLATORIN_SMODE( MRQ_TRIGGER_PATTSMODE val0 );
int getISOLATORIN_SMODE(  MRQ_TRIGGER_PATTSMODE * val0, bool bQuery=true );
int setISOLATORIN_SPERIOD( uint32 val0 );
int getISOLATORIN_SPERIOD(  uint32 * val0, bool bQuery=true );
int setABSENCALARM_STATE( MRQ_IDENTITY_LABEL_1 val0
,MRQ_CAN_NETMANAGELED val1 );
int getABSENCALARM_STATE( MRQ_IDENTITY_LABEL_1 val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery=true );
int setABSENCALARM_UPLIMIT( MRQ_IDENTITY_LABEL_1 val0
,uint32 val1 );
int getABSENCALARM_UPLIMIT( MRQ_IDENTITY_LABEL_1 val0, uint32 * val1, bool bQuery=true );
int setABSENCALARM_DOWNLIMIT( MRQ_IDENTITY_LABEL_1 val0
,uint32 val1 );
int getABSENCALARM_DOWNLIMIT( MRQ_IDENTITY_LABEL_1 val0, uint32 * val1, bool bQuery=true );
int setABSENCALARM_ZEROVALUE( MRQ_IDENTITY_LABEL_1 val0
,uint32 val1 );
int getABSENCALARM_ZEROVALUE( MRQ_IDENTITY_LABEL_1 val0, uint32 * val1, bool bQuery=true );
int setABSENCALARM_ZEROPOSITION( MRQ_IDENTITY_LABEL_1 val0
,MRQ_ABSENCALARM_ZEROPOSITION_1 val1 );
int getABSENCALARM_ZEROPOSITION( MRQ_IDENTITY_LABEL_1 val0, MRQ_ABSENCALARM_ZEROPOSITION_1 * val1, bool bQuery=true );
int setABSENCALARM_RESPONSE( MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val0 );
int getABSENCALARM_RESPONSE(  MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val0, bool bQuery=true );
int setDISTANCEALARM_STATE( MRQ_IDENTITY_LABEL_1 val0
,MRQ_CAN_NETMANAGELED val1 );
int getDISTANCEALARM_STATE( MRQ_IDENTITY_LABEL_1 val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery=true );
int setDISTANCEALARM_ALARM1DIST( MRQ_IDENTITY_LABEL_1 val0
,uint16 val1 );
int getDISTANCEALARM_ALARM1DIST( MRQ_IDENTITY_LABEL_1 val0, uint16 * val1, bool bQuery=true );
int setDISTANCEALARM_ALARM2DIST( MRQ_IDENTITY_LABEL_1 val0
,uint16 val1 );
int getDISTANCEALARM_ALARM2DIST( MRQ_IDENTITY_LABEL_1 val0, uint16 * val1, bool bQuery=true );
int setDISTANCEALARM_ALARM3DIST( MRQ_IDENTITY_LABEL_1 val0
,uint16 val1 );
int getDISTANCEALARM_ALARM3DIST( MRQ_IDENTITY_LABEL_1 val0, uint16 * val1, bool bQuery=true );
int setOTP_STATE( MRQ_CAN_NETMANAGELED val0 );
int getOTP_STATE(  MRQ_CAN_NETMANAGELED * val0, bool bQuery=true );
int setOTP_THRESHOLD( uint16 val0 );
int getOTP_THRESHOLD(  uint16 * val0, bool bQuery=true );
int setOTP_RESPONSE( MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val0 );
int getOTP_RESPONSE(  MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val0, bool bQuery=true );
int setOTP_PERIOD( uint32 val0 );
int getOTP_PERIOD(  uint32 * val0, bool bQuery=true );
int getOTP_DATA(  uint32 * val0, bool bQuery=true );
int setANALOGIN_STATE( MRQ_CAN_NETMANAGELED val0 );
int getANALOGIN_STATE(  MRQ_CAN_NETMANAGELED * val0, bool bQuery=true );
int setANALOGIN_THRESHOLDH( f32 val0 );
int getANALOGIN_THRESHOLDH(  f32 * val0, bool bQuery=true );
int setANALOGIN_THRESHOLDL( f32 val0 );
int getANALOGIN_THRESHOLDL(  f32 * val0, bool bQuery=true );
int setANALOGIN_RESPONSEH( MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val0 );
int getANALOGIN_RESPONSEH(  MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val0, bool bQuery=true );
int setANALOGIN_RESPONSEL( MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val0 );
int getANALOGIN_RESPONSEL(  MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val0, bool bQuery=true );
};
}
#endif
