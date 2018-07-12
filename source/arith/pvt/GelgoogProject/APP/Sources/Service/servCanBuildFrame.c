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
#include "servCanBuildFrame.h"
#include "bspSdio.h"
#include "servFPGA.h"
#include "comCmd.h"

BitMap  bitMap;



/*********************************************************************************************
�� �� ��: servCanBuildExtFrameInit;
ʵ�ֹ���: ��չ֡��ʼ��
�������: RTR:�Ƿ���Զ��֡
�������: ��
�� �� ֵ: ��
˵    ��:
*********************************************************************************************/
static void servCanBuildExtFrameInit(u8 RTR)
{
   int i = 0;
   
   for(i = 0;i < MAP_SHORT_LEN;i++)
   {
        bitMap.map[i] = 0x0000;
   }
   //sof ����
   mapClearBit(&bitMap,BIT_SOF);
  
   //IDE
   mapSetBit(&bitMap,BIT_EXT_IDE);//��չ֡,����
   //SRR
   mapSetBit(&bitMap,BIT_EXT_SRR);//����
   
   //����֡,Զ��֡
   if(RTR)
   {
      mapSetBit(&bitMap,BIT_EXT_RTR);//Զ��֡,���� 
   }
   else
   {
      mapClearBit(&bitMap,BIT_EXT_RTR);//����֡,����
   }
   //r1,r0 ����,����������λ���
   mapClearBit(&bitMap,BIT_EXT_R0);
   mapClearBit(&bitMap,BIT_EXT_R1);
   //crc�����,���� 
   mapSetBit(&bitMap,BIT_EXT_CRC_DEL);
   //ACK��ACK�����
   mapSetBit(&bitMap,BIT_EXT_ACK);
   mapSetBit(&bitMap,BIT_EXT_ACK_DEL);
   //EOF
   for(i = 0;i < 7;i++)
   {
       mapSetBit(&bitMap,BIT_EXT_EOF + i);
   }
}

/*********************************************************************************************
�� �� ��: servCanBuildStdFrameInit
ʵ�ֹ���: ��׼֡��ʼ��
�������: RTR:�Ƿ���Զ��֡
�������: ��
�� �� ֵ: ��
˵    ��:
*********************************************************************************************/
static void servCanBuildStdFrameInit(u8 RTR)
{
   int i = 0;
   for(i = 0;i < MAP_SHORT_LEN;i++)
   {
        bitMap.map[i] = 0x0000;
   }
   //SOF ����
   mapClearBit(&bitMap,BIT_SOF);
   
   //IDE
   mapClearBit(&bitMap,BIT_STD_IDE);//��׼֡,����
  
   //����֡,Զ��֡
   if(RTR)
   {
      mapSetBit(&bitMap,BIT_STD_RTR);//Զ��֡,����
   }
   else
   {
      mapClearBit(&bitMap,BIT_STD_RTR);//����֡,����
   }
   //r0 ����,����������λ���
   mapClearBit(&bitMap,BIT_STD_R0);
   //crc�����,����
   mapSetBit(&bitMap,BIT_STD_CRC_DEL);
   //ACK��ACK�����
   mapSetBit(&bitMap,BIT_STD_ACK);
   mapSetBit(&bitMap,BIT_STD_ACK_DEL);
   //EOF
   for(i = 0;i < 7;i++)
   {
       mapSetBit(&bitMap,BIT_STD_EOF + i);
   }
}
/*********************************************************************************************
�� �� ��: servMakeCRC15
ʵ�ֹ���: ����֡��CRC
�������: 
�������: ��
�� �� ֵ: ��
˵    ��:
    ˵�����и�����α����:
            CRC_RG = 0; // initialize shift register
        REPEAT
            CRCNXT = NXTBIT EXOR CRC_RG(14);
            CRC_RG(14:1) = CRC_RG(13:0); // shift left by
            CRC_RG(0) = 0; // 1 position        
            IF CRCNXT THEN
                CRC_RG(14:0) = CRC_RG(14:0) EXOR (4599hex);
            ENDIF
        UNTIL (CRC SEQUENCE starts or there is an ERROR condition)
*********************************************************************************************/
static u16 servMakeCRC15(BitMap * pBitMap,int len)  
{    
    u16 crc15 = 0,bit = 0,next;    //crc����
    int i=0;
    while(i<len)
    {
          bit = ((u16)mapGetBit(pBitMap,i))<<14;
          next = bit^crc15;
          crc15<<=1;
          if(next & 0x4000)
          {
               crc15^=0x4599;
          }
          i++;
    }   
    return (crc15&0x7fff);
}

