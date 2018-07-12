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
#include "servCanBuildFrame.h"
#include "bspSdio.h"
#include "servFPGA.h"
#include "comCmd.h"

BitMap  bitMap;



/*********************************************************************************************
函 数 名: servCanBuildExtFrameInit;
实现功能: 扩展帧初始化
输入参数: RTR:是否是远程帧
输出参数: 无
返 回 值: 无
说    明:
*********************************************************************************************/
static void servCanBuildExtFrameInit(u8 RTR)
{
   int i = 0;
   
   for(i = 0;i < MAP_SHORT_LEN;i++)
   {
        bitMap.map[i] = 0x0000;
   }
   //sof 显性
   mapClearBit(&bitMap,BIT_SOF);
  
   //IDE
   mapSetBit(&bitMap,BIT_EXT_IDE);//扩展帧,隐性
   //SRR
   mapSetBit(&bitMap,BIT_EXT_SRR);//隐性
   
   //数据帧,远程帧
   if(RTR)
   {
      mapSetBit(&bitMap,BIT_EXT_RTR);//远程帧,隐性 
   }
   else
   {
      mapClearBit(&bitMap,BIT_EXT_RTR);//数据帧,显性
   }
   //r1,r0 保留,总是用显性位填充
   mapClearBit(&bitMap,BIT_EXT_R0);
   mapClearBit(&bitMap,BIT_EXT_R1);
   //crc定界符,隐性 
   mapSetBit(&bitMap,BIT_EXT_CRC_DEL);
   //ACK及ACK定界符
   mapSetBit(&bitMap,BIT_EXT_ACK);
   mapSetBit(&bitMap,BIT_EXT_ACK_DEL);
   //EOF
   for(i = 0;i < 7;i++)
   {
       mapSetBit(&bitMap,BIT_EXT_EOF + i);
   }
}

