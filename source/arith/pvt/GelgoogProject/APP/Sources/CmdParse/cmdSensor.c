/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdSensor.c��
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
#include "cmdSensor.h"
#include "pvrfSensor.h"
#include "pvrfInterface.h"
#include "cmdMainParse.h"
#include "servSoftTimer.h"
#include "servCommIntfc.h"
#include "servSystemPara.h"
#include "servStreamBuffer.h"



/****************************************�ⲿ��������*****************************************/
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
#endif

extern bool g_bCmdPostSemToFunc;
extern bool g_bPerdPostSemToFunc;
extern bool g_bPerdPostSemToCmd;
extern bool g_bPerdPostSemToEvent;

extern WaveTableStruct  g_waveTable[CH_TOTAL][WTTYPE_RESERVE];

extern UartIntfcStruct    g_sensorUartIntfc[UARTNUM_RESERVE];
extern StreamBufferStruct g_senUartRxBuffer[UARTNUM_RESERVE][SENSOR_RESERVE];

extern TrigInInfoStruct   g_trigInInfo;



/*****************************************�ֲ��궨��******************************************/
#define    UART_SENSOR_FRAME_DATA_LEN_MAX    3    //ÿ֡����͵����ݳ���



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pReportCmdFunc[REPORTCMD_RESERVE];
SubCmdProFunc pSensorUartCmdFunc[SNUARTCMD_RESERVE];

#ifdef PROJECT_QUBELEY
SubCmdProFunc pOtpCmdFunc[OTPCMD_RESERVE];
SubCmdProFunc pAnalogInCmdFunc[ANALOGINCMD_RESERVE];

SoftTimerStruct  g_otpTimer;
#endif

SoftTimerStruct  g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];
SoftTimerStruct  g_driverMonitorTimer[CH_TOTAL];