/*********************************************************************************************
�� �� ��: servCanBuileFrame
ʵ�ֹ���: ����CAN֡
�������: 
�������: pBitMap:CAN֡����
�� �� ֵ: ����֡����(total)
˵    ��: pBitMap�Ŀռ�̶�Ϊ112bit(7*16bit)
*********************************************************************************************/
static int servCanBuileFrame(u32 id,u8 type,u8 rtr,u8 * data,u8 datalen,BitMap * pBitMap)
{
    int i = 0,j = 0,k1 = 0,k0 = 0;
    u16 crc15 = 0;
    int frame_len = 0;
    u8 bit = 0;
    
    if(type == CAN_ID_STD)//��׼֡
    {
        servCanBuildStdFrameInit(rtr);
        frame_len = GET_STD_CRC_LEN(datalen)+ 16;//����CRC��CRC���������
        for(i = 10;i >= 0;i--)
        {
            if(((id >> i)&0x01) == 0)
            {
                mapClearBit(&bitMap,BIT_EXT_ID_11 + i);
            }
            else
            {
                mapSetBit(&bitMap,BIT_EXT_ID_11 + i);
            }
        }
        for(i = 0;i < 4;i++)//����DLC
        {
            if((datalen>>(3 - i))&0x01)//�ȷŸ�λ(MSB)
            {
                mapSetBit(&bitMap,BIT_STD_DLC + i);//����
            }
            else
            {
                mapClearBit(&bitMap,BIT_STD_DLC + i);//����
            }
        }
        for(i = 0;i < datalen;i++)//��������
        {
            for(j = 0;j < 8;j++)
            {
                if((data[i]>>(7-j))&0x01)//�ȷŸ�λ(MSB)
                {
                    mapSetBit(&bitMap,BIT_STD_DATA + j + i * 8);//����
                }
                else
                {
                    mapClearBit(&bitMap,BIT_STD_DATA + j + i * 8);//����
                }
            }
        }
        //����CRC �����CRC 
        crc15 = servMakeCRC15(&bitMap,GET_STD_CRC_LEN(datalen));
        for(i = 0;i< 15;i++)
        {
            if((crc15>>(14 - i))&0x01)//�ȷŸ�λ(MSB)
            {
                mapSetBit(&bitMap,BIT_STD_CRC + i );//����
            }
            else
            {
                mapClearBit(&bitMap,BIT_STD_CRC + i);//����
            }
        } 
    }
    else
    {
        servCanBuildExtFrameInit(rtr);//��ʼ����չ֡,����Ҫ��λ
        frame_len = GET_EXT_CRC_LEN(datalen) + 16; //����CRC��CRC���������
        //����ID
        for(i = 0;i < 10;i++)
        {
            if(((id >> (28-i))&0x01))//�ȷŸ�λ(MSB)
            {
                mapSetBit(&bitMap,BIT_EXT_ID_11 + i);
            }
            else
            {
                mapClearBit(&bitMap,BIT_EXT_ID_11 + i);
            }
        }
        for(i = 0;i < 18;i++)//�ȷŸ�λ(MSB)
        {
            if(((id >> (17 - i))&0x01))
            {
                mapSetBit(&bitMap,BIT_EXT_ID_18 + i);
            }
            else
            {
                mapClearBit(&bitMap,BIT_EXT_ID_18 + i);//����
            }
        }

        for(i = 0;i < 4;i++)//����DLC
        {
            if((datalen>>(3 - i))&0x01)//�ȷŸ�λ(MSB)
            {
                mapSetBit(&bitMap,BIT_EXT_DLC + i);//����
            }
            else
            {
                mapClearBit(&bitMap,BIT_EXT_DLC + i);//����
            }
        }
        for(i = 0;i < datalen;i++)//��������
        {
            for(j = 0;j < 8;j++)
            {
                if((data[i]>>(7-j))&0x01)//�ȷŸ�λ(MSB)
                {
                    mapSetBit(&bitMap,BIT_EXT_DATA + j + i * 8);//����
                }
                else
                {
                    mapClearBit(&bitMap,BIT_EXT_DATA + j + i * 8);//����
                }
            }
        }
        //����CRC �����CRC 
        crc15 = servMakeCRC15(&bitMap,GET_EXT_CRC_LEN(datalen));
        for(i = 0;i< 15;i++)
        {
            if((crc15>>(14 - i))&0x01)//�ȷŸ�λ(MSB)
            {
                mapSetBit(&bitMap,BIT_EXT_CRC + i );//����
            }
            else
            {
                mapClearBit(&bitMap,BIT_EXT_CRC + i);//����
            }
        } 
    }
    
    /**************************** �� λ **************************************/
    for(i = 0;i < MAP_SHORT_LEN;i++)
    {
        pBitMap->map[i] = 0x0000;
    }
    for(i = 0,j = 0;i < frame_len;i++,j++)//ȥ��֡��������ACK��
    {
        bit = mapGetBit(&bitMap,i);
        if(bit)
        {
             mapSetBit(pBitMap,j);
             k1++;
             k0 = 0;
        }
        else
        {
             mapClearBit(pBitMap,j);
             k0++;
             k1 = 0;
        }
        if(k1 == 5)
        {
             j++;
             mapClearBit(pBitMap,j);//���1����
             k1 = 0;
             k0 = 1; //�����´β�λ
        }
        if(k0 == 5)
        {
             j++;
             mapSetBit(pBitMap,j);//������1
             k0 = 0;
             k1 = 1; //�����´β�λ
        }
    }
    //���112λ
    for(i = 0;i < 112;i++)
    {
        if(i < j)
        {
            if(mapGetBit(pBitMap,j-i-1))
            {
                mapSetBit(pBitMap,111-i);
            }
            else
            {
                mapClearBit(pBitMap,111-i);
            }
        }
        else
        {
            mapClearBit(pBitMap,111-i);
        }
    }
    return (j);//��������֡����(����ACK��EOF)
}
/*********************************************************************************************
�� �� ��: servCanBuileFrameSendFrame2Fpga
ʵ�ֹ���: ����CAN֡,�����͸��߼�
�������: baudrate:can�ڲ�����,��λbps
          id: ���Ϳ�ʼ����ʱ,ʹ�õ�CAN ID
          type: ��չ֡���Ǳ�׼֡
          pData:CAN�ڷ��͵�����,��������Ͳ���
          datalen:���ݳ���
�������: ��
�� �� ֵ: 0:������ȷ; �����ͳ���
˵    ��: 
*********************************************************************************************/
static int servCanBuileFrameSendFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud,u8 * pData,u8 datalen)
{
    BitMap  map;
    int len = 0,i = 0;
    u32 baudrate = 0;
    u32 rate = 0;
    u16 senddata = 0;

    switch(enBaud)
    {
    case CANBAUD_1000:   //1000kBps
      baudrate = 1000000;
      break;
    case CANBAUD_500:
      baudrate = 500000;
      break;
    case CANBAUD_250:
      baudrate = 250000;
      break;
    case CANBAUD_125:
      baudrate = 125000;
      break;
    case CANBAUD_100:
      baudrate = 100000;
      break;
    case CANBAUD_50:
      baudrate = 50000;
      break;
    case CANBAUD_20:
      baudrate = 20000;
      break;
    case CANBAUD_10:
      baudrate = 10000;
      break;
    default:
      return -4;
    }
    rate = (u32)(baudrate/1000000000.0*0x1000000*12.5);
    len = servCanBuileFrame(id,type,0,pData,datalen,&map);
    //����֡������rate��8λ
    senddata = (len&0xFF)<<8|((rate>>16)&0xFF);
    if(servFpgaRegisterWrite(0, CAN_RATE_H, senddata) != 0)
    {
        return -1;
    }
    //����rate��16λ
    senddata = rate&0xFFFF;
    if(servFpgaRegisterWrite(0, CAN_RATE_L, senddata) != 0)
    {
        return -2;
    }
    //����֡����(112λ)
    for(i = 0;i < MAP_SHORT_LEN;i++)
    {
        if(servFpgaRegisterWrite(0, CAN_START_FRAM+i, map.map[6-i]) != 0)
        {
            return -3;
        }
    }
    return 0;
}

