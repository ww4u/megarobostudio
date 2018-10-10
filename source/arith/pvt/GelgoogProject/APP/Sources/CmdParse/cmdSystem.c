/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdSystem.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
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



/****************************************外部变量声明*****************************************/
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


/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pSystemCmdFunc[SYSCMD_RESERVE];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdSystemErrorQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemSoftVerQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSystemSoftVerQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = SOFTWARE_VER_LEN - 1;    //编译版本号不对外
    pData = g_deviceInfo.software;
    cmdFrameSend(CMD_SYSTME, SYSCMD_SOFTVERQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdSystemFpgaVerQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemHardVerQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemBootVerQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSystemModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    WorkModeEnum workMode;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSystemModeVerify(cmdDataLen, pCmdData, (void *)&workMode))
    {    
        g_systemInfo.workMode = workMode;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSystemModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemPoweronSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSystemPoweronSet(u8 cmdDataLen, u8 *pCmdData)
{
    PowerOnModeEnum powerOn;

    
    //进行参数验证 
    if (PARA_VERIFY_NO_ERROR == pvrfSystemPowerOnVerify(cmdDataLen, pCmdData, (void *)&powerOn))
    {    
        g_systemInfo.powerOn = powerOn;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSystemPoweronQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemSnQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
        //高4bit代表分子，所以左移4bit，低4bit代表分母
        //总共可以发送16 x 5Bytes = 80Bytes(640bits)的数据，目前没有大数据，所以不做frameNum大小的判断了
        tempData[0] = FRACTION(i, frameNum);

        if (i == frameNum)
        {
            dataLen = DEVICE_SN_LEN % FRACTION_FRAME_DATA_LEN_MAX;    //发送剩余的
        }
        else
        {
            dataLen = FRACTION_FRAME_DATA_LEN_MAX;            
        }
        memcpy(&tempData[1], pData, dataLen);
        pData += dataLen;
        dataLen++;    //加1是为了算上tempData[0]
        
        cmdFrameSend(CMD_SYSTME, SYSCMD_SNQ, dataLen, tempData);
    }
}


/*********************************************************************************************
函 数 名: cmdSystemTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemDioStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSystemDioStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u16 data;

    
    data = servFpgaDioRefStatusRead();
    cmdFrameSend(CMD_SYSTME, SYSCMD_DISTATEQ, 2, (u8 *)&data);
}


/*********************************************************************************************
函 数 名: cmdSystemReverseMotionSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSystemReverseMotionSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum  revMotion;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSystemReverseMotionVerify(cmdDataLen, pCmdData, (void *)&revMotion))
    {    
        g_systemInfo.revMotion = revMotion;

        //给FUNC发信号量
        g_systemCfgBmp.bRevMotion = true;
        g_bCmdPostSemToFunc = true;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSystemReverseMotionQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemFanParaSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSystemFanParaSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8  fanDuty;
    u32 fanFrequency;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSystemFanParaVerify(cmdDataLen, pCmdData, (void *)&fanDuty, (void *)&fanFrequency))
    {    
        g_systemInfo.fanDuty      = fanDuty;
        g_systemInfo.fanFrequency = fanFrequency;

        servFanConfig(g_systemInfo.fanFrequency, g_systemInfo.fanDuty);

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSystemFanParaQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemArmLedParaSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSystemArmLedParaSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8  index;
    u8  armLedDuty;
    u32 armLedFreq;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSystemArmLedParaVerify(cmdDataLen, pCmdData, (void *)&armLedDuty, (void *)&armLedFreq, &index))
    {    
        g_systemInfo.armLedDuty[index] = armLedDuty;
        g_systemInfo.armLedFreq[index] = armLedFreq;

        servArmLedConfig((ArmLedEnum)index, g_systemInfo.armLedFreq[index], g_systemInfo.armLedDuty[index]);

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSystemArmLedParaQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemArmLedStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSystemArmLedStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum ledState;
    u16             ledFreq;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSystemArmLedFreqVerify(cmdDataLen, pCmdData, &ledState, &ledFreq))
    {    
        g_systemState.ledFlickerState = ledState;
        g_systemState.ledFlickerFreq  = ledFreq;

        //距离传感器报警处于释放状态对LED进行操作才有效
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
函 数 名: cmdSystemArmLedStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: SystemCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: cmdSystemCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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



/*******************************************文件尾********************************************/