/******************************************����ʵ��*******************************************/
#ifdef PROJECT_QUBELEY
/*********************************************************************************************
�� �� ��: cmdOtpTempTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdOtpTempTimerCB(void *timeOutPara)
{
    u8 dataLen;
    u8 *pData;


    dataLen = sizeof(g_sensorData.temperature);
    pData = (u8 *)&g_sensorData.temperature;
    cmdFrameSend(CMD_OTP, OTPCMD_DATAQ, dataLen, pData);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;

    g_systemCfgBmp.bQryTemp = true;

    //�ص������������������߳��е��ã������������Ը�Func�����ź���
    g_bPerdPostSemToFunc = true;

    if (g_otpTimer.bUsed)
    {
        servStimerAdd(&g_otpTimer);
    }
}


/*********************************************************************************************
�� �� ��: cmdOtpStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdOtpStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfOtpStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        g_systemInfo.otpInfo.state = state;

        //�浽EEPROM��
        servSystemInfoWrite(&g_systemInfo);

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
�� �� ��: cmdOtpStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdOtpThresholdSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdOtpThresholdSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16 threshold;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfOtpThresholdVerify(cmdDataLen, pCmdData, (void *)&threshold))
    {
        g_systemInfo.otpInfo.threshold = threshold;

        //�浽EEPROM��
        servSystemInfoWrite(&g_systemInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdOtpThresholdQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdOtpResponseSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdOtpResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfOtpResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_systemInfo.otpInfo.response = response;

        //�浽EEPROM��
        servSystemInfoWrite(&g_systemInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdOtpResponseQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdOtpPeriodSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdOtpPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32  period;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfOtpPeriodVerify(cmdDataLen, pCmdData, (void *)&period))
    {
        g_systemInfo.otpInfo.period = period;

        servSystemInfoWrite(&g_systemInfo);

        servStimerInit(&g_otpTimer, period, cmdOtpTempTimerCB);
    }
}


/*********************************************************************************************
�� �� ��: cmdOtpPeriodQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdOtpDataQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdOtpCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdOtpCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdOtpCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < OTPCMD_RESERVE) && (pOtpCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pOtpCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


/*********************************************************************************************
�� �� ��: cmdAnalogInStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdAnalogInStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        g_analogInfo.state = state;

        //�浽EEPROM��
        servAnalogInInfoWrite(&g_analogInfo);
        
        //��FUNC���ź���
        g_systemCfgBmp.bAnalogIn = true;
        g_bCmdPostSemToFunc = true;
    }
}


/*********************************************************************************************
�� �� ��: cmdAnalogInStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdAnalogInThresholdHSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdAnalogInThresholdHSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 threshold;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInThresholdVerify(cmdDataLen, pCmdData, (void *)&threshold))
    {
        g_analogInfo.thresholdH = threshold;

        //������ADC��ֵ    NICK MARK
        //g_analogInfo.adcCodeH = ;

        //�浽EEPROM��
        servAnalogInInfoWrite(&g_analogInfo);
        
        if (SENSOR_ON == g_analogInfo.state)
        {
            //��FUNC���ź���
            g_systemCfgBmp.bAnalogIn = true;
            g_bCmdPostSemToFunc = true;
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdAnalogInThresholdHQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdAnalogInThresholdLSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdAnalogInThresholdLSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 threshold;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInThresholdVerify(cmdDataLen, pCmdData, (void *)&threshold))
    {
        g_analogInfo.thresholdL = threshold;

        //������ADC��ֵ    NICK MARK
        //g_analogInfo.adcCodeL = ;

        //�浽EEPROM��
        servAnalogInInfoWrite(&g_analogInfo);
        
        if (SENSOR_ON == g_analogInfo.state)
        {
            //��FUNC���ź���
            g_systemCfgBmp.bAnalogIn = true;
            g_bCmdPostSemToFunc = true;
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdAnalogInThresholdLQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdAnalogInResponseHSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdAnalogInResponseHSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_analogInfo.responseH = response;

        //�浽EEPROM��
        servAnalogInInfoWrite(&g_analogInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdAnalogInResponseHQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdAnalogInResponseLSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdAnalogInResponseLSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfAnalogInResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_analogInfo.responseL = response;

        //�浽EEPROM��
        servAnalogInInfoWrite(&g_analogInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdAnalogInResponseLQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdAnalogInCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdAnalogInCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdAnalogInCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < ANALOGINCMD_RESERVE) && (pAnalogInCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pAnalogInCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}
#endif


/*********************************************************************************************
�� �� ��: cmdReportTorqueTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //�����ź�����FUNC
    /*g_chanCfgBmp[chanNum].bReportTorque = true;
    g_bPerdPostSemToFunc = true;*/
    
    //����������ʱ����ֱ���յ��ر�����
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
�� �� ��: cmdReportCycleTimerCB;
ʵ�ֹ���: ѭ��ģʽ��,����״̬���ϴ���ǰ��ѭ������
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

        //�ص������������������߳��е��ã������������Ը���������ź���
        g_bPerdPostSemToCmd = true;
    }

    g_chanCfgBmp[chanNum].bCycleQry = true;
    g_bPerdPostSemToFunc = true; 
    
    //����������ʱ��
    if (g_reportTimer[chanNum][REPTTYPE_CYCLE].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_CYCLE]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportXAngleTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportXAngleTimerCB(void *timeOutPara)
{   
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_XANGLE;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_XANGLE]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_XANGLE];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_XANGLE]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_XANGLE].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_XANGLE]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportYAngleTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportYAngleTimerCB(void *timeOutPara)
{    
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_YANGLE;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_YANGLE]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_YANGLE];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_YANGLE]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_YANGLE].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_YANGLE]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportZAngleTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportZAngleTimerCB(void *timeOutPara)
{    
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_ZANGLE;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_ZANGLE]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_ZANGLE];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_ZANGLE]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_ZANGLE].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_ZANGLE]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportXVelocityTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportXVelocityTimerCB(void *timeOutPara)
{    
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_XVELOC;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_XVELOC]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_XVELOC];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_XVELOC]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_XVELOC].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_XVELOC]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportYVelocityTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportYVelocityTimerCB(void *timeOutPara)
{           
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_YVELOC;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_YVELOC]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_YVELOC];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_YVELOC]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_YVELOC].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_YVELOC]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportZVelocityTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportZVelocityTimerCB(void *timeOutPara)
{           
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_ZVELOC;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_ZVELOC]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_ZVELOC];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_ZVELOC]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_ZVELOC].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_ZVELOC]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportXAccTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportXAccTimerCB(void *timeOutPara)
{    
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_XACC;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_XACC]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_XACC];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_XACC]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_XACC].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_XACC]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportYAccTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportYAccTimerCB(void *timeOutPara)
{    
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_YACC;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_YACC]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_YACC];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_YACC]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_YACC].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_YACC]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportZAccTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportZAccTimerCB(void *timeOutPara)
{    
    u8 dataLen, i;
    u8 *pData;
    u8 data[6];
    u8 chanNum = *(u8 *)timeOutPara;
    u8 index = (u8)REPTTYPE_ZACC;

    
    dataLen = sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_ZACC]) + sizeof(index) + sizeof(chanNum);
    pData = (u8 *)&g_sensorData.reporterData[chanNum][REPTTYPE_ZACC];
    data[0] = chanNum;
    data[1] = index;
    for (i = 0;i < sizeof(g_sensorData.reporterData[chanNum][REPTTYPE_ZACC]);i++)
    {
        data[2 + i] = *pData++;
    }
    cmdFrameSend(CMD_REPORT, REPORTCMD_DATAQ, dataLen, data);

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_ZACC].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_ZACC]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportAngleSensorTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportAngleSensorTimerCB(void *timeOutPara)
{
    u8 chanNum = *(u8 *)timeOutPara;


    if (chanNum < ANGLE_SENSOR_NUM)
    {
        g_eventSrcBmp.bAngleSen[chanNum] = true;

        g_bPerdPostSemToEvent = true;

        if (g_reportTimer[chanNum][REPTTYPE_ANGLESEN].bUsed)
        {
            servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_ANGLESEN]);
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdReportSgAllTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_DRVSGALL].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_DRVSGALL]);
    }   
}


