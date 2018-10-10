/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  eventManageTask.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.06.23;
历史版本:  无;
*********************************************************************************************/
#include <os.h>
#include <string.h>
#include "project.h"
#include "eventManageTask.h"
#include "servFpga.h"
#include "servSoftTimer.h"
#include "cmdMotion.h"
#include "servLed.h"
#include "servDriver.h"
#include "cmdMainParse.h"
#include "servCommIntfc.h"
#include "servStreamBuffer.h"
#include "systemInit.h"



/****************************************外部变量声明*****************************************/
extern OS_SEM g_semEventManageTask;
extern OS_SEM g_semFunctionTask;
extern OS_SEM g_semCmdParseTask;

extern OS_MUTEX  g_mutexSdioInterface;

extern ChanCfgBmpStruct g_chanCfgBmp[CH_TOTAL];

extern SystemCfgBmpStruct g_systemCfgBmp;

extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;
extern MotorInfoStruct   g_motorInfo; 
extern MotionInfoStruct  g_motionInfo;
extern WaveTableStruct   g_waveTable[CH_TOTAL][WTTYPE_RESERVE];
extern OutputDataStruct  g_outputData[CH_TOTAL];
extern DebugInfoStruct   g_debugInfo;

extern SoftTimerStruct g_ledTimer;

extern EventSrcBmpStruct g_eventSrcBmp;
extern PlanInfoStruct    g_planInfo;

extern SoftTimerStruct    g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];
extern SensorUartStruct   g_sensorUart;
extern StreamBufferStruct g_senUartRxBuffer[UARTNUM_RESERVE][SENSOR_RESERVE];
extern ReportInfoStruct   g_reportInfo;
extern SensorDataStruct   g_sensorData;

extern DriverInfoStruct  g_driverInfo;

extern bool g_bPerdPostSemToCmd;
extern bool g_bPerdPostSemToEvent;


#ifdef PROJECT_GELGOOG

#if GELGOOG_SINANJU
extern SensorAlarmStruct g_sensorAlarm;

#else

extern IsolatorInStruct g_isolatorIn;
#endif

#endif



/*****************************************局部宏定义******************************************/
#define    INT_SRC_STOP_STATE_MARK       0x003E    //BIT[1:5]
#define    INT_SRC_ALARM_STATE_MARK      0x1F00    //BIT[9:13]
#define    INT_SRC_ALARM_STATE_LSHIFT    9         //左移9bit

#define    ANGLE_SENSOR_FRAME_NUM        3

#define    ABS_ENCODER_VALID_BIT_WIDTH   0x3FFFF

#define    EVENT_ABS_ENCODER_SOF         0x1A
#define    EVENT_DIST_SENSOR_SOF         0x5A



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
bool bEventPostSemToFunc = false;
bool bEventPostSemToCmd  = false;
EventSrcBmpStruct eventSrcBmp;
SoftTimerStruct g_motionStateTimer[CH_TOTAL];
SoftTimerStruct g_motionMonitorTimer[CH_TOTAL];
SoftTimerStruct g_lineOOStepTimer[CH_TOTAL];
SoftTimerStruct g_driverCurrTimer[CH_TOTAL];

SoftTimerStruct g_runTimeCountTimer[CH_TOTAL];

#if GELGOOG_AXIS_10
SoftTimerStruct g_driverFaultTimer;
#else
SoftTimerStruct g_drvStateReadTimer[CH_TOTAL];    //除了10轴其他MRQ都支持按通道读状态
#endif


#ifdef PROJECT_GELGOOG

#if !GELGOOG_SINANJU    //4轴或者10轴
LevelStatusEnum lastLevelStatus;
    
SoftTimerStruct g_isolatorInTimer;
    
#else    //机械臂

SoftTimerStruct g_distAlarmLedTimer;
#endif

#endif



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: irqEventOccur;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqEventOccur(EventSourceEnum eventSrc, u16 eventValue)
{
    OS_ERR  os_err;
    

    switch (eventSrc)
    {
        case EVENTSRC_FPGA:
            g_eventSrcBmp.bFpgaInt = true;
          break;

        case EVENTSRC_ANALOGIN:
            g_eventSrcBmp.bAnalogInInt = true;
          break;

        default:
          break;
    }
    
    g_systemState.eventValue = eventValue;

    //发送信号量
    OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, &os_err);
}


