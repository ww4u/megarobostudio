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
#include "cmdMainParse.h"
#include "servCommIntfc.h"
#include "servStreamBuffer.h"



/****************************************外部变量声明*****************************************/
extern OS_SEM g_semEventManageTask;
extern OS_SEM g_semFunctionTask;

extern ChanCfgBmpStruct g_chanCfgBmp[CH_TOTAL];

extern SystemCfgBmpStruct g_systemCfgBmp;

extern SystemStateStruct g_systemState;
extern MotorInfoStruct   g_motorInfo; 
extern MotionInfoStruct  g_motionInfo;
extern WaveTableStruct   g_waveTable[CH_TOTAL][WTTYPE_RESERVE];
extern OutputDataStruct  g_outputData[CH_TOTAL];

extern SoftTimerStruct g_ledTimer;

extern EventSrcBmpStruct g_eventSrcBmp;
extern PlanInfoStruct    g_planInfo;

extern SoftTimerStruct    g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];
extern SensorUartStruct   g_sensorUart;
extern StreamBufferStruct g_senUartRxBuffer[UARTNUM_RESERVE][SENSOR_RESERVE];
extern ReportInfoStruct   g_reportInfo;
extern SensorDataStruct   g_sensorData;

extern bool g_bPerdPostSemToCmd;
extern bool g_bPerdPostSemToEvent;


#ifdef PROJECT_GELGOOG
extern IsolatorInStruct g_isolatorIn;
#endif



/*****************************************局部宏定义******************************************/
#define    INT_SRC_STOP_STATE_MARK       0x003E    //BIT[1:5]
#define    INT_SRC_ALARM_STATE_MARK      0x1F00    //BIT[9:13]
#define    INT_SRC_ALARM_STATE_LSHIFT    9         //左移9bit

#define    ANGLE_SENSOR_FRAME_NUM        3



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
bool bEventPostSemToFunc = false;
EventSrcBmpStruct eventSrcBmp;
SoftTimerStruct g_motionStateTimer[CH_TOTAL];
SoftTimerStruct g_motionMonitorTimer[CH_TOTAL];
SoftTimerStruct g_lineOOStepTimer[CH_TOTAL];

#ifdef PROJECT_GELGOOG
SoftTimerStruct g_isolatorInTimer;
LevelStatusEnum lastLevelStatus;
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
函 数 名: EventMotionStateTimerCB
实现功能: 上传逻辑进行运动准备的结果
输入参数: 
输出参数: 无;
返 回 值: 
说    明: 无;
*********************************************************************************************/
void EventMotionStateTimerCB(void *timeOutPara)
{
    u8 data[3];
    u8 chanNum = *(u8 *)timeOutPara;

    
    data[0] = chanNum;
    data[1] = g_systemState.outpWaveTable[chanNum];
    data[2] = g_waveTable[chanNum][g_systemState.outpWaveTable[chanNum]].waveState;

    cmdFrameSend(CMD_MOTION, MOTIONCMD_STATE, sizeof(data), data);

    //回调函数都是在周期性线程中调用，所以是周期性给命令处理发送信号量
    g_bPerdPostSemToCmd = true;   
    
    //继续开启定时器，收到回复的时候关闭
    if (g_motionStateTimer[chanNum].bUsed)
    {
        servStimerAdd(&g_motionStateTimer[chanNum]);
    }
}


