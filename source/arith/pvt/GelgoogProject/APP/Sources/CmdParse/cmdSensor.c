/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdSensor.c；
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
#include "cmdSensor.h"
#include "pvrfSensor.h"
#include "pvrfInterface.h"
#include "cmdMainParse.h"
#include "servSoftTimer.h"
#include "servCommIntfc.h"
#include "servSystemPara.h"
#include "servStreamBuffer.h"
#include "servFpga.h"



/****************************************外部变量声明*****************************************/
extern SystemInfoStruct   g_systemInfo;
extern MotorInfoStruct    g_motorInfo; 
extern ReportInfoStruct   g_reportInfo;
extern SensorDataStruct   g_sensorData;
extern ChanCfgBmpStruct   g_chanCfgBmp[CH_TOTAL];
extern SystemCfgBmpStruct g_systemCfgBmp;
extern SystemStateStruct  g_systemState;
extern SensorUartStruct   g_sensorUart;
extern EventSrcBmpStruct  g_eventSrcBmp;

#ifdef PROJECT_QUBELEY
extern AnalogInfoStruct g_analogInfo;
extern DeviceInfoStruct g_deviceInfo;
#endif

extern bool g_bCmdPostSemToFunc;
extern bool g_bPerdPostSemToFunc;
extern bool g_bPerdPostSemToCmd;
extern bool g_bPerdPostSemToEvent;

extern WaveTableStruct  g_waveTable[CH_TOTAL][WTTYPE_RESERVE];

extern UartIntfcStruct    g_sensorUartIntfc[UARTNUM_RESERVE];
extern StreamBufferStruct g_senUartRxBuffer[UARTNUM_RESERVE][SENSOR_RESERVE];

extern TrigInInfoStruct   g_trigInInfo;

#if GELGOOG_SINANJU
extern SensorAlarmStruct g_sensorAlarm;
#endif

extern SoftTimerStruct g_paraSaveTimer;



/*****************************************局部宏定义******************************************/
#define    UART_SENSOR_FRAME_DATA_LEN_MAX    3    //每帧最大发送的数据长度
#define    UART_SENSOR_FRAME_DATA_INDEX      3    //数据索引



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pReportCmdFunc[REPORTCMD_RESERVE];
SubCmdProFunc pSensorUartCmdFunc[SNUARTCMD_RESERVE];

#if GELGOOG_SINANJU
SubCmdProFunc pAbsEncAlarmCmdFunc[ENCALARMCMD_RESERVE];
SubCmdProFunc pDistAlarmCmdFunc[DISTALARMCMD_RESERVE];
SubCmdProFunc pPdmCmdFunc[FACTORYCMD_RESERVE];
#endif

#ifdef PROJECT_QUBELEY
SubCmdProFunc pOtpCmdFunc[OTPCMD_RESERVE];
SubCmdProFunc pAnalogInCmdFunc[ANALOGINCMD_RESERVE];

SoftTimerStruct  g_otpTimer;
#endif

SoftTimerStruct  g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];
SoftTimerStruct  g_driverMonitorTimer[CH_TOTAL];



