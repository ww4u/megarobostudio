/*********************************************************************************************
Copyright (C) 2017������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servCanNetManage.c;
��������:  can�ڵ��������
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  xyzheng
�������:  2017.04.15;
��ʷ�汾:  ��;

    CAN����λ����:�������ͳ�һλ��ĳ���ڵ�ͬʱҲ�ڼ������ߣ������ӵ�����λ�ĵ�ƽ���ͳ��ĵ�ƽ��ͬʱ�����ڸ�λʱ�̼�⵽һ��λ����
    �������ٲ��������λ���ڼ��Ӧ���϶�ͳ�����λ����⵽����λʱ������Ϊ�Ǵ���λ.
    1. ��λ������stm32��������ǩ��,����һ��32λ�Ĺ�ϣֵ(���ܻ���ײ).
    2. ��λ��(����)���������������.
    3. ��λ���յ�������������,��32λ�Ĺ�ϣֵ���͸�����.
       (���ǻ����͹�ϣֵʱ,���ܻ��������߾���(��ͬ��ID);��ͬ��ID��ͬ������ͬʱ����ʱ,�����λ����.����λ����Ĵӻ�,��Ҫ���·���)
    4. ��λ�������յ��Ĺ�ϣֵ��ID,�����ж��м�̨�ӻ�(�����м����ӻ�).
    5. ����ÿһ̨�ӻ�,��λ�����䷢������(���Ϲ�ϣֵ),�����������״̬.
    6. ��λ���ҵ�һ�����ʵ�ID,�������ǰ�ӻ�(ֻ�н�������״̬�Ĵӻ�,�Ż��޸�ID).
    7. ����ID��ɺ�,����ʹ����ID,�����˳�����״̬.
    8. �ظ�����5~7,�޸����дӻ�ID.
    9. ����ID��ɺ�,����ÿһ̨�ӻ�,��λ����Ҫ������Ҫ����ǩ��(���ܻ��ж��˫��(32λ)).
    10.��λ�����η��͵���ǩ����˫��(ÿ�η���32λ),����,��������Ҫ���ϵ�ǰ����˫�ֵ�����.
    11.����ͬһ̨�ӻ�,�����λ���յ��������ǩ��,����Ϊ,��ǰID���ж�̨�ӻ�,��Ҫ�ٷ����ַ.(���ݵ���ǩ��).
    12.����ͬһ̨�ӻ�,�����λ��ֻ�յ�һ�ݵ���ǩ��,����Ϊ��ǰID��ֻ��һ̨�ӻ�.
*********************************************************************************************/
#include "string.h"
#include "servCanNetManage.h"
#include "comDebug.h"
CanNetManage_Stru g_stCanNetManageInfo = {0};

/*********************************************************************************************
�� �� ��: servCanNetManageGetSTM32Signature
ʵ�ֹ���: ��ȡSTM32�ĵ���ǩ��(���ֲ���˵��,�����ʶ����������ȫ��Ψһ)
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
static u8 servCanNetManageGetSTM32Signature(u8 * data,u8 len)
{
#ifdef PROJECT_GELGOOG
    int i = 0, address =  0x1FFF7A10;    //The 96-bit unique device identifier
#else
    int i = 0, address =  0x1FFFF7E8;    //The 96-bit unique device identifier
#endif

    if(data == NULL)//����������ΪNULL,��Ĭ��ʹ��stm32������ǩ��
    {
        data = (u8*)g_stCanNetManageInfo.signature;
        while(i < 12 ) 
        {
           *(data + i) = *(__IO u8*) address++;
           i++;
        }
        return 3 ;//3*4 = 12�ֽ� = 96 bit
    }
    if(len & 0x03)//��Ҫ4�ֽڶ��� 
    {
      return 0;
    }
    memcpy((u8*)g_stCanNetManageInfo.signature,data,len);
    return (len/4);
}

/*********************************************************************************************
�� �� ��: servCanNetManageBuildHash
ʵ�ֹ���: ����һ��32λ��(α)Ψһʶ����(BKDR Hash Function)
�������: 
�������: ��;
�� �� ֵ:
˵    ��: str������ȫ������Ψһ�ģ�str�ĳ��������ϲ�����,��Խ��,����Խ�鷳.
          ����stm32��˵,str������ѡ��:1,���к� ;  2,stm32��������ǩ��(96λ,12���ֽ�)
*********************************************************************************************/
static u32 servCanNetManageBuildHashCode(u8 * str,u8 len)
{
   u32 seed = 131; 
   u32 hash = 0;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++)  
   {  
      hash = hash * seed + *str;  
   }  
   return hash;  
}
/*********************************************************************************************
�� �� ��:servCanNetManageGetHashCode 
ʵ�ֹ���: 
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
u32  servCanNetManageGetHashCode()
{
    return g_stCanNetManageInfo.hash;
}
/*********************************************************************************************
�� �� ��:servCanNetManageIsHashCode 
ʵ�ֹ���:��ϣֵ�Ƿ����
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
u8  servCanNetManageIsHashCodeEqual(u32 hashcode)
{
    return (g_stCanNetManageInfo.hash == hashcode);
}
/*********************************************************************************************
�� �� ��:servCanNetManageIsSignatureEqual 
ʵ�ֹ���:Ψһʶ����(����ǩ��)�Ƿ����
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
u8  servCanNetManageIsSignatureEqual(u8 index,u32 value)
{
    if(index > SIGNATURE_MAX_LEN)
    {
        return 0;
    }
    return (g_stCanNetManageInfo.signature[index] == value);
}
/*********************************************************************************************
�� �� ��:servCanNetManangeSetConfigState 
ʵ�ֹ���: 
�������: enable: 1,��ʾ��hashcode�׶�  2:��ʾ��Ψһʶ����(����ǩ��)�׶�  0:��ʾ���ý���
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
void servCanNetManageSetConfigState(u8 enable)
{
    g_stCanNetManageInfo.config = enable;
}
/*********************************************************************************************
�� �� ��:servCanNetManageGetConfigState
ʵ�ֹ���: 
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
u8 servCanNetManageGetConfigState()
{
    return g_stCanNetManageInfo.config;
}
/*********************************************************************************************
�� �� ��:servCanNetManangeSetConfigState 
ʵ�ֹ���: 
�������: enable: 1,��ʾ��hashcode�׶�  2:��ʾ��Ψһʶ����(����ǩ��)�׶�  0:��ʾ���ý���
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
void servCanNetManageSetConfigPreState(u8 enable)
{
    g_stCanNetManageInfo.pre_config = enable;
}
/*********************************************************************************************
�� �� ��:servCanNetManageGetConfigState
ʵ�ֹ���: 
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
u8 servCanNetManageGetConfigPreState()
{
    return g_stCanNetManageInfo.pre_config;
}
/*********************************************************************************************
�� �� ��:servCanNetManageInit
ʵ�ֹ���: 
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
void servCanNetManageInit(u8 * unique,int len)
{
    if(len > SIGNATURE_MAX_LEN*4)
    {
        return;
    }
    g_stCanNetManageInfo.config = 0;
    memset((u8*)g_stCanNetManageInfo.signature,0,SIGNATURE_MAX_LEN);
    g_stCanNetManageInfo.signature_size = servCanNetManageGetSTM32Signature(unique,len);
    if(g_stCanNetManageInfo.signature_size)
    {
        g_stCanNetManageInfo.hash = servCanNetManageBuildHashCode((u8*)g_stCanNetManageInfo.signature,g_stCanNetManageInfo.signature_size<<2);
    }
    //g_stCanNetManageInfo.hash = 0x5a5a5a5a; //������ͬHashCode����� 
}
