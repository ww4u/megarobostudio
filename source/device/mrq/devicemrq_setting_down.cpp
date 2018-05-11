#include "deviceMRQ.h"

//#ifdef DEVICE_EMIT_SIGNAL
//#include "../../app/dpcobj.h"
//#endif


namespace MegaDevice
{

int deviceMRQ::_applySetting()
{
    int ret=-1;

MRQ_PROGRESS( 0, ("LINK") );

    checked_call( setLINK_INTFC(mLINK_INTFC) );
    checked_call( setSYSTEM_WORKMODE(mSYSTEM_WORKMODE) );
    checked_call( setSYSTEM_POWERON(mSYSTEM_POWERON));

    checked_call( setRS232_BAUD(mRS232_BAUD) );
    checked_call( setRS232_WORDLEN(mRS232_WORDLEN) );
    checked_call( setRS232_FLOWCTL(mRS232_FLOWCTL) );
    checked_call( setRS232_PARITY(mRS232_PARITY) );

    checked_call( setRS232_STOPBIT(mRS232_STOPBIT) );
//    checked_call( setCAN_TYPE(mCAN_TYPE) );
//    checked_call( setCAN_BAUD(mCAN_BAUD) );
//    checked_call( setCAN_GROUP(mCAN_GROUP) );

//    checked_call( setCAN_SENDID(mCAN_SENDID) );
//    checked_call( setCAN_RECEIVEID(mCAN_RECEIVEID) );

//    checked_call( setCAN_GROUPID1(mCAN_GROUPID1) );
//    checked_call( setCAN_GROUPID2(mCAN_GROUPID2) );
//    checked_call( setCAN_BROADCASTID(mCAN_BROADCASTID) );

//    checked_call( setCAN_NETMANAGESTATE(mCAN_NETMANAGESTATE) );
//    checked_call( setCAN_NETMANAGEHASH(mCAN_NETMANAGEHASH) );

//    checked_call( setCLOCK_FREQUENCY(mCLOCK_FREQUENCY) );
//    checked_call( setCLOCK_SYNCREGISTER(mCLOCK_SYNCREGISTER) );
//    checked_call( setCLOCK_STARTTYPE(mCLOCK_STARTTYPE) );

//    checked_call( setCLOCK_SYNCSTATE(mCLOCK_SYNCSTATE) );
//    checked_call( setCLOCK_COUNT(mCLOCK_COUNT) );

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
//        checked_call( setMOTION_PVTSTEPS( i, mMOTION_PVTSTEPS[i]) );
//        checked_call( setMOTION_COUNTSTEPS( i, mMOTION_COUNTSTEPS[i]) );
//        checked_call( setMOTION_PVTCIRCLE( i, mMOTION_PVTCIRCLE[i]) );

//        checked_call( setMOTION_COUNTCIRCLE( i, mMOTION_COUNTCIRCLE[i]) );
//        checked_call( setMOTION_ABCOUNT( i, mMOTION_ABCOUNT[i]) );
//        checked_call( setMOTION_REVMOTION( i, mMOTION_REVMOTION[i]) );
    }

MRQ_PROGRESS( 20, ("MOTOR") );

    checked_call( setIDENTITY_DISTDEVICE( mIDENTITY_DISTDEVICE ) );
    for ( int i = 0; i < axes(); i++ )
    {
        checked_call( setIDENTITY_GROUP( i, MRQ_IDENTITY_GROUP_GROUP1, mIDENTITY_GROUP[i][0] ) );
        checked_call( setIDENTITY_GROUP( i, MRQ_IDENTITY_GROUP_GROUP2, mIDENTITY_GROUP[i][1] ) );

//        checked_call( setIDENTITY_LABEL( i,
//                                         mIDENTITY_LABEL + i,
//                                         mIDENTITY_LABEL1+ i ) );

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
        checked_call( setENCODER_LINENUM( i, mENCODER_LINENUM[i]) );
        checked_call( setENCODER_CHANNELNUM( i, mENCODER_CHANNELNUM[i]) );

        checked_call( setENCODER_TYPE( i, mENCODER_TYPE[i]) );
        checked_call( setENCODER_MULTIPLE( i, mENCODER_MULTIPLE[i]) );
        checked_call( setENCODER_STATE( i, mENCODER_STATE[i]) );
        checked_call( setENCODER_FEEDBACKRATIO( i, mENCODER_FEEDBACKRATIO[i]) );
    }

MRQ_PROGRESS( 30, ("MOTORPLAN") );

