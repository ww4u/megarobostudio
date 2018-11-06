// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� MEGACANDEVICE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// MEGACANDEVICE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifndef _MEGA_CAN_DEVICE_H_
#define _MEGA_CAN_DEVICE_H_
#ifdef MEGACANDEVICE_EXPORTS
#define MEGACANDEVICE_API extern "C" __declspec(dllexport)
#else
//#define MEGACANDEVICE_API __declspec(dllimport)
#define MEGACANDEVICE_API
#endif

#define VCI_UNKNOW      0
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_MR_COMCAN 5
#define VCI_MR_USBCAN 6
#define VCI_MR_USBTMC 7  //tmc
#define VCI_MR_LANCAN 8

#define VCI_MCP_CAN  9

//CAN������
#define	ERR_CAN_OVERFLOW			0x0001	//CAN�������ڲ�FIFO���
#define	ERR_CAN_ERRALARM			0x0002	//CAN���������󱨾�
#define	ERR_CAN_PASSIVE				0x0004	//CAN��������������
#define	ERR_CAN_LOSE				0x0008	//CAN�������ٲö�ʧ
#define	ERR_CAN_BUSERR				0x0010	//CAN���������ߴ���
#define	ERR_CAN_REG_FULL			0x0020	//CAN���ռĴ�����
#define	ERR_CAN_REG_OVER			0x0040	//CAN���ռĴ������
#define	ERR_CAN_ZHUDONG	    		0x0080	//CAN��������������

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

#define CMD_DESIP			0
#define CMD_DESPORT			1
#define CMD_CHGDESIPANDPORT		2


//1.ZLGCANϵ�нӿڿ���Ϣ���������͡�
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

//2.����CAN��Ϣ֡���������͡�
typedef  struct  _CAN_OBJ {
	unsigned int	ID;
	unsigned int	TimeStamp;
	unsigned char	TimeFlag;
	unsigned char	SendType;
	unsigned char	RemoteFlag;//�Ƿ���Զ��֡
	unsigned char	ExternFlag;//�Ƿ�����չ֡
	unsigned char	DataLen;
	unsigned char	Data[8];
	unsigned char	Reserved[3];
}CAN_OBJ, *P_CAN_OBJ;

//3.����CAN������״̬���������͡�
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

//4.���������Ϣ���������͡�
typedef struct _ERR_INFO {
	unsigned int	ErrCode;
	unsigned char	Passive_ErrData[3];
	unsigned char	ArLost_ErrData;
} ERR_INFO, *P_ERR_INFO;

//5.�����ʼ��CAN����������
typedef struct _INIT_CONFIG {
	unsigned int	AccCode;
	unsigned int	AccMask;
	unsigned int	baud;
	unsigned char	Filter;
	unsigned char	Timing0;//not use
	unsigned char	Timing1;//not use
	unsigned char	Mode;
}INIT_CONFIG, *P_INIT_CONFIG;

typedef struct _tagChgDesIPAndPort
{
	char szpwd[10];
	char szdesip[20];
	int desport;
}CHGDESIPANDPORT;
 
//#define CALL  __stdcall 
//#define CALL  __cdecl
#define CALL

/***********************************************************************************************
* 函 数 名：VCI_FindDevice
* 功    能：查找设备
* 输入参数： DeviceType: 设备类型
*            desc_len  : 设备描述符长度
* 输出参数： desc[]    : 设备描述符；设备描述符用 ";" 隔开
* 返 回 值：返回值 = 0，没有设备； 大于0，查找到的资源的个数
* 说    明：
* ********************************************************************************************/
unsigned int VCI_FindDevice(unsigned int DeviceType, char desc[], int desc_len);

/***********************************************************************************************
* 函 数 名：VCI_OpenDevice
* 功    能：打开设备
* 输入参数： DeviceType: 设备类型
*       desc[]    : 设备描述符
* 输出参数： instr     : 设备句柄
* 返 回 值：返回值 = 0，打开成功；否则打开失败
* 说    明：
* ********************************************************************************************/
unsigned int VCI_OpenDevice(unsigned int DeviceType, char desc[], unsigned int* instr);

/***********************************************************************************************
* 函 数 名：VCI_CloseDevice
* 功    能：关闭设备
* 输入参数： DeviceType: 设备类型
*            handle    : 需要关闭的句柄
* 输出参数：
* 返 回 值：返回值 = 0，关闭成功；否则关闭失败
* 说    明：
* ********************************************************************************************/
unsigned int VCI_CloseDevice(unsigned int DeviceType, unsigned int handle);

