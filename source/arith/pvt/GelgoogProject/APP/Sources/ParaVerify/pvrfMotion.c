/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfMotion.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#include "project.h"
#include "pvrfMotion.h"



/****************************************外部变量声明*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern MotionInfoStruct g_motionInfo;
extern SystemStateStruct g_systemState;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
#if 0    //移到Advanced命令中去    NICK MARK
/*********************************************************************************************
函 数 名: pvrfMotionEmergEndSpeedVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionEmergEndSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.endSpeed)
        {
            if (tempValue >= g_paraLimit.downLimit.endSpeed)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}
#endif

/*********************************************************************************************
函 数 名: pvrfMotionStateReportVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionStateReportVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(StateReportEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(StateReportEnum *)pParaValue = STATEQRY_ACTIVE;
              break;
              
            case 1:
                *(StateReportEnum *)pParaValue = STATEQRY_QUERY;
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
函 数 名: pvrfMotionStartSrcVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionStartSrcVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(StartSourceEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(StartSourceEnum *)pParaValue = SSRC_SOFT;
              break;

            case 1:
                *(StartSourceEnum *)pParaValue = SSRC_HARD;
                break;

            case 2:
                *(StartSourceEnum *)pParaValue = SSRC_CAN;
              break;

            case 3:
                *(StartSourceEnum *)pParaValue = SSRC_ALL;
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
函 数 名: pvrfMotionStartTypeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionStartTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
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


/*********************************************************************************************
函 数 名: pvrfMotionMaxSpeedVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionMaxSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.motionMaxSpeed)
        {
            if (tempValue > g_motionInfo.motion[CH1].minSpeed)    //要比设定的最小速度大
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfMotionMinSpeedVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionMinSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue < g_motionInfo.motion[CH1].maxSpeed)    //上限是最大速度
        {
            if (tempValue >= g_paraLimit.downLimit.motionMinSpeed)
            {
                *(f32 *)pParaValue = tempValue;
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
函 数 名: pvrfMotionMaxPosnVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionMaxPosnVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.motionMaxPosn)
        {
            if (tempValue > g_motionInfo.motion[CH1].minPosn)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfMotionMinPosnVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionMinPosnVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue < g_motionInfo.motion[CH1].maxPosn)
        {
            if (tempValue >= g_paraLimit.downLimit.motionMinPosn)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfMotionMaxTorqueVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionMaxTorqueVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.motionMaxTorque)
        {
            if (tempValue > g_motionInfo.motion[CH1].minTorque)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfMotionMinTorqueVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionMinTorqueVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue < g_motionInfo.motion[CH1].maxTorque)
        {
            if (tempValue >= g_paraLimit.downLimit.motionMinTorque)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfMotionMaxAccVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionMaxAccVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.motionMaxAcc)
        {
            if (tempValue > g_motionInfo.motion[CH1].minAcc)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfMotionMinAccVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionMinAccVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue < g_motionInfo.motion[CH1].maxAcc)
        {
            if (tempValue >= g_paraLimit.downLimit.motionMinAcc)
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
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
函 数 名: pvrfMotionOffsetStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionOffsetStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
函 数 名: pvrfMotionReverseMotionVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotionReverseMotionVerify(u8 dataLen, u8 *pData, void *pParaValue)
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



/*******************************************文件尾********************************************/
