/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfPreset.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.06.29;
历史版本:  无;
*********************************************************************************************/
#include "pvrfPreset.h"



/****************************************外部变量声明*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfPresetPvtPositionVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfPresetPvtPositionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(f32) + sizeof(u8)) == dataLen)    //长度先要正确
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
函 数 名: pvrfPresetPvtSpeedVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfPresetPvtSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(f32) + sizeof(u8)) == dataLen)    //长度先要正确
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
函 数 名: pvrfPresetPvtTimeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfPresetPvtTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(f32) + sizeof(u8)) == dataLen)    //长度先要正确
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
函 数 名: pvrfPresetStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfPresetStateVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8)) == dataLen)    //长度先要正确
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



/*******************************************文件尾********************************************/
