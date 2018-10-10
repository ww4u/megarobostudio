/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdInterface.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include "cmdInterface.h"
#include "cmdMainParse.h"
#include "pvrfInterface.h"
#include "servSystemPara.h"
#include "servCanNetManage.h"
#include "servCommIntfc.h"
#include "servSoftTimer.h"



/****************************************外部变量声明*****************************************/
extern CommIntfcStruct   g_commIntfc;
extern SystemStateStruct g_systemState;
extern DeviceInfoStruct  g_deviceInfo;

extern SoftTimerStruct g_paraSaveTimer;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pLinkCmdFunc[LINKCMD_RESERVE];
SubCmdProFunc pUartIntfcCmdFunc[UARTCMD_RESERVE];
SubCmdProFunc pCanIntfcCmdFunc[CANCMD_RESERVE];

extern CanIntfcStruct  g_ciCanInterface;     
extern UartIntfcStruct g_ciUartInterface;



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdLinkIntfcSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdLinkIntfcSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 subType;
    LinkTypeEnum linkType = g_commIntfc.linkType;
    

    if (sizeof(LinkTypeEnum) == cmdDataLen)    //长度先要正确
    {
        subType = *pCmdData;    
        switch (subType)
        {
            case 0:
                g_commIntfc.linkType = LINK_NONE;//断掉连接
              break;
              
            case 1:
                g_commIntfc.linkType = LINK_CAN;
              break;
              
            case 2: 
                g_commIntfc.linkType = LINK_UART;
              break;

            default:
                g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = PARA_VERIFY_ERROR_TYPE;
              break;
        }

        if (linkType != g_commIntfc.linkType)
        {
            servStimerAdd(&g_paraSaveTimer);
        }
    }
}


