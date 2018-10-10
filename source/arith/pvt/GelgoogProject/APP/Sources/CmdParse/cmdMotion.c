/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdMotion.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include <os.h>
#include "project.h"
#include "cmdMotion.h"
#include "pvrfMotion.h"
#include "cmdMainParse.h"
#include "pvtAlgorithm.h"
#include "servSoftTimer.h"
#include "servFPGA.h"
#include "servCommIntfc.h"
#include "servSystemPara.h"
#include "systemInit.h"

#if MRV_SUPPORT
#include "servValve.h"
#endif



/****************************************外部变量声明*****************************************/
extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;

extern DriverInfoStruct  g_driverInfo;

extern PlanInfoStruct    g_planInfo;

extern MotionInfoStruct g_motionInfo;
extern MotorInfoStruct  g_motorInfo;
extern SensorDataStruct g_sensorData;

extern OutputDataStruct  g_outputData[CH_TOTAL];
extern WaveTableStruct   g_waveTable[CH_TOTAL][WTTYPE_RESERVE];

extern EventSrcBmpStruct  g_eventSrcBmp;
extern SystemCfgBmpStruct g_systemCfgBmp;
extern ChanCfgBmpStruct   g_chanCfgBmp[CH_TOTAL];

extern bool g_bCmdPostSemToFunc;
extern bool g_bCmdPostSemToPvt;
extern bool g_bCmdPostSemToEvent;

extern OS_SEM g_semPvtCalcTask;

extern SoftTimerStruct g_motionStateTimer[CH_TOTAL];
extern SoftTimerStruct g_lineOOStepTimer[CH_TOTAL];

extern SoftTimerStruct g_motionMonitorTimer[CH_TOTAL];

extern SoftTimerStruct g_paraSaveTimer;

#if MRV_SUPPORT
extern MrvWaveTableStruct  g_mrvWaveTable[WTTYPE_RESERVE];
extern MrvChanCfgBmpStruct g_mrvChanCfgBmp;
#endif

