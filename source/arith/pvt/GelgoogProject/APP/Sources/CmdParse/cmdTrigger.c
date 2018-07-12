/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdTrigger.c；
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
#include "cmdTrigger.h"
#include "pvrfTrigger.h"
#include "cmdMainParse.h"
#include "servSoftTimer.h"
#include "servCommIntfc.h"
#include "servSystemPara.h"



/****************************************外部变量声明*****************************************/
extern SystemInfoStruct   g_systemInfo;
extern SystemStateStruct  g_systemState;
extern MotorInfoStruct    g_motorInfo;
extern TrigInInfoStruct   g_trigInInfo;
extern DigitalOutStruct   g_digitalOut;
extern IsolatorOutStruct  g_isolatorOut;
extern SystemCfgBmpStruct g_systemCfgBmp;
extern ChanCfgBmpStruct   g_chanCfgBmp[CH_TOTAL];

#ifdef PROJECT_GELGOOG
extern IsolatorInStruct g_isolatorIn;
extern SoftTimerStruct  g_isolatorInTimer;
#endif

extern SensorUartStruct  g_sensorUart;

extern bool g_bCmdPostSemToFunc;
extern bool g_bPerdPostSemToFunc;
extern bool g_bPerdPostSemToCmd;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pTrigInCmdFunc[TRIGINCMD_RESERVE];
SubCmdProFunc pIsolInCmdFunc[ISOLINCMD_RESERVE];

SubCmdProFunc pTrigOutCmdFunc[TRIGOUTCMD_RESERVE];
SubCmdProFunc pIsolOutCmdFunc[ISOLOUTCMD_RESERVE];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdTrigInModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    TriggerModeEnum trigMode;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInModeVerify(cmdDataLen, pCmdData, (void *)&trigMode))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].trigMode = trigMode;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }

        //存到EEPROM中
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_trigInInfo.trigIn[CH1].trigMode) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_trigInInfo.trigIn[chanNum].trigMode;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].trigMode);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_MODEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_trigInInfo.trigIn[i].trigMode;
                data[0] = i;
                for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].trigMode);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TRIGIN, TRIGINCMD_MODEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum pattState;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattStateVerify(cmdDataLen, pCmdData, (void *)&pattState))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].pattState = pattState;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_trigInInfo.trigIn[CH1].pattState) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_trigInInfo.trigIn[chanNum].pattState;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].pattState);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_MODEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_trigInInfo.trigIn[i].pattState;
                data[0] = i;
                for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].pattState);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTSTATEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    TrigInTypeEnum type[TRIGPIN_RESERVE];
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattTypeVerify(cmdDataLen, pCmdData, (void *)&type))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                memcpy(g_trigInInfo.trigIn[i].type, type, cmdDataLen);

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_trigInInfo.trigIn[CH1].type) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_trigInInfo.trigIn[chanNum].type;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].type);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTERNQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_trigInInfo.trigIn[i].type;
                data[0] = i;
                for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].type);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTERNQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInResponseSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum pattResponse;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattResponseVerify(cmdDataLen, pCmdData, (void *)&pattResponse))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].pattResponse = pattResponse;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattResponseQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_trigInInfo.trigIn[CH1].pattResponse) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_trigInInfo.trigIn[chanNum].pattResponse;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].pattResponse);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTRESPQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_trigInInfo.trigIn[i].pattResponse;
                data[0] = i;
                for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].pattResponse);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTRESPQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattSModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattSModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    SamplingModeEnum  pattSMode;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattSModeVerify(cmdDataLen, pCmdData, (void *)&pattSMode))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].pattSMode = pattSMode;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattSModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattSModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_trigInInfo.trigIn[CH1].pattSMode) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_trigInInfo.trigIn[chanNum].pattSMode;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].pattSMode);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTSMODEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_trigInInfo.trigIn[i].pattSMode;
                data[0] = i;
                for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].pattSMode);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTSMODEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattSPeriodSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattSPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 pattSPeriod;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattSPeriodVerify(cmdDataLen, pCmdData, (void *)&pattSPeriod))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].pattSPeriod = pattSPeriod;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInPattSPeriodQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInPattSPeriodQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_trigInInfo.trigIn[CH1].pattSPeriod) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_trigInInfo.trigIn[chanNum].pattSPeriod;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].pattSPeriod);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTPERIODQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_trigInInfo.trigIn[i].pattSPeriod;
                data[0] = i;
                for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].pattSPeriod);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_TRIGIN, TRIGINCMD_PATTPERIODQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8   index;
    u8   i;
