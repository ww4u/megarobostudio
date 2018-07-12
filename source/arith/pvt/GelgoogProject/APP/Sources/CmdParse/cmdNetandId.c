/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdNetandId.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.11;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "project.h"
#include "cmdNetandId.h"
#include "pvrfNetandId.h"
#include "cmdMainParse.h"
#include "servCommIntfc.h"
#include "servSystemPara.h"



/****************************************�ⲿ��������*****************************************/
extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;
extern EventSrcBmpStruct g_eventSrcBmp;

extern bool g_bCmdPostSemToEvent;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pIdCmdFunc[IDCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdIdentityGroupSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIdentityGroupSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 groupNum;
    u8 groupIndex;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;

    //���в�����֤ 
    if (PARA_VERIFY_NO_ERROR == pvrfIdentityGroupVerify(cmdDataLen, pCmdData, (void *)&groupNum, &groupIndex))
    {    
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_systemInfo.group[i][groupIndex] = groupNum;
            }
        }
        servSystemInfoWrite(&g_systemInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdIdentityGroupQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIdentityGroupQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 groupIndex;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;


    cmdDataLen -= 1;
    groupIndex = *pCmdData;
    if (groupIndex < GROUP_NUM)
    {
        dataLen = sizeof(g_systemInfo.group[CH1][groupIndex]) + sizeof(groupIndex) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_systemInfo.group[chanNum][groupIndex];
            data[0] = chanNum;
            data[1] = groupIndex;
            for (i = 0;i < sizeof(g_systemInfo.group[chanNum][groupIndex]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_IDENTITY, IDCMD_GROUPQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_systemInfo.group[i][groupIndex];
                    data[0] = i;
                    data[1] = groupIndex;
                    for (j = 0;j < sizeof(g_systemInfo.group[i][groupIndex]);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_IDENTITY, IDCMD_GROUPQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdIdentityDistDeviceSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIdentityDistDeviceSet(u8 cmdDataLen, u8 *pCmdData)
{
    DistingStateEnum distState;
    

    //���в�����֤ 
    if (PARA_VERIFY_NO_ERROR == pvrfIdentityDistStateVerify(cmdDataLen, pCmdData, (void *)&distState))
    {    
        g_systemState.distState = distState;

        g_eventSrcBmp.bDeviceId = true;
        g_bCmdPostSemToEvent = true;
    }
}


/*********************************************************************************************
�� �� ��: cmdIdentityDistDeviceQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIdentityDistDeviceQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_IDENTITY, IDCMD_DISTDEVICEQ, sizeof(g_systemState.distState), (u8 *)&g_systemState.distState);
}


/*********************************************************************************************
�� �� ��: cmdIdentityLabelSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIdentityLabelSet(u8 cmdDataLen, u8 *pCmdData)
{
    MainLabelEnum  mainLabel;
    SubLabelEnum   subLabel;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;

    //���в�����֤ 
    if (PARA_VERIFY_NO_ERROR == pvrfIdentityLabelVerify(cmdDataLen, pCmdData, (void *)&mainLabel, (void *)&subLabel))
    {    
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_systemInfo.mainLabel[i] = mainLabel;
                g_systemInfo.subLabel[i]  = subLabel;
            }
        }
        servSystemInfoWrite(&g_systemInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdIdentityLabelQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIdentityLabelQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];
    u8 i;
    u8 chanNum = *pCmdData++;


    dataLen = sizeof(g_systemInfo.mainLabel[CH1]) + sizeof(g_systemInfo.subLabel[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        data[0] = chanNum;
        data[1] = g_systemInfo.mainLabel[chanNum];
        data[2] = g_systemInfo.subLabel[chanNum];
        cmdFrameSend(CMD_IDENTITY, IDCMD_LABELQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                data[0] = i;
                data[1] = g_systemInfo.mainLabel[i];
                data[2] = g_systemInfo.subLabel[i];
                cmdFrameSend(CMD_IDENTITY, IDCMD_LABELQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdIdentityCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIdentityCmdInit(void)
{
    memset(pIdCmdFunc, 0, sizeof(pIdCmdFunc));
    
    pIdCmdFunc[IDCMD_GROUP]       = cmdIdentityGroupSet;
    pIdCmdFunc[IDCMD_GROUPQ]      = cmdIdentityGroupQuery;
    pIdCmdFunc[IDCMD_DISTDEVICE]  = cmdIdentityDistDeviceSet;
    pIdCmdFunc[IDCMD_DISTDEVICEQ] = cmdIdentityDistDeviceQuery;
    
    pIdCmdFunc[IDCMD_LABEL]  = cmdIdentityLabelSet;
    pIdCmdFunc[IDCMD_LABELQ] = cmdIdentityLabelQuery;
}

            
/*********************************************************************************************
�� �� ��: cmdIdentityCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIdentityCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < IDCMD_RESERVE) && (pIdCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pIdCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************�ļ�β********************************************/