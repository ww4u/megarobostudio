/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdInterface.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "cmdInterface.h"
#include "cmdMainParse.h"
#include "pvrfInterface.h"
#include "servSystemPara.h"
#include "servCanNetManage.h"
#include "servCommIntfc.h"
#include "servSoftTimer.h"



/****************************************�ⲿ��������*****************************************/
extern CommIntfcStruct   g_commIntfc;
extern SystemStateStruct g_systemState;
extern DeviceInfoStruct  g_deviceInfo;

extern SoftTimerStruct g_paraSaveTimer;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pLinkCmdFunc[LINKCMD_RESERVE];
SubCmdProFunc pUartIntfcCmdFunc[UARTCMD_RESERVE];
SubCmdProFunc pCanIntfcCmdFunc[CANCMD_RESERVE];

extern CanIntfcStruct  g_ciCanInterface;     
extern UartIntfcStruct g_ciUartInterface;



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdLinkIntfcSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdLinkIntfcSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 subType;
    LinkTypeEnum linkType = g_commIntfc.linkType;
    

    if (sizeof(LinkTypeEnum) == cmdDataLen)    //������Ҫ��ȷ
    {
        subType = *pCmdData;    
        switch (subType)
        {
            case 0:
                g_commIntfc.linkType = LINK_NONE;//�ϵ�����
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
�� �� ��: cmdLinkIntfcQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdLinkDeviceInfoQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdLinkCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdLinkCmdInit(void)
{
    memset(pLinkCmdFunc, 0, sizeof(pLinkCmdFunc));

    pLinkCmdFunc[LINKCMD_INTFC]  = cmdLinkIntfcSet;
    pLinkCmdFunc[LINKCMD_INTFCQ] = cmdLinkIntfcQuery;
    
    pLinkCmdFunc[LINKCMD_DEVICEINFOQ] = cmdLinkDeviceInfoQuery;
}


/*********************************************************************************************
�� �� ��: cmdLinkCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdUartBaudSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUartBaudSet(u8 cmdDataLen, u8 *pCmdData)
{
    UartBaudEnum baud;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfUartBaudVerify(cmdDataLen, pCmdData, (void *)&baud))
    {
        g_ciUartInterface.baud = baud;
    }
}


/*********************************************************************************************
�� �� ��: cmdUartBaudQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdUartWordLenSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUartWordLenSet(u8 cmdDataLen, u8 *pCmdData)
{
    WordLenEnum wordLen;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfUartWordLenVerify(cmdDataLen, pCmdData, (void *)&wordLen))
    {
        g_ciUartInterface.wordLen = wordLen;
    }
}


/*********************************************************************************************
�� �� ��: cmdUartWordLenQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdUartFlowCtlSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUartFlowCtlSet(u8 cmdDataLen, u8 *pCmdData)
{
    FlowCtlEnum flowCtl;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfUartFlowCtlVerify(cmdDataLen, pCmdData, (void *)&flowCtl))
    {
        g_ciUartInterface.flowCtl = flowCtl;
    }
}


/*********************************************************************************************
�� �� ��: cmdUartFlowCtlQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdUartParitySet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUartParitySet(u8 cmdDataLen, u8 *pCmdData)
{
    ParityEnum parity;

    
    //���в�����֤ 
    if (PARA_VERIFY_NO_ERROR == pvrfUartParityVerify(cmdDataLen, pCmdData, (void *)&parity))
    {
        g_ciUartInterface.parity = parity;
    }
}


/*********************************************************************************************
�� �� ��: cmdUartParityQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdUartStopBitSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUartStopBitSet(u8 cmdDataLen, u8 *pCmdData)
{
    StopBitEnum stopBit;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfUartStopBitVerify(cmdDataLen, pCmdData, (void *)&stopBit))
    {
        g_ciUartInterface.stopBit = stopBit;
    }
}


/*********************************************************************************************
�� �� ��: cmdUartStopBitQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdUartApplyPara;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUartApplyPara(u8 cmdDataLen, u8 *pCmdData)
{    
    memcpy(&g_commIntfc.uartIntfc, &g_ciUartInterface, sizeof(UartIntfcStruct));

    servCiUartConfig(g_commIntfc.uartIntfc);

    /*�洢��EEPROM��*/
    servStimerAdd(&g_paraSaveTimer);
}


/*********************************************************************************************
�� �� ��: cmdUartIntfcCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdUartIntfcCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    CanTypeEnum idType;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfCanTypeVerify(cmdDataLen, pCmdData, (void *)&idType))
    {
        g_ciCanInterface.idType = idType;
    }
}


/*********************************************************************************************
�� �� ��: cmdCanTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanSendIdSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanSendIdSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 sendId;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfCanSendIdVerify(cmdDataLen, pCmdData, (void *)&sendId))
    {
        g_ciCanInterface.sendId = sendId;
    }
}


/*********************************************************************************************
�� �� ��: cmdCanSendIdQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanBaudSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanBaudSet(u8 cmdDataLen, u8 *pCmdData)
{
    CanBaudEnum baud;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfCanBaudVerify(cmdDataLen, pCmdData, (void *)&baud))
    {
        g_ciCanInterface.baud = baud;
    }
}


/*********************************************************************************************
�� �� ��: cmdCanBaudQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanGroupSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanGroupSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 group;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfCanGroupVerify(cmdDataLen, pCmdData, (void *)&group))
    {
        g_ciCanInterface.group = group;
    }
}


/*********************************************************************************************
�� �� ��: cmdCanGroupQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanReceiveIdSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanReceiveIdSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 receiveId;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfCanReceiveIdVerify(cmdDataLen, pCmdData, (void *)&receiveId))
    {
        g_ciCanInterface.receiveId = receiveId;
    }
}


/*********************************************************************************************
�� �� ��: cmdCanReceiveIdQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanGroupIdSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanGroupId1Set(u8 cmdDataLen, u8 *pCmdData)
{
    u32 groupId;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfCanGroupIdVerify(cmdDataLen, pCmdData, (void *)&groupId))
    {
        g_ciCanInterface.groupId1 = groupId;
    }
}


/*********************************************************************************************
�� �� ��: cmdCanGroupIdQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanGroupIdSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanGroupId2Set(u8 cmdDataLen, u8 *pCmdData)
{
    u32 groupId;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfCanGroupIdVerify(cmdDataLen, pCmdData, (void *)&groupId))
    {
        g_ciCanInterface.groupId2 = groupId;
    }
}


/*********************************************************************************************
�� �� ��: cmdCanGroupIdQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanRadioIdSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanRadioIdSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 radioId;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfCanRadioIdVerify(cmdDataLen, pCmdData, (void *)&radioId))
    {
        g_ciCanInterface.radioId = radioId;
    }
}


/*********************************************************************************************
�� �� ��: cmdCanRadioIdQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanApplyPara;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCanApplyPara(u8 cmdDataLen, u8 *pCmdData)
{
    memcpy(&g_commIntfc.canIntfc, &g_ciCanInterface, sizeof(CanIntfcStruct));
    
    servCiCanConfig(g_commIntfc.canIntfc);

    /*�洢��EEPROM��*/
    servStimerAdd(&g_paraSaveTimer);
    
    //�޸���CAN�ڲ�����,��Ҫ֪ͨ�߼�CAN�ڽ���ģ�鸴λ xyzheng 20170330
    //servCanBuildFrameRstFpgaCanRecieve();
    //For Modify NICK
}


