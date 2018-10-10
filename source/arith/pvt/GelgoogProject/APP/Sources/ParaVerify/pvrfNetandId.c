/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfNetandId.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.11;
历史版本:  无;
*********************************************************************************************/
#include "pvrfNetandId.h"



/****************************************外部变量声明*****************************************/
extern SystemStateStruct g_systemState;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfIdentityGroupVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIdentityGroupVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pGroup)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8)) == dataLen)    //长度先要正确
    {
        tempValue = *pData++;    //先取GroupIndex
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
函 数 名: pvrfIdentityDistStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIdentityDistStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(DistingStateEnum) == dataLen)    //长度先要正确
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
函 数 名: pvrfIdentityLabelVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfIdentityLabelVerify(u8 dataLen, u8 *pData, void *pMainLabel, void *pSubLabel)
{
    MainLabelEnum mainLabel;
    SubLabelEnum  subLabel;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(MainLabelEnum) + sizeof(SubLabelEnum)) == dataLen)    //长度先要正确
    {
        mainLabel = (MainLabelEnum)*pData++;    //先取mainLabel
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



/*******************************************文件尾********************************************/