/*********************************************************************************************
�� �� ��: servCanBuildFrameRstFpgaCanRecieve
ʵ�ֹ���: ��λ�߼�CAN����ģ��
�������: 
�������: ��
�� �� ֵ: 0:������ȷ; -1:���ͳ���
˵    ��: 
*********************************************************************************************/
int servCanBuildFrameRstFpgaCanRecieve()
{
    return servFpgaRegisterWrite(0, CAN_RX_CTRL, 1);
}

/*********************************************************************************************
�� �� ��: servCanBuildFrameSendMotionStartFrame2Fpga
ʵ�ֹ���: ����MOTIONRUN֡,�����͸��߼�
�������: 
�������: ��
�� �� ֵ: 0:������ȷ; -1:���ͳ���
˵    ��: 
*********************************************************************************************/
int servCanBuildFrameSendMotionStartFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud)
{
    u8 cmd[] = {CMD_MOTION, MOTIONCMD_SWITCH, CH1, MTSWITCH_RUN}; //MOTION:SWITCH RUN

    return servCanBuileFrameSendFrame2Fpga(id,type,enBaud,cmd,3);
}
/*********************************************************************************************
�� �� ��: servCanBuildFrameSendClockSyncStartFrame2Fpga
ʵ�ֹ���: ����CLOCK:SYNC START֡,�����͸��߼�
�������: 
�������: ��
�� �� ֵ: 0:������ȷ; -1:���ͳ���
˵    ��: 
*********************************************************************************************/
int servCanBuildFrameSendClockSyncStartFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud)
{
    u8 cmd[] = {CMD_CLOCK, CLOCK_SYNCSTATE, CLOCKSYNC_START}; //CLOCK:SYNC START
    return servCanBuileFrameSendFrame2Fpga(id,type,enBaud,cmd,3);
}
/*********************************************************************************************
�� �� ��: servCanBuildFrameSendClockSyncEndFrame2Fpga
ʵ�ֹ���: ����CLOCK:SYNC END֡,�����͸��߼�
�������: 
�������: ��
�� �� ֵ: 0:������ȷ; -1:���ͳ���
˵    ��: 
*********************************************************************************************/
int servCanBuildFrameSendClockSyncEndFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud)
{
    u8 cmd[] = {CMD_CLOCK, CLOCK_SYNCSTATE, CLOCKSYNC_END}; //CLOCK:SYNC END
    return servCanBuileFrameSendFrame2Fpga(id,type,enBaud,cmd,3);
}