/*********************************************************************************************
函 数 名: eventWaveEndStateProcess
实现功能: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
void eventWaveEndStateProcess(u8 chanNum)
{
    WaveTableTypeEnum outpWaveTable = g_systemState.outpWaveTable[chanNum];


    g_systemState.chanState[chanNum] = CHSTATE_IDLE;
    
    //恢复波表状态到上一次
    g_waveTable[chanNum][outpWaveTable].waveState = g_waveTable[chanNum][outpWaveTable].lastState;

    servFpgaWaveEnable(chanNum, false);

    g_systemState.bStateSwitch[chanNum][outpWaveTable] = true;

    //FIFO模式下有可能还在下载PVT
    if (WTWORKMODE_FIFO == g_waveTable[chanNum][outpWaveTable].waveWorkMode)
    {
        sysTaskClear(SYSTASK_PVTCALC);
    }

    servStimerDelete(&g_runTimeCountTimer[chanNum]);

    //触发状态监控事件
    eventSrcBmp.bStateMonitor[chanNum] = true;

    //等待电机运行彻底停止
    bspDelayMs(10); 

    servFpgaEncoderCountRead(chanNum, &g_systemState.circleNum[chanNum], 
                                      &g_systemState.abCount[chanNum], 
                                      NULL);
    g_systemState.lastAbCount[chanNum] = 0;

    //开启运动监测定时器
    if (SENSOR_ON == g_motionInfo.motion[chanNum].offsetState)
    {
        servStimerAdd(&g_motionMonitorTimer[chanNum]);
    }

    //关闭循环数查询定时器
    servStimerDelete(&g_reportTimer[chanNum][REPTTYPE_CYCLE]);
    g_systemState.cycleNumCount[chanNum] = 0;

#if !GELGOOG_AXIS_10
    //是否需要设置空闲电流
    if (0 == g_driverInfo.driver[chanNum].switchTime)
    {
        eventDriverCurrProcess(chanNum, SENSOR_OFF);
    }
    else
    {
        //开启定时器
        servStimerInit(&g_driverCurrTimer[chanNum], g_driverInfo.driver[chanNum].switchTime, eventDriverCurrTimerCB);
        servStimerAdd(&g_driverCurrTimer[chanNum]);
    }
#endif
}


/*********************************************************************************************
函 数 名: eventMotionStateTimerCB
实现功能: 上传逻辑进行运动准备的结果
输入参数: 
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventMotionStateTimerCB(void *timeOutPara)
{
    u8 chanNum = *(u8 *)timeOutPara;
    u8 i;
    u8 data[3];


    data[0] = chanNum;
    for (i = 0;i < WTTYPE_RESERVE;i++)
    {
        //发生了状态切换
        if (g_systemState.bStateSwitch[chanNum][i])
        {
            data[1] = i;
            data[2] = g_waveTable[chanNum][i].waveState;

            cmdFrameSend(CMD_MOTION, MOTIONCMD_STATE, sizeof(data), data);

            g_bPerdPostSemToCmd = true;
        }
    }  
    
    //继续开启定时器，收到回复的时候关闭
    if ((g_motionStateTimer[chanNum].bUsed) && (STATEQRY_ACTIVE == g_motionInfo.motion[chanNum].stateReport))
    {
        servStimerAdd(&g_motionStateTimer[chanNum]);
    }
}


/*********************************************************************************************
函 数 名: eventMotionStateProcess
实现功能: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*********************************************************************************************/
void eventMotionStateProcess(u8 chanNum)
{
    u8 i;
    bool bIdle = false;
    u8 data[3];


    //非初始化状态且上报开关开启才上报
    if ((SYSTATE_POWERON != g_systemState.systemState) &&   
        (STATEQRY_ACTIVE == g_motionInfo.motion[chanNum].stateReport))
    {
        data[0] = chanNum;
        for (i = 0;i < WTTYPE_RESERVE;i++)
        {
            //发生了状态切换
            if (g_systemState.bStateSwitch[chanNum][i])
            {
                data[1] = i;
                data[2] = g_waveTable[chanNum][i].waveState;

                cmdFrameSend(CMD_MOTION, MOTIONCMD_STATE, sizeof(data), data);

                bEventPostSemToCmd = true;
            }
        }

        servStimerAdd(&g_motionStateTimer[chanNum]);
    }

    bIdle = true;
    for (i = 0;i < WTTYPE_RESERVE;i++)
    {
        if (MTSTATE_OUTPUTING == g_waveTable[chanNum][i].waveState)
        {
            bIdle = false;
            break;
        }
    }
    if (bIdle)
    {
        g_systemState.chanState[chanNum] = CHSTATE_IDLE;
    }

    switch (g_systemState.chanState[chanNum])
    {
        case CHSTATE_IDLE:
            bIdle = true;
            for (i = 0;i < CH_TOTAL;i++)
            {
                //所有通道都是IDLE则系统进入IDLE状态
                if (g_systemState.chanState[i] != CHSTATE_IDLE)
                {
                    bIdle = false;
                    break;
                }
            }
            if (bIdle)
            {
                g_systemState.systemState = SYSTATE_IDLE; 
                servLedSet(LED_IDEL);
            }
          break;
          
        case CHSTATE_OUTPUTING:
            if (g_systemState.systemState != SYSTATE_ERROR)
            {
                g_systemState.systemState = SYSTATE_WORKING;
                servLedSet(LED_WORKING);
            }
          break;

        case CHSTATE_ERROR:
            g_systemState.systemState = SYSTATE_ERROR;
            servLedSet(LED_ERROR);
          break;

        default:
          break;
    }
    
    if (CHSTATE_POWERON == g_systemState.lastChanState[chanNum])
    {
        //通道上电完成
        g_systemState.lastChanState[chanNum] = g_systemState.chanState[chanNum];

        //LED闪烁定时器
        servStimerAdd(&g_ledTimer);

#if GELGOOG_AXIS_10
        servStimerAdd(&g_driverFaultTimer);
        
#else

#if TUNING_SUPPORT
        if (SENSOR_ON == g_driverInfo.driver[chanNum].state)
        {
            servStimerAdd(&g_drvStateReadTimer[chanNum]);
        }
#endif
#endif
        //开启运动监测定时器
        if (SENSOR_ON == g_motionInfo.motion[chanNum].offsetState)
        {
            servStimerAdd(&g_motionMonitorTimer[chanNum]);
        }
    }
}

