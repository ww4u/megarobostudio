/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdMotion.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
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



/****************************************�ⲿ��������*****************************************/
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



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pMotionCmdFunc[MOTIONCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdMotionSwitch;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        
        //ָ������
        if (sizeof(WaveTableTypeEnum) == cmdDataLen)
        {
            waveTable = *(WaveTableTypeEnum *)pCmdData;

            if (waveTable >= WTTYPE_RESERVE)
            {
                g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = PARA_VERIFY_ERROR_TYPE;
                return;
            }
        }
        else    //û��ָ������
        {
            //��ָ������ʱֻ֧��ֹͣ�ͼ�ͣ����
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
        //�ȴ����ŷ�ͨ��
        if (bConfigMrv)
        {
            switch (motnSwitch)
            {
                case MTSWITCH_RESET:
                    //ֱ�ӽ���IDLE״̬
                    if ((MTSTATE_IDLE    == g_mrvWaveTable[waveTable].waveState) ||
                        (MTSTATE_CALCEND == g_mrvWaveTable[waveTable].waveState) ||
                        (MTSTATE_ERROR   == g_mrvWaveTable[waveTable].waveState))
                    {
                        //״̬���˲ŷ��ź���
                        if (g_mrvWaveTable[waveTable].waveState != MTSTATE_IDLE)
                        {
                            g_mrvWaveTable[waveTable].waveState = MTSTATE_IDLE;
                            
                            g_systemState.bStateSwitch[CH_MRV][waveTable] = true;

                            //����״̬����¼�
                            g_eventSrcBmp.bStateMonitor[CH_MRV] = true;
                            g_bCmdPostSemToEvent = true;
                        }
                    }
                    else if ((MTSTATE_CALCING == g_mrvWaveTable[waveTable].waveState))
                    {
                        g_mrvWaveTable[waveTable].waveState = MTSTATE_IDLE;

                        g_systemState.bStateSwitch[CH_MRV][waveTable] = true;

                        //����״̬����¼�
                        g_eventSrcBmp.bStateMonitor[CH_MRV] = true;
                        g_bCmdPostSemToEvent = true;
                    }
                    else if ((MTSTATE_STANDBY == g_mrvWaveTable[waveTable].waveState))
                    {
                        g_mrvWaveTable[waveTable].waveState = MTSTATE_CALCEND;

                        g_systemState.bStateSwitch[CH_MRV][waveTable] = true;

                        //����״̬����¼�
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
                        //��FUNC���ź���
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
                        //��FUNC���ź���
                        g_mrvChanCfgBmp.bStop = true;
                        g_bCmdPostSemToFunc = true;
                    }
                    else
                    {
                        errorCode = PARA_VERIFY_ERROR_TYPE;
                    }
                  break;
                  
                case MTSWITCH_RUN:
                    //ֻ����standby״̬�²��ܽ���running״̬
                    if (MTSTATE_STANDBY == g_mrvWaveTable[waveTable].waveState)
                    {
                        //��FUNC���ź���
                        g_systemCfgBmp.bRun = true;
                        g_bCmdPostSemToFunc = true;
                    }
                    else
                    {
                        errorCode = PARA_VERIFY_ERROR_TYPE;
                    }
                  break;
                  
                case MTSWITCH_PREPARE:
                    //ֻ���ڼ������״̬�²��ܽ���standby״̬
                    //�����״̬�²���Ԥȡ
                    if ((CHSTATE_OUTPUTING != g_systemState.chanState[CH_MRV]) &&
                        (MTSTATE_CALCEND   == g_mrvWaveTable[waveTable].waveState))
                    {
                        //�Ȱѵ�ǰͨ������STANDBY״̬�Ĳ����л��ؼ������״̬
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
                        //ֱ�ӽ���IDLE״̬
                        if ((MTSTATE_IDLE    == pCurrWaveTable->waveState) ||
                            (MTSTATE_CALCEND == pCurrWaveTable->waveState) ||
                            (MTSTATE_ERROR   == pCurrWaveTable->waveState))
                        {
                            //״̬���˲ŷ��ź���
                            if (pCurrWaveTable->waveState != MTSTATE_IDLE)
                            {
                                pCurrWaveTable->waveState = MTSTATE_IDLE;
                                
                                g_systemState.bStateSwitch[i][waveTable] = true;

                                //����״̬����¼�
                                g_eventSrcBmp.bStateMonitor[i] = true;
                                g_bCmdPostSemToEvent = true;

                                //FIFOģʽ���п��ܻ��ڼ���PVT
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

                            //����״̬����¼�
                            g_eventSrcBmp.bStateMonitor[i] = true;
                            g_bCmdPostSemToEvent = true;

                            sysTaskClear(SYSTASK_PVTCALC);
                        }
                        else if ((MTSTATE_STANDBY == pCurrWaveTable->waveState))
                        {
                            pCurrWaveTable->waveState = MTSTATE_CALCEND;

                            g_systemState.bStateSwitch[i][waveTable] = true;

                            //����״̬����¼�
                            g_eventSrcBmp.bStateMonitor[i] = true;
                            g_bCmdPostSemToEvent = true;

                            //FIFOģʽ���п��ܻ��ڼ���PVT
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
                            //��FUNC���ź���
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
                            //��FUNC���ź���
                            g_chanCfgBmp[i].bStop = true;
                            g_bCmdPostSemToFunc = true;
                        }
                        else
                        {
                            errorCode = PARA_VERIFY_ERROR_TYPE;
                        }
                      break;
                      
                    case MTSWITCH_RUN:
                        //ֻ����standby״̬�²��ܽ���running״̬
                        if (MTSTATE_STANDBY == pCurrWaveTable->waveState)
                        {
                            //��FUNC���ź���
                            g_systemCfgBmp.bRun = true;
                            g_bCmdPostSemToFunc = true;
                        }
                        else
                        {
                            errorCode = PARA_VERIFY_ERROR_TYPE;
                        }
                      break;
                      
                    case MTSWITCH_PREPARE:
                        //ֻ���ڼ������״̬�²��ܽ���standby״̬
                        //�����״̬�²���Ԥȡ
                        if ((CHSTATE_OUTPUTING != g_systemState.chanState[i]) &&
                            (MTSTATE_CALCEND   == pCurrWaveTable->waveState))
                        {
                            //�Ȱѵ�ǰͨ������STANDBY״̬�Ĳ����л��ؼ������״̬
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
�� �� ��: cmdMotionStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        if ((sizeof(chanNum) + sizeof(WaveTableTypeEnum)) == cmdDataLen)    //��ѯ
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
        else if ((sizeof(chanNum) + sizeof(MotionStateEnum) + sizeof(WaveTableTypeEnum)) == cmdDataLen)    //��Ӧ
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

                    //�ظ��ĺ͵�ǰ״̬һ����ֹͣ�ϱ�
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
�� �� ��: cmdMotionStateReportSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionStateReportVerify(cmdDataLen, pCmdData, (void *)&stateReport))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].stateReport = stateReport;

                //�ĳɲ�ѯ��ʽʱҪ�رն�ʱ��
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

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionStateReportQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionStartSrcSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionStartSrcVerify(cmdDataLen, pCmdData, (void *)&startSrc))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].startSrc = startSrc;

                //��FUNC���ź���
                g_chanCfgBmp[i].bStartSrc = true;
                g_bCmdPostSemToFunc = true;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionStartSrcQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionStartTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionStartTypeVerify(cmdDataLen, pCmdData, (void *)&startType))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].startType = startType;

                //��FUNC���ź���
                g_chanCfgBmp[i].bStartSrc = true;
                g_bCmdPostSemToFunc = true;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionStartTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionMaxSpeedSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMaxSpeedVerify(cmdDataLen, pCmdData, (void *)&maxSpeed))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].maxSpeed = maxSpeed;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionMaxSpeedQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionMinSpeedSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMinSpeedVerify(cmdDataLen, pCmdData, (void *)&minSpeed))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].minSpeed = minSpeed;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionMinSpeedQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionMaxPositionSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMaxPosnVerify(cmdDataLen, pCmdData, (void *)&maxPosn))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].maxPosn = maxPosn;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionMaxPositionQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionMinPositionSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMinPosnVerify(cmdDataLen, pCmdData, (void *)&minPosn))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].minPosn = minPosn;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionMinPositionQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionMaxTorqueSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMaxTorqueVerify(cmdDataLen, pCmdData, (void *)&maxTorque))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].maxTorque = maxTorque;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionMaxTorqueQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionMinTorqueSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMinTorqueVerify(cmdDataLen, pCmdData, (void *)&minTorque))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].minTorque = minTorque;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionMinTorqueQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionMaxAccSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMaxTorqueVerify(cmdDataLen, pCmdData, (void *)&maxAcc))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].maxAcc = maxAcc;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionMaxAccQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionMinAccSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotionMinTorqueVerify(cmdDataLen, pCmdData, (void *)&minAcc))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motionInfo.motion[i].minAcc = minAcc;
            }
        }

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionMinAccQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionOriginSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    //����EEPROM��
    servStimerAdd(&g_paraSaveTimer);
}


/*********************************************************************************************
�� �� ��: cmdMotionOriginQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionGoOrigin;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionOffsetStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
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

        //����EEPROM��
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotionOffsetStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionCountCircleQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionABCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionABCountClear;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionRunTimeLCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionRunTimeHCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdMotionCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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


/*******************************************�ļ�β********************************************/