/*********************************************************************************************
函 数 名: cmdLinkIntfcQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdLinkIntfcQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.linkType);
    pData = (u8 *)&g_commIntfc.linkType;
    cmdFrameSend(CMD_LINK, LINKCMD_INTFCQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdLinkDeviceInfoQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdLinkDeviceInfoQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;
    u8 data[6];
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_deviceInfo.mDevcModel) + sizeof(g_deviceInfo.sDevcModel) + sizeof(g_ciCanInterface.receiveId);
    data[0] = g_deviceInfo.mDevcModel;
    data[1] = g_deviceInfo.sDevcModel;
    pData = (u8 *)&g_ciCanInterface.receiveId;
    for (i = 0;i < sizeof(g_ciCanInterface.receiveId);i++)
    {
        data[2 + i] = *pData++;
    }
    
    cmdFrameSend(CMD_LINK, LINKCMD_DEVICEINFOQ, dataLen, data);
}


/*********************************************************************************************
函 数 名: cmdLinkCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdLinkCmdInit(void)
{
    memset(pLinkCmdFunc, 0, sizeof(pLinkCmdFunc));

    pLinkCmdFunc[LINKCMD_INTFC]  = cmdLinkIntfcSet;
    pLinkCmdFunc[LINKCMD_INTFCQ] = cmdLinkIntfcQuery;
    
    pLinkCmdFunc[LINKCMD_DEVICEINFOQ] = cmdLinkDeviceInfoQuery;
}


/*********************************************************************************************
函 数 名: cmdLinkCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdLinkCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < LINKCMD_RESERVE) && (pLinkCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pLinkCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


/*********************************************************************************************
函 数 名: cmdUartBaudSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartBaudSet(u8 cmdDataLen, u8 *pCmdData)
{
    UartBaudEnum baud;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfUartBaudVerify(cmdDataLen, pCmdData, (void *)&baud))
    {
        g_ciUartInterface.baud = baud;
    }
}


/*********************************************************************************************
函 数 名: cmdUartBaudQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartBaudQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.uartIntfc.baud);
    pData = (u8 *)&g_commIntfc.uartIntfc.baud;
    cmdFrameSend(CMD_UART, UARTCMD_BAUDQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdUartWordLenSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartWordLenSet(u8 cmdDataLen, u8 *pCmdData)
{
    WordLenEnum wordLen;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfUartWordLenVerify(cmdDataLen, pCmdData, (void *)&wordLen))
    {
        g_ciUartInterface.wordLen = wordLen;
    }
}


/*********************************************************************************************
函 数 名: cmdUartWordLenQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartWordLenQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.uartIntfc.wordLen);
    pData = (u8 *)&g_commIntfc.uartIntfc.wordLen;
    cmdFrameSend(CMD_UART, UARTCMD_WORDLENQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdUartFlowCtlSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartFlowCtlSet(u8 cmdDataLen, u8 *pCmdData)
{
    FlowCtlEnum flowCtl;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfUartFlowCtlVerify(cmdDataLen, pCmdData, (void *)&flowCtl))
    {
        g_ciUartInterface.flowCtl = flowCtl;
    }
}


/*********************************************************************************************
函 数 名: cmdUartFlowCtlQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartFlowCtlQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.uartIntfc.flowCtl);
    pData = (u8 *)&g_commIntfc.uartIntfc.flowCtl;
    cmdFrameSend(CMD_UART, UARTCMD_FLOWCTLQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdUartParitySet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartParitySet(u8 cmdDataLen, u8 *pCmdData)
{
    ParityEnum parity;

    
    //进行参数验证 
    if (PARA_VERIFY_NO_ERROR == pvrfUartParityVerify(cmdDataLen, pCmdData, (void *)&parity))
    {
        g_ciUartInterface.parity = parity;
    }
}


/*********************************************************************************************
函 数 名: cmdUartParityQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartParityQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.uartIntfc.parity);
    pData = (u8 *)&g_commIntfc.uartIntfc.parity;
    cmdFrameSend(CMD_UART, UARTCMD_PARITYQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdUartStopBitSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartStopBitSet(u8 cmdDataLen, u8 *pCmdData)
{
    StopBitEnum stopBit;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfUartStopBitVerify(cmdDataLen, pCmdData, (void *)&stopBit))
    {
        g_ciUartInterface.stopBit = stopBit;
    }
}


/*********************************************************************************************
函 数 名: cmdUartStopBitQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartStopBitQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.uartIntfc.stopBit);
    pData = (u8 *)&g_commIntfc.uartIntfc.stopBit;
    cmdFrameSend(CMD_UART, UARTCMD_STOPBITQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdUartApplyPara;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartApplyPara(u8 cmdDataLen, u8 *pCmdData)
{    
    memcpy(&g_commIntfc.uartIntfc, &g_ciUartInterface, sizeof(UartIntfcStruct));

    servCiUartConfig(g_commIntfc.uartIntfc);

    /*存储到EEPROM中*/
    servStimerAdd(&g_paraSaveTimer);
}


