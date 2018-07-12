/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdMotionPlan.c；
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
#include "cmdMotionPlan.h"
#include "pvrfMotionPlan.h"
#include "servSoftTimer.h"
#include "cmdMainParse.h"
#include "pvtAlgorithm.h"
#include "servSystemPara.h"



/****************************************外部变量声明*****************************************/
extern DriverInfoStruct  g_driverInfo;

extern PlanInfoStruct    g_planInfo;

extern MotorInfoStruct   g_motorInfo; 

extern SystemStateStruct g_systemState;
extern SystemInfoStruct  g_systemInfo;

extern bool g_bCmdPostSemToPvt;
extern bool g_bCmdPostSemToFunc;

extern ChanCfgBmpStruct g_chanCfgBmp[CH_TOTAL];
extern WaveTableStruct  g_waveTable[CH_TOTAL][WTTYPE_RESERVE];



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pPvtCmdFunc[PVTCMD_RESERVE];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdPvtConfigSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtConfigSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   errorCode = PARA_VERIFY_NO_ERROR; 
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;

    
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

    if ((sizeof(WaveTableTypeEnum) + sizeof(ConfigStateEnum)) == cmdDataLen)
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData++;
        
        if (waveTable < WTTYPE_RESERVE)
        {
            switch ((ConfigStateEnum)*pCmdData)
            {
                case CONFIG_END:
                    for (i = 0;i < g_systemState.chanNum;i++)
                    {
                        if (bConfig[i])
                        {
                            g_systemState.configState[i][waveTable] = CONFIG_END;

                            //手动添加结束点(time为-1.0作为结束点)
                            g_systemState.pvtManage[i][waveTable].pvtPoint[g_systemState.pvtManage[i][waveTable].pointIndex].time = -1.0;
                            g_systemState.pvtManage[i][waveTable].tailPoint++;

                            g_bCmdPostSemToPvt = true;
                        }
                    }
                  break;

                case CONFIG_CLEAR:
                    for (i = 0;i < g_systemState.chanNum;i++)
                    {
                        if (bConfig[i])
                        {
                            g_systemState.configState[i][waveTable] = CONFIG_CLEAR;

                            memset(g_systemState.pvtManage[i][waveTable].pvtPoint, 
                                   0, 
                                   sizeof(g_systemState.pvtManage[i][waveTable].pvtPoint));
                            g_systemState.pvtManage[i][waveTable].tailPoint  = 0;
                            g_systemState.pvtManage[i][waveTable].pointIndex = 0;
                            g_systemState.pvtManage[i][waveTable].bRewind = false;
                            g_systemState.pvtManage[i][waveTable].bClear  = true;
                        }
                    }
                  break;

                default:
                    errorCode = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            errorCode = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        errorCode = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = errorCode;
}


/*********************************************************************************************
函 数 名: cmdPvtConfigQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtConfigQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_systemState.configState[CH1][WTTYPE_MAIN]) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_systemState.configState[chanNum][waveTable];
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_systemState.configState[chanNum][waveTable]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_CONFIGQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_systemState.configState[i][waveTable];
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_systemState.configState[i][waveTable]);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_CONFIGQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtPresetStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtPresetStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    //
}


/*********************************************************************************************
函 数 名: cmdPvtPresetStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtPresetStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    //
}


/*********************************************************************************************
函 数 名: cmdPvtExecModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtExecModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    ExecModeEnum pvtExecMode;
    WaveTableTypeEnum waveTable;
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

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtExecModeVerify(cmdDataLen, pCmdData, (void *)&pvtExecMode, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].execMode = pvtExecMode;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtExecModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtExecModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].execMode) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].execMode;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].execMode);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_EXECMODEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].execMode;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].execMode);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_EXECMODEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtPlanModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtPlanModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    PlanModeEnum pvtPlanMode;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtPlanModeVerify(cmdDataLen, pCmdData, (void *)&pvtPlanMode, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].planMode = pvtPlanMode;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtPlanModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtPlanModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].planMode) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].planMode;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].planMode);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_PLANMODEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].planMode;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].planMode);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_PLANMODEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtMotionModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtMotionModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    MotionModeEnum motionMode;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtMotionModeVerify(cmdDataLen, pCmdData, (void *)&motionMode, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].motionMode = motionMode;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtMotionModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtMotionModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].motionMode) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].motionMode;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].motionMode);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_MOTIONMODEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].motionMode;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].motionMode);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_MOTIONMODEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtModifyDutySet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtModifyDutySet(u8 cmdDataLen, u8 *pCmdData)
{
    ModifyDutyEnum modifyDuty;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtModifyDutyVerify(cmdDataLen, pCmdData, (void *)&modifyDuty, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].modifyDuty = modifyDuty;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtModifyDutyQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtModifyDutyQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].modifyDuty) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].modifyDuty;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].modifyDuty);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_MODIFYDUTYQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].modifyDuty;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].modifyDuty);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_MODIFYDUTYQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtRemainPointQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtRemainPointQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    u16 remainPoint;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(remainPoint) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            remainPoint = PVT_POINT_BUFFER_SIZE - g_systemState.pvtManage[chanNum][waveTable].tailPoint;
            pData = (u8 *)&remainPoint;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(remainPoint);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_REMAINPOINTQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    remainPoint = PVT_POINT_BUFFER_SIZE - g_systemState.pvtManage[i][waveTable].tailPoint;
                    pData = (u8 *)&remainPoint;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(remainPoint);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_REMAINPOINTQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtNcyclesSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtNcyclesSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 cycleNum;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtNcyclesVerify(cmdDataLen, pCmdData, (void *)&cycleNum, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].cycleNum = cycleNum;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtNcyclesQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtNcyclesQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].cycleNum) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].cycleNum;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].cycleNum);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_NCYCLESQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].cycleNum;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].cycleNum);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_NCYCLESQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtWarnPointSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtWarnPointSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16 warnPoint;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtWarnPointVerify(cmdDataLen, pCmdData, (void *)&warnPoint, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].warnPoint = warnPoint;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtWarnPointQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtWarnPointQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].warnPoint) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].warnPoint;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].warnPoint);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_WARNPOINTQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].warnPoint;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].warnPoint);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_WARNPOINTQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtEndStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtEndStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    EndStateEnum endState;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtEndStateVerify(cmdDataLen, pCmdData, (void *)&endState, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].endState = endState;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtEndStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtEndStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].endState) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].endState;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].endState);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_ENDSTATEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].endState;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].endState);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_ENDSTATEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtBufferSizeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtBufferSizeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_waveTable[CH1][WTTYPE_MAIN].totalSize) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_waveTable[chanNum][waveTable].totalSize;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_waveTable[chanNum][waveTable].totalSize);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_BUFFERSIZEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_waveTable[i][waveTable].totalSize;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_waveTable[i][waveTable].totalSize);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_BUFFERSIZEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtAccScaleSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtAccScaleSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16  accScale;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtSpeedScaleVerify(cmdDataLen, pCmdData, (void *)&accScale, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].accScale = accScale;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtAccScaleQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtAccScaleQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].accScale) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].accScale;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].accScale);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_ACCSCALEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].accScale;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].accScale);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_ACCSCALEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtDecScaleSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtDecScaleSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16  decScale;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtSpeedScaleVerify(cmdDataLen, pCmdData, (void *)&decScale, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].decScale = decScale;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtDecScaleQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtDecScaleQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].decScale) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].decScale;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].decScale);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_DECSCALEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].decScale;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].decScale);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_DECSCALEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtStopDecelModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtStopDecelModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    StopModeEnum stopMode;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtStopDecelModeVerify(cmdDataLen, pCmdData, (void *)&stopMode, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].stopDecelInfo.stopMode = stopMode;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtStopDecelModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtStopDecelModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].stopDecelInfo.stopMode) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].stopDecelInfo.stopMode;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].stopDecelInfo.stopMode);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_STOPMODEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].stopDecelInfo.stopMode;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].stopDecelInfo.stopMode);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_STOPMODEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtStopDecelDistanceSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtStopDecelDistanceSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 distance;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtStopDecelDistanceVerify(cmdDataLen, pCmdData, (void *)&distance, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].stopDecelInfo.distance = distance;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtStopDecelDistanceQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtStopDecelDistanceQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].stopDecelInfo.distance) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].stopDecelInfo.distance;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].stopDecelInfo.distance);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_STOPDISTQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].stopDecelInfo.distance;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].stopDecelInfo.distance);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_STOPDISTQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtStopDecelTimeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtStopDecelTimeSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 time;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtStopDecelTimeVerify(cmdDataLen, pCmdData, (void *)&time, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].stopDecelInfo.time = time;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtStopDecelTimeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtStopDecelTimeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].stopDecelInfo.time) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].stopDecelInfo.time;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].stopDecelInfo.time);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_STOPTIMEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].stopDecelInfo.time;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].stopDecelInfo.time);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_STOPTIMEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtLineOutOfStepStateSet;
实现功能: ; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtLineOutOfStepStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum   lineState;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtOOSStateVerify(cmdDataLen, pCmdData, (void *)&lineState, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].outOfStepInfo.lineState = lineState;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtLineOutOfStepStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtLineOutOfStepStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].outOfStepInfo.lineState) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].outOfStepInfo.lineState;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].outOfStepInfo.lineState);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_LINESTATEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].outOfStepInfo.lineState;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].outOfStepInfo.lineState);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_LINESTATEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtLineOutOfStepOutNumSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtLineOutOfStepOutNumSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16  lineOutNum;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtOOSLineOutNumVerify(cmdDataLen, pCmdData, (void *)&lineOutNum, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].outOfStepInfo.lineOutNum = lineOutNum;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtLineOutOfStepOutNumQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtLineOutOfStepOutNumQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].outOfStepInfo.lineOutNum) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].outOfStepInfo.lineOutNum;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].outOfStepInfo.lineOutNum);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_LINEOUTNUMQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].outOfStepInfo.lineOutNum;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].outOfStepInfo.lineOutNum);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_LINEOUTNUMQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtLineOutOfStepResponseSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtLineOutOfStepResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum  lineResp;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtOOSResponseVerify(cmdDataLen, pCmdData, (void *)&lineResp, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].outOfStepInfo.lineResp = lineResp;
            }
        }
    }
}



/*********************************************************************************************
函 数 名: cmdPvtLineOutOfStepResponseQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtLineOutOfStepResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].outOfStepInfo.lineResp) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].outOfStepInfo.lineResp;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].outOfStepInfo.lineResp);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_LINERESPONSEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].outOfStepInfo.lineResp;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].outOfStepInfo.lineResp);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_LINERESPONSEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtTotalOutOfStepStateSet;
实现功能: ; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtTotalOutOfStepStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum   totalState;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtOOSStateVerify(cmdDataLen, pCmdData, (void *)&totalState, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].outOfStepInfo.totalState = totalState;
            }
        }
    }
}



/*********************************************************************************************
函 数 名: cmdPvtTotalOutOfStepStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtTotalOutOfStepStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].outOfStepInfo.totalState) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].outOfStepInfo.totalState;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].outOfStepInfo.totalState);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_TOTALSTATEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].outOfStepInfo.totalState;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].outOfStepInfo.totalState);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_TOTALSTATEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtTotalOutOfStepOutNumSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtTotalOutOfStepOutNumSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32  totalOutNum;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtOOSLineOutNumVerify(cmdDataLen, pCmdData, (void *)&totalOutNum, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].outOfStepInfo.totalOutNum = totalOutNum;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtTotalOutOfStepOutNumQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtTotalOutOfStepOutNumQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].outOfStepInfo.totalOutNum) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].outOfStepInfo.totalOutNum;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].outOfStepInfo.totalOutNum);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_TOTALOUTNUMQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].outOfStepInfo.totalOutNum;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].outOfStepInfo.totalOutNum);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_TOTALOUTNUMQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtTotalOutOfStepResponseSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtTotalOutOfStepResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum  totalResp;
    WaveTableTypeEnum waveTable;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtOOSResponseVerify(cmdDataLen, pCmdData, (void *)&totalResp, (void *)&waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_planInfo.plan[i][waveTable].outOfStepInfo.totalResp = totalResp;
            }
        }
    }
}



/*********************************************************************************************
函 数 名: cmdPvtTotalOutOfStepResponseQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtTotalOutOfStepResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        dataLen = sizeof(g_planInfo.plan[CH1][WTTYPE_MAIN].outOfStepInfo.totalResp) + sizeof(waveTable) + sizeof(chanNum);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_planInfo.plan[chanNum][waveTable].outOfStepInfo.totalResp;
            data[0] = chanNum;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_planInfo.plan[chanNum][waveTable].outOfStepInfo.totalResp);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, PVTCMD_TOTALRESPONSEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_planInfo.plan[i][waveTable].outOfStepInfo.totalResp;
                    data[0] = i;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_planInfo.plan[i][waveTable].outOfStepInfo.totalResp);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, PVTCMD_TOTALRESPONSEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtCmdInit(void)
{
    memset(pPvtCmdFunc, 0, sizeof(pPvtCmdFunc));

    pPvtCmdFunc[PVTCMD_CONFIG]       = cmdPvtConfigSet;
    pPvtCmdFunc[PVTCMD_CONFIGQ]      = cmdPvtConfigQuery;
    pPvtCmdFunc[PVTCMD_PRESETSTATE]  = cmdPvtPresetStateSet;
    pPvtCmdFunc[PVTCMD_PRESETSTATEQ] = cmdPvtPresetStateQuery;

    pPvtCmdFunc[PVTCMD_EXECMODE]     = cmdPvtExecModeSet;
    pPvtCmdFunc[PVTCMD_EXECMODEQ]    = cmdPvtExecModeQuery;
    pPvtCmdFunc[PVTCMD_PLANMODE]     = cmdPvtPlanModeSet;
    pPvtCmdFunc[PVTCMD_PLANMODEQ]    = cmdPvtPlanModeQuery;
    pPvtCmdFunc[PVTCMD_MOTIONMODE]   = cmdPvtMotionModeSet;
    pPvtCmdFunc[PVTCMD_MOTIONMODEQ]  = cmdPvtMotionModeQuery;
    pPvtCmdFunc[PVTCMD_MODIFYDUTY]   = cmdPvtModifyDutySet;
    pPvtCmdFunc[PVTCMD_MODIFYDUTYQ]  = cmdPvtModifyDutyQuery;
    pPvtCmdFunc[PVTCMD_REMAINPOINTQ] = cmdPvtRemainPointQuery;
    
    pPvtCmdFunc[PVTCMD_NCYCLES]      = cmdPvtNcyclesSet;
    pPvtCmdFunc[PVTCMD_NCYCLESQ]     = cmdPvtNcyclesQuery;
    pPvtCmdFunc[PVTCMD_WARNPOINT]    = cmdPvtWarnPointSet;
    pPvtCmdFunc[PVTCMD_WARNPOINTQ]   = cmdPvtWarnPointQuery;
    pPvtCmdFunc[PVTCMD_ENDSTATE]     = cmdPvtEndStateSet;
    pPvtCmdFunc[PVTCMD_ENDSTATEQ]    = cmdPvtEndStateQuery;
    pPvtCmdFunc[PVTCMD_BUFFERSIZEQ]  = cmdPvtBufferSizeQuery;

    pPvtCmdFunc[PVTCMD_ACCSCALE]  = cmdPvtAccScaleSet;
    pPvtCmdFunc[PVTCMD_ACCSCALEQ] = cmdPvtAccScaleQuery;
    pPvtCmdFunc[PVTCMD_DECSCALE]  = cmdPvtDecScaleSet;
    pPvtCmdFunc[PVTCMD_DECSCALEQ] = cmdPvtDecScaleQuery;

    pPvtCmdFunc[PVTCMD_STOPMODE]  = cmdPvtStopDecelModeSet;
    pPvtCmdFunc[PVTCMD_STOPMODEQ] = cmdPvtStopDecelModeQuery;
    pPvtCmdFunc[PVTCMD_STOPDIST]  = cmdPvtStopDecelDistanceSet;
    pPvtCmdFunc[PVTCMD_STOPDISTQ] = cmdPvtStopDecelDistanceQuery;
    pPvtCmdFunc[PVTCMD_STOPTIME]  = cmdPvtStopDecelTimeSet;
    pPvtCmdFunc[PVTCMD_STOPTIMEQ] = cmdPvtStopDecelTimeQuery;
    
    pPvtCmdFunc[PVTCMD_LINESTATE]      = cmdPvtLineOutOfStepStateSet;
    pPvtCmdFunc[PVTCMD_LINESTATEQ]     = cmdPvtLineOutOfStepStateQuery;
    pPvtCmdFunc[PVTCMD_LINEOUTNUM]     = cmdPvtLineOutOfStepOutNumSet;
    pPvtCmdFunc[PVTCMD_LINEOUTNUMQ]    = cmdPvtLineOutOfStepOutNumQuery;
    pPvtCmdFunc[PVTCMD_LINERESPONSE]   = cmdPvtLineOutOfStepResponseSet;
    pPvtCmdFunc[PVTCMD_LINERESPONSEQ]  = cmdPvtLineOutOfStepResponseQuery;
    pPvtCmdFunc[PVTCMD_TOTALSTATE]     = cmdPvtTotalOutOfStepStateSet;
    pPvtCmdFunc[PVTCMD_TOTALSTATEQ]    = cmdPvtTotalOutOfStepStateQuery;
    pPvtCmdFunc[PVTCMD_TOTALOUTNUM]    = cmdPvtTotalOutOfStepOutNumSet;
    pPvtCmdFunc[PVTCMD_TOTALOUTNUMQ]   = cmdPvtTotalOutOfStepOutNumQuery;
    pPvtCmdFunc[PVTCMD_TOTALRESPONSE]  = cmdPvtTotalOutOfStepResponseSet;
    pPvtCmdFunc[PVTCMD_TOTALRESPONSEQ] = cmdPvtTotalOutOfStepResponseQuery;    
}

            
/*********************************************************************************************
函 数 名: cmdPvtCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < PVTCMD_RESERVE) && (pPvtCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pPvtCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}

#if 0
#endif

/*********************************************************************************************
函 数 名: cmdPvtPositionSetCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtPositionSetCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{
    u8 chanNum    = pCmdStackFrame->subType;
    u8 cmdDataLen = pCmdStackFrame->dataLen;
    u8 *pCmdData  = pCmdStackFrame->payload;
    
    WaveTableTypeEnum waveTable;
    f32  position;
    u8   index;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};

    
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

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtPositionVerify(cmdDataLen, pCmdData, (void *)&position, &index, &waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_systemState.pvtManage[i][waveTable].pvtPoint[index].position = position;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtPositionQueryCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtPositionQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{
    u8 chanNum    = pCmdStackFrame->subType;
    u8 cmdDataLen = pCmdStackFrame->dataLen;
    u8 *pCmdData  = pCmdStackFrame->payload;
    
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 index   = *pCmdData;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(index) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        //长度不包含通道号，通道号放在子命令号里
        dataLen = sizeof(g_systemState.pvtManage[CH1][WTTYPE_MAIN].pvtPoint[0].position) + sizeof(index);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_systemState.pvtManage[chanNum][waveTable].pvtPoint[index].position;
            data[0] = index;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_systemState.pvtManage[chanNum][waveTable].pvtPoint[index].position);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, chanNum, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_systemState.pvtManage[i][waveTable].pvtPoint[index].position;
                    data[0] = index;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_systemState.pvtManage[i][waveTable].pvtPoint[index].position);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, i, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtSpeedSetCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtSpeedSetCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{
    u8 chanNum    = pCmdStackFrame->subType;
    u8 cmdDataLen = pCmdStackFrame->dataLen;
    u8 *pCmdData  = pCmdStackFrame->payload;
    
    WaveTableTypeEnum waveTable;
    f32  speed;
    u8   index;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};

    
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

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtSpeedVerify(cmdDataLen, pCmdData, (void *)&speed, &index, &waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_systemState.pvtManage[i][waveTable].pvtPoint[index].speed = speed;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtSpeedQueryCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtSpeedQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{
    u8 chanNum    = pCmdStackFrame->subType;
    u8 cmdDataLen = pCmdStackFrame->dataLen;
    u8 *pCmdData  = pCmdStackFrame->payload;
    
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 index   = *pCmdData;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(index) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        //长度不包含通道号，通道号放在子命令号里
        dataLen = sizeof(g_systemState.pvtManage[CH1][WTTYPE_MAIN].pvtPoint[0].speed) + sizeof(index);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_systemState.pvtManage[chanNum][waveTable].pvtPoint[index].speed;
            data[0] = index;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_systemState.pvtManage[chanNum][waveTable].pvtPoint[index].speed);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, chanNum, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_systemState.pvtManage[i][waveTable].pvtPoint[index].speed;
                    data[0] = index;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_systemState.pvtManage[i][waveTable].pvtPoint[index].speed);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, i, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdPvtTimeSetCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtTimeSetCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{
    u8 chanNum    = pCmdStackFrame->subType;
    u8 cmdDataLen = pCmdStackFrame->dataLen;
    u8 *pCmdData  = pCmdStackFrame->payload;
    
    WaveTableTypeEnum waveTable;
    f32  time;
    u8   index;
    u8   i, j;
    bool bAdd = true;
    bool bConfig[CH_TOTAL] = {0};

    
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

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfPvtTimeVerify(cmdDataLen, pCmdData, (void *)&time, &index, &waveTable))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {                
                g_systemState.pvtManage[i][waveTable].pvtPoint[index].time = time;

                if (index == g_systemState.pvtManage[i][waveTable].pointIndex)    //保证上位机按顺序发送，且重复发送的话不会导致再次给解算线程发送信号量
                {
                    g_systemState.pvtManage[i][waveTable].pointIndex++;
                    if (0 == g_systemState.pvtManage[i][waveTable].pointIndex)
                    {
                        g_systemState.pvtManage[i][waveTable].bRewind = true;
                    }
                    g_systemState.pvtManage[i][waveTable].tailPoint++;

                    //Buffer中有效点超过两个后才开始计算
                    if (g_systemState.pvtManage[i][waveTable].tailPoint >= PVT_CALC_NEED_POINT_NUM)
                    {
                        g_bCmdPostSemToPvt = true;
                        
                        //未进入计算中状态(才收到第二个点)
                        if ((PVT_CALC_NEED_POINT_NUM == g_systemState.pvtManage[i][waveTable].pointIndex) &&
                            (!g_systemState.pvtManage[i][waveTable].bRewind))
                        {
                            //是否已经在队列里了
                            bAdd = true;
                            for (j = g_systemState.calcIndex[i];j < g_systemState.tailIndex[i];j++)
                            {
                                if (waveTable == g_systemState.calcQueue[i][j])
                                {
                                    bAdd = false;
                                    break;
                                }
                            }

                            //加入到解算队列中
                            if (bAdd)
                            {
                                g_systemState.calcQueue[i][g_systemState.tailIndex[i]] = waveTable;
                                g_systemState.tailIndex[i]++;
                                if (g_systemState.tailIndex[i] >= PVT_CALC_QUEUE_SIZE)
                                {
                                    g_systemState.tailIndex[i] = 0;
                                }
                            }
                            
                            //重新计算下位置到微步和编码器线的转换系数
                            pvtPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                                 g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                                                 &g_systemState.posnConvertInfo[i]);
                        }
                    }
                }    //if (index == g_planInfo.pvtInfo[i].pointIndex)
            }    //if (bConfig[i])
        }    //for (i = 0;i < g_systemState.chanNum;i++)
    }
}


/*********************************************************************************************
函 数 名: cmdPvtTimeQueryCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdPvtTimeQueryCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{
    u8 chanNum    = pCmdStackFrame->subType;
    u8 cmdDataLen = pCmdStackFrame->dataLen;
    u8 *pCmdData  = pCmdStackFrame->payload;
    
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 index   = *pCmdData;
    WaveTableTypeEnum waveTable;


    if (((sizeof(WaveTableTypeEnum) + sizeof(index) + sizeof(chanNum)) == cmdDataLen) &&
        (*(WaveTableTypeEnum *)pCmdData  < WTTYPE_RESERVE))
    {
        waveTable = *(WaveTableTypeEnum *)pCmdData;
        
        //长度不包含通道号，通道号放在子命令号里
        dataLen = sizeof(g_systemState.pvtManage[CH1][WTTYPE_MAIN].pvtPoint[0].time) + sizeof(index);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_systemState.pvtManage[chanNum][waveTable].pvtPoint[index].time;
            data[0] = index;
            data[1] = waveTable;
            for (i = 0;i < sizeof(g_systemState.pvtManage[chanNum][waveTable].pvtPoint[index].time);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_PVT, chanNum, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_systemState.pvtManage[i][waveTable].pvtPoint[index].time;
                    data[0] = index;
                    data[1] = waveTable;
                    for (j = 0;j < sizeof(g_systemState.pvtManage[i][waveTable].pvtPoint[index].time);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_PVT, i, dataLen, data);
                }
            }
        }
    }    //if ((sizeof(chanNum) + sizeof(index)) == cmdDataLen)
}



/*******************************************文件尾********************************************/
