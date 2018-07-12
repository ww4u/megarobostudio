/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdTrigger.c��
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
#include "cmdTrigger.h"
#include "pvrfTrigger.h"
#include "cmdMainParse.h"
#include "servSoftTimer.h"
#include "servCommIntfc.h"
#include "servSystemPara.h"



/****************************************�ⲿ��������*****************************************/
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



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pTrigInCmdFunc[TRIGINCMD_RESERVE];
SubCmdProFunc pIsolInCmdFunc[ISOLINCMD_RESERVE];

SubCmdProFunc pTrigOutCmdFunc[TRIGOUTCMD_RESERVE];
SubCmdProFunc pIsolOutCmdFunc[ISOLOUTCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdTrigInModeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInModeVerify(cmdDataLen, pCmdData, (void *)&trigMode))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].trigMode = trigMode;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }

        //�浽EEPROM��
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInModeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInPattStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattStateVerify(cmdDataLen, pCmdData, (void *)&pattState))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].pattState = pattState;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInPattStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInPattTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattTypeVerify(cmdDataLen, pCmdData, (void *)&type))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                memcpy(g_trigInInfo.trigIn[i].type, type, cmdDataLen);

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInPattTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInResponseSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattResponseVerify(cmdDataLen, pCmdData, (void *)&pattResponse))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].pattResponse = pattResponse;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInPattResponseQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInPattSModeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattSModeVerify(cmdDataLen, pCmdData, (void *)&pattSMode))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].pattSMode = pattSMode;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInPattSModeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInPattSPeriodSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInPattSPeriodVerify(cmdDataLen, pCmdData, (void *)&pattSPeriod))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].pattSPeriod = pattSPeriod;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInPattSPeriodQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInLevelStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
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
                //ֻ��TrigR��U2_S2����
                if ((SENSOR_ON == state) && (CH4 == i) && (TRIGPIN_DIR == index) &&
                    (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state))
                {
                    g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state = SENSOR_OFF;

                    //֪ͨ��λ��IO״̬�仯��
                    dataLen = sizeof(g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state) + sizeof(SenUartNumEnum) + sizeof(SensorNumEnum);
                    data[0] = UARTNUM_U2;    //UARTNUM_U2
                    data[1] = SENSOR_S2;     //SENSOR_S2
                    data[2] = g_sensorUart.sensor[UARTNUM_U2][SENSOR_S2].state;
                    cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STATEQ, dataLen, data);
            
                    servSensor2UartReciveOff();    //��ʱ���������ã������ĳ���FUNC�߳��йر�    NICK MARK

                    servSensorUartInfoStore(&g_sensorUart);
                }
#else
                //�����޸�ΪCH4��������UartSensor2����
                if ((SENSOR_ON == state) && (CH4 == i))
                {
                    for (j = 0;j < SENSOR_RESERVE;j++)
                    {
                        if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][j].state)
                        {
                            g_sensorUart.sensor[UARTNUM_U2][j].state = SENSOR_OFF;

                            //֪ͨ��λ��IO״̬�仯��
                            dataLen = sizeof(g_sensorUart.sensor[UARTNUM_U2][j].state) + sizeof(SenUartNumEnum) + sizeof(SensorNumEnum);
                            data[0] = UARTNUM_U2;    //UARTNUM_U2
                            data[1] = j;             //SENSOR_S1
                            data[2] = g_sensorUart.sensor[UARTNUM_U2][j].state;
                            cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STATEQ, dataLen, data);
                    
                            servSensor2UartReciveOff();    //��ʱ���������ã������ĳ���FUNC�߳��йر�    NICK MARK
                        }
                    }

                    servSensorUartInfoStore(&g_sensorUart);
                }
