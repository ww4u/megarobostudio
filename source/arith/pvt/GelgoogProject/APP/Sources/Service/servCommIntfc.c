/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servCommIntfc.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.08.22;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <os.h>
#include "bspCan.h"
#include "bspUart.h"
#include "cmdMainParse.h"
#include "comInterface.h"
#include "servFpga.h"
#include "servSoftTimer.h"
#include "servStreamBuffer.h"
#include "servCommIntfc.h"



/****************************************外部变量声明*****************************************/
extern StreamBufferStruct g_CmdParseBuffer;
extern StreamBufferStruct g_CmdParseBuffer;

extern OS_SEM g_semCmdParseTask;
extern OS_SEM g_semEventManageTask;

extern CommIntfcStruct g_commIntfc;

extern SensorUartStruct g_sensorUart;

extern EventSrcBmpStruct g_eventSrcBmp;



/*****************************************局部宏定义******************************************/
#define    SERV_INTFC_UART_RCV_LEN_MIN    3




/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
StreamBufferStruct g_ciCanRxBuffer;
StreamBufferStruct g_ciCanTxBuffer;
u8  canRxBuffer[480];
u8  canTxBuffer[480];

SoftTimerStruct    g_ciUartDmaRecTimer;
StreamBufferStruct g_ciUartRxBuffer;
StreamBufferStruct g_ciUartTxBuffer;
u8  ciUartRxBuffer[1024];
u8  ciUartTxBuffer[1024];

SoftTimerStruct    g_senUartDmaRecTimer[UARTNUM_RESERVE];
SoftTimerStruct    g_senUartSwitchTimer[UARTNUM_RESERVE];
SensorManageStruct sensorUartRecvInfo[UARTNUM_RESERVE];

StreamBufferStruct g_senUartRxBuffer[UARTNUM_RESERVE][SENSOR_RESERVE];
u8  senUartRxBuffer[UARTNUM_RESERVE][SENSOR_RESERVE][1024];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servCommStreamBufferInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servCommStreamBufferInit(void)
{
    u8 i;

    
    //CAN phy层Buffer初始化
    servStreamBufferInit(&g_ciCanRxBuffer, canRxBuffer, sizeof(canRxBuffer));
    servStreamBufferInit(&g_ciCanTxBuffer, canTxBuffer, sizeof(canTxBuffer));

    //Uart phy层Buffer初始化
    servStreamBufferInit(&g_ciUartRxBuffer, ciUartRxBuffer, sizeof(ciUartRxBuffer));
    servStreamBufferInit(&g_ciUartTxBuffer, ciUartTxBuffer, sizeof(ciUartTxBuffer)); 
    
    //Uart phy层Buffer初始化
    for (i = 0;i < SENSOR_RESERVE;i++)
    {
        servStreamBufferInit(&g_senUartRxBuffer[UARTNUM_U1][i], 
                             senUartRxBuffer[UARTNUM_U1][i], 
                             sizeof(senUartRxBuffer[UARTNUM_U1][i]));
                             
        servStreamBufferInit(&g_senUartRxBuffer[UARTNUM_U2][i], 
                             senUartRxBuffer[UARTNUM_U2][i], 
                             sizeof(senUartRxBuffer[UARTNUM_U2][i]));
    }
}