void servSyncHandle(RegisterFuncEnum func, ReceiveTypeEnum receiveType, CanIntfcStruct canIntfc)    //CJ 2017.06.20 Modify
{
    u32 canId = 0;
    switch(func)
     {
     case REGISTER_NONE:
       servSetFpgaMultiSyncFunc(MULTI_SYNC_TRIG_SRC_OFF);
       break;
     case REGISTER_SOFTWARE:
       servSetFpgaMultiSyncFunc(MULTI_SYNC_TRIG_SRC_SW);
       break;
     case REGISTER_DIOSTART:
       servSetFpgaMultiSyncFunc(MULTI_SYNC_TRIG_SRC_HW);
       break;
     case REGISTER_CANSTART:
       servSetFpgaMultiSyncFunc(MULTI_SYNC_TRIG_SRC_CAN);
       if(receiveType == RECEIVE_RADIOID)
       {
            canId = canIntfc.radioId;
       }
       else if(receiveType == RECEIVE_GROUPID1)
       {
            canId = canIntfc.groupId1;
       }
       else if(receiveType == RECEIVE_GROUPID2)
       {
            canId = canIntfc.groupId2;
       }
       else if(receiveType == RECEIVE_RECEIVEID)
       {
            canId = canIntfc.receiveId;
       }
       servCanBuildFrameSendMotionStartFrame2Fpga(canId, canIntfc.idType, canIntfc.baud);
       break;
     case REGISTER_SYNCSTART:
       if(receiveType == RECEIVE_RADIOID)
       {
            canId = canIntfc.radioId;
       }
       else if(receiveType == RECEIVE_GROUPID1)
       {
            canId = canIntfc.groupId1;
       }
       else if(receiveType == RECEIVE_GROUPID2)
       {
            canId = canIntfc.groupId2;
       }
       else if(receiveType == RECEIVE_RECEIVEID)
       {
            canId = canIntfc.receiveId;
       }
       servSetFpgaMultiSyncFunc(MULTI_SYNC_RST);//����ϴμ���
       servSetFpgaMultiSyncFunc(MULTI_SYNC_START);//���ü���ʹ��,�ȴ���ʼ����
       servCanBuildFrameSendClockSyncStartFrame2Fpga(canId, canIntfc.idType, canIntfc.baud);
       //bspDelayUs(100);
       break;
     case REGISTER_SYNCEND:
       if(receiveType == RECEIVE_RADIOID)
       {
            canId = canIntfc.radioId;
       }
       else if(receiveType == RECEIVE_GROUPID1)
       {
            canId = canIntfc.groupId1;
       }
       else if(receiveType == RECEIVE_GROUPID2)
       {
            canId = canIntfc.groupId2;
       }
       else if(receiveType == RECEIVE_RECEIVEID)
       {
            canId = canIntfc.receiveId;
       }
       servSetFpgaMultiSyncFunc(MULTI_SYNC_END);//����ֹͣʹ��,�ȴ�ֹͣ����
       servCanBuildFrameSendClockSyncEndFrame2Fpga(canId, canIntfc.idType, canIntfc.baud);
       //bspDelayUs(100);
       break;
     }
}










