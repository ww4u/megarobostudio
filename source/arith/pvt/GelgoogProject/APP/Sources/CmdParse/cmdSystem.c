/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdSystem.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "project.h"
#include "cmdSystem.h"
#include "pvrfSystem.h"
#include "cmdMainParse.h"
#include "servCommIntfc.h"
#include "servSystemPara.h"



/****************************************�ⲿ��������*****************************************/
extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;
extern DeviceInfoStruct  g_deviceInfo;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pSystemCmdFunc[SYSCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdSystemErrorQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemErrorQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = ERROR_CODE_LEN;
    pData = g_systemState.errorCode;
    cmdFrameSend(CMD_SYSTME, SYSCMD_ERRORQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: cmdSystemSoftVerQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemSoftVerQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = SOFTWARE_VER_LEN - 1;    //����汾�Ų�����
    pData = g_deviceInfo.software;
    cmdFrameSend(CMD_SYSTME, SYSCMD_SOFTVERQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: cmdSystemFpgaVerQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemFpgaVerQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = FPGA_VER_LEN - 1;    //�޶��汾�Ų�����
    pData = g_deviceInfo.fpga.verArray;
    cmdFrameSend(CMD_SYSTME, SYSCMD_FPGAVERQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: cmdSystemHardVerQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemHardVerQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = HARDWARE_VER_LEN;
    pData = g_deviceInfo.hardware;
    cmdFrameSend(CMD_SYSTME, SYSCMD_HARDVERQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: cmdSystemBootVerQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemBootVerQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = BOOT_VER_LEN;
    pData = g_deviceInfo.boot;
    cmdFrameSend(CMD_SYSTME, SYSCMD_BOOTVERQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: cmdSystemModeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    WorkModeEnum workMode;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSystemModeVerify(cmdDataLen, pCmdData, (void *)&workMode))
    {    
        g_systemInfo.workMode = workMode;

        //�浽EEPROM��
        servSystemInfoWrite(&g_systemInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemModeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemInfo.workMode);
    pData = (u8 *)&g_systemInfo.workMode;
    cmdFrameSend(CMD_SYSTME, SYSCMD_WORKMODEQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: cmdSystemPoweronSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemPoweronSet(u8 cmdDataLen, u8 *pCmdData)
{
    PowerOnModeEnum powerOn;

    
    //���в�����֤ 
    if (PARA_VERIFY_NO_ERROR == pvrfSystemPowerOnVerify(cmdDataLen, pCmdData, (void *)&powerOn))
    {    
        g_systemInfo.powerOn = powerOn;

        //�浽EEPROM��
        servSystemInfoWrite(&g_systemInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemPoweronQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemPoweronQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemInfo.powerOn);
    pData = (u8 *)&g_systemInfo.powerOn;
    cmdFrameSend(CMD_SYSTME, SYSCMD_POWERONQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: cmdSystemSnQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemSnQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 i;
    u8 frameNum;
    u8 tempData[6];

    
    if ((DEVICE_SN_LEN % FRACTION_FRAME_DATA_LEN_MAX) != 0)
    {
        frameNum = DEVICE_SN_LEN / FRACTION_FRAME_DATA_LEN_MAX + 1;
    }
    else
    {
        frameNum = DEVICE_SN_LEN / FRACTION_FRAME_DATA_LEN_MAX;
    }
    pData = (u8 *)g_deviceInfo.storageInfo.deviceSn;

    for (i = 1;i <= frameNum;i++)
    {
        //��4bit������ӣ���������4bit����4bit�����ĸ
        //�ܹ����Է���16 x 5Bytes = 80Bytes(640bits)�����ݣ�Ŀǰû�д����ݣ����Բ���frameNum��С���ж���
        tempData[0] = FRACTION(i, frameNum);

        if (i == frameNum)
        {
            dataLen = DEVICE_SN_LEN % FRACTION_FRAME_DATA_LEN_MAX;    //����ʣ���
        }
        else
        {
            dataLen = FRACTION_FRAME_DATA_LEN_MAX;            
        }
        memcpy(&tempData[1], pData, dataLen);
        pData += dataLen;
        dataLen++;    //��1��Ϊ������tempData[0]
        
        cmdFrameSend(CMD_SYSTME, SYSCMD_SNQ, dataLen, tempData);
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

  
    dataLen = sizeof(g_deviceInfo.mDevcModel) + sizeof(g_deviceInfo.sDevcModel);
    pData = (u8 *)&g_deviceInfo.mDevcModel;
    cmdFrameSend(CMD_SYSTME, SYSCMD_TYPEQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: cmdSystemReptSwitchSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemReptSwitchSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum reportSwitch;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSystemReptSwitchVerify(cmdDataLen, pCmdData, (void *)&reportSwitch))
    {    
        g_systemState.reportSwitch = reportSwitch;
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemReptSwitchQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemReptSwitchQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemState.reportSwitch);
    pData = (u8 *)&g_systemState.reportSwitch;
    cmdFrameSend(CMD_SYSTME, SYSCMD_REPORTSWQ, dataLen, pData);
}


/*********************************************************************************************
�� �� ��: SystemCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemCmdInit(void)
{
    memset(pSystemCmdFunc, 0, sizeof(pSystemCmdFunc));
    
    pSystemCmdFunc[SYSCMD_WORKMODE]  = cmdSystemModeSet;
    pSystemCmdFunc[SYSCMD_WORKMODEQ] = cmdSystemModeQuery;
    pSystemCmdFunc[SYSCMD_POWERON]   = cmdSystemPoweronSet;
    pSystemCmdFunc[SYSCMD_POWERONQ]  = cmdSystemPoweronQuery;
    pSystemCmdFunc[SYSCMD_SNQ]       = cmdSystemSnQuery;
    pSystemCmdFunc[SYSCMD_TYPEQ]     = cmdSystemTypeQuery;
    pSystemCmdFunc[SYSCMD_SOFTVERQ]  = cmdSystemSoftVerQuery;
    pSystemCmdFunc[SYSCMD_FPGAVERQ]  = cmdSystemFpgaVerQuery;
    pSystemCmdFunc[SYSCMD_HARDVERQ]  = cmdSystemHardVerQuery;
    pSystemCmdFunc[SYSCMD_BOOTVERQ]  = cmdSystemBootVerQuery;
    pSystemCmdFunc[SYSCMD_ERRORQ]    = cmdSystemErrorQuery;
    pSystemCmdFunc[SYSCMD_REPORTSW]  = cmdSystemReptSwitchSet;
    pSystemCmdFunc[SYSCMD_REPORTSWQ] = cmdSystemReptSwitchQuery;
}

            
/*********************************************************************************************
�� �� ��: cmdSystemCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < SYSCMD_RESERVE) && (pSystemCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pSystemCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************�ļ�β********************************************/