/*********************************************************************************************
Copyright (C) 2017，北京镁伽机器人科技有限公司
文 件 名:  servCanBuildFrame.c;
功能描述:  软件模拟构建CAN总线上的帧数据
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  xyzheng
完成日期:  2017.03.29;
历史版本:  无;
*********************************************************************************************/
#ifndef __SERV_CAN_BUILD_FRAME__
#define __SERV_CAN_BUILD_FRAME__
#include "bspCan.h"



#define CAN_RX_CTRL    0x01 //寄存器地址
#define CAN_RATE_L   2  //寄存器地址
#define CAN_RATE_H   3  //寄存器地址
#define CAN_START_FRAM 4   //寄存器地址

/* CAN总线上的电平有显性电平和隐性电平两种。
  显性电平的逻辑值为“0”，隐性电平为“1”。
*/

#define MAP_SHORT_LEN     7
#define MAP_BIT_LEN    (MAP_SHORT_LEN*16)  /*16*6*/
typedef struct //最多支持80bit
{
   u16 map[MAP_SHORT_LEN];
}BitMap;

//位图操作
static inline void mapSetBit(BitMap* pBitmap,u8 bitPosition)
{
    if(bitPosition < MAP_BIT_LEN)
    {
        pBitmap->map[bitPosition>>4] |= 0x01<<(15-bitPosition&0x0F);
    } 
}
static inline void mapClearBit(BitMap* pBitmap,u8 bitPosition)
{
    if(bitPosition < MAP_BIT_LEN)
    {
        pBitmap->map[bitPosition>>4] &=~(0x01<<(15-bitPosition&0x0F));
    }
}
static inline u8 mapGetBit(BitMap* pBitmap,u8 bitPosition)
{
    u8 bit = 0;
    if(bitPosition < MAP_BIT_LEN)
    {
        bit = (u8)(pBitmap->map[bitPosition>>4] >>(15-bitPosition&0x0F));
    }
    return bit&0x01;
}

//各个场在扩展帧位图中的位置
#define BIT_SOF       0
#define BIT_EXT_ID_11 1
#define BIT_EXT_SRR   12
#define BIT_EXT_IDE   13
#define BIT_EXT_ID_18 14
#define BIT_EXT_RTR   32
#define BIT_EXT_R1    33
#define BIT_EXT_R0    34
#define BIT_EXT_DLC   35
#define BIT_EXT_DATA  39
#define BIT_EXT_CRC   63
#define BIT_EXT_CRC_DEL  78
#define BIT_EXT_ACK      79
#define BIT_EXT_ACK_DEL  80
#define BIT_EXT_EOF      81

//各个场在标准帧位图中的位置
#define BIT_STD_ID   1
#define BIT_STD_RTR  12
#define BIT_STD_IDE  13
#define BIT_STD_R0   14
#define BIT_STD_DLC  15
#define BIT_STD_DATA 19
#define BIT_STD_CRC  43
#define BIT_STD_CRC_DEL 58
#define BIT_STD_ACK     59
#define BIT_STD_ACK_DEL 60
#define BIT_STD_EOF     61

#define GET_EXT_CRC_LEN(x)  (39 + x*8)  //获取扩展帧CRC校验数据长度,x为数据长度,单位字节
#define GET_STD_CRC_LEN(x)  (19 + x*8)  //获取标准帧CRC校验数据长度,x为数据长度,单位字节

/*********************************************************************************************
函 数 名: servCanBuildFrameSendMotionStartFrame2Fpga
实现功能: 
输入参数: 
输出参数: 无
返 回 值: 0:发送正确; -1:发送出错
说    明: 
*********************************************************************************************/
int servCanBuildFrameSendMotionStartFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud);
/*********************************************************************************************
函 数 名: servCanBuildFrameRstFpgaCanRecieve
实现功能: 复位逻辑CAN接收模块
输入参数: 
输出参数: 无
返 回 值: 0:发送正确; -1:发送出错
说    明: 
*********************************************************************************************/
int servCanBuildFrameRstFpgaCanRecieve();
/*********************************************************************************************
函 数 名: servCanBuildFrameSendClockSyncStartFrame2Fpga
实现功能: 构建CLOCK:SYNC START帧,并发送给逻辑
输入参数: 
输出参数: 无
返 回 值: 0:发送正确; -1:发送出错
说    明: 
*********************************************************************************************/
int servCanBuildFrameSendClockSyncStartFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud);
/*********************************************************************************************
函 数 名: servCanBuildFrameSendClockSyncEndFrame2Fpga
实现功能: 构建CLOCK:SYNC END帧,并发送给逻辑
输入参数: 
输出参数: 无
返 回 值: 0:发送正确; -1:发送出错
说    明: 
*********************************************************************************************/
int servCanBuildFrameSendClockSyncEndFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud);



#endif
