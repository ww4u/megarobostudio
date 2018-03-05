#include "deviceid.h"

namespace MegaDevice {

DeviceId::DeviceId( int rId, int sId )
{
    mSendId = sId;
    mRecvId = rId;
    mBroadcastId = 0x01;

    mGroupId[0] = 0;
    mGroupId[1] = 0;
}

int DeviceId::setSendId( int id )
{
    mSendId = id;
    return 0;
}
int DeviceId::getSendId()
{
    return mSendId;
}
int DeviceId::sendId()
{
    return mSendId;
}

int DeviceId::setRecvId( int id )
{
    mRecvId = id;
    return 0;
}
int DeviceId::getRecvId()
{
    return mRecvId;
}
int DeviceId::recvId()
{
    return mRecvId;
}

int DeviceId::setBroadId( int id )
{
    mBroadcastId =id;
    return 0;
}
int DeviceId::getBroadId()
{
    return mBroadcastId;
}

int DeviceId::setSignature( uint32 sig )
{
    mSignature = sig;
    return 0;
}
uint32 DeviceId::getSignature( )
{
    return mSignature;
}

}
