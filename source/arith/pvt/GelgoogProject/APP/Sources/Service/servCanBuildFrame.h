/*********************************************************************************************
Copyright (C) 2017������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servCanBuildFrame.c;
��������:  ���ģ�⹹��CAN�����ϵ�֡����
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  xyzheng
�������:  2017.03.29;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __SERV_CAN_BUILD_FRAME__
#define __SERV_CAN_BUILD_FRAME__
#include "bspCan.h"



#define CAN_RX_CTRL    0x01 //�Ĵ�����ַ
#define CAN_RATE_L   2  //�Ĵ�����ַ
#define CAN_RATE_H   3  //�Ĵ�����ַ
#define CAN_START_FRAM 4   //�Ĵ�����ַ

/* CAN�����ϵĵ�ƽ�����Ե�ƽ�����Ե�ƽ���֡�
  ���Ե�ƽ���߼�ֵΪ��0�������Ե�ƽΪ��1����
*/

#define MAP_SHORT_LEN     7
#define MAP_BIT_LEN    (MAP_SHORT_LEN*16)  /*16*6*/
typedef struct //���֧��80bit
{
   u16 map[MAP_SHORT_LEN];
}BitMap;

//λͼ����
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

//����������չ֡λͼ�е�λ��
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

//�������ڱ�׼֡λͼ�е�λ��
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

#define GET_EXT_CRC_LEN(x)  (39 + x*8)  //��ȡ��չ֡CRCУ�����ݳ���,xΪ���ݳ���,��λ�ֽ�
#define GET_STD_CRC_LEN(x)  (19 + x*8)  //��ȡ��׼֡CRCУ�����ݳ���,xΪ���ݳ���,��λ�ֽ�

/*********************************************************************************************
�� �� ��: servCanBuildFrameSendMotionStartFrame2Fpga
ʵ�ֹ���: 
�������: 
�������: ��
�� �� ֵ: 0:������ȷ; -1:���ͳ���
˵    ��: 
*********************************************************************************************/
int servCanBuildFrameSendMotionStartFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud);
/*********************************************************************************************
�� �� ��: servCanBuildFrameRstFpgaCanRecieve
ʵ�ֹ���: ��λ�߼�CAN����ģ��
�������: 
�������: ��
�� �� ֵ: 0:������ȷ; -1:���ͳ���
˵    ��: 
*********************************************************************************************/
int servCanBuildFrameRstFpgaCanRecieve();
/*********************************************************************************************
�� �� ��: servCanBuildFrameSendClockSyncStartFrame2Fpga
ʵ�ֹ���: ����CLOCK:SYNC START֡,�����͸��߼�
�������: 
�������: ��
�� �� ֵ: 0:������ȷ; -1:���ͳ���
˵    ��: 
*********************************************************************************************/
int servCanBuildFrameSendClockSyncStartFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud);
/*********************************************************************************************
�� �� ��: servCanBuildFrameSendClockSyncEndFrame2Fpga
ʵ�ֹ���: ����CLOCK:SYNC END֡,�����͸��߼�
�������: 
�������: ��
�� �� ֵ: 0:������ȷ; -1:���ͳ���
˵    ��: 
*********************************************************************************************/
int servCanBuildFrameSendClockSyncEndFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud);



#endif