#endif
#else

                //��������DIR���ұ�����Ϊ��ͨ�������������ڿ���״̬��رձ�����
                if ((INTFC_ON == g_motorInfo.motor[i].encoderState) && 
                    (ECCHAN_3 == g_motorInfo.motor[i].encoderChanNum) &&
                    ((TRIGPIN_DIR == index) && (SENSOR_ON == state)))
                {
                    g_motorInfo.motor[i].encoderState = INTFC_OFF;
                    servMotorInfoWrite(&g_motorInfo);
                    
                    //֪ͨ��λ��������״̬�仯��
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

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInLevelStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInLevelTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelTypeVerify(cmdDataLen, pCmdData, (void *)&type, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].type[index] = type;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInLevelTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInLevelResponseSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelResponseVerify(cmdDataLen, pCmdData, (void *)&response, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].levelResponse[index] = response;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInLevelResponseQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInLevelSModeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelSModeVerify(cmdDataLen, pCmdData, (void *)&levelSMode, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].levelSMode[index] = levelSMode;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInLevelSModeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInLevelSPeriodSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigInLevelSPeriodVerify(cmdDataLen, pCmdData, (void *)&levelSPeriod, &index))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_trigInInfo.trigIn[i].levelSPeriod[index] = levelSPeriod;

                //��FUNC���ź���
                g_chanCfgBmp[i].bTrigIn = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servTrigInInfoWrite(&g_trigInInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdTrigInLevelSPeriodQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigInCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdIsolInStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    IntfcStateEnum state;
    

    //���в�����֤
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
�� �� ��: cmdIsolInStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.state);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_STATEQ, dataLen, (u8 *)&g_isolatorIn.state);
}


