/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspCan.c
功能描述:  
函数列表:  
当前版本:  1.0.0
版本改动:  
作    者:  xyzheng
完成日期:  Oct 11, 2017  3:18:22 PM
历史版本:  无
*********************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <linux/can.h>
#include "bspCan.h"
//#include "os_interface.h"
#include <linux/spi/spi-mcp251x.h>


#if DEBUG_BSP_CAN
#define can_debug(fmt,...)  do{printf(":%s:%d:",__FUNCTION__,__LINE__);printf(fmt,##__VA_ARGS__);}while(0)
#else
#define can_debug(fmt,...)
#endif


#define   DEVICE_NAME  "/dev/mcp251x" //设备名
CAN_INFO g_stCanInfo =
{
    -1,
};

/*********************************************************************************************
函 数 名: bspCanSend;
实现功能: can 发送CAN帧数据
输入参数:
输出参数: 无
返 回 值: 无
说    明:
*********************************************************************************************/
int bspCanSend(CAN_OBJ * message)
{
    struct can_frame frame ;
    int nbytes = 0,try_time = 50;

    if(g_stCanInfo.fd < 0)
	{
		return -1;
	}

    frame.can_id = message->ID;
    if(message->ExternFlag)
	{
		frame.can_id  &= CAN_EFF_MASK;
		frame.can_id  |= CAN_EFF_FLAG;
	}
	else
	{
		frame.can_id  &= CAN_SFF_MASK;
	}
#if 0
	if(message->RemoteFlag)
	{
		frame.can_id |= CAN_RTR_FLAG;
	}
#endif
    frame.can_dlc = message->DataLen;
    memcpy(frame.data,message->Data,frame.can_dlc);
    while(try_time)
    {
        nbytes = write(g_stCanInfo.fd,reinterpret_cast<unsigned char*>(&frame), sizeof(struct can_frame));

        if(nbytes ==  sizeof(struct can_frame) )
        {
            return 0;//发送正确
        }
        usleep(10000);
        try_time--;
    }
    printf("can send error: %d\n",nbytes);
	return -2;
}
/*********************************************************************************************
函 数 名: bspCanRead;
实现功能: 从驱动层读取一个CAN帧
输入参数:
输出参数: 无
返 回 值: 无
说    明: 阻塞读函数。如果没有数据，则在此等待，直到有数据。
*********************************************************************************************/
int bspCanRead(CAN_OBJ * message)
{
	struct can_frame frame;

    int nbytes = read(g_stCanInfo.fd, &frame, sizeof(struct can_frame));

	if(nbytes != sizeof(frame))
	{
		return -1;
	}

	if(frame.can_id & CAN_EFF_FLAG)
	{
		message->ExternFlag = 1;
		message->ID = frame.can_id & CAN_EFF_MASK;
	}
	else
	{
		message->ExternFlag = 0;
		message->ID = frame.can_id & CAN_SFF_MASK;
	}
	message->RemoteFlag = (frame.can_id & CAN_RTR_FLAG)?1:0;
	message->DataLen = frame.can_dlc;
	memcpy(message->Data,frame.data,frame.can_dlc);
	return 0;
}
/*********************************************************************************************
函 数 名: bspCanClearRxBuf
实现功能: 清空接收队列
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspCanClearRxBuf()
{
    ioctl(g_stCanInfo.fd, SPI_CAN_CLEARBUF,0); 
}
void bspCanConfigMode(unsigned char mode)
{
#if 0
    //ioctl(g_stCanInfo.fd,SPI_CAN_WR_CONFIG,(unsigned int)(pConfig));
#else
    UNUSED(mode);
#endif
}
void bspCanConfigBaud(unsigned int baud)
{
#if 0
    //ioctl(g_stCanInfo.fd,SPI_CAN_WR_CONFIG,(unsigned int)(pConfig));
#else
    UNUSED(baud);
#endif
}

void bspCanReset()
{
    ioctl(g_stCanInfo.fd, SPI_CAN_WR_RESET,0);
}

/*********************************************************************************************
函 数 名: bspCanInit;
实现功能: can 初始化
输入参数: baud:波特率 ：1000000，500000
	     mode: 0->normal; 1->loopback
输出参数: 无
返 回 值: 无
说    明:
*********************************************************************************************/
int bspCanInit()
{
    g_stCanInfo.fd = open(DEVICE_NAME,O_RDWR);
    if(g_stCanInfo.fd < 0)
    {
        perror("open /dev/mcp251x");
        return -1;
    }
    //!can接收默认是阻塞模式,如果不想使用阻塞模式，则打开此处
    fcntl(g_stCanInfo.fd, F_SETFL, O_NONBLOCK);

    ioctl(g_stCanInfo.fd, SPI_CAN_CLEARBUF,0);

    return 0;
}

void bspCanClose()
{
    close(g_stCanInfo.fd);
}
