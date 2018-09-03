#ifndef BSP_CANAPI_H
#define BSP_CANAPI_H

/*24字节*/
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

#define VCI_UNKNOW      0
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_MR_COMCAN 5
#define VCI_MR_USBCAN 6
#define VCI_MR_USBTMC 7  //tmc
#define VCI_MR_LANCAN 8

#define VCI_MCP_CAN  9

void bspCanConfigMode(unsigned char mode);
void bspCanConfigBaud(unsigned int baud);

int  bspCanRead(CAN_OBJ * message);
int  bspCanSend(CAN_OBJ * message);
int  bspCanInit();
void bspCanReset();
int  bspCanGetRxCount();
void bspCanClearRxBuf();
void bspCanClose();

//! packer api
unsigned int bspcan_OpenDevice(unsigned int DeviceType, unsigned int DeviceInd, unsigned int Reserved);
unsigned int  bspcan_CloseDevice(unsigned int DeviceType, unsigned int DeviceInd);

unsigned int bspcan_InitCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_INIT_CONFIG pInitConfig);

//unsigned int  VCI_ReadBoardInfo(unsigned int DeviceType, unsigned int DeviceInd, P_BOARD_INFO pInfo);
//unsigned int VCI_ReadErrInfo(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_ERR_INFO pErrInfo);
//unsigned int VCI_ReadCANStatus(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_STATUS pCANStatus);

//unsigned int  VCI_GetReference(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, unsigned int RefType, void* pData);
//unsigned int  VCI_SetReference(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, unsigned int RefType, void* pData);

unsigned int  bspcan_GetReceiveNum(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);
unsigned int bspcan_ClearBuffer(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);

unsigned int bspcan_StartCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);
unsigned int  bspcan_ResetCAN(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd);

unsigned int  bspcan_Transmit(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pSend, unsigned int Len);
unsigned int  bspcan_Receive(unsigned int DeviceType, unsigned int DeviceInd, unsigned int CANInd, P_CAN_OBJ pReceive, unsigned int Len, int WaitTime);

unsigned int bspcan_Write(unsigned int DeviceType, unsigned int DeviceInd, char * data, unsigned int Len);
unsigned int bspcan_Read(unsigned int DeviceType, unsigned int DeviceInd, char* pReceive, unsigned int Length, int WaitTime);

unsigned int bspcan_FindDevice(unsigned int DeviceType, char* desc);

#endif // CANAPI_H
