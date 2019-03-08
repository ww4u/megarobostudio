#include "deviceMRQ.h"

//#ifdef DEVICE_EMIT_SIGNAL
//#include "../../app/dpcobj.h"
//#endif


namespace MegaDevice
{


#define MRQ_PROGRESS_DOWN( prog, info )      MRQ_PROGRESS( prog, info );

//                                    QThread::msleep( 50 );

int deviceMRQ::_applySetting()
{
    int ret=-1;
#if 1

MRQ_PROGRESS_DOWN( 0, ("LINK") );

    checked_call( setLINK_INTFC(mLINK_INTFC) );
    checked_call( setSYSTEM_WORKMODE(mSYSTEM_WORKMODE) );
    checked_call( setSYSTEM_POWERON(mSYSTEM_POWERON));

    checked_call( setRS232_BAUD(mRS232_BAUD) );
    checked_call( setRS232_WORDLEN(mRS232_WORDLEN) );
    checked_call( setRS232_FLOWCTL(mRS232_FLOWCTL) );
    checked_call( setRS232_PARITY(mRS232_PARITY) );

    checked_call( setRS232_STOPBIT(mRS232_STOPBIT) );

MRQ_PROGRESS( 10, ("MOTION") );

    //! state
    foreach_page()
    {
//        checked_call( setMOTION_STATE( _i, _axPage, mMOTION_STATE[_i] + _j) );
    }
    end_foreach_page()

    //! motion 1--4
    for ( int i = 0; i < axes(); i++ )
    {
//        checked_call( setMOTION_STATEREPORT( i, mMOTION_STATEREPORT[i]) );

        checked_call( setMOTION_STARTSOURCE( i, mMOTION_STARTSOURCE[i]) );
        checked_call( setMOTION_STARTTYPE( i, mMOTION_STARTTYPE[i]) );
        checked_call( setMOTION_MAXSPEED( i, mMOTION_MAXSPEED[i]) );
        checked_call( setMOTION_MINSPEED( i, mMOTION_MINSPEED[i]) );

        checked_call( setMOTION_MAXPOSITION( i, mMOTION_MAXPOSITION[i]) );
        checked_call( setMOTION_MINPOSITION( i, mMOTION_MINPOSITION[i]) );
        checked_call( setMOTION_MAXTORQUE( i, mMOTION_MAXTORQUE[i]) );
        checked_call( setMOTION_MINTORQUE( i, mMOTION_MINTORQUE[i]) );

        checked_call( setMOTION_MAXPOSITION( i, mMOTION_MAXPOSITION[i]) );
        checked_call( setMOTION_MINPOSITION( i, mMOTION_MINPOSITION[i]) );
        checked_call( setMOTION_MAXTORQUE( i, mMOTION_MAXTORQUE[i]) );
        checked_call( setMOTION_MINTORQUE( i, mMOTION_MINTORQUE[i]) );

        checked_call( setMOTION_MAXACCELERATION( i, mMOTION_MAXACCELERATION[i]) );
        checked_call( setMOTION_MINACCELERATION( i, mMOTION_MINACCELERATION[i]) );
        checked_call( setMOTION_ORIGIN( i, mMOTION_ORIGIN[i]) );

        checked_call( setMOTION_OFFSETSTATE( i, mMOTION_OFFSETSTATE[i]) );

        MRQ_PROGRESS_DOWN( 20, ("MOTOR") );
    }

    checked_call( setIDENTITY_DISTDEVICE( mIDENTITY_DISTDEVICE ) );
    for ( int i = 0; i < axes(); i++ )
    {
        //! motor
        checked_call( setMOTOR_STEPANGLE( i, mMOTOR_STEPANGLE[i]) );
        checked_call( setMOTOR_TYPE( i, mMOTOR_TYPE[i]) );

        checked_call( setMOTOR_POSITIONUNIT( i, mMOTOR_POSITIONUNIT[i]) );
        checked_call( setMOTOR_GEARRATIONUM( i, mMOTOR_GEARRATIONUM[i]) );
        checked_call( setMOTOR_GEARRATIODEN( i, mMOTOR_GEARRATIODEN[i]) );
        checked_call( setMOTOR_LEAD( i, mMOTOR_LEAD[i]) );

        checked_call( setMOTOR_PEAKSPEED( i, mMOTOR_PEAKSPEED[i]) );
        checked_call( setMOTOR_PEAKACCELERATION( i, mMOTOR_PEAKACCELERATION[i]) );
        checked_call( setMOTOR_SIZE( i, mMOTOR_SIZE[i]) );
        checked_call( setMOTOR_VOLTAGE( i, mMOTOR_VOLTAGE[i]) );

        checked_call( setMOTOR_CURRENT( i, mMOTOR_CURRENT[i]) );
        checked_call( setMOTOR_BACKLASH( i, mMOTOR_BACKLASH[i]) );

        //! encoder
        if ( encoderAble() )
        {
            checked_call( setENCODER_LINENUM( i, mENCODER_LINENUM[i]) );
            checked_call( setENCODER_CHANNELNUM( i, mENCODER_CHANNELNUM[i]) );

            checked_call( setENCODER_TYPE( i, mENCODER_TYPE[i]) );
            checked_call( setENCODER_MULTIPLE( i, mENCODER_MULTIPLE[i]) );
            checked_call( setENCODER_STATE( i, mENCODER_STATE[i]) );
            checked_call( setENCODER_FEEDBACKRATIO( i, mENCODER_FEEDBACKRATIO[i]) );
        }

        MRQ_PROGRESS_DOWN( 30, ("MOTORPLAN") );
    }

    foreach_page()
    {
        checked_call( setMOTIONPLAN_EXECUTEMODE( _i, _axPage, mMOTIONPLAN_EXECUTEMODE[_i][_j] ) );
        checked_call( setMOTIONPLAN_FIFOBUFFERTIME( _i, _axPage, mMOTIONPLAN_FIFOBUFFERTIME[_i][_j] ) );

        checked_call( setMOTIONPLAN_PLANMODE( _i, _axPage, mMOTIONPLAN_PLANMODE[_i][_j] ) );
        checked_call( setMOTIONPLAN_MOTIONMODE( _i, _axPage, mMOTIONPLAN_MOTIONMODE[_i][_j]) );
        checked_call( setMOTIONPLAN_MODIFYDUTY( _i, _axPage, mMOTIONPLAN_MODIFYDUTY[_i][_j]) );

        checked_call( setMOTIONPLAN_CYCLENUM( _i, _axPage, mMOTIONPLAN_CYCLENUM[_i][_j]) );

        checked_call( setMOTIONPLAN_WARNPOINT( _i, _axPage, mMOTIONPLAN_WARNPOINT[_i][_j]) );
        checked_call( setMOTIONPLAN_ENDSTATE( _i, _axPage, mMOTIONPLAN_ENDSTATE[_i][_j]) );

        checked_call( setMOTIONPLAN_STOPMODE( _i, _axPage, mMOTIONPLAN_STOPMODE[_i][_j]) );
        checked_call( setMOTIONPLAN_STOPDISTANCE( _i, _axPage, mMOTIONPLAN_STOPDISTANCE[_i][_j]) );
        checked_call( setMOTIONPLAN_STOPTIME( _i, _axPage, mMOTIONPLAN_STOPTIME[_i][_j]) );

        checked_call( setMOTIONPLAN_OOSLINESTATE( _i, _axPage, mMOTIONPLAN_OOSLINESTATE[_i][_j]) );
        checked_call( setMOTIONPLAN_OOSLINEOUTNUM( _i, _axPage, mMOTIONPLAN_OOSLINEOUTNUM[_i][_j]) );
        checked_call( setMOTIONPLAN_OOSLINERESPONSE( _i, _axPage, mMOTIONPLAN_OOSLINERESPONSE[_i][_j]) );

        checked_call( setMOTIONPLAN_OOSTOTALSTATE( _i, _axPage, mMOTIONPLAN_OOSTOTALSTATE[_i][_j]) );
        checked_call( setMOTIONPLAN_OOSTOTALOUTNUM( _i, _axPage, mMOTIONPLAN_OOSTOTALOUTNUM[_i][_j]) );
        checked_call( setMOTIONPLAN_OOSTOTALRESPONSE( _i, _axPage, mMOTIONPLAN_OOSTOTALRESPONSE[_i][_j]) );

        MRQ_PROGRESS_DOWN( 30 + (_i*regions() + _j)*20/(axes()*regions()), ("MOTORPLAN") );
    }
    end_foreach_page()
#endif
#if 1
    for ( int i = 0; i < axes(); i++ )
    {
        for ( int j = 0; j < regions(); j++ )
        {
            checked_call( setREPORT_STATE( i, (MRQ_REPORT_STATE)j, mREPORT_STATE[i][j]) );
            checked_call( setREPORT_PERIOD( i, (MRQ_REPORT_STATE)j, mREPORT_PERIOD[i][j]) );
        }
        Q_ASSERT( i < sizeof_array(mTRIGGER_MODE) );
        checked_call( setTRIGGER_MODE( i, mTRIGGER_MODE[i]) );
        checked_call( setTRIGGER_PATTSTATE( i, mTRIGGER_PATTSTATE[i]) );

        checked_call( setTRIGGER_PATTRESP( i, mTRIGGER_PATTRESP[i]) );
        checked_call( setTRIGGER_PATTSMODE( i, mTRIGGER_PATTSMODE[i]) );
        checked_call( setTRIGGER_PATTSPERIOD( i, mTRIGGER_PATTSPERIOD[i]) );

        MRQ_TRIGGER_LEVELSTATE trigId;
        for( int j = 0; j < mTrigSrcs; j++ )
        {
            trigId = (MRQ_TRIGGER_LEVELSTATE)mTrigIdsList.at( j );

            checked_call( setTRIGGER_LEVELSTATE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSTATE[i][j]) );
            checked_call( setTRIGGER_LEVELTYPE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELTYPE[i][j]) );
            checked_call( setTRIGGER_LEVELRESP( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELRESP[i][j]) );
            checked_call( setTRIGGER_LEVELSMODE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSMODE[i][j]) );
            checked_call( setTRIGGER_LEVELSPERIOD( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSPERIOD[i][j]) );

