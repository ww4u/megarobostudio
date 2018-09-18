#include "deviceMRQ.h"

namespace MegaDevice
{

//! upload all data
int deviceMRQ::_uploadSetting()
{
    int ret=-1;

MRQ_PROGRESS( 0, ("LINK") );

    checked_call( getLINK_INTFC(&mLINK_INTFC) );
    checked_call( getSYSTEM_WORKMODE(&mSYSTEM_WORKMODE) );
    checked_call( getSYSTEM_POWERON(&mSYSTEM_POWERON));

    checked_call( getRS232_BAUD(&mRS232_BAUD) );
    checked_call( getRS232_WORDLEN(&mRS232_WORDLEN) );
    checked_call( getRS232_FLOWCTL(&mRS232_FLOWCTL) );
    checked_call( getRS232_PARITY(&mRS232_PARITY) );

    checked_call( getRS232_STOPBIT(&mRS232_STOPBIT) );
    checked_call( getCAN_TYPE(&mCAN_TYPE) );
    checked_call( getCAN_BAUD(&mCAN_BAUD) );
    checked_call( getCAN_GROUP(&mCAN_GROUP) );

//    checked_call( getCAN_SENDID(&mCAN_SENDID) );
//    checked_call( getCAN_RECEIVEID(&mCAN_RECEIVEID) );

//    checked_call( getCAN_GROUPID1(&mCAN_GROUPID1) );
//    checked_call( getCAN_GROUPID2(&mCAN_GROUPID2) );
//    checked_call( getCAN_BROADCASTID(&mCAN_BROADCASTID) );

    checked_call( getCAN_NETMANAGESTATE(&mCAN_NETMANAGESTATE) );
    checked_call( getCAN_NETMANAGEHASH(&mCAN_NETMANAGEHASH) );

    checked_call( getCLOCK_FREQUENCY(&mCLOCK_FREQUENCY) );
    checked_call( getCLOCK_SYNCREGISTER(&mCLOCK_SYNCREGISTER) );
    checked_call( getCLOCK_STARTTYPE(&mCLOCK_STARTTYPE) );

    checked_call( getCLOCK_SYNCSTATE(&mCLOCK_SYNCSTATE) );
    checked_call( getCLOCK_COUNT(&mCLOCK_COUNT) );

MRQ_PROGRESS( 10, ("MOTION") );

    //! state
    foreach_page()
    {
//        checked_call( getMOTION_STATE( _i, _axPage, mMOTION_STATE[_i] + _j) );
    }
    end_foreach_page()

    //! motion 1--4
    for ( int i = 0; i < axes(); i++ )
    {
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

        checked_call( getMOTION_OFFSETSTATE( i, mMOTION_OFFSETSTATE+i) );
//        checked_call( getMOTION_PVTSTEPS( i, mMOTION_PVTSTEPS+i) );
//        checked_call( getMOTION_COUNTSTEPS( i, mMOTION_COUNTSTEPS+i) );
//        checked_call( getMOTION_PVTCIRCLE( i, mMOTION_PVTCIRCLE+i) );

//        checked_call( getMOTION_COUNTCIRCLE( i, mMOTION_COUNTCIRCLE+i) );
//        checked_call( getMOTION_ABCOUNT( i, mMOTION_ABCOUNT+i) );
//        checked_call( getMOTION_REVMOTION( i, mMOTION_REVMOTION+i) );
    }

MRQ_PROGRESS( 20, ("MOTOR") );

    checked_call( getIDENTITY_DISTDEVICE( &mIDENTITY_DISTDEVICE ) );
    for ( int i = 0; i < axes(); i++ )
    {
        checked_call( getIDENTITY_GROUP( i, MRQ_IDENTITY_GROUP_GROUP1, mIDENTITY_GROUP[i] ) );
        checked_call( getIDENTITY_GROUP( i, MRQ_IDENTITY_GROUP_GROUP2, mIDENTITY_GROUP[i] + 1 ) );

        checked_call( getIDENTITY_LABEL( i,
                                         mIDENTITY_LABEL + i,
                                         mIDENTITY_LABEL1+ i ) );

        //! motor
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

        //! encoder
        if ( encoderAble() )
        {
            checked_call( getENCODER_LINENUM( i, mENCODER_LINENUM+i) );
            checked_call( getENCODER_CHANNELNUM( i, mENCODER_CHANNELNUM+i) );

            checked_call( getENCODER_TYPE( i, mENCODER_TYPE+i) );
            checked_call( getENCODER_MULTIPLE( i, mENCODER_MULTIPLE+i) );
            checked_call( getENCODER_STATE( i, mENCODER_STATE+i) );
            checked_call( getENCODER_FEEDBACKRATIO( i, mENCODER_FEEDBACKRATIO+i) );
        }
    }

MRQ_PROGRESS( 30, ("MOTORPLAN") );

    foreach_page()
    {
//        checked_call( getMOTIONPLAN_PVTCONFIG( _i, _axPage, mMOTIONPLAN_PVTCONFIG[_i]+_j ) );
//        checked_call( getMOTIONPLAN_PRESETSTATE( _i,
//                                                 (MRQ_MOTIONPLAN_PRESETSTATE)_axPage,
//                                                 mMOTIONPLAN_PRESETSTATE[_i]+_j ) );

        checked_call( getMOTIONPLAN_EXECUTEMODE( _i, _axPage, mMOTIONPLAN_EXECUTEMODE[_i]+_j ) );

        checked_call( getMOTIONPLAN_PLANMODE( _i, _axPage, mMOTIONPLAN_PLANMODE[_i]+_j ) );
        checked_call( getMOTIONPLAN_MOTIONMODE( _i, _axPage, mMOTIONPLAN_MOTIONMODE[_i]+_j) );
        checked_call( getMOTIONPLAN_MODIFYDUTY( _i, _axPage, mMOTIONPLAN_MODIFYDUTY[_i]+_j) );
        checked_call( getMOTIONPLAN_REMAINPOINT( _i, _axPage, mMOTIONPLAN_REMAINPOINT[_i]+_j) );

        checked_call( getMOTIONPLAN_CYCLENUM( _i, _axPage, mMOTIONPLAN_CYCLENUM[_i]+_j) );

        checked_call( getMOTIONPLAN_WARNPOINT( _i, _axPage, mMOTIONPLAN_WARNPOINT[_i]+_j) );
        checked_call( getMOTIONPLAN_ENDSTATE( _i, _axPage, mMOTIONPLAN_ENDSTATE[_i]+_j) );

        checked_call( getMOTIONPLAN_BUFFERSIZE( _i, _axPage, mMOTIONPLAN_BUFFERSIZE[_i]+_j) );

        checked_call( getMOTIONPLAN_STOPMODE( _i, _axPage, mMOTIONPLAN_STOPMODE[_i]+_j) );
        checked_call( getMOTIONPLAN_STOPDISTANCE( _i, _axPage, mMOTIONPLAN_STOPDISTANCE[_i]+_j) );
        checked_call( getMOTIONPLAN_STOPTIME( _i, _axPage, mMOTIONPLAN_STOPTIME[_i]+_j) );

        checked_call( getMOTIONPLAN_OOSLINESTATE( _i, _axPage, mMOTIONPLAN_OOSLINESTATE[_i]+_j) );
        checked_call( getMOTIONPLAN_OOSLINEOUTNUM( _i, _axPage, mMOTIONPLAN_OOSLINEOUTNUM[_i]+_j) );
        checked_call( getMOTIONPLAN_OOSLINERESPONSE( _i, _axPage, mMOTIONPLAN_OOSLINERESPONSE[_i]+_j) );

        checked_call( getMOTIONPLAN_OOSTOTALSTATE( _i, _axPage, mMOTIONPLAN_OOSTOTALSTATE[_i]+_j) );
        checked_call( getMOTIONPLAN_OOSTOTALOUTNUM( _i, _axPage, mMOTIONPLAN_OOSTOTALOUTNUM[_i]+_j) );
        checked_call( getMOTIONPLAN_OOSTOTALRESPONSE( _i, _axPage, mMOTIONPLAN_OOSTOTALRESPONSE[_i]+_j) );

        MRQ_PROGRESS( 30 + (_i*regions() + _j)*20/(axes()*regions()) ,
                  ("MOTORPLAN") );
    }
    end_foreach_page()

MRQ_PROGRESS( 50, ("TRIGGER") );

    for ( int i = 0; i < axes(); i++ )
    {
        for ( int j = 0; j < regions(); j++ )
        {
            checked_call( getREPORT_STATE( i, (MRQ_REPORT_STATE)j, mREPORT_STATE[i]+j) );
            checked_call( getREPORT_PERIOD( i, (MRQ_REPORT_STATE)j, mREPORT_PERIOD[i]+j) );
        }
        Q_ASSERT( i < sizeof_array(mTRIGGER_MODE) );
        checked_call( getTRIGGER_MODE( i, mTRIGGER_MODE+i) );
//        checked_call( getTRIGGER_PATTSTATE( i, mTRIGGER_PATTSTATE+i) );

//        checked_call( getTRIGGER_PATTERN( i,
//                                          mTRIGGER_PATTERN+i,
//                                          mTRIGGER_PATTERN1+i,
//                                          mTRIGGER_PATTERN2+i,
//                                          mTRIGGER_PATTERN3+i,
//                                          mTRIGGER_PATTERN4+i
//                                          ) ); U16?

//        checked_call( getTRIGGER_PATTRESP( i, mTRIGGER_PATTRESP+i) );
//        checked_call( getTRIGGER_PATTSMODE( i, mTRIGGER_PATTSMODE+i) );
//        checked_call( getTRIGGER_PATTSPERIOD( i, mTRIGGER_PATTSPERIOD+i) );

        int trigId;
        for( int j = 0; j < mTrigSrcs; j++ )
        {
            trigId = mTrigIdsList.at( j );

            checked_call( getTRIGGER_LEVELSTATE( i, (MRQ_TRIGGER_LEVELSTATE)trigId, mTRIGGER_LEVELSTATE[i]+trigId) );
            checked_call( getTRIGGER_LEVELTYPE( i, (MRQ_TRIGGER_LEVELSTATE)trigId, mTRIGGER_LEVELTYPE[i]+trigId) );
            checked_call( getTRIGGER_LEVELRESP( i, (MRQ_TRIGGER_LEVELSTATE)trigId, mTRIGGER_LEVELRESP[i]+trigId) );
            checked_call( getTRIGGER_LEVELSMODE( i, (MRQ_TRIGGER_LEVELSTATE)trigId, mTRIGGER_LEVELSMODE[i]+trigId) );
            checked_call( getTRIGGER_LEVELSPERIOD( i, (MRQ_TRIGGER_LEVELSTATE)trigId, mTRIGGER_LEVELSPERIOD[i]+trigId) );

            if ( mbRunWaveAble )
            { checked_call( getTRIGGER_LEVELRUNWAVE( i, (MRQ_TRIGGER_LEVELSTATE)trigId, mTRIGGER_LEVELRUNWAVE[i]+trigId) ); }
        }

        if ( mDriverId == VRobot::motor_driver_262 )
        {
            checked_call( getDRIVER_TYPE( i, mDRIVER_TYPE+i) );
            for( int j = 0; j < 3; j++ )
            {
                Q_ASSERT( j < sizeof_array(mDRIVER_STATEREG[0]) );
                checked_call( getDRIVER_STATEREG( i, (MRQ_DRIVER_STATEREG)j, mDRIVER_STATEREG[i]+j) );
            }
            checked_call( getDRIVER_CURRENT( i, mDRIVER_CURRENT+i) );

            checked_call( getDRIVER_MICROSTEPS( i, mDRIVER_MICROSTEPS+i) );
            checked_call( getDRIVER_STATE( i, mDRIVER_STATE+i) );


            checked_call( getDRIVER_IDLECURRENT( i, mDRIVER_IDLECURRENT+i) );
            checked_call( getDRIVER_SWITCHTIME( i, mDRIVER_SWITCHTIME+i) );
            checked_call( getDRIVER_MINICURRRATIO( i, mDRIVER_MINICURRRATIO+i) );
        }
        else if ( mDriverId == VRobot::motor_driver_820 )
        {
            checked_call( getNEWDRIVER_TYPE( i, mNEWDRIVER_TYPE+i) );
            checked_call( getNEWDRIVER_CURRENT( &mNEWDRIVER_CURRENT ) );
            checked_call( getNEWDRIVER_MICROSTEPS( &mNEWDRIVER_MICROSTEPS ) );
            checked_call( getNEWDRIVER_STATE( i, mNEWDRIVER_STATE + i ) );
        }
        else
        { Q_ASSERT(false); }
    }

MRQ_PROGRESS( 60, ("DO") );

    for ( int i = 0; i < mDOs; i++ )
    {
        Q_ASSERT( i < sizeof_array(mDIGITALOUTPUT_STATE) );
        checked_call( getDIGITALOUTPUT_STATE( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_STATE + i) );
        checked_call( getDIGITALOUTPUT_SIGNAL( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_SIGNAL + i) );
        checked_call( getDIGITALOUTPUT_POLARITY( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_POLARITY + i) );

        checked_call( getDIGITALOUTPUT_SOURCE( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_SOURCE + i) );
        checked_call( getDIGITALOUTPUT_CONDITION( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_CONDITION + i) );
        checked_call( getDIGITALOUTPUT_PERIOD( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_PERIOD + i) );
        checked_call( getDIGITALOUTPUT_DUTY( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_DUTY + i) );
    }

MRQ_PROGRESS( 65, ("ISO") );

    for( int i = 0; i < mISOs; i++ )
    {
        Q_ASSERT( i < sizeof_array(mISOLATOROUTPUT_STATE) );
        checked_call( getISOLATOROUTPUT_STATE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_STATE + i) );
        checked_call( getISOLATOROUTPUT_SOURCE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_SOURCE + i) );
        checked_call( getISOLATOROUTPUT_CONDITION( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_CONDITION + i) );
        checked_call( getISOLATOROUTPUT_RESPONSE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_RESPONSE + i) );
    }