/*********************************************************************************************
函 数 名: cmdUartIntfcCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartIntfcCmdInit(void)
{
    memset(pUartIntfcCmdFunc, 0, sizeof(pUartIntfcCmdFunc));

    pUartIntfcCmdFunc[UARTCMD_BAUD]      = cmdUartBaudSet;
    pUartIntfcCmdFunc[UARTCMD_BAUDQ]     = cmdUartBaudQuery;
    pUartIntfcCmdFunc[UARTCMD_WORDLEN]   = cmdUartWordLenSet;
    pUartIntfcCmdFunc[UARTCMD_WORDLENQ]  = cmdUartWordLenQuery;
    pUartIntfcCmdFunc[UARTCMD_FLOWCTL]   = cmdUartFlowCtlSet;
    pUartIntfcCmdFunc[UARTCMD_FLOWCTLQ]  = cmdUartFlowCtlQuery;
    pUartIntfcCmdFunc[UARTCMD_PARITY]    = cmdUartParitySet;
    pUartIntfcCmdFunc[UARTCMD_PARITYQ]   = cmdUartParityQuery;
    pUartIntfcCmdFunc[UARTCMD_STOPBIT]   = cmdUartStopBitSet;
    pUartIntfcCmdFunc[UARTCMD_STOPBITQ]  = cmdUartStopBitQuery;
    pUartIntfcCmdFunc[UARTCMD_APPLY]     = cmdUartApplyPara;
}

            
/*********************************************************************************************
函 数 名: cmdUartIntfcCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdUartIntfcCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < UARTCMD_RESERVE) && (pUartIntfcCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pUartIntfcCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


/*********************************************************************************************
函 数 名: cmdCanTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    CanTypeEnum idType;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfCanTypeVerify(cmdDataLen, pCmdData, (void *)&idType))
    {
        g_ciCanInterface.idType = idType;
    }
}


/*********************************************************************************************
函 数 名: cmdCanTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.canIntfc.idType);
    pData = (u8 *)&g_commIntfc.canIntfc.idType;
    cmdFrameSend(CMD_CAN, CANCMD_TYPEQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdCanSendIdSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanSendIdSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 sendId;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfCanSendIdVerify(cmdDataLen, pCmdData, (void *)&sendId))
    {
        g_ciCanInterface.sendId = sendId;
    }
}


/*********************************************************************************************
函 数 名: cmdCanSendIdQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanSendIdQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.canIntfc.sendId);
    pData = (u8 *)&g_commIntfc.canIntfc.sendId;
    cmdFrameSend(CMD_CAN, CANCMD_SENDIDQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdCanBaudSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanBaudSet(u8 cmdDataLen, u8 *pCmdData)
{
    CanBaudEnum baud;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfCanBaudVerify(cmdDataLen, pCmdData, (void *)&baud))
    {
        g_ciCanInterface.baud = baud;
    }
}


/*********************************************************************************************
函 数 名: cmdCanBaudQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanBaudQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.canIntfc.baud);
    pData = (u8 *)&g_commIntfc.canIntfc.baud;
    cmdFrameSend(CMD_CAN, CANCMD_BAUDQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdCanGroupSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanGroupSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 group;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfCanGroupVerify(cmdDataLen, pCmdData, (void *)&group))
    {
        g_ciCanInterface.group = group;
    }
}


/*********************************************************************************************
函 数 名: cmdCanGroupQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanGroupQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.canIntfc.group);
    pData = &g_commIntfc.canIntfc.group;
    cmdFrameSend(CMD_CAN, CANCMD_GROUPQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdCanReceiveIdSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanReceiveIdSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 receiveId;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfCanReceiveIdVerify(cmdDataLen, pCmdData, (void *)&receiveId))
    {
        g_ciCanInterface.receiveId = receiveId;
    }
}


/*********************************************************************************************
函 数 名: cmdCanReceiveIdQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanReceiveIdQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.canIntfc.receiveId);
    pData = (u8 *)&g_commIntfc.canIntfc.receiveId;
    cmdFrameSend(CMD_CAN, CANCMD_RECEIVEIDQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdCanGroupIdSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanGroupId1Set(u8 cmdDataLen, u8 *pCmdData)
{
    u32 groupId;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfCanGroupIdVerify(cmdDataLen, pCmdData, (void *)&groupId))
    {
        g_ciCanInterface.groupId1 = groupId;
    }
}


/*********************************************************************************************
函 数 名: cmdCanGroupIdQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanGroupId1Query(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.canIntfc.groupId1);
    pData = (u8 *)&g_commIntfc.canIntfc.groupId1;
    cmdFrameSend(CMD_CAN, CANCMD_GROUPID1Q, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdCanGroupIdSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanGroupId2Set(u8 cmdDataLen, u8 *pCmdData)
{
    u32 groupId;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfCanGroupIdVerify(cmdDataLen, pCmdData, (void *)&groupId))
    {
        g_ciCanInterface.groupId2 = groupId;
    }
}


/*********************************************************************************************
函 数 名: cmdCanGroupIdQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanGroupId2Query(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    dataLen = sizeof(g_commIntfc.canIntfc.groupId2);
    pData = (u8 *)&g_commIntfc.canIntfc.groupId2;
    cmdFrameSend(CMD_CAN, CANCMD_GROUPID2Q, dataLen, pData);
}



/*********************************************************************************************
函 数 名: cmdCanRadioIdSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanRadioIdSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 radioId;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfCanRadioIdVerify(cmdDataLen, pCmdData, (void *)&radioId))
    {
        g_ciCanInterface.radioId = radioId;
    }
}


/*********************************************************************************************
函 数 名: cmdCanRadioIdQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanRadioIdQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_commIntfc.canIntfc.radioId);
    pData = (u8 *)&g_commIntfc.canIntfc.radioId;
    cmdFrameSend(CMD_CAN, CANCMD_RADIOIDQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdCanApplyPara;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanApplyPara(u8 cmdDataLen, u8 *pCmdData)
{
    memcpy(&g_commIntfc.canIntfc, &g_ciCanInterface, sizeof(CanIntfcStruct));
    
    servCiCanConfig(g_commIntfc.canIntfc);

    /*存储到EEPROM中*/
    servStimerAdd(&g_paraSaveTimer);
    
    //修改完CAN口参数后,需要通知逻辑CAN口接收模块复位 xyzheng 20170330
    //servCanBuildFrameRstFpgaCanRecieve();
    //For Modify NICK
}