/*********************************************************************************************
函 数 名: servCanBuildStdFrameInit
实现功能: 标准帧初始化
输入参数: RTR:是否是远程帧
输出参数: 无
返 回 值: 无
说    明:
*********************************************************************************************/
static void servCanBuildStdFrameInit(u8 RTR)
{
   int i = 0;
   for(i = 0;i < MAP_SHORT_LEN;i++)
   {
        bitMap.map[i] = 0x0000;
   }
   //SOF 显性
   mapClearBit(&bitMap,BIT_SOF);
   
   //IDE
   mapClearBit(&bitMap,BIT_STD_IDE);//标准帧,显性
  
   //数据帧,远程帧
   if(RTR)
   {
      mapSetBit(&bitMap,BIT_STD_RTR);//远程帧,隐性
   }
   else
   {
      mapClearBit(&bitMap,BIT_STD_RTR);//数据帧,显性
   }
   //r0 保留,总是用显性位填充
   mapClearBit(&bitMap,BIT_STD_R0);
   //crc定界符,隐性
   mapSetBit(&bitMap,BIT_STD_CRC_DEL);
   //ACK及ACK定界符
   mapSetBit(&bitMap,BIT_STD_ACK);
   mapSetBit(&bitMap,BIT_STD_ACK_DEL);
   //EOF
   for(i = 0;i < 7;i++)
   {
       mapSetBit(&bitMap,BIT_STD_EOF + i);
   }
}
/*********************************************************************************************
函 数 名: servMakeCRC15
实现功能: 计算帧的CRC
输入参数: 
输出参数: 无
返 回 值: 无
说    明:
    说明书中给出了伪代码:
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
    u16 crc15 = 0,bit = 0,next;    //crc计算
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
函 数 名: servCanBuileFrame
实现功能: 构建CAN帧
输入参数: 
输出参数: pBitMap:CAN帧数据
返 回 值: 返回帧长度(total)
说    明: pBitMap的空间固定为112bit(7*16bit)
*********************************************************************************************/
static int servCanBuileFrame(u32 id,u8 type,u8 rtr,u8 * data,u8 datalen,BitMap * pBitMap)
{
    int i = 0,j = 0,k1 = 0,k0 = 0;
    u16 crc15 = 0;
    int frame_len = 0;
    u8 bit = 0;
    
    if(type == CAN_ID_STD)//标准帧
    {
        servCanBuildStdFrameInit(rtr);
        frame_len = GET_STD_CRC_LEN(datalen)+ 16;//加上CRC和CRC定界符长度
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
        for(i = 0;i < 4;i++)//设置DLC
        {
            if((datalen>>(3 - i))&0x01)//先放高位(MSB)
            {
                mapSetBit(&bitMap,BIT_STD_DLC + i);//隐性
            }
            else
            {
                mapClearBit(&bitMap,BIT_STD_DLC + i);//显性
            }
        }
        for(i = 0;i < datalen;i++)//设置数据
        {
            for(j = 0;j < 8;j++)
            {
                if((data[i]>>(7-j))&0x01)//先放高位(MSB)
                {
                    mapSetBit(&bitMap,BIT_STD_DATA + j + i * 8);//隐性
                }
                else
                {
                    mapClearBit(&bitMap,BIT_STD_DATA + j + i * 8);//显性
                }
            }
        }
        //计算CRC 并填充CRC 
        crc15 = servMakeCRC15(&bitMap,GET_STD_CRC_LEN(datalen));
        for(i = 0;i< 15;i++)
        {
            if((crc15>>(14 - i))&0x01)//先放高位(MSB)
            {
                mapSetBit(&bitMap,BIT_STD_CRC + i );//隐性
            }
            else
            {
                mapClearBit(&bitMap,BIT_STD_CRC + i);//显性
            }
        } 
    }
    else
    {
        servCanBuildExtFrameInit(rtr);//初始化扩展帧,填充必要的位
        frame_len = GET_EXT_CRC_LEN(datalen) + 16; //加上CRC和CRC定界符长度
        //设置ID
        for(i = 0;i < 10;i++)
        {
            if(((id >> (28-i))&0x01))//先放高位(MSB)
            {
                mapSetBit(&bitMap,BIT_EXT_ID_11 + i);
            }
            else
            {
                mapClearBit(&bitMap,BIT_EXT_ID_11 + i);
            }
        }
        for(i = 0;i < 18;i++)//先放高位(MSB)
        {
            if(((id >> (17 - i))&0x01))
            {
                mapSetBit(&bitMap,BIT_EXT_ID_18 + i);
            }
            else
            {
                mapClearBit(&bitMap,BIT_EXT_ID_18 + i);//显性
            }
        }

        for(i = 0;i < 4;i++)//设置DLC
        {
            if((datalen>>(3 - i))&0x01)//先放高位(MSB)
            {
                mapSetBit(&bitMap,BIT_EXT_DLC + i);//隐性
            }
            else
            {
                mapClearBit(&bitMap,BIT_EXT_DLC + i);//显性
            }
        }
        for(i = 0;i < datalen;i++)//设置数据
        {
            for(j = 0;j < 8;j++)
            {
                if((data[i]>>(7-j))&0x01)//先放高位(MSB)
                {
                    mapSetBit(&bitMap,BIT_EXT_DATA + j + i * 8);//隐性
                }
                else
                {
                    mapClearBit(&bitMap,BIT_EXT_DATA + j + i * 8);//显性
                }
            }
        }
        //计算CRC 并填充CRC 
        crc15 = servMakeCRC15(&bitMap,GET_EXT_CRC_LEN(datalen));
        for(i = 0;i< 15;i++)
        {
            if((crc15>>(14 - i))&0x01)//先放高位(MSB)
            {
                mapSetBit(&bitMap,BIT_EXT_CRC + i );//隐性
            }
            else
            {
                mapClearBit(&bitMap,BIT_EXT_CRC + i);//显性
            }
        } 
    }
    
    /**************************** 补 位 **************************************/
    for(i = 0;i < MAP_SHORT_LEN;i++)
    {
        pBitMap->map[i] = 0x0000;
    }
    for(i = 0,j = 0;i < frame_len;i++,j++)//去掉帧结束场和ACK场
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
             mapClearBit(pBitMap,j);//五个1后补零
             k1 = 0;
             k0 = 1; //参与下次补位
        }
        if(k0 == 5)
        {
             j++;
             mapSetBit(pBitMap,j);//五个零后补1
             k0 = 0;
             k1 = 1; //参与下次补位
        }
    }
    //填充112位
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
    return (j);//返回填充后帧长度(不含ACK和EOF)
}
/*********************************************************************************************
函 数 名: servCanBuileFrameSendFrame2Fpga
实现功能: 构建CAN帧,并发送给逻辑
输入参数: baudrate:can口波特率,单位bps
          id: 发送开始命令时,使用的CAN ID
          type: 扩展帧还是标准帧
          pData:CAN口发送的数据,包括命令和参数
          datalen:数据长度
输出参数: 无
返 回 值: 0:发送正确; 否则发送出错
说    明: 
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
    //发送帧长度与rate高8位
    senddata = (len&0xFF)<<8|((rate>>16)&0xFF);
    if(servFpgaRegisterWrite(0, CAN_RATE_H, senddata) != 0)
    {
        return -1;
    }
    //发送rate低16位
    senddata = rate&0xFFFF;
    if(servFpgaRegisterWrite(0, CAN_RATE_L, senddata) != 0)
    {
        return -2;
    }
    //发送帧数据(112位)
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
函 数 名: servCanBuildFrameRstFpgaCanRecieve
实现功能: 复位逻辑CAN接收模块
输入参数: 
输出参数: 无
返 回 值: 0:发送正确; -1:发送出错
说    明: 
*********************************************************************************************/
int servCanBuildFrameRstFpgaCanRecieve()
{
    return servFpgaRegisterWrite(0, CAN_RX_CTRL, 1);
}

