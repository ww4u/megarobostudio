// usbcanii.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ecanvci.h"

//! only for the windows used
#define MY_API  __cdecl

typedef unsigned int  (*p_VCI_OpenDevice)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int Reserved);
typedef unsigned int  (*p_VCI_CloseDevice)(unsigned int DeviceType, unsigned int DeviceInd);

typedef unsigned int  (*p_VCI_InitCAN)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_INIT_CONFIG pInitConfig);

typedef unsigned int  (*p_VCI_GetReceiveNum)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);
typedef unsigned int  (*p_VCI_ClearBuffer)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);

typedef unsigned int  (*p_VCI_StartCAN)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);
typedef unsigned int  (*p_VCI_ResetCAN)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);

typedef unsigned int  (*p_VCI_Transmit)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pSend, unsigned int Len);
typedef unsigned int  (*p_VCI_Receive)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pReceive, unsigned int Len, int WaitTime);

static HMODULE loadDll()
{
    return LoadLibrary("Ecanvci.dll");
}

unsigned int MY_API VCI_OpenDevice(unsigned int DeviceType, unsigned int DeviceInd, unsigned int Reserved)
{
    p_VCI_OpenDevice api;
    api = (p_VCI_OpenDevice)GetProcAddress( loadDll(), HEADER"OpenDevice");

    return api( DeviceType, DeviceInd, Reserved );
}
unsigned int MY_API VCI_CloseDevice(unsigned int DeviceType, unsigned int DeviceInd)
{
    p_VCI_CloseDevice api;
    api = (p_VCI_CloseDevice)GetProcAddress(loadDll(), HEADER"CloseDevice");

    return api(DeviceType, DeviceInd);
}

unsigned int MY_API VCI_InitCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_INIT_CONFIG pInitConfig)
{
    p_VCI_InitCAN api;
    api = (p_VCI_InitCAN)GetProcAddress(loadDll(), HEADER"InitCAN");

    return api(DeviceType, DeviceInd, CANInd, pInitConfig );
}

unsigned int MY_API VCI_GetReceiveNum(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd)
{
    p_VCI_GetReceiveNum api;
    api = (p_VCI_GetReceiveNum)GetProcAddress(loadDll(), HEADER"GetReceiveNum");

    return api(DeviceType, DeviceInd, CANInd );
}
unsigned int MY_API VCI_ClearBuffer(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd)
{
    p_VCI_ClearBuffer api;
    api = (p_VCI_ClearBuffer)GetProcAddress(loadDll(), HEADER"ClearBuffer");

    return api(DeviceType, DeviceInd, CANInd);
}

unsigned int MY_API VCI_StartCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd)
{
    p_VCI_StartCAN api;
    api = (p_VCI_StartCAN)GetProcAddress(loadDll(), HEADER"StartCAN");

    return api(DeviceType, DeviceInd, CANInd);
}

unsigned int MY_API VCI_ResetCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd)
{
    p_VCI_ResetCAN api;
    api = (p_VCI_ResetCAN)GetProcAddress(loadDll(), HEADER"ResetCAN");

    return api(DeviceType, DeviceInd, CANInd);
}

unsigned int MY_API VCI_Transmit(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pSend, unsigned int Len)
{
    p_VCI_Transmit api;
    api = (p_VCI_Transmit)GetProcAddress(loadDll(), HEADER"Transmit");

    return api(DeviceType, DeviceInd, CANInd, pSend, Len );
}
unsigned int MY_API VCI_Receive(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pReceive, unsigned int Len, int WaitTime)
{
    p_VCI_Receive api;
    api = (p_VCI_Receive)GetProcAddress(loadDll(), HEADER"Receive");

    return api(DeviceType, DeviceInd, CANInd, pReceive, Len, WaitTime );
}
