/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdDriver.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  无;
作    者:  CJ;
完成日期:  2017.08.11;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include "project.h"
#include "cmdDriver.h"
#include "pvrfDriver.h"
#include "cmdMainParse.h"
#include "servDriver.h"
#include "servSoftTimer.h"
#include "servSystemPara.h"
#include "servAlgorithm.h"



/****************************************外部变量声明*****************************************/
extern SystemInfoStruct   g_systemInfo;
extern SystemStateStruct  g_systemState;
extern DriverInfoStruct   g_driverInfo; 
extern ChanCfgBmpStruct   g_chanCfgBmp[CH_TOTAL];
extern MotorInfoStruct    g_motorInfo;
extern SystemCfgBmpStruct g_systemCfgBmp;

extern bool g_bCmdPostSemToFunc;
extern bool g_bPerdPostSemToFunc;

#if GELGOOG_AXIS_10
extern SoftTimerStruct g_driverFaultTimer;
#else
extern SoftTimerStruct g_drvStateReadTimer[CH_TOTAL];
#endif

extern SoftTimerStruct g_paraSaveTimer;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pDriverCmdFunc[DRIVERCMD_RESERVE];

#if !GELGOOG_AXIS_10
SubCmdProFunc pTuningCmdFunc[TUNINGCMD_RESERVE];
#endif



