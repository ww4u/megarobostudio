/**********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspFlash.c
功能描述:  系统flash读写
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  xyzheng
完成日期:  2017.08.08;
历史版本:  无;
**********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bspFlash.h"

/*
说明：
    因为F407内部Flash操作中没有页擦写，擦写只能以扇区为单位。但因扇区太大，不能按以前的回读完再写的方式进行写操作，
    所以，F407的内部Flash中写入前，需要先擦除整个扇区再写入数据！！！！
*/

/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/************************************局部常量和类型定义**************************************/

/******************************************局部变量*******************************************/

/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名：bspFlashWrite
实现功能：flash写  
输入参数：  无；
输出参数：  无；   
函数返回值：
*********************************************************************************************/
int bspFlashWrite(__IO u32 address, u8* Data ,u32 u32Bytes)
{
    u32 i = 0;
    if (u32Bytes == 0 || Data == NULL)
    {
        return 0;
    }
    __set_PRIMASK(1);       /* 关中断 */
    FLASH_Unlock(); 
    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    for (i = 0; i < u32Bytes ; i++)
    {
        /* 按字节模式编程（为提高效率，可以按字编程，一次写入4字节） */
        FLASH_ProgramByte(address++, Data[i]);
    }
    /* Flash 加锁，禁止写Flash控制寄存器 */
    FLASH_Lock();
    __set_PRIMASK(0);       /* 开中断 */
    return (0);
}
/*********************************************************************************************
函 数 名： bspFlashRead 
实现功能：  
输入参数：  无；
输出参数：  无；   
函数返回值：无；  
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
函 数 名: GetSector
功能说明: 根据地址计算扇区首地址
形    参: Address  指定地址
返 回 值: 扇区首地址
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
函 数 名： bspFlashSectorErase 
实现功能：擦除扇区(请谨慎使用)  
输入参数：start起始地址，end:结束地址
输出参数：  无   
函数返回值：无  
*********************************************************************************************/
int bspFlashSectorErase(u32 start,u32 end)
{
    u32 startSector = GetSector(start);
    u32 endSector = GetSector(end);
    int i = 0;
    __set_PRIMASK(1);       /* 关中断 */
    FLASH_Unlock(); 
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    for(i = startSector; i <= endSector; i+= 8)//转化为Sector编号
    {
        if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
        {
            return (1);
        }
    }
    /* Flash 加锁，禁止写Flash控制寄存器 */
    FLASH_Lock();
    __set_PRIMASK(0);       /* 开中断 */
    return (0);
}

/*
*********************************************************************************************************
*   函 数 名: FLASH_If_GetWriteProtectionStatus
*   功能说明: 用户flash区的写保护状态
*   形    参: 无
*   返 回 值: 0 用户flash区中没有写保护
*             1 用户flash区某些山扇区有写保护
*********************************************************************************************************
*/
int bspFlashGetWriteProtectionStatus(u32 addr)
{
    uint32_t UserStartSector = FLASH_Sector_1;

    /* 用户flash区所在的扇区 */
    UserStartSector = GetSector(addr);

    /* 检测用户flash区中的某些扇区是否有写保护 */
    if ((FLASH_OB_GetWRP() >> (UserStartSector/8)) == (0xFFF >> (UserStartSector/8)))
    { 
        /* 没有写保护 */
        return 1;
    }
    else
    { 
        /* 某些扇区有写保护 */
        return 0;
    }
}
/**********************************************************************************************************
*   函 数 名: FLASH_If_DisableWriteProtection
*   功能说明: 解开写保护
*   形    参: 无
*   返 回 值: 0 解保护成功
*             -1 解保护失败
**********************************************************************************************************/
int bspFlashDisableWriteProtection(u32 addr)
{
    __IO u32 UserStartSector = FLASH_Sector_1, UserWrpSectors = OB_WRP_Sector_1;

    /* 用户flash区所在的扇区 */
    UserStartSector = GetSector(addr);

    UserWrpSectors = 0xFFF-((1 << (UserStartSector/8))-1);

    /* 解锁 Option Bytes */
    FLASH_OB_Unlock();

    /* 所有用户扇区解保护 */
    FLASH_OB_WRPConfig(UserWrpSectors, DISABLE);

    /* 进行 Option Bytes 编程. */  
    if (FLASH_OB_Launch() != FLASH_COMPLETE)
    {
        /* 解保护失败 */
        return (-1);
    }

    /* 解保护成功 */
    return (0);
}
/*******************************************文件尾********************************************/




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