/*********************************************************************************************
函 数 名: servCiCanFrameSend;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servCiCanFrameSend(u8 dataLen, u8 *pDataBuffer)
{
    u8 sendLen = dataLen + sizeof(CanPhyFrameStruct);
    CanPhyFrameStruct *pCanPhyFrame = NULL;
    

    pCanPhyFrame = (CanPhyFrameStruct *)servReserveMemory(&g_ciCanTxBuffer, sendLen);
    if (NULL != pCanPhyFrame)
    {
        pCanPhyFrame->frameLen = sendLen;
        
        if (dataLen > 0)
        {
            memcpy(&pCanPhyFrame->payload, pDataBuffer, dataLen);
        }

        servEnqueue(&g_ciCanTxBuffer, pCanPhyFrame->frameLen);
    }
}


/*********************************************************************************************
函 数 名: servCiCanRxFrameProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servCiCanRxFrameProcess(void)
{    
    CanPhyFrameStruct   *pCanPhyFrame = NULL;
    CmdParseFrameStruct *pCmdPareFrame = NULL;
    u8 dataLen;

    
    //如果CAN接收缓冲区有数据就处理
    if (g_ciCanRxBuffer.bNotEmpty)
    {   
        pCanPhyFrame = (CanPhyFrameStruct *)servGetHead(&g_ciCanRxBuffer);
        dataLen = pCanPhyFrame->GET_CAN_PHY_FRMAE_DATA_SIZE;

        //只把payload中的数据拷贝出来放入CmdParseBuffer中
        pCmdPareFrame = (CmdParseFrameStruct *)servReserveMemory(&g_CmdParseBuffer, dataLen);
        if (NULL != pCmdPareFrame)
        {
            pCmdPareFrame->mainType = (CmdTypeEnum)pCanPhyFrame->payload[0];
            pCmdPareFrame->subType = pCanPhyFrame->payload[1];
            pCmdPareFrame->dataLen = dataLen - 2;    //CmdParse帧的长度不包括主类型和子类型，所以减2
            memcpy(pCmdPareFrame->payload, &pCanPhyFrame->payload[2], pCmdPareFrame->dataLen);    //拷贝剩余的数据
            servEnqueue(&g_CmdParseBuffer, dataLen);
        }
        //=====================================================================================================

        //关接收中断
        bspCiCanITConfig(DISABLE);
        
        servDequeue(&g_ciCanRxBuffer, pCanPhyFrame->frameLen); //处理完成后出队
        
        //开接收中断
        bspCiCanITConfig(ENABLE);
    }
}


/*********************************************************************************************
函 数 名: servCiCanTxFrameProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servCiCanTxFrameProcess(void)
{    
    CanPhyFrameStruct   *pCanPhyFrame = NULL;
    u8 dataLen;
    u8 canData[8];
    

    //如果有数据要发送
#if 1
    while (g_ciCanTxBuffer.bNotEmpty)    //一次把Buffer中数据发完
#else
    if (g_ciCanTxBuffer.bNotEmpty)
#endif
    {
        pCanPhyFrame = (CanPhyFrameStruct *)servGetHead(&g_ciCanTxBuffer);

        //目前定义的命令集中长度都限制在了8Bytes内，所以不用分帧发，直接发就是了
        dataLen = pCanPhyFrame->GET_CAN_PHY_FRMAE_DATA_SIZE;
        if (dataLen <= CAN_FRAME_BYTES_MAX)
        {
            memcpy(canData, pCanPhyFrame->payload, dataLen);
            bspCiCanSend(pCanPhyFrame->canID, CAN_RTR_DATA, dataLen, canData);
            bspDelayUs(160);    //短暂延时，不然在高速率下发送会丢帧
        }

        servDequeue(&g_ciCanTxBuffer, pCanPhyFrame->frameLen);
    }
}



/*********************************************************************************************
函 数 名: servCiUartDmaRecTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servCiUartDmaRecTimerCB(void *timeOutPara)
{
    bspCiUartDmaOff();
    
    servStimerDelete(&g_ciUartDmaRecTimer);
}


/*********************************************************************************************
函 数 名: servCiUartFrameSend;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servCiUartFrameSend(u8 dataLen, u8 *pDataBuffer)
{
    UartPhyFrameStruct *pUartPhyFrame = NULL;
    UartPhyFrameStruct uartPhyFrame;
    u8 *pPhyFrame;
    u8 *pTempFrame;
    u8 xorValue = UART_START_OF_FRAME;
    u8 headLen = sizeof(UartPhyFrameStruct) - 1;
    u8 i;


    u8 sendLen = dataLen * 2 + sizeof(UartPhyFrameStruct);    //防止转义多出来空间，先申请两倍数据空间
    pPhyFrame = (u8 *)servReserveMemory(&g_ciUartTxBuffer, sendLen);
    if (NULL != pPhyFrame)
    {
        pUartPhyFrame = (UartPhyFrameStruct *)pPhyFrame;

        pPhyFrame[0] = UART_START_OF_FRAME;
        
        uartPhyFrame.frameLen = dataLen + sizeof(UartPhyFrameStruct);
        uartPhyFrame.xorValue = 0;
        pTempFrame = &uartPhyFrame.frameLen;

        //计算帧头校验值，不包括SOF
        for (i = 0;i < headLen;i++)
        {
            xorValue ^= pTempFrame[i];
        }

        //计算数据校验值
        for (i = 0;i < dataLen;i++)
        {
            xorValue ^= pDataBuffer[i];
        }
        uartPhyFrame.xorValue = xorValue;

        //转义帧头
        servEscapeMsg(pPhyFrame + 1, pTempFrame, &headLen);    //加一是跳过SOF

        //转义数据
        servEscapeMsg(pPhyFrame + 1 + headLen, pDataBuffer, &dataLen);    //加一是跳过SOF
        pUartPhyFrame->frameLen = headLen + 1 + dataLen;    //加一是算上SOF
        
        servEnqueue(&g_ciUartTxBuffer, pUartPhyFrame->frameLen);
    }
}

/*********************************************************************************************
函 数 名: servCiUartRxFrameProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servCiUartRxFrameProcess()
{
    UartPhyFrameStruct *pUartPhyFrame = NULL;
    CmdParseFrameStruct *pCmdPareFrame = NULL;
    u8 frameLen;

    
    //如果Usart1接收缓冲区有数据就处理
    if (g_ciUartRxBuffer.bNotEmpty)
    {   
        pUartPhyFrame = (UartPhyFrameStruct *)servGetHead(&g_ciUartRxBuffer);
        frameLen = pUartPhyFrame->frameLen;
        if (servDeescapeMsgAndXorCheck((u8 *)pUartPhyFrame, &frameLen))    //校验反转义，并且返回数据的真实长度                              
        {
            pCmdPareFrame = (CmdParseFrameStruct *)servReserveMemory(&g_CmdParseBuffer,  
                                                                     pUartPhyFrame->GET_UART_PHY_FRMAE_PAYLOAD_SIZE);

            if (NULL != pCmdPareFrame)
            {
                pCmdPareFrame->mainType = (CmdTypeEnum)pUartPhyFrame->payload[0];
                pCmdPareFrame->subType = pUartPhyFrame->payload[1];
                pCmdPareFrame->dataLen = frameLen - sizeof(UartPhyFrameStruct) - 2;    //CmdParse帧的长度不包括主类型和子类型，所以减2
                memcpy(pCmdPareFrame->payload, &pUartPhyFrame->payload[2], pCmdPareFrame->dataLen);    //拷贝剩余的数据
                
                servEnqueue(&g_CmdParseBuffer, pCmdPareFrame->GET_CMD_PARSE_FRMAE_TOTAL_SIZE);
            }
        }

        //关接收中断
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
        
        servDequeue(&g_ciUartRxBuffer, pUartPhyFrame->frameLen); //处理完成后出队
        
        //开接收中断
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    }
}

/*********************************************************************************************
函 数 名: servCiUartTxFrameProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servCiUartTxFrameProcess(void)
{
    UartPhyFrameStruct *pUartPhyFrame = NULL;
    

    //如果发送缓冲区有数据就发送
#if 1
    while (g_ciUartTxBuffer.bNotEmpty)    //一次把Buffer中数据发完
#else
    if (g_ciUartTxBuffer.bNotEmpty)
#endif
    {
        pUartPhyFrame = (UartPhyFrameStruct *)servGetHead(&g_ciUartTxBuffer);

        bspCiUartSend((u8 *)pUartPhyFrame, pUartPhyFrame->frameLen);

        servDequeue(&g_ciUartTxBuffer, pUartPhyFrame->frameLen); //处理完成后出队
    }
}


/*********************************************************************************************
函 数 名: servFrameSend;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFrameSend(CmdTypeEnum cmdMainType, u8 cmdSubType, u8 dataLen, u8 *pData, LinkTypeEnum linkType)
{
    u8 sendLen = dataLen + sizeof(CmdTypeEnum) + sizeof(cmdSubType);
    u8 *pSendBuffer;
    
    switch (linkType)
    {
        case LINK_CAN:
            if (sendLen <= CAN_FRAME_BYTES_MAX)
            {
                pSendBuffer = (u8 *)malloc(sendLen);
                if (pSendBuffer != NULL)
                {
                    pSendBuffer[0] = cmdMainType;
                    pSendBuffer[1] = cmdSubType;
                    memcpy(pSendBuffer + 2, pData, dataLen);
                    servCiCanFrameSend(sendLen, pSendBuffer);
                }
                free(pSendBuffer);
            }
          break;
          
        case LINK_UART:
            if (sendLen <= UART_FRAME_BYTES_BFESCAPE_MAX)
            {
                pSendBuffer = (u8 *)malloc(sendLen);
                if (pSendBuffer != NULL)
                {
                    pSendBuffer[0] = cmdMainType;
                    pSendBuffer[1] = cmdSubType;
                    memcpy(pSendBuffer + 2, pData, dataLen);
                    servCiUartFrameSend(sendLen, pSendBuffer);
                }
                free(pSendBuffer);
            }
          break;

        default:
          break;
    }
}


/*********************************************************************************************
函 数 名: servRxFrameProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servRxFrameProcess(void)
{
    //处理CAN接收数据
    servCiCanRxFrameProcess();
    
    //处理Usart接收数据
    servCiUartRxFrameProcess();
}


/*********************************************************************************************
函 数 名: servTxFrameProcess;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servTxFrameProcess(void)
{
    servCiCanTxFrameProcess();

    servCiUartTxFrameProcess();
}


/*********************************************************************************************
函 数 名: irqCiCanReceive
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqCiCanReceive(void)
{
    OS_ERR os_err;
    CanRxMsg RxMessage;
    u32 canID;
    u8  dataLen;
    u8 *pReceiveBuffer = NULL;
    static CanPhyFrameStruct *pCanPhyFrame = NULL;
    

    bspCiCanReceive(&RxMessage); //接收FIFO0中的数据

    //数据帧才处理
    if (CAN_RTR_DATA == RxMessage.RTR)
    {
        if (CAN_ID_STD == RxMessage.IDE)
        {
            canID = RxMessage.StdId;
        }
        else
        {
            canID = RxMessage.ExtId;
        }
        
        dataLen = sizeof(CanPhyFrameStruct) + RxMessage.DLC;    

        pReceiveBuffer = (u8 *)servFastReserveMemory(&g_ciCanRxBuffer, dataLen);

        //如果缓冲区不为空
        if (NULL != pReceiveBuffer)
        {
            //存入ID和数据
            memcpy(pReceiveBuffer, &canID, sizeof(canID));
            pReceiveBuffer += sizeof(canID);
            *pReceiveBuffer++ = dataLen;
            memcpy(pReceiveBuffer, RxMessage.Data, RxMessage.DLC); 

            pCanPhyFrame = (CanPhyFrameStruct *)servFastGetTail(g_ciCanRxBuffer);          
            servFastEnqueue(&g_ciCanRxBuffer, pCanPhyFrame->frameLen);

            //接收完成后发送信号量
            OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, &os_err);
        }
    }
}


static bool bCiUartFrameHeadRcv = false;        //是否收到帧头
/*********************************************************************************************
函 数 名: irqCiUartReceive
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqCiUartReceive(void)
{
    u8 *pReceiveBuffer = NULL;
    u8 ciUartRcvByte;


    ciUartRcvByte = bspCiUartRecive();
    
    if (UART_START_OF_FRAME == ciUartRcvByte)
    {
        bCiUartFrameHeadRcv = true;
    }
    else
    {
        //如果已经收到帧头，则启动DMA接收数据，并且关闭Uart的中断
        if (bCiUartFrameHeadRcv)
        {
            bCiUartFrameHeadRcv = false;

            //验证下长度是否正确
            if ((ciUartRcvByte > 0) && (ciUartRcvByte < UART_FRAME_BYTES_AFESCAPE_MAX))
            {
                //关闭Uart中断
                bspCiUartITConfig(DISABLE);

                //预留缓冲区
                pReceiveBuffer = (u8 *)servFastReserveMemory(&g_ciUartRxBuffer, ciUartRcvByte);

                //如果缓冲区不为空
                if (NULL != pReceiveBuffer)
                {
                    //存入帧长和数据长度
                    *pReceiveBuffer++ = UART_START_OF_FRAME;
                    //*pReceiveBuffer++ = ciUartRcvByte;
                    
                    //启动DMA
                    bspCiUartDmaRecive(pReceiveBuffer, ciUartRcvByte);

                    //开启DMA接收超时定时器
                    servStimerAdd(&g_ciUartDmaRecTimer);
                }
                else
                {
                    //打开Uart中断
                    bspCiUartITConfig(ENABLE);
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: irqCiUartDmaComplReceive;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqCiUartDmaComplReceive(void)
{ 
    OS_ERR os_err;
    static UartPhyFrameStruct *pUsartPhyFrame = NULL;

    
    //关闭超时定时器
    servStimerDelete(&g_ciUartDmaRecTimer);

    //开启UART接收
    bspCiUartReciveOn();

    //处理下Buffer中的数据
    pUsartPhyFrame = (UartPhyFrameStruct *)servFastGetTail(g_ciUartRxBuffer);
    servFastEnqueue(&g_ciUartRxBuffer, pUsartPhyFrame->frameLen);

    //接收完成后发送信号量
    OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, &os_err);
}


#if 0
#endif


static SensorNumEnum sn1UartCurrNum = SENSOR_S1;
/*********************************************************************************************
函 数 名: irqSensor1UartReceive;
实现功能: 串口接收中断回调函数
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqSensor1UartReceive(void)
{
    u8 *pReceiveBuffer = NULL;
    u8  recByte;


    recByte = bspSensor1UartRecive();
    
    if (sensorUartRecvInfo[UARTNUM_U1].SOF == recByte)
    {
        sensorUartRecvInfo[UARTNUM_U1].recvNum--;
        
        //关闭Uart中断
        bspSensor1UartITConfig(DISABLE);

        //预留缓冲区
        pReceiveBuffer = (u8 *)servFastReserveMemory(&g_senUartRxBuffer[UARTNUM_U1][sn1UartCurrNum], 
                                                     sensorUartRecvInfo[UARTNUM_U1].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U1].frameLen));

        //如果缓冲区不为空
        if (NULL != pReceiveBuffer)
        {
            //存入帧长和数据长度
            *pReceiveBuffer++ = sensorUartRecvInfo[UARTNUM_U1].frameLen;
            //*pReceiveBuffer++ = sensorUartRecvInfo[UARTNUM_U1].SOF;
            //*pReceiveBuffer++ = sn1UartRcvByte;
            
            //启动DMA
            bspSensor1UartDmaRecive(pReceiveBuffer, sensorUartRecvInfo[UARTNUM_U1].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U1].frameLen));

            //开启DMA接收超时定时器
            servStimerAdd(&g_senUartDmaRecTimer[UARTNUM_U1]);
        }
        else
        {
            //打开Uart中断
            bspSensor1UartITConfig(ENABLE);
        }
    }
}


/*********************************************************************************************
函 数 名: irqSensor1UartDmaComplReceive;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqSensor1UartDmaComplReceive(void)
{ 
    SensorUartPhyFrameStruct *pUsartPhyFrame = NULL;


    if (!bspSensor1UartDmaCounterGet())
    {
        //关闭超时定时器
        servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U1]);

        //处理下Buffer中的数据
        pUsartPhyFrame = (SensorUartPhyFrameStruct *)servFastGetTail(g_senUartRxBuffer[UARTNUM_U1][sn1UartCurrNum]);
        servFastEnqueue(&g_senUartRxBuffer[UARTNUM_U1][sn1UartCurrNum], pUsartPhyFrame->frameLen);
        g_senUartRxBuffer[UARTNUM_U1][sn1UartCurrNum].frameCount++;

        //如果recvNum为0了就不再接收数据，直到切换接收后或者下个周期才开启UART接收
        if (sensorUartRecvInfo[UARTNUM_U1].recvNum > 0)
        {
            //开启UART接收
            bspSensor1UartReciveOn();
        }
        else
        {
            bspSensor1UartReciveOff();

            //接收完成后发送信号量
            g_eventSrcBmp.bSenUart[UARTNUM_U1][sn1UartCurrNum] = true;
            OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, NULL);
        }
    }
}


/*********************************************************************************************
函 数 名: servSensor1UartDmaRecTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servSensor1UartDmaRecTimerCB(void *timeOutPara)
{
    //SensorNumEnum sensorNum = *(SensorNumEnum *)timeOutPara;

    bspSensor1UartDmaOff();

    servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U1]);
}


/*********************************************************************************************
函 数 名: servSensor1UartSwitchTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servSensor1UartSwitchTimerCB(void *timeOutPara)
{
    u8  i, temp;
    //SensorNumEnum sensorNum = *(SensorNumEnum *)timeOutPara;
    
    
    //切换端口
    temp = sn1UartCurrNum;
    for (i = 0;i < SENSOR_RESERVE;i++)
    {
        temp += 1;
        if (temp >= SENSOR_RESERVE)
        {
            temp = SENSOR_S1;
        }

        if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U1][temp].state)
        {
            //sn1UartCurrNum  = (SensorNumEnum)temp;
            sensorUartRecvInfo[UARTNUM_U1] = g_sensorUart.sensor[UARTNUM_U1][temp];
            break;
        }
    }

    if (sn1UartCurrNum != (SensorNumEnum)temp)
    {
        sn1UartCurrNum = (SensorNumEnum)temp;
    
        servFpgaSensorUart1RxSelect(sn1UartCurrNum);

        //重新设计定时器超时时间
        servStimerInit(&g_senUartSwitchTimer[UARTNUM_U1], sensorUartRecvInfo[UARTNUM_U1].swTime, servSensor1UartSwitchTimerCB); 
        servStimerAdd(&g_senUartSwitchTimer[UARTNUM_U1]);
    }
    
    bspSensor1UartReciveOn();
    
    if (g_senUartSwitchTimer[UARTNUM_U1].bUsed)
    {
        servStimerAdd(&g_senUartSwitchTimer[UARTNUM_U1]);
    }
}


/*********************************************************************************************
函 数 名: servSensor1UartReciveOff;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servSensor1UartReciveOff(void)
{
    servStimerDelete(&g_senUartSwitchTimer[UARTNUM_U1]);

    bspSensor1UartReciveOff();
}


/*********************************************************************************************
函 数 名: servSensor1UartReciveOn;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servSensor1UartReciveOn(SensorManageStruct sensor, SensorNumEnum sensorNum)
{
    if (false == g_senUartSwitchTimer[UARTNUM_U1].bUsed)
    {
        sn1UartCurrNum = sensorNum;
        sensorUartRecvInfo[UARTNUM_U1] = sensor;

        servFpgaSensorUart1RxSelect(sn1UartCurrNum); 
        
        servStimerInit(&g_senUartSwitchTimer[UARTNUM_U1], sensor.swTime, servSensor1UartSwitchTimerCB); 
        servStimerAdd(&g_senUartSwitchTimer[UARTNUM_U1]);

        bspSensor1UartReciveOn();
    }
}



#if !UART_SN2_UART_USE_DMA
static u8  *pReceiveBuffer = NULL;
static u8   recByteCount;
static bool bsn2UartFrameHeadRcv = false;        //是否收到帧头
#endif

static SensorNumEnum sn2UartCurrNum = SENSOR_S1; 
/*********************************************************************************************
函 数 名: irqSensor2UartReceive;
实现功能: 串口接收中断回调函数
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqSensor2UartReceive(void)
{
#if UART_SN2_UART_USE_DMA

    u8 *pReceiveBuffer = NULL;
    u8  recByte;
    

    recByte = bspSensor2UartRecive();
    
    if (sensorUartRecvInfo[UARTNUM_U2].SOF == recByte)
    {
        sensorUartRecvInfo[UARTNUM_U2].recvNum--;
        
        //关闭Uart中断
        bspSensor2UartITConfig(DISABLE);

        //预留缓冲区
        pReceiveBuffer = (u8 *)servFastReserveMemory(&g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum], 
                                                     sensorUartRecvInfo[UARTNUM_U2].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U2].frameLen));

        //如果缓冲区不为空
        if (NULL != pReceiveBuffer)
        {
            //存入帧长和数据长度
            *pReceiveBuffer++ = sensorUartRecvInfo[UARTNUM_U2].frameLen;
            //*pReceiveBuffer++ = sensorUartRecvInfo[UARTNUM_U2].SOF;
            //*pReceiveBuffer++ = sn2UartRcvByte;
            
            //启动DMA
            bspSensor2UartDmaRecive(pReceiveBuffer, sensorUartRecvInfo[UARTNUM_U2].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U2].frameLen));

            //开启DMA接收超时定时器
            servStimerAdd(&g_senUartDmaRecTimer[UARTNUM_U2]);
        }
        else
        {
            //打开Uart中断
            bspSensor2UartITConfig(ENABLE);
        }
    }

#else

    SensorUartPhyFrameStruct *pUsartPhyFrame = NULL;
    u8  recByte;
    u8  frameLen;


    recByte = bspSensor2UartRecive();
    
    if ((sensorUartRecvInfo[UARTNUM_U2].SOF == recByte) && (!bsn2UartFrameHeadRcv))
    {
        bsn2UartFrameHeadRcv = true;

        //预留缓冲区
        frameLen = sensorUartRecvInfo[UARTNUM_U2].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U2].frameLen);
        pReceiveBuffer = (u8 *)servFastReserveMemory(&g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum], frameLen);

        if (NULL != pReceiveBuffer)
        {
            //存入帧长和数据长度
            *pReceiveBuffer++ = frameLen;
            *pReceiveBuffer++ = recByte;
            recByteCount++;

            //开启接收超时定时器
            servStimerAdd(&g_senUartDmaRecTimer[UARTNUM_U2]);
        }
        else
        {
            bsn2UartFrameHeadRcv = false;
        }
    }
    else
    {
        //已经接收到帧头
        if (bsn2UartFrameHeadRcv)
        {
            *pReceiveBuffer++ = recByte;
            recByteCount++;

            if (recByteCount == sensorUartRecvInfo[UARTNUM_U2].frameLen)    //接收完一帧
            {
                sensorUartRecvInfo[UARTNUM_U2].recvNum--;

                //关闭超时定时器
                servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U2]);

                //处理Buffer中的数据
                pUsartPhyFrame = (SensorUartPhyFrameStruct *)servFastGetTail(g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum]);
                servFastEnqueue(&g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum], pUsartPhyFrame->frameLen);
                g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum].frameCount++;
                
                bsn2UartFrameHeadRcv = false;
                recByteCount = 0;

                //如果recvNum为0了就不再接收数据，直到切换接收后或者下个周期才开启UART接收
                if (sensorUartRecvInfo[UARTNUM_U2].recvNum > 0)
                {
                    //开启UART接收
                    bspSensor2UartReciveOn();
                }
                else
                {
                    bspSensor2UartReciveOff();

                    //接收完成后发送信号量
                    g_eventSrcBmp.bSenUart[UARTNUM_U2][sn2UartCurrNum] = true;
                    OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, NULL);
                }
            }
        }
    }
    
#endif
}



/*********************************************************************************************
函 数 名: irqSensor2UartDmaComplReceive;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void irqSensor2UartDmaComplReceive(void)
{ 
    SensorUartPhyFrameStruct *pUsartPhyFrame = NULL;

    
    if (!bspSensor2UartDmaCounterGet())
    {
        //关闭超时定时器
        servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U2]);

        //处理下Buffer中的数据
        pUsartPhyFrame = (SensorUartPhyFrameStruct *)servFastGetTail(g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum]);
        servFastEnqueue(&g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum], pUsartPhyFrame->frameLen);
        g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum].frameCount++;

        //如果recvNum为0了就不再接收数据，直到切换接收后或者下个周期才开启UART接收
        if (sensorUartRecvInfo[UARTNUM_U2].recvNum > 0)
        {
            //开启UART接收
            bspSensor2UartReciveOn();
        }
        else
        {
            bspSensor2UartReciveOff();

            //接收完成后发送信号量
            g_eventSrcBmp.bSenUart[UARTNUM_U2][sn2UartCurrNum] = true;
            OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, NULL);
        }
    }
}


/*********************************************************************************************
函 数 名: servSensor2UartDmaRecTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servSensor2UartDmaRecTimerCB(void *timeOutPara)
{
    //SensorNumEnum sensorNum = *(SensorNumEnum *)timeOutPara;
#if UART_SN2_UART_USE_DMA

    bspSensor2UartDmaOff();

#else

    bsn2UartFrameHeadRcv = false;
    pReceiveBuffer = NULL;
    recByteCount = 0;
    USART_ClearFlag(USART3, USART_FLAG_CTS  | 
                            USART_FLAG_LBD  | 
                            USART_FLAG_TXE  | 
                            USART_FLAG_TC   | 
                            USART_FLAG_RXNE | 
                            USART_FLAG_IDLE | 
                            USART_FLAG_ORE  | 
                            USART_FLAG_NE   | 
                            USART_FLAG_FE   | 
                            USART_FLAG_PE);
#endif

    servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U2]);
}


/*********************************************************************************************
函 数 名: servSensor2UartSwitchTimerCB;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servSensor2UartSwitchTimerCB(void *timeOutPara)
{
    u8  i, temp;
    //SensorNumEnum sensorNum = *(SensorNumEnum *)timeOutPara;
    
    
    //切换端口
    temp = sn2UartCurrNum;
    for (i = 0;i < SENSOR_RESERVE;i++)
    {
        temp += 1;
        if (temp >= SENSOR_RESERVE)
        {
            temp = SENSOR_S1;
        }

        //下一个Sensor的状态是开启则不再看后面的Sensor状态
        if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][temp].state)
        {
            //sn2UartCurrNum  = (SensorNumEnum)temp;
            sensorUartRecvInfo[UARTNUM_U2] = g_sensorUart.sensor[UARTNUM_U2][temp];
            break;
        }
    }

    if (sn2UartCurrNum != (SensorNumEnum)temp)
    {
        sn2UartCurrNum  = (SensorNumEnum)temp;
        
        servFpgaSensorUart2RxSelect(sn2UartCurrNum);

        //重新设计定时器超时时间
        servStimerInit(&g_senUartSwitchTimer[UARTNUM_U2], sensorUartRecvInfo[UARTNUM_U2].swTime, servSensor2UartSwitchTimerCB); 
        servStimerAdd(&g_senUartSwitchTimer[UARTNUM_U2]);
    }
    
    bspSensor2UartReciveOn();

#if GELGOOG_SINANJU
    //绝对值编码器时需要发送查询命令    CJ 2018.02.28 Add
    servFpgaAbsEncReadTrig(sn2UartCurrNum);
#endif

    if (g_senUartSwitchTimer[UARTNUM_U2].bUsed)
    {
        servStimerAdd(&g_senUartSwitchTimer[UARTNUM_U2]);
    }
}


/*********************************************************************************************
函 数 名: servSensor2UartReciveOff;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servSensor2UartReciveOff(void)
{
    servStimerDelete(&g_senUartSwitchTimer[UARTNUM_U2]);

    bspSensor2UartReciveOff();
    servFpgaSensorUart2RxEnable(false);
}


/*********************************************************************************************
函 数 名: servSensor2UartReciveOn;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servSensor2UartReciveOn(SensorManageStruct sensor, SensorNumEnum sensorNum)
{
    if (false == g_senUartSwitchTimer[UARTNUM_U2].bUsed)
    {
        sn2UartCurrNum = sensorNum;
        sensorUartRecvInfo[UARTNUM_U2] = sensor;
  
        servFpgaSensorUart2RxEnable(true);
        servFpgaSensorUart2RxSelect(sn2UartCurrNum);
        
        servStimerInit(&g_senUartSwitchTimer[UARTNUM_U2], sensor.swTime, servSensor2UartSwitchTimerCB); 
        servStimerAdd(&g_senUartSwitchTimer[UARTNUM_U2]);

        bspSensor2UartReciveOn();

#if GELGOOG_SINANJU
        //绝对值编码器时需要发送查询命令    CJ 2018.02.28 Add
        servFpgaAbsEncReadTrig(sn2UartCurrNum);
#endif
    }
}



/*******************************************文件尾********************************************/
