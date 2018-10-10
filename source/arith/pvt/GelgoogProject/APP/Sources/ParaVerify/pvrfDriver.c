/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfDriver.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.10.25;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "pvrfDriver.h"



/****************************************�ⲿ��������*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfDriverCurrVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDriverCurrVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        if (tempValue <= g_paraLimit.upLimit.motorCurr)
        {
            if (tempValue >= g_paraLimit.downLimit.motorCurr)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfDriverMicroStepsVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDriverMicroStepsVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(MicroStepEnum) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        switch (tempValue)
        {
#if QUBELEY_HARDVER_1
            case 0:
                *(MicroStepEnum *)pParaValue = MICROSTEP_256;
              break;
    
            case 1:
                *(MicroStepEnum *)pParaValue = MICROSTEP_128;
              break;
#endif

#if !GELGOOG_AXIS_10
            case 2:
                *(MicroStepEnum *)pParaValue = MICROSTEP_64;
              break;
#endif
            case 3:
                *(MicroStepEnum *)pParaValue = MICROSTEP_32;
              break;
    
            case 4:
                *(MicroStepEnum *)pParaValue = MICROSTEP_16;
              break;
    
            case 5:
                *(MicroStepEnum *)pParaValue = MICROSTEP_8;
              break;
    
            case 6:
                *(MicroStepEnum *)pParaValue = MICROSTEP_4;
              break;
    
            case 7:
                *(MicroStepEnum *)pParaValue = MICROSTEP_2;
              break;
    
            case 8:
                *(MicroStepEnum *)pParaValue = MICROSTEP_1;
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
�� �� ��: pvrfDriverStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDriverStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
�� �� ��: pvrfDriverMonitorPeriodVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDriverMonitorPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;


    if (sizeof(u32) == dataLen)    //������Ҫ��ȷ
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
�� �� ��: pvrfDriverRegisterVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDriverRegisterVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
    {
        tempIndex = *pData++;
        if (tempIndex < DRVREG_RESERVE)
        {
            *pIndex = tempIndex;
            *(u32 *)pParaValue = *(u32 *)pData;
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
�� �� ��: pvrfDriverSwitchTimeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDriverSwitchTimeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)
    {
        //tempValue = *(u32 *)pData;
        memcpy(&tempValue, pData, dataLen);
        if (tempValue <= g_paraLimit.upLimit.switchTime)
        {
            if (tempValue >= g_paraLimit.downLimit.switchTime)
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


#if 0
#endif


/*********************************************************************************************
�� �� ��: pvrfTuningCurrRatioVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTuningCurrRatioVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(CurrRatioEnum) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(CurrRatioEnum *)pParaValue = CURRRATIO_HALF;
              break;
    
            case 1:
                *(CurrRatioEnum *)pParaValue = CURRRATIO_QUARTER;
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
�� �� ��: pvrfTuningEnergyEfficVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTuningEnergyEfficVerify(u8 dataLen, u8 *pData, u16 *pEnergEfficMax, u16 *pEnergEfficOffset)
{
    u16 energEfficMax;
    u16 energEfficOffset;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(energEfficMax) + sizeof(energEfficOffset)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
    {
        memcpy(&energEfficMax, pData, sizeof(energEfficMax));
        pData += sizeof(energEfficMax);
        memcpy(&energEfficOffset, pData, sizeof(energEfficOffset));

        if ((energEfficMax <= SCALE_MAGNIFICATION) && 
            (energEfficMax >= (SCALE_MAGNIFICATION / 2)) &&
            (energEfficOffset <= (SCALE_MAGNIFICATION / 2)))
        {
            *pEnergEfficMax = energEfficMax;
            *pEnergEfficOffset = energEfficOffset;
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
�� �� ��: pvrfTuningCurrRatioVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfTuningCurrRegulateVerify(u8 dataLen, u8 *pData, CurrIncreEnum *pCurrIncre, CurrDecreEnum *pCurrDecre)
{
    u8 tempIncre;
    u8 tempDecre;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(CurrIncreEnum) + sizeof(CurrDecreEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempIncre = *pData++;
        tempDecre = *pData;
        switch (tempIncre)
        {
            case 0:
                *pCurrIncre = CURRINCRE_1;
              break;
    
            case 1:
                *pCurrIncre = CURRINCRE_2;
              break;
    
            case 2:
                *pCurrIncre = CURRINCRE_4;
              break;
    
            case 3:
                *pCurrIncre = CURRINCRE_8;
              break;
    
            default:
                verifyResult = PARA_VERIFY_ERROR_TYPE;
              break;
        }
        
        switch (tempDecre)
        {
            case 0:
                *pCurrDecre = CURRDECRE_32;
              break;
    
            case 1:
                *pCurrDecre = CURRDECRE_8;
              break;
    
            case 2:
                *pCurrDecre = CURRDECRE_2;
              break;
    
            case 3:
                *pCurrDecre = CURRDECRE_1;
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



/*******************************************�ļ�β********************************************/
