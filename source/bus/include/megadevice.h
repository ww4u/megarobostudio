#ifndef MEGADEVICE_H
#define MEGADEVICE_H

#define VCI_UNKNOW      0
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_MR_COMCAN 5
#define VCI_MR_USBCAN 6
#define VCI_MR_USBCAN2 7  //tmc
#define VCI_MR_LANCAN 8

#define VCI_MCP_CAN  9

typedef struct _INIT_CONFIG {
    unsigned int	AccCode;
    unsigned int	AccMask;
    unsigned int    Reserved;
    unsigned int	baud;
    unsigned char	Filter;
    unsigned char	Timing0;//not use
    unsigned char	Timing1;//not use
    unsigned char	Mode;
}VCI_INIT_CONFIG, *P_INIT_CONFIG;

#define INIT_CONFIG VCI_INIT_CONFIG

typedef struct _CAN_OBJ
   {
       unsigned int ID;               //报文帧ID
       unsigned int TimeStamp;        //时间标识
       unsigned char TimeFlag;         //是否使用时间标识
       unsigned char SendType;         //发送标志。保留，未用
       unsigned char RemoteFlag;       //是否是远程帧;=0时为数据帧，=1时为远程帧
       unsigned char ExternFlag;       //是否是扩展帧;=0时为标准帧（11位帧ID），=1时为扩展帧（29位帧ID）
       unsigned char DataLen;
       unsigned char Data[8];
       unsigned char Reserved[3];
}VCI_CAN_OBJ, *P_CAN_OBJ;

#define CAN_OBJ VCI_CAN_OBJ

typedef  struct  _BOARD_INFO {
    unsigned short	hw_Version;
    unsigned short	fw_Version;
    unsigned short	dr_Version;
    unsigned short	in_Version;
    unsigned short	irq_Num;
    unsigned char	can_Num;
    char	str_Serial_Num[20];
    char	str_hw_Type[40];
    unsigned short	Reserved[4];
} BOARD_INFO, *P_BOARD_INFO;
    
typedef struct _ERR_INFO {
    unsigned int	ErrCode;
    unsigned char	Passive_ErrData[3];
    unsigned char	ArLost_ErrData;
} ERR_INFO, *P_ERR_INFO;

typedef struct _CAN_STATUS {
    unsigned char	ErrInterrupt;
    unsigned char	regMode;
    unsigned char	regStatus;
    unsigned char	regALCapture;
    unsigned char	regECCapture;
    unsigned char	regEWLimit;
    unsigned char	regRECounter;
    unsigned char	regTECounter;
    unsigned int	Reserved;
}CAN_STATUS, *P_CAN_STATUS;

#ifdef __cplusplus
extern "C" {
#endif

unsigned int VCI_OpenDevice(unsigned int DeviceType, unsigned int DeviceInd, unsigned int Reserved);
unsigned int  VCI_CloseDevice(unsigned int DeviceType, unsigned int DeviceInd);

unsigned int VCI_InitCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_INIT_CONFIG pInitConfig);

unsigned int  VCI_ReadBoardInfo(unsigned int DeviceType, unsigned int DeviceInd, P_BOARD_INFO pInfo);
unsigned int VCI_ReadErrInfo(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_ERR_INFO pErrInfo);
unsigned int VCI_ReadCANStatus(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_STATUS pCANStatus);

unsigned int  VCI_GetReference(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, unsigned int RefType, void* pData);
unsigned int  VCI_SetReference(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, unsigned int RefType, void* pData);

unsigned int  VCI_GetReceiveNum(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);
unsigned int VCI_ClearBuffer(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);

unsigned int VCI_StartCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);
unsigned int  VCI_ResetCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);

unsigned int  VCI_Transmit(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pSend, unsigned int Len);
unsigned int  VCI_Receive(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pReceive, unsigned int Len, int WaitTime);

unsigned int VCI_Write(unsigned int DeviceType, unsigned int DeviceInd, char * data, unsigned int Len);
unsigned int VCI_Read(unsigned int DeviceType, unsigned int DeviceInd, char* pReceive, unsigned int Length, int WaitTime);

unsigned int VCI_FindDevice(unsigned int DeviceType, char* desc);
#ifdef __cplusplus
}
#endif

#endif // MEGADEVICE_H
