#ifndef CANAPI_H
#define CANAPI_H

#include <QtCore>

#define ULONG   uint32
#include "./include/MegaCanDevice.h"

namespace MegaDevice {

#undef CALL
#define CALL
typedef unsigned int  CALL (*p_VCI_OpenDevice)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int Reserved);
typedef unsigned int  CALL (*p_VCI_CloseDevice)(unsigned int DeviceType, unsigned int DeviceInd);

typedef unsigned int  CALL (*p_VCI_InitCAN)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_INIT_CONFIG pInitConfig);

typedef unsigned int  CALL (*p_VCI_GetReceiveNum)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);
typedef unsigned int  CALL (*p_VCI_ClearBuffer)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);

typedef unsigned int  CALL (*p_VCI_StartCAN)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);

typedef unsigned int  CALL (*p_VCI_Transmit)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pSend, unsigned int Len);
typedef unsigned int  CALL (*p_VCI_Receive)(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pReceive, unsigned int Len, int WaitTime);

class CANApi
{
public:
    CANApi();
    ~CANApi();
    bool load( QString str );
    void unload();

protected:
    bool loadApi();

public:
    p_VCI_OpenDevice open;
    p_VCI_CloseDevice close;

    p_VCI_InitCAN init;

    p_VCI_GetReceiveNum getSize;
    p_VCI_ClearBuffer clear;

    p_VCI_StartCAN start;

    p_VCI_Transmit transmit;
    p_VCI_Receive receive;

public:
    QLibrary mDll;
};

}

#endif // CANAPI_H
