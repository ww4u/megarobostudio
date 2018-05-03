
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
    loadSwVer();
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
        ret = getABSENCALARM_ZEROVALUE(
                                        (MRQ_IDENTITY_LABEL_1)(i),
                                        &mABSENCALARM_ZEROVALUE[i]
                                        );
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