MRQ_PROGRESS( 70, ("SENSOR") );

    //! sensor uart
    for ( int i = 0; i < uarts(); i++ )
    {
        Q_ASSERT( i < sizeof_array(mSENSORUART_BAUD) );
        checked_call( getSENSORUART_BAUD( (MRQ_SENSORUART_BAUD)i, mSENSORUART_BAUD + i) );
        checked_call( getSENSORUART_WORDLEN( (MRQ_SENSORUART_BAUD)i, mSENSORUART_WORDLEN + i) );
        checked_call( getSENSORUART_FLOWCTL( (MRQ_SENSORUART_BAUD)i, mSENSORUART_FLOWCTL + i) );
        checked_call( getSENSORUART_PARITY( (MRQ_SENSORUART_BAUD)i, mSENSORUART_PARITY + i) );

        checked_call( getSENSORUART_STOPBIT( (MRQ_SENSORUART_BAUD)i, mSENSORUART_STOPBIT + i) );

        for ( int j = 0; j < uartSensors(); j++ )
        {
            Q_ASSERT( j < sizeof_array(mSENSORUART_STATE[0]) );
            checked_call( getSENSORUART_STATE( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_STATE[i] + j) );
            checked_call( getSENSORUART_SOF( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_SOF[i] + j) );
            checked_call( getSENSORUART_FRAMELEN( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_FRAMELEN[i] + j) );

            checked_call( getSENSORUART_RECEIVENUM( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_RECEIVENUM[i] + j) );
            checked_call( getSENSORUART_SWITCHTIME( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_SWITCHTIME[i] + j) );
        }
    }

    //! tunning
    if ( mbTunningAble )
    {
        MRQ_PROGRESS( 72, ("TUNNING") );

        for ( int i = 0; i < axes(); i++ )
        {
            checked_call( getTUNING_STATE( i, mTUNING_STATE + i ) );
            checked_call( getTUNING_MINICURRRATIO( i, mTUNING_MINICURRRATIO + i ) );
            checked_call( getTUNING_ENERGYEFFIC( i, mTUNING_ENERGYEFFIC + i, mTUNING_ENERGYEFFIC1 + i) );
            checked_call( getTUNING_CURRREGULATE( i, mTUNING_CURRREGULATE + i, mTUNING_CURRREGULATE1 + i ) );
        }
    }

