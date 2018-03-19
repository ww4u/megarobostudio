#include "deviceMRQ.h"

#include "scpi/scpi.h"

#ifdef DEVICE_EMIT_SIGNAL
#include "../../app/dpcobj.h"
#endif

#define ANGLE_TO_DEG( angle )   (360.0f*angle)/(1<<18)

namespace MegaDevice
{

deviceMRQ::deviceMRQ()
{
    mTpvCap = 0x200;

    //! attach device
    for ( int i = 0; i < sizeof_array(mDownloader); i++ )
    {
        mDownloader[i].attachDevice( this, i );
    }

    for ( int i = 0; i < sizeof_array(mMrqFsms); i++ )
    {
        mMrqFsms[i].build();

        mMrqFsms[i].set( this, i );

        mMrqFsms[i].setId( (quint32)this, i );
    }

    for ( int i = 0; i < sizeof_array(mMotors); i++ )
    {
        mMotors[i].set( this, i );
    }

    //! \note only a sema for one device
    mDownloaderSema.release( 1 );
}

deviceMRQ::~deviceMRQ()
{
    for( int i = 0; i < 4; i++ )
    {
        if ( mDownloader[i].isRunning() )
        {
            mDownloader[i].terminate();
            mDownloader[i].wait();
        }
    }
}

int deviceMRQ::setDeviceId(DeviceId &id, int siblingCnt )
{
    int ret;

    ret = MRQ::setDeviceId( id, siblingCnt );
    if ( ret != 0 )
    { return ret; }

    //! vdevice ids
    VDevice::setCanSendId( id.mSendId );
    VDevice::setCanRecvId( id.mRecvId );
    VDevice::setCanBroadcastId( id.mBroadcastId );

    //! id apply
    _MRQ_model::setCAN_SENDID( id.mSendId );
    _MRQ_model::setCAN_RECEIVEID( id.mRecvId );
    _MRQ_model::setCAN_BROADCASTID( id.mBroadcastId );

    //! write the device
    ret = applyIds( siblingCnt );
    if ( ret != 0 )
    { return ret; }

    //! verify
//    setCanRecvId( mDeviceId.mRecvId );

//    ret = verifyIds( siblingCnt );
    ret = verifyIds( 0 );
    if ( ret != 0 )
    { return ret; }

    setSignature( mDeviceId.mSignature );

    return 0;
}

//! over write
int deviceMRQ::setMOTIONPLAN_POSITION( uint16 val0
,f32 val1 )
{ return m_pBus->write( DEVICE_RECEIVE_ID, mc_MOTIONPLAN, sc_MOTIONPLAN_POSITION , val0, val1 ); }

int deviceMRQ::setMOTIONPLAN_VELOCITY( uint16 val0
,f32 val1 )
{ return m_pBus->write( DEVICE_RECEIVE_ID, mc_MOTIONPLAN, sc_MOTIONPLAN_VELOCITY , val0, val1 ); }

int deviceMRQ::setMOTIONPLAN_TIME( uint16 val0
,f32 val1 )
{ return m_pBus->write( DEVICE_RECEIVE_ID, mc_MOTIONPLAN, sc_MOTIONPLAN_TIME , val0, val1 ); }

//! cache to 0~4
int deviceMRQ::setMOTIONPLAN_POSITION( int axesid, uint16 val0
,f32 val1 )
{
    //! 55 2
    int ret;
    ret = setMOTIONPLAN_POSITION(val0,val1);

    if ( ret == 0)
    {
        MRQ_model::setMOTIONPLAN_POSITION( axesid, val1 );
    }
    if ( ret != 0){ log_device(); }
    return ret;
}
int deviceMRQ::setMOTIONPLAN_VELOCITY( int axesid, uint16 val0
,f32 val1 )
{
    //! 55 4
    int ret;
    ret = setMOTIONPLAN_VELOCITY(val0,val1);

    if ( ret == 0)
    {
        MRQ_model::setMOTIONPLAN_VELOCITY( axesid, val1 );
    }
    if ( ret != 0){ log_device(); }
    return ret;
}

int deviceMRQ::setMOTIONPLAN_TIME( int axesid, uint16 val0
,f32 val1 )
{
    //! 55 6
    int ret;
    ret = setMOTIONPLAN_TIME( val0, val1 );

    if ( ret == 0)
    {
        MRQ_model::setMOTIONPLAN_TIME( axesid, val1 );
    }
    if ( ret != 0){ log_device(); }
    return ret;
}

void deviceMRQ::setName( const QString &strName )
{
    if( mName == strName )
    { return; }

    mcModelObj::setName( strName );

    //! \todo write to the device

}

MRQ_model *deviceMRQ::getModel()
{
    return this;
}

//! upload all data
int deviceMRQ::uploadSetting()
{
    int ret;

    checked_call( getLINK_INTFC(&mLINK_INTFC) );
    checked_call( getSYSTEM_WORKMODE(&mSYSTEM_WORKMODE) );
    checked_call( getSYSTEM_POWERON(&mSYSTEM_POWERON));

    checked_call( getSYSTEM_REPORTSWITCH(&mSYSTEM_REPORTSWITCH) );

    checked_call( getRS232_BAUD(&mRS232_BAUD) );
    checked_call( getRS232_WORDLEN(&mRS232_WORDLEN) );
    checked_call( getRS232_FLOWCTL(&mRS232_FLOWCTL) );
    checked_call( getRS232_PARITY(&mRS232_PARITY) );

    checked_call( getRS232_STOPBIT(&mRS232_STOPBIT) );
    checked_call( getCAN_TYPE(&mCAN_TYPE) );
    checked_call( getCAN_BAUD(&mCAN_BAUD) );
    checked_call( getCAN_GROUP(&mCAN_GROUP) );

    checked_call( getCAN_SENDID(&mCAN_BROADCASTID) );
    checked_call( getCAN_NETMANAGESTATE(&mCAN_NETMANAGESTATE) );
    checked_call( getCAN_GROUPID1(&mCAN_GROUPID1) );
    checked_call( getCAN_GROUPID2(&mCAN_GROUPID2) );

    checked_call( getCAN_BROADCASTID(&mCAN_SENDID) );
    checked_call( getCAN_RECEIVEID(&mCAN_RECEIVEID) );

    checked_call( getCAN_NETMANAGEHASH(&mCAN_NETMANAGEHASH) );

//    checked_call( getCAN_NETMANAGESIGNATURESIZE(&mCAN_NETMANAGESIGNATURESIZE) );
    checked_call( getCLOCK_FREQUENCY(&mCLOCK_FREQUENCY) );
    checked_call( getCLOCK_SYNCREGISTER(&mCLOCK_SYNCREGISTER) );
    checked_call( getCLOCK_STARTTYPE(&mCLOCK_STARTTYPE) );

    checked_call( getCLOCK_SYNCSTATE(&mCLOCK_SYNCSTATE) );
    checked_call( getCLOCK_COUNT(&mCLOCK_COUNT) );

    //! motion 1--4
    for ( int i = 0; i < axes(); i++ )
    {
        checked_call( getMOTION_STATE( i, mMOTION_STATE+i) );
        checked_call( getMOTION_STATEREPORT( i, mMOTION_STATEREPORT+i) );

        checked_call( getMOTION_STARTSOURCE( i, mMOTION_STARTSOURCE+i) );
        checked_call( getMOTION_STARTTYPE( i, mMOTION_STARTTYPE+i) );
        checked_call( getMOTION_MAXSPEED( i, mMOTION_MAXSPEED+i) );
        checked_call( getMOTION_MINSPEED( i, mMOTION_MINSPEED+i) );

        checked_call( getMOTION_MAXPOSITION( i, mMOTION_MAXPOSITION+i) );
        checked_call( getMOTION_MINPOSITION( i, mMOTION_MINPOSITION+i) );
        checked_call( getMOTION_MAXTORQUE( i, mMOTION_MAXTORQUE+i) );
        checked_call( getMOTION_MINTORQUE( i, mMOTION_MINTORQUE+i) );

        checked_call( getMOTION_MAXPOSITION( i, mMOTION_MAXPOSITION+i) );
        checked_call( getMOTION_MINPOSITION( i, mMOTION_MINPOSITION+i) );
        checked_call( getMOTION_MAXTORQUE( i, mMOTION_MAXTORQUE+i) );
        checked_call( getMOTION_MINTORQUE( i, mMOTION_MINTORQUE+i) );

        checked_call( getMOTION_MAXACCELERATION( i, mMOTION_MAXACCELERATION+i) );
        checked_call( getMOTION_MINACCELERATION( i, mMOTION_MINACCELERATION+i) );
        checked_call( getMOTION_ORIGIN( i, mMOTION_ORIGIN+i) );
        checked_call( getMOTION_INITPOSITIONUNIT( i, mMOTION_INITPOSITIONUNIT+i) );

        checked_call( getMOTION_INITPOSITION( i, mMOTION_INITPOSITION+i) );
        checked_call( getMOTION_INITIOSIGNAL( i, mMOTION_INITIOSIGNAL+i) );
        checked_call( getMOTION_INCHINGMODE( i, mMOTION_INCHINGMODE+i) );
        checked_call( getMOTION_INCHINGTIME( i, mMOTION_INCHINGTIME+i) );

        checked_call( getMOTION_OFFSETSTATE( i, mMOTION_OFFSETSTATE+i) );
        checked_call( getMOTION_PVTSTEPS( i, mMOTION_PVTSTEPS+i) );
        checked_call( getMOTION_COUNTSTEPS( i, mMOTION_COUNTSTEPS+i) );
        checked_call( getMOTION_PVTCIRCLE( i, mMOTION_PVTCIRCLE+i) );

        checked_call( getMOTION_COUNTCIRCLE( i, mMOTION_COUNTCIRCLE+i) );
        checked_call( getMOTION_ABCOUNT( i, mMOTION_ABCOUNT+i) );
        checked_call( getMOTION_REVMOTION( i, mMOTION_REVMOTION+i) );
        checked_call( getIDENTITY_GROUP( i, MRQ_IDENTITY_GROUP_GROUP1, mIDENTITY_GROUP[i] ) );
        checked_call( getIDENTITY_GROUP( i, MRQ_IDENTITY_GROUP_GROUP2, mIDENTITY_GROUP[i] + 1 ) );


        checked_call( getSTOPDECEL_MODE( i, mSTOPDECEL_MODE+i) );
        checked_call( getSTOPDECEL_DISTANCE( i, mSTOPDECEL_DISTANCE+i) );
        checked_call( getSTOPDECEL_TIME( i, mSTOPDECEL_TIME+i) );

        checked_call( getOUTOFSTEP_LINESTATE( i, mOUTOFSTEP_LINESTATE+i) );
//        checked_call( getOUTOFSTEP_LINEOUTNUM( i, mOUTOFSTEP_LINEOUTNUM+i) );U16?
        checked_call( getOUTOFSTEP_LINERESPONSE( i, mOUTOFSTEP_LINERESPONSE+i) );
        checked_call( getOUTOFSTEP_TOTALSTATE( i, mOUTOFSTEP_TOTALSTATE+i) );

        checked_call( getOUTOFSTEP_TOTALOUTNUM( i, mOUTOFSTEP_TOTALOUTNUM+i) );
        checked_call( getOUTOFSTEP_TOTALRESPONSE( i, mOUTOFSTEP_TOTALRESPONSE+i) );
        checked_call( getMOTOR_STEPANGLE( i, mMOTOR_STEPANGLE+i) );
        checked_call( getMOTOR_TYPE( i, mMOTOR_TYPE+i) );

        checked_call( getMOTOR_POSITIONUNIT( i, mMOTOR_POSITIONUNIT+i) );
        checked_call( getMOTOR_GEARRATIONUM( i, mMOTOR_GEARRATIONUM+i) );
        checked_call( getMOTOR_GEARRATIODEN( i, mMOTOR_GEARRATIODEN+i) );
        checked_call( getMOTOR_LEAD( i, mMOTOR_LEAD+i) );

        checked_call( getMOTOR_PEAKSPEED( i, mMOTOR_PEAKSPEED+i) );
        checked_call( getMOTOR_PEAKACCELERATION( i, mMOTOR_PEAKACCELERATION+i) );
        checked_call( getMOTOR_SIZE( i, mMOTOR_SIZE+i) );
        checked_call( getMOTOR_VOLTAGE( i, mMOTOR_VOLTAGE+i) );

        checked_call( getMOTOR_CURRENT( i, mMOTOR_CURRENT+i) );
        checked_call( getMOTOR_BACKLASH( i, mMOTOR_BACKLASH+i) );
        checked_call( getENCODER_LINENUM( i, mENCODER_LINENUM+i) );
        checked_call( getENCODER_CHANNELNUM( i, mENCODER_CHANNELNUM+i) );

        checked_call( getENCODER_TYPE( i, mENCODER_TYPE+i) );
        checked_call( getENCODER_MULTIPLE( i, mENCODER_MULTIPLE+i) );
        checked_call( getENCODER_STATE( i, mENCODER_STATE+i) );
        checked_call( getMOTIONPLAN_PVTCONFIG( i, mMOTIONPLAN_PVTCONFIG+i) );

        checked_call( getMOTIONPLAN_EXECUTEMODE( i, mMOTIONPLAN_EXECUTEMODE+i) );

        checked_call( getMOTIONPLAN_PLANMODE( i, mMOTIONPLAN_PLANMODE+i) );
        checked_call( getMOTIONPLAN_MOTIONMODE( i, mMOTIONPLAN_MOTIONMODE+i) );
        checked_call( getMOTIONPLAN_MODIFYDUTY( i, mMOTIONPLAN_MODIFYDUTY+i) );
        checked_call( getMOTIONPLAN_REMAINPOINT( i, mMOTIONPLAN_REMAINPOINT+i) );

//        checked_call( getMOTIONPLAN_OUTPUTPOINT( i, mMOTIONPLAN_OUTPUTPOINT+i) );
        checked_call( getMOTIONPLAN_STARTPOINT( i, mMOTIONPLAN_STARTPOINT+i) );
        checked_call( getMOTIONPLAN_ENDPOINT( i, mMOTIONPLAN_ENDPOINT+i) );
        checked_call( getMOTIONPLAN_CYCLENUM( i, mMOTIONPLAN_CYCLENUM+i) );

        checked_call( getMOTIONPLAN_WARNPOINT( i, mMOTIONPLAN_WARNPOINT+i) );
        checked_call( getMOTIONPLAN_ENDSTATE( i, mMOTIONPLAN_ENDSTATE+i) );
        checked_call( getMOTIONPLAN_FEEDBACKRATIO( i, mMOTIONPLAN_FEEDBACKRATIO+i) );
        for ( int j = 0; j < 14; j++ )
        {
            checked_call( getREPORT_STATE( i, (MRQ_REPORT_STATE)i, mREPORT_STATE[i]+j) );
            checked_call( getREPORT_PERIOD( i, (MRQ_REPORT_STATE)i, mREPORT_PERIOD[i]+j) );
            checked_call( getREPORT_DATA( i, (MRQ_REPORT_STATE)i, mREPORT_DATA[i]+j) );
        }

        checked_call( getTRIGGER_MODE( i, mTRIGGER_MODE+i) );
        checked_call( getTRIGGER_PATTSTATE( i, mTRIGGER_PATTSTATE+i) );

//        checked_call( getTRIGGER_PATTERN( i,
//                                          mTRIGGER_PATTERN+i,
//                                          mTRIGGER_PATTERN1+i,
//                                          mTRIGGER_PATTERN2+i,
//                                          mTRIGGER_PATTERN3+i,
//                                          mTRIGGER_PATTERN4+i
//                                          ) ); U16?

        checked_call( getTRIGGER_PATTRESP( i, mTRIGGER_PATTRESP+i) );
        checked_call( getTRIGGER_PATTSMODE( i, mTRIGGER_PATTSMODE+i) );
        checked_call( getTRIGGER_PATTSPERIOD( i, mTRIGGER_PATTSPERIOD+i) );

        for( int j = 0; j < 5; j++ )
        {
//            checked_call( getTRIGGER_LEVELSTATE( i, (MOTION_INITIOSIGNAL)j, mTRIGGER_LEVELSTATE[i]+j) ); 4Byte?
//            checked_call( getTRIGGER_LEVELTYPE( i, (MOTION_INITIOSIGNAL)j, mTRIGGER_LEVELTYPE[i]+j) );
//            checked_call( getTRIGGER_LEVELRESP( i, (MOTION_INITIOSIGNAL)j, mTRIGGER_LEVELRESP[i]+j) );
//            checked_call( getTRIGGER_LEVELSMODE( i, (MOTION_INITIOSIGNAL)j, mTRIGGER_LEVELSMODE[i]+j) );
//            checked_call( getTRIGGER_LEVELSPERIOD( i, (MOTION_INITIOSIGNAL)j, mTRIGGER_LEVELSPERIOD[i]+j) );
        }

        checked_call( getDRIVER_TYPE( i, mDRIVER_TYPE+i) );
        for( int j = 0; j < 3; j++ )
        {
            checked_call( getDRIVER_STATEREG( i, (MRQ_DRIVER_STATEREG)j, mDRIVER_STATEREG[i]+j) );
        }
        checked_call( getDRIVER_CURRENT( i, mDRIVER_CURRENT+i) );

        checked_call( getDRIVER_MICROSTEPS( i, mDRIVER_MICROSTEPS+i) );
        checked_call( getDRIVER_SWITCH( i, mDRIVER_SWITCH+i) );
        for ( int j = 0; j < 5; j++ )
        {
            checked_call( getDRIVER_REGCONFIG( i, (MRQ_DRIVER_REGCONFIG)j, mDRIVER_REGCONFIG[i]+j) );
        }
        for ( int j = 0; j < 2; j++ )
        {
//            checked_call( getDRIVER_SGLIMIT( i, (DRIVER_SGLIMIT)j, mDRIVER_SGLIMIT[i]+j) );
        }

        for ( int j = 0; j < 4; j++ )
        {
//            checked_call( getDRIVER_SGPARASET( i, (DRIVER_SGPARASET)j, mDRIVER_SGPARASET[i]+j) );
        }
    }

    checked_call( getIDENTITY_DISTDEVICE(&mIDENTITY_DISTDEVICE) );

    for ( int i = 0; i < 2; i++ )
    {
        checked_call( getDIGITALOUT_STATE( (MRQ_DIGITALOUT_STATE)i, mDIGITALOUT_STATE + i) );
        checked_call( getDIGITALOUT_SIGNAL( (MRQ_DIGITALOUT_STATE)i, mDIGITALOUT_SIGNAL + i) );
        checked_call( getDIGITALOUT_POLARITY( (MRQ_DIGITALOUT_STATE)i, mDIGITALOUT_POLARITY + i) );

        checked_call( getDIGITALOUT_SOURCE( (MRQ_DIGITALOUT_STATE)i, mDIGITALOUT_SOURCE + i) );
        checked_call( getDIGITALOUT_CONDITION( (MRQ_DIGITALOUT_STATE)i, mDIGITALOUT_CONDITION + i) );
        checked_call( getDIGITALOUT_PERIOD( (MRQ_DIGITALOUT_STATE)i, mDIGITALOUT_PERIOD + i) );
        checked_call( getDIGITALOUT_DUTY( (MRQ_DIGITALOUT_STATE)i, mDIGITALOUT_DUTY + i) );
    }

    for( int i = 0; i < 2; i++ )
    {
        checked_call( getISOLATOROUTPUT_STATE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_STATE + i) );
        checked_call( getISOLATOROUTPUT_SOURCE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_SOURCE + i) );
        checked_call( getISOLATOROUTPUT_CONDITION( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_CONDITION + i) );
        checked_call( getISOLATOROUTPUT_RESPONSE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_RESPONSE + i) );
    }

    //! sensor uart
    for ( int i = 0; i < 2; i++ )
    {
        checked_call( getSENSORUART_BAUD( (MRQ_SENSORUART_BAUD)i, mSENSORUART_BAUD + i) );
        checked_call( getSENSORUART_WORDLEN( (MRQ_SENSORUART_BAUD)i, mSENSORUART_WORDLEN + i) );
        checked_call( getSENSORUART_FLOWCTL( (MRQ_SENSORUART_BAUD)i, mSENSORUART_FLOWCTL + i) );
        checked_call( getSENSORUART_PARITY( (MRQ_SENSORUART_BAUD)i, mSENSORUART_PARITY + i) );

        checked_call( getSENSORUART_STOPBIT( (MRQ_SENSORUART_BAUD)i, mSENSORUART_STOPBIT + i) );
    }

    //! uart, s[4]
    for( int i = 0; i < 2; i++ )
    {
        for ( int j = 0; j < axes(); j++ )
        {
            checked_call( getSENSORUART_STATE( (MRQ_SENSORUART_BAUD)i, (MRQ_SENSORUART_STATE_1)j, mSENSORUART_STATE[i] + j) );
            checked_call( getSENSORUART_SOF( (MRQ_SENSORUART_BAUD)i, (MRQ_SENSORUART_STATE_1)j, mSENSORUART_SOF[i] + j) );
            checked_call( getSENSORUART_FRAMELEN( (MRQ_SENSORUART_BAUD)i, (MRQ_SENSORUART_STATE_1)j, mSENSORUART_FRAMELEN[i] + j) );

            checked_call( getSENSORUART_RECEIVENUM( (MRQ_SENSORUART_BAUD)i, (MRQ_SENSORUART_STATE_1)j, mSENSORUART_RECEIVENUM[i] + j) );
            checked_call( getSENSORUART_SWITCHTIME( (MRQ_SENSORUART_BAUD)i, (MRQ_SENSORUART_STATE_1)j, mSENSORUART_SWITCHTIME[i] + j) );
        }
    }