/******************************************函数实现*******************************************/
#if !GELGOOG_AXIS_10
/*********************************************************************************************
函 数 名: cmdDriverTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].type) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].type;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.driver[chanNum].type);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_TYPEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].type;
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.driver[i].type);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_TYPEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdDriverCurrentSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverCurrVerify(cmdDataLen, pCmdData, (void *)&driverCurr))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].curr = driverCurr;

                //计算下电流相关的CS和VSENSE，保持寄存器值一致
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
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverCurrentQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
        for (i = 0;i < CH_TOTAL;i++)
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
函 数 名: cmdDriverMicroStepsSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverMicroStepsVerify(cmdDataLen, pCmdData, (void *)&microSteps))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES = microSteps;

                //更新下转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                                      &g_systemState.posnConvertInfo[i]);

                g_chanCfgBmp[i].bMicroStep = true;
                g_bCmdPostSemToFunc = true;
            }
        }

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverMicroStepsQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
        for (i = 0;i < CH_TOTAL;i++)
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
函 数 名: cmdDriverStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverStateSet(u8 cmdDataLen, u8 *pCmdData)
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].state = state;
        
                g_chanCfgBmp[i].bDriverState = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverStateQuery(u8 cmdDataLen, u8 *pCmdData)
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
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_STATEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
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
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_STATEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdDriverRegisterSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverRegisterVerify(cmdDataLen, pCmdData, (void *)&regValue, &drvReg))
    {
        for (i = 0;i < CH_TOTAL;i++)
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

                        //根据CS和VSENSE值更新下CURR
                        g_driverInfo.driver[i].curr = servDriverCurrCalc(g_driverInfo.driver[i].type,
                                                                         g_driverInfo.driver[i].DRVCONF.regBitFiled.VSENSE,
                                                                         g_driverInfo.driver[i].SGCSCONF.regBitFiled.CS);
                        
                        g_chanCfgBmp[i].bSgcsConfig = true;
                        g_bCmdPostSemToFunc = true;
                      break;

                    case DRVREG_DRVCONF:
                        g_driverInfo.driver[i].DRVCONF.regValue = regValue;

                        //根据CS和VSENSE值更新下CURR
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
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverRegisterQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
            for (i = 0;i < CH_TOTAL;i++)
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
函 数 名: cmdDriverStateRegQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
                data[1] = readReg;
                for (i = 0;i < sizeof(regValue);i++)
                {
                    data[2 + i] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_STATEREGQ, dataLen, data);
            }
            else
            {
                for (i = 0;i < CH_TOTAL;i++)
                {
                    if ((CH_ALL == chanNum) ||
                        (chanNum == g_systemInfo.group[i][0]) ||
                        (chanNum == g_systemInfo.group[i][1]))
                    {
                        servDriverStateRegRead(i, readReg, &regValue);
                        
                        pData = (u8 *)&regValue;
                        data[0] = i;
                        data[1] = readReg;
                        for (j = 0;j < sizeof(regValue);j++)
                        {
                            data[2 + j] = *pData++;
                        }
                        cmdFrameSend(CMD_DRIVER, DRIVERCMD_STATEREGQ, dataLen, data);
                    }
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdDriverSGLimitSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverSGLimitSet(u8 cmdDataLen, u8 *pCmdData)
{
}


/*********************************************************************************************
函 数 名: cmdDriverSGLimitQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
                    sgLimit = 0;
                  break;

                case LIMIT_DOWN:
                    sgLimit = 0;
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
            for (i = 0;i < CH_TOTAL;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    switch (drvReg)
                    {
                        case LIMIT_UP:
                            sgLimit = 0;
                          break;

                        case LIMIT_DOWN:
                            sgLimit = 0;
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
函 数 名: cmdDriverSGParaSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverSGParaSet(u8 cmdDataLen, u8 *pCmdData)
{
}



/*********************************************************************************************
函 数 名: cmdDriverSGParaQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
                    sgParaValue = 0;
                  break;

                case SGPARA_SGT:
                    sgParaValue = 0;
                  break;

                case SGPARA_SEMAX:
                    sgParaValue = 0;
                  break;

                case SGPARA_SEMIN:
                    sgParaValue = 0;
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
            for (i = 0;i < CH_TOTAL;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    switch (sgPara)
                    {
                        case SGPARA_SG0:
                            sgParaValue = 0;
                          break;

                        case SGPARA_SGT:
                            sgParaValue = 0;
                          break;

                        case SGPARA_SEMAX:
                            sgParaValue = 0;
                          break;

                        case SGPARA_SEMIN:
                            sgParaValue = 0;
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
函 数 名: cmdDriverIdleCurrentSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverIdleCurrentSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   idleCurr;
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverCurrVerify(cmdDataLen, pCmdData, (void *)&idleCurr))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].idleCurr = idleCurr;

                //如果空闲电流和驱动电流变成一样且此时通道不处于运动状态则需要配置下电流
                if ((g_driverInfo.driver[i].idleCurr == g_driverInfo.driver[i].curr) &&
                    (CHSTATE_OUTPUTING != g_systemState.chanState[i]))
                {
                    //计算下电流相关的CS和VSENSE，保持寄存器值一致
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
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverIdleCurrentQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverIdleCurrentQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].idleCurr) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].idleCurr;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.driver[chanNum].idleCurr);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_IDLECURRQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].idleCurr;
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.driver[i].idleCurr);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_IDLECURRQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdDriverSwitchTimeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverSwitchTimeSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32  switchTime;
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverSwitchTimeVerify(cmdDataLen, pCmdData, (void *)&switchTime))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].switchTime = switchTime;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverSwitchTimeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverSwitchTimeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].switchTime) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].switchTime;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.driver[chanNum].switchTime);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_SWITCHTIMEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].switchTime;
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.driver[i].switchTime);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_SWITCHTIMEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdDriverMiniCurrRatioSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverMiniCurrRatioSet(u8 cmdDataLen, u8 *pCmdData)
{
}


/*********************************************************************************************
函 数 名: cmdDriverMiniCurrRatioQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverMiniCurrRatioQuery(u8 cmdDataLen, u8 *pCmdData)
{
    CurrRatioEnum currRatio;
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(CurrRatioEnum) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        currRatio = (CurrRatioEnum)g_driverInfo.driver[chanNum].SMARTEN.regBitFiled.SEIMIN;
        pData = (u8 *)&currRatio;
        data[0] = chanNum;
        for (i = 0;i < sizeof(CurrRatioEnum);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_DRIVER, DRIVERCMD_MINICURRRATIOQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                currRatio = (CurrRatioEnum)g_driverInfo.driver[i].SMARTEN.regBitFiled.SEIMIN;
                pData = (u8 *)&currRatio;
                data[0] = i;
                for (j = 0;j < sizeof(CurrRatioEnum);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_DRIVER, DRIVERCMD_MINICURRRATIOQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdDriverCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
    pDriverCmdFunc[DRIVERCMD_STATE]       = cmdDriverStateSet;
    pDriverCmdFunc[DRIVERCMD_STATEQ]      = cmdDriverStateQuery; 
    
    pDriverCmdFunc[DRIVERCMD_SGLIMIT]  = cmdDriverSGLimitSet;
    pDriverCmdFunc[DRIVERCMD_SGLIMITQ] = cmdDriverSGLimitQuery;
    pDriverCmdFunc[DRIVERCMD_SGPARA]   = cmdDriverSGParaSet;
    pDriverCmdFunc[DRIVERCMD_SGPARAQ]  = cmdDriverSGParaQuery;
    
    pDriverCmdFunc[DRIVERCMD_IDLECURR]       = cmdDriverIdleCurrentSet;
    pDriverCmdFunc[DRIVERCMD_IDLECURRQ]      = cmdDriverIdleCurrentQuery;
    pDriverCmdFunc[DRIVERCMD_SWITCHTIME]     = cmdDriverSwitchTimeSet;
    pDriverCmdFunc[DRIVERCMD_SWITCHTIMEQ]    = cmdDriverSwitchTimeQuery;
    pDriverCmdFunc[DRIVERCMD_MINICURRRATIO]  = cmdDriverMiniCurrRatioSet;
    pDriverCmdFunc[DRIVERCMD_MINICURRRATIOQ] = cmdDriverMiniCurrRatioQuery;
    
    pDriverCmdFunc[DRIVERCMD_REGCONF]  = cmdDriverRegisterSet;
    pDriverCmdFunc[DRIVERCMD_REGCONFQ] = cmdDriverRegisterQuery;
}

            
/*********************************************************************************************
函 数 名: cmdDriverCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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


#if 0
#endif


/*********************************************************************************************
函 数 名: cmdTuningStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTuningStateSet(u8 cmdDataLen, u8 *pCmdData)
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].tuningState = state;

                servDriverTuningSet(i, &g_driverInfo.driver[i]);
                        
                g_chanCfgBmp[i].bSmarten = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdTuningStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTuningStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].tuningState) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].tuningState;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.driver[chanNum].tuningState);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TUNING, TUNINGCMD_STATEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].tuningState;
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.driver[i].tuningState);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TUNING, TUNINGCMD_STATEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTuningMiniCurrRatioSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdTuningMiniCurrRatioSet(u8 cmdDataLen, u8 *pCmdData)
{
    CurrRatioEnum currRatio;
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTuningCurrRatioVerify(cmdDataLen, pCmdData, (void *)&currRatio))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].currRatio = currRatio;

                servDriverTuningSet(i, &g_driverInfo.driver[i]);
        
                g_chanCfgBmp[i].bSmarten = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdTuningMiniCurrRatioQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdTuningMiniCurrRatioQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].currRatio) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].currRatio;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.driver[chanNum].currRatio);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TUNING, TUNINGCMD_MINICURRRATIOQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].currRatio;
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.driver[i].currRatio);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TUNING, TUNINGCMD_MINICURRRATIOQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTuningEnergyEfficSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdTuningEnergyEfficSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16  energEfficMax;
    u16  energEfficOffset;
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTuningEnergyEfficVerify(cmdDataLen, pCmdData, &energEfficMax, &energEfficOffset))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].energEfficMax    = energEfficMax;
                g_driverInfo.driver[i].energEfficOffset = energEfficOffset;

                servDriverTuningSet(i, &g_driverInfo.driver[i]);
        
                g_chanCfgBmp[i].bSmarten = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdTuningEnergyEfficQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdTuningEnergyEfficQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].energEfficMax) + sizeof(g_driverInfo.driver[CH1].energEfficOffset) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].energEfficMax;
        data[0] = chanNum;
        for (i = 0;i < (dataLen - sizeof(chanNum));i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TUNING, TUNINGCMD_ENERGYEFFICQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].energEfficMax;
                data[0] = i;
                for (j = 0;j < (dataLen - sizeof(chanNum));j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TUNING, TUNINGCMD_ENERGYEFFICQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTuningCurrRegulateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTuningCurrRegulateSet(u8 cmdDataLen, u8 *pCmdData)
{
    CurrIncreEnum currIncre;
    CurrDecreEnum currDecre;
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTuningCurrRegulateVerify(cmdDataLen, pCmdData, &currIncre, &currDecre))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.driver[i].currIncre = currIncre;
                g_driverInfo.driver[i].currDecre = currDecre;

                servDriverTuningSet(i, &g_driverInfo.driver[i]);
        
                g_chanCfgBmp[i].bSmarten = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdTuningCurrRegulateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTuningCurrRegulateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.driver[CH1].currIncre) + sizeof(g_driverInfo.driver[CH1].currDecre) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.driver[chanNum].currIncre;
        data[0] = chanNum;
        for (i = 0;i < (dataLen - sizeof(chanNum));i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TUNING, TUNINGCMD_CURRREGULATEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.driver[i].currIncre;
                data[0] = i;
                for (j = 0;j < (dataLen - sizeof(chanNum));j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TUNING, TUNINGCMD_CURRREGULATEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTuningCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTuningCmdInit(void)
{
    memset(pTuningCmdFunc, 0, sizeof(pTuningCmdFunc));

    pTuningCmdFunc[TUNINGCMD_STATE]  = cmdTuningStateSet;
    pTuningCmdFunc[TUNINGCMD_STATEQ] = cmdTuningStateQuery;
    
    pTuningCmdFunc[TUNINGCMD_MINICURRRATIO]  = cmdTuningMiniCurrRatioSet;
    pTuningCmdFunc[TUNINGCMD_MINICURRRATIOQ] = cmdTuningMiniCurrRatioQuery; 
    pTuningCmdFunc[TUNINGCMD_ENERGYEFFIC]    = cmdTuningEnergyEfficSet;
    pTuningCmdFunc[TUNINGCMD_ENERGYEFFICQ]   = cmdTuningEnergyEfficQuery; 
    
    pTuningCmdFunc[TUNINGCMD_CURRREGULATE]    = cmdTuningCurrRegulateSet;
    pTuningCmdFunc[TUNINGCMD_CURRREGULATEQ]   = cmdTuningCurrRegulateQuery; 
}

            
/*********************************************************************************************
函 数 名: cmdTuningCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTuningCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < TUNINGCMD_RESERVE) && (pTuningCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pTuningCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


#else


/*********************************************************************************************
函 数 名: cmdDriverTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdDriverTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.type[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.type[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.type[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_NEWDRIVER, DRIVERCMD_TYPEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.type[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.type[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_NEWDRIVER, DRIVERCMD_TYPEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdDriverCurrentSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverCurrentSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   driverCurr;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverCurrVerify(cmdDataLen, pCmdData, (void *)&driverCurr))
    {
        g_driverInfo.curr = driverCurr;

        g_systemCfgBmp.bDriverCurr = true;
        g_bCmdPostSemToFunc = true;
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverCurrentQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverCurrentQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_NEWDRIVER, DRIVERCMD_CURRENTQ, sizeof(g_driverInfo.curr), &g_driverInfo.curr);
}


/*********************************************************************************************
函 数 名: cmdDriverMicroStepsSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverMicroStepsSet(u8 cmdDataLen, u8 *pCmdData)
{
    MicroStepEnum microSteps;
    u8 i;
    
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverMicroStepsVerify(cmdDataLen, pCmdData, (void *)&microSteps))
    {
        g_driverInfo.microStep = microSteps;

        for (i = 0;i < CH_TOTAL;i++)
        {
            //更新下转换系数
            servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                  g_driverInfo.microStep,
                                  &g_systemState.posnConvertInfo[i]);
        }
        
        g_systemCfgBmp.bDriverMode = true;
        g_bCmdPostSemToFunc = true;

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverMicroStepsQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverMicroStepsQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_NEWDRIVER, DRIVERCMD_MICROSTEPSQ, sizeof(g_driverInfo.microStep), (u8 *)&g_driverInfo.microStep);
}


/*********************************************************************************************
函 数 名: cmdDriverStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverStateSet(u8 cmdDataLen, u8 *pCmdData)
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

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfDriverStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_driverInfo.state[i] = state;
        
                g_chanCfgBmp[i].bDriverState = true;
                g_bCmdPostSemToFunc = true;

                /*if (SENSOR_ON == state)
                {
                    servStimerAdd(&g_driverFaultTimer);
                }*/
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdDriverStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_driverInfo.state[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_driverInfo.state[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_driverInfo.state[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_NEWDRIVER, DRIVERCMD_STATEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_driverInfo.state[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_driverInfo.state[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_NEWDRIVER, DRIVERCMD_STATEQ, dataLen, data);
            }
        }
    }
}

/*********************************************************************************************
函 数 名: cmdDriverCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void cmdDriverCmdInit(void)
{
    memset(pDriverCmdFunc, 0, sizeof(pDriverCmdFunc));

    pDriverCmdFunc[DRIVERCMD_TYPEQ]     = cmdDriverTypeQuery;
    pDriverCmdFunc[DRIVERCMD_CURRENT]   = cmdDriverCurrentSet;
    pDriverCmdFunc[DRIVERCMD_CURRENTQ]  = cmdDriverCurrentQuery;  
    
    pDriverCmdFunc[DRIVERCMD_MICROSTEPS]  = cmdDriverMicroStepsSet;
    pDriverCmdFunc[DRIVERCMD_MICROSTEPSQ] = cmdDriverMicroStepsQuery; 
    pDriverCmdFunc[DRIVERCMD_STATE]       = cmdDriverStateSet;
    pDriverCmdFunc[DRIVERCMD_STATEQ]      = cmdDriverStateQuery;
}

            
/*********************************************************************************************
函 数 名: cmdDriverCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
#endif



/*******************************************文件尾********************************************/
