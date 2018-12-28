
#include "deviceMRQ.h"


//! for desc
#include "../mrv_board/MRV_model.h"

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

    //! foreach sub group
    for ( int i = 0; i < axes(); i++ )
    {
        checked_call( getIDENTITY_GROUP( i, MRQ_IDENTITY_GROUP_GROUP1, &mIDENTITY_GROUP[i][0] ) );
        checked_call( getIDENTITY_GROUP( i, MRQ_IDENTITY_GROUP_GROUP2, &mIDENTITY_GROUP[i][1] ) );
    }

    return ret;
}

int deviceMRQ::upload( EnumDeviceContent content )
{
    int ret = 0;

    if ( content == e_device_content_id )
    {
        uploadIDs();
    }
    else
    {
        ret = uploadDesc();
        if ( ret != 0 ){ return ret; }

        ret = uploadBaseInfo();
        if ( ret != 0 ){ return ret; }

        ret = uploadIDs();
        if ( ret != 0 ){ return ret; }

        ret = loadRawSystem();
        if ( ret != 0 ){ return ret; }

        ret = loadTpvCap();
        if ( ret != 0 ){ return ret; }

        ret = loadMotorBasic();
        if ( ret != 0 ){ return ret; }

        if ( mId == robot_geogoog_5_1 )
        {
            ret = loadPwms();
            if ( ret != 0 ){ return ret; }

            ret = loadEncoderZero();
            if ( ret != 0 ){ return ret; }
        }
    }
    return ret;
}

QList<int> deviceMRQ::deviceIds()
{
    QList<int> ids;

    ids<<mCAN_RECEIVEID<<mCAN_SENDID<<mCAN_GROUPID1<<mCAN_GROUPID2;
    return ids;
}

QList<int> deviceMRQ::subIDs( int ch )
{
    Q_ASSERT( ch >= 0 && ch < axes() );

    QList<int> ids;

    ids<<mIDENTITY_GROUP[ch][0]<<mIDENTITY_GROUP[ch][1];
    return ids;
}

QString deviceMRQ::deviceFullDesc()
{ return getFullDesc(0); }

int deviceMRQ::testAdd( int a, int b )
{ return a + b; }

QString deviceMRQ::loadDesc()
{
    int ret;
    MRQ_LINK_DEVICEINFO type;
    MRQ_LINK_DEVICEINFO_1 type2;
    ret = getSYSTEM_TYPE( &type, &type2 );
    if ( ret != 0 )
    { return mDesc; }

    //! format the type
    //! mrq
    if ( type == MRQ_LINK_DEVICEINFO_MRQ )
    {
        mDesc = QString("%1-%2").arg( MRQ_model::toString( type ),
                                      MRQ_model::toString( type2 ) );
    }
    //! mrv
    else if ( type == MRQ_LINK_DEVICEINFO_MRV )
    {
        MRV_model mrvModel;
        mDesc = QString("%1-%2").arg( mrvModel.toString( (MRV_LINK_DEVICEINFO)type ),
                                      mrvModel.toString( (MRV_LINK_DEVICEINFO_1)type2 ) );
    }
    else
    {}

    return mDesc;
}

QString deviceMRQ::loadSN()
{
    int ret;

    //! write
    ret = m_pBus->write(DEVICE_RECEIVE_ID, (byte)MRQ_mc_SYSTEM, (byte)MRQ_sc_SYSTEM_SN_Q);
    if (ret != 0)
    { return mSN; }

    byte buf[32];
    int retLen;
    ret = m_pBus->doSplitRead(DEVICE_RECEIVE_ID, 2, buf, sizeof(buf), &retLen );
    if (ret != 0)
    { return mSN; }

    //! enugh
    if ( retLen < sizeof_array(buf) && retLen > 0 )
    {
        mSN = QByteArray( (const char*)buf, retLen );
    }

    return mSN;
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
    char v0, v1, v2,v3,v4,v5;
    int ret;

    ret = getSYSTEM_FPGAVER( &v0, &v1, &v2, &v3, &v4, &v5 );
    if ( ret == 0 )
    {
        mFwVer = QString("%1.%2.%3.%4.%5.%6").arg((int)v0)
                                             .arg((int)v1)
                                             .arg((int)v2)
                                             .arg((int)v3)
                                             .arg((int)v4)
                                             .arg((int)v5);
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
{
    int ret;
    ret = m_pBus->read(DEVICE_RECEIVE_ID,
                        (byte)MRQ_mc_SYSTEM,
                        (byte)(MRQ_sc_SYSTEM_FANPARA_Q),
                        &mFanInfo.mDuty,
                        &mFanInfo.mFreq );
    if (ret != 0)
    { return ret; }

    return 0;
}
int deviceMRQ::loadLedPwm()
{
    int ret;
    for ( int i = 0; i < 4; i++ )
    {
        ret = m_pBus->read(DEVICE_RECEIVE_ID,
                            (byte)MRQ_mc_SYSTEM,
                            (byte)(MRQ_sc_SYSTEM_ARMLEDPARA_Q),
                            (byte)(i),
                            &mLedInfo[i].mDuty,
                            &mLedInfo[i].mFreq );
        if (ret != 0)
        { return ret; }
    }
    return 0;
}

//! only for t4
int deviceMRQ::loadEncoderZero()
{
    int ret;
    for ( int i = 0; i < 4; i++ )
    {
        checked_call( getABSENCALARM_STATE( (MRQ_IDENTITY_LABEL_1)i,
                                            &mABSENCALARM_STATE[i] ) );
        checked_call( getABSENCALARM_UPLIMIT( (MRQ_IDENTITY_LABEL_1)i,
                                            &mABSENCALARM_UPLIMIT[i] ) );
        checked_call( getABSENCALARM_DOWNLIMIT( (MRQ_IDENTITY_LABEL_1)i,
                                            &mABSENCALARM_DOWNLIMIT[i] ) );
        checked_call( getABSENCALARM_ZEROVALUE( (MRQ_IDENTITY_LABEL_1)i,
                                            &mABSENCALARM_ZEROVALUE[i] ) );

//        checked_call( getABSENCALARM_ZEROPOSITION( (MRQ_IDENTITY_LABEL_1)i,
//                                            &mABSENCALARM_ZEROPOSITION[i] ) );
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

int deviceMRQ::loadSensorUartData( MRQ_SENSORUART_BAUD u,
                        MRQ_IDENTITY_LABEL_1 s,
                        QByteArray &ary )
{
    int ret;

    //! write
    ret = m_pBus->write(DEVICE_RECEIVE_ID, (byte)MRQ_mc_SENSORUART,
                                           (byte)MRQ_sc_SENSORUART_DATA_Q,
                                           (byte)u,
                                           (byte)s );
    if (ret != 0)
    { return -1; }

    byte buf[128];
    int retLen;
    ret = m_pBus->doSplitRead(DEVICE_RECEIVE_ID, 4, buf, sizeof(buf), &retLen );
    if (ret != 0)
    { return -1; }

    //! enough
    if ( retLen < sizeof_array(buf) && retLen > 0 )
    {
        ary.append( (const char*)buf, retLen );
        return 0;
    }
    else
    { return -1; }
}

int deviceMRQ::uploadSetting()
{
    int ret;

    receiveCache::cli();

        ret = _uploadSetting();
        if ( ret != 0 )
        { MRQ_PROGRESS_HIDE(); }
        else
        { setFilled(true); }

    receiveCache::sti();

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
