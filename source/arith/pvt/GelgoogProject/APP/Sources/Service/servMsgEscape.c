/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servMsgEscape.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.11.16;
历史版本:  无;
*********************************************************************************************/
#include "servMsgEscape.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servEscapeMsg;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servEscapeMsg(u8 *pDestBuffer, u8 *pSrcBuffer, u8 *pLength)
{
    u8 i, j, u8Length = *pLength;    //j是新消息长度缓冲变量
    u8 receiveByte;

    for (i = 0,j = 0; i < u8Length; i++) 
    {
        receiveByte = pSrcBuffer[i];

        if (receiveByte == FE_BEFORE_ESCAPE) //如果遇到0xFE
        {
            pDestBuffer[j++] = FEFD_AFTER_ESCAPE_H;
            pDestBuffer[j++] = FE_AFTER_ESCAPE_L;

        }
        else if (receiveByte == FD_BEFORE_ESCAPE) //如果遇到0xFD
        {
            pDestBuffer[j++] = FEFD_AFTER_ESCAPE_H;
            pDestBuffer[j++] = FD_AFTER_ESCAPE_L;
        }
        else
        {
            pDestBuffer[j++] = receiveByte;
        }
    }
    *pLength = j;
}


/*********************************************************************************************
函 数 名: servDeescapeMsgAndXorCheck;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
bool servDeescapeMsgAndXorCheck(u8 *pSrcBuffer, u8 *plength)
{
    u8 i, j;
    u8 xorLen = *plength;
    u8 receiveByte, fcs = 0;

    for (i = 0, j = 0; i < xorLen; i++)    //j是新消息长度缓冲变量
    {
        receiveByte = pSrcBuffer[i];

        if (receiveByte == FEFD_AFTER_ESCAPE_H)
        {
            i++;
            if (pSrcBuffer[i] == FE_AFTER_ESCAPE_L)
            {
                receiveByte = FE_BEFORE_ESCAPE;
            }
            else if (pSrcBuffer[i] == FD_AFTER_ESCAPE_L)
            {
                receiveByte = FD_BEFORE_ESCAPE;
            }
            else
            {
                return false;
            }
        }

        fcs ^= receiveByte;
        pSrcBuffer[j++] = receiveByte;    //将转义之后的值赋回原消息
    }

    *plength = j;
    fcs ^= xorLen ^ j;    //将之前异或的长度消除，然后再和新的长度异或
    
    return (fcs == 0);
}



/*******************************************文件尾********************************************/