/*********************************************************************************************
�� �� ��: cmdReportSgSeTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_DRVSGSE].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_DRVSGSE]);
    }   
}


/*********************************************************************************************
�� �� ��: cmdReportDistanceTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

        g_eventSrcBmp.bDistSen[chanNum] = true;

        g_bPerdPostSemToEvent = true;

        //�ص������������������߳��е��ã������������Ը���������ź���
        g_bPerdPostSemToCmd = true;
        
        //����������ʱ����ֱ���յ��ر�����
        if (g_reportTimer[chanNum][REPTTYPE_DISTANCE].bUsed)
        {
            servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_DISTANCE]);
        }  
    }
}


/*********************************************************************************************
�� �� ��: cmdReportDriveMonitorTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
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
�� �� ��: cmdReportAbsEncoderTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportAbsEncoderTimerCB(void *timeOutPara)
{
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

        //�ص������������������߳��е��ã������������Ը���������ź���
        g_bPerdPostSemToCmd = true;
        
        //����������ʱ����ֱ���յ��ر�����
        if (g_reportTimer[chanNum][REPTTYPE_ABSENC].bUsed)
        {
            servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_ABSENC]);
        }  
    }
}


/*********************************************************************************************
�� �� ��: cmdReportOutNumTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_OUTNUM].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_OUTNUM]);
    }   
}


/*********************************************************************************************
�� �� ��: cmdReportStepsTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_STEPS].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_STEPS]);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportVelorityTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    //�ص������������������߳��е��ã������������Ը���������ź���
    g_bPerdPostSemToCmd = true;
    
    //����������ʱ����ֱ���յ��ر�����
    if (g_reportTimer[chanNum][REPTTYPE_VELOCITY].bUsed)
    {
        servStimerAdd(&g_reportTimer[chanNum][REPTTYPE_VELOCITY]);
    }
}



/*********************************************************************************************
�� �� ��: cmdReportStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8   index;    //�ϱ������±��ΪU8    CJ 2017.06.07 Modify
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
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
    if (PARA_VERIFY_NO_ERROR == pvrfReportStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_reportInfo.report[i].state[index] = state;

                //�ı䶨ʱ��״̬
                if (SENSOR_ON == state)
                {
                    switch ((ReportTypeEnum)index)
                    {
                        case REPTTYPE_TORQUE:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_TORQUE]);

                            //�ܺĺ�SGALL�Լ�SGSE�������ü��
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

                        case REPTTYPE_XANGLE:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_XANGLE]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_YANGLE:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_YANGLE]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_ZANGLE:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_ZANGLE]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_XVELOC:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_XVELOC]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_YVELOC:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_YVELOC]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_ZVELOC:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_ZVELOC]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_XACC:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_XACC]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_YACC:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_YACC]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_ZACC:
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_ZACC]);

                            if (!g_reportTimer[i][REPTTYPE_ANGLESEN].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_DRVSGALL:  
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_DRVSGALL]);

                            //�ܺĺ�SGALL�Լ�SGSE�������ü��
                            if (!g_reportTimer[i][REPTTYPE_DRVMONT].bUsed)
                            {
                                servStimerAdd(&g_reportTimer[i][REPTTYPE_DRVMONT]);
                            }
                          break;

                        case REPTTYPE_DRVSGSE:  
                            servStimerAdd(&g_reportTimer[i][REPTTYPE_DRVSGSE]);

                            //�ܺĺ�SGALL�Լ�SGSE�������ü��
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

                            //�ܺĺ�SGALL�Լ�SGSE�������ü��
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

                        case REPTTYPE_XANGLE:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_XANGLE]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_YANGLE:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_YANGLE]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_ZANGLE:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_ZANGLE]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_XVELOC:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_XVELOC]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_YVELOC:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_YVELOC]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_ZVELOC:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_ZVELOC]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_XACC:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_XACC]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_YACC:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_YACC]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;

                        case REPTTYPE_ZACC:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_ZACC]);

                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZANGLE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZVELOC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_XACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_YACC]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_ZACC]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_ANGLESEN]);
                            }
                          break;
                          
                        case REPTTYPE_DRVSGALL:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_DRVSGALL]);

                            //�ܺĺ�SGALL�Լ�SGSE�������ü��
                            if ((SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_TORQUE]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_DRVSGALL]) &&
                                (SENSOR_OFF == g_reportInfo.report[i].state[REPTTYPE_DRVSGSE]))
                            {
                                servStimerDelete(&g_reportTimer[i][REPTTYPE_DRVMONT]);
                            }
                          break;
                          
                        case REPTTYPE_DRVSGSE:
                            servStimerDelete(&g_reportTimer[i][REPTTYPE_DRVSGSE]);

                            //�ܺĺ�SGALL�Լ�SGSE�������ü��
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

        servReportInfoWrite(&g_reportInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
            for (j = 0;j < g_systemState.chanNum;j++)
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
�� �� ��: cmdReportPeriodSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdReportPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32  reptPeriod;
    u32  periodMin;
    u8   index;    //�ϱ������±��ΪU8    CJ 2017.06.07 Modify
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
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
    if (PARA_VERIFY_NO_ERROR == pvrfReportPeriodVerify(cmdDataLen, pCmdData, (void *)&reptPeriod, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_reportInfo.report[i].period[index] = reptPeriod;

                //������������
                switch ((ReportTypeEnum)index)
                {
                    case REPTTYPE_TORQUE:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_TORQUE], reptPeriod, cmdReportTorqueTimerCB);
                        
                        //����С�������ü��
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

                    case REPTTYPE_XANGLE:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_XANGLE], reptPeriod, cmdReportXAngleTimerCB);
                      break;

                    case REPTTYPE_YANGLE:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_YANGLE], reptPeriod, cmdReportYAngleTimerCB); 
                      break;

                    case REPTTYPE_ZANGLE:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_ZANGLE], reptPeriod, cmdReportZAngleTimerCB); 
                      break;

                    case REPTTYPE_XVELOC:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_XVELOC], reptPeriod, cmdReportXVelocityTimerCB);
                      break;

                    case REPTTYPE_YVELOC:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_YVELOC], reptPeriod, cmdReportYVelocityTimerCB); 
                      break;

                    case REPTTYPE_ZVELOC:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_ZVELOC], reptPeriod, cmdReportZVelocityTimerCB); 
                      break;

                    case REPTTYPE_XACC:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_XACC], reptPeriod, cmdReportXAccTimerCB); 
                      break;

                    case REPTTYPE_YACC:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_YACC], reptPeriod, cmdReportYAccTimerCB); 
                      break;

                    case REPTTYPE_ZACC:
                        servStimerInit(&g_reportTimer[i][REPTTYPE_ZACC], reptPeriod, cmdReportZAccTimerCB); 
                      break;

                    case REPTTYPE_DRVSGALL:  
                        servStimerInit(&g_reportTimer[i][REPTTYPE_DRVSGALL], reptPeriod, cmdReportSgAllTimerCB);
                        
                        //����С�������ü��
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
                        
                        //����С�������ü��
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

        servReportInfoWrite(&g_reportInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdReportPeriodQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
            for (j = 0;j< g_systemState.chanNum;j++)
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
�� �� ��: cmdReportDataQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

            switch ((ReportTypeEnum)index)
            {
                case REPTTYPE_TORQUE:
                case REPTTYPE_DRVSGALL:  
                case REPTTYPE_DRVSGSE:
                    g_chanCfgBmp[chanNum].bDriverQuery = true;
                  break;

                case REPTTYPE_CYCLE:
                    //
                  break;

                case REPTTYPE_XANGLE:
                case REPTTYPE_YANGLE:
                case REPTTYPE_ZANGLE:
                case REPTTYPE_XVELOC:
                case REPTTYPE_YVELOC:
                case REPTTYPE_ZVELOC:
                case REPTTYPE_XACC:
                case REPTTYPE_YACC:
                case REPTTYPE_ZACC:
                    if (chanNum < ANGLE_SENSOR_NUM)
                    {
                        g_eventSrcBmp.bAngleSen[chanNum] = true;
                    }
                  break;

                case REPTTYPE_DISTANCE:  
                    if (chanNum < DIST_SENSOR_NUM)
                    {
                        g_eventSrcBmp.bDistSen[chanNum] = true;
                    }
                  break;

                case REPTTYPE_ABSENC:  
#if GELGOOG_SINANJU
                    if (chanNum < ABS_ENCODER_NUM)
                    {
                        g_eventSrcBmp.bAbsEncoder[chanNum] = true;
                    }
#endif
                  break;
                  
                case REPTTYPE_OUTNUM:
                    //
                  break;

                case REPTTYPE_STEPS:
                    //
                  break;

                case REPTTYPE_VELOCITY:  
                    //
                  break;

                default:
                  break;
            }
        }
        else
        {
            for (j = 0;j < g_systemState.chanNum;j++)
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

                    switch ((ReportTypeEnum)index)
                    {
                        case REPTTYPE_TORQUE:
                        case REPTTYPE_DRVSGALL:  
                        case REPTTYPE_DRVSGSE:
                            g_chanCfgBmp[j].bDriverQuery = true;
                          break;

                        case REPTTYPE_CYCLE:
                            //
                          break;

                        case REPTTYPE_XANGLE:
                        case REPTTYPE_YANGLE:
                        case REPTTYPE_ZANGLE:
                        case REPTTYPE_XVELOC:
                        case REPTTYPE_YVELOC:
                        case REPTTYPE_ZVELOC:
                        case REPTTYPE_XACC:
                        case REPTTYPE_YACC:
                        case REPTTYPE_ZACC:
                            if (j < ANGLE_SENSOR_NUM)
                            {
                                g_eventSrcBmp.bAngleSen[j] = true;
                            }
                          break;

                        case REPTTYPE_DISTANCE:  
                            if (j < DIST_SENSOR_NUM)
                            {
                                g_eventSrcBmp.bDistSen[j] = true;
                            }
                          break;

                        case REPTTYPE_ABSENC:  
#if GELGOOG_SINANJU
                            if (chanNum < ABS_ENCODER_NUM)
                            {
                                g_eventSrcBmp.bAbsEncoder[chanNum] = true;
                            }
#endif
                          break;
                          
                        case REPTTYPE_OUTNUM:
                            //
                          break;

                        case REPTTYPE_STEPS:
                            //
                          break;

                        case REPTTYPE_VELOCITY:  
                            //
                          break;

                        default:
                          break;
                    }
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdReportCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdReportCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartBaudSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartBaudSet(u8 cmdDataLen, u8 *pCmdData)
{
    UartBaudEnum baud;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //���в�����֤
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartBaudVerify(cmdDataLen, pCmdData, (void *)&baud)))
    {
        g_sensorUartIntfc[index].baud = baud;
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartBaudQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartWordLenSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartWordLenSet(u8 cmdDataLen, u8 *pCmdData)
{
    WordLenEnum wordLen;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //���в�����֤
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartWordLenVerify(cmdDataLen, pCmdData, (void *)&wordLen)))
    {
        g_sensorUartIntfc[index].wordLen = wordLen;
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartWordLenQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartFlowCtlSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartFlowCtlSet(u8 cmdDataLen, u8 *pCmdData)
{
    FlowCtlEnum flowCtl;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //���в�����֤
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartFlowCtlVerify(cmdDataLen, pCmdData, (void *)&flowCtl)))
    {
        g_sensorUartIntfc[index].flowCtl = flowCtl;
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartFlowCtlQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartParitySet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartParitySet(u8 cmdDataLen, u8 *pCmdData)
{
    ParityEnum parity;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //���в�����֤
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartParityVerify(cmdDataLen, pCmdData, (void *)&parity)))
    {
        g_sensorUartIntfc[index].parity = parity;
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartParityQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartStopBitSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartStopBitSet(u8 cmdDataLen, u8 *pCmdData)
{
    StopBitEnum stopBit;
    u8 index = *pCmdData++;
    

    cmdDataLen--;    

    //���в�����֤
    if ((index < UARTNUM_RESERVE) &&
        (PARA_VERIFY_NO_ERROR == pvrfUartStopBitVerify(cmdDataLen, pCmdData, (void *)&stopBit)))
    {
        g_sensorUartIntfc[index].stopBit = stopBit;
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartStopBitQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartStopBitSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartApply(u8 cmdDataLen, u8 *pCmdData)
{
    u8 index = *pCmdData++;
    

    //���в�����֤
    if (index < UARTNUM_RESERVE)
    {
        memcpy(&g_sensorUart.uartIntfc[index], &g_sensorUartIntfc[index], sizeof(UartIntfcStruct));

        servSensorUartInfoStore(&g_sensorUart);

        //��FUNC���ź���
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
�� �� ��: cmdSensorUartStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartStateSet(u8 cmdDataLen, u8 *pCmdData)
{    
    SensorStateEnum state;
    u8 uartIndex;
    u8 sensorIndex;
#ifdef PROJECT_GELGOOG
    u8 dataLen;
    u8 data[6];
#endif
    

    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartStateVerify(cmdDataLen, pCmdData, (void *)&state, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].state = state;

        servSensorUartInfoStore(&g_sensorUart);

        if (SENSOR_ON == state)
        {
            if (UARTNUM_U1 == uartIndex)
            {
                servSensor1UartReciveOn(g_sensorUart.sensor[UARTNUM_U1][sensorIndex], (SensorNumEnum)sensorIndex);
            }
            else
            {
                servSensor2UartReciveOn(g_sensorUart.sensor[UARTNUM_U2][sensorIndex], (SensorNumEnum)sensorIndex);

#ifdef PROJECT_GELGOOG

                //U2_S1��CH4������L����
                if (SENSOR_S1 == sensorIndex)
                {
                    if (SENSOR_ON == g_trigInInfo.trigIn[CH4].levelState[TRIGPIN_DIL])
                    {
                        g_trigInInfo.trigIn[CH4].levelState[TRIGPIN_DIL] = SENSOR_OFF;
                        
                        //֪ͨ��λ��IO״̬�仯��
                        dataLen = sizeof(g_trigInInfo.trigIn[CH4].levelState[TRIGPIN_DIL]) + sizeof(CH4) + sizeof(TRIGPIN_DIL);
                        data[0] = CH4;            //ͨ����
                        data[1] = TRIGPIN_DIL;    //Trig���
                        data[2] = g_trigInInfo.trigIn[CH4].levelState[TRIGPIN_DIL];
                        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELSTATEQ, dataLen, data);
                
                        //g_chanCfgBmp[CH4].bTrigIn = true;
                    
                        servTrigInInfoWrite(&g_trigInInfo);
                    }
                }
                else if (SENSOR_S2 == sensorIndex)
                {
                    //U2_S2��CH4������R��CH4����ͨ��������Zͨ������
                    if (SENSOR_ON == g_trigInInfo.trigIn[CH4].levelState[TRIGPIN_DIR])
                    {
                        g_trigInInfo.trigIn[CH4].levelState[TRIGPIN_DIR] = SENSOR_OFF;
                        
                        //֪ͨ��λ��IO״̬�仯��
                        dataLen = sizeof(g_trigInInfo.trigIn[CH4].levelState[TRIGPIN_DIR]) + sizeof(CH4) + sizeof(TRIGPIN_DIR);
                        data[0] = CH4;            //ͨ����
                        data[1] = TRIGPIN_DIR;    //Trig���
                        data[2] = g_trigInInfo.trigIn[CH4].levelState[TRIGPIN_DIR];
                        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELSTATEQ, dataLen, data);
                
                        //g_chanCfgBmp[CH4].bTrigIn = true;
                    
                        servTrigInInfoWrite(&g_trigInInfo);
                    }
                    //Ŀǰֻ��TrigR����    NICK MARK
                    /*else if ((INTFC_ON  == g_motorInfo.motor[CH4].encoderState) &&
                             (ECCHAN_3  == g_motorInfo.motor[CH4].encoderChanNum))
                    {
                        g_motorInfo.motor[CH4].encoderState = INTFC_OFF;
                        
                        //֪ͨ��λ��������״̬�仯��
                        dataLen = sizeof(g_motorInfo.motor[CH4].encoderState) + sizeof(CH4);
                        data[0] = CH4;
                        data[1] = g_motorInfo.motor[CH4].encoderState;
                        cmdFrameSend(CMD_ENCODER, ENCODERCMD_STATEQ, dataLen, data);

                        //g_chanCfgBmp[CH4].bEncoder = true;
                        
                        servMotorInfoWrite(&g_motorInfo);
                    }*/
                }
                else if (SENSOR_S3 == sensorIndex)    //U2_S3��CH4�ı���������
                {
                    if (INTFC_ON  == g_motorInfo.motor[CH4].encoderState)
                    {
                        g_motorInfo.motor[CH4].encoderState = INTFC_OFF;
                        
                        //֪ͨ��λ��������״̬�仯��
                        dataLen = sizeof(g_motorInfo.motor[CH4].encoderState) + sizeof(CH4);
                        data[0] = CH4;
                        data[1] = g_motorInfo.motor[CH4].encoderState;
                        cmdFrameSend(CMD_ENCODER, ENCODERCMD_STATEQ, dataLen, data);

                        //g_chanCfgBmp[CH4].bEncoder = true;
                        
                        servMotorInfoWrite(&g_motorInfo);
                    }
                }
                else    //U2_S4��CH4����ͨ������������
                {
                    if ((INTFC_ON  == g_motorInfo.motor[CH4].encoderState) &&
                        (ECCHAN_3  == g_motorInfo.motor[CH4].encoderChanNum))
                    {
                        g_motorInfo.motor[CH4].encoderState = INTFC_OFF;
                        
                        //֪ͨ��λ��������״̬�仯��
                        dataLen = sizeof(g_motorInfo.motor[CH4].encoderState) + sizeof(CH4);
                        data[0] = CH4;
                        data[1] = g_motorInfo.motor[CH4].encoderState;
                        cmdFrameSend(CMD_ENCODER, ENCODERCMD_STATEQ, dataLen, data);

                        //g_chanCfgBmp[CH4].bEncoder = true;
                        
                        servMotorInfoWrite(&g_motorInfo);
                    }
                }
                
                //��FUNC���ź���
                g_bCmdPostSemToFunc = true;
                