extern DebugInfoStruct g_debugInfo;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pMotionCmdFunc[MOTIONCMD_RESERVE];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdMotionSwitch;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionSwitch(u8 cmdDataLen, u8 *pCmdData)
{
    u8   errorCode = PARA_VERIFY_NO_ERROR; 
    u8   i;
    u8   j;
    
#if MRV_SUPPORT
    bool bConfigMrv = false;
#endif
    bool bConfig[CH_TOTAL] = {0};

    u8   chanNum = *pCmdData++;
    MotionSwitchEnum  motnSwitch;
    WaveTableTypeEnum waveTable;
    WaveTableStruct  *pCurrWaveTable;


    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
#if MRV_SUPPORT
    else if (CH_MRV == chanNum)
    {
        bConfigMrv = true;
    }
#endif
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

    if (cmdDataLen >= sizeof(MotionSwitchEnum))
    {
        motnSwitch = *(MotionSwitchEnum *)pCmdData++;
        cmdDataLen -= 1;
        
        //指定波表
        if (sizeof(WaveTableTypeEnum) == cmdDataLen)
        {
            waveTable = *(WaveTableTypeEnum *)pCmdData;

            if (waveTable >= WTTYPE_RESERVE)
            {
                g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = PARA_VERIFY_ERROR_TYPE;
                return;
            }
        }
        else    //没有指定波表
        {
            //不指定波表时只支持停止和急停操作
            if ((MTSWITCH_EMERGSTOP == motnSwitch) ||
                (MTSWITCH_STOP == motnSwitch))
            {
                waveTable = WTTYPE_MAIN;
            }
            else
            {
                g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = PARA_VERIFY_ERROR_TYPE;
                return;
            }
        }
        
#if MRV_SUPPORT 
        //先处理电磁阀通道
        if (bConfigMrv)
        {
            switch (motnSwitch)
            {
                case MTSWITCH_RESET:
                    //直接进入IDLE状态
                    if ((MTSTATE_IDLE    == g_mrvWaveTable[waveTable].waveState) ||
                        (MTSTATE_CALCEND == g_mrvWaveTable[waveTable].waveState) ||
                        (MTSTATE_ERROR   == g_mrvWaveTable[waveTable].waveState))
                    {
                        //状态变了才发信号量
                        if (g_mrvWaveTable[waveTable].waveState != MTSTATE_IDLE)
                        {
                            g_mrvWaveTable[waveTable].waveState = MTSTATE_IDLE;
                            
                            g_systemState.bStateSwitch[CH_MRV][waveTable] = true;

                            //触发状态监控事件
                            g_eventSrcBmp.bStateMonitor[CH_MRV] = true;
                            g_bCmdPostSemToEvent = true;
                        }
                    }
                    else if ((MTSTATE_CALCING == g_mrvWaveTable[waveTable].waveState))
                    {
                        g_mrvWaveTable[waveTable].waveState = MTSTATE_IDLE;

                        g_systemState.bStateSwitch[CH_MRV][waveTable] = true;

                        //触发状态监控事件
                        g_eventSrcBmp.bStateMonitor[CH_MRV] = true;
                        g_bCmdPostSemToEvent = true;
                    }
                    else if ((MTSTATE_STANDBY == g_mrvWaveTable[waveTable].waveState))
                    {
                        g_mrvWaveTable[waveTable].waveState = MTSTATE_CALCEND;

                        g_systemState.bStateSwitch[CH_MRV][waveTable] = true;

                        //触发状态监控事件
                        g_eventSrcBmp.bStateMonitor[CH_MRV] = true;
                        g_bCmdPostSemToEvent = true;
                    }
                    else
                    {
                        errorCode = PARA_VERIFY_ERROR_TYPE;
                    }
                  break;
                 
                case MTSWITCH_EMERGSTOP:
                    if (CHSTATE_OUTPUTING == g_systemState.chanState[CH_MRV])
                    {
                        //给FUNC发信号量
                        g_mrvChanCfgBmp.bEmergStop = true;
                        g_bCmdPostSemToFunc = true;
                    }
                    else
                    {
                        errorCode = PARA_VERIFY_ERROR_TYPE;
                    }
                  break;
                  
                case MTSWITCH_STOP:
                    if (CHSTATE_OUTPUTING == g_systemState.chanState[CH_MRV])
                    {
                        //给FUNC发信号量
                        g_mrvChanCfgBmp.bStop = true;
                        g_bCmdPostSemToFunc = true;
                    }
                    else
                    {
                        errorCode = PARA_VERIFY_ERROR_TYPE;
                    }
                  break;
                  
                case MTSWITCH_RUN:
                    //只有在standby状态下才能进入running状态
                    if (MTSTATE_STANDBY == g_mrvWaveTable[waveTable].waveState)
                    {
                        //给FUNC发信号量
                        g_systemCfgBmp.bRun = true;
                        g_bCmdPostSemToFunc = true;
                    }
                    else
                    {
                        errorCode = PARA_VERIFY_ERROR_TYPE;
                    }
                  break;
                  
                case MTSWITCH_PREPARE:
                    //只有在计算结束状态下才能进入standby状态
                    //非输出状态下才能预取
                    if ((CHSTATE_OUTPUTING != g_systemState.chanState[CH_MRV]) &&
                        (MTSTATE_CALCEND   == g_mrvWaveTable[waveTable].waveState))
                    {
                        //先把当前通道处于STANDBY状态的波表切换回计算结束状态
                        for (j = 0;j < WTTYPE_RESERVE;j++)
                        {
                            if (MTSTATE_STANDBY == g_waveTable[CH_MRV][j].waveState)
                            {
                                g_waveTable[CH_MRV][j].waveState = MTSTATE_CALCEND;
                            }
                        }
                        
                        g_systemState.outpWaveTable[CH_MRV] = waveTable;

                        g_mrvChanCfgBmp.bPrepare = true;
                        g_bCmdPostSemToFunc = true;
                    }
                    else
                    {
                        errorCode = PARA_VERIFY_ERROR_TYPE;
                    }
                  break;

                default:
                    errorCode = PARA_VERIFY_ERROR_TYPE;    //CJ 2017.04.11 Add
                  break;
            }   //switch (motnSwitch)
        }   //if (bConfig[i])
#endif

        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                pCurrWaveTable = &g_waveTable[i][waveTable];

                switch (motnSwitch)
                {
                    case MTSWITCH_RESET:
                        //直接进入IDLE状态
                        if ((MTSTATE_IDLE    == pCurrWaveTable->waveState) ||
                            (MTSTATE_CALCEND == pCurrWaveTable->waveState) ||
                            (MTSTATE_ERROR   == pCurrWaveTable->waveState))
                        {
                            //状态变了才发信号量
                            if (pCurrWaveTable->waveState != MTSTATE_IDLE)
                            {
                                pCurrWaveTable->waveState = MTSTATE_IDLE;
                                
                                g_systemState.bStateSwitch[i][waveTable] = true;

                                //触发状态监控事件
                                g_eventSrcBmp.bStateMonitor[i] = true;
                                g_bCmdPostSemToEvent = true;

                                //FIFO模式下有可能还在计算PVT
                                if (WTWORKMODE_FIFO == pCurrWaveTable->waveWorkMode)
                                {
                                    sysTaskClear(SYSTASK_PVTCALC);
                                }
                            }
                        }
                        else if ((MTSTATE_CALCING == pCurrWaveTable->waveState))
                        {
                            pCurrWaveTable->waveState = MTSTATE_IDLE;

                            g_systemState.bStateSwitch[i][waveTable] = true;

                            //触发状态监控事件
                            g_eventSrcBmp.bStateMonitor[i] = true;
                            g_bCmdPostSemToEvent = true;

                            sysTaskClear(SYSTASK_PVTCALC);
                        }
                        else if ((MTSTATE_STANDBY == pCurrWaveTable->waveState))
                        {
                            pCurrWaveTable->waveState = MTSTATE_CALCEND;

                            g_systemState.bStateSwitch[i][waveTable] = true;

                            //触发状态监控事件
                            g_eventSrcBmp.bStateMonitor[i] = true;
                            g_bCmdPostSemToEvent = true;

                            //FIFO模式下有可能还在计算PVT
                            if (WTWORKMODE_FIFO == pCurrWaveTable->waveWorkMode)
                            {
                                pCurrWaveTable->waveState = MTSTATE_IDLE;

                                sysTaskClear(SYSTASK_PVTCALC);
                            }
                        }
                        else
                        {
                            errorCode = PARA_VERIFY_ERROR_TYPE;
                        }
                      break;
                     
                    case MTSWITCH_EMERGSTOP:
                        if (CHSTATE_OUTPUTING == g_systemState.chanState[i])
                        {
                            //给FUNC发信号量
                            g_chanCfgBmp[i].bEmergStop = true;
                            g_bCmdPostSemToFunc = true;
                        }
                        else
                        {
                            errorCode = PARA_VERIFY_ERROR_TYPE;
                        }
                      break;
                      
                    case MTSWITCH_STOP:
                        if (CHSTATE_OUTPUTING == g_systemState.chanState[i])
                        {
                            //给FUNC发信号量
                            g_chanCfgBmp[i].bStop = true;
                            g_bCmdPostSemToFunc = true;
                        }
                        else
                        {
                            errorCode = PARA_VERIFY_ERROR_TYPE;
                        }
                      break;
                      
                    case MTSWITCH_RUN:
                        //只有在standby状态下才能进入running状态
                        if (MTSTATE_STANDBY == pCurrWaveTable->waveState)
                        {
                            //给FUNC发信号量
                            g_systemCfgBmp.bRun = true;
                            g_bCmdPostSemToFunc = true;
                        }
                        else
                        {
                            errorCode = PARA_VERIFY_ERROR_TYPE;
                        }
                      break;
                      
                    case MTSWITCH_PREPARE:
                        //只有在计算结束状态下才能进入standby状态
                        //非输出状态下才能预取
                        if ((CHSTATE_OUTPUTING != g_systemState.chanState[i]) &&
                            (MTSTATE_CALCEND   == pCurrWaveTable->waveState))
                        {
                            //先把当前通道处于STANDBY状态的波表切换回计算结束状态
                            for (j = 0;j < WTTYPE_RESERVE;j++)
                            {
                                if (MTSTATE_STANDBY == g_waveTable[i][j].waveState)
                                {
                                    g_waveTable[i][j].waveState = MTSTATE_CALCEND;
                                }
                            }
                            
                            g_systemState.outpWaveTable[i] = waveTable;

                            g_chanCfgBmp[i].bPrepare = true;
                            g_bCmdPostSemToFunc = true;
                        }
                        else
                        {
                            errorCode = PARA_VERIFY_ERROR_TYPE;
                        }
                      break;

                    default:
                        errorCode = PARA_VERIFY_ERROR_TYPE;    //CJ 2017.04.11 Add
                      break;
                }   //switch (motnSwitch)
            }   //if (bConfig[i])
        }   //for (i = 0;i < CH_TOTAL;i++)
    }   //if (cmdDataLen >= sizeof(MotionSwitchEnum))
    else
    {
        errorCode = PARA_VERIFY_ERROR_LEN;    //CJ 2017.04.11 Modify
    }
    
    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = errorCode;
}