/******************************************函数实现*******************************************/
#ifdef PROJECT_QUBELEY
/*********************************************************************************************
函 数 名: cmdOtpTempTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpTempTimerCB(void *timeOutPara)
{
    u8 dataLen;
    u8 *pData;


    dataLen = sizeof(g_sensorData.temperature);
    pData = (u8 *)&g_sensorData.temperature;
    cmdFrameSend(CMD_OTP, OTPCMD_DATAQ, dataLen, pData);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;

    g_systemCfgBmp.bQryTemp = true;

    //回调函数都是在周期性线程中调用，所以是周期性给Func发送信号量
    g_bPerdPostSemToFunc = true;

    if (g_otpTimer.bUsed)
    {
        servStimerAdd(&g_otpTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdOtpStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfOtpStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        g_systemInfo.otpInfo.state = state;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);

        if (SENSOR_ON == state)
        {
            servStimerAdd(&g_otpTimer);
        }
        else
        {
            servStimerDelete(&g_otpTimer);
        }
    }
}


/*********************************************************************************************
函 数 名: cmdOtpStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemInfo.otpInfo.state);
    pData = (u8 *)&g_systemInfo.otpInfo.state;
    cmdFrameSend(CMD_OTP, OTPCMD_STATEQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdOtpThresholdSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpThresholdSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16 threshold;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfOtpThresholdVerify(cmdDataLen, pCmdData, (void *)&threshold))
    {
        g_systemInfo.otpInfo.threshold = threshold;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdOtpThresholdQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpThresholdQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemInfo.otpInfo.threshold);
    pData = (u8 *)&g_systemInfo.otpInfo.threshold;
    cmdFrameSend(CMD_OTP, OTPCMD_THRESHOLDQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdOtpResponseSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfOtpResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_systemInfo.otpInfo.response = response;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdOtpResponseQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemInfo.otpInfo.response);
    pData = (u8 *)&g_systemInfo.otpInfo.response;
    cmdFrameSend(CMD_OTP, OTPCMD_RESPONSEQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdOtpPeriodSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32  period;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfOtpPeriodVerify(cmdDataLen, pCmdData, (void *)&period))
    {
        g_systemInfo.otpInfo.period = period;

        servStimerAdd(&g_paraSaveTimer);

        servStimerInit(&g_otpTimer, period, cmdOtpTempTimerCB);
    }
}


/*********************************************************************************************
函 数 名: cmdOtpPeriodQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpPeriodQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;


    dataLen = sizeof(g_systemInfo.otpInfo.period);
    pData = (u8 *)&g_systemInfo.otpInfo.period;
    cmdFrameSend(CMD_OTP, OTPCMD_PERIODQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdOtpDataQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpDataQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;


    dataLen = sizeof(g_sensorData.temperature);
    pData = (u8 *)&g_sensorData.temperature;
    cmdFrameSend(CMD_OTP, OTPCMD_DATAQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdOtpCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpCmdInit(void)
{
    memset(pOtpCmdFunc, 0, sizeof(pOtpCmdFunc));

    pOtpCmdFunc[OTPCMD_STATE]      = cmdOtpStateSet;
    pOtpCmdFunc[OTPCMD_STATEQ]     = cmdOtpStateQuery;
    pOtpCmdFunc[OTPCMD_THRESHOLD]  = cmdOtpThresholdSet;
    pOtpCmdFunc[OTPCMD_THRESHOLDQ] = cmdOtpThresholdQuery;
    pOtpCmdFunc[OTPCMD_RESPONSE]   = cmdOtpResponseSet;
    pOtpCmdFunc[OTPCMD_RESPONSEQ]  = cmdOtpResponseQuery;
    pOtpCmdFunc[OTPCMD_PERIOD]     = cmdOtpPeriodSet;
    pOtpCmdFunc[OTPCMD_PERIODQ]    = cmdOtpPeriodQuery;
    pOtpCmdFunc[OTPCMD_DATAQ]      = cmdOtpDataQuery;
}

            
/*********************************************************************************************
函 数 名: cmdOtpCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdOtpCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < OTPCMD_RESERVE) && 
        (pOtpCmdFunc[pCmdStackFrame->subType] != NULL) &&
        (SDMODEL_C23D == g_deviceInfo.sDevcModel))    //-D才有AI和OTP
    {    
        pOtpCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


/*********************************************************************************************
函 数 名: cmdAnalogInStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        g_analogInfo.state = state;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
        
        //给FUNC发信号量
        g_systemCfgBmp.bAnalogIn = true;
        g_bCmdPostSemToFunc = true;
    }
}


/*********************************************************************************************
函 数 名: cmdAnalogInStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_analogInfo.state);
    pData = (u8 *)&g_analogInfo.state;
    cmdFrameSend(CMD_ANALOGIN, ANALOGINCMD_STATEQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdAnalogInThresholdHSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInThresholdHSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 threshold;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInThresholdVerify(cmdDataLen, pCmdData, (void *)&threshold))
    {
        g_analogInfo.thresholdH = threshold;

        //计算下ADC码值    NICK MARK
        //g_analogInfo.adcCodeH = ;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
        
        if (SENSOR_ON == g_analogInfo.state)
        {
            //给FUNC发信号量
            g_systemCfgBmp.bAnalogIn = true;
            g_bCmdPostSemToFunc = true;
        }
    }
}


/*********************************************************************************************
函 数 名: cmdAnalogInThresholdHQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInThresholdHQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_analogInfo.thresholdH);
    pData = (u8 *)&g_analogInfo.thresholdH;
    cmdFrameSend(CMD_ANALOGIN, ANALOGINCMD_THRESHOLDHQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdAnalogInThresholdLSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInThresholdLSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 threshold;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInThresholdVerify(cmdDataLen, pCmdData, (void *)&threshold))
    {
        g_analogInfo.thresholdL = threshold;

        //计算下ADC码值    NICK MARK
        //g_analogInfo.adcCodeL = ;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
        
        if (SENSOR_ON == g_analogInfo.state)
        {
            //给FUNC发信号量
            g_systemCfgBmp.bAnalogIn = true;
            g_bCmdPostSemToFunc = true;
        }
    }
}


/*********************************************************************************************
函 数 名: cmdAnalogInThresholdLQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInThresholdLQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_analogInfo.thresholdL);
    pData = (u8 *)&g_analogInfo.thresholdL;
    cmdFrameSend(CMD_ANALOGIN, ANALOGINCMD_THRESHOLDLQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdAnalogInResponseHSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInResponseHSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_analogInfo.responseH = response;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdAnalogInResponseHQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInResponseHQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_analogInfo.responseH);
    pData = (u8 *)&g_analogInfo.responseH;
    cmdFrameSend(CMD_ANALOGIN, ANALOGINCMD_RESPONSEHQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdAnalogInResponseLSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInResponseLSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_analogInfo.responseL = response;

        //存到EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdAnalogInResponseLQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInResponseLQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_analogInfo.responseL);
    pData = (u8 *)&g_analogInfo.responseL;
    cmdFrameSend(CMD_ANALOGIN, ANALOGINCMD_RESPONSELQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdAnalogInCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInCmdInit(void)
{
    memset(pAnalogInCmdFunc, 0, sizeof(pAnalogInCmdFunc));

    pAnalogInCmdFunc[ANALOGINCMD_STATE]       = cmdAnalogInStateSet;
    pAnalogInCmdFunc[ANALOGINCMD_STATEQ]      = cmdAnalogInStateQuery;
    pAnalogInCmdFunc[ANALOGINCMD_THRESHOLDH]  = cmdAnalogInThresholdHSet;
    pAnalogInCmdFunc[ANALOGINCMD_THRESHOLDHQ] = cmdAnalogInThresholdHQuery;
    pAnalogInCmdFunc[ANALOGINCMD_THRESHOLDL]  = cmdAnalogInThresholdLSet;
    pAnalogInCmdFunc[ANALOGINCMD_THRESHOLDLQ] = cmdAnalogInThresholdLQuery;
    pAnalogInCmdFunc[ANALOGINCMD_RESPONSEH]   = cmdAnalogInResponseHSet;
    pAnalogInCmdFunc[ANALOGINCMD_RESPONSEHQ]  = cmdAnalogInResponseHQuery;
    pAnalogInCmdFunc[ANALOGINCMD_RESPONSEL]   = cmdAnalogInResponseLSet;
    pAnalogInCmdFunc[ANALOGINCMD_RESPONSELQ]  = cmdAnalogInResponseLQuery;
}

            
/*********************************************************************************************
函 数 名: cmdAnalogInCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAnalogInCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < ANALOGINCMD_RESERVE) && 
        (pAnalogInCmdFunc[pCmdStackFrame->subType] != NULL) &&
        (SDMODEL_C23D == g_deviceInfo.sDevcModel))    //-D才有AI和OTP
    {    
        pAnalogInCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}
#endif


/*********************************************************************************************
函 数 名: cmdReportTorqueTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportTorqueTimerCB(void *timeOutPara)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_TORQUE;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_TORQUE]) + sizeof(index) + sizeof(chanNum);
    data[0] = chanNum;
    data[1] = index;
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_TORQUE];
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_TORQUE]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;
    
    //发送信号量给FUNC
    /*g_chanCfgBmp[chanNum].bReportTorque = true;
    g_bPerdPostSemToFunc = true;*/
    
    //继续开启定时器，直到收到关闭命令
    if (g_reportTimer[chanNum][REPTTYPE_TORQUE].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_TORQUE]);
    }
    else
    {
        //TO ADD NICK MARK
    }
}


/*********************************************************************************************
函 数 名: cmdReportCycleTimerCB;
实现功能: 循环模式下,运行状态中上传当前的循环序数
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportCycleTimerCB(void *timeOutPara)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_CYCLE;

    
    if (g_systemState.cycleNumCount[chanNum] != g_sensorData.reporterData[chanNum][REPTTYPE_CYCLE])
    {
        g_systemState.cycleNumCount[chanNum] = g_sensorData.reporterData[chanNum][REPTTYPE_CYCLE];
        
        dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_CYCLE]) + sizeof(index) + sizeof(chanNum);
        data[0] = chanNum;
        data[1] = index;
        pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_CYCLE];
        for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_CYCLE]);i++)
        {
            data[2 + i] = *pData++;
        }
        cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

        //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
        g_bPerdPostSemToCmd = true;
    }

    g_chanCfgBmp[chanNum].bCycleQry = true;
    g_bPerdPostSemToFunc = true; 
    
    //继续开启定时器
    if (g_reportTimer[chanNum][REPTTYPE_CYCLE].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_CYCLE]);
    }
}


/*********************************************************************************************
函 数 名: cmdReportSgAllTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportSgAllTimerCB(void *timeOutPara)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_DRVSGALL;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_DRVSGALL]) + sizeof(index) + sizeof(chanNum);
    data[0] = chanNum;
    data[1] = index;
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_DRVSGALL];
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_DRVSGALL]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;
    
    //继续开启定时器，直到收到关闭命令
    if (g_reportTimer[chanNum][REPTTYPE_DRVSGALL].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_DRVSGALL]);
    }   
}


/*********************************************************************************************
函 数 名: cmdReportSgSeTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportSgSeTimerCB(void *timeOutPara)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_DRVSGSE;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_DRVSGSE]) + sizeof(index) + sizeof(chanNum);
    data[0] = chanNum;
    data[1] = index;
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_DRVSGSE];
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_DRVSGSE]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;
    
    //继续开启定时器，直到收到关闭命令
    if (g_reportTimer[chanNum][REPTTYPE_DRVSGSE].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_DRVSGSE]);
    }   
}


/*********************************************************************************************
函 数 名: cmdReportDistanceTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportDistanceTimerCB(void *timeOutPara)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_DISTANCE;


    if (chanNum < DIST_SENSOR_NUM)
    {
        dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_DISTANCE]) + sizeof(index) + sizeof(chanNum);
        data[0] = chanNum;
        data[1] = index;
        pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_DISTANCE];
        for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_DISTANCE]);i++)
        {
            data[2 + i] = *pData++;
        }
        cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

        /*g_eventSrcBmp.bDistSen[chanNum] = true;

        g_bPerdPostSemToEvent = true;*/

        //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
        g_bPerdPostSemToCmd = true;
        
        //继续开启定时器，直到收到关闭命令
        if (g_reportTimer[chanNum][REPTTYPE_DISTANCE].bUsed)
        {
            servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_DISTANCE]);
        }  
    }
}