//    checked_call( getOTP_STATE(&mOTP_STATE) );

//    checked_call( getOTP_THRESHOLD(&mOTP_THRESHOLD) );
//    checked_call( getOTP_RESPONSE(&mOTP_RESPONSE) );
//    checked_call( getOTP_PERIOD(&mOTP_PERIOD) );
//    checked_call( getOTP_DATA(&mOTP_DATA) );

//    checked_call( getANALOGIN_STATE(&mANALOGIN_STATE) );
//    checked_call( getANALOGIN_THRESHOLDH(&mANALOGIN_THRESHOLDH) );
//    checked_call( getANALOGIN_THRESHOLDL(&mANALOGIN_THRESHOLDL) );
//    checked_call( getANALOGIN_RESPONSEH(&mANALOGIN_RESPONSEH) );

//    checked_call( getANALOGIN_RESPONSEL(&mANALOGIN_RESPONSEL) );

    return ret;
}

int deviceMRQ::applyIds( int siblingCnt )
{
    int ret;

    //! -1. cache dst id
    DeviceId dstId = mDeviceId;

    //! 0. broad id
    mDeviceId.setRecvId( dstId.mBroadcastId );

    //! 1. to hash mode
    ret = setCAN_NETMANAGESTATE( MRQ_CAN_NETMANAGESTATE_HASH );
    if ( ret != 0 )
    { return ret; }

    //! 2. set hash
    ret = setCAN_NETMANAGEHASH( mDeviceId.mSignature );
    if ( ret != 0 )
    { return ret; }

    //! 3. check mode

        //! clear all bus
        m_pBus->clear( );

        m_pBus->write( DEVICE_RECEIVE_ID, mc_CAN, sc_CAN_NETMANAGESTATE_Q );

        m_pBus->wait_us( m_pBus->enumTmo() );

        int frameCount = m_pBus->size();
        if ( frameCount < 1 )
        { return -1; }

        int frameSize = 3;
        int frameIds[ frameCount  ];
        byte frameBuf[ frameCount * frameSize ];

        memset( frameBuf, 0, frameCount * frameSize );

        ret = m_pBus->doFrameRead( mDeviceId, frameIds, frameBuf, frameSize, frameCount );
        if ( ret != frameCount )
        { return -1; }

        int stat = -1;
        for ( int i = 0; i < frameCount; i++ )
        {
            if ( frameBuf[ i *frameSize + 2] == MRQ_CAN_NETMANAGESTATE_HASH )
            {
                stat = frameBuf[ i *frameSize + 2];
                break;
            }
        }

        if ( stat != MRQ_CAN_NETMANAGESTATE_HASH )
        { logDbg()<<frameCount; return -2; }

    //! 4. set send id
    ret = setCAN_NETMANAGEID( MRQ_CAN_NETMANAGEID_SENDID, dstId.mSendId );
    if ( ret != 0 )
    { return ret; }

    //! 5. set recv id
    ret = setCAN_NETMANAGEID( MRQ_CAN_NETMANAGEID_RECEIVEID, dstId.mRecvId );
    if ( ret != 0 )
    { return ret; }

    //! 6. de mode
    ret = setCAN_NETMANAGESTATE( MRQ_CAN_NETMANAGESTATE_IDLE );
    if ( ret != 0 )
    { return ret; }

    //! 7. apply
    ret = setCAN_APPLYPARA();
    if ( ret != 0 )
    { return ret; }

    //! 8. recover the id
    mDeviceId = dstId;

    return 0;
}

