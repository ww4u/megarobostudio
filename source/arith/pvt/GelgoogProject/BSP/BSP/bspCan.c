/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspCan.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.22;
历史版本:  无;
*********************************************************************************************/
#include "bspCan.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    CAN_CI_CAN              CAN1

#define    CAN_CI_CAN_CLK          RCC_APB1Periph_CAN1
#define    CAN_CI_CAN_FIFO         CAN_FIFO0
#define    CAN_CI_CAN_RX_IRQn      CAN1_RX0_IRQn
#define    CAN_CI_CAN_IT_FMP       CAN_IT_FMP0

#define    CAN_CI_CAN_RX_PIN       GPIO_Pin_11
#define    CAN_CI_CAN_TX_PIN       GPIO_Pin_12
#define    CAN_CI_CAN_GPIO_PORT    GPIOA
#define    CAN_CI_CAN_GPIO_CLK     RCC_AHB1Periph_GPIOA
#define    CAN_CI_CAN_AF_PORT      GPIO_AF_CAN1
#define    CAN_CI_CAN_RX_SOURCE    GPIO_PinSource11
#define    CAN_CI_CAN_TX_SOURCE    GPIO_PinSource12  

#define    CAN_CI_DATA_TIMEOUT     13440000    //根据DELAY_COUNT_MS = 33600, 此超时时间大约为400ms



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
u32 canIdTypeReal;
u32 canSendId;

u32 canIdType[CANTYPE_RESERVE] = {CAN_ID_STD, CAN_ID_EXT};

//按顺序依次对应的波特率是1000、500、250、125、100、50、20、10kBps
u16 canPrescaler[CANBAUD_RESERVE] = {6, 12, 24, 48, 60, 120, 300, 600}; 



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: bspCiCanInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspCiCanInit(CanIntfcStruct canIntfc)
{
    GPIO_InitTypeDef      GPIO_InitStructure;
    CAN_InitTypeDef       CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef      NVIC_InitStructure;
    

    /****************CAN GPIO配置*****************************************/
    RCC_AHB1PeriphClockCmd(CAN_CI_CAN_GPIO_CLK, ENABLE);

    GPIO_PinAFConfig(CAN_CI_CAN_GPIO_PORT, CAN_CI_CAN_RX_SOURCE, CAN_CI_CAN_AF_PORT);
    GPIO_PinAFConfig(CAN_CI_CAN_GPIO_PORT, CAN_CI_CAN_TX_SOURCE, CAN_CI_CAN_AF_PORT); 

    GPIO_InitStructure.GPIO_Pin = CAN_CI_CAN_RX_PIN | CAN_CI_CAN_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(CAN_CI_CAN_GPIO_PORT, &GPIO_InitStructure);


    /*********************CAN配置*****************************************/
    RCC_APB1PeriphClockCmd(CAN_CI_CAN_CLK, ENABLE);

    CAN_DeInit(CAN_CI_CAN);

    CAN_InitStructure.CAN_TTCM = DISABLE;            //没有使能时间触发模式
    CAN_InitStructure.CAN_ABOM = DISABLE;            //没有使能自动离线管理
    CAN_InitStructure.CAN_AWUM = DISABLE;            //没有使能自动唤醒模式
    CAN_InitStructure.CAN_NART = DISABLE;            //没有使能非自动重传模式
    CAN_InitStructure.CAN_RFLM = DISABLE;            //没有使能接收FIFO锁定模式
    CAN_InitStructure.CAN_TXFP = DISABLE;            //没有使能发送FIFO优先级
    
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;    //CAN设置为正常模式
    
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         //重新同步跳跃宽度1个时间单位
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         //时间段1为3个时间单位
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;         //时间段2为3个时间单位
    CAN_InitStructure.CAN_Prescaler = canPrescaler[canIntfc.baud];
    
    CAN_Init(CAN_CI_CAN, &CAN_InitStructure);

    canSendId = canIntfc.sendId;    //记录下发送ID
    
    /****************CAN滤波器配置*****************************************/
    canIdTypeReal = canIdType[canIntfc.idType];
    
    //配置滤波器0
    CAN_FilterInitStructure.CAN_FilterNumber = 0;

    //为了过滤出一组标识符，应该设置过滤器组工作在屏蔽位模式
    //为了过滤出一个标识符，应该设置过滤器组工作在标识符列表模式
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    
    //(STID[10:3])(STID[2:0]EXID[17:13])(EXID[12:5])(EXID[4:0]IDE RTR 0)
    CAN_FilterInitStructure.CAN_FilterIdHigh = (u16)(((canIntfc.receiveId << 3) >>16) & 0xFFFF);
    CAN_FilterInitStructure.CAN_FilterIdLow = (u16)((canIntfc.receiveId << 3) | canIdTypeReal);
    
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (u16)(((canIntfc.groupId1 << 3) >>16) & 0xFFFF);
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (u16)((canIntfc.groupId1<< 3) | canIdTypeReal);

    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_CI_CAN_FIFO;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    //配置滤波器1
    CAN_FilterInitStructure.CAN_FilterNumber = 1;

    //为了过滤出一组标识符，应该设置过滤器组工作在屏蔽位模式
    //为了过滤出一个标识符，应该设置过滤器组工作在标识符列表模式
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    
    //(STID[10:3])(STID[2:0]EXID[17:13])(EXID[12:5])(EXID[4:0]IDE RTR 0)
    CAN_FilterInitStructure.CAN_FilterIdHigh = (u16)(((canIntfc.radioId << 3) >>16) & 0xFFFF);
    CAN_FilterInitStructure.CAN_FilterIdLow = (u16)((canIntfc.radioId << 3) | canIdTypeReal);
    
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (u16)(((canIntfc.groupId2 << 3) >>16) & 0xFFFF);
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (u16)((canIntfc.groupId2 << 3) | canIdTypeReal);

    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_CI_CAN_FIFO;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    
    /****************CAN中断配置*****************************************/
    NVIC_InitStructure.NVIC_IRQChannel = CAN_CI_CAN_RX_IRQn;;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN_CI_CAN, CAN_CI_CAN_IT_FMP, ENABLE);    //使能FIFO0消息挂号中断
}


