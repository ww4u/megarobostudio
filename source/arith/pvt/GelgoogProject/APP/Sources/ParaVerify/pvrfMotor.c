/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfMotor.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#include "pvrfMotor.h"



/****************************************外部变量声明*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************局部宏定义******************************************/
#define    MOTOR_MICRO_STEPS_INDEX_NUM    9
#define    MOTOR_STEPS_INDEX_NUM          9
#define    MOTOR_TYPE_INDEX_NUM           2



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfMotorGearRatioVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorGearRatioVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u16 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u16) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        if (tempValue <= g_paraLimit.upLimit.motorGearRatio)
        {
            if (tempValue >= g_paraLimit.downLimit.motorGearRatio)
            {
                *(u16 *)pParaValue = tempValue;
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
函 数 名: pvrfMotorStepAngleVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorStepAngleVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(StepAngleEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(StepAngleEnum *)pParaValue = STEPANGLE_18;
              break;
    
            case 1:
                *(StepAngleEnum *)pParaValue = STEPANGLE_09;
              break;
    
            case 2:
                *(StepAngleEnum *)pParaValue = STEPANGLE_150;
              break;
    
            case 3:
                *(StepAngleEnum *)pParaValue = STEPANGLE_75;
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
函 数 名: pvrfMotorPeakSpeedVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorPeakSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.motorPeakSpeed)
        {
            if (tempValue >= g_paraLimit.downLimit.motorPeakSpeed)
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
函 数 名: pvrfMotorPeakAccVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorPeakAccVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.motorPeakAcc)
        {
            if (tempValue >= g_paraLimit.downLimit.motorPeakAcc)
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
函 数 名: pvrfMotorTypeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(MotorTypeEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(MotorTypeEnum *)pParaValue = MOTOR_ROTARY;
              break;
    
            case 1:
                *(MotorTypeEnum *)pParaValue = MOTOR_LINEAR;
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
函 数 名: pvrfMotorPosnUnitVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorPosnUnitVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(PostUnitEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(PostUnitEnum *)pParaValue = POSTUNIT_ANGLE;
              break;
              
            case 1:
                *(PostUnitEnum *)pParaValue = POSTUNIT_RADIAN;
              break;
              
            case 2:
                *(PostUnitEnum *)pParaValue = POSTUNIT_MMETER;
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
函 数 名: pvrfMotorSizeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorSizeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(MotorSizeEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(MotorSizeEnum *)pParaValue = MOTOR_08;
              break;
    
            case 1:
                *(MotorSizeEnum *)pParaValue = MOTOR_11;
              break;
    
            case 2:
                *(MotorSizeEnum *)pParaValue = MOTOR_14;
              break;
    
            case 3:
                *(MotorSizeEnum *)pParaValue = MOTOR_17;
              break;
    
            case 4:
                *(MotorSizeEnum *)pParaValue = MOTOR_23;
              break;
    
            case 5:
                *(MotorSizeEnum *)pParaValue = MOTOR_24;
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
函 数 名: pvrfMotorLeadVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorLeadVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.motorLead)
        {
            if (tempValue >= g_paraLimit.downLimit.motorLead)
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
函 数 名: pvrfMotorVoltVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorVoltVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        if (tempValue <= g_paraLimit.upLimit.motorVolt)
        {
            if (tempValue >= g_paraLimit.downLimit.motorVolt)
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
函 数 名: pvrfMotorCurrVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorCurrVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
函 数 名: pvrfMotorBacklashVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfMotorBacklashVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) == dataLen)    //长度先要正确
    {
        tempValue = *(f32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.motorBacklash)
        {
            if (tempValue >= g_paraLimit.downLimit.motorBacklash)
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
函 数 名: pvrfEncoderLineNumVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfEncoderLineNumVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //长度先要正确
    {
        tempValue = *(u32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.encoderLineNum)
        {
            if (tempValue >= g_paraLimit.downLimit.encoderLineNum)
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
函 数 名: pvrfEncoderChanNumVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfEncoderChanNumVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(EncoderChanEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(EncoderChanEnum *)pParaValue = ECCHAN_1;
              break;
    
            case 1:
                *(EncoderChanEnum *)pParaValue = ECCHAN_3;
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
函 数 名: pvrfEncoderTypeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfEncoderTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(EncoderTypeEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(EncoderTypeEnum *)pParaValue = ENCODER_INC;
              break;
    
            case 1:
                *(EncoderTypeEnum *)pParaValue = ENCODER_ABS;
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
函 数 名: pvrfEncoderMultipleVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfEncoderMultipleVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(EncoderMultiEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(EncoderMultiEnum *)pParaValue = ENCMULT_SINGLE;
              break;
              
            case 1:
                *(EncoderMultiEnum *)pParaValue = ENCMULT_DOUBLE;
              break;
    
            case 2:
                *(EncoderMultiEnum *)pParaValue = ENCMULT_QUADR;
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
函 数 名: pvrfEncoderStateVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfEncoderStateVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(IntfcStateEnum) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(IntfcStateEnum *)pParaValue = INTFC_NONE;
              break;
    
            case 1:
                *(IntfcStateEnum *)pParaValue = INTFC_OFF;
              break;
    
            case 2:
                *(IntfcStateEnum *)pParaValue = INTFC_ON;
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
