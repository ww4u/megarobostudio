#include "devicemrv.h"

#include "../board/MRQ_model.h"

#include "tpdownloader.h"

namespace MegaDevice
{

deviceMRV::deviceMRV()
{
}
deviceMRV::~deviceMRV()
{
    delete_all( mAxesList );
}

void deviceMRV::postCtor()
{
    MrvAxes *pAxes;
    for ( int i = 0; i < axes(); i++ )
    {
        //! downloader
        mTpIndexes<<0;

        mDownloaders.append( new TpDownloader() );

        Q_ASSERT( NULL != mDownloaders.at(i) );
        mDownloaders.at(i)->attachDevice( this, i );

        //! mrv axes
        pAxes = new MrvAxes();
        Q_ASSERT( NULL != pAxes );

        pAxes->setParent( this );
        pAxes->setAxesId( i );

        mAxesList.append( pAxes );
    }

    mDownloaderSema.release( 1 );
}

int deviceMRV::applyDeviceId(DeviceId &id )
{
    return 0;
}

int deviceMRV::setDeviceId(DeviceId &id )
{
    int ret;

    ret = MRV::setDeviceId( id );
    if ( ret != 0 )
    { return ret; }

    //! vdevice ids
    VDevice::setCanSendId( id.mSendId );
    VDevice::setCanRecvId( id.mRecvId );
    VDevice::setCanBroadcastId( id.mBroadcastId );

    //! id apply
    _MRV_model::setCAN_SENDID( id.mSendId );
    _MRV_model::setCAN_RECEIVEID( id.mRecvId );
    _MRV_model::setCAN_BROADCASTID( id.mBroadcastId );

    setSignature( mDeviceId.mSignature );

    return ret;
}

void deviceMRV::rst()
{
    //! foreach clear
//    foreach_page()
//    {
//        setMOTIONPLAN_PVTCONFIG( _i,
//                                 _axPage,
//                                 MRQ_MOTIONPLAN_PVTCONFIG_1_CLEAR );

//    }
//    end_foreach_page()
}

int deviceMRV::uploadDesc()
{
    loadDesc();

    return 0;
}

int deviceMRV::uploadBaseInfo()
{
    loadSN();

    loadSwVer();

    loadHwVer();

    loadBtVer();

    loadCycles();       //! for pt used
    loadDeviceMode();
    loadExecMode();

    return 0;
}

//! send/recv/group
int deviceMRV::uploadIDs()
{
    int ret;
    checked_call( getCAN_SENDID(&mCAN_SENDID) );
    checked_call( getCAN_RECEIVEID(&mCAN_RECEIVEID) );

    checked_call( getCAN_GROUPID1(&mCAN_GROUPID1) );
    checked_call( getCAN_GROUPID2(&mCAN_GROUPID2) );
    checked_call( getCAN_BROADCASTID(&mCAN_BROADCASTID) );

    return ret;
}

int deviceMRV::upload( EnumDeviceContent content )
{
    if ( content == e_device_content_id )
    {
        uploadIDs();
    }
    else
    {
        uploadDesc();

        uploadBaseInfo();

        uploadIDs();
    }

    return 0;
}

QList<int> deviceMRV::deviceIds()
{
    QList<int> ids;

    ids<<mCAN_RECEIVEID<<mCAN_SENDID<<mCAN_GROUPID1<<mCAN_GROUPID2;
    return ids;
}

QString deviceMRV::deviceFullDesc()
{
    return QString( "%1(%2)").arg( mDesc ).arg( getName() ) ;
}

QString deviceMRV::loadDesc()
{
    int ret;
    MRV_SYSTEM_TYPE type;
    MRV_LINK_DEVICEINFO_1 type2;
    ret = getSYSTEM_TYPE( &type, &type2 );
    if ( ret != 0 )
    { logDbg();return mDesc; }

    //! format the type
    //! mrq
    if ( type == MRV_SYSTEM_TYPE_MRQ )
    {
        MRQ_model mrqModel;
        mDesc = QString("%1-%2").arg( mrqModel.toString( (MRQ_LINK_DEVICEINFO)type ),
                                      mrqModel.toString( (MRQ_LINK_DEVICEINFO_1)type2 ) );
    }
    //! mrv
    else if ( type == MRV_SYSTEM_TYPE_MRV )
    {
        MRV_model mrvModel;
        mDesc = QString("%1-%2").arg( mrvModel.toString( type ),
                                      mrvModel.toString( type2 ) );
    }
    else
    {}

    return mDesc;
}

QString deviceMRV::loadSN()
{
    int ret;

    //! write
    ret = m_pBus->write(DEVICE_RECEIVE_ID, (byte)MRV_mc_SYSTEM, (byte)MRV_sc_SYSTEM_SN_Q);
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
        logDbg()<<mSN;
    }

    return mSN;
}

QString deviceMRV::loadSwVer()
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
QString deviceMRV::loadHwVer()
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

QString deviceMRV::loadBtVer()
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

void deviceMRV::loadCycles()
{
    for ( int i = 0; i < axes(); i++ )
    {
        getPVT_CYCLES( i, mPVT_CYCLES + i );
    }
}

void deviceMRV::loadDeviceMode()
{
    for ( int i = 0; i < axes(); i++ )
    {
        getVALVECTRL_DEVICE( i, mVALVECTRL_DEVICE + i );
    }
}

void deviceMRV::loadExecMode()
{
    for ( int i = 0; i < axes(); i++ )
    {
        getPVT_EXECMODE( i, mPVT_EXECMODE + i );
    }
}

MRV_model *deviceMRV::getModel()
{
    return this;
}

}
