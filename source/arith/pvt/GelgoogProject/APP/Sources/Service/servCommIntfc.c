/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servCommIntfc.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.08.22;
��ʷ�汾:  ��;
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



/****************************************�ⲿ��������*****************************************/
extern StreamBufferStruct g_CmdParseBuffer;
extern StreamBufferStruct g_CmdParseBuffer;

extern OS_SEM g_semCmdParseTask;
extern OS_SEM g_semEventManageTask;

extern CommIntfcStruct g_commIntfc;

extern SensorUartStruct g_sensorUart;

extern EventSrcBmpStruct g_eventSrcBmp;



/*****************************************�ֲ��궨��******************************************/
#define    SERV_INTFC_UART_RCV_LEN_MIN    3




/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
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



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: servCommStreamBufferInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servCommStreamBufferInit(void)
{
    u8 i;

    
    //CAN phy��Buffer��ʼ��
    servStreamBufferInit(&g_ciCanRxBuffer, canRxBuffer, sizeof(canRxBuffer));
    servStreamBufferInit(&g_ciCanTxBuffer, canTxBuffer, sizeof(canTxBuffer));

    //Uart phy��Buffer��ʼ��
    servStreamBufferInit(&g_ciUartRxBuffer, ciUartRxBuffer, sizeof(ciUartRxBuffer));
    servStreamBufferInit(&g_ciUartTxBuffer, ciUartTxBuffer, sizeof(ciUartTxBuffer)); 
    
    //Uart phy��Buffer��ʼ��
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
�� �� ��: servCiCanFrameSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: servCiCanRxFrameProcess;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servCiCanRxFrameProcess(void)
{    
    CanPhyFrameStruct   *pCanPhyFrame = NULL;
    CmdParseFrameStruct *pCmdPareFrame = NULL;
    u8 dataLen;

    
    //���CAN���ջ����������ݾʹ���
    if (g_ciCanRxBuffer.bNotEmpty)
    {   
        pCanPhyFrame = (CanPhyFrameStruct *)servGetHead(&g_ciCanRxBuffer);
        dataLen = pCanPhyFrame->GET_CAN_PHY_FRMAE_DATA_SIZE;

        //ֻ��payload�е����ݿ�����������CmdParseBuffer��
        pCmdPareFrame = (CmdParseFrameStruct *)servReserveMemory(&g_CmdParseBuffer, dataLen);
        if (NULL != pCmdPareFrame)
        {
            pCmdPareFrame->mainType = (CmdTypeEnum)pCanPhyFrame->payload[0];
            pCmdPareFrame->subType = pCanPhyFrame->payload[1];
            pCmdPareFrame->dataLen = dataLen - 2;    //CmdParse֡�ĳ��Ȳ����������ͺ������ͣ����Լ�2
            memcpy(pCmdPareFrame->payload, &pCanPhyFrame->payload[2], pCmdPareFrame->dataLen);    //����ʣ�������
            servEnqueue(&g_CmdParseBuffer, dataLen);
        }
        //=====================================================================================================

        //�ؽ����ж�
        bspCiCanITConfig(DISABLE);
        
        servDequeue(&g_ciCanRxBuffer, pCanPhyFrame->frameLen); //������ɺ����
        
        //�������ж�
        bspCiCanITConfig(ENABLE);
    }
}


/*********************************************************************************************
�� �� ��: servCiCanTxFrameProcess;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servCiCanTxFrameProcess(void)
{    
    CanPhyFrameStruct   *pCanPhyFrame = NULL;
    u8 dataLen;
    u8 canData[8];
    

    //���������Ҫ����
#if 1
    while (g_ciCanTxBuffer.bNotEmpty)    //һ�ΰ�Buffer�����ݷ���
#else
    if (g_ciCanTxBuffer.bNotEmpty)
#endif
    {
        pCanPhyFrame = (CanPhyFrameStruct *)servGetHead(&g_ciCanTxBuffer);

        //Ŀǰ���������г��ȶ���������8Bytes�ڣ����Բ��÷�֡����ֱ�ӷ�������
        dataLen = pCanPhyFrame->GET_CAN_PHY_FRMAE_DATA_SIZE;
        if (dataLen <= CAN_FRAME_BYTES_MAX)
        {
            memcpy(canData, pCanPhyFrame->payload, dataLen);
            bspCiCanSend(pCanPhyFrame->canID, CAN_RTR_DATA, dataLen, canData);
            bspDelayUs(160);    //������ʱ����Ȼ�ڸ������·��ͻᶪ֡
        }

        servDequeue(&g_ciCanTxBuffer, pCanPhyFrame->frameLen);
    }
}



/*********************************************************************************************
�� �� ��: servCiUartDmaRecTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servCiUartDmaRecTimerCB(void *timeOutPara)
{
    bspCiUartDmaOff();
    
    servStimerDelete(&g_ciUartDmaRecTimer);
}


/*********************************************************************************************
�� �� ��: servCiUartFrameSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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


    u8 sendLen = dataLen * 2 + sizeof(UartPhyFrameStruct);    //��ֹת�������ռ䣬�������������ݿռ�
    pPhyFrame = (u8 *)servReserveMemory(&g_ciUartTxBuffer, sendLen);
    if (NULL != pPhyFrame)
    {
        pUartPhyFrame = (UartPhyFrameStruct *)pPhyFrame;

        pPhyFrame[0] = UART_START_OF_FRAME;
        
        uartPhyFrame.frameLen = dataLen + sizeof(UartPhyFrameStruct);
        uartPhyFrame.xorValue = 0;
        pTempFrame = &uartPhyFrame.frameLen;

        //����֡ͷУ��ֵ��������SOF
        for (i = 0;i < headLen;i++)
        {
            xorValue ^= pTempFrame[i];
        }

        //��������У��ֵ
        for (i = 0;i < dataLen;i++)
        {
            xorValue ^= pDataBuffer[i];
        }
        uartPhyFrame.xorValue = xorValue;

        //ת��֡ͷ
        servEscapeMsg(pPhyFrame + 1, pTempFrame, &headLen);    //��һ������SOF

        //ת������
        servEscapeMsg(pPhyFrame + 1 + headLen, pDataBuffer, &dataLen);    //��һ������SOF
        pUartPhyFrame->frameLen = headLen + 1 + dataLen;    //��һ������SOF
        
        servEnqueue(&g_ciUartTxBuffer, pUartPhyFrame->frameLen);
    }
}

/*********************************************************************************************
�� �� ��: servCiUartRxFrameProcess;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servCiUartRxFrameProcess()
{
    UartPhyFrameStruct *pUartPhyFrame = NULL;
    CmdParseFrameStruct *pCmdPareFrame = NULL;
    u8 frameLen;

    
    //���Usart1���ջ����������ݾʹ���
    if (g_ciUartRxBuffer.bNotEmpty)
    {   
        pUartPhyFrame = (UartPhyFrameStruct *)servGetHead(&g_ciUartRxBuffer);
        frameLen = pUartPhyFrame->frameLen;
        if (servDeescapeMsgAndXorCheck((u8 *)pUartPhyFrame, &frameLen))    //У�鷴ת�壬���ҷ������ݵ���ʵ����                              
        {
            pCmdPareFrame = (CmdParseFrameStruct *)servReserveMemory(&g_CmdParseBuffer,  
                                                                     pUartPhyFrame->GET_UART_PHY_FRMAE_PAYLOAD_SIZE);

            if (NULL != pCmdPareFrame)
            {
                pCmdPareFrame->mainType = (CmdTypeEnum)pUartPhyFrame->payload[0];
                pCmdPareFrame->subType = pUartPhyFrame->payload[1];
                pCmdPareFrame->dataLen = frameLen - sizeof(UartPhyFrameStruct) - 2;    //CmdParse֡�ĳ��Ȳ����������ͺ������ͣ����Լ�2
                memcpy(pCmdPareFrame->payload, &pUartPhyFrame->payload[2], pCmdPareFrame->dataLen);    //����ʣ�������
                
                servEnqueue(&g_CmdParseBuffer, pCmdPareFrame->GET_CMD_PARSE_FRMAE_TOTAL_SIZE);
            }
        }

        //�ؽ����ж�
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
        
        servDequeue(&g_ciUartRxBuffer, pUartPhyFrame->frameLen); //������ɺ����
        
        //�������ж�
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    }
}

/*********************************************************************************************
�� �� ��: servCiUartTxFrameProcess;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servCiUartTxFrameProcess(void)
{
    UartPhyFrameStruct *pUartPhyFrame = NULL;
    

    //������ͻ����������ݾͷ���
#if 1
    while (g_ciUartTxBuffer.bNotEmpty)    //һ�ΰ�Buffer�����ݷ���
#else
    if (g_ciUartTxBuffer.bNotEmpty)
#endif
    {
        pUartPhyFrame = (UartPhyFrameStruct *)servGetHead(&g_ciUartTxBuffer);

        bspCiUartSend((u8 *)pUartPhyFrame, pUartPhyFrame->frameLen);

        servDequeue(&g_ciUartTxBuffer, pUartPhyFrame->frameLen); //������ɺ����
    }
}


/*********************************************************************************************
�� �� ��: servFrameSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: servRxFrameProcess;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servRxFrameProcess(void)
{
    //����CAN��������
    servCiCanRxFrameProcess();
    
    //����Usart��������
    servCiUartRxFrameProcess();
}


/*********************************************************************************************
�� �� ��: servTxFrameProcess;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servTxFrameProcess(void)
{
    servCiCanTxFrameProcess();

    servCiUartTxFrameProcess();
}


/*********************************************************************************************
�� �� ��: irqCiCanReceive
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void irqCiCanReceive(void)
{
    OS_ERR os_err;
    CanRxMsg RxMessage;
    u32 canID;
    u8  dataLen;
    u8 *pReceiveBuffer = NULL;
    static CanPhyFrameStruct *pCanPhyFrame = NULL;
    

    bspCiCanReceive(&RxMessage); //����FIFO0�е�����

    //����֡�Ŵ���
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

        //�����������Ϊ��
        if (NULL != pReceiveBuffer)
        {
            //����ID������
            memcpy(pReceiveBuffer, &canID, sizeof(canID));
            pReceiveBuffer += sizeof(canID);
            *pReceiveBuffer++ = dataLen;
            memcpy(pReceiveBuffer, RxMessage.Data, RxMessage.DLC); 

            pCanPhyFrame = (CanPhyFrameStruct *)servFastGetTail(g_ciCanRxBuffer);          
            servFastEnqueue(&g_ciCanRxBuffer, pCanPhyFrame->frameLen);

            //������ɺ����ź���
            OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, &os_err);
        }
    }
}


static bool bCiUartFrameHeadRcv = false;        //�Ƿ��յ�֡ͷ
/*********************************************************************************************
�� �� ��: irqCiUartReceive
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        //����Ѿ��յ�֡ͷ��������DMA�������ݣ����ҹر�Uart���ж�
        if (bCiUartFrameHeadRcv)
        {
            bCiUartFrameHeadRcv = false;

            //��֤�³����Ƿ���ȷ
            if ((ciUartRcvByte > 0) && (ciUartRcvByte < UART_FRAME_BYTES_AFESCAPE_MAX))
            {
                //�ر�Uart�ж�
                bspCiUartITConfig(DISABLE);

                //Ԥ��������
                pReceiveBuffer = (u8 *)servFastReserveMemory(&g_ciUartRxBuffer, ciUartRcvByte);

                //�����������Ϊ��
                if (NULL != pReceiveBuffer)
                {
                    //����֡�������ݳ���
                    *pReceiveBuffer++ = UART_START_OF_FRAME;
                    //*pReceiveBuffer++ = ciUartRcvByte;
                    
                    //����DMA
                    bspCiUartDmaRecive(pReceiveBuffer, ciUartRcvByte);

                    //����DMA���ճ�ʱ��ʱ��
                    servStimerAdd(&g_ciUartDmaRecTimer);
                }
                else
                {
                    //��Uart�ж�
                    bspCiUartITConfig(ENABLE);
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: irqCiUartDmaComplReceive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void irqCiUartDmaComplReceive(void)
{ 
    OS_ERR os_err;
    static UartPhyFrameStruct *pUsartPhyFrame = NULL;

    
    //�رճ�ʱ��ʱ��
    servStimerDelete(&g_ciUartDmaRecTimer);

    //����UART����
    bspCiUartReciveOn();

    //������Buffer�е�����
    pUsartPhyFrame = (UartPhyFrameStruct *)servFastGetTail(g_ciUartRxBuffer);
    servFastEnqueue(&g_ciUartRxBuffer, pUsartPhyFrame->frameLen);

    //������ɺ����ź���
    OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, &os_err);
}


#if 0
#endif


static SensorNumEnum sn1UartCurrNum = SENSOR_S1;
/*********************************************************************************************
�� �� ��: irqSensor1UartReceive;
ʵ�ֹ���: ���ڽ����жϻص�����
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void irqSensor1UartReceive(void)
{
    u8 *pReceiveBuffer = NULL;
    u8  recByte;


    recByte = bspSensor1UartRecive();
    
    if (sensorUartRecvInfo[UARTNUM_U1].SOF == recByte)
    {
        sensorUartRecvInfo[UARTNUM_U1].recvNum--;
        
        //�ر�Uart�ж�
        bspSensor1UartITConfig(DISABLE);

        //Ԥ��������
        pReceiveBuffer = (u8 *)servFastReserveMemory(&g_senUartRxBuffer[UARTNUM_U1][sn1UartCurrNum], 
                                                     sensorUartRecvInfo[UARTNUM_U1].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U1].frameLen));

        //�����������Ϊ��
        if (NULL != pReceiveBuffer)
        {
            //����֡�������ݳ���
            *pReceiveBuffer++ = sensorUartRecvInfo[UARTNUM_U1].frameLen;
            //*pReceiveBuffer++ = sensorUartRecvInfo[UARTNUM_U1].SOF;
            //*pReceiveBuffer++ = sn1UartRcvByte;
            
            //����DMA
            bspSensor1UartDmaRecive(pReceiveBuffer, sensorUartRecvInfo[UARTNUM_U1].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U1].frameLen));

            //����DMA���ճ�ʱ��ʱ��
            servStimerAdd(&g_senUartDmaRecTimer[UARTNUM_U1]);
        }
        else
        {
            //��Uart�ж�
            bspSensor1UartITConfig(ENABLE);
        }
    }
}


/*********************************************************************************************
�� �� ��: irqSensor1UartDmaComplReceive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void irqSensor1UartDmaComplReceive(void)
{ 
    SensorUartPhyFrameStruct *pUsartPhyFrame = NULL;


    if (!bspSensor1UartDmaCounterGet())
    {
        //�رճ�ʱ��ʱ��
        servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U1]);

        //������Buffer�е�����
        pUsartPhyFrame = (SensorUartPhyFrameStruct *)servFastGetTail(g_senUartRxBuffer[UARTNUM_U1][sn1UartCurrNum]);
        servFastEnqueue(&g_senUartRxBuffer[UARTNUM_U1][sn1UartCurrNum], pUsartPhyFrame->frameLen);
        g_senUartRxBuffer[UARTNUM_U1][sn1UartCurrNum].frameCount++;

        //���recvNumΪ0�˾Ͳ��ٽ������ݣ�ֱ���л����պ�����¸����ڲſ���UART����
        if (sensorUartRecvInfo[UARTNUM_U1].recvNum > 0)
        {
            //����UART����
            bspSensor1UartReciveOn();
        }
        else
        {
            bspSensor1UartReciveOff();

            //������ɺ����ź���
            g_eventSrcBmp.bSenUart[UARTNUM_U1][sn1UartCurrNum] = true;
            OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, NULL);
        }
    }
}


/*********************************************************************************************
�� �� ��: servSensor1UartDmaRecTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servSensor1UartDmaRecTimerCB(void *timeOutPara)
{
    //SensorNumEnum sensorNum = *(SensorNumEnum *)timeOutPara;

    bspSensor1UartDmaOff();

    servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U1]);
}


/*********************************************************************************************
�� �� ��: servSensor1UartSwitchTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servSensor1UartSwitchTimerCB(void *timeOutPara)
{
    u8  i, temp;
    //SensorNumEnum sensorNum = *(SensorNumEnum *)timeOutPara;
    
    
    //�л��˿�
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

        //������ƶ�ʱ����ʱʱ��
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
�� �� ��: servSensor1UartReciveOff;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servSensor1UartReciveOff(void)
{
    servStimerDelete(&g_senUartSwitchTimer[UARTNUM_U1]);

    bspSensor1UartReciveOff();
}


/*********************************************************************************************
�� �� ��: servSensor1UartReciveOn;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
static bool bsn2UartFrameHeadRcv = false;        //�Ƿ��յ�֡ͷ
#endif

static SensorNumEnum sn2UartCurrNum = SENSOR_S1; 
/*********************************************************************************************
�� �� ��: irqSensor2UartReceive;
ʵ�ֹ���: ���ڽ����жϻص�����
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        
        //�ر�Uart�ж�
        bspSensor2UartITConfig(DISABLE);

        //Ԥ��������
        pReceiveBuffer = (u8 *)servFastReserveMemory(&g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum], 
                                                     sensorUartRecvInfo[UARTNUM_U2].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U2].frameLen));

        //�����������Ϊ��
        if (NULL != pReceiveBuffer)
        {
            //����֡�������ݳ���
            *pReceiveBuffer++ = sensorUartRecvInfo[UARTNUM_U2].frameLen;
            //*pReceiveBuffer++ = sensorUartRecvInfo[UARTNUM_U2].SOF;
            //*pReceiveBuffer++ = sn2UartRcvByte;
            
            //����DMA
            bspSensor2UartDmaRecive(pReceiveBuffer, sensorUartRecvInfo[UARTNUM_U2].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U2].frameLen));

            //����DMA���ճ�ʱ��ʱ��
            servStimerAdd(&g_senUartDmaRecTimer[UARTNUM_U2]);
        }
        else
        {
            //��Uart�ж�
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

        //Ԥ��������
        frameLen = sensorUartRecvInfo[UARTNUM_U2].frameLen + sizeof(sensorUartRecvInfo[UARTNUM_U2].frameLen);
        pReceiveBuffer = (u8 *)servFastReserveMemory(&g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum], frameLen);

        if (NULL != pReceiveBuffer)
        {
            //����֡�������ݳ���
            *pReceiveBuffer++ = frameLen;
            *pReceiveBuffer++ = recByte;
            recByteCount++;

            //�������ճ�ʱ��ʱ��
            servStimerAdd(&g_senUartDmaRecTimer[UARTNUM_U2]);
        }
        else
        {
            bsn2UartFrameHeadRcv = false;
        }
    }
    else
    {
        //�Ѿ����յ�֡ͷ
        if (bsn2UartFrameHeadRcv)
        {
            *pReceiveBuffer++ = recByte;
            recByteCount++;

            if (recByteCount == sensorUartRecvInfo[UARTNUM_U2].frameLen)    //������һ֡
            {
                sensorUartRecvInfo[UARTNUM_U2].recvNum--;

                //�رճ�ʱ��ʱ��
                servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U2]);

                //����Buffer�е�����
                pUsartPhyFrame = (SensorUartPhyFrameStruct *)servFastGetTail(g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum]);
                servFastEnqueue(&g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum], pUsartPhyFrame->frameLen);
                g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum].frameCount++;
                
                bsn2UartFrameHeadRcv = false;
                recByteCount = 0;

                //���recvNumΪ0�˾Ͳ��ٽ������ݣ�ֱ���л����պ�����¸����ڲſ���UART����
                if (sensorUartRecvInfo[UARTNUM_U2].recvNum > 0)
                {
                    //����UART����
                    bspSensor2UartReciveOn();
                }
                else
                {
                    bspSensor2UartReciveOff();

                    //������ɺ����ź���
                    g_eventSrcBmp.bSenUart[UARTNUM_U2][sn2UartCurrNum] = true;
                    OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, NULL);
                }
            }
        }
    }
    
#endif
}



/*********************************************************************************************
�� �� ��: irqSensor2UartDmaComplReceive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void irqSensor2UartDmaComplReceive(void)
{ 
    SensorUartPhyFrameStruct *pUsartPhyFrame = NULL;

    
    if (!bspSensor2UartDmaCounterGet())
    {
        //�رճ�ʱ��ʱ��
        servStimerDelete(&g_senUartDmaRecTimer[UARTNUM_U2]);

        //������Buffer�е�����
        pUsartPhyFrame = (SensorUartPhyFrameStruct *)servFastGetTail(g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum]);
        servFastEnqueue(&g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum], pUsartPhyFrame->frameLen);
        g_senUartRxBuffer[UARTNUM_U2][sn2UartCurrNum].frameCount++;

        //���recvNumΪ0�˾Ͳ��ٽ������ݣ�ֱ���л����պ�����¸����ڲſ���UART����
        if (sensorUartRecvInfo[UARTNUM_U2].recvNum > 0)
        {
            //����UART����
            bspSensor2UartReciveOn();
        }
        else
        {
            bspSensor2UartReciveOff();

            //������ɺ����ź���
            g_eventSrcBmp.bSenUart[UARTNUM_U2][sn2UartCurrNum] = true;
            OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, NULL);
        }
    }
}


/*********************************************************************************************
�� �� ��: servSensor2UartDmaRecTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: servSensor2UartSwitchTimerCB;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servSensor2UartSwitchTimerCB(void *timeOutPara)
{
    u8  i, temp;
    //SensorNumEnum sensorNum = *(SensorNumEnum *)timeOutPara;
    
    
    //�л��˿�
    temp = sn2UartCurrNum;
    for (i = 0;i < SENSOR_RESERVE;i++)
    {
        temp += 1;
        if (temp >= SENSOR_RESERVE)
        {
            temp = SENSOR_S1;
        }

        //��һ��Sensor��״̬�ǿ������ٿ������Sensor״̬
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

        //������ƶ�ʱ����ʱʱ��
        servStimerInit(&g_senUartSwitchTimer[UARTNUM_U2], sensorUartRecvInfo[UARTNUM_U2].swTime, servSensor2UartSwitchTimerCB); 
        servStimerAdd(&g_senUartSwitchTimer[UARTNUM_U2]);
    }
    
    bspSensor2UartReciveOn();

#if GELGOOG_SINANJU
    //����ֵ������ʱ��Ҫ���Ͳ�ѯ����    CJ 2018.02.28 Add
    servFpgaAbsEncReadTrig(sn2UartCurrNum);
#endif

    if (g_senUartSwitchTimer[UARTNUM_U2].bUsed)
    {
        servStimerAdd(&g_senUartSwitchTimer[UARTNUM_U2]);
    }
}


/*********************************************************************************************
�� �� ��: servSensor2UartReciveOff;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servSensor2UartReciveOff(void)
{
    servStimerDelete(&g_senUartSwitchTimer[UARTNUM_U2]);

    bspSensor2UartReciveOff();
    servFpgaSensorUart2RxEnable(false);
}


/*********************************************************************************************
�� �� ��: servSensor2UartReciveOn;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        //����ֵ������ʱ��Ҫ���Ͳ�ѯ����    CJ 2018.02.28 Add
        servFpgaAbsEncReadTrig(sn2UartCurrNum);
#endif
    }
}



/*******************************************�ļ�β********************************************/