#if  0  //���Դ���
void servCanFrameBuild_FpgaTest()
{
    //while(1)
    {
        servSetFpgaTrigSource(START_TRIG_SRC_CAN);
        //servSetFpgaTrigSource(START_TRIG_SRC_SW);
        servCanBuildFrameSendMotionStartFrame2Fpga(0x10000,CAN_ID_EXT,CANBAUD_1000);
    }
}


void servCanFrameBuild_test()
{
  int i = 0;
  BitMap testBitMap;
  u8  bit = 0;
  int frame_len = 0;
  u8 cmd[] = {8,0,3}; //MOTION:SWITCH RUN
  u8 str[256] = {0};
#if 0 
  for(i = 0;i < 96;i++)
  {
    mapSetBit(i);
  }
  for(i = 0;i < 96;i++)
  {
    mapClearBit(i);
  }
#endif

  frame_len = servCanBuileFrame(0x10000,CAN_ID_EXT,0,cmd,3,&testBitMap);
  printf("can frame:  ");
  for(i = 0; i< 112;i++)
  {
      bit = mapGetBit(&testBitMap,i);
      if(bit)
      {
           str[i] = 0x31;
      }
      else
      {
          str[i] = 0x30;
      }
  }
  printf("%s\n",str);
  printf("%04x%04x%04x%04x%04x%04x%04x\n",testBitMap.map[0],
         testBitMap.map[1],
         testBitMap.map[2],
         testBitMap.map[3],
         testBitMap.map[4],
         testBitMap.map[5],
         testBitMap.map[6]
         );
}
#endif

//000000000000010100000000000000000110011000010000000000000000011
//00000100000100010100000100000100000100110011000010000010000010000010011
//00000008228208266104104f1d3d
//00000100000100010100000100000100000100110011000010000010000010000010011  1100011101001111   01
//                               8   2  6   8    2  0    8   2  0   b    0  8   2   0   8   2    7   6  4    7  a
//00000000000000000000000000000100000100110100000100000100000100000101100001000001000001000001001110110010001111010
//                        0000010000010011010000010000010000010000010110000100000100000100000100111011001000111101
//  0  0    0  0   0    0   0  4   1   3   4   1   0   4   1   0    5  8   4   1   0   4   1   3   b   2   3   d
//0000000000000000000000000000010000010011010000010000010000010000010110000100000100000100000100111011001000111101

//00000100000100010100000100000100000100110011000010000010000010000010011  1000011011101111
//
//00000000000001010000000000000001001100110000100000000000000000111000011011101111
//