/*********************************************************************************************
函 数 名: cmdReportDriveMonitorTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdReportDriveMonitorTimerCB(void *timeOutPara)
{
    u8 chanNum = *(u8 *)timeOutPara;


    if (chanNum <= CH_MAX)
    {
        g_chanCfgBmp[chanNum].bDriverQuery = true;

        g_bPerdPostSemToFunc = true;

        if (g_reportTimer[chanNum][REPTTYPE_DRVMONT].bUsed)
        {
            servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_DRVMONT]);
        }
    }
}


/*********************************************************************************************
函 数 名: cmdReportAbsEncoderTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportAbsEncoderTimerCB(void *timeOutPara)
{
#if GELGOOG_SINANJU
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_ABSENC;


    if (chanNum < ABS_ENCODER_NUM)
    {
        dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_ABSENC]) + sizeof(index) + sizeof(chanNum);
        data[0] = chanNum;
        data[1] = index;
        pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_ABSENC];
        for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_ABSENC]);i++)
        {
            data[2 + i] = *pData++;
        }
        cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

        /*g_eventSrcBmp.bAbsEncoder[chanNum] = true;

        g_bPerdPostSemToEvent = true;*/

        //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
        g_bPerdPostSemToCmd = true;
        
        //继续开启定时器，直到收到关闭命令
        if (g_reportTimer[chanNum][REPTTYPE_ABSENC].bUsed)
        {
            servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_ABSENC]);
        }  
    }
#endif
}


