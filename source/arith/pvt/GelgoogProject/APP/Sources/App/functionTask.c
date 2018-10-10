/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  functiontask.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.02.07;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include <os.h>
#include "project.h"
#include "functiontask.h"
#include "servCommIntfc.h"
#include "servFpga.h"
#include "servDriver.h"
#include "servSoftTimer.h"
#include "eventManageTask.h"

#ifdef PROJECT_QUBELEY
#include "servPT100.h"
#endif



/****************************************外部变量声明*****************************************/
extern OS_SEM g_semFunctionTask;
extern OS_SEM g_semEventManageTask;

extern OS_MUTEX  g_mutexSdioInterface;

extern CommIntfcStruct   g_commIntfc;
extern OutputDataStruct  g_outputData[CH_TOTAL];
extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;
extern EventSrcBmpStruct g_eventSrcBmp;
extern DebugInfoStruct   g_debugInfo;

extern TrigInInfoStruct  g_trigInInfo;

extern MotorInfoStruct   g_motorInfo;
extern MotionInfoStruct  g_motionInfo;
extern ReportInfoStruct  g_reportInfo;

extern DriverInfoStruct  g_driverInfo;

#ifdef PROJECT_QUBELEY
extern AnalogInfoStruct g_analogInfo;
#endif

extern SensorDataStruct  g_sensorData;
extern SoftTimerStruct   g_motionMonitorTimer[CH_TOTAL];
extern WaveTableStruct   g_waveTable[CH_TOTAL][WTTYPE_RESERVE];
extern SoftTimerStruct   g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];

extern DigitalOutStruct  g_digitalOut;
extern IsolatorOutStruct g_isolatorOut;
extern SensorUartStruct  g_sensorUart;

extern ChanCfgBmpStruct   g_chanCfgBmp[CH_TOTAL];
extern SystemCfgBmpStruct g_systemCfgBmp;

extern PlanInfoStruct    g_planInfo;

extern SoftTimerStruct g_runTimeCountTimer[CH_TOTAL];



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/
typedef struct
{    
    StartSourceEnum  startSrc[CH_TOTAL];         //启动源
    ReceiveTypeEnum  startType[CH_TOTAL];        //接收ID类型
    
    MicroStepEnum    microSteps[CH_TOTAL];       //微步数

    IntfcStateEnum   encoderState[CH_TOTAL];
    EncoderMultiEnum encoderMult[CH_TOTAL];
    EncoderChanEnum  encoderChan[CH_TOTAL];

    SensorStateEnum    revMotion;    //反向运行

    WaveTableTypeEnum  calcWaveTable[CH_TOTAL];    //当前解算的波表
    WaveTableTypeEnum  outpWaveTable[CH_TOTAL];    //当前输出的波表

#if !GELGOOG_AXIS_10 
    DriverManageStruct  driverManage[CH_TOTAL];
    DriverMonitorStruct driverMont[CH_TOTAL];
#else
    DriverInfoStruct driverInfo;
#endif

    TrigInManageStruct  triggerInfo[CH_TOTAL];
    
    DigitalOutManageStruct  digitalIO[DIO_RESERVE];
    IsolatorOutManageStruct isolatorIO[YOUT_RESERVE];
    UartIntfcStruct         sensorUart[UARTNUM_RESERVE];

    PlanManageStruct wavePlanInfo[CH_TOTAL];

    PosnConvertInfoStruct posnConvertInfo[CH_TOTAL]; 

#ifdef PROJECT_QUBELEY
    AnalogInfoStruct asensorInfo;
#endif

#if GELGOOG_SINANJU
    PdmInfoStruct pdmInfo[CH_TOTAL];
#endif

    CanIntfcStruct    canIntfc;
    
    WaveTableStruct   *pCalcWaveTable[CH_TOTAL];
    WaveTableStruct   *pOutpWaveTable[CH_TOTAL];
      
}FuncCfgInfoStruct;



/******************************************局部变量*******************************************/
ChanCfgBmpStruct   funcChanCfgBmp[CH_TOTAL];
SystemCfgBmpStruct funcSystemCfgBmp;
FuncCfgInfoStruct  funcCfgInfo; 

