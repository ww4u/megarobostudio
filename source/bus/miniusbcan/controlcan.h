
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CONTROLCAN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CONTROLCAN_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

/*
#ifdef CONTROLCAN_EXPORT
#define CONTROLCAN_API  __declspec(dllexport)
#else
#define CONTROLCAN_API  __declspec(dllimport)
#endif
*/
#define CONTROLCAN_API extern "C" __declspec(dllexport)

//�ӿڿ����Ͷ���
#define VCI_PCI5121		1
#define VCI_PCI9810		2
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_PCI9820		5
#define VCI_CAN232		6
#define VCI_PCICAN		7

//CAN������
#define	ERR_CAN_OVERFLOW			0x0001	//CAN�������ڲ�FIFO���
#define	ERR_CAN_ERRALARM			0x0002	//CAN���������󱨾�
#define	ERR_CAN_PASSIVE				0x0004	//CAN��������������
#define	ERR_CAN_LOSE				0x0008	//CAN�������ٲö�ʧ
#define	ERR_CAN_BUSERR				0x0010	//CAN���������ߴ���

//ͨ�ô�����
#define	ERR_DEVICEOPENED			0x0100	//�豸�Ѿ���
#define	ERR_DEVICEOPEN				0x0200	//���豸����
#define	ERR_DEVICENOTOPEN			0x0400	//�豸û�д�
#define	ERR_BUFFEROVERFLOW			0x0800	//���������
#define	ERR_DEVICENOTEXIST			0x1000	//���豸������
#define	ERR_LOADKERNELDLL			0x2000	//װ�ض�̬��ʧ��
#define ERR_CMDFAILED				0x4000	//ִ������ʧ�ܴ�����
#define	ERR_BUFFERCREATE			0x8000	//�ڴ治��


//�������÷���״ֵ̬
#define	STATUS_OK					1
#define STATUS_ERR					0


//dsPIC33�ڲ��Ĵ�����ַ��
#define REG_C1CTRL1						0x0400
#define REG_C1CTRL2						0x0402
#define REG_C1VEC						0x0404
#define	REG_C1FCTRL						0x0406
#define REG_C1FIFO						0x0408
#define REG_C1INTF						0x040A
#define	REG_C1INTE						0x040C
#define REG_C1EC						0x040E 
#define REG_C1CFG1						0x0410
#define REG_C1CFG2						0x0412
#define REG_C1FEN1						0x0414
#define REG_C1FMSKSEL1					0x0418 
#define REG_C1FMSKSEL2					0x041A
#define REG_C1RXFUL1					0x0420
#define REG_C1RXFUL2					0x0422


//1.ZLGCANϵ�нӿڿ���Ϣ���������͡�
typedef  struct  _VCI_BOARD_INFO{
		USHORT	hw_Version;
		USHORT	fw_Version;
		USHORT	dr_Version;
		USHORT	in_Version;
		USHORT	irq_Num;
		BYTE	can_Num;
		CHAR	str_Serial_Num[20];
		CHAR	str_hw_Type[40];
		USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//2.����CAN��Ϣ֡���������͡�
typedef  struct  _VCI_CAN_OBJ{
	UINT	ID;
	UINT	TimeStamp;
	BYTE	TimeFlag;
	BYTE	SendType;
	BYTE	RemoteFlag;//�Ƿ���Զ��֡
	BYTE	ExternFlag;//�Ƿ�����չ֡
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.����CAN������״̬���������͡�
typedef struct _VCI_CAN_STATUS{
	UCHAR	ErrInterrupt;
	UCHAR	regMode;
	UCHAR	regStatus;
	UCHAR	regALCapture;
	UCHAR	regECCapture; 
	UCHAR	regEWLimit;
	UCHAR	regRECounter; 
	UCHAR	regTECounter;
	DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.���������Ϣ���������͡�
typedef struct _ERR_INFO{
		UINT	ErrCode;
		BYTE	Passive_ErrData[3];
		BYTE	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.�����ʼ��CAN����������
typedef struct _INIT_CONFIG{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	Reserved;
	UCHAR	Filter;
	UCHAR	Timing0;
	UCHAR	Timing1;
	UCHAR	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;
//#define DECL    __cdecl
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//CONTROLCAN_API DWORD DECL VCI_OpenDevice(DWORD DeviceType,DWORD DeviceInd,DWORD Reserved);
//CONTROLCAN_API DWORD DECL VCI_CloseDevice(DWORD DeviceType,DWORD DeviceInd);
//CONTROLCAN_API DWORD DECL VCI_InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);
//
//CONTROLCAN_API ULONG DECL VCI_GetReceiveNum(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
//
//CONTROLCAN_API DWORD DECL VCI_ClearBuffer(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
//
//CONTROLCAN_API DWORD DECL VCI_StartCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
//CONTROLCAN_API DWORD DECL VCI_ResetCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
//
//CONTROLCAN_API ULONG DECL VCI_Transmit(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pSend,ULONG Len);
//CONTROLCAN_API ULONG DECL VCI_Receive(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime=-1);
//
//
//#ifdef __cplusplus
//}
//#endif
