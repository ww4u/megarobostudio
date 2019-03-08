
#include "../../include/datatype.h"
#include "vdevice.h"

namespace MegaDevice 
{

VDevice::VDevice()
{
   m_pBus = NULL;

   mCanRecvId = 0x01;
   mCanSendId = 0x01;
   mCanGroupSel = 0;
   mCanGroupId = 0x01;

   mCanBroadcastId = 0x01;

   mInterval = 1;
   mTmo = 1;
}

int VDevice::applyDeviceId(DeviceId &id )
{ return 0; }

int VDevice::setDeviceId( DeviceId &id )
{
    mDeviceId = id;

    return 0;
}

DeviceId VDevice::getDeviceId()
{
    return mDeviceId;
}

void VDevice::setCanRecvId( int id )
{ mCanRecvId = id; }
int VDevice::getCanRecvId()
{ return mCanRecvId; }
int VDevice::canRecvId()
{ return mCanRecvId; }

void VDevice::setCanSendId( int id )
{ mCanSendId = id; }
int VDevice::getCanSendId()
{ return mCanSendId; }
int VDevice::canSendId()
{ return mCanSendId; }

void VDevice::setCanBroadcastId( int id )
{ mCanBroadcastId = id; }
int VDevice::getCanBroadcastId()
{ return mCanBroadcastId; }
int VDevice::canBroadcastId()
{ return mCanBroadcastId; }

void VDevice::setGroupSel( int id )
{ mCanGroupSel = id; }
int VDevice::groupSel()
{ return mCanGroupSel; }

void VDevice::setCanGroupId( int id )
{ mCanGroupId = id; }
int VDevice::getCanGroupId()
{ return mCanGroupId; }
int VDevice::canGroupId()
{ return mCanGroupId; }

void VDevice::setInterval( int interval )
{ mInterval = interval; }
int VDevice::getInterval()
{ return mInterval; }
void VDevice::setTimeout( int tmo )
{ mTmo = tmo; }
int VDevice::getTimeout()
{ return mTmo; }

void VDevice::rst()
{}

int VDevice::upload( EnumDeviceContent content )
{ return 0; }

int VDevice::uploadSetting()
{ return 0; }

void VDevice::adaptToFirmware( QList<FirmwarePackage*> * )
{}

}