bool bFuncPostSemToEvent;



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: FuncCfgInfoGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void FuncCfgInfoGet(void)
{
    u8 i;


    memcpy(&funcSystemCfgBmp, &g_systemCfgBmp, sizeof(SystemCfgBmpStruct));
    memset(&g_systemCfgBmp, 0, sizeof(SystemCfgBmpStruct));
    

    for (i = 0;i < CH_TOTAL;i++)
    {
        memcpy(&funcChanCfgBmp[i], &g_chanCfgBmp[i], sizeof(ChanCfgBmpStruct));
        memset(&g_chanCfgBmp[i], 0, sizeof(ChanCfgBmpStruct));
    
        funcCfgInfo.microSteps[i]   = g_motorInfo.motor[i].microSteps;
        funcCfgInfo.encoderMult[i]  = g_motorInfo.motor[i].encoderMult;
        funcCfgInfo.encoderChan[i]  = g_motorInfo.motor[i].encoderChanNum;
        funcCfgInfo.encoderState[i] = g_motorInfo.motor[i].encoderState;

        funcCfgInfo.startSrc[i]  = g_motionInfo.motion[i].startSrc;
        funcCfgInfo.startType[i] = g_motionInfo.motion[i].startType;
        
        funcCfgInfo.calcWaveTable[i] = g_systemState.calcWaveTable[i];
        funcCfgInfo.outpWaveTable[i] = g_systemState.outpWaveTable[i];
        
        //是否用本地变量代替? 但是状态切换时还是需要修改全局变量中的值...
        funcCfgInfo.pCalcWaveTable[i] = &g_waveTable[i][g_systemState.calcWaveTable[i]];
        funcCfgInfo.pOutpWaveTable[i] = &g_waveTable[i][g_systemState.outpWaveTable[i]];
        
        funcCfgInfo.wavePlanInfo[i] = g_planInfo.plan[i][g_systemState.outpWaveTable[i]];
        
        funcCfgInfo.posnConvertInfo[i] = g_systemState.posnConvertInfo[i];
        
        funcCfgInfo.triggerInfo[i]  = g_trigInInfo.trigIn[i];

#if !GELGOOG_AXIS_10
        funcCfgInfo.driverManage[i] = g_driverInfo.driver[i];

        
        funcCfgInfo.driverMont[i].energyState = g_reportInfo.report[i].state[REPTTYPE_TORQUE];  
        funcCfgInfo.driverMont[i].sgAllState  = g_reportInfo.report[i].state[REPTTYPE_DRVSGALL];
        funcCfgInfo.driverMont[i].sgseState   = g_reportInfo.report[i].state[REPTTYPE_DRVSGSE];

        funcCfgInfo.driverMont[i].csValue   = g_driverInfo.driver[i].SGCSCONF.regBitFiled.CS;

        funcCfgInfo.driverMont[i].energyValue = &g_sensorData.reporterData[i][REPTTYPE_TORQUE];
        funcCfgInfo.driverMont[i].sgAllValue  = &g_sensorData.reporterData[i][REPTTYPE_DRVSGALL]; 
        funcCfgInfo.driverMont[i].sgseValue   = &g_sensorData.reporterData[i][REPTTYPE_DRVSGSE]; 

#else
        funcCfgInfo.driverInfo = g_driverInfo;
#endif 

#if GELGOOG_SINANJU
        funcCfgInfo.pdmInfo[i] = g_systemState.pdmInfo[i];
#endif
    }
    
    memcpy(funcCfgInfo.digitalIO, g_digitalOut.output, sizeof(g_digitalOut.output));
    
    memcpy(funcCfgInfo.isolatorIO, g_isolatorOut.output, sizeof(g_isolatorOut.output));
    
    memcpy(funcCfgInfo.sensorUart, g_sensorUart.uartIntfc, sizeof(g_sensorUart.uartIntfc));
    
#ifdef PROJECT_QUBELEY
    funcCfgInfo.asensorInfo = g_analogInfo;
#endif

    funcCfgInfo.revMotion = g_systemInfo.revMotion;

    funcCfgInfo.canIntfc = g_commIntfc.canIntfc; 
}