int deviceMRQ::verifyIds( int siblingCnt )
{
    int ret;
    uint32 sendId, recvId;
    int sibling;

    //! send id
    sibling = siblingCnt;
    bool bQuery = true;
    do
    {
        ret = getCAN_SENDID( &sendId, bQuery );
        if ( ret != 0 )
        { return ret; }
        bQuery = false;

        if ( (int)sendId == mDeviceId.mSendId )
        { break; }

    }while( sibling-- > 0 );

    if ( (int)sendId != mDeviceId.mSendId )
    { return -1; }

    //! receive id
    sibling = siblingCnt;
    bQuery = true;
    do
    {
        ret = getCAN_RECEIVEID( &recvId, bQuery );
        if ( ret != 0 )
        { return ret; }
        bQuery = false;

        if ( (int)recvId == mDeviceId.mRecvId )
        { break; }
    }while( sibling-- > 0 );

    if ( (int)recvId != mDeviceId.mRecvId )
    {
        logDbg()<<recvId<<mDeviceId.mRecvId;
        return -3;
    }

    return 0;
}

void deviceMRQ::rst()
{
    //! foreach clear
    for ( int i = 0; i < getAxes(); i++ )
    {
        setMOTIONPLAN_PVTCONFIG( i, MRQ_MOTIONPLAN_PVTCONFIG_CLEAR );
    }
}

