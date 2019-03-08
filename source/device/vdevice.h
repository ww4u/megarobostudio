
#ifndef _V_DEVICE_H_
#define _V_DEVICE_H_

#include "../../include/mcstd.h"
#include "../bus/ibus.h"
#include "../bus/deviceid.h"
#include "../scpishell/scpishell.h"
#include "../com/drivercontroller.h"

#define  log_device()   logDbg()

namespace MegaDevice 
{

class VDevice : public scpiShell
{
public:
    enum EnumDeviceContent
    {
        e_device_content_full = 0,
        e_device_content_id,
        e_device_content_ver,
    };


protected:
    IBus *m_pBus;

public:
    VDevice();
    virtual ~VDevice()
    {}

    //! device id
    virtual int applyDeviceId(DeviceId &id );

    virtual int setDeviceId( DeviceId &id );
    DeviceId getDeviceId();

    //! recv id
    virtual void setCanRecvId( int id );
    int getCanRecvId();
    int canRecvId();

    virtual void setCanSendId( int id );
    int getCanSendId();
    int canSendId();

    virtual void setCanBroadcastId( int id );
    int getCanBroadcastId();
    int canBroadcastId();

    void setGroupSel( int id );
    int groupSel();

    virtual void setCanGroupId( int id );
    int getCanGroupId();
    int canGroupId();

    void attachBus( IBus *pBus )
    { m_pBus = pBus; }

    void detachBus()
    { m_pBus = NULL; }

    IBus *Bus()
    { return m_pBus; }

    void setInterval( int interval );
    int getInterval();
    void setTimeout( int tmo );
    int getTimeout();

    void wait_ms( int ms )
    {
        Q_ASSERT( NULL != m_pBus );
        m_pBus->wait_ms( ms );
    }
    void wait_us( int us )
    {
        Q_ASSERT( NULL != m_pBus );
        m_pBus->wait_us( us );
    }

public:
    virtual void rst();
    virtual int upload( EnumDeviceContent content = e_device_content_full );       //! upload a few data

    virtual int uploadSetting();

    virtual void adaptToFirmware( QList<FirmwarePackage*> * );

protected:
                        //! can ids
    int mCanRecvId;     //! node id = recv id
    int mCanSendId;
    int mCanGroupSel;   //! gp0/1
    int mCanGroupId;

    int mCanBroadcastId;

    int mInterval;  //! us
    int mTmo;       //! read timeout us

public:
    DeviceId mDeviceId;
};

#define DEVICE_RECEIVE_ID   mDeviceId

}

#endif