/*********************************************************************************************
�� �� ��: cmdCanNetManageHash
ʵ�ֹ���: 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanIdentifyQuery;
ʵ�ֹ���: ʶ���ѯ����,��ǰ�ڵ㷵��һ��32λ��ǩ����
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: 
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
�� �� ��: cmdCanConfigStateSet;
ʵ�ֹ���: 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    else if(*pCmdData == NMT_CONF_IDEL && servCanNetManageGetConfigState() != NMT_CONF_IDEL)//���ý���
    {
        servCanNetManageSetConfigState(NMT_CONF_IDEL);
        servCanNetManageSetConfigPreState(NMT_CONF_IDEL);
    }
   
    return;
}
/*********************************************************************************************
�� �� ��: cmdCanConfigStateQuery
ʵ�ֹ���: ��ѯ��ǰ�ڵ��Ƿ��������״̬
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanConfigIDSet;
ʵ�ֹ���: ������״̬��,��ѯ��ǰ�ڵ��ID
�������: ����ǰһ���ֽڱ�ʾ����,��ѯ�ĸ�ID
�������: ��;
�� �� ֵ: ��;
˵    ��: ֻ��ѯsend id��target id
*********************************************************************************************/
static void cmdCanNetManageIDQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    if(cmdDataLen != 1 || servCanNetManageGetConfigState() == 0)//�����ǰ�ڵ㲻������״̬��,�����޸�ID
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
�� �� ��: cmdCanConfigIDSet;
ʵ�ֹ���: ������״̬��,�޸ĵ�ǰ �ڵ��ID
�������: ����ǰһ���ֽڱ�ʾ���ĸ�����״̬��,�ڶ����ֽڱ�ʾ�����ĸ�ID;��4�ڵ�Ϊ��Ӧ��IDֵ
�������: ��;
�� �� ֵ: ��;
˵    ��: ֻ����send id��target id
pCmdData[0] : ״̬
pCmdData[1] : ID����
pCmdData[2:5] : IDֵ
*********************************************************************************************/
static void cmdCanNetManageIDSet(u8 cmdDataLen, u8 *pCmdData)
{
    if(servCanNetManageGetConfigState() == NMT_CONF_IDEL || cmdDataLen != 5)//�����ǰ�ڵ㲻������״̬��,��ѯID
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
�� �� ��: cmdCanNetManageSignture;
ʵ�ֹ���: ʹ��ǰ�ڵ�������ǩ������״̬
�������: 
�������: ��;
�� �� ֵ: ��;
˵    ��: 
*********************************************************************************************/
static void cmdCanNetManageSignture(u8 cmdDataLen, u8 *pCmdData)
{
    u32 value = 0;
    if(cmdDataLen != 5)
    {
         return;
    }
    if(servCanNetManageGetConfigPreState() != NMT_CONF_IDEL)//�����ǰ��Ԥ��״̬,���������״̬
    {
        memcpy(&value,&pCmdData[1],4);
        if(servCanNetManageIsSignatureEqual(pCmdData[0],value))//��һ���ֽ�,������
        {
            servCanNetManageSetConfigState(NMT_CONF_SIGNATURE);
        }
        else//ǩ���β����ʱ,�˳�����״̬(һ��ǩ��������,���ܻ��ж���ڵ��������״̬)
        {
            servCanNetManageSetConfigState(NMT_CONF_IDEL);//�������ͬ,��������״̬
        }
    }
    else
    {}
}
/*********************************************************************************************
�� �� ��: cmdCanConfigIDSet;
ʵ�ֹ���: ����ǩ����ѯ
�������: 
�������: ��;
�� �� ֵ: ��;
˵    ��: 
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
�� �� ��: cmdCanConfigIDSet;
ʵ�ֹ���: ����ǩ�����Ȳ�ѯ,��λ:word(32λ)
�������: 
�������: ��;
�� �� ֵ: ��;
˵    ��: 
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
�� �� ��: cmdCanIntfcCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdCanIntfcCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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



/*******************************************�ļ�β********************************************/