int deviceMRQ::upload()
{
    loadDesc();
    loadSN();
    loadSwVer();
    loadHwVer();

    loadFwVer();
    loadBtVer();

    loadTpvCap();

    return 0;
}

int deviceMRQ::testAdd( int a, int b )
{ return a + b; }

QString deviceMRQ::loadDesc()
{
    int ret;
    MRQ_SYSTEM_TYPE type;
    MRQ_SYSTEM_TYPE_1 type2;
    ret = getSYSTEM_TYPE( &type, &type2 );
    if ( ret != 0 )
    { return mDesc; }

    //! format the type
    mDesc = QString("%1-%2").arg( MRQ_model::toString( type ),
                                  MRQ_model::toString( type2 ) );

    return mDesc;
}

QString deviceMRQ::loadSN()
{
    int ret;

    //! write
    ret = m_pBus->write(DEVICE_RECEIVE_ID, (byte)mc_SYSTEM, (byte)sc_SYSTEM_SN_Q);
    if (ret != 0)
    { return mSn; }

    byte buf[32];
    int retLen;
    ret = m_pBus->doSplitRead(DEVICE_RECEIVE_ID, 2, buf, sizeof(buf), &retLen );
    if (ret != 0)
    { return mSn; }

    //! enugh
    if ( retLen < sizeof_array(buf) && retLen > 0 )
    {
        mSn = QByteArray( (const char*)buf, retLen );
    }

    return mSn;
}

