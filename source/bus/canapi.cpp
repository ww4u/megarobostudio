#include "canapi.h"
#include "../../include/mcstd.h"

namespace MegaDevice {

CANApi::CANApi()
{
    open = NULL;
    close = NULL;

    init = NULL;
    getSize = NULL;

    clear = NULL;

    start = NULL;

    transmit = NULL;
    receive = NULL;
}

CANApi::~CANApi()
{
    unload();

}

bool CANApi::load( QString str )
{
    mDll.setFileName( str );

    if ( !mDll.load() )
    {
        return false;
    }

    if ( !loadApi() )
    {
        unload();
        return false;
    }

    return true;
}
void CANApi::unload()
{
    if ( mDll.isLoaded() )
    {
        mDll.unload();
    }
}

bool CANApi::loadApi()
{
    //! usb can-ii do not have find device
    find = (p_VCI_FindDevice)mDll.resolve("VCI_FindDevice");
    if ( !find )
    {}

    open = (p_VCI_OpenDevice)mDll.resolve("VCI_OpenDevice");
    if ( !open )
    { logDbg();return false; }

    close = (p_VCI_CloseDevice)mDll.resolve("VCI_CloseDevice");
    if ( !close )
    { logDbg();return false; }

    init = (p_VCI_InitCAN)mDll.resolve("VCI_InitCAN");
    if ( !init )
    { logDbg();return false; }

    getSize = (p_VCI_GetReceiveNum)mDll.resolve("VCI_GetReceiveNum");
    if ( !getSize )
    { logDbg();return false; }

    clear = (p_VCI_ClearBuffer)mDll.resolve("VCI_ClearBuffer");
    if ( !clear )
    { logDbg();return false; }

    start = (p_VCI_StartCAN)mDll.resolve("VCI_StartCAN");
    if ( !start )
    { logDbg();return false; }

    transmit = (p_VCI_Transmit)mDll.resolve("VCI_Transmit");
    if ( !transmit )
    { logDbg();return false; }

    receive = (p_VCI_Receive)mDll.resolve("VCI_Receive");
    if ( !receive )
    { logDbg();return false; }

    return true;
}

}
