/*********************************************************************************************
Copyright (C) 2017，北京镁伽机器人科技有限公司
文 件 名:  servCanNetManage.c;
功能描述:  can节点网络管理
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  xyzheng
完成日期:  2017.04.15;
历史版本:  无;

    CAN总线位错误:向总线送出一位的某个节点同时也在监视总线，当监视到总线位的电平和送出的电平不同时，则在该位时刻检测到一个位错误。
    但是在仲裁区的填充位流期间或应答间隙送出隐性位而检测到显性位时，不认为是错误位.
    1. 下位机根据stm32器件电子签名,生成一个32位的哈希值(可能会碰撞).
    2. 上位机(主机)发送网络管理命令.
    3. 下位机收到网络管理命令后,将32位的哈希值发送给主机.
       (下们机发送哈希值时,可能会引起总线竞争(不同的ID);相同的ID不同的数据同时发送时,会产生位错误.产生位错误的从机,需要重新发送)
    4. 上位机根据收到的哈希值及ID,初步判断有几台从机(至少有几个从机).
    5. 对于每一台从机,上位机向其发旁命令(带上哈希值),令其进入配置状态.
    6. 上位机找到一个合适的ID,分配给当前从机(只有进入配置状态的从机,才会修改ID).
    7. 配置ID完成后,主机使用新ID,令其退出配置状态.
    8. 重复步骤5~7,修改所有从机ID.
    9. 配置ID完成后,对于每一台从机,上位机需要向其索要电子签名(可能会有多个双字(32位)).
    10.下位机依次发送电子签名的双字(每次发送32位),另外,命令中需要加上当前发送双字的索引.
    11.对于同一台从机,如果上位机收到多个电子签名,则认为,当前ID下有多台从机,需要再分配地址.(根据电子签名).
    12.对于同一台从机,如果上位机只收到一份电子签名,则认为当前ID下只有一台从机.
*********************************************************************************************/
#include "string.h"
#include "servCanNetManage.h"
#include "comDebug.h"
CanNetManage_Stru g_stCanNetManageInfo = {0};

/*********************************************************************************************
函 数 名: servCanNetManageGetSTM32Signature
实现功能: 读取STM32的电子签名(按手册上说明,这个标识符可以做到全球唯一)
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
static u8 servCanNetManageGetSTM32Signature(u8 * data,u8 len)
{
#ifdef PROJECT_GELGOOG
    int i = 0, address =  0x1FFF7A10;    //The 96-bit unique device identifier
#else
    int i = 0, address =  0x1FFFF7E8;    //The 96-bit unique device identifier
#endif

    if(data == NULL)//如果输入参数为NULL,则默认使用stm32的数字签名
    {
        data = (u8*)g_stCanNetManageInfo.signature;
        while(i < 12 ) 
        {
           *(data + i) = *(__IO u8*) address++;
           i++;
        }
        return 3 ;//3*4 = 12字节 = 96 bit
    }
    if(len & 0x03)//需要4字节对齐 
    {
      return 0;
    }
    memcpy((u8*)g_stCanNetManageInfo.signature,data,len);
    return (len/4);
}

/*********************************************************************************************
函 数 名: servCanNetManageBuildHash
实现功能: 构建一个32位的(伪)唯一识别码(BKDR Hash Function)
输入参数: 
输出参数: 无;
返 回 值:
说    明: str必须是全网段中唯一的，str的长度理论上不限制,但越长,操作越麻烦.
          对于stm32来说,str有两个选择:1,序列号 ;  2,stm32器件电子签名(96位,12个字节)
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
函 数 名:servCanNetManageGetHashCode 
实现功能: 
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
u32  servCanNetManageGetHashCode()
{
    return g_stCanNetManageInfo.hash;
}
/*********************************************************************************************
函 数 名:servCanNetManageIsHashCode 
实现功能:哈希值是否相等
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
u8  servCanNetManageIsHashCodeEqual(u32 hashcode)
{
    return (g_stCanNetManageInfo.hash == hashcode);
}
/*********************************************************************************************
函 数 名:servCanNetManageIsSignatureEqual 
实现功能:唯一识别码(电子签名)是否相等
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
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
函 数 名:servCanNetManangeSetConfigState 
实现功能: 
输入参数: enable: 1,表示在hashcode阶段  2:表示在唯一识别码(电子签名)阶段  0:表示配置结束
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
void servCanNetManageSetConfigState(u8 enable)
{
    g_stCanNetManageInfo.config = enable;
}
/*********************************************************************************************
函 数 名:servCanNetManageGetConfigState
实现功能: 
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
u8 servCanNetManageGetConfigState()
{
    return g_stCanNetManageInfo.config;
}
/*********************************************************************************************
函 数 名:servCanNetManangeSetConfigState 
实现功能: 
输入参数: enable: 1,表示在hashcode阶段  2:表示在唯一识别码(电子签名)阶段  0:表示配置结束
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
void servCanNetManageSetConfigPreState(u8 enable)
{
    g_stCanNetManageInfo.pre_config = enable;
}
/*********************************************************************************************
函 数 名:servCanNetManageGetConfigState
实现功能: 
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
u8 servCanNetManageGetConfigPreState()
{
    return g_stCanNetManageInfo.pre_config;
}
/*********************************************************************************************
函 数 名:servCanNetManageInit
实现功能: 
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
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
    //g_stCanNetManageInfo.hash = 0x5a5a5a5a; //测试相同HashCode的情况 
}