            if ( mbRunWaveAble )
            { checked_call( setTRIGGER_LEVELRUNWAVE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELRUNWAVE[i][j]) ); }
        }

        MRQ_PROGRESS_DOWN( 50, ("TRIGGER") );

        if ( mDriverId == VRobot::motor_driver_262 )
        {
            checked_call( setDRIVER_CURRENT( i, mDRIVER_CURRENT[i]) );

            checked_call( setDRIVER_MICROSTEPS( i, mDRIVER_MICROSTEPS[i]) );
            checked_call( setDRIVER_STATE( i, mDRIVER_STATE[i]) );


            checked_call( setDRIVER_IDLECURRENT( i, mDRIVER_IDLECURRENT[i]) );
            checked_call( setDRIVER_SWITCHTIME( i, mDRIVER_SWITCHTIME[i]) );
            checked_call( setDRIVER_MINICURRRATIO( i, mDRIVER_MINICURRRATIO[i]) );
        }
        else if ( mDriverId == VRobot::motor_driver_820 )
        {
            checked_call( setNEWDRIVER_CURRENT( mNEWDRIVER_CURRENT ) );
            checked_call( setNEWDRIVER_MICROSTEPS( mNEWDRIVER_MICROSTEPS ) );
            checked_call( setNEWDRIVER_STATE( i, mNEWDRIVER_STATE[i] ) );
        }
        else
        { Q_ASSERT(false); }

        MRQ_PROGRESS_DOWN( 55, ("DRIVER") );
    }
