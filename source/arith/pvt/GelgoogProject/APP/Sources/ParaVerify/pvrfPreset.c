/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfPreset.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.06.29;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "pvrfPreset.h"



/****************************************�ⲿ��������*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfPresetPvtPositionVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPresetPvtPositionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(f32) + sizeof(u8)) == dataLen)    //������Ҫ��ȷ
    {
        tempIndex = *(u8 *)pData;
        if (tempIndex < PRESET_PVT_BUFFER_SIZE)
        {
            *pIndex = tempIndex;
            tempValue = *(f32 *)(pData + sizeof(tempIndex));
            if (tempValue <= g_paraLimit.upLimit.pvtPosn)
            {
                if (tempValue >= g_paraLimit.downLimit.pvtPosn)
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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}



/*********************************************************************************************
�� �� ��: pvrfPresetPvtSpeedVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPresetPvtSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(f32) + sizeof(u8)) == dataLen)    //������Ҫ��ȷ
    {
        tempIndex = *(u8 *)pData;
        if (tempIndex < PRESET_PVT_BUFFER_SIZE)
        {
            *pIndex = tempIndex;
            tempValue = *(f32 *)(pData + sizeof(tempIndex));
            if (tempValue <= g_paraLimit.upLimit.pvtSpeed)
            {
                if (tempValue >= g_paraLimit.downLimit.pvtSpeed)
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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}



/*********************************************************************************************
�� �� ��: pvrfPresetPvtTimeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPresetPvtTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(f32) + sizeof(u8)) == dataLen)    //������Ҫ��ȷ
    {
        tempIndex = *(u8 *)pData;
        if (tempIndex < PRESET_PVT_BUFFER_SIZE)
        {
            *pIndex = tempIndex;
            tempValue = *(f32 *)(pData + sizeof(tempIndex));
            if (tempValue <= g_paraLimit.upLimit.pvtTime)
            {
                if ((tempValue >= g_paraLimit.downLimit.pvtTime) || (tempValue == -1.0f))
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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPresetStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPresetStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8)) == dataLen)    //������Ҫ��ȷ
    {
        tempIndex = *(u8 *)pData;
        if (tempIndex < PRESET_PVT_BUFFER_SIZE)
        {
            *pIndex = tempIndex;
            
            tempValue = *(pData + sizeof(tempIndex));
            switch (tempValue)
            {
                case 0:
                    *(PresetStateEnum *)pParaValue = PRESET_OFF;
                  break;
                  
                case 1:
                    *(PresetStateEnum *)pParaValue = PRESET_ON;
                  break;
                  
                case 2:
                    *(PresetStateEnum *)pParaValue = PRESET_CALCABLE;
                  break;

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}



/*******************************************�ļ�β********************************************/