    foreach_page()
    {
//        checked_call( setMOTIONPLAN_PVTCONFIG( _i, _axPage, mMOTIONPLAN_PVTCONFIG[_i][_j] ) );
//        checked_call( setMOTIONPLAN_PRESETSTATE( _i,
//                                                 (MRQ_MOTIONPLAN_PRESETSTATE)_axPage,
//                                                 mMOTIONPLAN_PRESETSTATE[_i][_j] ) );

        checked_call( setMOTIONPLAN_EXECUTEMODE( _i, _axPage, mMOTIONPLAN_EXECUTEMODE[_i][_j] ) );

        checked_call( setMOTIONPLAN_PLANMODE( _i, _axPage, mMOTIONPLAN_PLANMODE[_i][_j] ) );
        checked_call( setMOTIONPLAN_MOTIONMODE( _i, _axPage, mMOTIONPLAN_MOTIONMODE[_i][_j]) );
        checked_call( setMOTIONPLAN_MODIFYDUTY( _i, _axPage, mMOTIONPLAN_MODIFYDUTY[_i][_j]) );
//        checked_call( setMOTIONPLAN_REMAINPOINT( _i, _axPage, mMOTIONPLAN_REMAINPOINT[_i][_j]) );

        checked_call( setMOTIONPLAN_CYCLENUM( _i, _axPage, mMOTIONPLAN_CYCLENUM[_i][_j]) );

        checked_call( setMOTIONPLAN_WARNPOINT( _i, _axPage, mMOTIONPLAN_WARNPOINT[_i][_j]) );
        checked_call( setMOTIONPLAN_ENDSTATE( _i, _axPage, mMOTIONPLAN_ENDSTATE[_i][_j]) );

//        checked_call( setMOTIONPLAN_BUFFERSIZE( _i, _axPage, mMOTIONPLAN_BUFFERSIZE[_i][_j]) );
        checked_call( setMOTIONPLAN_ACCSCALE( _i, _axPage, mMOTIONPLAN_ACCSCALE[_i][_j]) );
        checked_call( setMOTIONPLAN_DECSCALE( _i, _axPage, mMOTIONPLAN_DECSCALE[_i][_j]) );

        checked_call( setMOTIONPLAN_STOPMODE( _i, _axPage, mMOTIONPLAN_STOPMODE[_i][_j]) );
        checked_call( setMOTIONPLAN_STOPDISTANCE( _i, _axPage, mMOTIONPLAN_STOPDISTANCE[_i][_j]) );
        checked_call( setMOTIONPLAN_STOPTIME( _i, _axPage, mMOTIONPLAN_STOPTIME[_i][_j]) );

        checked_call( setMOTIONPLAN_OOSLINESTATE( _i, _axPage, mMOTIONPLAN_OOSLINESTATE[_i][_j]) );
        checked_call( setMOTIONPLAN_OOSLINEOUTNUM( _i, _axPage, mMOTIONPLAN_OOSLINEOUTNUM[_i][_j]) );
        checked_call( setMOTIONPLAN_OOSLINERESPONSE( _i, _axPage, mMOTIONPLAN_OOSLINERESPONSE[_i][_j]) );

        checked_call( setMOTIONPLAN_OOSTOTALSTATE( _i, _axPage, mMOTIONPLAN_OOSTOTALSTATE[_i][_j]) );
        checked_call( setMOTIONPLAN_OOSTOTALOUTNUM( _i, _axPage, mMOTIONPLAN_OOSTOTALOUTNUM[_i][_j]) );
        checked_call( setMOTIONPLAN_OOSTOTALRESPONSE( _i, _axPage, mMOTIONPLAN_OOSTOTALRESPONSE[_i][_j]) );

        MRQ_PROGRESS( 30 + (_i*regions() + _j)*20/(axes()*regions()) ,
                  ("MOTORPLAN") );
    }
    end_foreach_page()

MRQ_PROGRESS( 50, ("TRIGGER") );

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

//        checked_call( setTRIGGER_PATTERN( i,
//                                          mTRIGGER_PATTERN+i,
//                                          mTRIGGER_PATTERN1+i,
//                                          mTRIGGER_PATTERN2+i,
//                                          mTRIGGER_PATTERN3+i,
//                                          mTRIGGER_PATTERN4+i
//                                          ) ); U16?

        checked_call( setTRIGGER_PATTRESP( i, mTRIGGER_PATTRESP[i]) );
        checked_call( setTRIGGER_PATTSMODE( i, mTRIGGER_PATTSMODE[i]) );
        checked_call( setTRIGGER_PATTSPERIOD( i, mTRIGGER_PATTSPERIOD[i]) );

