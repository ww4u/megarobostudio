/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfSystem.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "pvrfSystem.h"



/****************************************�ⲿ��������*****************************************/
extern SystemStateStruct g_systemState;
extern ParaLimitStruct   g_paraLimit;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfSystemParaVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSystemModeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(WorkModeEnum) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(WorkModeEnum *)pParaValue = WORK_NORMAL;
              break;
    
            case 1:
                *(WorkModeEnum *)pParaValue = WORK_UNIFORM;
              break;  
    
            case 2:
                *(WorkModeEnum *)pParaValue = WORK_POSITION;
              break; 
    
            case 3:
                *(WorkModeEnum *)pParaValue = WORK_TORQUE;
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
�� �� ��: pvrfSystemPowerOnVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSystemPowerOnVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(PowerOnModeEnum) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(PowerOnModeEnum *)pParaValue = POWERON_DEFAULT;
              break;
    
            case 1:
                *(PowerOnModeEnum *)pParaValue = POWERON_LAST;
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
�� �� ��: pvrfSystemParaVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSystemDioRefReadVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
�� �� ��: pvrfSystemReverseMotionVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSystemReverseMotionVerify(u8 dataLen, u8 *pData, void *pParaValue)
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


#if GELGOOG_SINANJU
/*********************************************************************************************
�� �� ��: pvrfSystemFanParaVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSystemFanParaVerify(u8 dataLen, u8 *pData, void *pParaValue1, void *pParaValue2)
{
    u8  tempValue1;
    u32 tempValue2;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u32)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue1 = *pData++;
        if (tempValue1 <= g_paraLimit.upLimit.fanDuty)
        {
            *(u8 *)pParaValue1 = tempValue1;
            memcpy(&tempValue2, pData, sizeof(tempValue2));

            /*if ()    //TO ADD NICK
            {*/
                *(u32 *)pParaValue2 = tempValue2;
            /*}
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }*/
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
�� �� ��: pvrfSystemArmLedParaVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSystemArmLedParaVerify(u8 dataLen, u8 *pData, void *pParaValue1, void *pParaValue2, u8 *pIndex)
{
    u8  tempValue1;
    u32 tempValue2;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8) + sizeof(u32)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue1 = *pData++;
        if (tempValue1 < ARMLED_RESERVE)
        {
            *(u8 *)pIndex = tempValue1;

            tempValue1 = *pData++;
            if (tempValue1 <= g_paraLimit.upLimit.fanDuty)
            {
                *(u8 *)pParaValue1 = tempValue1;
                memcpy(&tempValue2, pData, sizeof(tempValue2));

                /*if ()    //TO ADD NICK
                {*/
                    *(u32 *)pParaValue2 = tempValue2;
                /*}
                else
                {
                    verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
                }*/
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
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
�� �� ��: pvrfSystemArmLedFreqVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfSystemArmLedFreqVerify(u8 dataLen, u8 *pData, SensorStateEnum *pLedState, u16 *pLedFreq)
{
    u8  tempValue1;
    u16 tempValue2;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u16)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue1 = *pData++;
        if (tempValue1 <= (u8)SENSOR_ON)
        {
            *pLedState = (SensorStateEnum)tempValue1;

            memcpy(&tempValue2, pData, sizeof(tempValue2));
            
            if (tempValue2 <= g_paraLimit.upLimit.ledFlickerFreq)
            {
                if (tempValue2 >= g_paraLimit.downLimit.ledFlickerFreq)
                {
                    *pLedFreq = tempValue2;
                }
                else
                {
                    verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
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