QString deviceMRQ::loadSwVer()
{
    char v0, v1, v2;
    int ret;

    ret = getSYSTEM_SOFTVER( &v0, &v1, &v2 );
    if ( ret == 0 )
    {
        mSwVer = QString("%1.%2.%3").arg((int)v0).arg((int)v1).arg((int)v2);
    }

    return mSwVer;
}
QString deviceMRQ::loadHwVer()
{
    char v0, v1;
    int ret;

    ret = getSYSTEM_HARDVER( &v0, &v1 );
    if ( ret == 0 )
    {
        mHwVer = QString("%1.%2").arg((int)v0).arg((int)v1);
    }

    return mHwVer;
}
QString deviceMRQ::loadFwVer()
{
    char v0, v1, v2;
    int ret;

    ret = getSYSTEM_FPGAVER( &v0, &v1, &v2 );
    if ( ret == 0 )
    {
        mFwVer = QString("%1.%2.%3").arg((int)v0).arg((int)v1).arg((int)v2);
    }

    return mFwVer;
}
QString deviceMRQ::loadBtVer()
{
    char v0, v1;
    int ret;

    ret = getSYSTEM_BOOTVER( &v0, &v1 );
    if ( ret == 0 )
    {
        mBtVer = QString("%1.%2").arg((int)v0).arg((int)v1);
    }

    return mBtVer;
}