/*********************************************************************************************
函 数 名: EventMotionMonitorTimerCB
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void EventMotionMonitorTimerCB(void *timeOutPara)
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
函 数 名: EventLineOOStepTimerCB
实现功能: 无;
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void EventLineOOStepTimerCB(void *timeOutPara)
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
/*********************************************************************************************
函 数 名: EventIsolInMonitorTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void EventIsolInMonitorTimerCB(void *timeOutPara)
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
#endif


/*********************************************************************************************
函 数 名: EventInfoGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void EventInfoGet(void)
{
    memcpy(&eventSrcBmp, &g_eventSrcBmp, sizeof(g_eventSrcBmp));
    memset(&g_eventSrcBmp, 0, sizeof(g_eventSrcBmp));
}


/*********************************************************************************************
函 数 名: EventProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void EventProcess(void)
{
    u8   i, j;
    u8   uartNum;
    bool bIdle = false;
    bool bCalc = false;
    IntrSourceUnion intrSource;

    
    //关中断
    CPU_IntDis();
    
    //获取数据
    EventInfoGet();

    //开中断
    CPU_IntEn();


    if (eventSrcBmp.bFpgaInt)
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            intrSource.intrSrcValue = servFpgaIntrSourceGet(i);

            if (intrSource.intrSrc.stopSwitch)
            {
                //如果允许上报触发状态则上报触发状态    TO ADD NICK
            }

            //波表输出结束
            if (intrSource.intrSrc.waveEndState)
            {
                //恢复波表状态到上一次
                g_waveTable[i][g_systemState.outpWaveTable[i]].waveState = 
                    g_waveTable[i][g_systemState.outpWaveTable[i]].lastState;

                servFpgaWaveEnable(i, false);

                eventSrcBmp.bStateSwitch[i] = true;

                //等待电机运行彻底停止
                bspDelayMs(10);    
                servFpgaEncoderCountRead(i, &g_systemState.circleNum[i], 
                                            &g_systemState.abCount[i], 
                                            &g_systemState.steps[i]);
                servFpgaEncoderCountReset(i);
                g_systemState.lastAbCount[i] = 0;

                //开启运动监测定时器
                if (SENSOR_ON == g_motionInfo.motion[i].offsetState)
                {
                    servStimerAdd(&g_motionMonitorTimer[i]);
                }
                
                //关闭循环数查询定时器
                servStimerDelete(&g_reportTimer[i][REPTTYPE_CYCLE]);
                g_systemState.cycleNumCount[i] = 0;
            }

            //丢步告警
            if (intrSource.intrSrc.outOfStep)
            {
                switch (g_planInfo.plan[i][g_systemState.outpWaveTable[i]].outOfStepInfo.lineResp)
                {
                    case RESPONSE_NONE:
                      break;
                      
                    case RESPONSE_ALARM:
                      break;
                      
                    case RESPONSE_STOP:
                      break;
                      
                    case RESPONSE_ALARMSTOP:
                      break;

                    default:
                      break;
                }
            }
        }
    }

    for (i = 0;i < g_systemState.chanNum;i++)
    {
        if (eventSrcBmp.bStateSwitch[i])
        {
            //
            bIdle = true;
            bCalc = false;
            for (j = 0;j < WTTYPE_RESERVE;j++)
            {
#if 0
                if ((g_waveTable[i][j].waveState != MTSTATE_IDLE) &&
                    (g_waveTable[i][j].waveState != MTSTATE_CALCEND) &&
                    (g_waveTable[i][j].waveState != MTSTATE_STANDBY))
                {
                    bIdle = false;
                    break;
                }
#else
                //如果是预取切换到输出状态的话，chanState已经变成CHSTATE_OUTPUTING
                //所以这里不考虑CHSTATE_OUTPUTING，只考虑chanState是否要变成IDLE或者CALCING
                if (MTSTATE_OUTPUTING == g_waveTable[i][j].waveState)
                {
                    bIdle = false;
                    bCalc = false;
                    break;
                }
                else if (MTSTATE_CALCING == g_waveTable[i][j].waveState)
                {
                    bCalc = true;
                    bIdle = false;
                }
                else if (((MTSTATE_IDLE    == g_waveTable[i][j].waveState) ||
                          (MTSTATE_CALCEND == g_waveTable[i][j].waveState) ||
                          (MTSTATE_STANDBY == g_waveTable[i][j].waveState)) &&
                         (!bCalc))
                {
                    bIdle = true;
                }
#endif
            }
            if (bCalc)
            {
                g_systemState.chanState[i] = CHSTATE_CALCING;
            }
            else if (bIdle)
            {
                g_systemState.chanState[i] = CHSTATE_IDLE;
            }

            switch (g_systemState.chanState[i])
            {
                case CHSTATE_IDLE:
                    bIdle = true;
                    for (j = 0;j < g_systemState.chanNum;j++)
                    {
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
                  
                case CHSTATE_CALCING:
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
            
            if (CHSTATE_POWERON != g_systemState.lastChanState[i])
            {
                if (STATEQRY_ACTIVE == g_motionInfo.motion[i].stateReport)
                {
                    //上报状态
                    EventMotionStateTimerCB((void *)&i);
                    servStimerAdd(&g_motionStateTimer[i]);
                }
            }
            else
            {
                //上电完成
                g_systemState.lastChanState[i] = g_systemState.chanState[i];

                //开启运动监测定时器
                if (SENSOR_ON == g_motionInfo.motion[i].offsetState)
                {
                    servStimerAdd(&g_motionMonitorTimer[i]);
                }
                servStimerAdd(&g_ledTimer);
            }
        }
    }

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

#ifdef PROJECT_GELGOOG
    if (eventSrcBmp.bXInMatch)
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
#endif

    //处理传感器串口数据
    for (uartNum = 0;uartNum < UARTNUM_RESERVE;uartNum++)
    {
        for (i = 0;i < SENSOR_RESERVE;i++)
        {
            if (eventSrcBmp.bSenUart[uartNum][i])
            {
                //如果Buffer中有旧数据就把旧数据出队，只留新数据在里面
                SensorUartPhyFrameStruct *pUartPhyFrame = NULL;

                if (g_senUartRxBuffer[uartNum][i].frameCount > g_sensorUart.sensor[uartNum][i].recvNum)
                {
                    for (j = 0;j < (g_senUartRxBuffer[uartNum][i].frameCount - g_sensorUart.sensor[uartNum][i].recvNum);j++)
                    {
                        pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[uartNum][i]);
                        servDequeue(&g_senUartRxBuffer[uartNum][i], pUartPhyFrame->frameLen); //处理完成后出队
                        g_senUartRxBuffer[uartNum][i].frameCount--;
                    }

#if GELGOOG_SINANJU

                    //绝对值编码器    CJ 2018.02.28 Add
                    eventSrcBmp.bAbsEncoder[i] = true;
#endif
                }
            }
        }
    }

    //处理测距传感器数据
    for (i = 0;i < DIST_SENSOR_NUM;i++)
    {
        if (eventSrcBmp.bDistSen[i])
        {
            SensorUartPhyFrameStruct  *pUartPhyFrame = NULL;
            DistanceSensorFrameStruct *pSensorFrame  = NULL;

            if (g_senUartRxBuffer[UARTNUM_U1][i].frameCount > 0)    //有数据
            {
                pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[UARTNUM_U1][i]);

                pSensorFrame = (DistanceSensorFrameStruct *)&pUartPhyFrame->SOF;
                g_sensorData.reporterData[i][REPTTYPE_DISTANCE] = (pSensorFrame->dataH << 8) + pSensorFrame->dataL;
                
                servDequeue(&g_senUartRxBuffer[UARTNUM_U1][i], pUartPhyFrame->frameLen); //处理完成后出队
                g_senUartRxBuffer[UARTNUM_U1][i].frameCount--;
            }
        }
    }

#if GELGOOG_SINANJU

    //绝对值编码器    CJ 2018.02.28 Add
    for (i = 0;i < ABS_ENCODER_NUM;i++)
    {
        if (eventSrcBmp.bAbsEncoder[i])
        {
            SensorUartPhyFrameStruct  *pUartPhyFrame = NULL;
            AbsEncoderFrameStruct     *pSensorFrame  = NULL;

            if (g_senUartRxBuffer[UARTNUM_U2][i].frameCount > 0)    //有数据
            {
                pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[UARTNUM_U2][i]);

                pSensorFrame = (AbsEncoderFrameStruct *)&pUartPhyFrame->SOF;
                g_sensorData.reporterData[i][REPTTYPE_ABSENC] = (pSensorFrame->abs2 << 16) + 
                                                                (pSensorFrame->abs1 << 8) + 
                                                                 pSensorFrame->abs0;
                
                servDequeue(&g_senUartRxBuffer[UARTNUM_U2][i], pUartPhyFrame->frameLen); //处理完成后出队
                g_senUartRxBuffer[UARTNUM_U2][i].frameCount--;
            }
        }
    }
    
#else

    //处理角度传感器数据
    for (i = 0;i < ANGLE_SENSOR_NUM;i++)
    {
        if (eventSrcBmp.bAngleSen[i])
        {
            SensorUartPhyFrameStruct *pUartPhyFrame = NULL;
            AngleSensorFrameStruct   *pMpu6050Frame = NULL;

            if (g_senUartRxBuffer[UARTNUM_U2][i].frameCount >= ANGLE_SENSOR_FRAME_NUM)    //角度传感器一包是3帧
            {
                for (j = 0;j < ANGLE_SENSOR_FRAME_NUM;j++)
                {
                    pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[UARTNUM_U2][i]);

                    pMpu6050Frame = (AngleSensorFrameStruct *)&pUartPhyFrame->SOF;
                    switch (pMpu6050Frame->dataType)
                    {
                        case MPUFRAME_ACC:    //加速度
                            g_sensorData.reporterData[i][REPTTYPE_XACC] = pMpu6050Frame->xAxis;
                            g_sensorData.reporterData[i][REPTTYPE_YACC] = pMpu6050Frame->yAxis;
                            g_sensorData.reporterData[i][REPTTYPE_ZACC] = pMpu6050Frame->zAxis;
                          break;

                        case MPUFRAME_VEL:    //速度
                            g_sensorData.reporterData[i][REPTTYPE_XVELOC] = pMpu6050Frame->xAxis;
                            g_sensorData.reporterData[i][REPTTYPE_YVELOC] = pMpu6050Frame->yAxis;
                            g_sensorData.reporterData[i][REPTTYPE_ZVELOC] = pMpu6050Frame->zAxis;
                          break; 

                        case MPUFRAME_ANGLE:    //角度
                            g_sensorData.reporterData[i][REPTTYPE_XANGLE] = pMpu6050Frame->xAxis;
                            g_sensorData.reporterData[i][REPTTYPE_YANGLE] = pMpu6050Frame->yAxis;
                            g_sensorData.reporterData[i][REPTTYPE_ZANGLE] = pMpu6050Frame->zAxis;
                          break;

                        default:
                          break;
                    }
                    
                    servDequeue(&g_senUartRxBuffer[UARTNUM_U2][i], pUartPhyFrame->frameLen); //处理完成后出队
                    g_senUartRxBuffer[UARTNUM_U2][i].frameCount--;
                }
            }
        }
    }
    
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

        EventProcess();

        if (bEventPostSemToFunc)
        {
            OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);

            bEventPostSemToFunc = false;
        }
    }
}



/*******************************************文件尾********************************************/