        for( int j = 0; j < 2; j++ )
        {
            checked_call( setTRIGGER_LEVELSTATE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSTATE[i][j]) );
            checked_call( setTRIGGER_LEVELTYPE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELTYPE[i][j]) );
            checked_call( setTRIGGER_LEVELRESP( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELRESP[i][j]) );
            checked_call( setTRIGGER_LEVELSMODE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSMODE[i][j]) );
            checked_call( setTRIGGER_LEVELSPERIOD( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSPERIOD[i][j]) );
        }

//        checked_call( setDRIVER_TYPE( i, mDRIVER_TYPE[i]) );
        for( int j = 0; j < 3; j++ )
        {
            Q_ASSERT( j < sizeof_array(mDRIVER_STATEREG[0]) );
//            checked_call( setDRIVER_STATEREG( i, (MRQ_DRIVER_STATEREG)j, mDRIVER_STATEREG[i][j]) );
        }
        checked_call( setDRIVER_CURRENT( i, mDRIVER_CURRENT[i]) );

        checked_call( setDRIVER_MICROSTEPS( i, mDRIVER_MICROSTEPS[i]) );
        checked_call( setDRIVER_STATE( i, mDRIVER_STATE[i]) );


        checked_call( setDRIVER_IDLECURRENT( i, mDRIVER_IDLECURRENT[i]) );
        checked_call( setDRIVER_SWITCHTIME( i, mDRIVER_SWITCHTIME[i]) );
        checked_call( setDRIVER_MINICURRRATIO( i, mDRIVER_MINICURRRATIO[i]) );
    }

MRQ_PROGRESS( 60, ("DO") );

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
    }

MRQ_PROGRESS( 65, ("ISO") );

    for( int i = 0; i < mISOs; i++ )
    {
        Q_ASSERT( i < sizeof_array(mISOLATOROUTPUT_STATE) );
        checked_call( setISOLATOROUTPUT_STATE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_STATE[i]) );
        checked_call( setISOLATOROUTPUT_SOURCE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_SOURCE[i]) );
        checked_call( setISOLATOROUTPUT_CONDITION( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_CONDITION[i]) );
        checked_call( setISOLATOROUTPUT_RESPONSE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_RESPONSE[i]) );
    }
MRQ_PROGRESS( 70, ("SENSOR") );

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
        }
    }

MRQ_PROGRESS( 75, ("ISI") );

    for ( int i = 0; i < mISIs; i++ )
    {
        Q_ASSERT( i < 1 );
        checked_call( setISOLATORIN_STATE( mISOLATORIN_STATE ) );
        checked_call( setISOLATORIN_TYPE( mISOLATORIN_TYPE ) );
        checked_call( setISOLATORIN_RESPONSE( mISOLATORIN_RESPONSE ) );
        checked_call( setISOLATORIN_RESPCHAN( mISOLATORIN_RESPCHAN ) );

        checked_call( setISOLATORIN_SMODE( mISOLATORIN_SMODE ) );
        checked_call( setISOLATORIN_SPERIOD( mISOLATORIN_SPERIOD ) );
    }

    if ( mAlarms > 0 )
    {
    MRQ_PROGRESS( 80, ("ENCODER ALARM") );
        for ( int i = 0; i < mAbsEncoderAlarms; i++ )
        {
            checked_call( setABSENCALARM_STATE( (MRQ_IDENTITY_LABEL_1)i,
                                                mABSENCALARM_STATE[i] ) );
//            checked_call( setABSENCALARM_UPLIMIT( (MRQ_IDENTITY_LABEL_1)i,
//                                                mABSENCALARM_UPLIMIT[i] ) );
//            checked_call( setABSENCALARM_DOWNLIMIT( (MRQ_IDENTITY_LABEL_1)i,
//                                                mABSENCALARM_DOWNLIMIT[i] ) );
//            //! \todo zero value can not be motified by setup
//            checked_call( setABSENCALARM_ZEROVALUE( (MRQ_IDENTITY_LABEL_1)i,
//                                                mABSENCALARM_ZEROVALUE[i] ) );

//            checked_call( setABSENCALARM_ZEROPOSITION( (MRQ_IDENTITY_LABEL_1)i,
//                                                mABSENCALARM_ZEROPOSITION[i] ) );
        }
        checked_call( setABSENCALARM_RESPONSE(
                                            mABSENCALARM_RESPONSE ) );


    MRQ_PROGRESS( 85, ("DISTANCE ALARM") );
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

MRQ_PROGRESS_HIDE();

    return ret;
}

}