//! [0~360)
float deviceMRQ::getAngle( int ax )
{
    int ret;
    quint32 xangle;

    ret = getREPORT_DATA( ax, MRQ_REPORT_STATE_XANGLE, &xangle );
    if ( ret != 0 )
    { return -1; }
    else
    {
        return ANGLE_TO_DEG( xangle );
    }
}

int deviceMRQ::loadTpvCap()
{
    int ret = 0;
    quint16 tpvCap;
    ret = getMOTIONPLAN_REMAINPOINT( 0, &tpvCap );
    if ( ret == 0 )
    { mTpvCap = tpvCap; }

    return mTpvCap;
}

int deviceMRQ::goInit( int axesId,
             MRQ_MOTION_INITPOSITIONUNIT unit,
             MRQ_MOTION_INITIOSIGNAL iosig,
             float pos
             )
{
    setMOTION_INITPOSITIONUNIT( axesId, unit );
    setMOTION_INITIOSIGNAL( axesId, iosig );
    setMOTION_INITPOSITION( axesId, pos );

    setMOTION_GOINITPOSITION( axesId );

    //! \todo wait completed

    return 0;
}

int deviceMRQ::beginTpvDownload( int axesId )
{
    int ret;

    //! \errant exec mode to cycle
    checked_call( setMOTIONPLAN_EXECUTEMODE( axesId,
                                             mMOTIONPLAN_EXECUTEMODE[axesId]
                                             ) );

    checked_call( setMOTIONPLAN_PVTCONFIG( axesId, MRQ_MOTIONPLAN_PVTCONFIG_CLEAR ) );
    checked_call( getMOTIONPLAN_REMAINPOINT( axesId, mMOTIONPLAN_REMAINPOINT + axesId ) );
    checked_call( setMOTIONPLAN_PVTCONFIG( axesId, MRQ_MOTIONPLAN_PVTCONFIG_START ) );

    checked_call( setSYSTEM_REPORTSWITCH( MRQ_SYSTEM_REPORTSWITCH_OFF ) );

    setTpvIndex( axesId, 0 );

    return ret;
}
int deviceMRQ::tpvDownload(
                 int axesId,
                 int index,
                 f32 t,
                 f32 p,
                 f32 v )
{
    int ret;

    checked_call( setMOTIONPLAN_POSITION( axesId, index, p * mPBase ) );
    checked_call( setMOTIONPLAN_VELOCITY( axesId, index, v * mVBase ) );
    checked_call( setMOTIONPLAN_TIME( axesId, index, t * mTBase ) );
//logWarning()<<axesId<<index<<p<<v<<t;
    return ret;
}
int deviceMRQ::tpvDownload( int axesId,
                 QList<tpvRow *> &list,
                 int from,
                 int len )
{
    int ret = 0;

    int id = 0;
    for( int i = 0; i < mMOTIONPLAN_REMAINPOINT[axesId] && i < len && id < list.size(); i++ )
    {
        id = i + from;
        checked_call( tpvDownload( axesId,
                                   i,
                                   list[id]->mT,
                                   list[id]->mP,
                                   list[id]->mV
                                   )
                      );

        #ifdef DEVICE_EMIT_SIGNAL
        dpcObj::instance()->tlsProgress( i, 0, len );
        logDbg()<<i;
        #endif
    }

    return ret;
}

