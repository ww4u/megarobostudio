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

bool CANApi::load3RdApi( int portId )
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

//    reset = (p_VCI_ResetCAN)mDll.resolve("VCI_ResetCAN");
//    if ( !reset )
//    { logDbg();return false; }

    transmit = (p_VCI_Transmit)mDll.resolve("VCI_Transmit");
    if ( !transmit )
    { logDbg();return false; }

    receive = (p_VCI_Receive)mDll.resolve("VCI_Receive");
    if ( !receive )
    { logDbg();return false; }

    return true;
}
bool CANApi::loadMegaApi( int portId )
{
    find = (p_VCI_FindDevice)mDll.resolve("VCI_FindDevice");
    if ( !find )
    { logDbg();return false; }

    openExt = (p_VCI_OpenDeviceExt)mDll.resolve("VCI_OpenDevice");
    if ( !openExt )
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

//    if ( portId == 0
//         || portId == 2
//         || portId == 6 )
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

bool CANApi::loadApi( int portId )
{
    if ( portId == 0 || portId == 2 || portId == 6 )
    { return loadMegaApi( portId ); }
    else
    { return load3RdApi( portId); }
}
