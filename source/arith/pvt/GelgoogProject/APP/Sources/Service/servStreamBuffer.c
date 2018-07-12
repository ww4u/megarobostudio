/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servStreamBuffer.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.11.14;
历史版本:  无;
*********************************************************************************************/
#include "servStreamBuffer.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servStreamBufferInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servStreamBufferInit(StreamBufferStruct *pStreamBuffer, u8* pBuffer, u16 bufferSize)
{
    pStreamBuffer->bufferSize = bufferSize;
    pStreamBuffer->headOffset = 0;
    pStreamBuffer->tailOffset = 0;
    pStreamBuffer->reserveMemorySize = 0;
    pStreamBuffer->bRewind = false;
    pStreamBuffer->pBuffer = pBuffer;
}


/*********************************************************************************************
函 数 名: servGetHead;
实现功能: 获取第一包数据的头部; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 *servGetHead(StreamBufferStruct *pStreamBuffer)
{
    return pStreamBuffer->pBuffer + pStreamBuffer->headOffset;
}


/*********************************************************************************************
函 数 名: ReserveMemory;
实现功能: 申请一个保留空间给外部使用; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 *servReserveMemory(StreamBufferStruct *pStreamBuffer, u16 bufferSize)
{
    pStreamBuffer->reserveMemorySize = bufferSize;
    if ( (pStreamBuffer->bufferSize - pStreamBuffer->tailOffset >= bufferSize) )
    {
        if (pStreamBuffer->bRewind)
        {
            if (pStreamBuffer->tailOffset + bufferSize > pStreamBuffer->headOffset)
            {
                pStreamBuffer->reserveMemorySize = 0;
                return NULL;
            }
        }
        return (pStreamBuffer->pBuffer + pStreamBuffer->tailOffset);  
    }
    else //尾部空间浪费一些，不用了，这样处理简单
    {                
        pStreamBuffer->tailOffset = 0;
        pStreamBuffer->bRewind = true;
        if (pStreamBuffer->bNotEmpty)
        {
            if (pStreamBuffer->tailOffset + bufferSize > pStreamBuffer->headOffset)
            {
                pStreamBuffer->reserveMemorySize = 0;
                return NULL;
            }
        }
        return pStreamBuffer->pBuffer;
    }
}


/*********************************************************************************************
函 数 名: Enqueue;
实现功能: 将数据压入队列，这里只是确认一下长度，数据实际已经进去; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servEnqueue(StreamBufferStruct *pStreamBuffer, u16 length)
{   
    if (!pStreamBuffer->bNotEmpty)
    {	
        pStreamBuffer->headOffset = pStreamBuffer->tailOffset;
        pStreamBuffer->dataTailOffset = pStreamBuffer->headOffset;	
    }
    
    pStreamBuffer->tailOffset += length;
  
    if (pStreamBuffer->dataTailOffset < pStreamBuffer->tailOffset)
    {
        pStreamBuffer->dataTailOffset = pStreamBuffer->tailOffset;
    }
    
    pStreamBuffer->bNotEmpty= true;
    pStreamBuffer->reserveMemorySize = 0;

    if (pStreamBuffer->tailOffset >= pStreamBuffer->headOffset)
    {
        if ((pStreamBuffer->tailOffset - pStreamBuffer->headOffset) > (pStreamBuffer->bufferSize >> 1))
        {
            pStreamBuffer->bAlmostFull = true;
        }
        else
        {
            pStreamBuffer->bAlmostFull = false;
        }
    }
    else
    {
        if ((pStreamBuffer->dataTailOffset - pStreamBuffer->headOffset + pStreamBuffer->tailOffset) > (pStreamBuffer->bufferSize >> 1))
        {
            pStreamBuffer->bAlmostFull = true;  
        }
        else
        {
            pStreamBuffer->bAlmostFull = false; 
        }
    }
}


/*********************************************************************************************
函 数 名: Dequeue;
实现功能: 出队，相当于丢弃头上指定长度的数据; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servDequeue(StreamBufferStruct *pStreamBuffer, u16 length)
{   
    if ((pStreamBuffer->headOffset + length) >= pStreamBuffer->dataTailOffset)
    {        
        if (pStreamBuffer->reserveMemorySize > 0)
        {           
            pStreamBuffer->headOffset = pStreamBuffer->tailOffset;            
            pStreamBuffer->dataTailOffset = pStreamBuffer->tailOffset;
            pStreamBuffer->bNotEmpty= false; 
        }
        else
        {
            pStreamBuffer->headOffset = 0;
            if (pStreamBuffer->tailOffset == pStreamBuffer->dataTailOffset)
            {     
                pStreamBuffer->tailOffset = 0;
            }

            pStreamBuffer->dataTailOffset = pStreamBuffer->tailOffset;
            if (pStreamBuffer->tailOffset == 0)
            {    
                pStreamBuffer->bNotEmpty= false;
            }
        }
        
        pStreamBuffer->bRewind= false;
    }
    else
    {  
        pStreamBuffer->headOffset += length;
    }
    
    //更新缓冲区快满标记

    if (pStreamBuffer->tailOffset >= pStreamBuffer->headOffset)
    {    
        if ((pStreamBuffer->tailOffset - pStreamBuffer->headOffset) > (pStreamBuffer->bufferSize >> 1))
        {
            pStreamBuffer->bAlmostFull = true;
        }
        else
        {
            pStreamBuffer->bAlmostFull = false;
        }
    }
    else
    {    
        if ((pStreamBuffer->dataTailOffset - pStreamBuffer->headOffset + pStreamBuffer->tailOffset) > (pStreamBuffer->bufferSize >> 1))
        {
            pStreamBuffer->bAlmostFull = true;
        }
        else
        {
            pStreamBuffer->bAlmostFull = false;
        }
    }
}


/*********************************************************************************************
函 数 名: servFastReserveMemory;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void *servFastReserveMemory(StreamBufferStruct *pStreamBuffer, u16 bufferSize)
{
    void *pBuffer;

    
    pStreamBuffer->reserveMemorySize = bufferSize;
    if ((pStreamBuffer->bufferSize - pStreamBuffer->tailOffset) >= bufferSize)
    {
        if (pStreamBuffer->bRewind)
        {
            if ((pStreamBuffer->tailOffset + bufferSize) > pStreamBuffer->headOffset)
            {
                pStreamBuffer->reserveMemorySize = 0;
                pBuffer = NULL;
            }
            else 
            {
                pBuffer = pStreamBuffer->pBuffer + pStreamBuffer->tailOffset;
            }
        }
        else 
        {
            pBuffer = pStreamBuffer->pBuffer + pStreamBuffer->tailOffset;
        }
    }
    else
    {
        pStreamBuffer->tailOffset = 0;
        pStreamBuffer->bRewind = true;
        if (pStreamBuffer->bNotEmpty)
        {
            if ((pStreamBuffer->tailOffset + bufferSize) > pStreamBuffer->headOffset)
            {
                pStreamBuffer->reserveMemorySize = 0;
                pBuffer = NULL;
            }
            else
            {
                pBuffer = pStreamBuffer->pBuffer;
            }
        }
        else
        {
            pBuffer = pStreamBuffer->pBuffer;
        }
    }

    return pBuffer;
}


/*********************************************************************************************
函 数 名: servFastGetTail;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void *servFastGetTail(StreamBufferStruct streamBuffer)
{
    return (streamBuffer.pBuffer + streamBuffer.tailOffset);
}


/*********************************************************************************************
函 数 名: servFastEnqueue;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFastEnqueue(StreamBufferStruct *pStreamBuffer, u16 bufferSize)
{
    pStreamBuffer->tailOffset += bufferSize;
    if (pStreamBuffer->dataTailOffset < pStreamBuffer->tailOffset)
    {
        pStreamBuffer->dataTailOffset = pStreamBuffer->tailOffset;
    }
    pStreamBuffer->bNotEmpty = true;
    pStreamBuffer->reserveMemorySize = 0; 
}



/*******************************************文件尾********************************************/
