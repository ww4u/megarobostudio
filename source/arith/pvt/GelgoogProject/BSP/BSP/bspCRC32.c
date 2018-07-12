/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servCRC32.c;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  xyzheng
�������:  2017.03.26;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "bspCRC32.h"

void bspCRC32Init()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
}


u32 bspCRC32(u8 * pBuffer, u32 NumOfByte)
{
  u32 last_data;
  u32 NumOfDWord = NumOfByte>>2;
  u32 NumOfTailByte =  NumOfByte & 3 ;
  u32 crc32 = 0;
  CRC_ResetDR();
  crc32 = CRC_CalcBlockCRC( (u32  *)pBuffer, NumOfDWord );

  switch ( NumOfTailByte )
  {
    case 0:
      return crc32;
    case 1:
      last_data = pBuffer[NumOfByte-1] << 24;
      break;
    case 2:
      last_data = *( (u16 *)(&pBuffer[NumOfByte-2]) );
      last_data <<= 16;
      break;
    case 3:
      last_data = *( (uint16_t *)(&pBuffer[NumOfByte-3]) );
      last_data <<= 8;
      last_data += pBuffer[NumOfByte-1]<<24;
      break;
  }
  return CRC_CalcCRC( last_data );
}