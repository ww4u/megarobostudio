/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdUpdate.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
      升级流程：
     boot , app 或 fpga升级
1. 上位机发送start命令，下位机根据命令参数，判断是否要重启进入boot,并应答。
2. 上位机发送地址设置命令，下位机判断地址合法性，并保存地址，并应答。
3. 上们机发送开始升级(start)命令,下位机分配缓存,做接收数据前的准备，,并应答.
4. 上位机发送擦除（erase）命令，擦除相应的flash,并应答。
5. 上位机循环发送升级数据，下位机保存数据，并应答。
6. 上位机发送升级数据结束命令，下位机结束本次升级，并应答。
7. 上位机发送跳转命令，从boot跳转入app. 不应答。
*********************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "cmdUpdate.h"
#include "cmdMainParse.h"
#include "servSystemPara.h"
#include "servSoftTimer.h"
#include "bspFlash.h"
#include "bspTimer.h"
#include "comMemoryMap.h"



/****************************************外部变量声明*****************************************/
extern SystemStateStruct g_systemState;
extern BootInfoStruct    g_bootInfo;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pUpdateCmdFunc[UPDATECMD_RESERVE];

u8 * g_update_buf = NULL;
u32  g_flash_position = 0;
u16  g_buf_offset = 0;



/******************************************函数实现*******************************************/
//向Flash中写入数据
static int updateWriteFlash(u8* data,u16 len)
{
    u32 u32left = 0;
    if(g_update_buf == NULL)
    {
      return -2;
    }
    if(g_buf_offset + len >= FLASH_PAGE_SIZE)//超出缓冲区，则写入flash
    {
        u32left = FLASH_PAGE_SIZE - g_buf_offset;
        memcpy(&g_update_buf[g_buf_offset],data,u32left);
        //只有满一页才会写入
        bspFlashWrite(g_flash_position,g_update_buf,FLASH_PAGE_SIZE);//写入Flash
        bspDelayMs(20);
       
        memcpy(&g_update_buf[0],&data[u32left],len - u32left);//将data中剩余的数据缓存下来
        g_buf_offset = len - u32left;//缓冲区的数据长度
        g_flash_position += FLASH_PAGE_SIZE;//移动位置
    }
    else //否则，暂且缓存
    {
        memcpy(&g_update_buf[g_buf_offset],data,len);
        g_buf_offset += len;
    }
    return 0;
}
//将缓冲区的中数据,全部写入Flash
static void updateFlushData()
{
    if(g_buf_offset == 0 )
    {
      return;
    }
    if((g_buf_offset&0x01))//非双字节对齐，补0
    {
      g_update_buf[g_buf_offset] = 0;
      g_buf_offset += 1;
    }
    bspFlashWrite(g_flash_position,g_update_buf,g_buf_offset);

    g_flash_position += g_buf_offset;//移动位置
    bspDelayMs(20);//这个延时是必要的,给Flash写的时间
}


/*********************************************************************************************
函 数 名: cmdUpdateStart;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateStart(u8 cmdDataLen, u8 *pCmdData)
{   
    if(pCmdData[0] != UPDATE_TYPE_BOOT)
    {
        g_bootInfo.u8BootFlag = 0x01;//置位升级标志
        servBootInfoWrite(&g_bootInfo);//保存至EEPROM中
        //别的暂不处理
        bspSoftReset();
    }
    else//如果是升级boot，则不跳转
    {
        g_flash_position = 0x08000000;
        g_buf_offset = 0;
        if(g_update_buf == NULL)
        {
            g_update_buf = (u8*)malloc(2048);
            if(g_update_buf == NULL)
            {
                return;
            }
        }
    }
    cmdFrameSend(CMD_UPDATE, UPDATECMD_START, 0, NULL);

}


/*********************************************************************************************
函 数 名: cmdUpdateErase
实现功能: 擦除flash
输入参数: 总共5个字节,第一个字节表示类型,接下来的4个字节是擦除的长度
输出参数: 无;
返 回 值: 无;
说    明: 
*********************************************************************************************/
void cmdUpdateErase(u8 cmdDataLen, u8 *pCmdData)
{    
    u8  type = 0;
    
    
    type = pCmdData[0];

    if(type != UPDATE_TYPE_BOOT)//在APP中,暂时只能升级boot
    {
        return;
    }
    //memcpy(&len,&pCmdData[1],4);
#ifdef PROJECT_GELGOOG
    bspFlashSectorErase(FLASH_BOOT_CODE_ADDR, 0x08007FFF);
#endif
    cmdFrameSend(CMD_UPDATE, UPDATECMD_ERASE, 0, NULL);
}

