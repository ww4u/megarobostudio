/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdClockSync.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "cmdClockSync.h"
#include "pvrfClockSync.h"
#include "cmdMainParse.h"
#include "servSystemPara.h"



/****************************************�ⲿ��������*****************************************/
extern SystemStateStruct  g_systemState;
extern SystemInfoStruct   g_systemInfo;
extern SystemCfgBmpStruct g_systemCfgBmp;

extern bool g_bCmdPostSemToFunc;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pClockCmdFunc[CLOCK_RESERVE];

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
u32 fpgaClockCount = 0;
#endif



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdClockFrequencySet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdClockFrequencySet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 fpgaPwmClock;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfClockFrequencyVerify(cmdDataLen, pCmdData, (void *)&fpgaPwmClock))
    {
#if PVT_CALC_USE_FPGA_CLOCK_ERROR
        fpgaClockCount = fpgaPwmClock;

        if (fpgaClockCount > 0)
        {
            //�����һ��������
            g_systemInfo.fpgaPwmClock = ((f64)((s64)g_systemState.clockCount - fpgaClockCount)) * FPGA_PWM_CLOCK / fpgaClockCount;
        }
        
#else

        g_systemInfo.fpgaPwmClock = fpgaPwmClock;
#endif

        //�浽EEPROM��
        servSystemInfoWrite(&g_systemInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdClockFrequencyQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdClockSyncRegSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdClockSyncRegQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdClockSyncStartTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdClockSyncStartTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdClockSyncStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdClockSyncStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    ClockSyncStateEnum clockSyncState;
    

    //��λ�����ʹ�����ǰ��Ҫ������MOTIONREGISTER�Ĵ���
    //����ֱ����FPGA������Ӧ�����ﲻ��Ҫ����ֻ��¼��״̬
    //ʵ����FPGA�ڽ��յ���������ʱ(��������Ϊ��ʼ)�Ѿ���ʼ����ʱ�Ӽ�����
    //����������Ϊ����ʱ����ҪFUNC�̶߳�ȡ����ֵ���ȴ���λ����ȡ
    if (PARA_VERIFY_NO_ERROR == pvrfClockSyncStateVerify(cmdDataLen, pCmdData, (void *)&clockSyncState))
    {
        if ((CLOCKSYNC_END == clockSyncState) && (CLOCKSYNC_START == g_systemState.clockSyncState))
        {
            //��FUNC�����ź�������ȡ����ֵ����ȡ��ɺ����ͬ������
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
�� �� ��: cmdClockSyncStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdClockCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdClcokCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdClcokCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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



/*******************************************�ļ�β********************************************/