MRQ_PROGRESS( 75, ("ISI") );

    for ( int i = 0; i < mISIs; i++ )
    {
        Q_ASSERT( i < 1 );
        checked_call( getISOLATORIN_STATE( &mISOLATORIN_STATE ) );
        checked_call( getISOLATORIN_TYPE( &mISOLATORIN_TYPE ) );
        checked_call( getISOLATORIN_RESPONSE( &mISOLATORIN_RESPONSE ) );
        checked_call( getISOLATORIN_RESPCHAN( &mISOLATORIN_RESPCHAN ) );

        checked_call( getISOLATORIN_SMODE( &mISOLATORIN_SMODE ) );
        checked_call( getISOLATORIN_SPERIOD( &mISOLATORIN_SPERIOD ) );
    }

    if ( mAlarms > 0 )
    {
    MRQ_PROGRESS( 80, ("ENCODER ALARM") );
        for ( int i = 0; i < mAbsEncoderAlarms; i++ )
        {
            checked_call( getABSENCALARM_STATE( (MRQ_IDENTITY_LABEL_1)i,
                                                &mABSENCALARM_STATE[i] ) );
            checked_call( getABSENCALARM_UPLIMIT( (MRQ_IDENTITY_LABEL_1)i,
                                                &mABSENCALARM_UPLIMIT[i] ) );
            checked_call( getABSENCALARM_DOWNLIMIT( (MRQ_IDENTITY_LABEL_1)i,
                                                &mABSENCALARM_DOWNLIMIT[i] ) );
            checked_call( getABSENCALARM_ZEROVALUE( (MRQ_IDENTITY_LABEL_1)i,
                                                &mABSENCALARM_ZEROVALUE[i] ) );

            checked_call( getABSENCALARM_ZEROPOSITION( (MRQ_IDENTITY_LABEL_1)i,
                                                &mABSENCALARM_ZEROPOSITION[i] ) );
        }
        checked_call( getABSENCALARM_RESPONSE(
                                            &mABSENCALARM_RESPONSE ) );


    MRQ_PROGRESS( 85, ("DISTANCE ALARM") );
        for ( int i = 0; i < mDistanceAlarms; i++ )
        {
            checked_call( getDISTANCEALARM_STATE( (MRQ_IDENTITY_LABEL_1)i,
                                                &mDISTANCEALARM_STATE[i] ) );
            checked_call( getDISTANCEALARM_ALARM1DIST( (MRQ_IDENTITY_LABEL_1)i,
                                                &mDISTANCEALARM_ALARM1DIST[i] ) );
            checked_call( getDISTANCEALARM_ALARM2DIST( (MRQ_IDENTITY_LABEL_1)i,
                                                &mDISTANCEALARM_ALARM2DIST[i] ) );
            checked_call( getDISTANCEALARM_ALARM3DIST( (MRQ_IDENTITY_LABEL_1)i,
                                                &mDISTANCEALARM_ALARM3DIST[i] ) );
        }
    }

    //! analog input
    if ( ains() > 0 )
    {
        MRQ_PROGRESS( 90, ("ANALOG INPUT") );
        checked_call( getANALOGIN_STATE( &mANALOGIN_STATE) );

        checked_call( getANALOGIN_THRESHOLDH( &mANALOGIN_THRESHOLDH) );
        checked_call( getANALOGIN_THRESHOLDL( &mANALOGIN_THRESHOLDL) );

        checked_call( getANALOGIN_RESPONSEH( &mANALOGIN_RESPONSEH) );
        checked_call( getANALOGIN_RESPONSEL( &mANALOGIN_RESPONSEL) );
    }

    //! otp
    if ( temperatures() > 0 )
    {
        MRQ_PROGRESS( 95, ("OTP") );

        checked_call( getOTP_STATE( &mOTP_STATE) );

        checked_call( getOTP_THRESHOLD( &mOTP_THRESHOLD) );
        checked_call( getOTP_RESPONSE( &mOTP_RESPONSE) );

        checked_call( getOTP_PERIOD( &mOTP_PERIOD ) );
    }

MRQ_PROGRESS_HIDE();

    return ret;
}

}
