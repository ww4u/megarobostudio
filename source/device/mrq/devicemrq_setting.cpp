
#include "deviceMRQ.h"

//#ifdef DEVICE_EMIT_SIGNAL
//#include "../../app/dpcobj.h"
//#endif

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

int deviceMRQ::uploadDesc()
{
    loadDesc();

    return 0;
}

int deviceMRQ::uploadBaseInfo()
{
    loadSN();
//    loadSwVer();
    loadSeqVer();
    loadHwVer();

    loadFwVer();
    loadBtVer();

    return 0;
}

//! send/recv/group
int deviceMRQ::uploadIDs()
{
    int ret;
    checked_call( getCAN_SENDID(&mCAN_SENDID) );
    checked_call( getCAN_RECEIVEID(&mCAN_RECEIVEID) );

    checked_call( getCAN_GROUPID1(&mCAN_GROUPID1) );
    checked_call( getCAN_GROUPID2(&mCAN_GROUPID2) );
    checked_call( getCAN_BROADCASTID(&mCAN_BROADCASTID) );

    return ret;
}

int deviceMRQ::upload()
{
    uploadDesc();

    uploadBaseInfo();

    uploadIDs();

    loadTpvCap();

    loadMotorBasic();

    if ( mId == robot_geogoog_5_1 )
    {
        loadPwms();

        loadEncoderZero();
    }

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
        logDbg()<<mSn;
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
        logDbg()<<mSwVer;
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

QString deviceMRQ::loadSeqVer()
{
    byte v0, v1, v2, v3;

    int ret;

    ret = getSeqVer( &v0, &v1, &v2, &v3 );
    if ( ret == 0 )
    {
        mSeqVer = QString( "%1.%2.%3.%4").arg(v0).arg(v1).arg(v2).arg(v3);
    }

    return mSeqVer;
}

int deviceMRQ::loadPwms()
{
    int ret;
    ret = loadFanPwm();
    if ( ret != 0 )
    { return ret; }

    ret = loadLedPwm();
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int deviceMRQ::loadFanPwm()
{logDbg()<<mFanInfo.mDuty<<mFanInfo.mFreq;
    int ret;
    ret = m_pBus->read(DEVICE_RECEIVE_ID,
                        (byte)mc_SYSTEM,
                        (byte)(sc_SYSTEM_FANPARA_Q),
                        &mFanInfo.mDuty,
                        &mFanInfo.mFreq );
    if (ret != 0)
    { return ret; }

    logDbg()<<mFanInfo.mDuty<<mFanInfo.mFreq;

    return 0;
}
int deviceMRQ::loadLedPwm()
{
    int ret;
    for ( int i = 0; i < 4; i++ )
    {logDbg()<<mLedInfo[i].mDuty<<mLedInfo[i].mFreq;
        ret = m_pBus->read(DEVICE_RECEIVE_ID,
                            (byte)mc_SYSTEM,
                            (byte)(sc_SYSTEM_ARMLEDPARA_Q),
                            (byte)(i),
                            &mLedInfo[i].mDuty,
                            &mLedInfo[i].mFreq );
        if (ret != 0)
        { return ret; }

        logDbg()<<mLedInfo[i].mDuty<<mLedInfo[i].mFreq;
    }
    return 0;
}

//! only for t4
int deviceMRQ::loadEncoderZero()
{
    int ret;
    for ( int i = 0; i < 4; i++ )
    {
//        ret = getABSENCALARM_ZEROVALUE(
//                                        (MRQ_IDENTITY_LABEL_1)(i),
//                                        &mABSENCALARM_ZEROVALUE[i]
//                                        );
//        if ( ret != 0 )
//        { return ret; }

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

    return 0;
}

#define mc_FACTORY  0XCA
#define mc_SN       0X00

#define SPLIT_SIZE      5
int deviceMRQ::setSN( const QString &sn )
{
    int ret, snLen;

    snLen = sn.length();
    if ( snLen <= 0 || snLen > SPLIT_SIZE * 0XF )
    { return -1; }

    QByteArray ary = sn.toLatin1();

    //! split by SPLIT_SIZE
    byte buf[ SPLIT_SIZE + 1];
    int n,m;
    n = 1;
    m = (snLen + SPLIT_SIZE-1)/SPLIT_SIZE;      //! group

    int offset, payLen;
    offset = 0;
    payLen = snLen;
    for ( int i = 1; i <= m; i++ )
    {
        //! head
        buf[0] = (i<<4) | m;                    //! sub/total

        if ( snLen - offset > SPLIT_SIZE )
        { payLen = SPLIT_SIZE; }
        else
        { payLen = snLen - offset; }

        //! payload
        memcpy( buf + 1, ary.data() + offset, payLen );

        offset = offset + SPLIT_SIZE;

        ret = m_pBus->write( DEVICE_RECEIVE_ID,
                             (byte)mc_FACTORY, (byte)mc_SN,
                             buf,
                             payLen + 1 );

        if ( ret != 0 )
        { return ret; }
    }

    return 0;
}

int deviceMRQ::uploadSetting()
{
    int ret;

    ret = _uploadSetting();
    if ( ret != 0 )
    { MRQ_PROGRESS_HIDE(); }
    else
    { setFilled(true); }

    return ret;
}

//! apply the settings to device
int deviceMRQ::applySetting()
{
    int ret;

    ret = _applySetting();
    if ( ret != 0 )
    { MRQ_PROGRESS_HIDE(); }

    return ret;
}


}
