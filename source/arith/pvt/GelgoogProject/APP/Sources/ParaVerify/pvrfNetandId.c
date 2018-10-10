/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfNetandId.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.11;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "pvrfNetandId.h"



/****************************************�ⲿ��������*****************************************/
extern SystemStateStruct g_systemState;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfIdentityGroupVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIdentityGroupVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pGroup)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;    //��ȡGroupIndex
        if (tempValue < GROUP_NUM)
        {
            *pGroup = tempValue;
            tempValue = *pData;

            if (tempValue >= GROUP_NUM_VALUE_MIN)
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
�� �� ��: pvrfIdentityDistStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIdentityDistStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(DistingStateEnum) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(DistingStateEnum *)pParaValue = DISTING_OFF;
              break;
    
            case 1:
                *(DistingStateEnum *)pParaValue = DISTING_ON;
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
�� �� ��: pvrfIdentityLabelVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfIdentityLabelVerify(u8 dataLen, u8 *pData, void *pMainLabel, void *pSubLabel)
{
    MainLabelEnum mainLabel;
    SubLabelEnum  subLabel;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(MainLabelEnum) + sizeof(SubLabelEnum)) == dataLen)    //������Ҫ��ȷ
    {
        mainLabel = (MainLabelEnum)*pData++;    //��ȡmainLabel
        if (mainLabel < MLABEL_RESERVE)
        {
            *(MainLabelEnum *)pMainLabel = mainLabel;
            subLabel = (SubLabelEnum)*pData;

            if (subLabel < SLABEL_RESERVE)
            {
                *(SubLabelEnum *)pSubLabel = subLabel;
            }
            else
            {
                verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
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



/*******************************************�ļ�β********************************************/