/*********************************************************************************************
函 数 名: FuncCfgTask;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void FuncCfgTask(void)
{
    u8  i;

    
    //关中断
    CPU_IntDis();
    
    //获取数据
    FuncCfgInfoGet();

    //开中断
    CPU_IntEn();


    //等待互斥信号量
    OSMutexPend(&g_mutexSdioInterface, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);

    /************************************通道相关********************************************/
    for (i = 0;i < CH_TOTAL;i++)
    {
        //写波表
        if (funcChanCfgBmp[i].bWaveTableData)
        {
            if ((MTSTATE_IDLE == funcCfgInfo.pCalcWaveTable[i]->waveState) &&
                (CHSTATE_POWERON != g_systemState.chanState[i]))
            {
                g_systemState.bStateSwitch[i][funcCfgInfo.calcWaveTable[i]] = true;

                //通知事件管理发生了状态切换
                g_eventSrcBmp.bStateMonitor[i] = true;
                bFuncPostSemToEvent = true;
            }
            
            servFpgaWaveDataWrite(i, &g_outputData[i], funcCfgInfo.pCalcWaveTable[i]);
        }
        
        //解算并发送波表结束
        if (funcChanCfgBmp[i].bQryReady)    //CJ 2017.06.19 Modify
        {
            bspDelayMs(5);  //等待5ms保证最后的数据下载到内存中
            
            funcCfgInfo.pCalcWaveTable[i]->waveState = MTSTATE_CALCEND;

            g_systemState.bStateSwitch[i][funcCfgInfo.calcWaveTable[i]] = true;

            //触发状态监控事件
            g_eventSrcBmp.bStateMonitor[i] = true;
            bFuncPostSemToEvent = true;
        }

        //运动准备的实际意义在于让逻辑进行数据预取,待上位机发送运行命令后可以立即执行波表以达到最好的同步效果 cp 170310
        if (funcChanCfgBmp[i].bPrepare)   
        {
            //预取前需要将运行相关的所有参数配置完成

            //置位循环数计数
            g_sensorData.reporterData[i][REPTTYPE_CYCLE] = 1;

            g_debugInfo.runTime[i] = 0;

            //删除运动监控定时器
            servStimerDelete(&g_motionMonitorTimer[i]);

            //预取配置
            servFpgaWavePrepareConfig(i, 
                                      funcCfgInfo.pOutpWaveTable[i], 
                                      funcCfgInfo.wavePlanInfo[i], 
                                      funcCfgInfo.posnConvertInfo[i]);

            //配置下驱动电流
            eventDriverCurrProcess(i, SENSOR_ON);

            //开始预取
            if (servFpgaReadyQuery(i, *funcCfgInfo.pOutpWaveTable[i]))
            {
                if (funcCfgInfo.pOutpWaveTable[i]->waveWorkMode != WTWORKMODE_FIFO)
                {
                    funcCfgInfo.pOutpWaveTable[i]->lastState = MTSTATE_CALCEND;    //主流程中进入预取必然是从计算结束进入的    CJ 2017.11.22 Modify
                }
                else
                {
                    funcCfgInfo.pOutpWaveTable[i]->lastState = MTSTATE_IDLE;    //CJ 2017.12.24 Add
                }
                
                funcCfgInfo.pOutpWaveTable[i]->waveState = MTSTATE_STANDBY;
            }
            else
            {
                funcCfgInfo.pOutpWaveTable[i]->waveState = MTSTATE_ERROR;

                g_systemState.chanState[i] = CHSTATE_ERROR;
            }

            g_systemState.bStateSwitch[i][funcCfgInfo.outpWaveTable[i]] = true;

            //触发状态监控事件
            g_eventSrcBmp.bStateMonitor[i] = true;
            bFuncPostSemToEvent = true;
        }

        if (funcChanCfgBmp[i].bEmergStop)
        {
            //紧急停止
            servFpgaWaveEmergStop(i);    //FOR MODIFY NICK
        }

        if (funcChanCfgBmp[i].bStop)
        {
            servFpgaWaveStop(i, funcCfgInfo.pOutpWaveTable[i]->stopConfigInfo);
        }

        if (funcChanCfgBmp[i].bCycleQry)
        {
            servFPGA_Cycle_Cnt_Get(i, &g_sensorData.reporterData[i][REPTTYPE_CYCLE]);
        }
    
        if (funcChanCfgBmp[i].bReportTorque)    //上报力矩
        {
            //servTmcReadSelectedReg(&g_sensorData.reporterData[i][REPTTYPE_TORQUE]);
        }

        if (funcChanCfgBmp[i].bEncoder)
        {
            servFpgaEncoderSet(i, funcCfgInfo.encoderState[i], 
                                  funcCfgInfo.encoderMult[i], 
                                  funcCfgInfo.encoderChan[i]);
        }

#if 0    //预取的时候配置
        if (funcChanCfgBmp[i].bOutOfStep)
        {
            servFpgaLineOutOfStepSet(i, funcCfgInfo.wavePlanInfo[i].outOfStepInfo.lineState, 
                                        funcCfgInfo.lineSteps[i], 
                                        funcCfgInfo.wavePlanInfo[i].outOfStepInfo.lineOutNum);
        }
#endif

        if (funcChanCfgBmp[i].bTrigIn)    //配置Trigger
        {
            servFpgaTriggerSet(i, funcCfgInfo.triggerInfo[i], funcCfgInfo.encoderChan[i]);
        }

        if (funcChanCfgBmp[i].bStartSrc)
        {
            servFpgaStartSourceSet(i, funcCfgInfo.startSrc[i]);
        }

#if !GELGOOG_AXIS_10

        if (funcChanCfgBmp[i].bDriverState)
        {
            servDriverSwitch(i, funcCfgInfo.driverManage[i]);
        }

        if (funcChanCfgBmp[i].bMicroStep)
        {
            servDriverMicroStepSet(i, funcCfgInfo.driverManage[i]);
        }

        if (funcChanCfgBmp[i].bCurrent)
        {
            servDriverCurrentSet(i, funcCfgInfo.driverManage[i]);
        }

        if (funcChanCfgBmp[i].bDriverQuery)
        {
            servDriverStateMonitor(i, funcCfgInfo.driverMont[i]);
        }

        if (funcChanCfgBmp[i].bDrvCtrl)
        {
            servDriverRegWrite(i, ADDR_TMC_DRVCTRL, funcCfgInfo.driverManage[i].DRVCTRL.regValue);
        }

        if (funcChanCfgBmp[i].bChopConfig)
        {
            servDriverRegWrite(i, ADDR_TMC_CHOPCONF, funcCfgInfo.driverManage[i].CHOPCONF.regValue);
        }

        if (funcChanCfgBmp[i].bSmarten)
        {
            servDriverRegWrite(i, ADDR_TMC_SMARTEN, funcCfgInfo.driverManage[i].SMARTEN.regValue);
        }

        if (funcChanCfgBmp[i].bSgcsConfig)
        {
            servDriverRegWrite(i, ADDR_TMC_SGCSCONF, funcCfgInfo.driverManage[i].SGCSCONF.regValue);
        }

        if (funcChanCfgBmp[i].bDrvConfig)
        {
            servDriverRegWrite(i, ADDR_TMC_DRVCONF, funcCfgInfo.driverManage[i].DRVCONF.regValue);
        }

#else
        if (funcChanCfgBmp[i].bDriverState)
        {
            servDriverSwitch(i, funcCfgInfo.driverInfo.state[i]);
        }
#endif

        if (funcChanCfgBmp[i].bBlReset)
        {
            servFpgaBackLashReset((BackLashChanEnum)i);
        }

#if GELGOOG_SINANJU
        if (funcChanCfgBmp[i].bPdmSample)
        {
            servFpgaPdmSampleStateSet((SampleChanEnum)i, funcCfgInfo.pdmInfo[i]);
        }

        if (funcChanCfgBmp[i].bPdmDataRead)
        {
            servFpgaPdmMstepDataProcess((SampleChanEnum)i, funcCfgInfo.pdmInfo[i]);
        }
#endif

#if 0    //暂时屏蔽了，统一在预取时配置
        if (funcChanCfgBmp[i].bMotnMode)   
        {
            servFpgaWaveCalcModeSet(i, funcCfgInfo.pOutpWaveTable[i]->motionMode);
        }

        if (funcChanCfgBmp[i].bModifyDuty)   
        {
            servFpgaWaveModifyDutySet(i, funcCfgInfo.pOutpWaveTable[i]->modifyDuty);
        }

        if (funcChanCfgBmp[i].bLineMult)   
        {
            servFpgaWaveLineMultSet(i, funcCfgInfo.pOutpWaveTable[i]->lineMult);
        }
#endif
    }
    /************************************通道相关********************************************/
    
    if (funcSystemCfgBmp.bRun)
    {
        servFpgaWaveStart(); 
        
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (MTSTATE_STANDBY == funcCfgInfo.pOutpWaveTable[i]->waveState)
            {
                //进入到预取状态的通道都会被开启运行，但是需要查询下是否真的进入到了运行状态    CJ 2017.11.22 Modify
                if (RUNSTETE_RUN != servFpgaRunStateGet(i))
                {
                    funcCfgInfo.pOutpWaveTable[i]->waveState = MTSTATE_ERROR;

                    g_systemState.chanState[i] = CHSTATE_ERROR;
                }
                else
                {
                    funcCfgInfo.pOutpWaveTable[i]->waveState = MTSTATE_OUTPUTING;

                    g_systemState.chanState[i] = CHSTATE_OUTPUTING;

                    //开启循环数查询定时器
                    if (funcCfgInfo.pOutpWaveTable[i]->bCycleQuery)
                    {
                        servStimerAdd(&g_reportTimer[i][REPTTYPE_CYCLE]);
                    }

                    servStimerAdd(&g_runTimeCountTimer[i]);
                }
                
                g_systemState.bStateSwitch[i][funcCfgInfo.outpWaveTable[i]] = true;

                //触发状态监控事件
                g_eventSrcBmp.bStateMonitor[i] = true;
                bFuncPostSemToEvent = true;
            }
            else
            {
                //报错
            }
        }
    }

    if (funcSystemCfgBmp.bDigitalOut1)
    {
        servFpgaDigitalOutSet(DIO_DO1, funcCfgInfo.digitalIO[DIO_DO1]);
    }
    
    if (funcSystemCfgBmp.bDigitalOut2)
    {
        servFpgaDigitalOutSet(DIO_DO2, funcCfgInfo.digitalIO[DIO_DO2]);
    }

