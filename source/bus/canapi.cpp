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

    write = NULL;
    read = NULL;

    mPortId = 0;
}

CANApi::~CANApi()
{
    unload();
}

#ifdef ARCH_32

bool CANApi::load( QString str, int portId )
{
    mDll.setFileName( str );

    if ( !mDll.load() )
    {
        return false;
    }

    if ( !loadApi( portId ) )
    {
        unload();
        return false;
    }

    mPortId = portId;

    return true;
}

void CANApi::unload()
{
    if ( mDll.isLoaded() )
    {
        mDll.unload();
    }
}

bool CANApi::loadApi( int portId )
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

    if ( portId == 0 )
    {
        write = (p_VCI_Write)mDll.resolve("VCI_Write");
        if ( !write )
        { logDbg();return false; }

        read = (p_VCI_Read)mDll.resolve("VCI_Read");
        if ( !read )
        { logDbg();return false; }
    }

    return true;
}

#else

bool CANApi::load( QString str, int portId )
{
    mDll.setFileName( str );

    if ( !loadApi( portId ) )
    {
        unload();
        return false;
    }

    mPortId = portId;

    return true;
}

void CANApi::unload()
{
}

bool CANApi::loadApi( int portId )
{
    //! usb can-ii do not have find device
    find = (p_VCI_FindDevice)VCI_FindDevice;
    if ( !find )
    {}

    open = (p_VCI_OpenDevice)VCI_OpenDevice;
    if ( !open )
    { logDbg();return false; }

    close = (p_VCI_CloseDevice)VCI_CloseDevice;
    if ( !close )
    { logDbg();return false; }

    init = (p_VCI_InitCAN)VCI_InitCAN;
    if ( !init )
    { logDbg();return false; }

    getSize = (p_VCI_GetReceiveNum)VCI_GetReceiveNum;
    if ( !getSize )
    { logDbg();return false; }

    clear = (p_VCI_ClearBuffer)VCI_ClearBuffer;
    if ( !clear )
    { logDbg();return false; }

    start = (p_VCI_StartCAN)VCI_StartCAN;
    if ( !start )
    { logDbg();return false; }

    transmit = (p_VCI_Transmit)VCI_Transmit;
    if ( !transmit )
    { logDbg();return false; }

    receive = (p_VCI_Receive)VCI_Receive;
    if ( !receive )
    { logDbg();return false; }

    if ( portId == 0 )
    {
        write = (p_VCI_Write)VCI_Write;
        if ( !write )
        { logDbg();return false; }

        read = (p_VCI_Read)VCI_Read;
        if ( !read )
        { logDbg();return false; }
    }

    return true;
}

#endif

}
