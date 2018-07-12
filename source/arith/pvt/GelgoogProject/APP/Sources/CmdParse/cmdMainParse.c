/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdMainParse.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "cmdMainParse.h"
#include "cmdSystem.h"
#include "cmdInterface.h"
#include "cmdMotionPlan.h"
#include "cmdMotion.h"
#include "cmdCalibrate.h"
#include "cmdUpdate.h"
#include "cmdDebugMotor.h"
#include "cmdMotor.h"
#include "cmdSensor.h"
#include "cmdClockSync.h"
#include "cmdFactory.h"
#include "cmdNetandId.h"
#include "cmdTrigger.h"
#include "cmdDriver.h"
#include "comInterface.h"
#include "servCommIntfc.h"
#include "servStreamBuffer.h"



/****************************************�ⲿ��������*****************************************/
extern StreamBufferStruct g_ciCanRxBuffer;
extern StreamBufferStruct g_ciUartRxBuffer;
extern CommIntfcStruct    g_commIntfc;
extern SystemStateStruct  g_systemState;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
StreamBufferStruct g_CmdParseBuffer;    //�������������
u8                 CmdParseBuffer[480];
CmdParseFunc       pCmdParseFunc[CMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdFrameSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFrameSend(CmdTypeEnum cmdMainType, u8 cmdSubType, u8 dataLen, u8 *pData)
{
    servFrameSend(cmdMainType, cmdSubType, dataLen, pData, g_commIntfc.linkType);
}


/*********************************************************************************************
�� �� ��: cmdMainParseInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdMainParseInit()
{
    //ע���������ģ��ص�����
    memset(pCmdParseFunc, 0, sizeof(pCmdParseFunc));

    
    pCmdParseFunc[CMD_LINK]       = cmdLinkCmdProc;
    pCmdParseFunc[CMD_SYSTME]     = cmdSystemCmdProc;
    pCmdParseFunc[CMD_UART]       = cmdUartIntfcCmdProc;
    pCmdParseFunc[CMD_CAN]        = cmdCanIntfcCmdProc;
    pCmdParseFunc[CMD_CLOCK]      = cmdClcokCmdProc;
    
    pCmdParseFunc[CMD_MOTION]     = cmdMotionCmdProc;
    pCmdParseFunc[CMD_IDENTITY]   = cmdIdentityCmdProc;
    
    pCmdParseFunc[CMD_MOTOR]      = cmdMotorCmdProc;
    pCmdParseFunc[CMD_ENCODER]    = cmdEncoderCmdProc;
    
    pCmdParseFunc[CMD_PVT]      = cmdPvtCmdProc;
    pCmdParseFunc[CMD_PVTPOSN]  = cmdPvtPositionSetCmdProc;
    pCmdParseFunc[CMD_PVTPOSNQ] = cmdPvtPositionQueryCmdProc;
    pCmdParseFunc[CMD_PVTVELC]  = cmdPvtSpeedSetCmdProc;
    pCmdParseFunc[CMD_PVTVELCQ] = cmdPvtSpeedQueryCmdProc;
    pCmdParseFunc[CMD_PVTTIME]  = cmdPvtTimeSetCmdProc;
    pCmdParseFunc[CMD_PVTTIMEQ] = cmdPvtTimeQueryCmdProc;

    pCmdParseFunc[CMD_REPORT]     = cmdReportCmdProc;
    pCmdParseFunc[CMD_TRIGIN]     = cmdTrigInCmdProc;
    pCmdParseFunc[CMD_DRIVER]     = cmdDriverCmdProc;
    pCmdParseFunc[CMD_TRIGOUT]    = cmdTrigOutCmdProc;
    pCmdParseFunc[CMD_ISOLOUT]    = cmdIsolatorOutCmdProc;
    pCmdParseFunc[CMD_SENSORUART] = cmdSensorUartCmdProc;
    
#ifdef PROJECT_QUBELEY
    pCmdParseFunc[CMD_OTP]      = cmdOtpCmdProc;
    pCmdParseFunc[CMD_ANALOGIN] = cmdAnalogInCmdProc;
#endif

#ifdef PROJECT_GELGOOG
    pCmdParseFunc[CMD_ISOLIN] = cmdIsolInCmdProc;
#endif

    //CMD_ADVANCED

    pCmdParseFunc[CMD_UPDATE]     = cmdUpdateCmdProc;
    pCmdParseFunc[CMD_FACTORY]    = cmdFactoryCmdProc;
    /*pCmdParseFunc[CMD_CALIBRATE]  = cmdCalibrateCmdProc;
    pCmdParseFunc[CMD_DEBUG]      = cmdDebugCmdProc;*/


    //ע�����������ģ��Ļص�����
    cmdLinkCmdInit();
    cmdSystemCmdInit();
    cmdUartIntfcCmdInit();
    cmdCanIntfcCmdInit();
    cmdClcokCmdInit();
    
    cmdMotionCmdInit();
    cmdIdentityCmdInit();
    
    cmdMotorCmdInit();
    cmdEncoderCmdInit();
    
    cmdPvtCmdInit();
    //PVT��λ�á��ٶȺ�ʱ��������û��������
    
    cmdReportCmdInit();
    cmdTrigInCmdInit();
    cmdDriverCmdInit();
    cmdTrigOutCmdInit();
    cmdIsolatorOutCmdInit();
    cmdSensorUartCmdInit();

#ifdef PROJECT_QUBELEY
    cmdOtpCmdInit();
    cmdAnalogInCmdInit();
#endif

#ifdef PROJECT_GELGOOG
    cmdIsolInCmdInit();
#endif

    //CMD_ADVANCED

    cmdUpdateCmdInit();
    cmdFactoryCmdInit();
    /*cmdCalibrateCmdInit();
    cmdDebugCmdInit();*/

    //��ʼ���������Buffer
    servStreamBufferInit(&g_CmdParseBuffer, CmdParseBuffer, sizeof(CmdParseBuffer));  
}


/*********************************************************************************************
�� �� ��: cmdFrameProcess;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFrameProcess(void)
{
    CmdParseFrameStruct *pCmdParseFrame = NULL;


    //�����ݻ�����˾ͷ�
    if (g_CmdParseBuffer.bNotEmpty || g_CmdParseBuffer.bAlmostFull)
    {
        pCmdParseFrame = (CmdParseFrameStruct *)servGetHead(&g_CmdParseBuffer);
        
        if ((pCmdParseFrame->mainType < CMD_RESERVE) && 
            (pCmdParseFunc[pCmdParseFrame->mainType] != NULL) &&
            (SYSTATE_POWERON != g_systemState.systemState))    //�ϵ�����в���������
        {    
            pCmdParseFunc[pCmdParseFrame->mainType](pCmdParseFrame);
        }
        
        servDequeue(&g_CmdParseBuffer, pCmdParseFrame->GET_CMD_PARSE_FRMAE_TOTAL_SIZE);
    }
}



/*******************************************�ļ�β********************************************/