int deviceMRQ::tpvDownload( int axesId,
                            tpvRow *pItem )
{
    Q_ASSERT( NULL != pItem );
    int ret;

    checked_call( tpvDownload( axesId,
                               getTpvIndex(axesId),
                               pItem->mT,
                               pItem->mP,
                               pItem->mV
                               )
                  );

    accTpvIndex( axesId );

    return ret;
}

int deviceMRQ::endTpvDownload( int axesId )
{
    int ret;

    checked_call( setMOTIONPLAN_PVTCONFIG( axesId, MRQ_MOTIONPLAN_PVTCONFIG_END ) );

    return ret;
}

//! \todo not send end
int deviceMRQ::tpvDownloadMission( int axesId,
                                   QList<tpvRow *> &list,
                                   int from,
                                   int len )
{
    int ret;

    checked_call( beginTpvDownload( axesId ) );

    ret = tpvDownload( axesId, list, from, len );

    checked_call( endTpvDownload(axesId) );

    return ret;
}

int deviceMRQ::pvtWrite( int axesId,
              QList<tpvRow *> &list,
              int from,
              int len )
{logDbg();
    mDownloader[axesId].append( list, from, len );

    if ( mDownloader[axesId].isRunning() )
    {
    }
    else
    { mDownloader[axesId].start(); }

    return 0;
}

