/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdClockSync.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include "cmdClockSync.h"
#include "pvrfClockSync.h"
#include "cmdMainParse.h"
#include "servSystemPara.h"



/****************************************外部变量声明*****************************************/
extern SystemStateStruct  g_systemState;
extern SystemInfoStruct   g_systemInfo;
extern SystemCfgBmpStruct g_systemCfgBmp;

extern bool g_bCmdPostSemToFunc;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pClockCmdFunc[CLOCK_RESERVE];

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
u32 fpgaClockCount = 0;
#endif



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdClockFrequencySet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockFrequencySet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 fpgaPwmClock;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfClockFrequencyVerify(cmdDataLen, pCmdData, (void *)&fpgaPwmClock))
    {
#if PVT_CALC_USE_FPGA_CLOCK_ERROR
        fpgaClockCount = fpgaPwmClock;

        if (fpgaClockCount > 0)
        {
            //计算归一化后的误差
            g_systemInfo.fpgaPwmClock = ((f64)((s64)g_systemState.clockCount - fpgaClockCount)) * FPGA_PWM_CLOCK / fpgaClockCount;
        }
        
#else

        g_systemInfo.fpgaPwmClock = fpgaPwmClock;
#endif

        //存到EEPROM中
        servSystemInfoWrite(&g_systemInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdClockFrequencyQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockFrequencyQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;


#if PVT_CALC_USE_FPGA_CLOCK_ERROR
    dataLen = sizeof(fpgaClockCount);
    pData = (u8 *)&fpgaClockCount;
    
#else

    dataLen = sizeof(g_systemInfo.fpgaPwmClock);
    pData = (u8 *)&g_systemInfo.fpgaPwmClock;
#endif

    cmdFrameSend(CMD_CLOCK, CLOCK_FREQUENCYQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdClockSyncRegSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockSyncRegSet(u8 cmdDataLen, u8 *pCmdData)
{
    ClockSyncRegEnum clockSyncReg;
    

    if (PARA_VERIFY_NO_ERROR == pvrfClockSyncRegVerify(cmdDataLen, pCmdData, (void *)&clockSyncReg))
    {
        g_systemState.clockSyncReg = clockSyncReg;

        g_systemCfgBmp.bClockReg = true;
        g_bCmdPostSemToFunc = true;
    }
}


/*********************************************************************************************
函 数 名: cmdClockSyncRegQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockSyncRegQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemState.clockSyncReg);
    pData = (u8 *)&g_systemState.clockSyncReg;
    cmdFrameSend(CMD_CLOCK, CLOCK_SYNCREGQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdClockSyncStartTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockSyncStartTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    ReceiveTypeEnum receiveType;
    

    if (PARA_VERIFY_NO_ERROR == pvrfClockSyncStartTypeVerify(cmdDataLen, pCmdData, (void *)&receiveType))
    {
        g_systemState.clockSyncType = receiveType;
    }
}


/*********************************************************************************************
函 数 名: cmdClockSyncStartTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockSyncStartTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemState.clockSyncType);
    pData = (u8 *)&g_systemState.clockSyncType;
    cmdFrameSend(CMD_CLOCK, CLOCK_STARTTYPEQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdClockSyncStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockSyncStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    ClockSyncStateEnum clockSyncState;
    

    //上位机发送此命令前需要先配置MOTIONREGISTER寄存器
    //命令直接由FPGA进行响应，这里不需要处理只记录下状态
    //实际上FPGA在接收到这条命令时(命令内容为开始)已经开始进行时钟计数了
    //当命令内容为结束时，需要FUNC线程读取计数值，等待上位机读取
    if (PARA_VERIFY_NO_ERROR == pvrfClockSyncStateVerify(cmdDataLen, pCmdData, (void *)&clockSyncState))
    {
        if ((CLOCKSYNC_END == clockSyncState) && (CLOCKSYNC_START == g_systemState.clockSyncState))
        {
            //给FUNC发送信号量，获取计数值，获取完成后才算同步结束
            g_systemCfgBmp.bClockCount = true;
            g_bCmdPostSemToFunc = true;
        }
        else
        {
            g_systemState.clockSyncState = clockSyncState;
        }
    }
}


/*********************************************************************************************
函 数 名: cmdClockSyncStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockSyncStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = sizeof(g_systemState.clockSyncState);
    pData = (u8 *)&g_systemState.clockSyncState;
    cmdFrameSend(CMD_CLOCK, CLOCK_SYNCSTATEQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdClockCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClockCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
	dataLen = sizeof(g_systemState.clockCount);
    pData = (u8 *)&g_systemState.clockCount;
    cmdFrameSend(CMD_CLOCK, CLOCK_COUNTQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdClcokCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClcokCmdInit(void)
{
    memset(pClockCmdFunc, 0, sizeof(pClockCmdFunc));
    
    pClockCmdFunc[CLOCK_FREQUENCY]  = cmdClockFrequencySet;
    pClockCmdFunc[CLOCK_FREQUENCYQ] = cmdClockFrequencyQuery;
    pClockCmdFunc[CLOCK_SYNCREG]    = cmdClockSyncRegSet;
    pClockCmdFunc[CLOCK_SYNCREGQ]   = cmdClockSyncRegQuery;
    pClockCmdFunc[CLOCK_STARTTYPE]  = cmdClockSyncStartTypeSet;
    pClockCmdFunc[CLOCK_STARTTYPEQ] = cmdClockSyncStartTypeQuery; 
    pClockCmdFunc[CLOCK_SYNCSTATE]  = cmdClockSyncStateSet;
    pClockCmdFunc[CLOCK_SYNCSTATEQ] = cmdClockSyncStateQuery;
    pClockCmdFunc[CLOCK_COUNTQ]     = cmdClockCountQuery;
}

            
/*********************************************************************************************
函 数 名: cmdClcokCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdClcokCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < CLOCK_RESERVE) && (pClockCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pClockCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************文件尾********************************************/