#if GELGOOG_AXIS_10
/*********************************************************************************************
函 数 名: eventDriverFaultTimerCB
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDriverFaultTimerCB(void *timeOutPara)
{
    g_eventSrcBmp.bDriverFault = true;

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToEvent = true;
    
    
    //继续开启定时器，收到回复的时候关闭
    if (g_driverFaultTimer.bUsed)
    {
        servStimerAdd(&g_driverFaultTimer);
    }    
}


/*********************************************************************************************
函 数 名: eventDriverFaultProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDriverFaultProcess(void *timeOutPara)
{
    FpgaResponseReg12Union responseReg;

    
    //读DriverFault状态
    responseReg.regValue = servFpgaDriverFaultRead();

    if ((0 == responseReg.regBitFiled.faultState0) &&
        (SENSOR_ON == g_driverInfo.state[CH1]))
    {
        servFpgaDriverEnable(CH1, SENSOR_OFF);
        g_driverInfo.state[CH1] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState1) &&
        (SENSOR_ON == g_driverInfo.state[CH2]))
    {
        servFpgaDriverEnable(CH2, SENSOR_OFF);
        g_driverInfo.state[CH2] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState2) &&
        (SENSOR_ON == g_driverInfo.state[CH3]))
    {
        servFpgaDriverEnable(CH3, SENSOR_OFF);
        g_driverInfo.state[CH3] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState3) &&
        (SENSOR_ON == g_driverInfo.state[CH4]))
    {
        servFpgaDriverEnable(CH4, SENSOR_OFF);
        g_driverInfo.state[CH4] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState4) &&
        (SENSOR_ON == g_driverInfo.state[CH5]))
    {
        servFpgaDriverEnable(CH5, SENSOR_OFF);
        g_driverInfo.state[CH5] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState5) &&
        (SENSOR_ON == g_driverInfo.state[CH6]))
    {
        servFpgaDriverEnable(CH6, SENSOR_OFF);
        g_driverInfo.state[CH6] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState6) &&
        (SENSOR_ON == g_driverInfo.state[CH7]))
    {
        servFpgaDriverEnable(CH7, SENSOR_OFF);
        g_driverInfo.state[CH7] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState7) &&
        (SENSOR_ON == g_driverInfo.state[CH8]))
    {
        servFpgaDriverEnable(CH8, SENSOR_OFF);
        g_driverInfo.state[CH8] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState8) &&
        (SENSOR_ON == g_driverInfo.state[CH9]))
    {
        servFpgaDriverEnable(CH9, SENSOR_OFF);
        g_driverInfo.state[CH9] = SENSOR_OFF;

        //发送过流警告
    }

    if ((0 == responseReg.regBitFiled.faultState9) &&
        (SENSOR_ON == g_driverInfo.state[CH10]))
    {
        servFpgaDriverEnable(CH10, SENSOR_OFF);
        g_driverInfo.state[CH10] = SENSOR_OFF;

        //发送过流警告
    }
}


#else


/*********************************************************************************************
函 数 名: eventDriverStateReadTimerCB
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
#if TUNING_SUPPORT
void eventDriverStateReadTimerCB(void *timeOutPara)
{
    u8  chanNum = *(u8 *)timeOutPara;


    g_eventSrcBmp.drvStateRead[chanNum] = true;

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToEvent = true; 
    
    //继续开启定时器，收到回复的时候关闭
    if (g_drvStateReadTimer[chanNum].bUsed)
    {
        servStimerAdd(&g_drvStateReadTimer[chanNum]);
    }
}
#endif


/*********************************************************************************************
函 数 名: eventDriverStateReadProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDriverStateReadProcess(u8 chanNum)
{
#if TUNING_SUPPORT

    DriverReadRespUnion readResponseReg;
    s8  sgPercent;
    u8  sePercent;
    s16 sgZero  = g_driverInfo.driver[chanNum].sgZero;
    u8  csValue = g_driverInfo.driver[chanNum].SGCSCONF.regBitFiled.CS;

    
    servDriverStateRegRead(chanNum, READREG_R0, &readResponseReg.regValue);

    if (0 == readResponseReg.readResp0.STST)    //非静止状态
    {
        //
    }
    else
    {
        //
    }
    
    if (readResponseReg.readResp0.S2GA || readResponseReg.readResp0.S2GB) 
    {
        //短路错误
    }
    
    if (readResponseReg.readResp0.OT || readResponseReg.readResp0.OTPW)
    {
        //过温错误
    }

    /**********************************R1********************************************/
    servDriverStateRegRead(chanNum, READREG_R1, &readResponseReg.regValue);
    g_sensorData.reporterData[chanNum][REPTTYPE_DRVSGALL] = readResponseReg.regValue;
    /**********************************R1********************************************/

    
    
    /**********************************R2********************************************/
    servDriverStateRegRead(chanNum, READREG_R2, &readResponseReg.regValue);

    //sgPercent(百分百)放大了100倍，所以乘100；
    //SGH是高5位，所以需要乘32，然后再放大一百倍就是3200.0了
    sgPercent = (s8)(100 - (f32)readResponseReg.readResp2.SGH * 3200.0f / sgZero);
    sePercent = (u8)(readResponseReg.readResp2.SE * 100.0f / csValue);

    g_sensorData.reporterData[chanNum][REPTTYPE_TORQUE]  = (sgZero << 16) | 
                                                           ((u8)sgPercent << 8) | 
                                                            sePercent;

    g_sensorData.reporterData[chanNum][REPTTYPE_DRVSGSE] = readResponseReg.regValue;
    /**********************************R2********************************************/
#endif
}
#endif    //#if GELGOOG_AXIS_10