/*********************************************************************************************
函 数 名: cmdCanNetManageHash
实现功能: 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
static void cmdCanNetManageHash(u8 cmdDataLen, u8 *pCmdData)
{
    u32 value = 0;
    if(cmdDataLen != 4)
    {
        return;
    }
    if(servCanNetManageGetConfigPreState() == NMT_C0NF_HASH)
    {
        memcpy(&value,&pCmdData[0],4);
        if(servCanNetManageIsHashCodeEqual(value))
        {
            servCanNetManageSetConfigState(NMT_C0NF_HASH);
        }
    }
    else
    {
      
    }
}
/*********************************************************************************************
函 数 名: cmdCanIdentifyQuery;
实现功能: 识别查询命令,当前节点返回一个32位的签名段
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 
*********************************************************************************************/
static void cmdCanNetManageHashQuery(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 0)
    {
        return;
    }
    servCanNetManageInit(NULL,0);
    cmdFrameSend(CMD_CAN, CANCMD_NMTHASHQ, 4, (u8*)&g_stCanNetManageInfo.hash);  
}
/*********************************************************************************************
函 数 名: cmdCanConfigStateSet;
实现功能: 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
static void cmdCanNetManageStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    if((*pCmdData != 1 && *pCmdData != 0 && *pCmdData != 2))
    {
        return;
    }
    if(*pCmdData == NMT_C0NF_HASH)
    {
        servCanNetManageSetConfigPreState(NMT_C0NF_HASH);
    }
    else if(*pCmdData == NMT_CONF_SIGNATURE )
    {
        servCanNetManageSetConfigPreState(NMT_CONF_SIGNATURE);
    }
    else if(*pCmdData == NMT_CONF_IDEL && servCanNetManageGetConfigState() != NMT_CONF_IDEL)//配置结束
    {
        servCanNetManageSetConfigState(NMT_CONF_IDEL);
        servCanNetManageSetConfigPreState(NMT_CONF_IDEL);
    }
   
    return;
}
/*********************************************************************************************
函 数 名: cmdCanConfigStateQuery
实现功能: 查询当前节点是否进入配置状态
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
static void cmdCanNetManageStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    dataLen = sizeof(g_stCanNetManageInfo.config);
    pData = &g_stCanNetManageInfo.config;
    cmdFrameSend(CMD_CAN, CANCMD_NMTSTATEQ, dataLen, pData);
}

/*********************************************************************************************
函 数 名: cmdCanConfigIDSet;
实现功能: 在配置状态下,查询当前节点的ID
输入参数: 参数前一个字节表示索引,查询哪个ID
输出参数: 无;
返 回 值: 无;
说    明: 只查询send id和target id
*********************************************************************************************/
static void cmdCanNetManageIDQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    if(cmdDataLen != 1 || servCanNetManageGetConfigState() == 0)//如果当前节点不在配置状态下,不能修改ID
    {
        return;
    }
    dataLen = 4;
    switch(*pCmdData)
    {
    case 1:
        pData = (u8*)&g_ciCanInterface.sendId;
        break;
    case 2:
        pData = (u8*)&g_ciCanInterface.receiveId;
        break;
    default:
        return;
    }
    cmdFrameSend(CMD_CAN, CANCMD_NMTIDQ, dataLen, pData);
    return;
}
/*********************************************************************************************
函 数 名: cmdCanConfigIDSet;
实现功能: 在配置状态下,修改当前 节点的ID
输入参数: 参数前一个字节表示在哪个配置状态下,第二个字节表示配置哪个ID;后4节点为相应的ID值
输出参数: 无;
返 回 值: 无;
说    明: 只配置send id和target id
pCmdData[0] : 状态
pCmdData[1] : ID类型
pCmdData[2:5] : ID值
*********************************************************************************************/
static void cmdCanNetManageIDSet(u8 cmdDataLen, u8 *pCmdData)
{
    if(servCanNetManageGetConfigState() == NMT_CONF_IDEL || cmdDataLen != 5)//如果当前节点不在配置状态下,查询ID
    {
        return;
    }
    switch(pCmdData[0])
    {
    case 1:
        memcpy(&g_ciCanInterface.sendId,&pCmdData[1],4);
        break;
    case 2:
        memcpy(&g_ciCanInterface.receiveId,&pCmdData[1],4);
        break;
    default:
        break;
    }
    return;
}
/*********************************************************************************************
函 数 名: cmdCanNetManageSignture;
实现功能: 使当前节点进入电子签名配置状态
输入参数: 
输出参数: 无;
返 回 值: 无;
说    明: 
*********************************************************************************************/
static void cmdCanNetManageSignture(u8 cmdDataLen, u8 *pCmdData)
{
    u32 value = 0;
    if(cmdDataLen != 5)
    {
         return;
    }
    if(servCanNetManageGetConfigPreState() != NMT_CONF_IDEL)//如果当前在预置状态,则进入配置状态
    {
        memcpy(&value,&pCmdData[1],4);
        if(servCanNetManageIsSignatureEqual(pCmdData[0],value))//第一个字节,是索引
        {
            servCanNetManageSetConfigState(NMT_CONF_SIGNATURE);
        }
        else//签名段不相等时,退出配置状态(一个签名段下来,可能会有多个节点进入配置状态)
        {
            servCanNetManageSetConfigState(NMT_CONF_IDEL);//如果不相同,则进入空闲状态
        }
    }
    else
    {}
}
/*********************************************************************************************
函 数 名: cmdCanConfigIDSet;
实现功能: 电子签名查询
输入参数: 
输出参数: 无;
返 回 值: 无;
说    明: 
*********************************************************************************************/
static void cmdCanNetManageSigntureQuery(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 1 || servCanNetManageGetConfigState() == NMT_CONF_IDEL)
    {
        return;
    }
    u8 dataLen;
    u8 Data[8] = {0};
    dataLen = 5;
    Data[0] = *pCmdData;
    memcpy(&Data[1],&g_stCanNetManageInfo.signature[*pCmdData],4);
    cmdFrameSend(CMD_CAN, CANCMD_NMTSIGNATUREQ, dataLen, Data);
}
/*********************************************************************************************
函 数 名: cmdCanConfigIDSet;
实现功能: 电子签名长度查询,单位:word(32位)
输入参数: 
输出参数: 无;
返 回 值: 无;
说    明: 
*********************************************************************************************/
static void cmdCanNetManageSigntureSizeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 0)
    {
        return;
    }
    u8 dataLen;
    u8 *pData;
    dataLen = 1;
    pData = &g_stCanNetManageInfo.signature_size;
    cmdFrameSend(CMD_CAN, CANCMD_NMTSIGNATURESIZEQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdCanIntfcCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanIntfcCmdInit(void)
{
    memset(pCanIntfcCmdFunc, 0, sizeof(pCanIntfcCmdFunc));

    pCanIntfcCmdFunc[CANCMD_TYPE]      = cmdCanTypeSet;
    pCanIntfcCmdFunc[CANCMD_TYPEQ]     = cmdCanTypeQuery;
    pCanIntfcCmdFunc[CANCMD_BAUD]      = cmdCanBaudSet;
    pCanIntfcCmdFunc[CANCMD_BAUDQ]     = cmdCanBaudQuery;
    pCanIntfcCmdFunc[CANCMD_GROUP]     = cmdCanGroupSet;
    pCanIntfcCmdFunc[CANCMD_GROUPQ]    = cmdCanGroupQuery;
    pCanIntfcCmdFunc[CANCMD_SENDID]    = cmdCanSendIdSet;
    pCanIntfcCmdFunc[CANCMD_SENDIDQ]   = cmdCanSendIdQuery;
    pCanIntfcCmdFunc[CANCMD_RECEIVEID] = cmdCanReceiveIdSet;
    pCanIntfcCmdFunc[CANCMD_RECEIVEIDQ]= cmdCanReceiveIdQuery;
    pCanIntfcCmdFunc[CANCMD_GROUPID1]  = cmdCanGroupId1Set;
    pCanIntfcCmdFunc[CANCMD_GROUPID1Q] = cmdCanGroupId1Query;
    pCanIntfcCmdFunc[CANCMD_GROUPID2]  = cmdCanGroupId2Set;
    pCanIntfcCmdFunc[CANCMD_GROUPID2Q] = cmdCanGroupId2Query;
    pCanIntfcCmdFunc[CANCMD_RADIOID]   = cmdCanRadioIdSet;
    pCanIntfcCmdFunc[CANCMD_RADIOIDQ]  = cmdCanRadioIdQuery;
    pCanIntfcCmdFunc[CANCMD_APPLY]     = cmdCanApplyPara;

    pCanIntfcCmdFunc[CANCMD_NMTHASH]    = cmdCanNetManageHash;
    pCanIntfcCmdFunc[CANCMD_NMTHASHQ]   = cmdCanNetManageHashQuery;
    pCanIntfcCmdFunc[CANCMD_NMTSTATE]   = cmdCanNetManageStateSet;
    pCanIntfcCmdFunc[CANCMD_NMTSTATEQ]  = cmdCanNetManageStateQuery;
    pCanIntfcCmdFunc[CANCMD_NMTSETID]   = cmdCanNetManageIDSet;
    pCanIntfcCmdFunc[CANCMD_NMTIDQ]     = cmdCanNetManageIDQuery;
    
    pCanIntfcCmdFunc[CANCMD_NMTSIGNATURE]      = cmdCanNetManageSignture;
    pCanIntfcCmdFunc[CANCMD_NMTSIGNATUREQ]     = cmdCanNetManageSigntureQuery;
    pCanIntfcCmdFunc[CANCMD_NMTSIGNATURESIZEQ] = cmdCanNetManageSigntureSizeQuery;
}

            
/*********************************************************************************************
函 数 名: cmdCanIntfcCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdCanIntfcCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < CANCMD_RESERVE) && (pCanIntfcCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pCanIntfcCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************文件尾********************************************/