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
#include "servSoftTimer.h"
#include "servLed.h"
#include "servFpga.h"
#include "eventManageTask.h"



/****************************************�ⲿ��������*****************************************/
extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;
extern DeviceInfoStruct  g_deviceInfo;

extern EventSrcBmpStruct g_eventSrcBmp;

extern bool g_bPerdPostSemToEvent;
extern bool g_bCmdPostSemToFunc;

extern SoftTimerStruct g_paraSaveTimer;

extern SystemCfgBmpStruct g_systemCfgBmp;

#if GELGOOG_SINANJU
extern SoftTimerStruct g_distAlarmLedTimer;
#endif


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

    
    dataLen = EVENT_CODE_LEN;
    pData = g_systemState.eventCode;
    cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, dataLen, pData);
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

    
    dataLen = FPGA_VER_LEN;
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
        servStimerAdd(&g_paraSaveTimer);
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
        servStimerAdd(&g_paraSaveTimer);
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
�� �� ��: cmdSystemDioStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemDioStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u16 data;

    
    data = servFpgaDioRefStatusRead();
    cmdFrameSend(CMD_SYSTME, SYSCMD_DISTATEQ, 2, (u8 *)&data);
}


/*********************************************************************************************
�� �� ��: cmdSystemReverseMotionSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemReverseMotionSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum  revMotion;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSystemReverseMotionVerify(cmdDataLen, pCmdData, (void *)&revMotion))
    {    
        g_systemInfo.revMotion = revMotion;

        //��FUNC���ź���
        g_systemCfgBmp.bRevMotion = true;
        g_bCmdPostSemToFunc = true;

        //�浽EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemReverseMotionQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemReverseMotionQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemInfo.revMotion);
    pData   = (u8 *)&g_systemInfo.revMotion;
    cmdFrameSend(CMD_SYSTME, SYSCMD_REVMOTIONQ, dataLen, pData);
}


#if GELGOOG_SINANJU
/*********************************************************************************************
�� �� ��: cmdSystemFanParaSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemFanParaSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8  fanDuty;
    u32 fanFrequency;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSystemFanParaVerify(cmdDataLen, pCmdData, (void *)&fanDuty, (void *)&fanFrequency))
    {    
        g_systemInfo.fanDuty      = fanDuty;
        g_systemInfo.fanFrequency = fanFrequency;

        servFanConfig(g_systemInfo.fanFrequency, g_systemInfo.fanDuty);

        //�浽EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemFanParaQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemFanParaQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];

    
    dataLen = sizeof(g_systemInfo.fanDuty) + sizeof(g_systemInfo.fanFrequency);
    data[0] = g_systemInfo.fanDuty;
    memcpy(&data[1], &g_systemInfo.fanFrequency, sizeof(g_systemInfo.fanFrequency));
    cmdFrameSend(CMD_SYSTME, SYSCMD_FANPARAQ, dataLen, data);
}


/*********************************************************************************************
�� �� ��: cmdSystemArmLedParaSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemArmLedParaSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8  index;
    u8  armLedDuty;
    u32 armLedFreq;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSystemArmLedParaVerify(cmdDataLen, pCmdData, (void *)&armLedDuty, (void *)&armLedFreq, &index))
    {    
        g_systemInfo.armLedDuty[index] = armLedDuty;
        g_systemInfo.armLedFreq[index] = armLedFreq;

        servArmLedConfig((ArmLedEnum)index, g_systemInfo.armLedFreq[index], g_systemInfo.armLedDuty[index]);

        //�浽EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemArmLedParaQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemArmLedParaQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];
    u8 index = *pCmdData;

    
    if (index < ARMLED_RESERVE)
    {
        dataLen = sizeof(index) + sizeof(g_systemInfo.armLedDuty[index]) + sizeof(g_systemInfo.armLedFreq[index]);
        data[0] = index;
        data[1] = g_systemInfo.armLedDuty[index];
        memcpy(&data[2], &g_systemInfo.armLedFreq[index], sizeof(g_systemInfo.armLedFreq[index]));
        cmdFrameSend(CMD_SYSTME, SYSCMD_ARMLEDPARAQ, dataLen, data);
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemArmLedStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemArmLedStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum ledState;
    u16             ledFreq;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSystemArmLedFreqVerify(cmdDataLen, pCmdData, &ledState, &ledFreq))
    {    
        g_systemState.ledFlickerState = ledState;
        g_systemState.ledFlickerFreq  = ledFreq;

        //���봫�������������ͷ�״̬��LED���в�������Ч
        if (DISTALARM_RELEASE == g_systemState.sysDistAlarm)
        {
            if (SENSOR_ON == g_systemState.ledFlickerState)
            {
                servStimerInit(&g_distAlarmLedTimer, ledFreq, eventDistAlarmLedTimerCB);
                servStimerAdd(&g_distAlarmLedTimer);
            }
            else
            {
                servStimerDelete(&g_distAlarmLedTimer);
                eventDistAlarmLedProcess(true);
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdSystemArmLedStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSystemArmLedStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];


    dataLen = sizeof(g_systemState.ledFlickerState) + sizeof(g_systemState.ledFlickerFreq);
    data[0] = g_systemState.ledFlickerState;
    data[1] = g_systemState.ledFlickerFreq;
    data[2] = g_systemState.ledFlickerFreq >> 8;
    
    cmdFrameSend(CMD_SYSTME, SYSCMD_ARMLEDSTATEQ, dataLen, data);
}
#endif


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
    
    pSystemCmdFunc[SYSCMD_EVENTCODEQ] = cmdSystemErrorQuery;
    pSystemCmdFunc[SYSCMD_DISTATEQ]   = cmdSystemDioStateQuery;

    pSystemCmdFunc[SYSCMD_REVMOTION]  = cmdSystemReverseMotionSet;
    pSystemCmdFunc[SYSCMD_REVMOTIONQ] = cmdSystemReverseMotionQuery;

#if GELGOOG_SINANJU
    pSystemCmdFunc[SYSCMD_FANPARA]     = cmdSystemFanParaSet;
    pSystemCmdFunc[SYSCMD_FANPARAQ]    = cmdSystemFanParaQuery;
    pSystemCmdFunc[SYSCMD_ARMLEDPARA]  = cmdSystemArmLedParaSet;
    pSystemCmdFunc[SYSCMD_ARMLEDPARAQ] = cmdSystemArmLedParaQuery;

    pSystemCmdFunc[SYSCMD_ARMLEDSTATE]  = cmdSystemArmLedStateSet;
    pSystemCmdFunc[SYSCMD_ARMLEDSTATEQ] = cmdSystemArmLedStateQuery;
#endif
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