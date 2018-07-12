/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfInterface.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#include "pvrfInterface.h"



/****************************************外部变量声明*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/ 



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvrfUartBaudVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfUartBaudVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(UartBaudEnum *)pParaValue = UARTBAUD_4800;
              break;
    
            case 1:
                *(UartBaudEnum *)pParaValue = UARTBAUD_7200;
              break;  
    
            case 2:
                *(UartBaudEnum *)pParaValue = UARTBAUD_9600;
              break; 
    
            case 3:
                *(UartBaudEnum *)pParaValue = UARTBAUD_14400;
              break; 
    
            case 4:
                *(UartBaudEnum *)pParaValue = UARTBAUD_19200;
              break;  
    
            case 5:
                *(UartBaudEnum *)pParaValue = UARTBAUD_38400;
              break;  
    
            case 6:
                *(UartBaudEnum *)pParaValue = UARTBAUD_57600;
              break;  
    
            case 7:
                *(UartBaudEnum *)pParaValue = UARTBAUD_115200;
              break;  
    
            case 8:
                *(UartBaudEnum *)pParaValue = UARTBAUD_128000;
              break;    
    
            case 9:
                *(UartBaudEnum *)pParaValue = UARTBAUD_2000000;
              break;    
    
            case 10:
                *(UartBaudEnum *)pParaValue = UARTBAUD_2500000;
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
函 数 名: pvrfUartWordLenVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfUartWordLenVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(WordLenEnum *)pParaValue = WORDLEN_8;
              break;
    
            case 1:
                *(WordLenEnum *)pParaValue = WORDLEN_9;
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
函 数 名: pvrfUartFlowCtlVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfUartFlowCtlVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(FlowCtlEnum *)pParaValue = FLOWCTL_NONE;
              break;
    
            case 1:
                *(FlowCtlEnum *)pParaValue = FLOWCTL_RTS;
              break;  
    
            case 2:
                *(FlowCtlEnum *)pParaValue = FLOWCTL_CTS;
              break; 
    
            case 3:
                *(FlowCtlEnum *)pParaValue = FLOWCTL_RTS_CTS;
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
函 数 名: pvrfUartParityVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfUartParityVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(ParityEnum *)pParaValue = PARITY_NONE;
              break;
    
            case 1:
                *(ParityEnum *)pParaValue = PARITY_EVEN;
              break;  
    
            case 2:
                *(ParityEnum *)pParaValue = PARITY_ODD;
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
函 数 名: pvrfUartStopBitVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfUartStopBitVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(StopBitEnum *)pParaValue = STOPBIT_10;
              break;
    
            case 1:
                *(StopBitEnum *)pParaValue = STOPBIT_05;
              break;  
    
            case 2:
                *(StopBitEnum *)pParaValue = STOPBIT_20;
              break;  
    
            case 3:
                *(StopBitEnum *)pParaValue = STOPBIT_15;
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
函 数 名: pvrfCanTypeVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfCanTypeVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(CanTypeEnum *)pParaValue = CANTYPE_A;
              break;
    
            case 1:
                *(CanTypeEnum *)pParaValue = CANTYPE_B;
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
函 数 名: pvrfCanSendIdVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfCanSendIdVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8  verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //长度先要正确
    {
        tempValue = *(u32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.canSendId)
        {
            if (tempValue >= g_paraLimit.downLimit.canSendId)
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
函 数 名: pvrfCanBaudVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfCanBaudVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        switch (tempValue)
        {
            case 0:
                *(CanBaudEnum *)pParaValue = CANBAUD_1000;
              break;
    
            case 1:
                *(CanBaudEnum *)pParaValue = CANBAUD_500;
              break; 
    
            case 2:
                *(CanBaudEnum *)pParaValue = CANBAUD_250;
              break; 
    
            case 3:
                *(CanBaudEnum *)pParaValue = CANBAUD_125;
              break; 
    
            case 4:
                *(CanBaudEnum *)pParaValue = CANBAUD_100;
              break; 
    
            case 5:
                *(CanBaudEnum *)pParaValue = CANBAUD_50;
              break; 
    
            case 6:
                *(CanBaudEnum *)pParaValue = CANBAUD_20;
              break; 
    
            case 7:
                *(CanBaudEnum *)pParaValue = CANBAUD_10;
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
函 数 名: pvrfCanGroupVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfCanGroupVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u8) == dataLen)    //长度先要正确
    {
        tempValue = *pData;
        if (tempValue <= g_paraLimit.upLimit.canGroup)
        {
            if (tempValue >= g_paraLimit.downLimit.canGroup)
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
函 数 名: pvrfCanReceiveIdVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfCanReceiveIdVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //长度先要正确
    {
        tempValue = *(u32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.canReceiveId)
        {
            if (tempValue >= g_paraLimit.downLimit.canReceiveId)
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
函 数 名: pvrfCanGroupIdVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfCanGroupIdVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //长度先要正确
    {
        tempValue = *(u32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.canGroupId)
        {
            if (tempValue >= g_paraLimit.downLimit.canGroupId)
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
函 数 名: pvrfCanRadioIdVerify;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvrfCanRadioIdVerify(u8 dataLen, u8 *pData, void *pParaValue)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(u32) == dataLen)    //长度先要正确
    {
        tempValue = *(u32 *)pData;
        if (tempValue <= g_paraLimit.upLimit.canRadioId)
        {
            if (tempValue >= g_paraLimit.downLimit.canRadioId)
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



/*******************************************文件尾********************************************/