/*********************************************************************************************
函 数 名: servCanBuildFrameSendMotionStartFrame2Fpga
实现功能: 构建MOTIONRUN帧,并发送给逻辑
输入参数: 
输出参数: 无
返 回 值: 0:发送正确; -1:发送出错
说    明: 
*********************************************************************************************/
int servCanBuildFrameSendMotionStartFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud)
{
    u8 cmd[] = {CMD_MOTION, MOTIONCMD_SWITCH, CH1, MTSWITCH_RUN}; //MOTION:SWITCH RUN

    return servCanBuileFrameSendFrame2Fpga(id,type,enBaud,cmd,3);
}
/*********************************************************************************************
函 数 名: servCanBuildFrameSendClockSyncStartFrame2Fpga
实现功能: 构建CLOCK:SYNC START帧,并发送给逻辑
输入参数: 
输出参数: 无
返 回 值: 0:发送正确; -1:发送出错
说    明: 
*********************************************************************************************/
int servCanBuildFrameSendClockSyncStartFrame2Fpga(u32 id,u8 type,CanBaudEnum enBaud)
{
    u8 cmd[] = {CMD_CLOCK, CLOCK_SYNCSTATE, CLOCKSYNC_START}; //CLOCK:SYNC START
    return servCanBuileFrameSendFrame2Fpga(id,type,enBaud,cmd,3);
}
/*********************************************************************************************
函 数 名: servCanBuildFrameSendClockSyncEndFrame2Fpga
实现功能: 构建CLOCK:SYNC END帧,并发送给逻辑
输入参数: 
输出参数: 无
返 回 值: 0:发送正确; -1:发送出错
说    明: 
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
       servSetFpgaMultiSyncFunc(MULTI_SYNC_RST);//清除上次计数
       servSetFpgaMultiSyncFunc(MULTI_SYNC_START);//设置计数使能,等待开始命令
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
       servSetFpgaMultiSyncFunc(MULTI_SYNC_END);//设置停止使能,等待停止命令
       servCanBuildFrameSendClockSyncEndFrame2Fpga(canId, canIntfc.idType, canIntfc.baud);
       //bspDelayUs(100);
       break;
     }
}










#if  0  //测试代码
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