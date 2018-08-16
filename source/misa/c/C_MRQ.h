#include "visa_adapter.h"
#ifndef _C_MRQ_H
#define _C_MRQ_H
//!ENUM
//!NONE|CAN|RS232
int MRQ_setLINK_INTFC( ViSession vi, char * val0 );
//!
//!
//!MRQ_LINK_INTFC
//!val0
int MRQ_getLINK_INTFC( ViSession vi , char * val0 );
//!
//!
//!MRQ_LINK_DEVICEINFO,MRQ_LINK_DEVICEINFO_1,uint32
//!val0,val1,val2
int MRQ_getLINK_DEVICEINFO( ViSession vi , char * val0,char * val1,int *val2 );
//!ENUM
//!NORMAL|NONE
int MRQ_setSYSTEM_WORKMODE( ViSession vi, char * val0 );
//!
//!
//!MRQ_SYSTEM_WORKMODE
//!val0
int MRQ_getSYSTEM_WORKMODE( ViSession vi , char * val0 );
//!ENUM
//!DEFAULT|LAST
int MRQ_setSYSTEM_POWERON( ViSession vi, char * val0 );
//!
//!
//!MRQ_SYSTEM_POWERON
//!val0
int MRQ_getSYSTEM_POWERON( ViSession vi , char * val0 );
//!
//!
//!byte,char,char,char,char,char
//!val0,val1,val2,val3,val4,val5
int MRQ_getSYSTEM_SN( ViSession vi , int *val0,int *val1,int *val2,int *val3,int *val4,int *val5 );
//!
//!
//!MRQ_LINK_DEVICEINFO,MRQ_LINK_DEVICEINFO_1
//!val0,val1
int MRQ_getSYSTEM_TYPE( ViSession vi , char * val0,char * val1 );
//!
//!
//!char,char,char
//!val0,val1,val2
int MRQ_getSYSTEM_SOFTVER( ViSession vi , int *val0,int *val1,int *val2 );
//!
//!
//!char,char,char,char,char,char
//!val0,val1,val2,val3,val4,val5
int MRQ_getSYSTEM_FPGAVER( ViSession vi , int *val0,int *val1,int *val2,int *val3,int *val4,int *val5 );
//!
//!
//!char,char
//!val0,val1
int MRQ_getSYSTEM_HARDVER( ViSession vi , int *val0,int *val1 );
//!
//!
//!char,char
//!val0,val1
int MRQ_getSYSTEM_BOOTVER( ViSession vi , int *val0,int *val1 );
//!
//!
//!char,char,char,char,char,char
//!val0,val1,val2,val3,val4,val5
int MRQ_getSYSTEM_EVENTCODE( ViSession vi , int *val0,int *val1,int *val2,int *val3,int *val4,int *val5 );
//!
//!
//!uint16
//!val0
int MRQ_getSYSTEM_DIOSTATE( ViSession vi , int *val0 );
//!ENUM
//!OFF|ON
int MRQ_setSYSTEM_REVMOTION( ViSession vi, char * val0 );
//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getSYSTEM_REVMOTION( ViSession vi , char * val0 );
//!U8
//!
int MRQ_setSYSTEM_FANPARA( ViSession vi, int val0 );
//!
//!
//!byte
//!val0
int MRQ_getSYSTEM_FANPARA( ViSession vi , int *val0 );
//!ENUM,U8
//!BASE|BIGARM|SMALLARM|WRIST
int MRQ_setSYSTEM_ARMLEDPARA( ViSession vi, char * val0,int val1 );
//!ENUM
//!BASE|BIGARM|SMALLARM|WRIST
//!byte
//!val1
int MRQ_getSYSTEM_ARMLEDPARA( ViSession vi, char * val0 , int *val1 );
//!ENUM
//!4800|7200|9600|14400|19200|38400|57600|115200|128000
int MRQ_setRS232_BAUD( ViSession vi, char * val0 );
//!
//!
//!MRQ_RS232_BAUD
//!val0
int MRQ_getRS232_BAUD( ViSession vi , char * val0 );
//!ENUM
//!8|9
int MRQ_setRS232_WORDLEN( ViSession vi, char * val0 );
//!
//!
//!MRQ_RS232_WORDLEN
//!val0
int MRQ_getRS232_WORDLEN( ViSession vi , char * val0 );
//!ENUM
//!NONE|RTS|CTS|RTS&CTS
int MRQ_setRS232_FLOWCTL( ViSession vi, char * val0 );
//!
//!
//!MRQ_RS232_FLOWCTL
//!val0
int MRQ_getRS232_FLOWCTL( ViSession vi , char * val0 );
//!ENUM
//!NONE|EVEN|ODD
int MRQ_setRS232_PARITY( ViSession vi, char * val0 );
//!
//!
//!MRQ_RS232_PARITY
//!val0
int MRQ_getRS232_PARITY( ViSession vi , char * val0 );
//!ENUM
//!1|0.5|2|1.5
int MRQ_setRS232_STOPBIT( ViSession vi, char * val0 );
//!
//!
//!MRQ_RS232_STOPBIT
//!val0
int MRQ_getRS232_STOPBIT( ViSession vi , char * val0 );
//!
//!
int MRQ_setRS232_APPLYPARA( ViSession vi );
//!ENUM
//!A|B
int MRQ_setCAN_TYPE( ViSession vi, char * val0 );
//!
//!
//!MRQ_CAN_TYPE
//!val0
int MRQ_getCAN_TYPE( ViSession vi , char * val0 );
//!ENUM
//!1000|500|250|125|100|50|20|10
int MRQ_setCAN_BAUD( ViSession vi, char * val0 );
//!
//!
//!MRQ_CAN_BAUD
//!val0
int MRQ_getCAN_BAUD( ViSession vi , char * val0 );
//!U8
//!
int MRQ_setCAN_GROUP( ViSession vi, int val0 );
//!
//!
//!byte
//!val0
int MRQ_getCAN_GROUP( ViSession vi , int *val0 );
//!U32
//!
int MRQ_setCAN_SENDID( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getCAN_SENDID( ViSession vi , int *val0 );
//!U32
//!
int MRQ_setCAN_RECEIVEID( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getCAN_RECEIVEID( ViSession vi , int *val0 );
//!U32
//!
int MRQ_setCAN_GROUPID1( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getCAN_GROUPID1( ViSession vi , int *val0 );
//!U32
//!
int MRQ_setCAN_GROUPID2( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getCAN_GROUPID2( ViSession vi , int *val0 );
//!U32
//!
int MRQ_setCAN_BROADCASTID( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getCAN_BROADCASTID( ViSession vi , int *val0 );
//!
//!
int MRQ_setCAN_APPLYPARA( ViSession vi );
//!ENUM
//!OFF|ON
int MRQ_setCAN_NETMANAGELED( ViSession vi, char * val0 );
//!ENUM
//!IDLE|HASH|SIGNATURE
int MRQ_setCAN_NETMANAGESTATE( ViSession vi, char * val0 );
//!
//!
//!MRQ_CAN_NETMANAGESTATE
//!val0
int MRQ_getCAN_NETMANAGESTATE( ViSession vi , char * val0 );
//!ENUM,U32
//!NONE|SENDID|RECEIVEID
int MRQ_setCAN_NETMANAGEID( ViSession vi, char * val0,int val1 );
//!ENUM
//!NONE|SENDID|RECEIVEID
//!uint32
//!val1
int MRQ_getCAN_NETMANAGEID( ViSession vi, char * val0 , int *val1 );
//!U32
//!
int MRQ_setCAN_NETMANAGEHASH( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getCAN_NETMANAGEHASH( ViSession vi , int *val0 );
//!U32
//!
int MRQ_setCLOCK_FREQUENCY( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getCLOCK_FREQUENCY( ViSession vi , int *val0 );
//!ENUM
//!OFF|ON
int MRQ_setCLOCK_SYNCREGISTER( ViSession vi, char * val0 );
//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getCLOCK_SYNCREGISTER( ViSession vi , char * val0 );
//!ENUM
//!RADIOID|RECEIVEID|GROUPID1|GROUPID2
int MRQ_setCLOCK_STARTTYPE( ViSession vi, char * val0 );
//!
//!
//!MRQ_CLOCK_STARTTYPE
//!val0
int MRQ_getCLOCK_STARTTYPE( ViSession vi , char * val0 );
//!ENUM
//!END|START
int MRQ_setCLOCK_SYNCSTATE( ViSession vi, char * val0 );
//!
//!
//!MRQ_CLOCK_SYNCSTATE
//!val0
int MRQ_getCLOCK_SYNCSTATE( ViSession vi , char * val0 );
//!
//!
//!uint32
//!val0
int MRQ_getCLOCK_COUNT( ViSession vi , int *val0 );
//!U8,ENUM,ENUM
//![CHANNUM],RESET|STOP|RUN|PREPARE|EMERGSTOP,MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTION_SWITCH( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTION_STATE_2
//!val2
int MRQ_getMOTION_STATE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM
//![CHANNUM],ACTIVE|QUERY
int MRQ_setMOTION_STATEREPORT( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_MOTION_STATEREPORT
//!val1
int MRQ_getMOTION_STATEREPORT( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],SOFTWARE|DIGITALIO|CAN|ALL
int MRQ_setMOTION_STARTSOURCE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_MOTION_STARTSOURCE
//!val1
int MRQ_getMOTION_STARTSOURCE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],RADIOID|RECEIVEID|GROUPID1|GROUPID2
int MRQ_setMOTION_STARTTYPE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_CLOCK_STARTTYPE
//!val1
int MRQ_getMOTION_STARTTYPE( ViSession vi, int val0 , char * val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MAXSPEED( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MAXSPEED( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MINSPEED( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MINSPEED( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MAXPOSITION( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MAXPOSITION( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MINPOSITION( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MINPOSITION( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MAXTORQUE( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MAXTORQUE( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MINTORQUE( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MINTORQUE( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MAXACCELERATION( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MAXACCELERATION( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MINACCELERATION( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MINACCELERATION( ViSession vi, int val0 , float *val1 );
//!U8,U32
//![CHANNUM]
int MRQ_setMOTION_ORIGIN( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getMOTION_ORIGIN( ViSession vi, int val0 , int *val1 );
//!U8
//![CHANNUM]
int MRQ_setMOTION_GOORIGIN( ViSession vi, int val0 );
//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setMOTION_OFFSETSTATE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getMOTION_OFFSETSTATE( ViSession vi, int val0 , char * val1 );
//!U8,S32
//![CHANNUM]
int MRQ_setMOTION_OFFSET( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!int16
//!val1
int MRQ_getMOTION_COUNTCIRCLE( ViSession vi, int val0 , int *val1 );
//!U8
//![CHANNUM]
//!int32
//!val1
int MRQ_getMOTION_ABCOUNT( ViSession vi, int val0 , int *val1 );
//!U8
//![CHANNUM]
int MRQ_setMOTION_ABCOUNTCLEAR( ViSession vi, int val0 );
//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getMOTION_RUNTIMEL( ViSession vi, int val0 , int *val1 );
//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getMOTION_RUNTIMEH( ViSession vi, int val0 , int *val1 );
//!U8,ENUM,U8
//![CHANNUM],GROUP1|GROUP2
int MRQ_setIDENTITY_GROUP( ViSession vi, int val0,char * val1,int val2 );
//!U8,ENUM
//![CHANNUM],GROUP1|GROUP2
//!byte
//!val2
int MRQ_getIDENTITY_GROUP( ViSession vi, int val0,char * val1 , int *val2 );
//!ENUM
//!OFF|ON
int MRQ_setIDENTITY_DISTDEVICE( ViSession vi, char * val0 );
//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getIDENTITY_DISTDEVICE( ViSession vi , char * val0 );
//!U8,ENUM,ENUM
//![CHANNUM],SINANJU|MEGATRON|GOUF,S1|S2|S3|S4
int MRQ_setIDENTITY_LABEL( ViSession vi, int val0,char * val1,char * val2 );
//!U8
//![CHANNUM]
//!MRQ_IDENTITY_LABEL,MRQ_IDENTITY_LABEL_1
//!val1,val2
int MRQ_getIDENTITY_LABEL( ViSession vi, int val0 , char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],1.8|0.9|15|7.5
int MRQ_setMOTOR_STEPANGLE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_MOTOR_STEPANGLE
//!val1
int MRQ_getMOTOR_STEPANGLE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],ROTARY|LINEAR
int MRQ_setMOTOR_TYPE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_MOTOR_TYPE
//!val1
int MRQ_getMOTOR_TYPE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],ANGLE|RADIAN|MILLIMETER
int MRQ_setMOTOR_POSITIONUNIT( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_MOTOR_POSITIONUNIT
//!val1
int MRQ_getMOTOR_POSITIONUNIT( ViSession vi, int val0 , char * val1 );
//!U8,U16
//![CHANNUM]
int MRQ_setMOTOR_GEARRATIONUM( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!uint16
//!val1
int MRQ_getMOTOR_GEARRATIONUM( ViSession vi, int val0 , int *val1 );
//!U8,U16
//![CHANNUM]
int MRQ_setMOTOR_GEARRATIODEN( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!uint16
//!val1
int MRQ_getMOTOR_GEARRATIODEN( ViSession vi, int val0 , int *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTOR_LEAD( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTOR_LEAD( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTOR_PEAKSPEED( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTOR_PEAKSPEED( ViSession vi, int val0 , float *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTOR_PEAKACCELERATION( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTOR_PEAKACCELERATION( ViSession vi, int val0 , float *val1 );
//!U8,ENUM
//![CHANNUM],8|11|14|17|23|24
int MRQ_setMOTOR_SIZE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_MOTOR_SIZE
//!val1
int MRQ_getMOTOR_SIZE( ViSession vi, int val0 , char * val1 );
//!U8,U8
//![CHANNUM]
int MRQ_setMOTOR_VOLTAGE( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getMOTOR_VOLTAGE( ViSession vi, int val0 , int *val1 );
//!U8,U8
//![CHANNUM]
int MRQ_setMOTOR_CURRENT( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getMOTOR_CURRENT( ViSession vi, int val0 , int *val1 );
//!U8,F32
//![CHANNUM]
int MRQ_setMOTOR_BACKLASH( ViSession vi, int val0,float val1 );
//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTOR_BACKLASH( ViSession vi, int val0 , float *val1 );
//!U8,U16
//![CHANNUM]
int MRQ_setMOTOR_BACKLASHTEST( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!uint16
//!val1
int MRQ_getMOTOR_BACKLASHTEST( ViSession vi, int val0 , int *val1 );
//!U8,U32
//![CHANNUM]
int MRQ_setENCODER_LINENUM( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getENCODER_LINENUM( ViSession vi, int val0 , int *val1 );
//!U8,ENUM
//![CHANNUM],1|3
int MRQ_setENCODER_CHANNELNUM( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_ENCODER_CHANNELNUM
//!val1
int MRQ_getENCODER_CHANNELNUM( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],INCREMENTAL|ABSOLUTE
int MRQ_setENCODER_TYPE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_ENCODER_TYPE
//!val1
int MRQ_getENCODER_TYPE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],SINGLE|DOUBLE|QUADRUPLE
int MRQ_setENCODER_MULTIPLE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_ENCODER_MULTIPLE
//!val1
int MRQ_getENCODER_MULTIPLE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],NONE|OFF|ON
int MRQ_setENCODER_STATE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_ENCODER_STATE
//!val1
int MRQ_getENCODER_STATE( ViSession vi, int val0 , char * val1 );
//!U8,U8
//![CHANNUM]
int MRQ_setENCODER_FEEDBACKRATIO( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getENCODER_FEEDBACKRATIO( ViSession vi, int val0 , int *val1 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,END|CLEAR
int MRQ_setMOTIONPLAN_PVTCONFIG( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_PVTCONFIG_1
//!val2
int MRQ_getMOTIONPLAN_PVTCONFIG( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON|CALC
int MRQ_setMOTIONPLAN_PRESETSTATE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_PRESETSTATE_1
//!val2
int MRQ_getMOTIONPLAN_PRESETSTATE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,CYCLE|FIFO
int MRQ_setMOTIONPLAN_EXECUTEMODE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_EXECUTEMODE_1
//!val2
int MRQ_getMOTIONPLAN_EXECUTEMODE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,CUBICPOLY|TRAPEZOID|SCURVE
int MRQ_setMOTIONPLAN_PLANMODE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_PLANMODE_1
//!val2
int MRQ_getMOTIONPLAN_PLANMODE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,PVT|LVT_CORRECT|LVT_NOCORRECT
int MRQ_setMOTIONPLAN_MOTIONMODE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_MOTIONMODE_1
//!val2
int MRQ_getMOTIONPLAN_MOTIONMODE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,4|8|16|32
int MRQ_setMOTIONPLAN_MODIFYDUTY( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_MODIFYDUTY_1
//!val2
int MRQ_getMOTIONPLAN_MODIFYDUTY( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint16
//!val2
int MRQ_getMOTIONPLAN_REMAINPOINT( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,ENUM,U32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_CYCLENUM( ViSession vi, int val0,char * val1,int val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint32
//!val2
int MRQ_getMOTIONPLAN_CYCLENUM( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_WARNPOINT( ViSession vi, int val0,char * val1,int val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!byte
//!val2
int MRQ_getMOTIONPLAN_WARNPOINT( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,STOP|HOLD
int MRQ_setMOTIONPLAN_ENDSTATE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_ENDSTATE_1
//!val2
int MRQ_getMOTIONPLAN_ENDSTATE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint32
//!val2
int MRQ_getMOTIONPLAN_BUFFERSIZE( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,IMMEDIATE|DISTANCE
int MRQ_setMOTIONPLAN_STOPMODE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_STOPMODE_1
//!val2
int MRQ_getMOTIONPLAN_STOPMODE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_STOPDISTANCE( ViSession vi, int val0,char * val1,float val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!f32
//!val2
int MRQ_getMOTIONPLAN_STOPDISTANCE( ViSession vi, int val0,char * val1 , float *val2 );
//!U8,ENUM,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_STOPTIME( ViSession vi, int val0,char * val1,float val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!f32
//!val2
int MRQ_getMOTIONPLAN_STOPTIME( ViSession vi, int val0,char * val1 , float *val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON
int MRQ_setMOTIONPLAN_OOSLINESTATE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getMOTIONPLAN_OOSLINESTATE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,U16
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_OOSLINEOUTNUM( ViSession vi, int val0,char * val1,int val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint16
//!val2
int MRQ_getMOTIONPLAN_OOSLINEOUTNUM( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,NONE|ALARM|STOP|ALARM&STOP
int MRQ_setMOTIONPLAN_OOSLINERESPONSE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val2
int MRQ_getMOTIONPLAN_OOSLINERESPONSE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON
int MRQ_setMOTIONPLAN_OOSTOTALSTATE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getMOTIONPLAN_OOSTOTALSTATE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,U32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_OOSTOTALOUTNUM( ViSession vi, int val0,char * val1,int val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint32
//!val2
int MRQ_getMOTIONPLAN_OOSTOTALOUTNUM( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,NONE|ALARM|STOP|ALARM&STOP
int MRQ_setMOTIONPLAN_OOSTOTALRESPONSE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val2
int MRQ_getMOTIONPLAN_OOSTOTALRESPONSE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,U8,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ_setPOSITION( ViSession vi, int val0,char * val1,int val2,float val3 );
//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
//!f32
//!val3
int MRQ_getPOSITION( ViSession vi, int val0,char * val1,int val2 , float *val3 );
//!U8,ENUM,U8,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ_setVELOCITY( ViSession vi, int val0,char * val1,int val2,float val3 );
//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
//!f32
//!val3
int MRQ_getVELOCITY( ViSession vi, int val0,char * val1,int val2 , float *val3 );
//!U8,ENUM,U8,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ_setTIME( ViSession vi, int val0,char * val1,int val2,float val3 );
//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
//!f32
//!val3
int MRQ_getTIME( ViSession vi, int val0,char * val1,int val2 , float *val3 );
//!U8,ENUM,U8,U16,U16
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ_setTIMESCALE( ViSession vi, int val0,char * val1,int val2,int val3,int val4 );
//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
//!uint16,uint16
//!val3,val4
int MRQ_getTIMESCALE( ViSession vi, int val0,char * val1,int val2 , int *val3,int *val4 );
//!U8,ENUM,ENUM
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC,OFF|ON
int MRQ_setREPORT_STATE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getREPORT_STATE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,U32
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
int MRQ_setREPORT_PERIOD( ViSession vi, int val0,char * val1,int val2 );
//!U8,ENUM
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
//!uint32
//!val2
int MRQ_getREPORT_PERIOD( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,ENUM
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
//!uint32
//!val2
int MRQ_getREPORT_DATA( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,ENUM
//![CHANNUM],PATTERN|LEVEL
int MRQ_setTRIGGER_MODE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_TRIGGER_MODE
//!val1
int MRQ_getTRIGGER_MODE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setTRIGGER_PATTSTATE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getTRIGGER_PATTSTATE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM,ENUM
//![CHANNUM],X|L|R|F|H,X|L|R|F|H
int MRQ_setTRIGGER_PATTERN( ViSession vi, int val0,char * val1,char * val2 );
//!U8
//![CHANNUM]
//!MRQ_TRIGGER_PATTERN,MRQ_TRIGGER_PATTERN
//!val1,val2
int MRQ_getTRIGGER_PATTERN( ViSession vi, int val0 , char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],NONE|ALARM|STOP|ALARM&STOP
int MRQ_setTRIGGER_PATTRESP( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val1
int MRQ_getTRIGGER_PATTRESP( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],CONTINUED|INTERVAL|SINGLE
int MRQ_setTRIGGER_PATTSMODE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_TRIGGER_PATTSMODE
//!val1
int MRQ_getTRIGGER_PATTSMODE( ViSession vi, int val0 , char * val1 );
//!U8,U32
//![CHANNUM]
int MRQ_setTRIGGER_PATTSPERIOD( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getTRIGGER_PATTSPERIOD( ViSession vi, int val0 , int *val1 );
//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,OFF|ON
int MRQ_setTRIGGER_LEVELSTATE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,OFF|ON
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getTRIGGER_LEVELSTATE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,RESERVE|LOW|RISE|FALL|HIGH
int MRQ_setTRIGGER_LEVELTYPE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!MRQ_TRIGGER_LEVELTYPE_1
//!val2
int MRQ_getTRIGGER_LEVELTYPE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,NONE|ALARM|STOP|ALARM&STOP
int MRQ_setTRIGGER_LEVELRESP( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val2
int MRQ_getTRIGGER_LEVELRESP( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,CONTINUED|INTERVAL|SINGLE
int MRQ_setTRIGGER_LEVELSMODE( ViSession vi, int val0,char * val1,char * val2 );
//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!MRQ_TRIGGER_PATTSMODE
//!val2
int MRQ_getTRIGGER_LEVELSMODE( ViSession vi, int val0,char * val1 , char * val2 );
//!U8,ENUM,F32
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ_setTRIGGER_LEVELSPERIOD( ViSession vi, int val0,char * val1,float val2 );
//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!f32
//!val2
int MRQ_getTRIGGER_LEVELSPERIOD( ViSession vi, int val0,char * val1 , float *val2 );
//!U8
//![CHANNUM]
//!MRQ_DRIVER_TYPE
//!val1
int MRQ_getDRIVER_TYPE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],R0|R1|R2
//!uint32
//!val2
int MRQ_getDRIVER_STATEREG( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,U8
//![CHANNUM]
int MRQ_setDRIVER_CURRENT( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getDRIVER_CURRENT( ViSession vi, int val0 , int *val1 );
//!U8,ENUM
//![CHANNUM],256|128|64|32|16|8|4|2|1
int MRQ_setDRIVER_MICROSTEPS( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_DRIVER_MICROSTEPS
//!val1
int MRQ_getDRIVER_MICROSTEPS( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setDRIVER_STATE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getDRIVER_STATE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM,S8
//![CHANNUM],UPLIMIT|DOWNLIMIT
int MRQ_setDRIVER_SGLIMIT( ViSession vi, int val0,char * val1,int val2 );
//!U8
//![CHANNUM],UPLIMIT|DOWNLIMIT
//!MRQ_DRIVER_SGLIMIT,int8
//!val1,val2
int MRQ_getDRIVER_SGLIMIT( ViSession vi, int val0 , char * val1,int *val2 );
//!U8,ENUM,S16
//![CHANNUM],SG0|SGT|SEMAX|SEMIN
int MRQ_setDRIVER_SGPARASET( ViSession vi, int val0,char * val1,int val2 );
//!U8,ENUM
//![CHANNUM],SG0|SGT|SEMAX|SEMIN
//!int16
//!val2
int MRQ_getDRIVER_SGPARASET( ViSession vi, int val0,char * val1 , int *val2 );
//!U8,U8
//![CHANNUM]
int MRQ_setDRIVER_IDLECURRENT( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getDRIVER_IDLECURRENT( ViSession vi, int val0 , int *val1 );
//!U8,U32
//![CHANNUM]
int MRQ_setDRIVER_SWITCHTIME( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getDRIVER_SWITCHTIME( ViSession vi, int val0 , int *val1 );
//!U8,ENUM
//![CHANNUM],1/2|1/4
int MRQ_setDRIVER_MINICURRRATIO( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_DRIVER_MINICURRRATIO
//!val1
int MRQ_getDRIVER_MINICURRRATIO( ViSession vi, int val0 , char * val1 );
//!U8,ENUM,U32
//![CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
int MRQ_setDRIVER_REGCONFIG( ViSession vi, int val0,char * val1,int val2 );
//!U8,ENUM
//![CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
//!uint32
//!val2
int MRQ_getDRIVER_REGCONFIG( ViSession vi, int val0,char * val1 , int *val2 );
//!ENUM,ENUM
//!DO1|DO2|DO3|DO4,DISABLE|ENABLE
int MRQ_setDIGITALOUTPUT_STATE( ViSession vi, char * val0,char * val1 );
//!ENUM
//!DO1|DO2|DO3|DO4
//!MRQ_DIGITALOUTPUT_STATE_1
//!val1
int MRQ_getDIGITALOUTPUT_STATE( ViSession vi, char * val0 , char * val1 );
//!ENUM,ENUM
//!DO1|DO2|DO3|DO4,LEVEL|NONE
int MRQ_setDIGITALOUTPUT_SIGNAL( ViSession vi, char * val0,char * val1 );
//!ENUM
//!DO1|DO2|DO3|DO4
//!MRQ_DIGITALOUTPUT_SIGNAL_1
//!val1
int MRQ_getDIGITALOUTPUT_SIGNAL( ViSession vi, char * val0 , char * val1 );
//!ENUM,ENUM
//!DO1|DO2|DO3|DO4,POSITIVE|NEGATIVE
int MRQ_setDIGITALOUTPUT_POLARITY( ViSession vi, char * val0,char * val1 );
//!ENUM
//!DO1|DO2|DO3|DO4
//!MRQ_DIGITALOUTPUT_POLARITY_1
//!val1
int MRQ_getDIGITALOUTPUT_POLARITY( ViSession vi, char * val0 , char * val1 );
//!ENUM,U8
//!DO1|DO2|DO3|DO4,[CHANNUM]
int MRQ_setDIGITALOUTPUT_SOURCE( ViSession vi, char * val0,int val1 );
//!ENUM
//!DO1|DO2|DO3|DO4
//!byte
//!val1
int MRQ_getDIGITALOUTPUT_SOURCE( ViSession vi, char * val0 , int *val1 );
//!ENUM,ENUM
//!DO1|DO2|DO3|DO4,AUTO|NONE
int MRQ_setDIGITALOUTPUT_CONDITION( ViSession vi, char * val0,char * val1 );
//!ENUM
//!DO1|DO2|DO3|DO4
//!MRQ_DIGITALOUTPUT_CONDITION_1
//!val1
int MRQ_getDIGITALOUTPUT_CONDITION( ViSession vi, char * val0 , char * val1 );
//!ENUM,U32
//!DO1|DO2|DO3|DO4
int MRQ_setDIGITALOUTPUT_PERIOD( ViSession vi, char * val0,int val1 );
//!ENUM
//!DO1|DO2|DO3|DO4
//!uint32
//!val1
int MRQ_getDIGITALOUTPUT_PERIOD( ViSession vi, char * val0 , int *val1 );
//!ENUM,U16
//!DO1|DO2|DO3|DO4
int MRQ_setDIGITALOUTPUT_DUTY( ViSession vi, char * val0,int val1 );
//!ENUM
//!DO1|DO2|DO3|DO4
//!uint16
//!val1
int MRQ_getDIGITALOUTPUT_DUTY( ViSession vi, char * val0 , int *val1 );
//!ENUM,ENUM
//!YO1|YO2,DISABLE|ENABLE
int MRQ_setISOLATOROUTPUT_STATE( ViSession vi, char * val0,char * val1 );
//!ENUM
//!YO1|YO2
//!MRQ_DIGITALOUTPUT_STATE_1
//!val1
int MRQ_getISOLATOROUTPUT_STATE( ViSession vi, char * val0 , char * val1 );
//!ENUM,U8
//!YO1|YO2,[CHANNUM]
int MRQ_setISOLATOROUTPUT_SOURCE( ViSession vi, char * val0,int val1 );
//!ENUM
//!YO1|YO2
//!byte
//!val1
int MRQ_getISOLATOROUTPUT_SOURCE( ViSession vi, char * val0 , int *val1 );
//!ENUM,ENUM
//!YO1|YO2,AUTO|NONE
int MRQ_setISOLATOROUTPUT_CONDITION( ViSession vi, char * val0,char * val1 );
//!ENUM
//!YO1|YO2
//!MRQ_DIGITALOUTPUT_CONDITION_1
//!val1
int MRQ_getISOLATOROUTPUT_CONDITION( ViSession vi, char * val0 , char * val1 );
//!ENUM,ENUM
//!YO1|YO2,TURNON|CUTOFF
int MRQ_setISOLATOROUTPUT_RESPONSE( ViSession vi, char * val0,char * val1 );
//!ENUM
//!YO1|YO2
//!MRQ_ISOLATOROUTPUT_RESPONSE_1
//!val1
int MRQ_getISOLATOROUTPUT_RESPONSE( ViSession vi, char * val0 , char * val1 );
//!ENUM,ENUM
//!UART1|UART2,4800|7200|9600|14400|19200|38400|57600|115200|128000|2000000|2500000
int MRQ_setSENSORUART_BAUD( ViSession vi, char * val0,char * val1 );
//!ENUM
//!UART1|UART2
//!MRQ_SENSORUART_BAUD_1
//!val1
int MRQ_getSENSORUART_BAUD( ViSession vi, char * val0 , char * val1 );
//!ENUM,ENUM
//!UART1|UART2,8|9
int MRQ_setSENSORUART_WORDLEN( ViSession vi, char * val0,char * val1 );
//!ENUM
//!UART1|UART2
//!MRQ_RS232_WORDLEN
//!val1
int MRQ_getSENSORUART_WORDLEN( ViSession vi, char * val0 , char * val1 );
//!ENUM,ENUM
//!UART1|UART2,NONE|RTS|CTS|RTS&CTS
int MRQ_setSENSORUART_FLOWCTL( ViSession vi, char * val0,char * val1 );
//!ENUM
//!UART1|UART2
//!MRQ_RS232_FLOWCTL
//!val1
int MRQ_getSENSORUART_FLOWCTL( ViSession vi, char * val0 , char * val1 );
//!ENUM,ENUM
//!UART1|UART2,NONE|EVEN|ODD
int MRQ_setSENSORUART_PARITY( ViSession vi, char * val0,char * val1 );
//!ENUM
//!UART1|UART2
//!MRQ_RS232_PARITY
//!val1
int MRQ_getSENSORUART_PARITY( ViSession vi, char * val0 , char * val1 );
//!ENUM,ENUM
//!UART1|UART2,1|0.5|2|1.5
int MRQ_setSENSORUART_STOPBIT( ViSession vi, char * val0,char * val1 );
//!ENUM
//!UART1|UART2
//!MRQ_RS232_STOPBIT
//!val1
int MRQ_getSENSORUART_STOPBIT( ViSession vi, char * val0 , char * val1 );
//!ENUM
//!UART1|UART2
int MRQ_setSENSORUART_APPLYPARA( ViSession vi, char * val0 );
//!ENUM,ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4,OFF|ON
int MRQ_setSENSORUART_STATE( ViSession vi, char * val0,char * val1,char * val2 );
//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getSENSORUART_STATE( ViSession vi, char * val0,char * val1 , char * val2 );
//!ENUM,ENUM,U8
//!UART1|UART2,S1|S2|S3|S4
int MRQ_setSENSORUART_SOF( ViSession vi, char * val0,char * val1,int val2 );
//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!byte
//!val2
int MRQ_getSENSORUART_SOF( ViSession vi, char * val0,char * val1 , int *val2 );
//!ENUM,ENUM,U8
//!UART1|UART2,S1|S2|S3|S4
int MRQ_setSENSORUART_FRAMELEN( ViSession vi, char * val0,char * val1,int val2 );
//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!byte
//!val2
int MRQ_getSENSORUART_FRAMELEN( ViSession vi, char * val0,char * val1 , int *val2 );
//!ENUM,ENUM,U8
//!UART1|UART2,S1|S2|S3|S4
int MRQ_setSENSORUART_RECEIVENUM( ViSession vi, char * val0,char * val1,int val2 );
//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!byte
//!val2
int MRQ_getSENSORUART_RECEIVENUM( ViSession vi, char * val0,char * val1 , int *val2 );
//!ENUM,ENUM,U32
//!UART1|UART2,S1|S2|S3|S4
int MRQ_setSENSORUART_SWITCHTIME( ViSession vi, char * val0,char * val1,int val2 );
//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!uint32
//!val2
int MRQ_getSENSORUART_SWITCHTIME( ViSession vi, char * val0,char * val1 , int *val2 );
//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!byte,char,char,char
//!val2,val3,val4,val5
int MRQ_getSENSORUART_DATA( ViSession vi, char * val0,char * val1 , int *val2,int *val3,int *val4,int *val5 );
//!ENUM
//!OFF|ON
int MRQ_setISOLATORIN_STATE( ViSession vi, char * val0 );
//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getISOLATORIN_STATE( ViSession vi , char * val0 );
//!ENUM
//!RESERVE|LOW|RISE|FALL|HIGH
int MRQ_setISOLATORIN_TYPE( ViSession vi, char * val0 );
//!
//!
//!MRQ_TRIGGER_LEVELTYPE_1
//!val0
int MRQ_getISOLATORIN_TYPE( ViSession vi , char * val0 );
//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setISOLATORIN_RESPONSE( ViSession vi, char * val0 );
//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getISOLATORIN_RESPONSE( ViSession vi , char * val0 );
//!U8
//![CHANNUM]
int MRQ_setISOLATORIN_RESPCHAN( ViSession vi, int val0 );
//!
//!
//!byte
//!val0
int MRQ_getISOLATORIN_RESPCHAN( ViSession vi , int *val0 );
//!ENUM
//!CONTINUED|INTERVAL|SINGLE
int MRQ_setISOLATORIN_SMODE( ViSession vi, char * val0 );
//!
//!
//!MRQ_TRIGGER_PATTSMODE
//!val0
int MRQ_getISOLATORIN_SMODE( ViSession vi , char * val0 );
//!U32
//!
int MRQ_setISOLATORIN_SPERIOD( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getISOLATORIN_SPERIOD( ViSession vi , int *val0 );
//!ENUM,ENUM
//!S1|S2|S3|S4,OFF|ON
int MRQ_setABSENCALARM_STATE( ViSession vi, char * val0,char * val1 );
//!ENUM
//!S1|S2|S3|S4
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getABSENCALARM_STATE( ViSession vi, char * val0 , char * val1 );
//!ENUM,U32
//!S1|S2|S3|S4
int MRQ_setABSENCALARM_UPLIMIT( ViSession vi, char * val0,int val1 );
//!ENUM
//!S1|S2|S3|S4
//!uint32
//!val1
int MRQ_getABSENCALARM_UPLIMIT( ViSession vi, char * val0 , int *val1 );
//!ENUM,U32
//!S1|S2|S3|S4
int MRQ_setABSENCALARM_DOWNLIMIT( ViSession vi, char * val0,int val1 );
//!ENUM
//!S1|S2|S3|S4
//!uint32
//!val1
int MRQ_getABSENCALARM_DOWNLIMIT( ViSession vi, char * val0 , int *val1 );
//!ENUM,U32
//!S1|S2|S3|S4
int MRQ_setABSENCALARM_ZEROVALUE( ViSession vi, char * val0,int val1 );
//!ENUM
//!S1|S2|S3|S4
//!uint32
//!val1
int MRQ_getABSENCALARM_ZEROVALUE( ViSession vi, char * val0 , int *val1 );
//!ENUM,ENUM
//!S1|S2|S3|S4,NONE|EXIS
int MRQ_setABSENCALARM_ZEROPOSITION( ViSession vi, char * val0,char * val1 );
//!ENUM
//!S1|S2|S3|S4
//!MRQ_ABSENCALARM_ZEROPOSITION_1
//!val1
int MRQ_getABSENCALARM_ZEROPOSITION( ViSession vi, char * val0 , char * val1 );
//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setABSENCALARM_RESPONSE( ViSession vi, char * val0 );
//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getABSENCALARM_RESPONSE( ViSession vi , char * val0 );
//!ENUM,ENUM
//!S1|S2|S3|S4,OFF|ON
int MRQ_setDISTANCEALARM_STATE( ViSession vi, char * val0,char * val1 );
//!ENUM
//!S1|S2|S3|S4
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getDISTANCEALARM_STATE( ViSession vi, char * val0 , char * val1 );
//!ENUM,U16
//!S1|S2|S3|S4
int MRQ_setDISTANCEALARM_ALARM1DIST( ViSession vi, char * val0,int val1 );
//!ENUM
//!S1|S2|S3|S4
//!uint16
//!val1
int MRQ_getDISTANCEALARM_ALARM1DIST( ViSession vi, char * val0 , int *val1 );
//!ENUM,U16
//!S1|S2|S3|S4
int MRQ_setDISTANCEALARM_ALARM2DIST( ViSession vi, char * val0,int val1 );
//!ENUM
//!S1|S2|S3|S4
//!uint16
//!val1
int MRQ_getDISTANCEALARM_ALARM2DIST( ViSession vi, char * val0 , int *val1 );
//!ENUM,U16
//!S1|S2|S3|S4
int MRQ_setDISTANCEALARM_ALARM3DIST( ViSession vi, char * val0,int val1 );
//!ENUM
//!S1|S2|S3|S4
//!uint16
//!val1
int MRQ_getDISTANCEALARM_ALARM3DIST( ViSession vi, char * val0 , int *val1 );
//!U8
//![CHANNUM]
//!MRQ_NEWDRIVER_TYPE
//!val1
int MRQ_getNEWDRIVER_TYPE( ViSession vi, int val0 , char * val1 );
//!U8
//!
int MRQ_setNEWDRIVER_CURRENT( ViSession vi, int val0 );
//!
//!
//!byte
//!val0
int MRQ_getNEWDRIVER_CURRENT( ViSession vi , int *val0 );
//!ENUM
//!RESERVE|RESERVE1|RESERVE2|32|16|8|4|2|1
int MRQ_setNEWDRIVER_MICROSTEPS( ViSession vi, char * val0 );
//!
//!
//!MRQ_NEWDRIVER_MICROSTEPS
//!val0
int MRQ_getNEWDRIVER_MICROSTEPS( ViSession vi , char * val0 );
//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setNEWDRIVER_STATE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getNEWDRIVER_STATE( ViSession vi, int val0 , char * val1 );
//!U8,ENUM
//![CHANNUM],END|START
int MRQ_setPDM_SAMPLESTATE( ViSession vi, int val0,char * val1 );
//!U8
//![CHANNUM]
//!MRQ_CLOCK_SYNCSTATE
//!val1
int MRQ_getPDM_SAMPLESTATE( ViSession vi, int val0 , char * val1 );
//!U8,U8
//![CHANNUM]
int MRQ_setPDM_ENCDIV( ViSession vi, int val0,int val1 );
//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getPDM_ENCDIV( ViSession vi, int val0 , int *val1 );
//!U8
//![CHANNUM]
//!uint16
//!val1
int MRQ_getPDM_MICSTEPCOUNT( ViSession vi, int val0 , int *val1 );
//!U8,U16,U16
//!
//!byte
//!val3
int MRQ_getPDM_MICSTEPDATA( ViSession vi, int val0,int val1,int val2 , int *val3 );
//!ENUM
//!OFF|ON
int MRQ_setOTP_STATE( ViSession vi, char * val0 );
//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getOTP_STATE( ViSession vi , char * val0 );
//!U16
//!
int MRQ_setOTP_THRESHOLD( ViSession vi, int val0 );
//!
//!
//!uint16
//!val0
int MRQ_getOTP_THRESHOLD( ViSession vi , int *val0 );
//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setOTP_RESPONSE( ViSession vi, char * val0 );
//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getOTP_RESPONSE( ViSession vi , char * val0 );
//!U32
//!
int MRQ_setOTP_PERIOD( ViSession vi, int val0 );
//!
//!
//!uint32
//!val0
int MRQ_getOTP_PERIOD( ViSession vi , int *val0 );
//!
//!
//!uint32
//!val0
int MRQ_getOTP_DATA( ViSession vi , int *val0 );
//!ENUM
//!OFF|ON
int MRQ_setANALOGIN_STATE( ViSession vi, char * val0 );
//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getANALOGIN_STATE( ViSession vi , char * val0 );
//!F32
//!
int MRQ_setANALOGIN_THRESHOLDH( ViSession vi, float val0 );
//!
//!
//!f32
//!val0
int MRQ_getANALOGIN_THRESHOLDH( ViSession vi , float *val0 );
//!F32
//!
int MRQ_setANALOGIN_THRESHOLDL( ViSession vi, float val0 );
//!
//!
//!f32
//!val0
int MRQ_getANALOGIN_THRESHOLDL( ViSession vi , float *val0 );
//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setANALOGIN_RESPONSEH( ViSession vi, char * val0 );
//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getANALOGIN_RESPONSEH( ViSession vi , char * val0 );
//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setANALOGIN_RESPONSEL( ViSession vi, char * val0 );
//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getANALOGIN_RESPONSEL( ViSession vi , char * val0 );
#endif
