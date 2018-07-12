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
#include "pvrfSystem.h"



/****************************************外部变量声明*****************************************/
extern SystemStateStruct g_systemState;



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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

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

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

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
u8 pvrfSystemReptSwitchVerify(u8 dataLen, u8 *pData, void *pParaValue)
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