/***********************************************************************************************
* 函 数 名：VCI_InitCAN
* 功    能：初始化can
* 输入参数： DeviceType  : 设备类型
*            instr       : 设备资源句柄
*			 CANInd      : can id
*            pInitConfig : can 配置信息
* 输出参数：
* 返 回 值：返回值 = 0，初始化成功；否则初始化失败
* 说    明：
* ********************************************************************************************/
unsigned int VCI_InitCAN(unsigned int DeviceType, unsigned int instr, unsigned int CANInd, P_INIT_CONFIG pInitConfig);

/***********************************************************************************************
* 函 数 名：VCI_ClearBuffer
* 功    能：清空设备缓存
* 输入参数： DeviceType  : 设备类型
*            instr       : 设备资源句柄
*			 CANInd      : can id
* 输出参数：
* 返 回 值：返回值 = 0，成功；否则失败
* 说    明：
* ********************************************************************************************/
unsigned int  VCI_ClearBuffer(unsigned int DeviceType, unsigned int instr, unsigned int CANInd);


/***********************************************************************************************
* 函 数 名：VCI_ResetCAN
* 功    能：复位can
* 输入参数： DeviceType  : 设备类型
*            instr       : 设备资源句柄
*			 CANInd      : can id
* 输出参数：
* 返 回 值：返回值 = 0，成功；否则失败
* 说    明：
* ********************************************************************************************/
MEGACANDEVICE_API unsigned int  CALL VCI_ResetCAN(unsigned int DeviceType, unsigned int instr, unsigned int CANInd);


/***********************************************************************************************
* 函 数 名：VCI_Transmit
* 功    能：传输数据
* 输入参数： DeviceType  : 设备类型
*            instr       : 设备资源句柄
*			 CANInd      : can id
*			 pSend       : 需要发送的can帧
*			 Len		 : 要发送的帧结构体数组的长度（发送的帧数量）。 最大1000
* 输出参数：
* 返 回 值：返回值 = 0，表示操作失败；否则为发送的CAN帧数
* 说    明：MRH-T USB 暂时只支持单个can帧发送
* ********************************************************************************************/
MEGACANDEVICE_API unsigned int  CALL VCI_Transmit(unsigned int DeviceType, unsigned int instr, unsigned int CANInd, P_CAN_OBJ pSend, unsigned int Len);


/***********************************************************************************************
* 函 数 名：VCI_Receive
* 功    能：回读数据
* 输入参数： DeviceType  : 设备类型
*            instr       : 设备资源句柄
*			 CANInd      : can id
*			 pReceive    : 接收can帧的缓存区
*			 Len		 : 接收can帧缓存区的长度
*			 WaitTime    : 超时时间（只支持MRH-E配置超时）
* 输出参数：
* 返 回 值：返回值 = 0，表示操作失败；否则为发送的CAN帧数
* 说    明：
* ********************************************************************************************/
MEGACANDEVICE_API unsigned int  CALL VCI_Receive(unsigned int DeviceType, unsigned int instr, unsigned int CANInd, P_CAN_OBJ pReceive, unsigned int Len, int WaitTime);


/***********************************************************************************************
* 函 数 名：VCI_Write
* 功    能：发送数据
* 输入参数： DeviceType  : 设备类型
*            instr       : 设备资源句柄
*			 data        : 所发送的数据
*			 Len		 : 发送的数据长度
* 输出参数：
* 返 回 值：返回值 = 0，表示发送成功；否则为发送失败
* 说    明：
* ********************************************************************************************/
MEGACANDEVICE_API unsigned int CALL VCI_Write(unsigned int DeviceType, unsigned int instr, char * data, unsigned int Len);

/***********************************************************************************************
* 函 数 名：VCI_Read
* 功    能：回读数据
* 输入参数： DeviceType  : 设备类型
*            instr       : 设备资源句柄
*			 pReceive    : 回读数据缓存区
*			 Length		 : 回读数据缓存区长度
*            WaitTime    : 超时时间（只支持MRH-E配置超时）
* 输出参数：
* 返 回 值：返回值 = 0，表示发送成功；成功：返回回读数据长度
* 说    明：
* ********************************************************************************************/
MEGACANDEVICE_API unsigned int CALL VCI_Read(unsigned int DeviceType, unsigned int instr, char* pReceive, unsigned int Length, int WaitTime);

#endif // MEGA_CAN_DEVICE_H_
