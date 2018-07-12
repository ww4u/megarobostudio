/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdMainParse.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
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



/****************************************外部变量声明*****************************************/
extern StreamBufferStruct g_ciCanRxBuffer;
extern StreamBufferStruct g_ciUartRxBuffer;
extern CommIntfcStruct    g_commIntfc;
extern SystemStateStruct  g_systemState;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
StreamBufferStruct g_CmdParseBuffer;    //命令解析缓冲区
u8                 CmdParseBuffer[480];
CmdParseFunc       pCmdParseFunc[CMD_RESERVE];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdFrameSend;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFrameSend(CmdTypeEnum cmdMainType, u8 cmdSubType, u8 dataLen, u8 *pData)
{
    servFrameSend(cmdMainType, cmdSubType, dataLen, pData, g_commIntfc.linkType);
}


/*********************************************************************************************
函 数 名: cmdMainParseInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMainParseInit()
{
    //注册命令解析模块回调函数
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


    //注册各个子命令模块的回调函数
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
    //PVT的位置、速度和时间主命令没有子命令
    
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

    //初始化命令解析Buffer
    servStreamBufferInit(&g_CmdParseBuffer, CmdParseBuffer, sizeof(CmdParseBuffer));  
}


/*********************************************************************************************
函 数 名: cmdFrameProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFrameProcess(void)
{
    CmdParseFrameStruct *pCmdParseFrame = NULL;


    //有数据或快满了就发
    if (g_CmdParseBuffer.bNotEmpty || g_CmdParseBuffer.bAlmostFull)
    {
        pCmdParseFrame = (CmdParseFrameStruct *)servGetHead(&g_CmdParseBuffer);
        
        if ((pCmdParseFrame->mainType < CMD_RESERVE) && 
            (pCmdParseFunc[pCmdParseFrame->mainType] != NULL) &&
            (SYSTATE_POWERON != g_systemState.systemState))    //上电过程中不处理命令
        {    
            pCmdParseFunc[pCmdParseFrame->mainType](pCmdParseFrame);
        }
        
        servDequeue(&g_CmdParseBuffer, pCmdParseFrame->GET_CMD_PARSE_FRMAE_TOTAL_SIZE);
    }
}



/*******************************************文件尾********************************************/
