/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  functiontask.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.02.07;
��ʷ�汾:  ��;
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



/****************************************�ⲿ��������*****************************************/
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



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/
typedef struct
{    
    StartSourceEnum  startSrc[CH_TOTAL];         //����Դ
    ReceiveTypeEnum  startType[CH_TOTAL];        //����ID����
    
    MicroStepEnum    microSteps[CH_TOTAL];       //΢����

    IntfcStateEnum   encoderState[CH_TOTAL];
    EncoderMultiEnum encoderMult[CH_TOTAL];
    EncoderChanEnum  encoderChan[CH_TOTAL];

    SensorStateEnum    revMotion;    //��������

    WaveTableTypeEnum  calcWaveTable[CH_TOTAL];    //��ǰ����Ĳ���
    WaveTableTypeEnum  outpWaveTable[CH_TOTAL];    //��ǰ����Ĳ���

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



/******************************************�ֲ�����*******************************************/
ChanCfgBmpStruct   funcChanCfgBmp[CH_TOTAL];
SystemCfgBmpStruct funcSystemCfgBmp;
FuncCfgInfoStruct  funcCfgInfo; 

bool bFuncPostSemToEvent;



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: FuncCfgInfoGet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        
        //�Ƿ��ñ��ر�������? ����״̬�л�ʱ������Ҫ�޸�ȫ�ֱ����е�ֵ...
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
�� �� ��: FuncCfgTask;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void FuncCfgTask(void)
{
    u8  i;

    
    //���ж�
    CPU_IntDis();
    
    //��ȡ����
    FuncCfgInfoGet();

    //���ж�
    CPU_IntEn();


    //�ȴ������ź���
    OSMutexPend(&g_mutexSdioInterface, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);

    /************************************ͨ�����********************************************/
    for (i = 0;i < CH_TOTAL;i++)
    {
        //д����
        if (funcChanCfgBmp[i].bWaveTableData)
        {
            if ((MTSTATE_IDLE == funcCfgInfo.pCalcWaveTable[i]->waveState) &&
                (CHSTATE_POWERON != g_systemState.chanState[i]))
            {
                g_systemState.bStateSwitch[i][funcCfgInfo.calcWaveTable[i]] = true;

                //֪ͨ�¼���������״̬�л�
                g_eventSrcBmp.bStateMonitor[i] = true;
                bFuncPostSemToEvent = true;
            }
            
            servFpgaWaveDataWrite(i, &g_outputData[i], funcCfgInfo.pCalcWaveTable[i]);
        }
        
        //���㲢���Ͳ������
        if (funcChanCfgBmp[i].bQryReady)    //CJ 2017.06.19 Modify
        {
            bspDelayMs(5);  //�ȴ�5ms��֤�����������ص��ڴ���
            
            funcCfgInfo.pCalcWaveTable[i]->waveState = MTSTATE_CALCEND;

            g_systemState.bStateSwitch[i][funcCfgInfo.calcWaveTable[i]] = true;

            //����״̬����¼�
            g_eventSrcBmp.bStateMonitor[i] = true;
            bFuncPostSemToEvent = true;
        }

        //�˶�׼����ʵ�������������߼���������Ԥȡ,����λ����������������������ִ�в����Դﵽ��õ�ͬ��Ч�� cp 170310
        if (funcChanCfgBmp[i].bPrepare)   
        {
            //Ԥȡǰ��Ҫ��������ص����в����������

            //��λѭ��������
            g_sensorData.reporterData[i][REPTTYPE_CYCLE] = 1;

            g_debugInfo.runTime[i] = 0;

            //ɾ���˶���ض�ʱ��
            servStimerDelete(&g_motionMonitorTimer[i]);

            //Ԥȡ����
            servFpgaWavePrepareConfig(i, 
                                      funcCfgInfo.pOutpWaveTable[i], 
                                      funcCfgInfo.wavePlanInfo[i], 
                                      funcCfgInfo.posnConvertInfo[i]);

            //��������������
            eventDriverCurrProcess(i, SENSOR_ON);

            //��ʼԤȡ
            if (servFpgaReadyQuery(i, *funcCfgInfo.pOutpWaveTable[i]))
            {
                if (funcCfgInfo.pOutpWaveTable[i]->waveWorkMode != WTWORKMODE_FIFO)
                {
                    funcCfgInfo.pOutpWaveTable[i]->lastState = MTSTATE_CALCEND;    //�������н���Ԥȡ��Ȼ�ǴӼ�����������    CJ 2017.11.22 Modify
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

            //����״̬����¼�
            g_eventSrcBmp.bStateMonitor[i] = true;
            bFuncPostSemToEvent = true;
        }

        if (funcChanCfgBmp[i].bEmergStop)
        {
            //����ֹͣ
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
    
        if (funcChanCfgBmp[i].bReportTorque)    //�ϱ�����
        {
            //servTmcReadSelectedReg(&g_sensorData.reporterData[i][REPTTYPE_TORQUE]);
        }

        if (funcChanCfgBmp[i].bEncoder)
        {
            servFpgaEncoderSet(i, funcCfgInfo.encoderState[i], 
                                  funcCfgInfo.encoderMult[i], 
                                  funcCfgInfo.encoderChan[i]);
        }

#if 0    //Ԥȡ��ʱ������
        if (funcChanCfgBmp[i].bOutOfStep)
        {
            servFpgaLineOutOfStepSet(i, funcCfgInfo.wavePlanInfo[i].outOfStepInfo.lineState, 
                                        funcCfgInfo.lineSteps[i], 
                                        funcCfgInfo.wavePlanInfo[i].outOfStepInfo.lineOutNum);
        }
#endif

        if (funcChanCfgBmp[i].bTrigIn)    //����Trigger
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

#if 0    //��ʱ�����ˣ�ͳһ��Ԥȡʱ����
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
    /************************************ͨ�����********************************************/
    
    if (funcSystemCfgBmp.bRun)
    {
        servFpgaWaveStart(); 
        
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (MTSTATE_STANDBY == funcCfgInfo.pOutpWaveTable[i]->waveState)
            {
                //���뵽Ԥȡ״̬��ͨ�����ᱻ�������У�������Ҫ��ѯ���Ƿ���Ľ��뵽������״̬    CJ 2017.11.22 Modify
                if (RUNSTETE_RUN != servFpgaRunStateGet(i))
                {
                    funcCfgInfo.pOutpWaveTable[i]->waveState = MTSTATE_ERROR;

                    g_systemState.chanState[i] = CHSTATE_ERROR;
                }
                else
                {
                    funcCfgInfo.pOutpWaveTable[i]->waveState = MTSTATE_OUTPUTING;

                    g_systemState.chanState[i] = CHSTATE_OUTPUTING;

                    //����ѭ������ѯ��ʱ��
                    if (funcCfgInfo.pOutpWaveTable[i]->bCycleQuery)
                    {
                        servStimerAdd(&g_reportTimer[i][REPTTYPE_CYCLE]);
                    }

                    servStimerAdd(&g_runTimeCountTimer[i]);
                }
                
                g_systemState.bStateSwitch[i][funcCfgInfo.outpWaveTable[i]] = true;

                //����״̬����¼�
                g_eventSrcBmp.bStateMonitor[i] = true;
                bFuncPostSemToEvent = true;
            }
            else
            {
                //����
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

    if (funcSystemCfgBmp.bClockReg)    //����ʱ��ͬ���Ĵ���
    {
        servFpgaClockSyncSet(g_systemState.clockSyncReg, 
                             g_systemState.clockSyncType, 
                             funcCfgInfo.canIntfc);
    }
    
    if (funcSystemCfgBmp.bClockCount)
    {
        //��ȡ����ֵ��ͬ��״̬�л�Ϊ����
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
        //����оƬϸ��
        servFpgaDriverModeSet(funcCfgInfo.driverInfo.microStep);
    }
    
    if (funcSystemCfgBmp.bDriverCurr)
    {
        //����оƬ����
        servDriverCurrentConfig(funcCfgInfo.driverInfo.curr);
    }
#endif

    //�ͷŻ����ź���
    OSMutexPost(&g_mutexSdioInterface, OS_OPT_POST_FIFO, NULL);


    if (funcSystemCfgBmp.bSensorUart1)
    {
        servSensor1UartConfig(funcCfgInfo.sensorUart[UARTNUM_U1]);
    }

#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4���10��ֻ֧��1·    
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
        g_sensorData.temperature = servGetPT100Value();    //�¶Ⱦ���0.01��
#endif
    }
}


/*********************************************************************************************
�� �� ��: FunctionTask;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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



/*******************************************�ļ�β********************************************/