/*********************************************************************************************
函 数 名: cmdUpdateData;
实现功能: 接收上位机发送的升级数据,在合适的时候写入flash
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateData(u8 cmdDataLen, u8 *pCmdData)
{    
    if(updateWriteFlash(pCmdData,cmdDataLen) != 0)
    {
        return;
    }
    cmdFrameSend(CMD_UPDATE, UPDATECMD_DATA, 0, NULL);
}


/*********************************************************************************************
函 数 名: cmdUpdateJump;
实现功能: 跳转,实际是重启系统 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateJump(u8 cmdDataLen, u8 *pCmdData)
{    
    //bspSoftReset();//重启
}


/*********************************************************************************************
函 数 名: cmdUpdateAppAddrSet;
实现功能: 设置App在flash中的位置
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 暂是没有参数验证
*********************************************************************************************/
void cmdUpdateAppAddrSet(u8 cmdDataLen, u8 *pCmdData)
{    
    if(cmdDataLen != 4 )
    {
        return;
    }
    memcpy((void*)&g_bootInfo.u32AppBaseAddr,pCmdData,4);
    servBootInfoWrite(&g_bootInfo);
    cmdFrameSend(CMD_UPDATE, UPDATECMD_APPADDR, 0, NULL);
}
/*********************************************************************************************
函 数 名: cmdUpdateAppAddrQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateAppAddrQuery(u8 cmdDataLen, u8 *pCmdData)
{    
    u8 dataLen;
    u8 *pData;

    dataLen = sizeof(g_bootInfo.u32AppBaseAddr);
    pData = (u8 *)&g_bootInfo.u32AppBaseAddr;
    cmdFrameSend(CMD_UPDATE, UPDATECMD_APPADDRQ, dataLen, pData);
}

/*********************************************************************************************
函 数 名: cmdUpdateEnd;
实现功能: 结束升级,将缓冲区中的数据写入flash
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateEnd(u8 cmdDataLen, u8 *pCmdData)
{    
    updateFlushData();
    if(g_update_buf)
    {
        free(g_update_buf);
    }
    cmdFrameSend(CMD_UPDATE, UPDATECMD_END, 0, NULL);
}


/*********************************************************************************************
函 数 名: cmdUpdateFpgaAddrSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateFpgaAddrSet(u8 cmdDataLen, u8 *pCmdData)
{    
    if(cmdDataLen != 4 )
    {
        return;
    }
    memcpy((void*)&g_bootInfo.u32FpgaBaseAddr,pCmdData,4);
    servBootInfoWrite(&g_bootInfo);
    cmdFrameSend(CMD_UPDATE, UPDATECMD_FPGAADDR, 0, NULL);
    
}

/*********************************************************************************************
函 数 名: cmdUpdateFpgaAddrQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateFpgaAddrQuery(u8 cmdDataLen, u8 *pCmdData)
{    
    u8 dataLen;
    u8 *pData;

    dataLen = sizeof(g_bootInfo.u32FpgaBaseAddr);
    pData = (u8 *)&g_bootInfo.u32FpgaBaseAddr;
    cmdFrameSend(CMD_UPDATE, UPDATECMD_FPGAADDRQ, dataLen, pData);
}
/*********************************************************************************************
函 数 名: cmdUpdateCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateCmdInit(void)
{
    memset(pUpdateCmdFunc, 0, sizeof(pUpdateCmdFunc));

    pUpdateCmdFunc[UPDATECMD_START]       = cmdUpdateStart;
    pUpdateCmdFunc[UPDATECMD_ERASE]       = cmdUpdateErase;
    pUpdateCmdFunc[UPDATECMD_DATA]        = cmdUpdateData;
    pUpdateCmdFunc[UPDATECMD_END]         = cmdUpdateEnd;
    pUpdateCmdFunc[UPDATECMD_JUMP]        = cmdUpdateJump;
    pUpdateCmdFunc[UPDATECMD_APPADDR]     = cmdUpdateAppAddrSet;
    pUpdateCmdFunc[UPDATECMD_FPGAADDR]    = cmdUpdateFpgaAddrSet;
    pUpdateCmdFunc[UPDATECMD_APPADDRQ]     = cmdUpdateAppAddrQuery;
    pUpdateCmdFunc[UPDATECMD_FPGAADDRQ]    = cmdUpdateFpgaAddrQuery;
}

            
/*********************************************************************************************
函 数 名: cmdUpdateCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUpdateCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < UPDATECMD_RESERVE) && (pUpdateCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pUpdateCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************文件尾********************************************/
