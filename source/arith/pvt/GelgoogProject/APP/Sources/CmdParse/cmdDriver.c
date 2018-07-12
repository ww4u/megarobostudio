/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdDriver.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ��;
��    ��:  CJ;
�������:  2017.08.11;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "project.h"
#include "cmdDriver.h"
#include "pvrfDriver.h"
#include "cmdMainParse.h"
#include "servDriver.h"
#include "servSoftTimer.h"
#include "servSystemPara.h"



/****************************************�ⲿ��������*****************************************/
extern SystemInfoStruct   g_systemInfo;
extern SystemStateStruct  g_systemState;
extern DriverInfoStruct   g_driverInfo; 
extern ChanCfgBmpStruct   g_chanCfgBmp[CH_TOTAL];

extern bool g_bCmdPostSemToFunc;
extern bool g_bPerdPostSemToFunc;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pDriverCmdFunc[DRIVERCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdDriverTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_systemState.drvType[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_systemState.drvType[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_systemState.drvType[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_TYPEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_systemState.drvType[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_systemState.drvType[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_TYPEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverCurrentSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverCurrentSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   driverCurr;
    u8   i;
    u8   vsense;
    u8   csValue;
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
    if (PARA_VERIFY_NO_ERROR == pvrfDriverCurrVerify(cmdDataLen, pCmdData, (void *)&driverCurr))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].curr = driverCurr;

                //�����µ�����ص�CS��VSENSE�����ּĴ���ֵһ��
                servDriverCurrRegParaCalc(g_driverInfo.driver[i].type, 
                                          g_driverInfo.driver[i].curr, 
                                          &vsense,
                                          &csValue);
                g_driverInfo.driver[i].DRVCONF.regBitFiled.VSENSE = vsense;
                g_driverInfo.driver[i].SGCSCONF.regBitFiled.CS    = csValue;
        
                g_chanCfgBmp[i].bCurrent = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servDriverInfoStore(&g_driverInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverCurrentQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverCurrentQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].curr) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].curr;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.driver[chanNum].curr);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_CURRENTQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].curr;
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.driver[i].curr);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_CURRENTQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverMicroStepsSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverMicroStepsSet(u8 cmdDataLen, u8 *pCmdData)
{
    MicroStepEnum microSteps;
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
    if (PARA_VERIFY_NO_ERROR == pvrfDriverMicroStepsVerify(cmdDataLen, pCmdData, (void *)&microSteps))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES = microSteps;

                g_chanCfgBmp[i].bMicroStep = true;
                g_bCmdPostSemToFunc = true;
            }
        }

        servDriverInfoStore(&g_driverInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverMicroStepsQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverMicroStepsQuery(u8 cmdDataLen, u8 *pCmdData)
{
    MicroStepEnum microSteps;
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(microSteps) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        microSteps = (MicroStepEnum)g_driverInfo.driver[chanNum].DRVCTRL.stepDirMode.MRES;
        pData = (u8 *)&microSteps;
        data[0] = chanNum;
        for (i = 0;i < sizeof(microSteps);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_MICROSTEPSQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                microSteps = (MicroStepEnum)g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES;
                pData = (u8 *)&microSteps;
                data[0] = i;
                for (j = 0;j < sizeof(microSteps);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_MICROSTEPSQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverSwitchSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverSwitchSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
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
    if (PARA_VERIFY_NO_ERROR == pvrfDriverStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].state = state;
        
                g_chanCfgBmp[i].bDriverState = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servDriverInfoStore(&g_driverInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverSwitchQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverSwitchQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].state) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].state;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.driver[chanNum].state);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_SWITCHQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].state;
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.driver[i].state);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_SWITCHQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverRegisterSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverRegisterSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   drvReg;
    u32  regValue;
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
    if (PARA_VERIFY_NO_ERROR == pvrfDriverRegisterVerify(cmdDataLen, pCmdData, (void *)&regValue, &drvReg))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                switch ((DriverRegisterEnum)drvReg)
                {
                    case DRVREG_DRVCTRL:
                        g_driverInfo.driver[i].DRVCTRL.regValue = regValue;
                        
                        g_chanCfgBmp[i].bDrvCtrl = true;
                        g_bCmdPostSemToFunc = true;
                      break;

                    case DRVREG_CHOPCONF:
                        g_driverInfo.driver[i].CHOPCONF.regValue = regValue;
                        
                        g_chanCfgBmp[i].bChopConfig = true;
                        g_bCmdPostSemToFunc = true;
                      break;

                    case DRVREG_SMARTEN:
                        g_driverInfo.driver[i].SMARTEN.regValue = regValue;
                        
                        g_chanCfgBmp[i].bSmarten = true;
                        g_bCmdPostSemToFunc = true;
                      break;

                    case DRVREG_SGCSCONF:
                        g_driverInfo.driver[i].SGCSCONF.regValue = regValue;

                        //����CS��VSENSEֵ������CURR
                        g_driverInfo.driver[i].curr = servDriverCurrCalc(g_driverInfo.driver[i].type,
                                                                         g_driverInfo.driver[i].DRVCONF.regBitFiled.VSENSE,
                                                                         g_driverInfo.driver[i].SGCSCONF.regBitFiled.CS);
                        
                        g_chanCfgBmp[i].bSgcsConfig = true;
                        g_bCmdPostSemToFunc = true;
                      break;

                    case DRVREG_DRVCONF:
                        g_driverInfo.driver[i].DRVCONF.regValue = regValue;

                        //����CS��VSENSEֵ������CURR
                        g_driverInfo.driver[i].curr = servDriverCurrCalc(g_driverInfo.driver[i].type,
                                                                         g_driverInfo.driver[i].DRVCONF.regBitFiled.VSENSE,
                                                                         g_driverInfo.driver[i].SGCSCONF.regBitFiled.CS);
                        
                        g_chanCfgBmp[i].bDrvConfig = true;
                        g_bCmdPostSemToFunc = true;
                      break;

                    default:
                      break;
                }
            }
        }
        
        servDriverInfoStore(&g_driverInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverRegisterQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverRegisterQuery(u8 cmdDataLen, u8 *pCmdData)
{
    DriverRegisterEnum drvReg;
    u8  dataLen;
    u8  *pData;
    u8  data[6];
    u8  i, j;
    u32 regValue;
    u8  chanNum = *pCmdData++;

    
    dataLen = sizeof(DriverRegisterEnum) + sizeof(chanNum);
    if (cmdDataLen == dataLen)
    {
        dataLen += sizeof(regValue);
        drvReg = (DriverRegisterEnum)*pCmdData;
        if (chanNum <= CH_MAX)
        {
            switch (drvReg)
            {
                case DRVREG_DRVCTRL:
                    regValue = g_driverInfo.driver[chanNum].DRVCTRL.regValue;
                  break;

                case DRVREG_CHOPCONF:
                    regValue = g_driverInfo.driver[chanNum].CHOPCONF.regValue;
                  break;

                case DRVREG_SMARTEN:
                    regValue = g_driverInfo.driver[chanNum].SMARTEN.regValue;
                  break;

                case DRVREG_SGCSCONF:
                    regValue = g_driverInfo.driver[chanNum].SGCSCONF.regValue;
                  break;

                case DRVREG_DRVCONF:
                    regValue = g_driverInfo.driver[chanNum].DRVCONF.regValue;
                  break;

                default:
                    return;
                  break;
            }
            
            pData = (u8 *)&regValue;
            data[0] = chanNum;
            data[1] = drvReg;
            for (i = 0;i < sizeof(regValue);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_DRIVER, DRIVERCMD_REGCONFQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    switch (drvReg)
                    {
                        case DRVREG_DRVCTRL:
                            regValue = g_driverInfo.driver[i].DRVCTRL.regValue;
                          break;

                        case DRVREG_CHOPCONF:
                            regValue = g_driverInfo.driver[i].CHOPCONF.regValue;
                          break;

                        case DRVREG_SMARTEN:
                            regValue = g_driverInfo.driver[i].SMARTEN.regValue;
                          break;

                        case DRVREG_SGCSCONF:
                            regValue = g_driverInfo.driver[i].SGCSCONF.regValue;
                          break;

                        case DRVREG_DRVCONF:
                            regValue = g_driverInfo.driver[i].DRVCONF.regValue;
                          break;

                        default:
                            return;
                          break;
                    }
                    
                    pData = (u8 *)&regValue;
                    data[0] = i;
                    data[1] = drvReg;
                    for (j = 0;j < sizeof(regValue);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_DRIVER, DRIVERCMD_REGCONFQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverStateRegQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverStateRegQuery(u8 cmdDataLen, u8 *pCmdData)
{
    DriverReadRespEnum readReg;
    u32 regValue;
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(DriverReadRespEnum) + sizeof(chanNum);
    if (cmdDataLen == dataLen)
    {
        dataLen += sizeof(regValue);
        readReg = (DriverReadRespEnum)*pCmdData;
        if (readReg < READREG_RESERVE)
        {
            if (chanNum <= CH_MAX)
            {
                servDriverStateRegRead(chanNum, readReg, &regValue);

                pData = (u8 *)&regValue;
                data[0] = chanNum;
                for (i = 0;i < sizeof(regValue);i++)
                {
                    data[1 + i] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_STATEREGQ, dataLen, data);
            }
            else
            {
                for (i = 0;i < g_systemState.chanNum;i++)
                {
                    if ((CH_ALL == chanNum) ||
                        (chanNum == g_systemInfo.group[i][0]) ||
                        (chanNum == g_systemInfo.group[i][1]))
                    {
                        servDriverStateRegRead(i, readReg, &regValue);
                        
                        pData = (u8 *)&regValue;
                        data[0] = i;
                        for (j = 0;j < sizeof(regValue);j++)
                        {
                            data[1 + j] = *pData++;
                        }
                        cmdFrameSend(CMD_DRIVER, DRIVERCMD_STATEREGQ, dataLen, data);
                    }
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverSGLimitSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverSGLimitSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   drvReg;
    s8   sgLimit;
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
    if (PARA_VERIFY_NO_ERROR == pvrfDriverSGLimitVerify(cmdDataLen, pCmdData, (void *)&sgLimit, &drvReg))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                switch ((LimitEnum)drvReg)
                {
                    case LIMIT_UP:
                        g_driverInfo.driver[i].sgUpLimit = sgLimit;
                      break;

                    case LIMIT_DOWN:
                        g_driverInfo.driver[i].sgDnLimit = sgLimit;
                      break;

                    default:
                      break;
                }
            }
        }
        
        servDriverInfoStore(&g_driverInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverSGLimitQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverSGLimitQuery(u8 cmdDataLen, u8 *pCmdData)
{
    LimitEnum drvReg;
    u8  dataLen;
    u8  *pData;
    u8  data[6];
    u8  i, j;
    s8  sgLimit;
    u8  chanNum = *pCmdData++;

    
    dataLen = sizeof(LimitEnum) + sizeof(chanNum);
    if (cmdDataLen == dataLen)
    {
        dataLen += sizeof(sgLimit);
        drvReg = (LimitEnum)*pCmdData;
        if (chanNum <= CH_MAX)
        {
            switch (drvReg)
            {
                case LIMIT_UP:
                    sgLimit = g_driverInfo.driver[chanNum].sgUpLimit;
                  break;

                case LIMIT_DOWN:
                    sgLimit = g_driverInfo.driver[chanNum].sgDnLimit;
                  break;

                default:
                    return;
                  break;
            }
            
            pData = (u8 *)&sgLimit;
            data[0] = chanNum;
            data[1] = drvReg;
            for (i = 0;i < sizeof(sgLimit);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_DRIVER, DRIVERCMD_SGLIMITQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    switch (drvReg)
                    {
                        case LIMIT_UP:
                            sgLimit = g_driverInfo.driver[i].sgUpLimit = sgLimit;
                          break;

                        case LIMIT_DOWN:
                            sgLimit = g_driverInfo.driver[i].sgDnLimit = sgLimit;
                          break;

                        default:
                            return;
                          break;
                    }
                    
                    pData = (u8 *)&sgLimit;
                    data[0] = i;
                    data[1] = drvReg;
                    for (j = 0;j < sizeof(sgLimit);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_DRIVER, DRIVERCMD_REGCONFQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverSGParaSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverSGParaSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   drvReg;
    s16  sgParaValue;
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
    if (PARA_VERIFY_NO_ERROR == pvrfDriverSGParaVerify(cmdDataLen, pCmdData, (void *)&sgParaValue, &drvReg))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                switch ((SGParaEnum)drvReg)
                {
                    case SGPARA_SG0:
                        g_driverInfo.driver[chanNum].sgZero = sgParaValue;
                      break;

                    case SGPARA_SGT:
                        g_driverInfo.driver[chanNum].sgThreshold = sgParaValue;
                      break;

                    case SGPARA_SEMAX:
                        g_driverInfo.driver[chanNum].seMax = sgParaValue;
                      break;

                    case SGPARA_SEMIN:
                        g_driverInfo.driver[chanNum].seMin = sgParaValue;
                      break;

                    default:
                        return;
                      break;
                }
            }
        }
        
        servDriverInfoStore(&g_driverInfo);
    }
}




/*********************************************************************************************
�� �� ��: cmdDriverSGParaQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverSGParaQuery(u8 cmdDataLen, u8 *pCmdData)
{
    SGParaEnum sgPara;
    u8  dataLen;
    u8  *pData;
    u8  data[6];
    u8  i, j;
    s16 sgParaValue;
    u8  chanNum = *pCmdData++;

    
    dataLen = sizeof(SGParaEnum) + sizeof(chanNum);
    if (cmdDataLen == dataLen)
    {
        dataLen += sizeof(sgParaValue);
        sgPara = (SGParaEnum)*pCmdData;
        if (chanNum <= CH_MAX)
        {
            switch (sgPara)
            {
                case SGPARA_SG0:
                    sgParaValue = g_driverInfo.driver[chanNum].sgZero;
                  break;

                case SGPARA_SGT:
                    sgParaValue = g_driverInfo.driver[chanNum].sgThreshold;
                  break;

                case SGPARA_SEMAX:
                    sgParaValue = g_driverInfo.driver[chanNum].seMax;
                  break;

                case SGPARA_SEMIN:
                    sgParaValue = g_driverInfo.driver[chanNum].seMin;
                  break;

                default:
                    return;
                  break;
            }
            
            pData = (u8 *)&sgParaValue;
            data[0] = chanNum;
            data[1] = sgPara;
            for (i = 0;i < sizeof(sgParaValue);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_DRIVER, DRIVERCMD_SGPARAQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    switch (sgPara)
                    {
                        case SGPARA_SG0:
                            sgParaValue = g_driverInfo.driver[i].sgUpLimit;
                          break;

                        case SGPARA_SGT:
                            sgParaValue = g_driverInfo.driver[i].sgDnLimit;
                          break;

                        case SGPARA_SEMAX:
                            sgParaValue = g_driverInfo.driver[i].sgDnLimit;
                          break;

                        case SGPARA_SEMIN:
                            sgParaValue = g_driverInfo.driver[i].sgDnLimit;
                          break;

                        default:
                            return;
                          break;
                    }
                    
                    pData = (u8 *)&sgParaValue;
                    data[0] = i;
                    data[1] = sgPara;
                    for (j = 0;j < sizeof(sgParaValue);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_DRIVER, DRIVERCMD_SGPARAQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdDriverCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverCmdInit(void)
{
    memset(pDriverCmdFunc, 0, sizeof(pDriverCmdFunc));

    pDriverCmdFunc[DRIVERCMD_TYPEQ]     = cmdDriverTypeQuery;
    pDriverCmdFunc[DRIVERCMD_STATEREGQ] = cmdDriverStateRegQuery; 
    pDriverCmdFunc[DRIVERCMD_CURRENT]   = cmdDriverCurrentSet;
    pDriverCmdFunc[DRIVERCMD_CURRENTQ]  = cmdDriverCurrentQuery;  
    pDriverCmdFunc[DRIVERCMD_MICROSTEPS]  = cmdDriverMicroStepsSet;
    pDriverCmdFunc[DRIVERCMD_MICROSTEPSQ] = cmdDriverMicroStepsQuery; 
    pDriverCmdFunc[DRIVERCMD_SWITCH]   = cmdDriverSwitchSet;
    pDriverCmdFunc[DRIVERCMD_SWITCHQ]  = cmdDriverSwitchQuery; 
    
    pDriverCmdFunc[DRIVERCMD_REGCONF]  = cmdDriverRegisterSet;
    pDriverCmdFunc[DRIVERCMD_REGCONFQ] = cmdDriverRegisterQuery;
    pDriverCmdFunc[DRIVERCMD_SGLIMIT]  = cmdDriverSGLimitSet;
    pDriverCmdFunc[DRIVERCMD_SGLIMITQ] = cmdDriverSGLimitQuery;
    pDriverCmdFunc[DRIVERCMD_SGPARA]   = cmdDriverSGParaSet;
    pDriverCmdFunc[DRIVERCMD_SGPARAQ]  = cmdDriverSGParaQuery;
}

            
/*********************************************************************************************
�� �� ��: cmdDriverCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void cmdDriverCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < DRIVERCMD_RESERVE) && (pDriverCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pDriverCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************�ļ�β********************************************/