/*********************************************************************************************
函 数 名: cmdMotionStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   errorCode = PARA_VERIFY_NO_ERROR;
    u8   data[3];
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;
    MotionStateEnum   waveState;
    WaveTableTypeEnum waveTable;


#if MRV_SUPPORT    
    if (chanNum <= CH_MAX + MRV_CH_TOTAL_NUM)
#else
    if (chanNum <= CH_MAX)
#endif
    {
        bConfig[chanNum] = true;
    }
    else
    {
#if MRV_SUPPORT 
        for (i = 0;i < CH_TOTAL + MRV_CH_TOTAL_NUM;i++)
#else
        for (i = 0;i < CH_TOTAL;i++)
#endif
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    
    waveTable = *(WaveTableTypeEnum *)pCmdData++;
    if (waveTable < WTTYPE_RESERVE)
    {   
        if ((sizeof(chanNum) + sizeof(WaveTableTypeEnum)) == cmdDataLen)    //查询
        {            
#if MRV_SUPPORT 
            for (i = 0;i < CH_TOTAL + MRV_CH_TOTAL_NUM;i++)
#else
            for (i = 0;i < CH_TOTAL;i++)
#endif
            {
                if (bConfig[i])
                {
                    data[0] = i;
                    data[1] = waveTable;

                    #if MRV_SUPPORT
                    if (CH_MRV == i)
                    {
                        data[2] = g_mrvWaveTable[waveTable].waveState;
                    }
                    else
                    {
                        data[2] = g_waveTable[i][waveTable].waveState;
                    }
                    
                    #else
                    
                    data[2] = g_waveTable[i][waveTable].waveState;
                    #endif
                    
                    cmdFrameSend(CMD_MOTION, MOTIONCMD_STATE, sizeof(data), data);
                }
            }
        }
        else if ((sizeof(chanNum) + sizeof(MotionStateEnum) + sizeof(WaveTableTypeEnum)) == cmdDataLen)    //回应
        {        
            for (i = 0;i < CH_TOTAL;i++)
            {
                if ((bConfig[i]) && (STATEQRY_ACTIVE == g_motionInfo.motion[i].stateReport))
                {
                    #if MRV_SUPPORT
                    if (CH_MRV == i)
                    {
                        waveState = g_mrvWaveTable[waveTable].waveState;
                    }
                    else
                    {
                        waveState = g_waveTable[i][waveTable].waveState;
                    }
                    
                    #else
                    
                    waveState = g_waveTable[i][waveTable].waveState;
                    #endif

                    //回复的和当前状态一致则停止上报
                    if (waveState == *(MotionStateEnum *)pCmdData)
                    {
                        g_systemState.bStateSwitch[i][waveTable] = false;
                        servStimerDelete(&g_motionStateTimer[i]);
                    }
                }
            }
        }
        else
        {
            errorCode = PARA_VERIFY_ERROR_LEN;
        }
    }
    else
    {
        errorCode = PARA_VERIFY_ERROR_INDEX;
    }
    
    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = errorCode;
}


/*********************************************************************************************
函 数 名: cmdMotionStateReportSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionStateReportSet(u8 cmdDataLen, u8 *pCmdData)
{
    StateReportEnum  stateReport;
    u8   i, j;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionStateReportVerify(cmdDataLen, pCmdData, (void *)&stateReport))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].stateReport = stateReport;

                //改成查询方式时要关闭定时器
                if (STATEQRY_QUERY == stateReport)
                {
                    servStimerDelete(&g_motionStateTimer[i]);
                    for (j = 0;j < WTTYPE_RESERVE;j++)
                    {
                        g_systemState.bStateSwitch[i][j] = false;
                    }
                }
            }
            
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionStateReportQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionStateReportQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];
    u8 i;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].stateReport) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        data[0] = chanNum;
        data[1] = g_motionInfo.motion[chanNum].stateReport;
        cmdFrameSend(CMD_MOTION, MOTIONCMD_STATEREPTQ, dataLen, data);  
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                data[0] = i;
                data[1] = g_motionInfo.motion[i].stateReport;
                cmdFrameSend(CMD_MOTION, MOTIONCMD_STATEREPTQ, dataLen, data);  
            }
        }
    } 
}


/*********************************************************************************************
函 数 名: cmdMotionStartSrcSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionStartSrcSet(u8 cmdDataLen, u8 *pCmdData)
{
    StartSourceEnum  startSrc;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionStartSrcVerify(cmdDataLen, pCmdData, (void *)&startSrc))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].startSrc = startSrc;

                //给FUNC发信号量
                g_chanCfgBmp[i].bStartSrc = true;
                g_bCmdPostSemToFunc = true;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionStartSrcQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionStartSrcQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];
    u8 i;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].startSrc) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        data[0] = chanNum;
        data[1] = g_motionInfo.motion[chanNum].startSrc;
        cmdFrameSend(CMD_MOTION, MOTIONCMD_STARTSRCQ, dataLen, data);  
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                data[0] = i;
                data[1] = g_motionInfo.motion[i].startSrc;
                cmdFrameSend(CMD_MOTION, MOTIONCMD_STARTSRCQ, dataLen, data);  
            }
        }
    } 
}


/*********************************************************************************************
函 数 名: cmdMotionStartTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionStartTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    ReceiveTypeEnum  startType;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionStartTypeVerify(cmdDataLen, pCmdData, (void *)&startType))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].startType = startType;

                //给FUNC发信号量
                g_chanCfgBmp[i].bStartSrc = true;
                g_bCmdPostSemToFunc = true;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionStartTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionStartTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];
    u8 i;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].startType) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        data[0] = chanNum;
        data[1] = g_motionInfo.motion[chanNum].startType;
        cmdFrameSend(CMD_MOTION, MOTIONCMD_STARTTYPEQ, dataLen, data);  
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                data[0] = i;
                data[1] = g_motionInfo.motion[i].startType;
                cmdFrameSend(CMD_MOTION, MOTIONCMD_STARTTYPEQ, dataLen, data);  
            }
        }
    } 
}


/*********************************************************************************************
函 数 名: cmdMotionMaxSpeedSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMaxSpeedSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 maxSpeed;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMaxSpeedVerify(cmdDataLen, pCmdData, (void *)&maxSpeed))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].maxSpeed = maxSpeed;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMaxSpeedQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMaxSpeedQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].maxSpeed) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].maxSpeed;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].maxSpeed);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_MAXSPEEDQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].maxSpeed;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].maxSpeed);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_MAXSPEEDQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMinSpeedSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMinSpeedSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 minSpeed;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMinSpeedVerify(cmdDataLen, pCmdData, (void *)&minSpeed))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].minSpeed = minSpeed;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMinSpeedQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMinSpeedQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].minSpeed) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].minSpeed;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].minSpeed);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_MINSPEEDQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].minSpeed;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].minSpeed);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_MINSPEEDQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMaxPositionSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMaxPositionSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 maxPosn;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMaxPosnVerify(cmdDataLen, pCmdData, (void *)&maxPosn))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].maxPosn = maxPosn;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMaxPositionQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMaxPositionQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].maxPosn) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].maxPosn;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].maxPosn);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_MAXPOSNQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].maxPosn;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].maxPosn);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_MAXPOSNQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMinPositionSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMinPositionSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 minPosn;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMinPosnVerify(cmdDataLen, pCmdData, (void *)&minPosn))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].minPosn = minPosn;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMinPositionQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMinPositionQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].minPosn) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].minPosn;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].minPosn);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_MINPOSNQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].minPosn;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].minPosn);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_MINPOSNQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMaxTorqueSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMaxTorqueSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 maxTorque;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMaxTorqueVerify(cmdDataLen, pCmdData, (void *)&maxTorque))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].maxTorque = maxTorque;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMaxTorqueQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMaxTorqueQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].maxTorque) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].maxTorque;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].maxTorque);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_MAXTORQUEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].maxTorque;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].maxTorque);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_MAXTORQUEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMinTorqueSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMinTorqueSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 minTorque;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMinTorqueVerify(cmdDataLen, pCmdData, (void *)&minTorque))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].minTorque = minTorque;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMinTorqueQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMinTorqueQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].minTorque) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].minTorque;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].minTorque);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_MINTORQUEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].minTorque;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].minTorque);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_MINTORQUEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMaxAccSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMaxAccSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 maxAcc;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMaxTorqueVerify(cmdDataLen, pCmdData, (void *)&maxAcc))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].maxAcc = maxAcc;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMaxAccQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMaxAccQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].maxAcc) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].maxAcc;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].maxAcc);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_MAXACCQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].maxAcc;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].maxAcc);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_MAXACCQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMinAccSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMinAccSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 minAcc;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMinTorqueVerify(cmdDataLen, pCmdData, (void *)&minAcc))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].minAcc = minAcc;
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionMinAccQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionMinAccQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].minAcc) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].minAcc;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].minAcc);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_MINACCQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].minAcc;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].minAcc);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_MINACCQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionOriginSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionOriginSet(u8 cmdDataLen, u8 *pCmdData)
{  
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


    for (i = 0;i < CH_TOTAL;i++)
    {
        if (bConfig[i])
        {
            g_motionInfo.motion[i].origin = g_systemState.currSteps;
        }
    }

    //存入EEPROM中
    servStimerAdd(&g_paraSaveTimer);
}


/*********************************************************************************************
函 数 名: cmdMotionOriginQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionOriginQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].origin) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motionInfo.motion[chanNum].origin;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motionInfo.motion[chanNum].origin);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_ORIGINQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motionInfo.motion[i].origin;
                data[0] = i;
                for (j = 0;j < sizeof(g_motionInfo.motion[i].origin);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_ORIGINQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionGoOrigin;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionGoOrigin(u8 cmdDataLen, u8 *pCmdData)
{
    /*u8   i;
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
    cmdDataLen -= 1;*/
}


