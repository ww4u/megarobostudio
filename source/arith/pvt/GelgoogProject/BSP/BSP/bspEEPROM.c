


#include "bspEEPROM.h"
#include "bspTimer.h"
/*********************************************************************************************
函 数 名: AT24CXX_ReadBytes;
实现功能: 从AT24CXX中读取N个数据
输入参数:
输出参数: 无
返 回 值: 无
说    明:此函数只提供原始的读取,没有限定页地址,仅内部使用
*********************************************************************************************/
static void AT24CXX_ReadBytes(u16 ReadAddr,u8* data,u32 len)
{
    int i = 0;
    bspI2C_Start(); //起始位
    //发送写命令
    bspI2C_SendByte(0xA0);
    bspI2C_WaitAck();
    //发送读数据地址
    bspI2C_SendByte(ReadAddr>>8);//高地址
    bspI2C_WaitAck();
    bspI2C_SendByte(ReadAddr&0xFF);//低地址
    bspI2C_WaitAck();
    //启动读取
    bspI2C_Start();
    bspI2C_SendByte(0xA1);           //进入接收模式
    bspI2C_WaitAck();
    for(i=0;i<len;i++)
    {
      data[i]=bspI2C_ReadByte(((i+1) == len)?0:1);//如果是顺序读的话,要发ACK,因为EEPROM要根据ACK增加地址
    }
    bspI2C_Stop();//停止位
}

/*********************************************************************************************
函 数 名: AT24CXX_WriteBytes;
实现功能: 向AT24CXX中写入N个数据,page write
输入参数:
输出参数: 无
返 回 值: 无
说    明:此函数只提供原始的写入,没有限定页地址,仅内部使用
*********************************************************************************************/
static void AT24CXX_WriteBytes(u16 WriteAddr,u8* Data,u32 len)
{
    int i = 0;
    bspI2C_Start();//启始位
    bspI2C_SendByte(0xA0);      //发送写命令
    bspI2C_WaitAck();
    bspI2C_SendByte(WriteAddr>>8);//发送高地址

    bspI2C_WaitAck();
    bspI2C_SendByte(WriteAddr&0xff);   //发送低地址
    bspI2C_WaitAck();
    for(i = 0;i < len;i++)
    {
        bspI2C_SendByte(Data[i]);     //发送字节
        bspI2C_WaitAck();
    }
    bspI2C_Stop();//停止位
    bspDelayMs(10);//在页写完时,需要一个小延时
}
/*********************************************************************************************
函 数 名: drvAT24CxxWrite
实现功能: 向EEPROM中写入数据
输入参数: u16 u16addr: 目的地址         地址单位为字节 范围为 0--16K
         u8 * src:    数据源地址指针
         u32 len:       要写入的数据长度  以字节为单位

输出参数: 无;
返 回 值: 0     发送成功,
          其他  发送失败;
说    明: EEPROM在连续写的时候可能会遇到分页的问题--在一页内地址可以自增,增加到页末尾时继续自加会到页第一个地址处,因此需要多页边界进行处理
*********************************************************************************************/
int bspAT24CxxWrite(u16 u16addr,u8 * src, u32 len)
{
    u8 u8PageCanWrite = 0;//在某一页上,可以写多少个字节
    u32 u32left = len;//要写入的数据,还剩余多少个字节
    //地址或者数据越界
    if((src == 0) || (u16addr + len > EEPROM_SIZE_MAX))
    {
        return -1;
    }
    u8PageCanWrite = EEPROM_PAGE_SIZE - (u8)(u16addr&(EEPROM_PAGE_SIZE-1));//第一页中还能写多少字节
    if(u32left <= u8PageCanWrite)//当前面即可写完,不需要处理跨页操作
    {
        u8PageCanWrite = u32left;
        //AT24CXX_WriteBytes(u16addr,src,u8PageCanWrite);
        //return 0;
    }
    /*对于需要跨页写的操作
     * 先写第一页,第一页比较特殊,需要单独处理
     * */
    AT24CXX_WriteBytes(u16addr,src,u8PageCanWrite);
    u16addr += u8PageCanWrite; //地址增加
    src += u8PageCanWrite;     //数据指针移动
    u32left -= u8PageCanWrite;//数据剩余字节

    while(u32left > 0)
    {
        if(u32left > EEPROM_PAGE_SIZE)
        {
            u8PageCanWrite = EEPROM_PAGE_SIZE;
        }
        else
        {
            u8PageCanWrite = u32left;
        }
        AT24CXX_WriteBytes(u16addr,src,u8PageCanWrite);
        u16addr += u8PageCanWrite; //地址增加
        src += u8PageCanWrite;     //数据指针移动
        u32left -= u8PageCanWrite; //数据剩余字节
    }
    return 0;
}
/*********************************************************************************************
函 数 名: drvAT24CxxRead
实现功能: 从EEPROM中读取数据
输入参数: u16 u16addr: 目的地址         地址单位为字节 范围为 0--16K
         u8 * src:    数据源地址指针
         u32 len:       要读取的数据长度  以字节为单位

输出参数: 无;
返 回 值: 0     发送成功,
          其他  发送失败;
说    明: EEPROM在连续写的时候可能会遇到分页的问题--在一页内地址可以自增,增加到页末尾时继续自加会到页第一个地址处,因此需要多页边界进行处理
*********************************************************************************************/
int bspAT24CxxRead(u16 u16addr,u8 * src, u32 len)
{
    u8 u8PageCanRead = 0;//在某一页上,可以读多少个字节
    u32 u32left = len;//还有多少个字节数据要取
    //地址或者数据越界
    if((src == 0) || (u16addr + len > EEPROM_SIZE_MAX))
    {
        return -1;
    }
    u8PageCanRead = EEPROM_PAGE_SIZE - (u8)(u16addr&(EEPROM_PAGE_SIZE-1));//第一页中还能写多少字节
    if(len <= u8PageCanRead)//当前面即可读完,不需要处理跨页操作
    {
        u8PageCanRead = len;
    }
    /*对于需要跨页读取的操作
     * 先从第一页中读取,第一页比较特殊,需要单独处理
     * */
    AT24CXX_ReadBytes(u16addr,src,u8PageCanRead);
    u16addr += u8PageCanRead; //地址增加
    src += u8PageCanRead;     //数据指针移动
    u32left -= u8PageCanRead;//数据剩余字节

    while(u32left > 0)
    {
        if(u32left > EEPROM_PAGE_SIZE)
        {
            u8PageCanRead = EEPROM_PAGE_SIZE;
        }
        else
        {
            u8PageCanRead = u32left;
        }
        AT24CXX_ReadBytes(u16addr,src,u8PageCanRead);
        u16addr += u8PageCanRead; //地址增加
        src += u8PageCanRead;     //数据指针移动
        u32left -= u8PageCanRead; //数据剩余字节
    }
    return 0;
}
extern void bspEEPROMTest();
/*********************************************************************************************
函 数 名: bspEepromInit
实现功能: EEPROM初始化
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
void bspEepromInit()
{
    bspI2C_init();
    //bspEEPROMTest();
}