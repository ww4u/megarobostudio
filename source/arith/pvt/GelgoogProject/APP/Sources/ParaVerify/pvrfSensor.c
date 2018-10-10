/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfSensor.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "pvrfSensor.h"



/****************************************�ⲿ��������*****************************************/
extern ParaLimitStruct   g_paraLimit;
extern SystemStateStruct g_systemState;
extern DigitalOutStruct  g_digitalOut;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfOtpStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfOtpStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //������Ҫ��ȷ
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
�� �� ��: pvrfOtpThresholdVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfOtpThresholdVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u16 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u16) == dataLen)    //������Ҫ��ȷ
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
�� �� ��: pvrfOtpResponseVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfOtpResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ResponseTypeEnum) == dataLen)    //������Ҫ��ȷ
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
�� �� ��: pvrfOtpPeriodVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfOtpPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfAnalogInStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfAnalogInStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //�����ĳ�����Ҫ��ȷ
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
�� �� ��: pvrfAnalogInThresholdVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfAnalogInThresholdVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfAnalogInResponseVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfAnalogInResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(ResponseTypeEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfReportStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfReportStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(tempIndex)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfReportPeriodVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfReportPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(tempIndex)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfSensorUartStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSensorUartStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfSensorUartSofVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSensorUartSofVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8 tempIndex;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfSensorUartFrameLenVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSensorUartFrameLenVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfSensorUartRecvNumVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSensorUartRecvNumVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfSensorUartSwTimeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSensorUartSwTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex, u8 *pUartIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(u8) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfAbsEncAlarmStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfAbsEncAlarmStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfAbsEncAlarmZeroPostVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfAbsEncAlarmZeroPostVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfAbsEncAlarmUpLimitVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfAbsEncAlarmLimitVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfAbsEncAlarmDnLimitVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfAbsEncAlarmResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ResponseTypeEnum) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfDistAlarmStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDistAlarmStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfDistAlarmDistVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDistAlarmDistVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pSensorIndex)
{
    u8  tempIndex;
    u16 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u16) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ��������±�ĳ���
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
�� �� ��: pvrfPdmSampleStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPdmSampleStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //������Ҫ��ȷ
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
�� �� ��: pvrfPdmMstepDataVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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



/*******************************************�ļ�β********************************************/