/*********************************************************************************************
函 数 名: eventMotionMonitorTimerCB
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventMotionMonitorTimerCB(void *timeOutPara)
{
    s16 circle;
    s32 abCnt;
    u8  dataLen;
    u8  data[6];
    u8  *pData;
    u8  i;
    u8  chanNum = *(u8 *)timeOutPara;

    
    servFpgaEncoderCountRead(chanNum, &circle, &abCnt, NULL);

    if (g_systemState.lastAbCount[chanNum] != abCnt)
    {
        g_systemState.lastAbCount[chanNum] = abCnt;
        
        //告警
        dataLen = sizeof(abCnt) + sizeof(data[0]);
        data[0] = chanNum;
        pData = (u8 *)&abCnt;
        for (i = 0;i < sizeof(abCnt);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTION, MOTIONCMD_OFFSET, dataLen, data);

        g_bPerdPostSemToCmd = true;
    }

    if (g_motionMonitorTimer[chanNum].bUsed)
    {
        servStimerAdd(&g_motionMonitorTimer[chanNum]);
    }
}


/*********************************************************************************************
函 数 名: eventLineOOStepTimerCB
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventLineOOStepTimerCB(void *timeOutPara)
{
    u8 chanNum = *(u8 *)timeOutPara;

    
    //cmdFrameSend(CMD_OUTOFSTEP, OOSCMD_LINEWARN, 1, (u8 *)&chanNum);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;   
    
    //继续开启定时器，收到回复的时候关闭
    if (g_lineOOStepTimer[chanNum].bUsed)
    {
        servStimerAdd(&g_lineOOStepTimer[chanNum]);
    }
}


#ifdef PROJECT_GELGOOG
#if !GELGOOG_SINANJU

/*********************************************************************************************
函 数 名: eventIsolInMonitorTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventIsolInMonitorTimerCB(void *timeOutPara)
{  
    LevelStatusEnum levelStatus;
    TrigInTypeEnum  trigType;

    
    levelStatus = servFpgaDioStatusRead(DIGITALIO_XIN);

    //对电平进行判断
    if (LEVEL_HIGH == levelStatus)
    {
        trigType = TRIGINTYPE_HIGH;

        if (levelStatus != lastLevelStatus)
        {
            trigType = TRIGINTYPE_RISE;
            lastLevelStatus = levelStatus;
        }
    }
    else
    {
        trigType = TRIGINTYPE_LOW;

        if (levelStatus != lastLevelStatus)
        {
            trigType = TRIGINTYPE_FALL;
            lastLevelStatus = levelStatus;
        }
    }

    if (trigType == g_isolatorIn.type)
    {
        g_eventSrcBmp.bXInMatch = true;
        g_bPerdPostSemToEvent = true;
    }

    if (g_isolatorInTimer.bUsed)
    {
        servStimerAdd(&g_isolatorInTimer);
    }
}

/*********************************************************************************************
函 数 名: eventIsolInMonitorProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventIsolInMonitorProcess(void)
{ 
    switch (g_isolatorIn.response)
    {
        case RESPONSE_NONE:
          break;
          
        case RESPONSE_ALARM:
          break;
          
        case RESPONSE_STOP:
            g_chanCfgBmp[g_isolatorIn.respChan].bStop = true;
            bEventPostSemToFunc = true;
          break;
          
        case RESPONSE_ALARMSTOP:
            g_chanCfgBmp[g_isolatorIn.respChan].bStop = true;
            bEventPostSemToFunc = true;
          break;
          
        case RESPONSE_RUN:
          break;
    
        default:
          break;
    }
}


#else    //#if !GELGOOG_SINANJU


/*********************************************************************************************
函 数 名: eventDistAlarmLedTimerCB
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDistAlarmLedTimerCB(void *timeOutPara)
{    
    g_eventSrcBmp.distAlarmLed = true;

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToEvent = true; 
    
    //继续开启定时器，收到回复的时候关闭
    if (g_distAlarmLedTimer.bUsed)
    {
        servStimerAdd(&g_distAlarmLedTimer);
    }
}


/*********************************************************************************************
函 数 名: eventDistAlarmLedProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDistAlarmLedProcess(bool bRecoverDuty)
{
    static bool bChangeDuty = false;
    u8 i;
    
    
    if (bRecoverDuty)
    {
        for (i = 0;i < ARMLED_RESERVE;i++)
        {
            servArmLedConfig((ArmLedEnum)i, g_systemInfo.armLedFreq[i], g_systemInfo.armLedDuty[i]);
        }
    }
    else
    {
        if (bChangeDuty)
        {
            for (i = 0;i < ARMLED_RESERVE;i++)
            {
                servArmLedConfig((ArmLedEnum)i, g_systemInfo.armLedFreq[i], g_systemInfo.armLedDuty[i]);
            }

            bChangeDuty = false;
        }
        else
        {
            for (i = 0;i < ARMLED_RESERVE;i++)
            {
                servArmLedConfig((ArmLedEnum)i, g_systemInfo.armLedFreq[i], 0);
            }
            
            bChangeDuty = true;
        } 
    }
}


/*********************************************************************************************
函 数 名: eventDistAlarmProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDistAlarmProcess(u8 chanNum, u16 distValue)
{
    static bool bAlarmLedOn = false;
    DistAlarmEnum sysDistAlarm = DISTALARM_RELEASE;
    DistAlarmEnum chDistAlarm;
    u8 alarmValue = 0;
    u8 i;
    

    if (distValue <= g_sensorAlarm.distAlarm[chanNum].alarm3Dist)
    {
        chDistAlarm = DISTALARM_LEVEL3;
    }
    else if (distValue <= g_sensorAlarm.distAlarm[chanNum].alarm2Dist)
    {
        chDistAlarm = DISTALARM_LEVEL2;
    }
    else if (distValue <= g_sensorAlarm.distAlarm[chanNum].alarm1Dist)
    {
        chDistAlarm = DISTALARM_LEVEL1;
    }
    else
    {
        chDistAlarm = DISTALARM_RELEASE;
    }
    
    g_systemState.chDistAlarm[chanNum] = chDistAlarm;


    //遍历所有通道，获取系统警报级别
    for (i = 0;i < DIST_SENSOR_NUM;i++)
    {
        alarmValue |= g_systemState.chDistAlarm[i];
    }
    
    if (alarmValue >= DISTALARM_LEVEL3)
    {
        sysDistAlarm = DISTALARM_LEVEL3;
    }
    else if (alarmValue >= DISTALARM_LEVEL2)
    {
        sysDistAlarm = DISTALARM_LEVEL2;
    }
    else if (alarmValue >= DISTALARM_LEVEL1)
    {
        sysDistAlarm = DISTALARM_LEVEL1;
    }
    else
    {
        sysDistAlarm = DISTALARM_RELEASE;
    }

    //根据系统警报进行处理
    switch (sysDistAlarm)
    {
        case DISTALARM_RELEASE:
            if (DISTALARM_RELEASE != g_systemState.sysDistAlarm)
            {
                g_systemState.sysDistAlarm = DISTALARM_RELEASE;
        
                g_systemState.eventCode[0] = chanNum;
                g_systemState.eventCode[1] = ERROR_CODE_INDEX_INTFACE;
                g_systemState.eventCode[2] = 0;
                g_systemState.eventCode[3] = DISTALARM_RELEASE;
                g_systemState.eventCode[4] = (u8)(distValue & 0x00FF);
                g_systemState.eventCode[5] = (u8)((distValue & 0xFF00) >> 8);
                
                cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

                g_bPerdPostSemToCmd = true;
            }
          break;
          
        case DISTALARM_LEVEL1:
            if (DISTALARM_LEVEL1 != g_systemState.sysDistAlarm)
            {
                g_systemState.sysDistAlarm = DISTALARM_LEVEL1;
        
                g_systemState.eventCode[0] = chanNum;
                g_systemState.eventCode[1] = ERROR_CODE_INDEX_INTFACE;
                g_systemState.eventCode[2] = 0;
                g_systemState.eventCode[3] = DISTALARM_LEVEL1;
                g_systemState.eventCode[4] = (u8)(distValue & 0x00FF);
                g_systemState.eventCode[5] = (u8)((distValue & 0xFF00) >> 8);
                
                cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

                g_bPerdPostSemToCmd = true;
            }
          break;

        case DISTALARM_LEVEL2:
            if (DISTALARM_LEVEL2 != g_systemState.sysDistAlarm)
            {
                g_systemState.sysDistAlarm = DISTALARM_LEVEL2;
        
                g_systemState.eventCode[0] = chanNum;
                g_systemState.eventCode[1] = ERROR_CODE_INDEX_INTFACE;
                g_systemState.eventCode[2] = 0;
                g_systemState.eventCode[3] = DISTALARM_LEVEL2;
                g_systemState.eventCode[4] = (u8)(distValue & 0x00FF);
                g_systemState.eventCode[5] = (u8)((distValue & 0xFF00) >> 8);
                
                cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

                g_bPerdPostSemToCmd = true;
                
                //减速停止所有通道输出
                for (i = CH1;i < CH_TOTAL;i++)
                {
                    g_chanCfgBmp[i].bStop = true;
                }

                bEventPostSemToFunc = true;
            }
          break;

        case DISTALARM_LEVEL3:
            if (DISTALARM_LEVEL3 != g_systemState.sysDistAlarm)
            {
                g_systemState.sysDistAlarm = DISTALARM_LEVEL3;
        
                g_systemState.eventCode[0] = chanNum;
                g_systemState.eventCode[1] = ERROR_CODE_INDEX_INTFACE;
                g_systemState.eventCode[2] = 0;
                g_systemState.eventCode[3] = DISTALARM_LEVEL3;
                g_systemState.eventCode[4] = (u8)(distValue & 0x00FF);
                g_systemState.eventCode[5] = (u8)((distValue & 0xFF00) >> 8);
                
                cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

                g_bPerdPostSemToCmd = true;

                //立即停止所有通道输出
                for (i = CH1;i < CH_TOTAL;i++)
                {
                    g_chanCfgBmp[i].bEmergStop = true;
                }

                bEventPostSemToFunc = true;
            }
          break;

        default:
          break;
    }

    if (DISTALARM_RELEASE == g_systemState.sysDistAlarm)
    {
        //警报解除后如果LED处于闪烁状态则需要进行两件事
        if (bAlarmLedOn)
        {
            bAlarmLedOn = false;

            //如果此时LED闪烁状态开启则按闪烁频率重新配置定时器
            if (SENSOR_ON == g_systemState.ledFlickerState)
            {
                servStimerInit(&g_distAlarmLedTimer, g_systemState.ledFlickerFreq, eventDistAlarmLedTimerCB);
                servStimerAdd(&g_distAlarmLedTimer);
            }
            else
            {
                //关闭闪烁定时器，恢复默认占空比
                if (g_distAlarmLedTimer.bUsed)
                {
                    servStimerDelete(&g_distAlarmLedTimer);

                    eventDistAlarmLedProcess(true);
                }
            }
        }
    }
    else
    {
        //开启闪烁定时器
        if (!bAlarmLedOn)
        {
            bAlarmLedOn = true;
            
            servStimerInit(&g_distAlarmLedTimer, LED_FLICKER_FREQ_MIN, eventDistAlarmLedTimerCB);
            servStimerAdd(&g_distAlarmLedTimer);
        }
    }
}


/*********************************************************************************************
函 数 名: eventAbsEncLimitProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventAbsEncLimitProcess(u8 chanNum, u32 encValue, ResponseTypeEnum encAlarmResponse)
{
    bool bProcessResp = false;
    u8 i;
    
    
    //如果上下限之间存在零点
    if (SENSOR_ON == g_sensorAlarm.encAlarm[chanNum].zeroPost)
    {
        //零点存在于上下限之间则大于下限且小于上限代表超出运行范围了
        if ((encValue > g_sensorAlarm.encAlarm[chanNum].dnLimit) && 
            (encValue < g_sensorAlarm.encAlarm[chanNum].upLimit))
        {
            bProcessResp = true;
        }
    }
    else
    {
        //零点不存在于上下限之间则小于下限或者大于上限代表超出运行范围了
        if ((encValue < g_sensorAlarm.encAlarm[chanNum].dnLimit) || 
            (encValue > g_sensorAlarm.encAlarm[chanNum].upLimit))
        {
            bProcessResp = true;
        }
    }

    //发生超限且未上报
    if ((bProcessResp) && (SENSOR_OFF == g_systemState.absEncLimit[chanNum]))
    {
        g_systemState.absEncLimit[chanNum] = SENSOR_ON;
        
        switch (encAlarmResponse)
        {                              
            case RESPONSE_ALARM:
                g_systemState.eventCode[0] = chanNum;
                g_systemState.eventCode[1] = ERROR_CODE_INDEX_INTFACE;
                g_systemState.eventCode[2] = 1;
                g_systemState.eventCode[3] = (u8)(encValue  & 0xFF);
                g_systemState.eventCode[4] = (u8)((encValue & 0xFF00) >> 8);
                g_systemState.eventCode[5] = (u8)((encValue & 0xFF0000) >> 16);
                
                cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

                bEventPostSemToFunc = true;
              break;
              
            case RESPONSE_STOP:
                //减速停止所有通道输出
                for (i = CH1;i < CH_TOTAL;i++)
                {
                    g_chanCfgBmp[i].bStop = true;
                }

                bEventPostSemToFunc = true;
              break;
              
            case RESPONSE_ALARMSTOP:
                
                //减速停止所有通道输出
                for (i = CH1;i < CH_TOTAL;i++)
                {
                    g_chanCfgBmp[i].bStop = true;
                }

                bEventPostSemToFunc = true;
                
                g_systemState.eventCode[0] = chanNum;
                g_systemState.eventCode[1] = ERROR_CODE_INDEX_INTFACE;
                g_systemState.eventCode[2] = 1;
                g_systemState.eventCode[3] = (u8)(encValue  & 0xFF);
                g_systemState.eventCode[4] = (u8)((encValue & 0xFF00) >> 8);
                g_systemState.eventCode[5] = (u8)((encValue & 0xFF0000) >> 16);
                
                cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

                bEventPostSemToFunc = true;
              break;

            default:
              break;
        }
    }
    else if ((!bProcessResp) && (SENSOR_ON == g_systemState.absEncLimit[chanNum]))
    {
        g_systemState.absEncLimit[chanNum] = SENSOR_OFF;
        
        //超限解除      
        g_systemState.eventCode[0] = chanNum;
        g_systemState.eventCode[1] = ERROR_CODE_INDEX_INTFACE;
        g_systemState.eventCode[2] = 1;
        g_systemState.eventCode[3] = (u8)(encValue  & 0xFF);
        g_systemState.eventCode[4] = (u8)((encValue & 0xFF00) >> 8);
        g_systemState.eventCode[5] = (u8)((encValue & 0xFF0000) >> 16);
        
        cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

        bEventPostSemToFunc = true;
        
    }
}
#endif    //#if !GELGOOG_SINANJU
#endif    //#ifdef PROJECT_GELGOOG


/*********************************************************************************************
函 数 名: eventDriverCurrTimerCB
实现功能: 
输入参数: 
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDriverCurrTimerCB(void *timeOutPara)
{
    u8 chanNum = *(u8 *)timeOutPara;


    g_eventSrcBmp.bDriverCurr[chanNum] = true;

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToEvent = true;
}


/*********************************************************************************************
函 数 名: eventDriverCurrProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDriverCurrProcess(u8 chanNum, SensorStateEnum state)
{
#if !GELGOOG_AXIS_10
    u8 curr;
    u8 vsense;
    u8 csValue;

    
    //判断下驱动电流和空闲电流是否不一致，不一致的话需要先配置下驱动电流
    if (g_driverInfo.driver[chanNum].curr != g_driverInfo.driver[chanNum].idleCurr)
    {
        if (SENSOR_ON == state)
        {
            curr = g_driverInfo.driver[chanNum].curr;
        }
        else
        {
            curr = g_driverInfo.driver[chanNum].idleCurr;
        }
        
        servDriverCurrRegParaCalc(g_driverInfo.driver[chanNum].type, 
                                  curr, 
                                  &vsense,
                                  &csValue);
        g_driverInfo.driver[chanNum].DRVCONF.regBitFiled.VSENSE = vsense;
        g_driverInfo.driver[chanNum].SGCSCONF.regBitFiled.CS    = csValue;
        
        servDriverCurrentSet(chanNum, g_driverInfo.driver[chanNum]);
    }

    //关闭定时器
    servStimerDelete(&g_driverCurrTimer[chanNum]);
#endif
}


/*********************************************************************************************
函 数 名: eventOutOfStepProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventOutOfStepProcess(u8 chanNum, ResponseTypeEnum lineResp)
{
#if GELGOOG_SINANJU
    u8 i;
#endif

    
    switch (lineResp)
    {
        case RESPONSE_NONE:
          break;
          
        case RESPONSE_ALARM:
            //发送eventCode
            g_systemState.eventCode[0] = chanNum;
            g_systemState.eventCode[1] = ERROR_CODE_INDEX_MEMORY;
            g_systemState.eventCode[2] = 0;
            g_systemState.eventCode[3] = 0;
            g_systemState.eventCode[4] = 0;
            g_systemState.eventCode[5] = 0;
            
            cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

            bEventPostSemToFunc = true;
          break;
          
        case RESPONSE_STOP:
#if GELGOOG_SINANJU
            //减速停止所有通道输出
            for (i = CH1;i < CH_TOTAL;i++)
            {
                g_chanCfgBmp[i].bStop = true;
            }
#else
            g_chanCfgBmp[chanNum].bStop = true;
#endif
            bEventPostSemToFunc = true;
          break;
          
        case RESPONSE_ALARMSTOP:
#if GELGOOG_SINANJU
            //减速停止所有通道输出
            for (i = CH1;i < CH_TOTAL;i++)
            {
                g_chanCfgBmp[i].bStop = true;
            }
#else
            g_chanCfgBmp[chanNum].bStop = true;
#endif

            bEventPostSemToFunc = true;

            //发送eventCode
            g_systemState.eventCode[0] = chanNum;
            g_systemState.eventCode[1] = ERROR_CODE_INDEX_MEMORY;
            g_systemState.eventCode[2] = 0;
            g_systemState.eventCode[3] = 0;
            g_systemState.eventCode[4] = 0;
            g_systemState.eventCode[5] = 0;
            
            cmdFrameSend(CMD_SYSTME, SYSCMD_EVENTCODEQ, EVENT_CODE_LEN, g_systemState.eventCode);

            bEventPostSemToFunc = true;
          break;

        default:
          break;
    }
}


/*********************************************************************************************
函 数 名: eventBackLashRead
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventBackLashRead(u8 chanNum)
{
    u16 backLash;
    u8  data[3];


    backLash = servFpgaBackLashRead((BackLashChanEnum)chanNum);

    data[0] = chanNum;
    data[1] = (u8)backLash;
    data[2] = (u8)(backLash >> 8);
    cmdFrameSend(CMD_MOTOR, MOTORCMD_BACKLASHTESTQ, sizeof(backLash) + sizeof(chanNum), data);

    bEventPostSemToCmd = true;
}


/*********************************************************************************************
函 数 名: eventSensorUartProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventSensorUartProcess(SenUartNumEnum uartNum, SensorNumEnum sensorNum)
{
    u8 j;
    SensorUartPhyFrameStruct *pUartPhyFrame = NULL;


    //如果Buffer中有旧数据就把旧数据出队，只留新数据在里面
    if (g_senUartRxBuffer[uartNum][sensorNum].frameCount > g_sensorUart.sensor[uartNum][sensorNum].recvNum)
    {
        for (j = 0;j < (g_senUartRxBuffer[uartNum][sensorNum].frameCount - g_sensorUart.sensor[uartNum][sensorNum].recvNum);j++)
        {
            pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[uartNum][sensorNum]);
            servDequeue(&g_senUartRxBuffer[uartNum][sensorNum], 
                        pUartPhyFrame->frameLen + sizeof(pUartPhyFrame->frameLen)); //处理完成后出队
            g_senUartRxBuffer[uartNum][sensorNum].frameCount--;
        }

#if GELGOOG_SINANJU    //机械臂固定的是测距和绝对值编码器，其他设备就不固定了
        if (UARTNUM_U1 == uartNum)
        {
            //测距传感器
            eventSrcBmp.bDistSen[sensorNum] = true;
        }
        else
        {
            //绝对值编码器    CJ 2018.02.28 Add
            eventSrcBmp.bAbsEncoder[sensorNum] = true;
        }
#endif
    }
}


/*********************************************************************************************
函 数 名: eventDistSensorProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventDistSensorProcess(SensorNumEnum sensorNum)
{
    SensorUartPhyFrameStruct  *pUartPhyFrame = NULL;
    DistanceSensorFrameStruct *pSensorFrame  = NULL;


    if (g_senUartRxBuffer[UARTNUM_U1][sensorNum].frameCount > 0)    //有数据
    {
        pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[UARTNUM_U1][sensorNum]);

        pSensorFrame = (DistanceSensorFrameStruct *)&pUartPhyFrame->SOF;

#if GELGOOG_SINANJU
        
        //机械臂测距传感器的2和3接在了UART口的3和2上了，所以这里要反下
        if (CH3 == (ChannelEnum)sensorNum)
        {
            sensorNum = (SensorNumEnum)CH2;
        }
        else if (CH2 == (ChannelEnum)sensorNum)
        {
            sensorNum = (SensorNumEnum)CH3;
        }
        
#endif
        //两个帧头都对了就认为数据正确，不做CRC校验
        if ((EVENT_DIST_SENSOR_SOF == pSensorFrame->SOF1) && (EVENT_DIST_SENSOR_SOF == pSensorFrame->SOF2))
        {
            g_sensorData.reporterData[sensorNum][REPTTYPE_DISTANCE]  = (pSensorFrame->model << 16) +
                                                                       (pSensorFrame->dataH << 8)  +
                                                                        pSensorFrame->dataL;
        }
        //servDequeue(&g_senUartRxBuffer[UARTNUM_U1][sensorNum], pUartPhyFrame->frameLen); //处理完成后出队
        //g_senUartRxBuffer[UARTNUM_U1][sensorNum].frameCount--;

#if GELGOOG_SINANJU
        
        //处理下测距传感器警报
        if (SENSOR_ON == g_sensorAlarm.distAlarm[sensorNum].state)
        {
            eventDistAlarmProcess(sensorNum, g_sensorData.reporterData[sensorNum][REPTTYPE_DISTANCE]);
        }
#endif
    }
}


#if GELGOOG_SINANJU
/*********************************************************************************************
函 数 名: eventAbsEncoderProcess
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventAbsEncoderProcess(SensorNumEnum sensorNum)
{
    SensorUartPhyFrameStruct  *pUartPhyFrame = NULL;
    AbsEncoderFrameStruct     *pSensorFrame  = NULL;


    if (g_senUartRxBuffer[UARTNUM_U2][sensorNum].frameCount > 0)    //有数据
    {
        pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[UARTNUM_U2][sensorNum]);

        pSensorFrame = (AbsEncoderFrameStruct *)&pUartPhyFrame->SOF;

        //两个帧头都对了就认为数据正确，不做CRC校验
        if ((EVENT_ABS_ENCODER_SOF == pSensorFrame->SOF1) && (EVENT_ABS_ENCODER_SOF == pSensorFrame->SOF2))
        {
            g_sensorData.reporterData[sensorNum][REPTTYPE_ABSENC] = (pSensorFrame->almc << 24) + 
                                                                    (pSensorFrame->abs2 << 16) + 
                                                                    (pSensorFrame->abs1 << 8) + 
                                                                     pSensorFrame->abs0;
        }
        //servDequeue(&g_senUartRxBuffer[UARTNUM_U2][sensorNum], pUartPhyFrame->frameLen); //处理完成后出队
        //g_senUartRxBuffer[UARTNUM_U2][sensorNum].frameCount--;

        //处理下绝对值编码器警报
        if (SENSOR_ON == g_sensorAlarm.encAlarm[sensorNum].state)
        {
            eventAbsEncLimitProcess(sensorNum, 
                                    g_sensorData.reporterData[sensorNum][REPTTYPE_ABSENC] & ABS_ENCODER_VALID_BIT_WIDTH,
                                    g_sensorAlarm.encAlarmResponse);
        }
        
    }
}
#endif


/*********************************************************************************************
函 数 名: eventRunTimeCountTimerCB
实现功能: 
输入参数: 
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventRunTimeCountTimerCB(void *timeOutPara)
{
    u8 chanNum = *(u8 *)timeOutPara;


    g_debugInfo.runTime[chanNum] += PERIOD_TASK_PERIOD + 1;
}


/*********************************************************************************************
函 数 名: eventInfoGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventInfoGet(void)
{
    memcpy(&eventSrcBmp, &g_eventSrcBmp, sizeof(g_eventSrcBmp));
    memset(&g_eventSrcBmp, 0, sizeof(g_eventSrcBmp));
}


/*********************************************************************************************
函 数 名: eventProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void eventProcess(void)
{
    u8   i;
    u8   uartNum;
    u8   readState;
    IntrSourceUnion  intrSource;

    
    //关中断
    CPU_IntDis();
    
    //获取数据
    eventInfoGet();

    //开中断
    CPU_IntEn();


    //FPGA中断
    if (eventSrcBmp.bFpgaInt)
    {        
        for (i = 0;i < CH_TOTAL;i++)
        {
            OSMutexPend(&g_mutexSdioInterface, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);
            readState = servFpgaIntrSourceGet(i, &intrSource.intrSrcValue);
            OSMutexPost(&g_mutexSdioInterface, OS_OPT_POST_FIFO, NULL);

            if (!readState)
            {
                if (intrSource.intrSrc.stopSwitch)
                {
                    //如果允许上报触发状态则上报触发状态    TO ADD NICK
                }

                //波表输出结束
                if (intrSource.intrSrc.waveEndState)
                {
                    servFpgaWaveEndIntClear(i);
                    
                    OSMutexPend(&g_mutexSdioInterface, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);
                    eventWaveEndStateProcess(i);
                    OSMutexPost(&g_mutexSdioInterface, OS_OPT_POST_FIFO, NULL);
                }

                //丢步告警
                if (intrSource.intrSrc.outOfStep)
                {
                    eventOutOfStepProcess(i, g_planInfo.plan[i][g_systemState.outpWaveTable[i]].outOfStepInfo.lineResp);  
                }

                #if SDIO_CRC_ERROR_ENABLE
                if (intrSource.intrSrc.sdioCrcError)
                {

                }
                #endif
            }
            else
            {
                //添加错误码    TO ADD NICK
            }
        }
    }    //if (eventSrcBmp.bFpgaInt)


    //通道相关
    for (i = 0;i < CH_TOTAL;i++)
    {
        //通道状态监测
        if (eventSrcBmp.bStateMonitor[i])
        {
            eventMotionStateProcess(i);
        }

        //驱动电流切换
        if (eventSrcBmp.bDriverCurr[i])
        {
            //驱动电流和空闲电流的切换
            OSMutexPend(&g_mutexSdioInterface, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);
            eventDriverCurrProcess(i, SENSOR_OFF);
            OSMutexPost(&g_mutexSdioInterface, OS_OPT_POST_FIFO, NULL);
        }

        //反向间隙读取
        if (eventSrcBmp.backLashRead[i])
        {
            OSMutexPend(&g_mutexSdioInterface, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);
            eventBackLashRead(i);
            OSMutexPost(&g_mutexSdioInterface, OS_OPT_POST_FIFO, NULL);
        }

#if !GELGOOG_AXIS_10
        //驱动状态读取
        if (eventSrcBmp.drvStateRead[i])
        {
            OSMutexPend(&g_mutexSdioInterface, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);
            eventDriverStateReadProcess(i);
            OSMutexPost(&g_mutexSdioInterface, OS_OPT_POST_FIFO, NULL);
        } 
#endif
    }

    //设备识别
    if (eventSrcBmp.bDeviceId)
    {
        if (DISTING_ON == g_systemState.distState)
        {
            servLedSet(LED_DEVICEID);
        }
        else
        {
            servLedSet((LedStatusEnum)g_systemState.systemState);
        }
    }

#if GELGOOG_AXIS_10
    if (eventSrcBmp.bDriverFault)
    {
        OSMutexPend(&g_mutexSdioInterface, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);
        eventDriverFaultProcess(NULL);
        OSMutexPost(&g_mutexSdioInterface, OS_OPT_POST_FIFO, NULL);
    }
#endif

    //处理传感器串口数据
    for (uartNum = 0;uartNum < UARTNUM_RESERVE;uartNum++)
    {
        for (i = 0;i < SENSOR_RESERVE;i++)
        {
            if (eventSrcBmp.bSenUart[uartNum][i])
            {
                eventSensorUartProcess((SenUartNumEnum)uartNum, (SensorNumEnum)i);
            }
        }
    }

    //处理测距传感器数据
    for (i = 0;i < DIST_SENSOR_NUM;i++)
    {
        if (eventSrcBmp.bDistSen[i])
        {
            eventDistSensorProcess((SensorNumEnum)i);
        }
    }

#if GELGOOG_SINANJU

    //绝对值编码器    CJ 2018.02.28 Add
    for (i = 0;i < ABS_ENCODER_NUM;i++)
    {
        if (eventSrcBmp.bAbsEncoder[i])
        {
            eventAbsEncoderProcess((SensorNumEnum)i);
        } 
    }
    
#endif

#ifdef PROJECT_GELGOOG
#if !GELGOOG_SINANJU
    if (eventSrcBmp.bXInMatch)
    {
        eventIsolInMonitorProcess();
    }
    
#else

    if (eventSrcBmp.distAlarmLed)
    {
        eventDistAlarmLedProcess(false);
    }
#endif
#endif
}


/*********************************************************************************************
函 数 名: EventManageTask;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void EventManageTask(void *p_arg)
{
    OS_ERR  os_err;
    
    
    while (1)
    {
        //等待信号量
        OSSemPend(&g_semEventManageTask, 0, OS_OPT_PEND_BLOCKING, NULL, &os_err);

        eventProcess();

        if (bEventPostSemToFunc)
        {
            bEventPostSemToFunc = false;
            
            OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);
        }

        if (bEventPostSemToCmd)
        {
            bEventPostSemToCmd = false;

            OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, &os_err);
        }
    }
}



/*******************************************文件尾********************************************/
