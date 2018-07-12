/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfDriver.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.10.25;
历史版本:  无;
*********************************************************************************************/
#include "pvrfDriver.h"



/****************************************外部变量声明*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfDriverCurrVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDriverCurrVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
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
函 数 名: pvrfDriverMicroStepsVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDriverMicroStepsVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(MicroStepEnum) == dataLen)    //长度先要正确
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
函 数 名: pvrfDriverStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDriverStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfDriverMonitorPeriodVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDriverMonitorPeriodVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;


    if (sizeof(u32) == dataLen)    //长度先要正确
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
函 数 名: pvrfDriverRegisterVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDriverRegisterVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8 tempIndex;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(u32) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
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
函 数 名: pvrfDriverSGLimitVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDriverSGLimitVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    s8 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(s8) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
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
                        if (tempValue >= g_paraLimit.downLimit.sgLimit)    //应该是大于下限的            NICK MARK
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
                    if (tempValue <= g_paraLimit.upLimit.sgLimit)    //应该是小于上限的            NICK MARK
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
函 数 名: pvrfDriverSGLimitVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfDriverSGParaVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex)
{
    u8  tempIndex;
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(s16) + sizeof(u8)) == dataLen)    //参数的长度加上下标的长度
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



/*******************************************文件尾********************************************/
