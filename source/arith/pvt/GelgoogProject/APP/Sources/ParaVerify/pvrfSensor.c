/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfSensor.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include "pvrfSensor.h"



/****************************************外部变量声明*****************************************/
extern ParaLimitStruct   g_paraLimit;
extern SystemStateStruct g_systemState;
extern DigitalOutStruct  g_digitalOut;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfOtpStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfOtpStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
函 数 名: pvrfOtpThresholdVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfOtpThresholdVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u16 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u16) == dataLen)    //长度先要正确
    {
        //tempValue = *(u16 *)pData;
        memcpy(&tempValue, pData, dataLen);
        if (tempValue <= g_paraLimit.upLimit.otpThr)
        {
            if (tempValue >= g_paraLimit.downLimit.otpThr)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfOtpResponseVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfOtpResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
函 数 名: pvrfOtpPeriodVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfOtpPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //参数的长度加上下标的长度
    {
        //tempValue = *(u32 *)pData;
        memcpy(&tempValue, pData, dataLen);
        if (tempValue <= g_paraLimit.upLimit.reportPeriod)
        {
            if (tempValue >= g_paraLimit.downLimit.reportPeriod)
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


/*********************************************************************************************
函 数 名: pvrfAnalogInStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfAnalogInStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //参数的长度先要正确
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
函 数 名: pvrfAnalogInThresholdVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfAnalogInThresholdVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //参数的长度加上下标的长度
    {
        tempValue = *(f32 *)pData;
        memcpy(&tempValue, pData, dataLen);
        if (tempValue <= g_paraLimit.upLimit.asensorThr)
        {
            if (tempValue >= g_paraLimit.downLimit.asensorThr)
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
函 数 名: pvrfAnalogInResponseVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfAnalogInResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(ResponseTypeEnum) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
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
函 数 名: pvrfReportStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfReportStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(tempIndex)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *(u8*)pData++;
        if (tempIndex < REPTTYPE_RESERVE)
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
函 数 名: pvrfReportPeriodVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfReportPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(tempIndex)) == dataLen)    //参数的长度加上下标的长度
    {
        tempIndex = *(u8*)pData++;
        if (tempIndex < REPTTYPE_RESERVE)
        {
            *pIndex = tempIndex;
            //tempValue = *(u32 *)pData;
            memcpy(&tempValue, pData, sizeof(u32));
            if (tempValue <= g_paraLimit.upLimit.reportPeriod)
            {
                if (tempValue >= g_paraLimit.downLimit.reportPeriod)
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
函 数 名: pvrfSensorUartStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSensorUartStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < UARTNUM_RESERVE)
        {
            *pUartIndex = tempIndex;
            tempIndex = *pData++;
            if (tempIndex < SENSOR_RESERVE)
            {
                *pSensorIndex = tempIndex;
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
函 数 名: pvrfSensorUartSofVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSensorUartSofVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8 tempIndex;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < UARTNUM_RESERVE)
        {
            *pUartIndex = tempIndex;
            tempIndex = *pData++;
            if (tempIndex < SENSOR_RESERVE)
            {
                *pSensorIndex = tempIndex;
                *(u8 *)pParaValue = *pData;
            }
            else
            {
                verifyResult = PARA_VERIFY_ERROR_INDEX;
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
函 数 名: pvrfSensorUartFrameLenVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSensorUartFrameLenVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < UARTNUM_RESERVE)
        {
            *pUartIndex = tempIndex;
            tempIndex = *pData++;
            if (tempIndex < SENSOR_RESERVE)
            {
                *pSensorIndex = tempIndex;
                tempValue = *(u8 *)pData;
                if (tempValue <= g_paraLimit.upLimit.snUartFrameLen)
                {
                    if (tempValue >= g_paraLimit.downLimit.snUartFrameLen)
                    {
                        *(u8 *)pParaValue = tempValue;
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
函 数 名: pvrfSensorUartRecvNumVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSensorUartRecvNumVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < UARTNUM_RESERVE)
        {
            *pUartIndex = tempIndex;
            tempIndex = *pData++;
            if (tempIndex < SENSOR_RESERVE)
            {
                *pSensorIndex = tempIndex;
                tempValue = *(u8 *)pData;
                if (tempValue <= g_paraLimit.upLimit.snUartRecvNum)
                {
                    if (tempValue >= g_paraLimit.downLimit.snUartRecvNum)
                    {
                        *(u8 *)pParaValue = tempValue;
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
函 数 名: pvrfSensorUartSwTimeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSensorUartSwTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(u8) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < UARTNUM_RESERVE)
        {
            *pUartIndex = tempIndex;
            tempIndex = *pData++;
            if (tempIndex < SENSOR_RESERVE)
            {
                *pSensorIndex = tempIndex;
                //tempValue = *(u32 *)pData;
                memcpy(&tempValue, pData, sizeof(u32));
                if (tempValue <= g_paraLimit.upLimit.snUartSwTime)
                {
                    if (tempValue >= g_paraLimit.downLimit.snUartSwTime)
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


#if GELGOOG_SINANJU
/*********************************************************************************************
函 数 名: pvrfAbsEncAlarmStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfAbsEncAlarmStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < ABS_ENCODER_NUM)
        {
            *pSensorIndex = tempIndex;
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
函 数 名: pvrfAbsEncAlarmZeroPostVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfAbsEncAlarmZeroPostVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < ABS_ENCODER_NUM)
        {
            *pSensorIndex = tempIndex;
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
函 数 名: pvrfAbsEncAlarmUpLimitVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfAbsEncAlarmLimitVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < ABS_ENCODER_NUM)
        {
            *pSensorIndex = tempIndex;
            //tempValue = *((u32 *)pData);
            memcpy(&tempValue, pData, sizeof(u32));
            if (tempValue <= g_paraLimit.upLimit.absEncValue)
            {
                if (tempValue >= g_paraLimit.downLimit.absEncValue)
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
函 数 名: pvrfAbsEncAlarmDnLimitVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfAbsEncAlarmResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ResponseTypeEnum) == dataLen)    //参数的长度加上两个下标的长度
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
函 数 名: pvrfDistAlarmStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDistAlarmStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < DIST_SENSOR_NUM)
        {
            *pSensorIndex = tempIndex;
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
函 数 名: pvrfDistAlarmDistVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDistAlarmDistVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8  tempIndex;
    u16 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u16) + sizeof(u8)) == dataLen)    //参数的长度加上两个下标的长度
    {
        tempIndex = *pData++;
        if (tempIndex < DIST_SENSOR_NUM)
        {
            *pSensorIndex = tempIndex;
            //tempValue = *(u16 *)pData;
            memcpy(&tempValue, pData, sizeof(u16));
            if (tempValue <= g_paraLimit.upLimit.alarmDist)
            {
                if (tempValue >= g_paraLimit.downLimit.alarmDist)
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


/*********************************************************************************************
函 数 名: pvrfPdmSampleStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfPdmSampleStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
函 数 名: pvrfPdmMstepDataVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfPdmMstepDataVerify(u8 dataLen, u8 *pData, u16 *pReadOffset, u16 *pReadLen)
{
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u16) + sizeof(u16)) == dataLen)
    {
        memcpy(pReadOffset, pData, sizeof(u16));
        pData += sizeof(u16);
        memcpy(pReadLen,    pData, sizeof(u16));
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