/*********************************************************************************************
函 数 名: cmdMotionOffsetStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionOffsetStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum  offsetState;
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
    if (PARA_VERIFY_NO_ERROR == pvrfMotionOffsetStateVerify(cmdDataLen, pCmdData, (void *)&offsetState))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].offsetState = offsetState;

                if (SENSOR_ON == offsetState)
                {
                    servStimerAdd(&g_motionMonitorTimer[i]);
                }
                else
                {
                    servStimerDelete(&g_motionMonitorTimer[i]);
                }
            }
        }

        //存入EEPROM中
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotionOffsetStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionOffsetStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];
    u8 i;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motionInfo.motion[CH1].offsetState) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        data[0] = chanNum;
        data[1] = g_motionInfo.motion[chanNum].offsetState;
        cmdFrameSend(CMD_MOTION, MOTIONCMD_OFFSETSTATEQ, dataLen, data);  
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                data[0] = i;
                data[1] = g_motionInfo.motion[i].offsetState;
                cmdFrameSend(CMD_MOTION, MOTIONCMD_OFFSETSTATEQ, dataLen, data);  
            }
        }
    } 
}


/*********************************************************************************************
函 数 名: cmdMotionCountCircleQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionCountCircleQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_systemState.circleNum[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_systemState.circleNum[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_systemState.circleNum[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_COUNTCIRCLEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_systemState.circleNum[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_systemState.circleNum[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_COUNTCIRCLEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionABCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionABCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_systemState.abCount[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        servFpgaEncoderCountRead(chanNum, NULL, &g_systemState.abCount[chanNum], NULL);
        
        pData = (u8 *)&g_systemState.abCount[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_systemState.abCount[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_ABCOUNTQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                servFpgaEncoderCountRead(i, NULL, &g_systemState.abCount[chanNum], NULL);

                pData = (u8 *)&g_systemState.abCount[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_systemState.abCount[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_ABCOUNTQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionABCountClear;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionABCountClear(u8 cmdDataLen, u8 *pCmdData)
{
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

    for (i = 0;i < CH_TOTAL;i++)
    {
        if (bConfig[i])
        {
            servFpgaEncoderCountReset(i);
            g_systemState.abCount[i] = 0;
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionRunTimeLCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionRunTimeLCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  dataLen;
    u8  *pData;
    u8  data[6];
    u8  i, j;
    u32 timeCount;
    u8  chanNum = *pCmdData++;

    
    dataLen = sizeof(timeCount) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        timeCount = (u32)g_debugInfo.runTime[chanNum];
        pData = (u8 *)&timeCount;
        data[0] = chanNum;
        for (i = 0;i < sizeof(timeCount);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_RUNTIMELQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                timeCount = (u32)g_debugInfo.runTime[i];
                pData = (u8 *)&timeCount;
                data[0] = i;
                for (j = 0;j < sizeof(timeCount);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_RUNTIMELQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionRunTimeHCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionRunTimeHCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  dataLen;
    u8  *pData;
    u8  data[6];
    u8  i, j;
    u32 timeCount;
    u8  chanNum = *pCmdData++;

    
    dataLen = sizeof(timeCount) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        timeCount = (u32)(g_debugInfo.runTime[chanNum] >> 32);
        pData = (u8 *)&timeCount;
        data[0] = chanNum;
        for (i = 0;i < sizeof(timeCount);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_RUNTIMEHQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                timeCount = (u32)(g_debugInfo.runTime[i] >> 32);
                pData = (u8 *)&timeCount;
                data[0] = i;
                for (j = 0;j < sizeof(timeCount);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTION, MOTIONCMD_RUNTIMEHQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotionCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionCmdInit(void)
{
    memset(pMotionCmdFunc, 0, sizeof(pMotionCmdFunc));

    pMotionCmdFunc[MOTIONCMD_SWITCH]     = cmdMotionSwitch;
    pMotionCmdFunc[MOTIONCMD_STATE]      = cmdMotionStateQuery;
    
    pMotionCmdFunc[MOTIONCMD_STATEREPT]  = cmdMotionStateReportSet;
    pMotionCmdFunc[MOTIONCMD_STATEREPTQ] = cmdMotionStateReportQuery;
    
    pMotionCmdFunc[MOTIONCMD_STARTSRC]   = cmdMotionStartSrcSet;
    pMotionCmdFunc[MOTIONCMD_STARTSRCQ]  = cmdMotionStartSrcQuery;
    pMotionCmdFunc[MOTIONCMD_STARTTYPE]  = cmdMotionStartTypeSet;
    pMotionCmdFunc[MOTIONCMD_STARTTYPEQ] = cmdMotionStartTypeQuery;
    
    pMotionCmdFunc[MOTIONCMD_MAXSPEED]   = cmdMotionMaxSpeedSet;
    pMotionCmdFunc[MOTIONCMD_MAXSPEEDQ]  = cmdMotionMaxSpeedQuery;
    pMotionCmdFunc[MOTIONCMD_MINSPEED]   = cmdMotionMinSpeedSet;
    pMotionCmdFunc[MOTIONCMD_MINSPEEDQ]  = cmdMotionMinSpeedQuery;
    pMotionCmdFunc[MOTIONCMD_MAXPOSN]    = cmdMotionMaxPositionSet;
    pMotionCmdFunc[MOTIONCMD_MAXPOSNQ]   = cmdMotionMaxPositionQuery;
    pMotionCmdFunc[MOTIONCMD_MINPOSN]    = cmdMotionMinPositionSet;
    pMotionCmdFunc[MOTIONCMD_MINPOSNQ]   = cmdMotionMinPositionQuery;
    pMotionCmdFunc[MOTIONCMD_MAXTORQUE]  = cmdMotionMaxTorqueSet;
    pMotionCmdFunc[MOTIONCMD_MAXTORQUEQ] = cmdMotionMaxTorqueQuery;
    pMotionCmdFunc[MOTIONCMD_MINTORQUE]  = cmdMotionMinTorqueSet;
    pMotionCmdFunc[MOTIONCMD_MINTORQUEQ] = cmdMotionMinTorqueQuery;
    pMotionCmdFunc[MOTIONCMD_MAXACC]     = cmdMotionMaxAccSet;
    pMotionCmdFunc[MOTIONCMD_MAXACCQ]    = cmdMotionMaxAccQuery;
    pMotionCmdFunc[MOTIONCMD_MINACC]     = cmdMotionMinAccSet;
    pMotionCmdFunc[MOTIONCMD_MINACCQ]    = cmdMotionMinAccQuery;
    
    pMotionCmdFunc[MOTIONCMD_ORIGIN]     = cmdMotionOriginSet;
    pMotionCmdFunc[MOTIONCMD_ORIGINQ]    = cmdMotionOriginQuery;
    pMotionCmdFunc[MOTIONCMD_GOORIGIN]   = cmdMotionGoOrigin;
    
    pMotionCmdFunc[MOTIONCMD_OFFSETSTATE]  = cmdMotionOffsetStateSet;
    pMotionCmdFunc[MOTIONCMD_OFFSETSTATEQ] = cmdMotionOffsetStateQuery;
    
    pMotionCmdFunc[MOTIONCMD_COUNTCIRCLEQ] = cmdMotionCountCircleQuery;
    pMotionCmdFunc[MOTIONCMD_ABCOUNTQ]     = cmdMotionABCountQuery;

    pMotionCmdFunc[MOTIONCMD_ABCOUNTCLEAR] = cmdMotionABCountClear;

    pMotionCmdFunc[MOTIONCMD_RUNTIMELQ] = cmdMotionRunTimeLCountQuery;
    pMotionCmdFunc[MOTIONCMD_RUNTIMEHQ] = cmdMotionRunTimeHCountQuery;
}

            
/*********************************************************************************************
函 数 名: cmdMotionCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotionCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < MOTIONCMD_RESERVE) && (pMotionCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pMotionCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


/*******************************************文件尾********************************************/