#ifdef PROJECT_GELGOOG
    u8 dataLen;
    u8 j;
    u8 data[6];
#endif
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].levelState[index] = state;

#ifdef PROJECT_GELGOOG
#if 1
#if 1
                //只有TrigR和U2_S2互斥
                if ((SENSOR_ON == state) && (CH4 == i) && (TRIGPIN_DIR == index) &&
                    (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state))
                {
                    g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state = SENSOR_OFF;

                    //通知上位机IO状态变化了
                    dataLen = sizeof(g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state) + sizeof(SenUartNumEnum) + sizeof(SensorNumEnum);
                    data[0] = UARTNUM_U2;    //UARTNUM_U2
                    data[1] = SENSOR_S2;     //SENSOR_S2
                    data[2] = g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state;
                    cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STATEQ, dataLen, data);
            
                    servSensor2UartReciveOff();    //临时在这里设置，后续改成在FUNC线程中关闭    NICK MARK

                    servSensorUartInfoStore(&g_sensorUart);
                }
#else
                //互斥修改为CH4编码器和UartSensor2互斥
                if ((SENSOR_ON == state) && (CH4 == i))
                {
                    for (j = 0;j < SENSOR_RESERVE;j++)
                    {
                        if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][j].state)
                        {
                            g_sensorUart.sensor[UARTNUM_U2][j].state = SENSOR_OFF;

                            //通知上位机IO状态变化了
                            dataLen = sizeof(g_sensorUart.sensor[UARTNUM_U2][j].state) + sizeof(SenUartNumEnum) + sizeof(SensorNumEnum);
                            data[0] = UARTNUM_U2;    //UARTNUM_U2
                            data[1] = j;             //SENSOR_S1
                            data[2] = g_sensorUart.sensor[UARTNUM_U2][j].state;
                            cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STATEQ, dataLen, data);
                    
                            servSensor2UartReciveOff();    //临时在这里设置，后续改成在FUNC线程中关闭    NICK MARK
                        }
                    }

                    servSensorUartInfoStore(&g_sensorUart);
                }
#endif
#else

                //开启的是DIR，且编码器为三通道编码器并处于开启状态则关闭编码器
                if ((INTFC_ON == g_motorInfo.motor[i].encoderState) && 
                    (ECCHAN_3 == g_motorInfo.motor[i].encoderChanNum) &&
                    ((TRIGPIN_DIR == index) && (SENSOR_ON == state)))
                {
                    g_motorInfo.motor[i].encoderState = INTFC_OFF;
                    servMotorInfoWrite(&g_motorInfo);
                    
                    //通知上位机编码器状态变化了
                    dataLen = sizeof(g_motorInfo.motor[i].encoderState) + sizeof(chanNum);
                    pData = (u8 *)&g_motorInfo.motor[chanNum].encoderState;
                    data[0] = chanNum;
                    for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].encoderState);i++)
                    {
                        data[1 + i] = *pData++;
                    }
                    cmdFrameSend(CMD_ENCODER, ENCODERCMD_STATEQ, dataLen, data);

                    g_chanCfgBmp[i].bEncoder = true;
                }
                
