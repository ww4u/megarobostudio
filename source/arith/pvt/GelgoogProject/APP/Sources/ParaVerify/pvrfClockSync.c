/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfClockSync.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.04.12;
历史版本:  无;
*********************************************************************************************/
#include "pvrfClockSync.h"



/****************************************外部变量声明*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfClockFrequencyVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfClockFrequencyVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //长度先要正确
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
函 数 名: pvrfClockSyncVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfClockSyncRegVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ClockSyncRegEnum) == dataLen)    //长度先要正确
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
函 数 名: pvrfClockSyncStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfClockSyncStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ClockSyncStateEnum) == dataLen)    //长度先要正确
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
函 数 名: pvrfClockSyncStartTypeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfClockSyncStartTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(ReceiveTypeEnum) == dataLen)    //长度先要正确
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



/*******************************************文件尾********************************************/
