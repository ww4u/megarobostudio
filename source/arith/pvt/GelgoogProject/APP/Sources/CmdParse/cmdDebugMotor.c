/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdDebugMotor.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "os.h"
#include <string.h>
#include "cmdDebugMotor.h"
#include "cmdMainParse.h"
#include "servDriver.h"
#include "servSoftTimer.h"
#include "servFPGA.h"

/****************************************�ⲿ��������*****************************************/
#if DEBUG_MOTOR
extern OS_SEM g_semFunctionTask;
extern FuncCfgBmpStruct  g_funcCfgBmp;
#endif

extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
typedef int (*DebugCmdFunc)(u8,u8*);
DebugCmdFunc pDebugCmdFunc[DEBUGCMD_RESERVE];



/******************************************����ʵ��*******************************************/
#if DEBUG_MOTOR

/*********************************************************************************************
�� �� ��: cmdSetWhichDriver
ʵ�ֹ���: ���õ������������
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdSetWhichDriver(u8 cmdDataLen, u8 *pCmdData)
{
    bspDelayUs(50);
    return 0;
}
/*********************************************************************************************
�� �� ��:cmdReadReg
ʵ�ֹ���:��ȡTMC26xx�ļĴ���
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdReadDrvType(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 0)
    {
      return -1;
    }
    
    cmdFrameSend(CMD_DEBUG, DEBUGCMD_DRIVER_TYPE_Query, 1, (u8*)&g_stTMC26xxDrvConf.driver_type);
    return 0;
}

/*********************************************************************************************
�� �� ��:cmdReadReg
ʵ�ֹ���:��ȡTMC26xx�ļĴ���
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdReadStatusReg(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 1)
    {
      return -1;
    }
    u32 u32Data = 0;
    u8 reg =  pCmdData[0];
    servTMC_Reg_Read(reg,&u32Data);
    bspDelayUs(10);
#if JTAG_MODE  
    if(reg == RDSEL_SG_LVL)
    {
        printf("sg=%d\n",(u32Data>>10)&0x3FF);
    }
    else if(reg == RDSEL_SG_CS_LVL)
    {
        printf("sg=%d;se=%d\n",((u32Data>>15)&0x1F)<<5,(u32Data>>10)&0x1F);
    }  
#endif
    cmdFrameSend(CMD_DEBUG, DEBUGCMD_READ_STATUS_REG, 4, (u8*)&u32Data);
    return 0;
}

static void cmdDebugHoldSpeed()
{

}
/*********************************************************************************************
�� �� ��:cmdDebugStartMotor
ʵ�ֹ���:
�������:level:�ٶ�ˮƽ->��(fast)--��(med)--��(low)
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdDebugStartMotor(u8 cmdDataLen, u8 *pCmdData)
{
    u32 speed;
    if(cmdDataLen != 4)
    {
      return -1;
    }
    memcpy((void*)&speed,pCmdData,4);
    g_stTMC26xxDrvConf.speed = speed;
    g_funcCfgBmp.bDebugMotor = true;
    cmdDebugHoldSpeed();
    OSSemPost(&g_semFunctionTask,OS_OPT_POST_ALL,NULL);
    return 0;
}
/*********************************************************************************************
�� �� ��:cmdDebugStopMotor
ʵ�ֹ���:
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdDebugStopMotor(u8 cmdDataLen, u8 *pCmdData)
{
    StopConfigStruct stopConfigInfo = {.stopMode = STOPMODE_IMMED};

    if(cmdDataLen != 0)
    {
        return -1;
    }
    //serv_FPGA_Break_Init();
    servFpgaWaveStop(0, stopConfigInfo);    //FOR MODIFY NICK
    
    //servFPGA_TMC_Init(g_stTMC26xxDrvConf.driver_type,MOSFET_OPEN,TMCCLK_OPEN);
    return 0;
}
/*********************************************************************************************
�� �� ��:cmdDebugConfigDrvCtrl
ʵ�ֹ���:
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdDebugConfigDrvCtrl(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 4)
    {
      return -1;
    }
    memcpy(&g_stTMC26xxDrvConf.unCtrlStepDir.u32CtrlStepDir,pCmdData,4);
    servTMC_Reg_Write(ADDR_TMC_DRVCTRL);
    return 0;
}
/*********************************************************************************************
�� �� ��:cmdDebugConfigSgcsConf
ʵ�ֹ���:����StallGuard�Ĵ���
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdDebugConfigSgcsConf(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 4)
    {
      return -1;
    }
    memcpy(&g_stTMC26xxDrvConf.unSgcsConf.u32SgcsConf,pCmdData,4);
    servTMC_Reg_Write(ADDR_TMC_SGCSCONF);
    return 0;
}

/*********************************************************************************************
�� �� ��:cmdDebugConfigChopConf
ʵ�ֹ���:����ն���Ĵ���
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdDebugConfigChopConf(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 4)
    {
      return -1;
    }
    memcpy(&g_stTMC26xxDrvConf.unChopConf.u32ChopConf,pCmdData,4);
    servTMC_Reg_Write(ADDR_TMC_CHOPCONF);
    return 0;
}
/*********************************************************************************************
�� �� ��:cmdDebugConfigDrvConf
ʵ�ֹ���:
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdDebugConfigDrvConf(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 4)
    {
      return -1;
    }
    memcpy(&g_stTMC26xxDrvConf.unDrvConf.u32DrvConf,pCmdData,4);
    servTMC_Reg_Write(ADDR_TMC_DRVCONF);
    return 0;
}
/*********************************************************************************************
�� �� ��:cmdDebugConfigSmarten
ʵ�ֹ���:����cool step �Ĵ���
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdDebugConfigSmarten(u8 cmdDataLen, u8 *pCmdData)
{
    if(cmdDataLen != 4)
    {
      return -1;
    }
    memcpy(&g_stTMC26xxDrvConf.unSmarten.u32Smarten,pCmdData,4);
    servTMC_Reg_Write(ADDR_TMC_SMARTEN);
    return 0;
}
/*********************************************************************************************
�� �� ��:cmdDebugReadConfigReg
ʵ�ֹ���:��ȡTMC26XX���üĴ�������
�������:
�������:
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
static s32 cmdDebugReadConfigReg(u8 cmdDataLen, u8 *pCmdData)
{
    u8 reg = 0;//��ȡ�ĸ��Ĵ���,ȡֵΪ 0,4,5,6,7
    u32 data = 0;
    if(cmdDataLen != 1)
    {
      return -1;
    }
    reg = pCmdData[0];
    switch(reg)
    {
    case ADDR_TMC_DRVCTRL:
      data = g_stTMC26xxDrvConf.unCtrlStepDir.u32CtrlStepDir;
      break;
    case ADDR_TMC_SGCSCONF:
      data = g_stTMC26xxDrvConf.unSgcsConf.u32SgcsConf;
      break;
    case ADDR_TMC_CHOPCONF:
      data = g_stTMC26xxDrvConf.unChopConf.u32ChopConf;
      break;
    case ADDR_TMC_DRVCONF:
      data = g_stTMC26xxDrvConf.unDrvConf.u32DrvConf;
      break;
    case ADDR_TMC_SMARTEN:
      data = g_stTMC26xxDrvConf.unSmarten.u32Smarten;
      break;
    default:
      break;
    }
    cmdFrameSend(CMD_DEBUG, DEBUGCMD_TMC26XX_REG_Query, 4, (u8*)&data);
    return 0;
}
#else
static s32 cmdSetWhichDriver(u8 cmdDataLen, u8 *pCmdData){return 0;};
static s32 cmdReadDrvType(u8 cmdDataLen, u8 *pCmdData){return 0;}
static s32 cmdReadStatusReg(u8 cmdDataLen, u8 *pCmdData){return 0;}
static s32 cmdDebugStartMotor(u8 cmdDataLen, u8 *pCmdData){return 0;}
static s32 cmdDebugStopMotor(u8 cmdDataLen, u8 *pCmdData){return 0;}


static s32 cmdDebugConfigDrvCtrl(u8 cmdDataLen, u8 *pCmdData){return 0;};
static s32 cmdDebugConfigSgcsConf(u8 cmdDataLen, u8 *pCmdData){return 0;};
static s32 cmdDebugConfigChopConf(u8 cmdDataLen, u8 *pCmdData){return 0;};
static s32 cmdDebugConfigDrvConf(u8 cmdDataLen, u8 *pCmdData){return 0;};
static s32 cmdDebugConfigSmarten(u8 cmdDataLen, u8 *pCmdData){return 0;};
static s32 cmdDebugReadConfigReg(u8 cmdDataLen, u8 *pCmdData){return 0;};
#endif

#if DEBUG_CAN_TEST
s32 cmdDebugCanTest(u8 cmdDataLen, u8 *pCmdData)
{
    u32 value = 0;
    int i = 0;
    u8 data[6];
    u8 chanNum = *pCmdData++;
    memcpy(&value,pCmdData,4);
    value+=1;
    
    if (chanNum <= CH_MAX)
    {
        data[0] = chanNum;
        memcpy(&data[1],&value,4);
        cmdFrameSend(CMD_DEBUG, DEBUGCMD_CAN_TEST,5, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                data[0] = i;
                memcpy(&data[1],&value,4);
                cmdFrameSend(CMD_DEBUG, DEBUGCMD_CAN_TEST,5, data);
            }
        }
    }

    return 0;
}
#else
s32 cmdDebugCanTest(u8 cmdDataLen, u8 *pCmdData){return 0;};
#endif
/*********************************************************************************************
�� �� ��: DebugCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdDebugCmdInit(void)
{
    memset(pDebugCmdFunc, 0, sizeof(pDebugCmdFunc));
    
    pDebugCmdFunc[DEBUGCMD_SET_DRIVER_TYPE]     = cmdSetWhichDriver;
    pDebugCmdFunc[DEBUGCMD_DRIVER_TYPE_Query]   = cmdReadDrvType;
    pDebugCmdFunc[DEBUGCMD_READ_STATUS_REG]     = cmdReadStatusReg;
    pDebugCmdFunc[DEBUGCMD_START_MOTOR]         = cmdDebugStartMotor;
    pDebugCmdFunc[DEBUGCMD_STOP_MOTOR]          = cmdDebugStopMotor; 
    pDebugCmdFunc[DEBUGCMD_SET_DRVCTRL_REG]     = cmdDebugConfigDrvCtrl;
    pDebugCmdFunc[DEBUGCMD_SET_DRVCONF_REG]     = cmdDebugConfigDrvConf;
    pDebugCmdFunc[DEBUGCMD_SET_CHOPCONF_REG]    = cmdDebugConfigChopConf;
    pDebugCmdFunc[DEBUGCMD_SET_SGCSCONF_REG]    = cmdDebugConfigSgcsConf;
    pDebugCmdFunc[DEBUGCMD_SET_SMARTEN_REG]     = cmdDebugConfigSmarten;
    pDebugCmdFunc[DEBUGCMD_TMC26XX_REG_Query]   = cmdDebugReadConfigReg;
    pDebugCmdFunc[DEBUGCMD_CAN_TEST]            = cmdDebugCanTest;
    g_stTMC26xxDrvConf.speed = 10000;
}
      
/*********************************************************************************************
�� �� ��: cmdDebugCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdDebugCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    if ((pCmdStackFrame->subType < DEBUGCMD_RESERVE) && (pDebugCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pDebugCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


#if DEBUG_MOTOR
#define MIN_SPEED_UP     10000
#define WAVE_TABLE_LEN   512
u32 wave_table_data[WAVE_TABLE_LEN];

void cmdDebugMotorStart(void )
{
  #if 0
    int i = 0,j = 0,count;
    u32 speed_start = MIN_SPEED_UP,speed_end = 0,speed;
    u32 wave_ddr_addr = 0;
    
    speed_end = g_stTMC26xxDrvConf.speed;
    //servFPGA_TMC_Init(g_stTMC26xxDrvConf.driver_type,MOSFET_OPEN,TMCCLK_OPEN);
   
    servFPGA_Ncycle_PWM_Reset();
    if(speed_end >= speed_start)
    {
        speed = speed_end;
        for(i = count;i < WAVE_TABLE_LEN;i++)
        {
            wave_table_data[i] = speed; 
            wave_table_data[i]  |= 0x01<<31;//���Ϊ��Ч����
            wave_table_data[i]  |= 0x02<<27;//���÷���
        }
        serv_Ncycle_Data_Write_do(EXECMODE_NCYCLE,wave_table_data,WAVE_TABLE_LEN*4);
    }
    else
    {
        for(i = speed_start,count = 0;i > speed_end;i-=10)
        {
            for(j = 0;j <= (count/500)*6;j++)
            {
                if(count != 0 && (count%WAVE_TABLE_LEN) == 0)
                {
                    serv_Ncycle_Data_Write_do(EXECMODE_NCYCLE,wave_table_data,WAVE_TABLE_LEN*4);
                }
                wave_table_data[count%WAVE_TABLE_LEN] = (u32)i; 
                wave_table_data[count%WAVE_TABLE_LEN]  |= 0x01<<31;//���Ϊ��Ч����
                wave_table_data[count%WAVE_TABLE_LEN]  |= 0x02<<27;//���÷���
                count++;
            }  
        }
        //��ȫΪһ������
        speed = i;
        if((count%WAVE_TABLE_LEN) < WAVE_TABLE_LEN)
        {
            for(i = count%WAVE_TABLE_LEN;i < WAVE_TABLE_LEN;i++)
            {
                wave_table_data[i] = speed; 
                wave_table_data[i]  |= 0x01<<31;//���Ϊ��Ч����
                wave_table_data[i]  |= 0x02<<27;//���÷���
            }
            serv_Ncycle_Data_Write_do(EXECMODE_NCYCLE,wave_table_data,WAVE_TABLE_LEN*4);
        }
    }
    servFPGA_RUN_Cfg(NCYCLE_BASE_ADDR,1,CYCLE_MODE);  //��ʼ��ַ  ѭ����   ֹͣģʽ
    while( servFPGA_PWM_IF_Ready(EXECMODE_NCYCLE) != 0)//ֻ����ready����²ſ��Կ�ʼ����
    {
        bspDelayUs(1);
    }
    servFPGA_PWM_Start();
    #endif
}

#endif

/*******************************************�ļ�β********************************************/
