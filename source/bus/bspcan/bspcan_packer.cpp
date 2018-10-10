#include "canapi.h"

//! 1--no error
unsigned int bspcan_OpenDevice(unsigned int DeviceType, unsigned int DeviceInd, unsigned int Reserved)
{
    int ret;

    ret = bspCanInit();
    //! fail
    if ( ret != 0 )
    { return 0; }
    else
    { return 1; }
}
//! 0--no error
unsigned int  bspcan_CloseDevice(unsigned int DeviceType, unsigned int DeviceInd)
{
    bspCanClose();
    return 0;
}

//! 1--success
unsigned int bspcan_InitCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_INIT_CONFIG pInitConfig)
{
    return 1;
}

unsigned int  bspcan_GetReceiveNum(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd)
{
//    return bspCanGetRxCount();
    return 0;
}
//! 0--no error
unsigned int bspcan_ClearBuffer(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd)
{
    bspCanClearRxBuf();

    return 0;
}

unsigned int bspcan_StartCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd)
{ return 1; }
unsigned int  bspcan_ResetCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd)
{
    bspCanReset();
    return 0;
}

//! return -- send len
unsigned int  bspcan_Transmit(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pSend, unsigned int len)
{
    int ret, transLen;
    transLen = 0;
    for ( int i = 0; i < len; i++ )
    {
        ret = bspCanSend( pSend + i );
        if ( ret != 0 )
        { return transLen; }

        transLen++;
    }

    return transLen;
}

unsigned int  bspcan_Receive(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pReceive, unsigned int Len, int WaitTime)
{
    //! try to read
    int readLen = 0;
    int ret;
    for ( int i = 0; i < Len; i++ )
    {
        ret = bspCanRead( pReceive + i );
        if ( ret != 0 )
        { break; }
        else
        { readLen++; }
    }

    return readLen;
}

unsigned int bspcan_Write(unsigned int DeviceType, unsigned int DeviceInd, char * data, unsigned int Len)
{ return 0; }
unsigned int bspcan_Read(unsigned int DeviceType, unsigned int DeviceInd, char* pReceive, unsigned int Length, int WaitTime)
{ return 0; }

unsigned int bspcan_FindDevice(unsigned int DeviceType, char* desc)
{ return 0; }
