/**********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspFlash.c
��������:  ϵͳflash��д
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  xyzheng
�������:  2017.08.08;
��ʷ�汾:  ��;
**********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bspFlash.h"

/*
˵����
    ��ΪF407�ڲ�Flash������û��ҳ��д����дֻ��������Ϊ��λ����������̫�󣬲��ܰ���ǰ�Ļض�����д�ķ�ʽ����д������
    ���ԣ�F407���ڲ�Flash��д��ǰ����Ҫ�Ȳ�������������д�����ݣ�������
*/

/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/



/************************************�ֲ����������Ͷ���**************************************/

/******************************************�ֲ�����*******************************************/

/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ����bspFlashWrite
ʵ�ֹ��ܣ�flashд  
���������  �ޣ�
���������  �ޣ�   
��������ֵ��
*********************************************************************************************/
int bspFlashWrite(__IO u32 address, u8* Data ,u32 u32Bytes)
{
    u32 i = 0;
    if (u32Bytes == 0 || Data == NULL)
    {
        return 0;
    }
    __set_PRIMASK(1);       /* ���ж� */
    FLASH_Unlock(); 
    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    for (i = 0; i < u32Bytes ; i++)
    {
        /* ���ֽ�ģʽ��̣�Ϊ���Ч�ʣ����԰��ֱ�̣�һ��д��4�ֽڣ� */
        FLASH_ProgramByte(address++, Data[i]);
    }
    /* Flash ��������ֹдFlash���ƼĴ��� */
    FLASH_Lock();
    __set_PRIMASK(0);       /* ���ж� */
    return (0);
}
/*********************************************************************************************
�� �� ���� bspFlashRead 
ʵ�ֹ��ܣ�  
���������  �ޣ�
���������  �ޣ�   
��������ֵ���ޣ�  
*********************************************************************************************/
s32 bspFlashRead(u32 iAddress, u8 *buf, u32 u32Bytes) 
{
    int i = 0;
    while(i < u32Bytes ) 
    {
       *(buf + i) = *(__IO u8*) iAddress++;
       i++;
    }
    return i;
}
/**********************************************************************************************************
�� �� ��: GetSector
����˵��: ���ݵ�ַ���������׵�ַ
��    ��: Address  ָ����ַ
�� �� ֵ: �����׵�ַ
**********************************************************************************************************/
static u32 GetSector(u32 addr)
{
    if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;  
    else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;  
    else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;  
    else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;  
    else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;  
    else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;  
    else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;  
    else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;  
    else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;  
    else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;  
    else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10;   
    return FLASH_Sector_11;      
}
/*********************************************************************************************
�� �� ���� bspFlashSectorErase 
ʵ�ֹ��ܣ���������(�����ʹ��)  
���������start��ʼ��ַ��end:������ַ
���������  ��   
��������ֵ����  
*********************************************************************************************/
int bspFlashSectorErase(u32 start,u32 end)
{
    u32 startSector = GetSector(start);
    u32 endSector = GetSector(end);
    int i = 0;
    __set_PRIMASK(1);       /* ���ж� */
    FLASH_Unlock(); 
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    for(i = startSector; i <= endSector; i+= 8)//ת��ΪSector���
    {
        if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
        {
            return (1);
        }
    }
    /* Flash ��������ֹдFlash���ƼĴ��� */
    FLASH_Lock();
    __set_PRIMASK(0);       /* ���ж� */
    return (0);
}

/*
*********************************************************************************************************
*   �� �� ��: FLASH_If_GetWriteProtectionStatus
*   ����˵��: �û�flash����д����״̬
*   ��    ��: ��
*   �� �� ֵ: 0 �û�flash����û��д����
*             1 �û�flash��ĳЩɽ������д����
*********************************************************************************************************
*/
int bspFlashGetWriteProtectionStatus(u32 addr)
{
    uint32_t UserStartSector = FLASH_Sector_1;

    /* �û�flash�����ڵ����� */
    UserStartSector = GetSector(addr);

    /* ����û�flash���е�ĳЩ�����Ƿ���д���� */
    if ((FLASH_OB_GetWRP() >> (UserStartSector/8)) == (0xFFF >> (UserStartSector/8)))
    { 
        /* û��д���� */
        return 1;
    }
    else
    { 
        /* ĳЩ������д���� */
        return 0;
    }
}
/**********************************************************************************************************
*   �� �� ��: FLASH_If_DisableWriteProtection
*   ����˵��: �⿪д����
*   ��    ��: ��
*   �� �� ֵ: 0 �Ᵽ���ɹ�
*             -1 �Ᵽ��ʧ��
**********************************************************************************************************/
int bspFlashDisableWriteProtection(u32 addr)
{
    __IO u32 UserStartSector = FLASH_Sector_1, UserWrpSectors = OB_WRP_Sector_1;

    /* �û�flash�����ڵ����� */
    UserStartSector = GetSector(addr);

    UserWrpSectors = 0xFFF-((1 << (UserStartSector/8))-1);

    /* ���� Option Bytes */
    FLASH_OB_Unlock();

    /* �����û������Ᵽ�� */
    FLASH_OB_WRPConfig(UserWrpSectors, DISABLE);

    /* ���� Option Bytes ���. */  
    if (FLASH_OB_Launch() != FLASH_COMPLETE)
    {
        /* �Ᵽ��ʧ�� */
        return (-1);
    }

    /* �Ᵽ���ɹ� */
    return (0);
}
/*******************************************�ļ�β********************************************/




#if 0  //flash test

void bspFlashTest()
{
    bspFlashSectorErase(0x08030000,0x0803FFFF);

    u8 data[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                 0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
                 0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
                 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38};

    
    u8 read[100] = {0};

    bspFlashWrite(0x08030000,data,32);

    bspFlashRead(0x08030000,read,32);

}


#endif