/*********************************************************************************************
函 数 名: cmdReportOutNumTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportOutNumTimerCB(void *timeOutPara)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_OUTNUM;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_OUTNUM]) + sizeof(index) + sizeof(chanNum);
    data[0] = chanNum;
    data[1] = index;
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_OUTNUM];
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_OUTNUM]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;
    
    //继续开启定时器，直到收到关闭命令
    if (g_reportTimer[chanNum][REPTTYPE_OUTNUM].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_OUTNUM]);
    }   
}


/*********************************************************************************************
函 数 名: cmdReportStepsTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportStepsTimerCB(void *timeOutPara)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_STEPS;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_STEPS]) + sizeof(index) + sizeof(chanNum);
    data[0] = chanNum;
    data[1] = index;
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_STEPS];
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_STEPS]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;
    
    //继续开启定时器，直到收到关闭命令
    if (g_reportTimer[chanNum][REPTTYPE_STEPS].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_STEPS]);
    }
}


/*********************************************************************************************
函 数 名: cmdReportVelorityTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportVelorityTimerCB(void *timeOutPara)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_VELOCITY;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_VELOCITY]) + sizeof(index) + sizeof(chanNum);
    data[0] = chanNum;
    data[1] = index;
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_VELOCITY];
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_VELOCITY]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;
    
    //继续开启定时器，直到收到关闭命令
    if (g_reportTimer[chanNum][REPTTYPE_VELOCITY].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_VELOCITY]);
    }
}



/*********************************************************************************************
函 数 名: cmdReportStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8   index;    //上报器的下标改为U8    CJ 2017.06.07 Modify
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfReportStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_reportInfo.report[i].state[index] = state;

                //改变定时器状态
                if (SENSOR_ON == state)
                {
                    switch ((ReportTypeEnum)index)
                    {
                        case REPTTYPE_TORQUE:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_TORQUE]);

                            //能耗和SGALL以及SGSE三个共用监测
                            if (!g_reportTimer[i][REPTTYPE_DRVMONT].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_DRVMONT]);
                            }
                          break;

                        case REPTTYPE_CYCLE:
                            if (WTWORKMODE_CYCLE == g_waveTable[i][g_systemState.outpWaveTable[i]].waveWorkMode)
                            {
                                g_waveTable[i][g_systemState.outpWaveTable[i]].bCycleQuery = true;

                                if (MTSTATE_OUTPUTING == g_waveTable[i][g_systemState.outpWaveTable[i]].waveState)
                                {
                                    g_systemState.cycleNumCount[i] = g_sensorData.reporterData[i][REPTTYPE_CYCLE];
                                    servStimerAdd(&g_reportTimer[i][REPTTYPE_CYCLE]);
                                }
                            }
                          break;

                        case REPTTYPE_DRVSGALL:  
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_DRVSGALL]);

                            //能耗和SGALL以及SGSE三个共用监测
                            if (!g_reportTimer[i][REPTTYPE_DRVMONT].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_DRVMONT]);
                            }
                          break;

                        case REPTTYPE_DRVSGSE:  
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_DRVSGSE]);

                            //能耗和SGALL以及SGSE三个共用监测
                            if (!g_reportTimer[i][REPTTYPE_DRVMONT].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_DRVMONT]);
                            }
                          break;

                        case REPTTYPE_DISTANCE:  
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_DISTANCE]);
                          break;

                        case REPTTYPE_ABSENC:  
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_ABSENC]);
                          break;
                          
                        case REPTTYPE_OUTNUM:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_OUTNUM]);
                          break;

                        case REPTTYPE_STEPS:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_STEPS]);
                          break;

                        case REPTTYPE_VELOCITY:  
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_VELOCITY]);
                          break;

                        default:
                          break;
                    }
                }
                else
                {
                    switch ((ReportTypeEnum)index)
                    {
                        case REPTTYPE_TORQUE:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_TORQUE]);

                            //能耗和SGALL以及SGSE三个共用监测
                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_TORQUE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_DRVSGALL]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_DRVSGSE]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_DRVMONT]);
                            }
                          break;

                        case REPTTYPE_CYCLE:
                            g_waveTable[i][g_systemState.outpWaveTable[i]].bCycleQuery = false;
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_CYCLE]);
                            g_systemState.cycleNumCount[i] = 0;
                          break;
                          
                        case REPTTYPE_DRVSGALL:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_DRVSGALL]);

                            //能耗和SGALL以及SGSE三个共用监测
                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_TORQUE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_DRVSGALL]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_DRVSGSE]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_DRVMONT]);
                            }
                          break;
                          
                        case REPTTYPE_DRVSGSE:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_DRVSGSE]);

                            //能耗和SGALL以及SGSE三个共用监测
                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_TORQUE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_DRVSGALL]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_DRVSGSE]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_DRVMONT]);
                            }
                          break;
                          
                        case REPTTYPE_DISTANCE:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_DISTANCE]);
                          break;

                        case REPTTYPE_ABSENC:  
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_ABSENC]);
                          break;
                          
                        case REPTTYPE_OUTNUM:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_OUTNUM]);
                          break;

                        case REPTTYPE_STEPS:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_STEPS]);
                          break;

                        case REPTTYPE_VELOCITY:  
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_VELOCITY]);
                          break;

                        default:
                          break;
                    }
                }
            }
        }

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdReportStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportStateQuery(u8 cmdDataLen, u8 *pCmdData)
{ 
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    u8 index = *(u8*)pCmdData;


    cmdDataLen -= 1;
    dataLen = sizeof(g_reportInfo.report[CH1].state[index]) + sizeof(chanNum) + sizeof(index);
    if ((sizeof(index) == cmdDataLen) && (index < REPTTYPE_RESERVE))
    {
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_reportInfo.report[chanNum].state[index];
            data[0] = chanNum;
            data[1] = index;
            for (i = 0;i < sizeof(g_reportInfo.report[chanNum].state[index]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_REPORT, REPORTCMD_STATEQ, dataLen, data);
        }
        else
        {
            for (j = 0;j < CH_TOTAL;j++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[j][0]) ||
                    (chanNum == g_systemInfo.group[j][1]))
                {
                    pData = (u8 *)&g_reportInfo.report[j].state[index];
                    data[0] = j;
                    data[1] = index;
                    for (i = 0;i < sizeof(g_reportInfo.report[j].state[index]);i++)
                    {
                        data[2 + i] = *pData++;
                    }
                    cmdFrameSend(CMD_REPORT, REPORTCMD_STATEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdReportPeriodSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32  reptPeriod;
    u32  periodMin;
    u8   index;    //上报器的下标改为U8    CJ 2017.06.07 Modify
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfReportPeriodVerify(cmdDataLen, pCmdData, (void *)&reptPeriod, &index))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_reportInfo.report[i].period[index] = reptPeriod;

                //重新设置周期
                switch ((ReportTypeEnum)index)
                {
                    case REPTTYPE_TORQUE:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_TORQUE], reptPeriod, cmdReportTorqueTimerCB);
                        
                        //以最小周期设置监控
                        if (g_reportInfo.report[i].period[REPTTYPE_DRVSGALL] < g_reportInfo.report[i].period[REPTTYPE_DRVSGSE])
                        {
                            periodMin = g_reportInfo.report[i].period[REPTTYPE_DRVSGALL];
                        }
                        else
                        {
                            periodMin = g_reportInfo.report[i].period[REPTTYPE_DRVSGSE];
                        }
                        if (periodMin > reptPeriod)
                        {
                            periodMin = reptPeriod;
                        }
                        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVMONT], periodMin, cmdReportDriveMonitorTimerCB);
                      break;

                    case REPTTYPE_CYCLE:
                        //
                      break;

                    case REPTTYPE_DRVSGALL:  
                        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVSGALL], reptPeriod, cmdReportSgAllTimerCB);
                        
                        //以最小周期设置监控
                        if (g_reportInfo.report[i].period[REPTTYPE_TORQUE] < g_reportInfo.report[i].period[REPTTYPE_DRVSGSE])
                        {
                            periodMin = g_reportInfo.report[i].period[REPTTYPE_TORQUE];
                        }
                        else
                        {
                            periodMin = g_reportInfo.report[i].period[REPTTYPE_DRVSGSE];
                        }
                        if (periodMin > reptPeriod)
                        {
                            periodMin = reptPeriod;
                        }
                        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVMONT], periodMin, cmdReportDriveMonitorTimerCB);
                      break;

                    case REPTTYPE_DRVSGSE: 
                        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVSGSE], reptPeriod, cmdReportSgSeTimerCB);
                        
                        //以最小周期设置监控
                        if (g_reportInfo.report[i].period[REPTTYPE_DRVSGALL] < g_reportInfo.report[i].period[REPTTYPE_TORQUE])
                        {
                            periodMin = g_reportInfo.report[i].period[REPTTYPE_DRVSGALL];
                        }
                        else
                        {
                            periodMin = g_reportInfo.report[i].period[REPTTYPE_TORQUE];
                        }
                        if (periodMin > reptPeriod)
                        {
                            periodMin = reptPeriod;
                        }
                        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVMONT], periodMin, cmdReportDriveMonitorTimerCB); 
                      break;
                      
                    case REPTTYPE_DISTANCE:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_DISTANCE], reptPeriod, cmdReportDistanceTimerCB);
                      break;

                    case REPTTYPE_ABSENC:  
                        servStimerInit(&g_reportTimer[i][REPTTYPE_ABSENC], reptPeriod, cmdReportAbsEncoderTimerCB);
                      break;
                      
                    case REPTTYPE_OUTNUM:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_OUTNUM], reptPeriod, cmdReportOutNumTimerCB);
                      break;

                    case REPTTYPE_STEPS:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_STEPS], reptPeriod, cmdReportStepsTimerCB);
                      break;

                    case REPTTYPE_VELOCITY:  
                        servStimerInit(&g_reportTimer[i][REPTTYPE_VELOCITY], reptPeriod, cmdReportVelorityTimerCB);
                      break;

                    default:
                      break;
                }
            }
        }

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdReportPeriodQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportPeriodQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    u8 index = *(u8*)pCmdData;


    cmdDataLen -= 1;
    dataLen = sizeof(g_reportInfo.report[CH1].period[index]) + sizeof(chanNum) + sizeof(index);
    if ((sizeof(index) == cmdDataLen) && (index < REPTTYPE_RESERVE))
    {
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_reportInfo.report[chanNum].period[index];
            data[0] = chanNum;
            data[1] = index;
            for (i = 0;i < sizeof(g_reportInfo.report[chanNum].period[index]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_REPORT, REPORTCMD_PERIODQ, dataLen, data);
        }
        else
        {
            for (j = 0;j< CH_TOTAL;j++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[j][0]) ||
                    (chanNum == g_systemInfo.group[j][1]))
                {
                    pData = (u8 *)&g_reportInfo.report[j].period[index];
                    data[0] = j;
                    data[1] = index;
                    for (i = 0;i < sizeof(g_reportInfo.report[j].period[index]);i++)
                    {
                        data[2 + i] = *pData++;
                    }
                    cmdFrameSend(CMD_REPORT, REPORTCMD_PERIODQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdReportDataQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportDataQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    u8 index = *(u8*)pCmdData;


    cmdDataLen -= 1;
    dataLen = sizeof(g_sensorData.reporterData[CH1][index]) + sizeof(chanNum) + sizeof(index);
    if ((sizeof(index) == cmdDataLen) && (index < REPTTYPE_RESERVE))
    {
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_sensorData.reporterData[chanNum][index];
            data[0] = chanNum;
            data[1] = index;
            for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][index]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);
        }
        else
        {
            for (j = 0;j < CH_TOTAL;j++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[j][0]) ||
                    (chanNum == g_systemInfo.group[j][1]))
                {
                    pData = (u8 *)&g_sensorData.reporterData[j][index];
                    data[0] = j;
                    data[1] = index;
                    for (i = 0;i < sizeof(g_sensorData.reporterData[j][index]);i++)
                    {
                        data[2 + i] = *pData++;
                    }
                    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdReportCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportCmdInit(void)
{
    memset(pReportCmdFunc, 0, sizeof(pReportCmdFunc));

    pReportCmdFunc[REPORTCMD_STATE]   = cmdReportStateSet;
    pReportCmdFunc[REPORTCMD_STATEQ]  = cmdReportStateQuery;
    pReportCmdFunc[REPORTCMD_PERIOD]  = cmdReportPeriodSet;
    pReportCmdFunc[REPORTCMD_PERIODQ] = cmdReportPeriodQuery;
    pReportCmdFunc[REPORTCMD_DATAQ]   = cmdReportDataQuery;
}

            
/*********************************************************************************************
函 数 名: cmdReportCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdReportCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < REPORTCMD_RESERVE) && (pReportCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pReportCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


#if 0
#endif


/*********************************************************************************************
函 数 名: cmdSensorUartBaudSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartBaudSet(u8 cmdDataLen, u8 *pCmdData)
{
    UartBaudEnum baud;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //进行参数验证
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartBaudVerify(cmdDataLen, pCmdData, (void *)&baud)))
    {
        g_sensorUartIntfc[index].baud = baud;
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartBaudQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartBaudQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 index = *pCmdData++;

    
    if (index < UARTNUM_RESERVE)
    {
        dataLen = sizeof(g_sensorUartIntfc[index].baud) + sizeof(index);
        
        pData = (u8 *)&g_sensorUartIntfc[index].baud;
        data[0] = index;
        for (i = 0;i < dataLen;i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_BAUDQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartWordLenSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartWordLenSet(u8 cmdDataLen, u8 *pCmdData)
{
    WordLenEnum wordLen;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //进行参数验证
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartWordLenVerify(cmdDataLen, pCmdData, (void *)&wordLen)))
    {
        g_sensorUartIntfc[index].wordLen = wordLen;
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartWordLenQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartWordLenQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 index = *pCmdData++;

    
    if (index < UARTNUM_RESERVE)
    {
        dataLen = sizeof(g_sensorUartIntfc[index].wordLen) + sizeof(index);
        
        pData = (u8 *)&g_sensorUartIntfc[index].wordLen;
        data[0] = index;
        for (i = 0;i < dataLen;i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_WORDLENQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartFlowCtlSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartFlowCtlSet(u8 cmdDataLen, u8 *pCmdData)
{
    FlowCtlEnum flowCtl;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //进行参数验证
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartFlowCtlVerify(cmdDataLen, pCmdData, (void *)&flowCtl)))
    {
        g_sensorUartIntfc[index].flowCtl = flowCtl;
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartFlowCtlQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartFlowCtlQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 index = *pCmdData++;

    
    if (index < UARTNUM_RESERVE)
    {
        dataLen = sizeof(g_sensorUartIntfc[index].flowCtl) + sizeof(index);
        
        pData = (u8 *)&g_sensorUartIntfc[index].flowCtl;
        data[0] = index;
        for (i = 0;i < dataLen;i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_FLOWCTLQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartParitySet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartParitySet(u8 cmdDataLen, u8 *pCmdData)
{
    ParityEnum parity;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //进行参数验证
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartParityVerify(cmdDataLen, pCmdData, (void *)&parity)))
    {
        g_sensorUartIntfc[index].parity = parity;
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartParityQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartParityQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 index = *pCmdData++;

    
    if (index < UARTNUM_RESERVE)
    {
        dataLen = sizeof(g_sensorUartIntfc[index].parity) + sizeof(index);
        
        pData = (u8 *)&g_sensorUartIntfc[index].parity;
        data[0] = index;
        for (i = 0;i < dataLen;i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_PARITYQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartStopBitSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartStopBitSet(u8 cmdDataLen, u8 *pCmdData)
{
    StopBitEnum stopBit;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //进行参数验证
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartStopBitVerify(cmdDataLen, pCmdData, (void *)&stopBit)))
    {
        g_sensorUartIntfc[index].stopBit = stopBit;
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartStopBitQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartStopBitQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 index = *pCmdData++;

    
    if (index < UARTNUM_RESERVE)
    {
        dataLen = sizeof(g_sensorUartIntfc[index].stopBit) + sizeof(index);
        
        pData = (u8 *)&g_sensorUartIntfc[index].stopBit;
        data[0] = index;
        for (i = 0;i < dataLen;i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STOPBITQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartStopBitSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartApply(u8 cmdDataLen, u8 *pCmdData)
{
    u8 index = *pCmdData++;
    

    //进行参数验证
    if (index < UARTNUM_RESERVE)
    {
        memcpy(&g_sensorUart.uartIntfc[index], &g_sensorUartIntfc[index], sizeof(UartIntfcStruct));
 
        servStimerAdd(&g_paraSaveTimer);

        //给FUNC发信号量
        if (UARTNUM_U1 == index)
        {
            g_systemCfgBmp.bSensorUart1 = true;
        }
        else
        {
            g_systemCfgBmp.bSensorUart2 = true;
        }
        g_bCmdPostSemToFunc = true;
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartStateSet(u8 cmdDataLen, u8 *pCmdData)
{    
    SensorStateEnum state;
    u8 uartIndex;
    u8 sensorIndex;
    

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartStateVerify(cmdDataLen, pCmdData, (void *)&state, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].state = state;

        servStimerAdd(&g_paraSaveTimer);

        if (SENSOR_ON == state)
        {
            if (UARTNUM_U1 == uartIndex)
            {
                servSensor1UartReciveOn(g_sensorUart.sensor[UARTNUM_U1][sensorIndex], (SensorNumEnum)sensorIndex);
            }
#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4轴和10轴只支持1路
            else
            {
                servSensor2UartReciveOn(g_sensorUart.sensor[UARTNUM_U2][sensorIndex], (SensorNumEnum)sensorIndex);
            }
#endif
        }
        else
        {
            if (UARTNUM_U1 == uartIndex)
            {
#ifdef PROJECT_GELGOOG

                //全关闭了就把接收也关闭了
                if ((SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U1][SENSOR_S1].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U1][SENSOR_S2].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U1][SENSOR_S3].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U1][SENSOR_S4].state))
#endif
                {
                    servSensor1UartReciveOff();
                }
            }
#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4轴和10轴只支持1路
            else
            {
#ifdef PROJECT_GELGOOG

                //全关闭了就把接收也关闭了
                if ((SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S1].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S3].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S4].state))
#endif
                {
                    servSensor2UartReciveOff();
                }
            }
#endif
        }
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 uartIndex = *pCmdData++;
    u8 sensorIndex = *pCmdData++;

    
    if ((sensorIndex < SENSOR_RESERVE) && (uartIndex < UARTNUM_RESERVE))
    {
        dataLen = sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].state) + sizeof(uartIndex) + sizeof(sensorIndex);
        
        pData = (u8 *)&g_sensorUart.sensor[uartIndex][sensorIndex].state;
        data[0] = uartIndex;
        data[1] = sensorIndex;
        for (i = 0;i < sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].state);i++)
        {
            data[2 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STATEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartSofSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartSofSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 sof;
    u8 uartIndex;
    u8 sensorIndex;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartSofVerify(cmdDataLen, pCmdData, (void *)&sof, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].SOF = sof;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartSofQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartSofQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 uartIndex = *pCmdData++;
    u8 sensorIndex = *pCmdData++;

    
    if ((sensorIndex < SENSOR_RESERVE) && (uartIndex < UARTNUM_RESERVE))
    {
        dataLen = sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].SOF) + sizeof(uartIndex) + sizeof(sensorIndex);
        
        pData = (u8 *)&g_sensorUart.sensor[uartIndex][sensorIndex].SOF;
        data[0] = uartIndex;
        data[1] = sensorIndex;
        for (i = 0;i < sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].SOF);i++)
        {
            data[2 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_SOFQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartFrameLenSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartFrameLenSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 frameLen;
    u8 uartIndex;
    u8 sensorIndex;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartFrameLenVerify(cmdDataLen, pCmdData, (void *)&frameLen, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].frameLen = frameLen;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartFrameLenQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartFrameLenQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 uartIndex = *pCmdData++;
    u8 sensorIndex = *pCmdData++;

    
    if ((sensorIndex < SENSOR_RESERVE) && (uartIndex < UARTNUM_RESERVE))
    {
        dataLen = sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].frameLen) + sizeof(uartIndex) + sizeof(sensorIndex);
        
        pData = (u8 *)&g_sensorUart.sensor[uartIndex][sensorIndex].frameLen;
        data[0] = uartIndex;
        data[1] = sensorIndex;
        for (i = 0;i < sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].frameLen);i++)
        {
            data[2 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_FRAMELENQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartRecvNumSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartRecvNumSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 recvNum;
    u8 uartIndex;
    u8 sensorIndex;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartRecvNumVerify(cmdDataLen, pCmdData, (void *)&recvNum, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].recvNum = recvNum;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartRecvNumQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartRecvNumQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 uartIndex = *pCmdData++;
    u8 sensorIndex = *pCmdData++;

    
    if ((sensorIndex < SENSOR_RESERVE) && (uartIndex < UARTNUM_RESERVE))
    {
        dataLen = sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].recvNum) + sizeof(uartIndex) + sizeof(sensorIndex);
        
        pData = (u8 *)&g_sensorUart.sensor[uartIndex][sensorIndex].recvNum;
        data[0] = uartIndex;
        data[1] = sensorIndex;
        for (i = 0;i < sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].recvNum);i++)
        {
            data[2 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_RECVNUMQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartSwTimeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartSwTimeSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 swTime;
    u8  uartIndex;
    u8  sensorIndex;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartSwTimeVerify(cmdDataLen, pCmdData, (void *)&swTime, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].swTime = swTime;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartSwTimeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartSwTimeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 uartIndex = *pCmdData++;
    u8 sensorIndex = *pCmdData++;

    
    if ((sensorIndex < SENSOR_RESERVE) && (uartIndex < UARTNUM_RESERVE))
    {
        dataLen = sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].swTime) + sizeof(uartIndex) + sizeof(sensorIndex);
        
        pData = (u8 *)&g_sensorUart.sensor[uartIndex][sensorIndex].swTime;
        data[0] = uartIndex;
        data[1] = sensorIndex;
        for (i = 0;i < sizeof(g_sensorUart.sensor[uartIndex][sensorIndex].swTime);i++)
        {
            data[2 + i] = *pData++;
        }
        cmdFrameSend(CMD_SENSORUART, SNUARTCMD_SWTIMEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartDataQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartDataQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen, i;
    u8 data[6];
    u8 *pData;
    u8 frameNum;
    u8 frameLen;
    SensorUartPhyFrameStruct *pUartPhyFrame = NULL;
    u8 uartIndex = *pCmdData++;
    u8 sensorIndex = *pCmdData++;

#if 1    //CJ 2018.07.10 Modify In Flex
    bool bRemainder = false;
#endif

    
    if ((sensorIndex < SENSOR_RESERVE) && (uartIndex < UARTNUM_RESERVE))
    {
        //有完整的帧才处理
        if (g_senUartRxBuffer[uartIndex][sensorIndex].frameCount > 0)
        {
            pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[uartIndex][sensorIndex]);

            if (pUartPhyFrame->frameLen > 0)
            {
                frameLen = pUartPhyFrame->frameLen;

#if 1    //CJ 2018.07.10 Modify In Flex
                frameNum = frameLen / UART_SENSOR_FRAME_DATA_LEN_MAX;
                
                if ((frameLen % UART_SENSOR_FRAME_DATA_LEN_MAX) != 0)
                {
                    bRemainder = true;    //多发一帧，最后一帧不满UART_SENSOR_FRAME_DATA_LEN_MAX个字节
                    frameNum++;
                }
                
                pData = (u8 *)&pUartPhyFrame->SOF;
                
                for (i = 1;i < frameNum;i++)
                {
                    dataLen = UART_SENSOR_FRAME_DATA_LEN_MAX;
                    memcpy(&data[UART_SENSOR_FRAME_DATA_INDEX], pData, dataLen);
                    pData += dataLen;

                    data[0] = uartIndex;      //串口号
                    data[1] = sensorIndex;    //传感器号

                    //高4bit代表分子，所以左移4bit，低4bit代表分母
                    //总共可以发送16 x 5Bytes = 80Bytes(640bits)的数据，目前没有大数据，所以不做frameNum大小的判断了
                    data[2] = FRACTION(i, frameNum);
                    dataLen += UART_SENSOR_FRAME_DATA_INDEX;    //加2是为了算上data[0]、data[1]和data[2]
                    
                    cmdFrameSend(CMD_SENSORUART, SNUARTCMD_DATAQ, dataLen, data);
                }

                if (bRemainder)
                {
                    dataLen = frameLen % UART_SENSOR_FRAME_DATA_LEN_MAX;
                    
                    memcpy(&data[UART_SENSOR_FRAME_DATA_INDEX], pData, dataLen);

                    data[0] = uartIndex;      //串口号
                    data[1] = sensorIndex;    //传感器号

                    //高4bit代表分子，所以左移4bit，低4bit代表分母
                    //总共可以发送16 x 5Bytes = 80Bytes(640bits)的数据，目前没有大数据，所以不做frameNum大小的判断了
                    data[2] = FRACTION(i, frameNum);
                    dataLen += UART_SENSOR_FRAME_DATA_INDEX;    //加2是为了算上data[0]、data[1]和data[2]
                    
                    cmdFrameSend(CMD_SENSORUART, SNUARTCMD_DATAQ, dataLen, data);
                }
                
#else

                if ((frameLen % UART_SENSOR_FRAME_DATA_LEN_MAX) != 0)
                {
                    frameNum = frameLen / UART_SENSOR_FRAME_DATA_LEN_MAX + 1;    //多发一帧，最后一帧不满UART_SENSOR_FRAME_DATA_LEN_MAX个字节
                }
                else
                {
                    frameNum = frameLen / UART_SENSOR_FRAME_DATA_LEN_MAX;
                }
                pData = (u8 *)&pUartPhyFrame->SOF;
                
                for (i = 1;i <= frameNum;i++)
                {
                    if (i == frameNum)
                    {
                        dataLen = frameLen % UART_SENSOR_FRAME_DATA_LEN_MAX;    //发送剩余的
                    }
                    else
                    {
                        dataLen = UART_SENSOR_FRAME_DATA_LEN_MAX;            
                    }
                    memcpy(&data[UART_SENSOR_FRAME_DATA_INDEX], pData, dataLen);
                    pData += dataLen;

                    data[0] = uartIndex;      //串口号
                    data[1] = sensorIndex;    //传感器号

                    //高4bit代表分子，所以左移4bit，低4bit代表分母
                    //总共可以发送16 x 5Bytes = 80Bytes(640bits)的数据，目前没有大数据，所以不做frameNum大小的判断了
                    data[2] = FRACTION(i, frameNum);
                    dataLen += UART_SENSOR_FRAME_DATA_INDEX;    //加2是为了算上data[0]、data[1]和data[2]
                    
                    cmdFrameSend(CMD_SENSORUART, SNUARTCMD_DATAQ, dataLen, data);
                }
#endif

                servDequeue(&g_senUartRxBuffer[uartIndex][sensorIndex], pUartPhyFrame->frameLen); //处理完成后出队
                g_senUartRxBuffer[uartIndex][sensorIndex].frameCount--;
            }
        }
        else
        {
            //返回0
            data[0] = uartIndex;      //串口号
            data[1] = sensorIndex;
            data[2] = 0;
            cmdFrameSend(CMD_SENSORUART, SNUARTCMD_DATAQ, UART_SENSOR_FRAME_DATA_INDEX, data);
        }
    }
}


/*********************************************************************************************
函 数 名: cmdSensorUartCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartCmdInit(void)
{
    memset(pSensorUartCmdFunc, 0, sizeof(pSensorUartCmdFunc));

    pSensorUartCmdFunc[SNUARTCMD_BAUD]      = cmdSensorUartBaudSet;
    pSensorUartCmdFunc[SNUARTCMD_BAUDQ]     = cmdSensorUartBaudQuery;
    pSensorUartCmdFunc[SNUARTCMD_WORDLEN]   = cmdSensorUartWordLenSet;
    pSensorUartCmdFunc[SNUARTCMD_WORDLENQ]  = cmdSensorUartWordLenQuery;
    pSensorUartCmdFunc[SNUARTCMD_FLOWCTL]   = cmdSensorUartFlowCtlSet;
    pSensorUartCmdFunc[SNUARTCMD_FLOWCTLQ]  = cmdSensorUartFlowCtlQuery;
    pSensorUartCmdFunc[SNUARTCMD_PARITY]    = cmdSensorUartParitySet;
    pSensorUartCmdFunc[SNUARTCMD_PARITYQ]   = cmdSensorUartParityQuery;
    pSensorUartCmdFunc[SNUARTCMD_STOPBIT]   = cmdSensorUartStopBitSet;
    pSensorUartCmdFunc[SNUARTCMD_STOPBITQ]  = cmdSensorUartStopBitQuery;
    pSensorUartCmdFunc[SNUARTCMD_APPLY]     = cmdSensorUartApply;

    pSensorUartCmdFunc[SNUARTCMD_STATE]     = cmdSensorUartStateSet;
    pSensorUartCmdFunc[SNUARTCMD_STATEQ]    = cmdSensorUartStateQuery;
    pSensorUartCmdFunc[SNUARTCMD_SOF]       = cmdSensorUartSofSet;
    pSensorUartCmdFunc[SNUARTCMD_SOFQ]      = cmdSensorUartSofQuery;
    pSensorUartCmdFunc[SNUARTCMD_FRAMELEN]  = cmdSensorUartFrameLenSet;
    pSensorUartCmdFunc[SNUARTCMD_FRAMELENQ] = cmdSensorUartFrameLenQuery;
    pSensorUartCmdFunc[SNUARTCMD_RECVNUM]   = cmdSensorUartRecvNumSet;
    pSensorUartCmdFunc[SNUARTCMD_RECVNUMQ]  = cmdSensorUartRecvNumQuery; 
    pSensorUartCmdFunc[SNUARTCMD_SWTIME]    = cmdSensorUartSwTimeSet;
    pSensorUartCmdFunc[SNUARTCMD_SWTIMEQ]   = cmdSensorUartSwTimeQuery;
    pSensorUartCmdFunc[SNUARTCMD_DATAQ]     = cmdSensorUartDataQuery;
}

            
/*********************************************************************************************
函 数 名: cmdSensorUartCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdSensorUartCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < SNUARTCMD_RESERVE) && (pSensorUartCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pSensorUartCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


#if GELGOOG_SINANJU

/*********************************************************************************************
函 数 名: cmdAbsEncAlarmStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8 index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAbsEncAlarmStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        g_sensorAlarm.encAlarm[index].state = state;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < ABS_ENCODER_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.encAlarm[index].state) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.encAlarm[index].state;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.encAlarm[index].state);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCALARM, ENCALARMCMD_STATEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmZeroPostSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmZeroPostSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum zeroPost;
    u8 index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAbsEncAlarmZeroPostVerify(cmdDataLen, pCmdData, (void *)&zeroPost, &index))
    {
        g_sensorAlarm.encAlarm[index].zeroPost = zeroPost;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmZeroPostQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmZeroPostQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < ABS_ENCODER_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.encAlarm[index].zeroPost) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.encAlarm[index].zeroPost;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.encAlarm[index].zeroPost);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCALARM, ENCALARMCMD_ZEROPOSTQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmUplimitSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmUplimitSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 upLimit;
    u8  index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAbsEncAlarmLimitVerify(cmdDataLen, pCmdData, (void *)&upLimit, &index))
    {
        g_sensorAlarm.encAlarm[index].upLimit = upLimit;
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmUplimitQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmUplimitQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < ABS_ENCODER_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.encAlarm[index].upLimit) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.encAlarm[index].upLimit;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.encAlarm[index].upLimit);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCALARM, ENCALARMCMD_UPLIMITQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmDownlimitSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmDownlimitSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 dnLimit;
    u8  index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAbsEncAlarmLimitVerify(cmdDataLen, pCmdData, (void *)&dnLimit, &index))
    {
        g_sensorAlarm.encAlarm[index].dnLimit = dnLimit;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmDownlimitQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmDownlimitQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < ABS_ENCODER_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.encAlarm[index].dnLimit) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.encAlarm[index].dnLimit;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.encAlarm[index].dnLimit);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCALARM, ENCALARMCMD_DOWNLIMITQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmZeroValueSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmZeroValueSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 zeroValue;
    u8  index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAbsEncAlarmLimitVerify(cmdDataLen, pCmdData, (void *)&zeroValue, &index))
    {
        g_sensorAlarm.encAlarm[index].zeroValue = zeroValue;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmZeroValueQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmZeroValueQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < ABS_ENCODER_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.encAlarm[index].zeroValue) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.encAlarm[index].zeroValue;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.encAlarm[index].zeroValue);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCALARM, ENCALARMCMD_ZEROVALUEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmResponseSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;
    

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfAbsEncAlarmResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_sensorAlarm.encAlarmResponse = response;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmResponseQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;


    dataLen = sizeof(g_sensorAlarm.encAlarmResponse);
    pData = (u8 *)&g_sensorAlarm.encAlarmResponse;
    
    cmdFrameSend(CMD_ENCALARM, ENCALARMCMD_RESPONSEQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdAbsEncAlarmCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmCmdInit(void)
{
    memset(pAbsEncAlarmCmdFunc, 0, sizeof(pAbsEncAlarmCmdFunc));

    pAbsEncAlarmCmdFunc[ENCALARMCMD_STATE]  = cmdAbsEncAlarmStateSet;
    pAbsEncAlarmCmdFunc[ENCALARMCMD_STATEQ] = cmdAbsEncAlarmStateQuery;
    
    pAbsEncAlarmCmdFunc[ENCALARMCMD_UPLIMIT]    = cmdAbsEncAlarmUplimitSet;
    pAbsEncAlarmCmdFunc[ENCALARMCMD_UPLIMITQ]   = cmdAbsEncAlarmUplimitQuery;
    pAbsEncAlarmCmdFunc[ENCALARMCMD_DOWNLIMIT]  = cmdAbsEncAlarmDownlimitSet;
    pAbsEncAlarmCmdFunc[ENCALARMCMD_DOWNLIMITQ] = cmdAbsEncAlarmDownlimitQuery;
    
    pAbsEncAlarmCmdFunc[ENCALARMCMD_ZEROVALUE]  = cmdAbsEncAlarmZeroValueSet;
    pAbsEncAlarmCmdFunc[ENCALARMCMD_ZEROVALUEQ] = cmdAbsEncAlarmZeroValueQuery;
    pAbsEncAlarmCmdFunc[ENCALARMCMD_ZEROPOST]   = cmdAbsEncAlarmZeroPostSet;
    pAbsEncAlarmCmdFunc[ENCALARMCMD_ZEROPOSTQ]  = cmdAbsEncAlarmZeroPostQuery;
    
    pAbsEncAlarmCmdFunc[ENCALARMCMD_RESPONSE]  = cmdAbsEncAlarmResponseSet;
    pAbsEncAlarmCmdFunc[ENCALARMCMD_RESPONSEQ] = cmdAbsEncAlarmResponseQuery;
}

            
/*********************************************************************************************
函 数 名: cmdAbsEncAlarmCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdAbsEncAlarmCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < ENCALARMCMD_RESERVE) && (pAbsEncAlarmCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pAbsEncAlarmCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}

#if 0
#endif

/*********************************************************************************************
函 数 名: cmdDistAlarmStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarmStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8 index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDistAlarmStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        g_sensorAlarm.distAlarm[index].state = state;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDistAlarmStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarmStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIST_SENSOR_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.distAlarm[index].state) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.distAlarm[index].state;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.distAlarm[index].state);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DISTALARM, DISTALARMCMD_STATEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdDistAlarm1DistSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarm1DistSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 alarm1Dist;
    u8  index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDistAlarmDistVerify(cmdDataLen, pCmdData, (void *)&alarm1Dist, &index))
    {
        g_sensorAlarm.distAlarm[index].alarm1Dist = alarm1Dist;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDistAlarm1DistQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarm1DistQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIST_SENSOR_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.distAlarm[index].alarm1Dist) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.distAlarm[index].alarm1Dist;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.distAlarm[index].alarm1Dist);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DISTALARM, DISTALARMCMD_ALARM1DISTQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdDistAlarm2DistSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarm2DistSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 alarm2Dist;
    u8  index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDistAlarmDistVerify(cmdDataLen, pCmdData, (void *)&alarm2Dist, &index))
    {
        g_sensorAlarm.distAlarm[index].alarm2Dist = alarm2Dist;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDistAlarm2DistQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarm2DistQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIST_SENSOR_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.distAlarm[index].alarm2Dist) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.distAlarm[index].alarm2Dist;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.distAlarm[index].alarm2Dist);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DISTALARM, DISTALARMCMD_ALARM2DISTQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdDistAlarm3DistSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarm3DistSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 alarm3Dist;
    u8  index;

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDistAlarmDistVerify(cmdDataLen, pCmdData, (void *)&alarm3Dist, &index))
    {
        g_sensorAlarm.distAlarm[index].alarm3Dist = alarm3Dist;
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDistAlarm1DistQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarm3DistQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIST_SENSOR_NUM)
    {
        dataLen = sizeof(g_sensorAlarm.distAlarm[index].alarm3Dist) + sizeof(index);
    
        pData = (u8 *)&g_sensorAlarm.distAlarm[index].alarm3Dist;
        data[0] = index;
        for (i = 0;i < sizeof(g_sensorAlarm.distAlarm[index].alarm3Dist);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DISTALARM, DISTALARMCMD_ALARM3DISTQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdDistAlarmCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarmCmdInit(void)
{
    memset(pDistAlarmCmdFunc, 0, sizeof(pDistAlarmCmdFunc));

    pDistAlarmCmdFunc[DISTALARMCMD_STATE]  = cmdDistAlarmStateSet;
    pDistAlarmCmdFunc[DISTALARMCMD_STATEQ] = cmdDistAlarmStateQuery;
    
    pDistAlarmCmdFunc[DISTALARMCMD_ALARM1DIST]  = cmdDistAlarm1DistSet;
    pDistAlarmCmdFunc[DISTALARMCMD_ALARM1DISTQ] = cmdDistAlarm1DistQuery;
    pDistAlarmCmdFunc[DISTALARMCMD_ALARM2DIST]  = cmdDistAlarm2DistSet;
    pDistAlarmCmdFunc[DISTALARMCMD_ALARM2DISTQ] = cmdDistAlarm2DistQuery;
    pDistAlarmCmdFunc[DISTALARMCMD_ALARM3DIST]  = cmdDistAlarm3DistSet;
    pDistAlarmCmdFunc[DISTALARMCMD_ALARM3DISTQ] = cmdDistAlarm3DistQuery;
}

            
/*********************************************************************************************
函 数 名: cmdDistAlarmCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDistAlarmCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < DISTALARMCMD_RESERVE) && (pDistAlarmCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pDistAlarmCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}

#if 0
#endif

/*********************************************************************************************
函 数 名: cmdPdmSampleStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdPdmSampleStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPdmSampleStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_systemState.pdmInfo[i].sampleState = state;
        
                g_chanCfgBmp[i].bPdmSample = true;
                g_bCmdPostSemToFunc = true;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPdmSampleStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdPdmSampleStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_systemState.pdmInfo[CH1].sampleState) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_systemState.pdmInfo[chanNum].sampleState;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_systemState.pdmInfo[chanNum].sampleState);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_PDM, PDMCMD_SAMPLESTATEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_systemState.pdmInfo[i].sampleState;
                data[0] = i;
                for (j = 0;j < sizeof(g_systemState.pdmInfo[i].sampleState);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_PDM, PDMCMD_SAMPLESTATEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPdmEncDivSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdPdmEncDivSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   encDiv;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
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

    
    //进行参数验证
    //if (PARA_VERIFY_NO_ERROR == pvrfDriverStateVerify(cmdDataLen, pCmdData, (void *)&state))
    encDiv = *pCmdData;
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_systemState.pdmInfo[i].encDiv = encDiv;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPdmEncDivQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdPdmEncDivQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_systemState.pdmInfo[CH1].encDiv) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_systemState.pdmInfo[chanNum].encDiv;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_systemState.pdmInfo[chanNum].encDiv);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_PDM, PDMCMD_ENCDIVQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_systemState.pdmInfo[i].encDiv;
                data[0] = i;
                for (j = 0;j < sizeof(g_systemState.pdmInfo[i].encDiv);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_PDM, PDMCMD_ENCDIVQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPdmMstepCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdPdmMstepCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_systemState.pdmInfo[CH1].mstepCount) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        g_systemState.pdmInfo[chanNum].mstepCount = servFpgaPdmMstepCountRead((SampleChanEnum)chanNum);
        
        pData = (u8 *)&g_systemState.pdmInfo[chanNum].mstepCount;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_systemState.pdmInfo[chanNum].mstepCount);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_PDM, PDMCMD_MSTEPCOUNTQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                g_systemState.pdmInfo[i].mstepCount = servFpgaPdmMstepCountRead((SampleChanEnum)i);
                
                pData = (u8 *)&g_systemState.pdmInfo[i].mstepCount;
                data[0] = i;
                for (j = 0;j < sizeof(g_systemState.pdmInfo[i].mstepCount);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_PDM, PDMCMD_MSTEPCOUNTQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPdmMstepDataQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdPdmMstepDataQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;
    u16  readOffset;
    u16  readLen;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPdmMstepDataVerify(cmdDataLen, pCmdData, &readOffset, &readLen))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_systemState.pdmInfo[i].readOffset = readOffset;
                g_systemState.pdmInfo[i].readLen    = readLen;

                memset(g_systemState.pdmInfo[i].mstepData, 0, PDM_MSTEP_BUFFER_SIZE);
                
                g_chanCfgBmp[i].bPdmDataRead = true;
                g_bCmdPostSemToFunc = true;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPdmCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPdmCmdInit(void)
{
    memset(pPdmCmdFunc, 0, sizeof(pPdmCmdFunc));

    pPdmCmdFunc[PDMCMD_SAMPLESTATE]  = cmdPdmSampleStateSet;
    pPdmCmdFunc[PDMCMD_SAMPLESTATEQ] = cmdPdmSampleStateQuery;
    
    pPdmCmdFunc[PDMCMD_ENCDIV]  = cmdPdmEncDivSet;
    pPdmCmdFunc[PDMCMD_ENCDIVQ] = cmdPdmEncDivQuery;
    
    pPdmCmdFunc[PDMCMD_MSTEPCOUNTQ] = cmdPdmMstepCountQuery;
    pPdmCmdFunc[PDMCMD_MSTEPDATAQ]  = cmdPdmMstepDataQuery;
}

            
/*********************************************************************************************
函 数 名: cmdPdmCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPdmCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < PDMCMD_RESERVE) && (pPdmCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pPdmCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}
#endif    //#if GELGOOG_SINANJU



/*******************************************文件尾********************************************/
