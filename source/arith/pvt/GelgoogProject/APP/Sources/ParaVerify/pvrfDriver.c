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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

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
            case 0:
                *(MicroStepEnum *)pParaValue = MICROSTEP_256;
              break;
    
            case 1:
                *(MicroStepEnum *)pParaValue = MICROSTEP_128;
              break;
    
            case 2:
                *(MicroStepEnum *)pParaValue = MICROSTEP_64;
              break;
    
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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

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
        tempValue = *(u32 *)pData;
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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfDriverSGLimitVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDriverSGLimitVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    s8 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(s8) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
    {
        tempIndex = *pData++;
        if (tempIndex < LIMIT_RESERVE)
        {
            *pIndex = tempIndex;
            tempValue = *(s8 *)pData;

            switch ((LimitEnum)tempIndex)
            {
                case LIMIT_UP:
                    if (tempValue <= g_paraLimit.upLimit.sgLimit)
                    {
                        if (tempValue >= g_paraLimit.downLimit.sgLimit)    //Ӧ���Ǵ������޵�            NICK MARK
                        {
                            *(s8 *)pParaValue = tempValue;
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
                  break;
               
                case LIMIT_DOWN:
                    if (tempValue <= g_paraLimit.upLimit.sgLimit)    //Ӧ����С�����޵�            NICK MARK
                    {
                        if (tempValue >= g_paraLimit.downLimit.sgLimit)
                        {
                            *(s8 *)pParaValue = tempValue;
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
                  break;

                default:
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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfDriverSGLimitVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfDriverSGParaVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(s16) + sizeof(u8)) == dataLen)    //�����ĳ��ȼ����±�ĳ���
    {
        tempIndex = *pData++;
        if (tempIndex < SGPARA_RESERVE)
        {
            *pIndex = tempIndex;
            tempValue = *(s16 *)pData;

            switch ((SGParaEnum)tempIndex)
            {
                case SGPARA_SG0:
                    /*if (tempValue <= g_paraLimit.upLimit.sgLimit)
                    {
                        if (tempValue >= g_paraLimit.downLimit.sgLimit)
                        {*/
                            *(s16 *)pParaValue = tempValue;    //TO ADD NICK
                        /*}
                        else
                        {
                            verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                        }
                    }
                    else
                    {
                        verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
                    }*/
                  break;
               
                case SGPARA_SGT:
                    *(s16 *)pParaValue = tempValue;    //TO ADD NICK
                  break;
               
                case SGPARA_SEMAX:
                    *(s16 *)pParaValue = tempValue;    //TO ADD NICK
                  break;
               
                case SGPARA_SEMIN:
                    *(s16 *)pParaValue = tempValue;    //TO ADD NICK
                  break;

                default:
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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}



/*******************************************�ļ�β********************************************/