#ifdef PROJECT_GELGOOG
    if (funcSystemCfgBmp.bDigitalIO3)
    {
        servFpgaDigitalOutSet(DIO_DO3, funcCfgInfo.digitalIO[DIO_DO3]);
    }
    
    if (funcSystemCfgBmp.bDigitalIO4)
    {
        servFpgaDigitalOutSet(DIO_DO4, funcCfgInfo.digitalIO[DIO_DO4]);
    }
#endif

    if (funcSystemCfgBmp.bIsolatorOut1)
    {
        servFpgaIsolatorOutSet(YOUT_YO1, funcCfgInfo.isolatorIO[YOUT_YO1]);
    }

    if (funcSystemCfgBmp.bIsolatorOut2)
    {
        servFpgaIsolatorOutSet(YOUT_YO2, funcCfgInfo.isolatorIO[YOUT_YO2]);
    }

    if (funcSystemCfgBmp.bClockReg)    //配置时钟同步寄存器
    {
        servFpgaClockSyncSet(g_systemState.clockSyncReg, 
                             g_systemState.clockSyncType, 
                             funcCfgInfo.canIntfc);
    }
    
    if (funcSystemCfgBmp.bClockCount)
    {
        //获取计数值，同步状态切换为结束
        g_systemState.clockCount     = servFpgaClockSyncCountRead();
        g_systemState.clockSyncState = CLOCKSYNC_END;
    }

    if (funcSystemCfgBmp.bRevMotion)
    {
        servFpgaReverseMotionSet(funcCfgInfo.revMotion);
    }

