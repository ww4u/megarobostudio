/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfTrigger.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include "pvrfTrigger.h"



/****************************************外部变量声明*****************************************/
extern ParaLimitStruct   g_paraLimit;
extern SystemStateStruct g_systemState;
extern DigitalOutStruct  g_digitalOut;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfTrigInModeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInModeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(TriggerModeEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(TriggerModeEnum *)pParaValue = TRIGMODE_PATTERN;
              break; 

            case 1:
                *(TriggerModeEnum *)pParaValue = TRIGMODE_LEVEL;
              break;       

            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInPattStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInPattStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(SensorStateEnum *)pParaValue = SENSOR_OFF;
              break;

            case 1:
                *(SensorStateEnum *)pParaValue = SENSOR_ON;
              break;       

            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInPattTypeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInPattTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 i;
    bool bHaveEdge = false;
    u8 edgeIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;
    TrigInTypeEnum *pTrigInType = (TrigInTypeEnum *)pParaValue;

    
    if (dataLen <= TRIGPIN_RESERVE)    //长度先要正确
    {
        for (i = 0, edgeIndex = 0;i < dataLen;i++)
        {
            tempValue = *pData++;
            switch (tempValue)
            {
                case 0:
                    *pTrigInType = TRIGINTYPE_NONE;
                  break;

                case 1:
                    *pTrigInType = TRIGINTYPE_LOW;
                  break;  

                case 2:
                    *pTrigInType = TRIGINTYPE_RISE;
                  break; 

                case 3:
                    *pTrigInType = TRIGINTYPE_FALL;
                  break;  

                case 4:
                    *pTrigInType = TRIGINTYPE_HIGH;
                  break;     

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }

            //同时只能有一个设置为边沿
            if ((TRIGINTYPE_RISE == *pTrigInType) || (TRIGINTYPE_FALL == *pTrigInType))
            {
                if (bHaveEdge)    //前面有设置成边沿的，则设置为NONE
                {
                    *(pTrigInType - edgeIndex) = TRIGINTYPE_NONE;
                }
                
                bHaveEdge = true;
                edgeIndex = i;    //记录下出边沿的位置
            }
            
            pTrigInType++;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInPattResponseVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInPattResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ResponseTypeEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_NONE;
              break;
    
            case 1:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_ALARM;
              break;  
    
            case 2:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_STOP;
              break; 
    
            case 3:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_ALARMSTOP;
              break;  
    
            case 4:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_RUN;
              break;      
    
            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInPattSModeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInPattSModeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SamplingModeEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(SamplingModeEnum *)pParaValue = SMODE_CONTINUED;
              break;
    
            case 1:
                *(SamplingModeEnum *)pParaValue = SMODE_INTERVAL;
              break;  
    
            case 2:
                *(SamplingModeEnum *)pParaValue = SMODE_SINGLE;
              break;       
    
            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInPattSPeriodVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInPattSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        //tempValue = *(f32 *)pData;
        memcpy(&tempValue, pData, dataLen);
        if (tempValue <= g_paraLimit.upLimit.trigInPeriod)
        {
            if (tempValue >= g_paraLimit.downLimit.trigInPeriod)
            {
                *(f32 *)pParaValue = tempValue;
            }
            else
            {
                verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInLevelStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInLevelStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < TRIGPIN_RESERVE)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(SensorStateEnum *)pParaValue = SENSOR_OFF;
                  break; 

                case 1:
                    *(SensorStateEnum *)pParaValue = SENSOR_ON;
                  break;       

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInLevelTypeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInLevelTypeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < TRIGPIN_RESERVE)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_NONE;
                  break;

                case 1:
                    *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_LOW;
                  break;  

                case 2:
                    *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_RISE;
                  break; 

                case 3:
                    *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_FALL;
                  break;  

                case 4:
                    *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_HIGH;
                  break;     

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInLevelResponseVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInLevelResponseVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < TRIGPIN_RESERVE)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_NONE;
                  break;
        
                case 1:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_ALARM;
                  break;  
        
                case 2:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_STOP;
                  break; 
        
                case 3:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_ALARMSTOP;
                  break;  
        
                case 4:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_RUN;
                  break;      
        
                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInLevelSModeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInLevelSModeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SamplingModeEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < TRIGPIN_RESERVE)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(SamplingModeEnum *)pParaValue = SMODE_CONTINUED;
                  break;
        
                case 1:
                    *(SamplingModeEnum *)pParaValue = SMODE_INTERVAL;
                  break;  
        
                case 2:
                    *(SamplingModeEnum *)pParaValue = SMODE_SINGLE;
                  break;    
        
                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigInLevelSPeriodVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigInLevelSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(f32) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < TRIGPIN_RESERVE)
        {
            *pIndex = tempIndex;
            //tempValue = *(f32 *)pData;
            memcpy(&tempValue, pData, sizeof(f32));
            if (tempValue <= g_paraLimit.upLimit.trigInPeriod)
            {
                if (tempValue >= g_paraLimit.downLimit.trigInPeriod)
                {
                    *(f32 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


#if !GELGOOG_SINANJU
/*********************************************************************************************
函 数 名: pvrfTrigOutStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigOutStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.doutNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(SensorStateEnum *)pParaValue = SENSOR_OFF;
                  break; 

                case 1:
                    *(SensorStateEnum *)pParaValue = SENSOR_ON;
                  break;       

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigOutSourceVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigOutSourceVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(ChannelEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.doutNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;

            if (tempValue <= CH_MAX)
            {
                *(ChannelEnum *)pParaValue = (ChannelEnum)tempValue;
            }
            else
            {
                verifyResult = PARA_VERIFY_ERROR_TYPE;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigOutConditionVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigOutConditionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOCondtEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.doutNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(DIOCondtEnum *)pParaValue = DIOCONDT_WTSTART;
                  break;

                case 1:
                    *(DIOCondtEnum *)pParaValue = DIOCONDT_WTEND;
                  break;

                case 2:
                    *(DIOCondtEnum *)pParaValue = DIOCONDT_AUTO;
                  break;         

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigOutSignalVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigOutSignalVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOSignalEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.doutNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(DIOSignalEnum *)pParaValue = DIOSIGNAL_LEVEL;
                  break;

                case 1:
                    *(DIOSignalEnum *)pParaValue = DIOSIGNAL_SQUARE;
                  break; 

                case 2:
                    *(DIOSignalEnum *)pParaValue = DIOSIGNAL_PULSE;
                  break;       

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigOutPolarityVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigOutPolarityVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOPolarityEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.doutNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(DIOPolarityEnum *)pParaValue = DIOPOLARITY_N;
                  break;

                case 1:
                    *(DIOPolarityEnum *)pParaValue = DIOPOLARITY_P;
                  break;        

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigOutPeriodVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigOutPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.doutNum)
        {
            *pIndex = tempIndex;
            //tempValue = *(u32 *)pData;
            memcpy(&tempValue, pData, sizeof(u32));
            if (tempValue <= g_paraLimit.upLimit.trigOutPeriod)
            {
                if (tempValue >= g_paraLimit.downLimit.trigOutPeriod)
                {
                    *(u32 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfTrigOutDutyVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfTrigOutDutyVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    u16 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u16) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.doutNum)
        {
            *pIndex = tempIndex;
            //tempValue = *(u16 *)pData;
            memcpy(&tempValue, pData, sizeof(u16));
            if (tempValue <= g_paraLimit.upLimit.trigOutDuty)
            {
                if (tempValue >= g_paraLimit.downLimit.trigOutDuty)
                {
                    *(u16 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}
#endif


#ifdef PROJECT_GELGOOG
#if !GELGOOG_SINANJU
/*********************************************************************************************
函 数 名: pvrfIsolatorInStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorInStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //参数的长度
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(SensorStateEnum *)pParaValue = SENSOR_OFF;
              break; 

            case 1:
                *(SensorStateEnum *)pParaValue = SENSOR_ON;
              break;       

            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfIsolatorInTypeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorInTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(TrigInTypeEnum) == dataLen)    //参数的长度
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_NONE;
              break;

            case 1:
                *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_LOW;
              break;  

            case 2:
                *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_RISE;
              break; 

            case 3:
                *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_FALL;
              break;  

            case 4:
                *(TrigInTypeEnum *)pParaValue = TRIGINTYPE_HIGH;
              break;     

            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfIsolatorInResponseVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorInResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ResponseTypeEnum) == dataLen)    //参数的长度
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_NONE;
              break;
    
            case 1:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_ALARM;
              break;  
    
            case 2:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_STOP;
              break; 
    
            case 3:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_ALARMSTOP;
              break;  
    
            case 4:
                *(ResponseTypeEnum *)pParaValue = RESPONSE_RUN;
              break;      
    
            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfIsolatorInRespChanVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorInRespChanVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ChannelEnum) == dataLen)    //参数的长度
    {
        tempValue = *pData;
        if (tempValue <= CH_MAX)
        {
            *(ChannelEnum *)pParaValue = (ChannelEnum)tempValue;
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_TYPE;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfIsolatorInSModeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorInSModeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SamplingModeEnum) == dataLen)    //参数的长度加上下标的长度
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(SamplingModeEnum *)pParaValue = SMODE_CONTINUED;
              break;
    
            case 1:
                *(SamplingModeEnum *)pParaValue = SMODE_INTERVAL;
              break;  
    
            case 2:
                *(SamplingModeEnum *)pParaValue = SMODE_SINGLE;
              break;    
    
            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfIsolatorInSPeriodVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorInSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //参数的长度
    {
        //tempValue = *(u32 *)pData;
        memcpy(&tempValue, pData, dataLen);
        if (tempValue <= g_paraLimit.upLimit.trigInPeriod)
        {
            if (tempValue >= g_paraLimit.downLimit.trigInPeriod)
            {
                *(u32 *)pParaValue = tempValue;
            }
            else
            {
                verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}
#endif
#endif


#if !GELGOOG_SINANJU
/*********************************************************************************************
函 数 名: pvrfIsolatorOutStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorOutStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.youtNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(SensorStateEnum *)pParaValue = SENSOR_OFF;
                  break; 

                case 1:
                    *(SensorStateEnum *)pParaValue = SENSOR_ON;
                  break;       

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfIsolatorOutConditionVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorOutConditionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOCondtEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.youtNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(DIOCondtEnum *)pParaValue = DIOCONDT_WTSTART;
                  break;

                case 1:
                    *(DIOCondtEnum *)pParaValue = DIOCONDT_WTEND;
                  break;

                case 2:
                    *(DIOCondtEnum *)pParaValue = DIOCONDT_AUTO;
                  break;         

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfIsolatorOutSourceVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorOutSourceVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(ChannelEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.youtNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            if (tempValue <= CH_MAX)
            {
                *(ChannelEnum *)pParaValue = (ChannelEnum)tempValue;
            }
            else
            {
                verifyResult = PARA_VERIFY_ERROR_TYPE;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfIsolatorOutResponseVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIsolatorOutResponseVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOPolarityEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < g_systemState.youtNum)
        {
            *pIndex = tempIndex;
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(DIOPolarityEnum *)pParaValue = DIOPOLARITY_N;
                  break;

                case 1:
                    *(DIOPolarityEnum *)pParaValue = DIOPOLARITY_P;
                  break;      

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}
#endif



/*******************************************文件尾********************************************/
