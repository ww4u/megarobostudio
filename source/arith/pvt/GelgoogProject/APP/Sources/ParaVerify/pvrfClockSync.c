/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfClockSync.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.04.12;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "pvrfClockSync.h"



/****************************************�ⲿ��������*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfClockFrequencyVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfClockFrequencyVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *(u32 *)pData;

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
        *(u32 *)pParaValue = tempValue;
        
#else

        if (tempValue <= g_paraLimit.upLimit.fpgaPwmClock)
        {
            if (tempValue >= g_paraLimit.downLimit.fpgaPwmClock)
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
#endif
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfClockSyncVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfClockSyncRegVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ClockSyncRegEnum) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(ClockSyncRegEnum *)pParaValue = CLOCKREG_OFF;
              break;
              
            case 1:
                *(ClockSyncRegEnum *)pParaValue = CLOCKREG_ON;
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
�� �� ��: pvrfClockSyncStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfClockSyncStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ClockSyncStateEnum) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(ClockSyncStateEnum *)pParaValue = CLOCKSYNC_END;
              break;
              
            case 1:
                *(ClockSyncStateEnum *)pParaValue = CLOCKSYNC_START;
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
�� �� ��: pvrfClockSyncStartTypeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfClockSyncStartTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ReceiveTypeEnum) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(ReceiveTypeEnum *)pParaValue = RECEIVE_RADIOID;
              break;
              
            case 1:
                *(ReceiveTypeEnum *)pParaValue = RECEIVE_RECEIVEID;
              break;
              
            case 2:
                *(ReceiveTypeEnum *)pParaValue = RECEIVE_GROUPID1;
              break;
 
             case 3:
                *(ReceiveTypeEnum *)pParaValue = RECEIVE_GROUPID2;
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



/*******************************************�ļ�β********************************************/