#if GELGOOG_AXIS_10
    if (funcSystemCfgBmp.bDriverMode)
    {
        //驱动芯片细分
        servFpgaDriverModeSet(funcCfgInfo.driverInfo.microStep);
    }
    
    if (funcSystemCfgBmp.bDriverCurr)
    {
        //驱动芯片电流
        servDriverCurrentConfig(funcCfgInfo.driverInfo.curr);
    }
#endif

    //释放互斥信号量
    OSMutexPost(&g_mutexSdioInterface, OS_OPT_POST_FIFO, NULL);


    if (funcSystemCfgBmp.bSensorUart1)
    {
        servSensor1UartConfig(funcCfgInfo.sensorUart[UARTNUM_U1]);
    }

#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4轴和10轴只支持1路    
    if (funcSystemCfgBmp.bSensorUart2)
    {
        servSensor2UartConfig(funcCfgInfo.sensorUart[UARTNUM_U2]);
    }
#endif

    if (funcSystemCfgBmp.bAnalogIn)
    {
#ifdef PROJECT_QUBELEY
        servAnalogInSet(funcCfgInfo.asensorInfo);
#endif
    }

    if (funcSystemCfgBmp.bQryTemp)
    {
#ifdef PROJECT_QUBELEY    
        g_sensorData.temperature = servGetPT100Value();    //温度精度0.01°
#endif
    }
}


/*********************************************************************************************
函 数 名: FunctionTask;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void FunctionTask(void *p_arg)
{
    OS_ERR  os_err;


    while (1)
    {
        OSSemPend(&g_semFunctionTask, 0, OS_OPT_PEND_BLOCKING, NULL, &os_err);

        FuncCfgTask();

        if (bFuncPostSemToEvent)
        {
            OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, &os_err);

            bFuncPostSemToEvent = false;
        }
    }
}



/*******************************************文件尾********************************************/