int deviceMRQ::pvtWrite( int axesId,
              float t1, float p1,
              float t2, float p2 )
{
    //! point 1
    tpvRow *pRow1 = new tpvRow();
    if ( NULL == pRow1 )
    { return ERR_ALLOC_FAIL; }

    pRow1->setGc( true );
    pRow1->mT = t1;
    pRow1->mP = p1;
    pRow1->mV = 0;

    //! point 2
    tpvRow *pRow2 = new tpvRow();
    if ( NULL == pRow2 )
    {
        delete pRow1;
        return ERR_ALLOC_FAIL;
    }

    pRow2->setGc( true );
    pRow2->mT = t2;
    pRow2->mP = p2;
    pRow2->mV = 0;

    QList< tpvRow *> rotList;
    rotList.append( pRow1 );
    rotList.append( pRow2 );

    return pvtWrite( axesId, rotList );
}

int deviceMRQ::pvtWrite( int axesId,
                         float dT,
                         float dAngle )
{
    return pvtWrite( axesId, 0, 0, dT, dAngle );
}

void deviceMRQ::setTpvIndex( int axesId, int index )
{
    Q_ASSERT( axesId >= 0 && axesId <= sizeof_array(mTpvIndex) );

    mTpvIndex[ axesId ] = index;
}
int  deviceMRQ::getTpvIndex( int axesId )
{
    Q_ASSERT( axesId >= 0 && axesId <= sizeof_array(mTpvIndex) );

    return mTpvIndex[ axesId ];
}
void deviceMRQ::accTpvIndex( int axesId )
{
    Q_ASSERT( axesId >= 0 && axesId <= sizeof_array(mTpvIndex) );

    //! turn around
    mTpvIndex[axesId]++;
    if ( mTpvIndex[axesId] >= mTpvCap )
    {
        mTpvIndex[axesId] = 0;
    }
    else
    {}
}

int  deviceMRQ::getTpvCap()
{
    return mTpvCap;
}

void deviceMRQ::terminate( int axesId )
{
    Q_ASSERT( axesId >= 0 && axesId <= sizeof_array(mTpvIndex) );

    mDownloader[ axesId ].terminate();
    mDownloader[ axesId ].wait();

    sysQueue()->postMsg( e_download_terminated,
                      mDownloader[ axesId ].name(),
                      mDownloader[ axesId ].axes() );
}

//! only write no read
int deviceMRQ::requestMotionState( int axesId )
{
    int ret;

    ret = m_pBus->write( DEVICE_RECEIVE_ID, mc_MOTION, sc_MOTION_STATE_Q, (byte)axesId );

    return ret;
}

void deviceMRQ::acquireDownloader()
{
    mDownloaderSema.acquire();
}
void deviceMRQ::releaseDownloader()
{
    mDownloaderSema.release();
}

//! assist api
int deviceMRQ::rotate( int ax, float t, float ang )
{
    run( ax );

    return pvtWrite( ax, t, ang );
}

int deviceMRQ::fsmState( int ax )
{
    Q_ASSERT( ax >= 0 && ax < i_sizeof(mMrqFsms) );
    return mMrqFsms[ax].state();
}

}