/*********************************************************************************************
�� �� ��: cmdIsolInTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    TrigInTypeEnum type;
    

    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInTypeVerify(cmdDataLen, pCmdData, (void *)&type))
    {
        g_isolatorIn.type = type;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
�� �� ��: cmdIsolInTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.type);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_TYPEQ, dataLen, (u8 *)&g_isolatorIn.type);
}


/*********************************************************************************************
�� �� ��: cmdIsolInResponseSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    ResponseTypeEnum response;
    

    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInResponseVerify(cmdDataLen, pCmdData, (void *)&response))
    {
        g_isolatorIn.response = response;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
�� �� ��: cmdIsolInResponseQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInResponseQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.response);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_RESPQ, dataLen, (u8 *)&g_isolatorIn.response);
}


/*********************************************************************************************
�� �� ��: cmdIsolInRespChanSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInRespChanSet(u8 cmdDataLen, u8 *pCmdData)
{
    ChannelEnum respChan;
    

    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInRespChanVerify(cmdDataLen, pCmdData, (void *)&respChan))
    {
        g_isolatorIn.respChan = respChan;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
�� �� ��: cmdIsolInRespChanQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInRespChanQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.respChan);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_RESPCHQ, dataLen, (u8 *)&g_isolatorIn.respChan);
}


/*********************************************************************************************
�� �� ��: cmdIsolInSModeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInSModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    SamplingModeEnum sMode;
    

    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInSModeVerify(cmdDataLen, pCmdData, (void *)&sMode))
    {
        g_isolatorIn.sMode = sMode;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
�� �� ��: cmdIsolInSModeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInSModeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.sMode);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_SMODEQ, dataLen, (u8 *)&g_isolatorIn.sMode);
}


/*********************************************************************************************
�� �� ��: cmdIsolInSPeriodSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInSPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 sPeriod;
    

    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorInSPeriodVerify(cmdDataLen, pCmdData, (void *)&sPeriod))
    {
        g_isolatorIn.sPeriod = sPeriod;
        
        servIsolatorInInfoStore(&g_isolatorIn);
    }
}


/*********************************************************************************************
�� �� ��: cmdIsolInSPeriodQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolInSPeriodQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;


    dataLen = sizeof(g_isolatorIn.sPeriod);
    cmdFrameSend(CMD_ISOLIN, ISOLINCMD_SPERIODQ, dataLen, (u8 *)&g_isolatorIn.sPeriod);
}


/*********************************************************************************************
�� �� ��: cmdIsolInCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdIsolInCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdTrigOutStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8 index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        g_digitalOut.output[index].state = state;

        //��FUNC���ź���
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
�� �� ��: cmdTrigOutStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutSourceSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdTrigOutSourceSet(u8 cmdDataLen, u8 *pCmdData)
{
    ChannelEnum source;
    u8 index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutSourceVerify(cmdDataLen, pCmdData, (void *)&source, &index))
    {
        g_digitalOut.output[index].source = source;

        //��FUNC���ź���
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
�� �� ��: cmdTrigOutSourceQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutConditionSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdTrigOutConditionSet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOCondtEnum condition;
    u8   index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutConditionVerify(cmdDataLen, pCmdData, (void *)&condition, &index))
    {
        g_digitalOut.output[index].condition = condition;

        //��FUNC���ź���
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
�� �� ��: cmdTrigOutConditionQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutSignalSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdTrigOutSignalSet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOSignalEnum signal;
    u8   index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutSignalVerify(cmdDataLen, pCmdData, (void *)&signal, &index))
    {
        g_digitalOut.output[index].signal = signal;

        //��FUNC���ź���
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
�� �� ��: cmdTrigOutSignalQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutPolaritySet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdTrigOutPolaritySet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOPolarityEnum polarity;
    u8   index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutPolarityVerify(cmdDataLen, pCmdData, (void *)&polarity, &index))
    {
        g_digitalOut.output[index].polarity = polarity;

        //��FUNC���ź���
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
�� �� ��: cmdTrigOutPolarityQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutPeriodSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdTrigOutPeriodSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32  period;
    u8   index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutPeriodVerify(cmdDataLen, pCmdData, (void *)&period, &index))
    {
        g_digitalOut.output[index].period = period;

        //��FUNC���ź���
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
�� �� ��: cmdTrigOutPeriodQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutDutySet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdTrigOutDutySet(u8 cmdDataLen, u8 *pCmdData)
{
    u16  duty;
    u8   index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfTrigOutDutyVerify(cmdDataLen, pCmdData, (void *)&duty, &index))
    {
        g_digitalOut.output[index].duty = duty;

        //��FUNC���ź���
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
�� �� ��: cmdTrigOutDutyQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdTrigOutCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdIsolatorStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolatorOutStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    SensorStateEnum state;
    u8 index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorOutStateVerify(cmdDataLen, pCmdData, (void *)&state, &index))
    {
        g_isolatorOut.output[index].state = state;

        //��FUNC���ź���
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
�� �� ��: cmdIsolatorStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdIsolatorOutSourceSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolatorOutSourceSet(u8 cmdDataLen, u8 *pCmdData)
{
    ChannelEnum source;
    u8 index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorOutSourceVerify(cmdDataLen, pCmdData, (void *)&source, &index))
    {
        g_isolatorOut.output[index].source = source;

        //��FUNC���ź���
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
�� �� ��: cmdIsolatorOutSourceQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdIsolatorOutConditionSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolatorOutConditionSet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOCondtEnum condition;
    u8   index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorOutConditionVerify(cmdDataLen, pCmdData, (void *)&condition, &index))
    {
        g_isolatorOut.output[index].condition = condition;

        //��FUNC���ź���
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
�� �� ��: cmdIsolatorOutConditionQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdIsolatorOutResponseSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdIsolatorOutResponseSet(u8 cmdDataLen, u8 *pCmdData)
{
    DIOPolarityEnum response;
    u8   index;

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfIsolatorOutResponseVerify(cmdDataLen, pCmdData, (void *)&response, &index))
    {
        g_isolatorOut.output[index].response = response;

        //��FUNC���ź���
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
�� �� ��: cmdIsolatorOutResponseQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdIsolatorOutCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdIsolatorOutCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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



/*******************************************�ļ�β********************************************/