#endif
#endif

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    u8 index   = *pCmdData;


    if (((sizeof(chanNum) + sizeof(index)) == cmdDataLen) && (index < TRIGPIN_RESERVE))
    {
        dataLen = sizeof(g_trigInInfo.trigIn[CH1].levelState[index]) + sizeof(chanNum) + sizeof(index);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_trigInInfo.trigIn[chanNum].levelState[index];
            data[0] = chanNum;
            data[1] = index;
            for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].levelState[index]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELSTATEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_trigInInfo.trigIn[i].levelState[index];
                    data[0] = i;
                    data[1] = index;
                    for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].levelState[index]);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELSTATEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    TrigInTypeEnum type;
    u8 index;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelTypeVerify(cmdDataLen, pCmdData, (void *)&type, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].type[index] = type;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 data[6];
    u8 i;
    u8 chanNum = *pCmdData++;
    u8 index   = *pCmdData;


    if (((sizeof(chanNum) + sizeof(index)) == cmdDataLen) && (index < TRIGPIN_RESERVE))
    {
        dataLen = sizeof(g_trigInInfo.trigIn[CH1].type[index]) + sizeof(chanNum) + sizeof(index);
        if (chanNum <= CH_MAX)
        {
            data[0] = chanNum;
            data[1] = index;
            data[2] = g_trigInInfo.trigIn[chanNum].type[index];
            cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELTYPEQ, dataLen, data);
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
                    data[1] = index;
                    data[2] = g_trigInInfo.trigIn[i].type[index];
                    cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELTYPEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelResponseSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;
    u8   index;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelResponseVerify(cmdDataLen, pCmdData, (void *)&response, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].levelResponse[index] = response;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelResponseQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    u8 index   = *pCmdData;


    if (((sizeof(chanNum) + sizeof(index)) == cmdDataLen) && (index < TRIGPIN_RESERVE))
    {
        dataLen = sizeof(g_trigInInfo.trigIn[CH1].levelResponse[index]) + sizeof(chanNum) + sizeof(index);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_trigInInfo.trigIn[chanNum].levelResponse[index];
            data[0] = chanNum;
            data[1] = index;
            for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].levelResponse[index]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELRESPQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_trigInInfo.trigIn[i].levelResponse[index];
                    data[0] = i;
                    data[1] = index;
                    for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].levelResponse[index]);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELRESPQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelSModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelSModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    SamplingModeEnum  levelSMode;
    u8   index;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelSModeVerify(cmdDataLen, pCmdData, (void *)&levelSMode, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].levelSMode[index] = levelSMode;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelSModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelSModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    u8 index   = *pCmdData;


    if (((sizeof(chanNum) + sizeof(index)) == cmdDataLen) && (index < TRIGPIN_RESERVE))
    {
        dataLen = sizeof(g_trigInInfo.trigIn[CH1].levelSMode[index]) + sizeof(chanNum) + sizeof(index);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_trigInInfo.trigIn[chanNum].levelSMode[index];
            data[0] = chanNum;
            data[1] = index;
            for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].levelSMode[index]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELSMODEQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_trigInInfo.trigIn[i].levelSMode[index];
                    data[0] = i;
                    data[1] = index;
                    for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].levelSMode[index]);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELSMODEQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelSPeriodSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelSPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 levelSPeriod;
    u8  index;
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
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelSPeriodVerify(cmdDataLen, pCmdData, (void *)&levelSPeriod, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].levelSPeriod[index] = levelSPeriod;

                //给FUNC发信号量
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInLevelSPeriodQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInLevelSPeriodQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;
    u8 index   = *pCmdData;


    if (((sizeof(chanNum) + sizeof(index)) == cmdDataLen) && (index < TRIGPIN_RESERVE))
    {
        dataLen = sizeof(g_trigInInfo.trigIn[CH1].levelSPeriod[index]) + sizeof(chanNum) + sizeof(index);
        if (chanNum <= CH_MAX)
        {
            pData = (u8 *)&g_trigInInfo.trigIn[chanNum].levelSPeriod[index];
            data[0] = chanNum;
            data[1] = index;
            for (i = 0;i < sizeof(g_trigInInfo.trigIn[chanNum].levelSPeriod[index]);i++)
            {
                data[2 + i] = *pData++;
            }
            cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELPERIODQ, dataLen, data);
        }
        else
        {
            for (i = 0;i < g_systemState.chanNum;i++)
            {
                if ((CH_ALL == chanNum) ||
                    (chanNum == g_systemInfo.group[i][0]) ||
                    (chanNum == g_systemInfo.group[i][1]))
                {
                    pData = (u8 *)&g_trigInInfo.trigIn[i].levelSPeriod[index];
                    data[0] = i;
                    data[1] = index;
                    for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].levelSPeriod[index]);j++)
                    {
                        data[2 + j] = *pData++;
                    }
                    cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELPERIODQ, dataLen, data);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdTrigInCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInCmdInit(void)
{
    memset(pTrigInCmdFunc, 0, sizeof(pTrigInCmdFunc));

    pTrigInCmdFunc[TRIGINCMD_MODE]  = cmdTrigInModeSet;
    pTrigInCmdFunc[TRIGINCMD_MODEQ] = cmdTrigInModeQuery;

    pTrigInCmdFunc[TRIGINCMD_PATTSTATE]  = cmdTrigInPattStateSet;
    pTrigInCmdFunc[TRIGINCMD_PATTSTATEQ] = cmdTrigInPattStateQuery;
    pTrigInCmdFunc[TRIGINCMD_PATTERN]    = cmdTrigInPattTypeSet;
    pTrigInCmdFunc[TRIGINCMD_PATTERNQ]   = cmdTrigInPattTypeQuery;
    pTrigInCmdFunc[TRIGINCMD_PATTRESP]   = cmdTrigInPattResponseSet;
    pTrigInCmdFunc[TRIGINCMD_PATTRESPQ]  = cmdTrigInPattResponseQuery;
    pTrigInCmdFunc[TRIGINCMD_PATTSMODE] = cmdTrigInPattSModeSet;
    pTrigInCmdFunc[TRIGINCMD_PATTSMODEQ]= cmdTrigInPattSModeQuery;
    pTrigInCmdFunc[TRIGINCMD_PATTPERIOD] = cmdTrigInPattSPeriodSet;
    pTrigInCmdFunc[TRIGINCMD_PATTPERIODQ]= cmdTrigInPattSPeriodQuery;

    pTrigInCmdFunc[TRIGINCMD_LEVELSTATE]  = cmdTrigInLevelStateSet;
    pTrigInCmdFunc[TRIGINCMD_LEVELSTATEQ] = cmdTrigInLevelStateQuery;
    pTrigInCmdFunc[TRIGINCMD_LEVELTYPE]   = cmdTrigInLevelTypeSet;
    pTrigInCmdFunc[TRIGINCMD_LEVELTYPEQ]  = cmdTrigInLevelTypeQuery;
    pTrigInCmdFunc[TRIGINCMD_LEVELRESP]   = cmdTrigInLevelResponseSet;
    pTrigInCmdFunc[TRIGINCMD_LEVELRESPQ]  = cmdTrigInLevelResponseQuery;
    pTrigInCmdFunc[TRIGINCMD_LEVELSMODE] = cmdTrigInLevelSModeSet;
    pTrigInCmdFunc[TRIGINCMD_LEVELSMODEQ]= cmdTrigInLevelSModeQuery;
    pTrigInCmdFunc[TRIGINCMD_LEVELPERIOD] = cmdTrigInLevelSPeriodSet;
    pTrigInCmdFunc[TRIGINCMD_LEVELPERIODQ]= cmdTrigInLevelSPeriodQuery;
}

            
/*********************************************************************************************
函 数 名: cmdTrigInCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigInCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < TRIGINCMD_RESERVE) && (pTrigInCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pTrigInCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


#ifdef PROJECT_GELGOOG
/*********************************************************************************************
函 数 名: cmdIsolInStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    IntfcStateEnum state;
    

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInStateVerify(cmdDataLen, pCmdData, (void *)&state))
    {
        g_isolatorIn.state = state;
        
        servIsolatorInInfoStore(&g_isolatorIn);

        if (INTFC_ON == state)
        {
            servStimerAdd(&g_isolatorInTimer);
        }
        else
        {
            servStimerDelete(&g_isolatorInTimer);
        }
    }
}


/*********************************************************************************************
函 数 名: cmdIsolInStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.state);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_STATEQ, dataLen, (u8 *)&g_isolatorIn.state);
}


/*********************************************************************************************
函 数 名: cmdIsolInTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    TrigInTypeEnum type;
    

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInTypeVerify(cmdDataLen, pCmdData, (void *)&type))
    {
        g_isolatorIn.type = type;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolInTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.type);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_TYPEQ, dataLen, (u8 *)&g_isolatorIn.type);
}


/*********************************************************************************************
函 数 名: cmdIsolInResponseSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;
    

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_isolatorIn.response = response;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolInResponseQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.response);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_RESPQ, dataLen, (u8 *)&g_isolatorIn.response);
}


/*********************************************************************************************
函 数 名: cmdIsolInRespChanSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInRespChanSet(u8 cmdDataLen, u8 *pCmdData)
{
    ChannelEnum respChan;
    

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInRespChanVerify(cmdDataLen, pCmdData, (void *)&respChan))
    {
        g_isolatorIn.respChan = respChan;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolInRespChanQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInRespChanQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.respChan);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_RESPCHQ, dataLen, (u8 *)&g_isolatorIn.respChan);
}


/*********************************************************************************************
函 数 名: cmdIsolInSModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInSModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    SamplingModeEnum sMode;
    

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInSModeVerify(cmdDataLen, pCmdData, (void *)&sMode))
    {
        g_isolatorIn.sMode = sMode;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolInSModeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInSModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.sMode);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_SMODEQ, dataLen, (u8 *)&g_isolatorIn.sMode);
}


/*********************************************************************************************
函 数 名: cmdIsolInSPeriodSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInSPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 sPeriod;
    

    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInSPeriodVerify(cmdDataLen, pCmdData, (void *)&sPeriod))
    {
        g_isolatorIn.sPeriod = sPeriod;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolInSPeriodQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInSPeriodQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.sPeriod);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_SPERIODQ, dataLen, (u8 *)&g_isolatorIn.sPeriod);
}


/*********************************************************************************************
函 数 名: cmdIsolInCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInCmdInit(void)
{
    memset(pIsolInCmdFunc, 0, sizeof(pIsolInCmdFunc));

    pIsolInCmdFunc[ISOLINCMD_STATE]    = cmdIsolInStateSet;
    pIsolInCmdFunc[ISOLINCMD_STATEQ]   = cmdIsolInStateQuery;
    pIsolInCmdFunc[ISOLINCMD_TYPE]     = cmdIsolInTypeSet;
    pIsolInCmdFunc[ISOLINCMD_TYPEQ]    = cmdIsolInTypeQuery;
    pIsolInCmdFunc[ISOLINCMD_RESP]     = cmdIsolInResponseSet;
    pIsolInCmdFunc[ISOLINCMD_RESPQ]    = cmdIsolInResponseQuery;
    pIsolInCmdFunc[ISOLINCMD_RESPCH]   = cmdIsolInRespChanSet;
    pIsolInCmdFunc[ISOLINCMD_RESPCHQ]  = cmdIsolInRespChanQuery;
    pIsolInCmdFunc[ISOLINCMD_SMODE]    = cmdIsolInSModeSet;
    pIsolInCmdFunc[ISOLINCMD_SMODEQ]   = cmdIsolInSModeQuery;
    pIsolInCmdFunc[ISOLINCMD_SPERIOD]  = cmdIsolInSPeriodSet;
    pIsolInCmdFunc[ISOLINCMD_SPERIODQ] = cmdIsolInSPeriodQuery;
}

            
/*********************************************************************************************
函 数 名: cmdIsolInCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolInCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < ISOLINCMD_RESERVE) && (pIsolInCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pIsolInCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}
#endif


/*********************************************************************************************
函 数 名: cmdTrigOutStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8 index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        g_digitalOut.output[index].state = state;

        //给FUNC发信号量
        switch ((DoutNumEnum)index)
        {
            case DIO_DO1:
                g_systemCfgBmp.bDigitalOut1 = true;
              break;
              
            case DIO_DO2:
                g_systemCfgBmp.bDigitalOut2 = true;
              break;

#ifdef PROJECT_GELGOOG              
            case DIO_DO3:
                g_systemCfgBmp.bDigitalIO3 = true;
              break;
              
            case DIO_DO4:
                g_systemCfgBmp.bDigitalIO4 = true;
              break;
#endif
            default:
              break;
        }
        g_bCmdPostSemToFunc = true;

        servTrigOutInfoWrite(&g_digitalOut);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIO_RESERVE)
    {
        dataLen = sizeof(g_digitalOut.output[index].state) + sizeof(index);
    
        pData = (u8 *)&g_digitalOut.output[index].state;
        data[0] = index;
        for (i = 0;i < sizeof(g_digitalOut.output[index].state);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGOUT, TRIGOUTCMD_STATEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutSourceSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutSourceSet(u8 cmdDataLen, u8 *pCmdData)
{
    ChannelEnum source;
    u8 index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutSourceVerify(cmdDataLen, pCmdData, (void *)&source, &index))
    {
        g_digitalOut.output[index].source = source;

        //给FUNC发信号量
        switch ((DoutNumEnum)index)
        {
            case DIO_DO1:
                g_systemCfgBmp.bDigitalOut1 = true;
              break;
              
            case DIO_DO2:
                g_systemCfgBmp.bDigitalOut2 = true;
              break;

#ifdef PROJECT_GELGOOG              
            case DIO_DO3:
                g_systemCfgBmp.bDigitalIO3 = true;
              break;
              
            case DIO_DO4:
                g_systemCfgBmp.bDigitalIO4 = true;
              break;
#endif
            default:
              break;
        }
        g_bCmdPostSemToFunc = true;

        servIsolatorOutInfoStore(&g_isolatorOut);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutSourceQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutSourceQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIO_RESERVE)
    {
        dataLen = sizeof(g_digitalOut.output[index].source) + sizeof(index);
    
        pData = (u8 *)&g_digitalOut.output[index].source;
        data[0] = index;
        for (i = 0;i < sizeof(g_digitalOut.output[index].source);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGOUT, TRIGOUTCMD_SOURCEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutConditionSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutConditionSet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOCondtEnum condition;
    u8   index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutConditionVerify(cmdDataLen, pCmdData, (void *)&condition, &index))
    {
        g_digitalOut.output[index].condition = condition;

        //给FUNC发信号量
        switch ((DoutNumEnum)index)
        {
            case DIO_DO1:
                g_systemCfgBmp.bDigitalOut1 = true;
              break;
              
            case DIO_DO2:
                g_systemCfgBmp.bDigitalOut2 = true;
              break;

#ifdef PROJECT_GELGOOG              
            case DIO_DO3:
                g_systemCfgBmp.bDigitalIO3 = true;
              break;
              
            case DIO_DO4:
                g_systemCfgBmp.bDigitalIO4 = true;
              break;
#endif
            default:
              break;
        }

        g_bCmdPostSemToFunc = true;

        servTrigOutInfoWrite(&g_digitalOut);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutConditionQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutConditionQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;


    if (index < DIO_RESERVE)
    {
        dataLen = sizeof(g_digitalOut.output[index].condition) + sizeof(index);

        pData = (u8 *)&g_digitalOut.output[index].condition;
        data[0] = index;
        for (i = 0;i < sizeof(g_digitalOut.output[index].condition);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGOUT, TRIGOUTCMD_CONDITIONQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutSignalSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutSignalSet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOSignalEnum signal;
    u8   index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutSignalVerify(cmdDataLen, pCmdData, (void *)&signal, &index))
    {
        g_digitalOut.output[index].signal = signal;

        //给FUNC发信号量
        switch ((DoutNumEnum)index)
        {
            case DIO_DO1:
                g_systemCfgBmp.bDigitalOut1 = true;
              break;
              
            case DIO_DO2:
                g_systemCfgBmp.bDigitalOut2 = true;
              break;

#ifdef PROJECT_GELGOOG              
            case DIO_DO3:
                g_systemCfgBmp.bDigitalIO3 = true;
              break;
              
            case DIO_DO4:
                g_systemCfgBmp.bDigitalIO4 = true;
              break;
#endif
            default:
              break;
        }

        g_bCmdPostSemToFunc = true;

        servTrigOutInfoWrite(&g_digitalOut);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutSignalQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutSignalQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIO_RESERVE)
    {
        dataLen = sizeof(g_digitalOut.output[index].signal) + sizeof(index);
        
        pData = (u8 *)&g_digitalOut.output[index].signal;
        data[0] = index;
        for (i = 0;i < sizeof(g_digitalOut.output[index].signal);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGOUT, TRIGOUTCMD_SIGNALQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutPolaritySet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutPolaritySet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOPolarityEnum polarity;
    u8   index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutPolarityVerify(cmdDataLen, pCmdData, (void *)&polarity, &index))
    {
        g_digitalOut.output[index].polarity = polarity;

        //给FUNC发信号量
        switch ((DoutNumEnum)index)
        {
            case DIO_DO1:
                g_systemCfgBmp.bDigitalOut1 = true;
              break;
              
            case DIO_DO2:
                g_systemCfgBmp.bDigitalOut2 = true;
              break;

#ifdef PROJECT_GELGOOG              
            case DIO_DO3:
                g_systemCfgBmp.bDigitalIO3 = true;
              break;
              
            case DIO_DO4:
                g_systemCfgBmp.bDigitalIO4 = true;
              break;
#endif
            default:
              break;
        }

        g_bCmdPostSemToFunc = true;

        servTrigOutInfoWrite(&g_digitalOut);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutPolarityQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutPolarityQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIO_RESERVE)
    {
        dataLen = sizeof(g_digitalOut.output[index].polarity) + sizeof(index);
        
        pData = (u8 *)&g_digitalOut.output[index].polarity;
        data[0] = index;
        for (i = 0;i < sizeof(g_digitalOut.output[index].polarity);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGOUT, TRIGOUTCMD_POLARITYQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutPeriodSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32  period;
    u8   index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutPeriodVerify(cmdDataLen, pCmdData, (void *)&period, &index))
    {
        g_digitalOut.output[index].period = period;

        //给FUNC发信号量
        switch ((DoutNumEnum)index)
        {
            case DIO_DO1:
                g_systemCfgBmp.bDigitalOut1 = true;
              break;
              
            case DIO_DO2:
                g_systemCfgBmp.bDigitalOut2 = true;
              break;

#ifdef PROJECT_GELGOOG              
            case DIO_DO3:
                g_systemCfgBmp.bDigitalIO3 = true;
              break;
              
            case DIO_DO4:
                g_systemCfgBmp.bDigitalIO4 = true;
              break;
#endif
            default:
              break;
        }

        g_bCmdPostSemToFunc = true;

        servTrigOutInfoWrite(&g_digitalOut);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutPeriodQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutPeriodQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIO_RESERVE)
    {
        dataLen = sizeof(g_digitalOut.output[index].period) + sizeof(index);
        
        pData = (u8 *)&g_digitalOut.output[index].period;
        data[0] = index;
        for (i = 0;i < sizeof(g_digitalOut.output[index].period);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGOUT, TRIGOUTCMD_PERIODQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutDutySet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutDutySet(u8 cmdDataLen, u8 *pCmdData)
{
    u16  duty;
    u8   index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutDutyVerify(cmdDataLen, pCmdData, (void *)&duty, &index))
    {
        g_digitalOut.output[index].duty = duty;

        //给FUNC发信号量
        switch ((DoutNumEnum)index)
        {
            case DIO_DO1:
                g_systemCfgBmp.bDigitalOut1 = true;
              break;
              
            case DIO_DO2:
                g_systemCfgBmp.bDigitalOut2 = true;
              break;

#ifdef PROJECT_GELGOOG              
            case DIO_DO3:
                g_systemCfgBmp.bDigitalIO3 = true;
              break;
              
            case DIO_DO4:
                g_systemCfgBmp.bDigitalIO4 = true;
              break;
#endif
            default:
              break;
        }

        g_bCmdPostSemToFunc = true;

        servTrigOutInfoWrite(&g_digitalOut);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutDutyQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutDutyQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < DIO_RESERVE)
    {
        dataLen = sizeof(g_digitalOut.output[index].duty) + sizeof(index);
        
        pData = (u8 *)&g_digitalOut.output[index].duty;
        data[0] = index;
        for (i = 0;i < sizeof(g_digitalOut.output[index].duty);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_TRIGOUT, TRIGOUTCMD_DUTYQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdTrigOutCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutCmdInit(void)
{
    memset(pTrigOutCmdFunc, 0, sizeof(pTrigOutCmdFunc));

    pTrigOutCmdFunc[TRIGOUTCMD_STATE]     = cmdTrigOutStateSet;
    pTrigOutCmdFunc[TRIGOUTCMD_STATEQ]    = cmdTrigOutStateQuery;
    pTrigOutCmdFunc[TRIGOUTCMD_SIGNAL]    = cmdTrigOutSignalSet;
    pTrigOutCmdFunc[TRIGOUTCMD_SIGNALQ]   = cmdTrigOutSignalQuery;
    pTrigOutCmdFunc[TRIGOUTCMD_POLARITY]  = cmdTrigOutPolaritySet;
    pTrigOutCmdFunc[TRIGOUTCMD_POLARITYQ] = cmdTrigOutPolarityQuery;
    pTrigOutCmdFunc[TRIGOUTCMD_SOURCE]    = cmdTrigOutSourceSet;
    pTrigOutCmdFunc[TRIGOUTCMD_SOURCEQ]   = cmdTrigOutSourceQuery;
    pTrigOutCmdFunc[TRIGOUTCMD_CONDITION] = cmdTrigOutConditionSet;
    pTrigOutCmdFunc[TRIGOUTCMD_CONDITIONQ]= cmdTrigOutConditionQuery;   
    pTrigOutCmdFunc[TRIGOUTCMD_PERIOD]    = cmdTrigOutPeriodSet;
    pTrigOutCmdFunc[TRIGOUTCMD_PERIODQ]   = cmdTrigOutPeriodQuery;
    pTrigOutCmdFunc[TRIGOUTCMD_DUTY]      = cmdTrigOutDutySet;
    pTrigOutCmdFunc[TRIGOUTCMD_DUTYQ]     = cmdTrigOutDutyQuery;
}

            
/*********************************************************************************************
函 数 名: cmdTrigOutCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdTrigOutCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < TRIGOUTCMD_RESERVE) && (pTrigOutCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pTrigOutCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8 index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorOutStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        g_isolatorOut.output[index].state = state;

        //给FUNC发信号量
        switch ((YoutNumEnum)index)
        {
            case YOUT_YO1:
                g_systemCfgBmp.bIsolatorOut1 = true;
              break;
              
            case YOUT_YO2:
                g_systemCfgBmp.bIsolatorOut2 = true;
              break;
              
            default:
              break;
        }
        g_bCmdPostSemToFunc = true;

        servIsolatorOutInfoStore(&g_isolatorOut);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < YOUT_RESERVE)
    {
        dataLen = sizeof(g_isolatorOut.output[index].state) + sizeof(index);
    
        pData = (u8 *)&g_isolatorOut.output[index].state;
        data[0] = index;
        for (i = 0;i < sizeof(g_isolatorOut.output[index].state);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ISOLOUT, ISOLOUTCMD_STATEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorOutSourceSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutSourceSet(u8 cmdDataLen, u8 *pCmdData)
{
    ChannelEnum source;
    u8 index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorOutSourceVerify(cmdDataLen, pCmdData, (void *)&source, &index))
    {
        g_isolatorOut.output[index].source = source;

        //给FUNC发信号量
        switch ((YoutNumEnum)index)
        {
            case YOUT_YO1:
                g_systemCfgBmp.bIsolatorOut1 = true;
              break;
              
            case YOUT_YO2:
                g_systemCfgBmp.bIsolatorOut2 = true;
              break;
              
            default:
              break;
        }
        g_bCmdPostSemToFunc = true;

        servIsolatorOutInfoStore(&g_isolatorOut);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorOutSourceQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutSourceQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < YOUT_RESERVE)
    {
        dataLen = sizeof(g_isolatorOut.output[index].source) + sizeof(index);
    
        pData = (u8 *)&g_isolatorOut.output[index].source;
        data[0] = index;
        for (i = 0;i < sizeof(g_isolatorOut.output[index].source);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ISOLOUT, ISOLOUTCMD_SOURCEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorOutConditionSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutConditionSet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOCondtEnum condition;
    u8   index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorOutConditionVerify(cmdDataLen, pCmdData, (void *)&condition, &index))
    {
        g_isolatorOut.output[index].condition = condition;

        //给FUNC发信号量
        switch ((YoutNumEnum)index)
        {
            case YOUT_YO1:
                g_systemCfgBmp.bIsolatorOut1 = true;
              break;
              
            case YOUT_YO2:
                g_systemCfgBmp.bIsolatorOut2 = true;
              break;
              
            default:
              break;
        }

        g_bCmdPostSemToFunc = true;

        servIsolatorOutInfoStore(&g_isolatorOut);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorOutConditionQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutConditionQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;


    if (index < YOUT_RESERVE)
    {
        dataLen = sizeof(g_isolatorOut.output[index].condition) + sizeof(index);

        pData = (u8 *)&g_isolatorOut.output[index].condition;
        data[0] = index;
        for (i = 0;i < sizeof(g_isolatorOut.output[index].condition);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ISOLOUT, ISOLOUTCMD_CONDITIONQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorOutResponseSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOPolarityEnum response;
    u8   index;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorOutResponseVerify(cmdDataLen, pCmdData, (void *)&response, &index))
    {
        g_isolatorOut.output[index].response = response;

        //给FUNC发信号量
        switch ((YoutNumEnum)index)
        {
            case YOUT_YO1:
                g_systemCfgBmp.bIsolatorOut1 = true;
              break;
              
            case YOUT_YO2:
                g_systemCfgBmp.bIsolatorOut2 = true;
              break;
              
            default:
              break;
        }

        g_bCmdPostSemToFunc = true;

        servIsolatorOutInfoStore(&g_isolatorOut);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorOutResponseQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i;
    u8 index = *pCmdData;

    
    if (index < YOUT_RESERVE)
    {
        dataLen = sizeof(g_isolatorOut.output[index].response) + sizeof(index);
        
        pData = (u8 *)&g_isolatorOut.output[index].response;
        data[0] = index;
        for (i = 0;i < sizeof(g_isolatorOut.output[index].response);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ISOLOUT, ISOLOUTCMD_RESPONSEQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdIsolatorOutCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutCmdInit(void)
{
    memset(pIsolOutCmdFunc, 0, sizeof(pIsolOutCmdFunc));

    pIsolOutCmdFunc[ISOLOUTCMD_STATE]     = cmdIsolatorOutStateSet;
    pIsolOutCmdFunc[ISOLOUTCMD_STATEQ]    = cmdIsolatorOutStateQuery;
    pIsolOutCmdFunc[ISOLOUTCMD_SOURCE]    = cmdIsolatorOutSourceSet;
    pIsolOutCmdFunc[ISOLOUTCMD_SOURCEQ]   = cmdIsolatorOutSourceQuery;
    pIsolOutCmdFunc[ISOLOUTCMD_CONDITION] = cmdIsolatorOutConditionSet;
    pIsolOutCmdFunc[ISOLOUTCMD_CONDITIONQ]= cmdIsolatorOutConditionQuery;
    pIsolOutCmdFunc[ISOLOUTCMD_RESPONSE]  = cmdIsolatorOutResponseSet;
    pIsolOutCmdFunc[ISOLOUTCMD_RESPONSEQ] = cmdIsolatorOutResponseQuery;
}

            
/*********************************************************************************************
函 数 名: cmdIsolatorOutCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdIsolatorOutCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < ISOLOUTCMD_RESERVE) && (pIsolOutCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pIsolOutCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************文件尾********************************************/