#endif
            }
        }
        else
        {
            if (UARTNUM_U1 == uartIndex)
            {
#ifdef PROJECT_GELGOOG

                //ȫ�ر��˾Ͱѽ���Ҳ�ر���
                if ((SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U1][SENSOR_S1].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U1][SENSOR_S2].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U1][SENSOR_S3].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U1][SENSOR_S4].state))
#endif
                {
                    servSensor1UartReciveOff();
                }
            }
            else
            {
#ifdef PROJECT_GELGOOG

                //ȫ�ر��˾Ͱѽ���Ҳ�ر���
                if ((SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S1].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S3].state) &&
                    (SENSOR_OFF == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S4].state))
#endif
                {
                    servSensor2UartReciveOff();
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartSofSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartSofSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 sof;
    u8 uartIndex;
    u8 sensorIndex;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartSofVerify(cmdDataLen, pCmdData, (void *)&sof, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].SOF = sof;

        servSensorUartInfoStore(&g_sensorUart);
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartSofQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartFrameLenSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartFrameLenSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 frameLen;
    u8 uartIndex;
    u8 sensorIndex;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartFrameLenVerify(cmdDataLen, pCmdData, (void *)&frameLen, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].frameLen = frameLen;

        servSensorUartInfoStore(&g_sensorUart);
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartFrameLenQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartRecvNumSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartRecvNumSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 recvNum;
    u8 uartIndex;
    u8 sensorIndex;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartRecvNumVerify(cmdDataLen, pCmdData, (void *)&recvNum, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].recvNum = recvNum;

        servSensorUartInfoStore(&g_sensorUart);
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartRecvNumQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartSwTimeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdSensorUartSwTimeSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 swTime;
    u8  uartIndex;
    u8  sensorIndex;
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfSensorUartSwTimeVerify(cmdDataLen, pCmdData, (void *)&swTime, &sensorIndex, &uartIndex))
    {
        g_sensorUart.sensor[uartIndex][sensorIndex].swTime = swTime;

        servSensorUartInfoStore(&g_sensorUart);
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartSwTimeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartDataQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    if ((sensorIndex < SENSOR_RESERVE) && (uartIndex < UARTNUM_RESERVE))
    {
        //��������֡�Ŵ���
        if (g_senUartRxBuffer[uartIndex][sensorIndex].frameCount > 0)
        {
            pUartPhyFrame = (SensorUartPhyFrameStruct *)servGetHead(&g_senUartRxBuffer[uartIndex][sensorIndex]);

            if (pUartPhyFrame->frameLen > 0)
            {
                frameLen = pUartPhyFrame->frameLen - sizeof(pUartPhyFrame->frameLen);    //�������������pUartPhyFrame->frameLen����
                if ((frameLen % UART_SENSOR_FRAME_DATA_LEN_MAX) != 0)
                {
                    frameNum = frameLen / UART_SENSOR_FRAME_DATA_LEN_MAX + 1;    //�෢һ֡�����һ֡����UART_SENSOR_FRAME_DATA_LEN_MAX���ֽ�
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
                        dataLen = frameLen % UART_SENSOR_FRAME_DATA_LEN_MAX;    //����ʣ���
                    }
                    else
                    {
                        dataLen = UART_SENSOR_FRAME_DATA_LEN_MAX;            
                    }
                    memcpy(&data[2], pData, dataLen);
                    pData += dataLen;

                    data[0] = uartIndex;      //���ں�
                    data[1] = sensorIndex;    //��������

                    //��4bit������ӣ���������4bit����4bit�����ĸ
                    //�ܹ����Է���16 x 5Bytes = 80Bytes(640bits)�����ݣ�Ŀǰû�д����ݣ����Բ���frameNum��С���ж���
                    data[2] = FRACTION(i, frameNum);
                    dataLen += 3;    //��2��Ϊ������data[0]��data[1]��data[2]
                    
                    cmdFrameSend(CMD_SENSORUART, SNUARTCMD_DATAQ, dataLen, data);
                }

                servDequeue(&g_senUartRxBuffer[uartIndex][sensorIndex], pUartPhyFrame->frameLen); //������ɺ����
                g_senUartRxBuffer[uartIndex][sensorIndex].frameCount--;
            }
        }
        else
        {
            //����0
            data[0] = uartIndex;      //���ں�
            data[1] = sensorIndex;
            data[2] = 0;
            cmdFrameSend(CMD_SENSORUART, SNUARTCMD_DATAQ, 3, data);
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdSensorUartCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdSensorUartCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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



/*******************************************�ļ�β********************************************/