#endif
#if 1
    for ( int i = 0; i < mDOs; i++ )
    {
        Q_ASSERT( i < sizeof_array(mDIGITALOUTPUT_STATE) );
        checked_call( setDIGITALOUTPUT_STATE( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_STATE[i]) );
        checked_call( setDIGITALOUTPUT_SIGNAL( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_SIGNAL[i]) );
        checked_call( setDIGITALOUTPUT_POLARITY( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_POLARITY[i]) );

        checked_call( setDIGITALOUTPUT_SOURCE( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_SOURCE[i]) );
        checked_call( setDIGITALOUTPUT_CONDITION( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_CONDITION[i]) );
        checked_call( setDIGITALOUTPUT_PERIOD( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_PERIOD[i]) );
        checked_call( setDIGITALOUTPUT_DUTY( (MRQ_DIGITALOUTPUT_STATE)i, mDIGITALOUTPUT_DUTY[i]) );

        MRQ_PROGRESS_DOWN( 60, ("DO") );
    }

    for( int i = 0; i < mISOs; i++ )
    {
        Q_ASSERT( i < sizeof_array(mISOLATOROUTPUT_STATE) );
        checked_call( setISOLATOROUTPUT_STATE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_STATE[i]) );
        checked_call( setISOLATOROUTPUT_SOURCE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_SOURCE[i]) );
        checked_call( setISOLATOROUTPUT_CONDITION( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_CONDITION[i]) );
        checked_call( setISOLATOROUTPUT_RESPONSE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_RESPONSE[i]) );

        MRQ_PROGRESS_DOWN( 65, ("ISO") );
    }

    //! sensor uart
    for ( int i = 0; i < uarts(); i++ )
    {
        Q_ASSERT( i < sizeof_array(mSENSORUART_BAUD) );
        checked_call( setSENSORUART_BAUD( (MRQ_SENSORUART_BAUD)i, mSENSORUART_BAUD[i]) );
        checked_call( setSENSORUART_WORDLEN( (MRQ_SENSORUART_BAUD)i, mSENSORUART_WORDLEN[i]) );
        checked_call( setSENSORUART_FLOWCTL( (MRQ_SENSORUART_BAUD)i, mSENSORUART_FLOWCTL[i]) );
        checked_call( setSENSORUART_PARITY( (MRQ_SENSORUART_BAUD)i, mSENSORUART_PARITY[i]) );

        checked_call( setSENSORUART_STOPBIT( (MRQ_SENSORUART_BAUD)i, mSENSORUART_STOPBIT[i]) );

        for ( int j = 0; j < uartSensors(); j++ )
        {
            Q_ASSERT( j < sizeof_array(mSENSORUART_STATE[0]) );
            checked_call( setSENSORUART_STATE( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_STATE[i][j]) );
            checked_call( setSENSORUART_SOF( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_SOF[i][j]) );
            checked_call( setSENSORUART_FRAMELEN( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_FRAMELEN[i][j]) );

            checked_call( setSENSORUART_RECEIVENUM( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_RECEIVENUM[i][j]) );
            checked_call( setSENSORUART_SWITCHTIME( (MRQ_SENSORUART_BAUD)i, (MRQ_IDENTITY_LABEL_1)j, mSENSORUART_SWITCHTIME[i][j]) );

            MRQ_PROGRESS_DOWN( 70, ("SENSOR") );
        }
    }

    //! tunning
    if ( mbTunningAble )
    {
        for ( int i = 0; i < axes(); i++ )
        {
            checked_call( setTUNING_STATE( i, mTUNING_STATE[i] ) );
            checked_call( setTUNING_MINICURRRATIO( i, mTUNING_MINICURRRATIO[i] ) );
            checked_call( setTUNING_ENERGYEFFIC( i, mTUNING_ENERGYEFFIC[i], mTUNING_ENERGYEFFIC1[i] ) );
            checked_call( setTUNING_CURRREGULATE( i, mTUNING_CURRREGULATE[i], mTUNING_CURRREGULATE1[i] ) );
        }
        MRQ_PROGRESS( 72, ("TUNNING") );
    }

    for ( int i = 0; i < mISIs; i++ )
    {
        Q_ASSERT( i < 1 );
        checked_call( setISOLATORIN_STATE( mISOLATORIN_STATE ) );
        checked_call( setISOLATORIN_TYPE( mISOLATORIN_TYPE ) );
        checked_call( setISOLATORIN_RESPONSE( mISOLATORIN_RESPONSE ) );
        checked_call( setISOLATORIN_RESPCHAN( mISOLATORIN_RESPCHAN ) );

        checked_call( setISOLATORIN_SMODE( mISOLATORIN_SMODE ) );
        checked_call( setISOLATORIN_SPERIOD( mISOLATORIN_SPERIOD ) );

        MRQ_PROGRESS_DOWN( 75, ("ISI") );
    }

    if ( mAlarms > 0 )
    {
    MRQ_PROGRESS_DOWN( 80, ("ENCODER ALARM") );
        for ( int i = 0; i < mAbsEncoderAlarms; i++ )
        {
            checked_call( setABSENCALARM_STATE( (MRQ_IDENTITY_LABEL_1)i,
                                                mABSENCALARM_STATE[i] ) );
        }
        checked_call( setABSENCALARM_RESPONSE(
                                            mABSENCALARM_RESPONSE ) );

    MRQ_PROGRESS_DOWN( 85, ("DISTANCE ALARM") );
        for ( int i = 0; i < mDistanceAlarms; i++ )
        {
            checked_call( setDISTANCEALARM_STATE( (MRQ_IDENTITY_LABEL_1)i,
                                                mDISTANCEALARM_STATE[i] ) );
            checked_call( setDISTANCEALARM_ALARM1DIST( (MRQ_IDENTITY_LABEL_1)i,
                                                mDISTANCEALARM_ALARM1DIST[i] ) );
            checked_call( setDISTANCEALARM_ALARM2DIST( (MRQ_IDENTITY_LABEL_1)i,
                                                mDISTANCEALARM_ALARM2DIST[i] ) );
            checked_call( setDISTANCEALARM_ALARM3DIST( (MRQ_IDENTITY_LABEL_1)i,
                                                mDISTANCEALARM_ALARM3DIST[i] ) );
        }
    }

    //! analog input
    if ( ains() > 0 )
    {
        MRQ_PROGRESS_DOWN( 90, ("ANALOG INPUT") );
        checked_call( setANALOGIN_STATE( mANALOGIN_STATE) );

        checked_call( setANALOGIN_THRESHOLDH( mANALOGIN_THRESHOLDH) );
        checked_call( setANALOGIN_THRESHOLDL( mANALOGIN_THRESHOLDL) );

        checked_call( setANALOGIN_RESPONSEH( mANALOGIN_RESPONSEH) );
        checked_call( setANALOGIN_RESPONSEL( mANALOGIN_RESPONSEL) );
    }

    //! otp
    if ( temperatures() > 0 )
    {
        MRQ_PROGRESS_DOWN( 95, ("OTP") );

        checked_call( setOTP_STATE( mOTP_STATE) );

        checked_call( setOTP_THRESHOLD( mOTP_THRESHOLD) );
        checked_call( setOTP_RESPONSE( mOTP_RESPONSE) );

        checked_call( setOTP_PERIOD( mOTP_PERIOD ) );
    }
#endif
MRQ_PROGRESS_HIDE();

    return ret;
}

}
