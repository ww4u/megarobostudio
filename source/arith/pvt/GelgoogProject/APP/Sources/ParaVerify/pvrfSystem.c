/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfSystem.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include "pvrfSystem.h"



/****************************************外部变量声明*****************************************/
extern SystemStateStruct g_systemState;
extern ParaLimitStruct   g_paraLimit;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfSystemParaVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSystemModeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(WorkModeEnum) == dataLen)    //长度先要正确
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
函 数 名: pvrfSystemPowerOnVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSystemPowerOnVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(PowerOnModeEnum) == dataLen)    //长度先要正确
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
函 数 名: pvrfSystemParaVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSystemDioRefReadVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //长度先要正确
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
函 数 名: pvrfSystemReverseMotionVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSystemReverseMotionVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(SensorStateEnum) == dataLen)    //长度先要正确
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
函 数 名: pvrfSystemFanParaVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSystemFanParaVerify(u8 dataLen, u8 *pData, void *pParaValue1, void *pParaValue2)
{
    u8  tempValue1;
    u32 tempValue2;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u32)) == dataLen)    //长度先要正确
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
函 数 名: pvrfSystemArmLedParaVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSystemArmLedParaVerify(u8 dataLen, u8 *pData, void *pParaValue1, void *pParaValue2, u8 *pIndex)
{
    u8  tempValue1;
    u32 tempValue2;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u8) + sizeof(u32)) == dataLen)    //长度先要正确
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
函 数 名: pvrfSystemArmLedFreqVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfSystemArmLedFreqVerify(u8 dataLen, u8 *pData, SensorStateEnum *pLedState, u16 *pLedFreq)
{
    u8  tempValue1;
    u16 tempValue2;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u8) + sizeof(u16)) == dataLen)    //长度先要正确
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



/*******************************************文件尾********************************************/
