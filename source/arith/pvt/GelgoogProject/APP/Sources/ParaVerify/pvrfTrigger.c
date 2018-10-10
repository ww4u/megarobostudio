/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfTrigger.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "pvrfTrigger.h"



/****************************************�ⲿ��������*****************************************/
extern ParaLimitStruct   g_paraLimit;
extern SystemStateStruct g_systemState;
extern DigitalOutStruct  g_digitalOut;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfTrigInModeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInModeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(TriggerModeEnum) == dataLen)    //������Ҫ��ȷ
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
�� �� ��: pvrfTrigInPattStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInPattStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
�� �� ��: pvrfTrigInPattTypeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInPattTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 i;
    bool bHaveEdge = false;
    u8 edgeIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;
    TrigInTypeEnum *pTrigInType = (TrigInTypeEnum *)pParaValue;

    
    if (dataLen <= TRIGPIN_RESERVE)    //������Ҫ��ȷ
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

            //ͬʱֻ����һ������Ϊ����
            if ((TRIGINTYPE_RISE == *pTrigInType) || (TRIGINTYPE_FALL == *pTrigInType))
            {
                if (bHaveEdge)    //ǰ�������óɱ��صģ�������ΪNONE
                {
                    *(pTrigInType - edgeIndex) = TRIGINTYPE_NONE;
                }
                
                bHaveEdge = true;
                edgeIndex = i;    //��¼�³����ص�λ��
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
�� �� ��: pvrfTrigInPattResponseVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInPattResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
�� �� ��: pvrfTrigInPattSModeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInPattSModeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SamplingModeEnum) == dataLen)    //������Ҫ��ȷ
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
�� �� ��: pvrfTrigInPattSPeriodVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInPattSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //������Ҫ��ȷ
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
�� �� ��: pvrfTrigInLevelStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInLevelStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigInLevelTypeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInLevelTypeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigInLevelResponseVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInLevelResponseVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigInLevelSModeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInLevelSModeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SamplingModeEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigInLevelSPeriodVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigInLevelSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(f32) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigOutStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigOutStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigOutSourceVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigOutSourceVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(ChannelEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigOutConditionVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigOutConditionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOCondtEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigOutSignalVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigOutSignalVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOSignalEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigOutPolarityVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigOutPolarityVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOPolarityEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigOutPeriodVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigOutPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfTrigOutDutyVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTrigOutDutyVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    u16 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u16) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfIsolatorInStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorInStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //�����ĳ���
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
�� �� ��: pvrfIsolatorInTypeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorInTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(TrigInTypeEnum) == dataLen)    //�����ĳ���
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
�� �� ��: pvrfIsolatorInResponseVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorInResponseVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ResponseTypeEnum) == dataLen)    //�����ĳ���
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
�� �� ��: pvrfIsolatorInRespChanVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorInRespChanVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ChannelEnum) == dataLen)    //�����ĳ���
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
�� �� ��: pvrfIsolatorInSModeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorInSModeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SamplingModeEnum) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfIsolatorInSPeriodVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorInSPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //�����ĳ���
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
�� �� ��: pvrfIsolatorOutStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorOutStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(SensorStateEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfIsolatorOutConditionVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorOutConditionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOCondtEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfIsolatorOutSourceVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorOutSourceVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(ChannelEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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
�� �� ��: pvrfIsolatorOutResponseVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIsolatorOutResponseVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(DIOPolarityEnum) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
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



/*******************************************�ļ�β********************************************/
