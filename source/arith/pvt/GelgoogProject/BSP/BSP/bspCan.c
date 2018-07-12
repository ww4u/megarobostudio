/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  bspCan.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.22;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "bspCan.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
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

#define    CAN_CI_DATA_TIMEOUT     13440000    //����DELAY_COUNT_MS = 33600, �˳�ʱʱ���ԼΪ400ms



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
u32 canIdTypeReal;
u32 canSendId;

u32 canIdType[CANTYPE_RESERVE] = {CAN_ID_STD, CAN_ID_EXT};

//��˳�����ζ�Ӧ�Ĳ�������1000��500��250��125��100��50��20��10kBps
u16 canPrescaler[CANBAUD_RESERVE] = {6, 12, 24, 48, 60, 120, 300, 600}; 



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: bspCiCanInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiCanInit(CanIntfcStruct canIntfc)
{
    GPIO_InitTypeDef      GPIO_InitStructure;
    CAN_InitTypeDef       CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef      NVIC_InitStructure;
    

    /****************CAN GPIO����*****************************************/
    RCC_AHB1PeriphClockCmd(CAN_CI_CAN_GPIO_CLK, ENABLE);

    GPIO_PinAFConfig(CAN_CI_CAN_GPIO_PORT, CAN_CI_CAN_RX_SOURCE, CAN_CI_CAN_AF_PORT);
    GPIO_PinAFConfig(CAN_CI_CAN_GPIO_PORT, CAN_CI_CAN_TX_SOURCE, CAN_CI_CAN_AF_PORT); 

    GPIO_InitStructure.GPIO_Pin = CAN_CI_CAN_RX_PIN | CAN_CI_CAN_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(CAN_CI_CAN_GPIO_PORT, &GPIO_InitStructure);


    /*********************CAN����*****************************************/
    RCC_APB1PeriphClockCmd(CAN_CI_CAN_CLK, ENABLE);

    CAN_DeInit(CAN_CI_CAN);

    CAN_InitStructure.CAN_TTCM = DISABLE;            //û��ʹ��ʱ�䴥��ģʽ
    CAN_InitStructure.CAN_ABOM = DISABLE;            //û��ʹ���Զ����߹���
    CAN_InitStructure.CAN_AWUM = DISABLE;            //û��ʹ���Զ�����ģʽ
    CAN_InitStructure.CAN_NART = DISABLE;            //û��ʹ�ܷ��Զ��ش�ģʽ
    CAN_InitStructure.CAN_RFLM = DISABLE;            //û��ʹ�ܽ���FIFO����ģʽ
    CAN_InitStructure.CAN_TXFP = DISABLE;            //û��ʹ�ܷ���FIFO���ȼ�
    
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;    //CAN����Ϊ����ģʽ
    
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         //����ͬ����Ծ���1��ʱ�䵥λ
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         //ʱ���1Ϊ3��ʱ�䵥λ
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;         //ʱ���2Ϊ3��ʱ�䵥λ
    CAN_InitStructure.CAN_Prescaler = canPrescaler[canIntfc.baud];
    
    CAN_Init(CAN_CI_CAN, &CAN_InitStructure);

    canSendId = canIntfc.sendId;    //��¼�·���ID
    
    /****************CAN�˲�������*****************************************/
    canIdTypeReal = canIdType[canIntfc.idType];
    
    //�����˲���0
    CAN_FilterInitStructure.CAN_FilterNumber = 0;

    //Ϊ�˹��˳�һ���ʶ����Ӧ�����ù������鹤��������λģʽ
    //Ϊ�˹��˳�һ����ʶ����Ӧ�����ù������鹤���ڱ�ʶ���б�ģʽ
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

    //�����˲���1
    CAN_FilterInitStructure.CAN_FilterNumber = 1;

    //Ϊ�˹��˳�һ���ʶ����Ӧ�����ù������鹤��������λģʽ
    //Ϊ�˹��˳�һ����ʶ����Ӧ�����ù������鹤���ڱ�ʶ���б�ģʽ
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

    
    /****************CAN�ж�����*****************************************/
    NVIC_InitStructure.NVIC_IRQChannel = CAN_CI_CAN_RX_IRQn;;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN_CI_CAN, CAN_CI_CAN_IT_FMP, ENABLE);    //ʹ��FIFO0��Ϣ�Һ��ж�
}


/*********************************************************************************************
�� �� ��: bspCiCanSend;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiCanSend(u32 sendId, u8 canRtr, u8 dataLen, u8 *data)
{
    CanTxMsg TxMessage;
    u8  i;
    u8  txMailbox;
    u32 timeout = CAN_CI_DATA_TIMEOUT;


    TxMessage.StdId = canSendId;  //��׼��ʶ��
    TxMessage.ExtId = canSendId;    //��չ��ʶ��
    
    TxMessage.RTR = canRtr;     //Ϊ����֡: CAN_RTR_DATA/CAN_RTR_REMOTE
    
    TxMessage.IDE = canIdTypeReal;     //ʹ����չ��ʶ��: CAN_ID_EXT/CAN_ID_STD

    if (dataLen <= 8)
    {
        TxMessage.DLC = dataLen;    //���ݳ���
        for (i = 0;i < dataLen;i++)
        {
            TxMessage.Data[i] = *data++;
        }
        
        txMailbox = CAN_Transmit(CAN_CI_CAN, &TxMessage); //��������  
        
        while ((CAN_TxStatus_Ok != CAN_TransmitStatus(CAN_CI_CAN, txMailbox)) && (timeout > 0))
        {            
            //�ȴ����ͽ������߳�ʱ
            timeout--;       
        }
    }
}


/*********************************************************************************************
�� �� ��: bspCiCanReceive;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiCanReceive(CanRxMsg* rxMessage)
{
    CAN_Receive(CAN_CI_CAN, CAN_CI_CAN_FIFO, rxMessage); //����FIFO0�е�����
}


/*********************************************************************************************
�� �� ��: bspCiCanITConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void bspCiCanITConfig(FunctionalState funcState)
{
    CAN_ITConfig(CAN_CI_CAN, CAN_CI_CAN_IT_FMP, funcState);
}



/*******************************************�ļ�β********************************************/