/*********************************************************************************************
函 数 名: bspCiCanSend;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspCiCanSend(u32 sendId, u8 canRtr, u8 dataLen, u8 *data)
{
    CanTxMsg TxMessage;
    u8  i;
    u8  txMailbox;
    u32 timeout = CAN_CI_DATA_TIMEOUT;


    TxMessage.StdId = canSendId;  //标准标识符
    TxMessage.ExtId = canSendId;    //扩展标识符
    
    TxMessage.RTR = canRtr;     //为数据帧: CAN_RTR_DATA/CAN_RTR_REMOTE
    
    TxMessage.IDE = canIdTypeReal;     //使用扩展标识符: CAN_ID_EXT/CAN_ID_STD

    if (dataLen <= 8)
    {
        TxMessage.DLC = dataLen;    //数据长度
        for (i = 0;i < dataLen;i++)
        {
            TxMessage.Data[i] = *data++;
        }
        
        txMailbox = CAN_Transmit(CAN_CI_CAN, &TxMessage); //发送数据  
        
        while ((CAN_TxStatus_Ok != CAN_TransmitStatus(CAN_CI_CAN, txMailbox)) && (timeout > 0))
        {            
            //等待发送结束或者超时
            timeout--;       
        }
    }
}


/*********************************************************************************************
函 数 名: bspCiCanReceive;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspCiCanReceive(CanRxMsg* rxMessage)
{
    CAN_Receive(CAN_CI_CAN, CAN_CI_CAN_FIFO, rxMessage); //接收FIFO0中的数据
}


/*********************************************************************************************
函 数 名: bspCiCanITConfig;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void bspCiCanITConfig(FunctionalState funcState)
{
    CAN_ITConfig(CAN_CI_CAN, CAN_CI_CAN_IT_FMP, funcState);
}



/*******************************************文件尾********************************************/