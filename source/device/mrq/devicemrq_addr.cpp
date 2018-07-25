#include "deviceMRQ.h"
namespace MegaDevice
{


int deviceMRQ::applyDeviceId(DeviceId &id )
{
    int ret;

    //! set
    ret = setDeviceId( id );
    if ( ret != 0 )
    { return ret; }

    //! write the device
    ret = applyIds( );
    if ( ret != 0 )
    { return ret; }

    //! verify
    ret = verifyIds( );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int deviceMRQ::setDeviceId(DeviceId &id )
{
    int ret;

    ret = MRQ::setDeviceId( id );
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

    setSignature( mDeviceId.mSignature );

//    //! write the device
//    ret = applyIds( );
//    if ( ret != 0 )
//    { return ret; }

//    //! verify
//    ret = verifyIds( );
//    if ( ret != 0 )
//    { return ret; }

    return 0;
}

void deviceMRQ::setName( const QString &strName )
{
    if( mName == strName )
    { return; }

    mcModelObj::setName( strName );

    //! \todo write to the device

}

int deviceMRQ::applyIds( )
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
        m_pBus->clear();

        m_pBus->write( DEVICE_RECEIVE_ID, MRQ_mc_CAN, MRQ_sc_CAN_NETMANAGESTATE_Q );

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

int deviceMRQ::verifyIds(  )
{
    int ret;
    uint32 sendId, recvId;

    //! send id
    bool bQuery = true;
    do
    {
        ret = getCAN_SENDID( &sendId, bQuery );
        if ( ret != 0 )
        { return ret; }
        bQuery = false;

        if ( (int)sendId == mDeviceId.mSendId )
        { break; }

    }while( 0 );

    if ( (int)sendId != mDeviceId.mSendId )
    { return -1; }

    //! receive id
    bQuery = true;
    do
    {
        ret = getCAN_RECEIVEID( &recvId, bQuery );
        if ( ret != 0 )
        { return ret; }
        bQuery = false;

        if ( (int)recvId == mDeviceId.mRecvId )
        { break; }
    }while( 0 );

    if ( (int)recvId != mDeviceId.mRecvId )
    {
        logDbg()<<recvId<<mDeviceId.mRecvId;
        return -3;
    }

    return 0;
}

}
