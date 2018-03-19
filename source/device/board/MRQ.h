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
,f32 val1 );
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
,MRQ_TRIGGER_PATTERN val2 );
int getTRIGGER_PATTERN( byte val0, MRQ_TRIGGER_PATTERN * val1, MRQ_TRIGGER_PATTERN * val2, bool bQuery=true );
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
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_SYSTEM_REPORTSWITCH val2 );
int getTRIGGER_LEVELSTATE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery=true );
int setTRIGGER_LEVELTYPE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_TRIGGER_LEVELTYPE_1 val2 );
int getTRIGGER_LEVELTYPE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_TRIGGER_LEVELTYPE_1 * val2, bool bQuery=true );
int setTRIGGER_LEVELRESP( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_OUTOFSTEP_LINERESPONSE val2 );
int getTRIGGER_LEVELRESP( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_OUTOFSTEP_LINERESPONSE * val2, bool bQuery=true );
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
int getDRIVER_SGLIMIT( byte val0, MRQ_DRIVER_SGLIMIT * val1, int8 * val2, bool bQuery=true );
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
int setISOLATORIN_STATE( MRQ_SYSTEM_REPORTSWITCH val0 );
int getISOLATORIN_STATE(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery=true );
int setISOLATORIN_TYPE( MRQ_TRIGGER_LEVELTYPE_1 val0 );
int getISOLATORIN_TYPE(  MRQ_TRIGGER_LEVELTYPE_1 * val0, bool bQuery=true );
int setISOLATORIN_RESPONSE( MRQ_OUTOFSTEP_LINERESPONSE val0 );
int getISOLATORIN_RESPONSE(  MRQ_OUTOFSTEP_LINERESPONSE * val0, bool bQuery=true );
int setISOLATORIN_RESPCHAN( byte val0 );
int getISOLATORIN_RESPCHAN(  byte * val0, bool bQuery=true );
int setISOLATORIN_SMODE( MRQ_TRIGGER_PATTSMODE val0 );
int getISOLATORIN_SMODE(  MRQ_TRIGGER_PATTSMODE * val0, bool bQuery=true );
int setISOLATORIN_SPERIOD( uint32 val0 );
int getISOLATORIN_SPERIOD(  uint32 * val0, bool bQuery=true );
};
}
#endif
