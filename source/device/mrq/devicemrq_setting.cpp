
#include "deviceMRQ.h"

#ifdef DEVICE_EMIT_SIGNAL
#include "../../app/dpcobj.h"
#endif

namespace MegaDevice
{

void deviceMRQ::rst()
{
    //! foreach clear
    foreach_page()
    {
        setMOTIONPLAN_PVTCONFIG( _i,
                                 _axPage,
                                 MRQ_MOTIONPLAN_PVTCONFIG_1_CLEAR );

    }
    end_foreach_page()
}

int deviceMRQ::uploadBaseInfo()
{
    loadDesc();
    loadSN();
    loadSwVer();
    loadHwVer();

    loadFwVer();
    loadBtVer();

//    loadTpvCap();

    return 0;
}

int deviceMRQ::upload()
{
    uploadBaseInfo();

    loadTpvCap();

    return 0;
}

int deviceMRQ::testAdd( int a, int b )
{ return a + b; }

QString deviceMRQ::loadDesc()
{
    int ret;
    MRQ_LINK_DEVICEINFO type;
    MRQ_LINK_DEVICEINFO_1 type2;
    ret = getSYSTEM_TYPE( &type, &type2 );
    if ( ret != 0 )
    { logDbg();return mDesc; }

    //! format the type
    mDesc = QString("%1-%2").arg( MRQ_model::toString( type ),
                                  MRQ_model::toString( type2 ) );
    logDbg()<<mDesc;

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


#define progress( prog, info )      sysProgress( prog, info ); \
                                    sysProgress( true );
#define progress_hide()             \
                                    sysProgress( false );

//! upload all data
int deviceMRQ::uploadSetting()
{
    int ret;

progress( 0, ("LINK") );

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

    checked_call( getCAN_SENDID(&mCAN_SENDID) );
    checked_call( getCAN_RECEIVEID(&mCAN_RECEIVEID) );

    checked_call( getCAN_GROUPID1(&mCAN_GROUPID1) );
    checked_call( getCAN_GROUPID2(&mCAN_GROUPID2) );
    checked_call( getCAN_BROADCASTID(&mCAN_BROADCASTID) );

    checked_call( getCAN_NETMANAGESTATE(&mCAN_NETMANAGESTATE) );
    checked_call( getCAN_NETMANAGEHASH(&mCAN_NETMANAGEHASH) );

    checked_call( getCLOCK_FREQUENCY(&mCLOCK_FREQUENCY) );
    checked_call( getCLOCK_SYNCREGISTER(&mCLOCK_SYNCREGISTER) );
    checked_call( getCLOCK_STARTTYPE(&mCLOCK_STARTTYPE) );

    checked_call( getCLOCK_SYNCSTATE(&mCLOCK_SYNCSTATE) );
    checked_call( getCLOCK_COUNT(&mCLOCK_COUNT) );

progress( 10, ("MOTION") );

    //! state
    foreach_page()
    {
        checked_call( getMOTION_STATE( _i, _axPage, mMOTION_STATE[_i] + _j) );
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

progress( 20, ("MOTOR") );

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
        checked_call( getENCODER_LINENUM( i, mENCODER_LINENUM+i) );
        checked_call( getENCODER_CHANNELNUM( i, mENCODER_CHANNELNUM+i) );

        checked_call( getENCODER_TYPE( i, mENCODER_TYPE+i) );
        checked_call( getENCODER_MULTIPLE( i, mENCODER_MULTIPLE+i) );
        checked_call( getENCODER_STATE( i, mENCODER_STATE+i) );
//        checked_call( getENCODER_FEEDBACKRATIO( i, mENCODER_FEEDBACKRATIO+i) );
    }

progress( 30, ("MOTORPLAN") );

    foreach_page()
    {
        checked_call( getMOTIONPLAN_PVTCONFIG( _i, _axPage, mMOTIONPLAN_PVTCONFIG[_i]+_j ) );
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
        checked_call( getMOTIONPLAN_ACCSCALE( _i, _axPage, mMOTIONPLAN_ACCSCALE[_i]+_j) );
        checked_call( getMOTIONPLAN_DECSCALE( _i, _axPage, mMOTIONPLAN_DECSCALE[_i]+_j) );

        checked_call( getMOTIONPLAN_STOPMODE( _i, _axPage, mMOTIONPLAN_STOPMODE[_i]+_j) );
        checked_call( getMOTIONPLAN_STOPDISTANCE( _i, _axPage, mMOTIONPLAN_STOPDISTANCE[_i]+_j) );
        checked_call( getMOTIONPLAN_STOPTIME( _i, _axPage, mMOTIONPLAN_STOPTIME[_i]+_j) );

        checked_call( getMOTIONPLAN_OOSLINESTATE( _i, _axPage, mMOTIONPLAN_OOSLINESTATE[_i]+_j) );
        checked_call( getMOTIONPLAN_OOSLINEOUTNUM( _i, _axPage, mMOTIONPLAN_OOSLINEOUTNUM[_i]+_j) );
        checked_call( getMOTIONPLAN_OOSLINERESPONSE( _i, _axPage, mMOTIONPLAN_OOSLINERESPONSE[_i]+_j) );

        checked_call( getMOTIONPLAN_OOSTOTALSTATE( _i, _axPage, mMOTIONPLAN_OOSTOTALSTATE[_i]+_j) );
        checked_call( getMOTIONPLAN_OOSTOTALOUTNUM( _i, _axPage, mMOTIONPLAN_OOSTOTALOUTNUM[_i]+_j) );
        checked_call( getMOTIONPLAN_OOSTOTALRESPONSE( _i, _axPage, mMOTIONPLAN_OOSTOTALRESPONSE[_i]+_j) );
    }
    end_foreach_page()

progress( 40, ("TRIGGER") );

    for ( int i = 0; i < axes(); i++ )
    {
        for ( int j = 0; j < regions(); j++ )
        {
            checked_call( getREPORT_STATE( i, (MRQ_REPORT_STATE)j, mREPORT_STATE[i]+j) );
            checked_call( getREPORT_PERIOD( i, (MRQ_REPORT_STATE)j, mREPORT_PERIOD[i]+j) );
        }
        Q_ASSERT( i < sizeof_array(mTRIGGER_MODE) );
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

        for( int j = 0; j < 2; j++ )
        {
//            checked_call( getTRIGGER_LEVELSTATE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSTATE[i]+j) );
//            checked_call( getTRIGGER_LEVELTYPE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELTYPE[i]+j) );
//            checked_call( getTRIGGER_LEVELRESP( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELRESP[i]+j) );
//            checked_call( getTRIGGER_LEVELSMODE( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSMODE[i]+j) );
//            checked_call( getTRIGGER_LEVELSPERIOD( i, (MRQ_TRIGGER_LEVELSTATE)j, mTRIGGER_LEVELSPERIOD[i]+j) );
        }

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

progress( 40, ("DO") );

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

progress( 50, ("ISO") );

    for( int i = 0; i < mISOs; i++ )
    {
        Q_ASSERT( i < sizeof_array(mISOLATOROUTPUT_STATE) );
        checked_call( getISOLATOROUTPUT_STATE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_STATE + i) );
        checked_call( getISOLATOROUTPUT_SOURCE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_SOURCE + i) );
        checked_call( getISOLATOROUTPUT_CONDITION( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_CONDITION + i) );
        checked_call( getISOLATOROUTPUT_RESPONSE( (MRQ_ISOLATOROUTPUT_STATE)i, mISOLATOROUTPUT_RESPONSE + i) );
    }
progress( 60, ("SENSOR") );

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

progress( 70, ("ISI") );

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

progress_hide();

    return ret;
}

}
