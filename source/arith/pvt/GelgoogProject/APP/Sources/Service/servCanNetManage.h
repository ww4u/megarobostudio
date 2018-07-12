
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
*********************************************************************************************/
#ifndef SERV_CAN_NET_MANAGE_H
#define SERV_CAN_NET_MANAGE_H
#include "bspCan.h"



#define SIGNATURE_MAX_LEN    16  //唯一识别码,最大长度为16*4 = 64 字节



typedef struct 
{
    u8  config;
    u8  pre_config;
    u8  signature_size;
    u32 hash;
    u32 signature[SIGNATURE_MAX_LEN];
    
}CanNetManage_Stru;

extern CanNetManage_Stru g_stCanNetManageInfo;
/*********************************************************************************************
函 数 名:servCanNetManageGetHashCode 
实现功能: 
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
u32  servCanNetManageGetHashCode();
/*********************************************************************************************
函 数 名:servCanNetManageIsHashCode 
实现功能:哈希值是否相等
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
u8  servCanNetManageIsHashCodeEqual(u32 hashcode);
/*********************************************************************************************
函 数 名:servCanNetManageIsSignatureEqual 
实现功能:数字签名是否相等
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
u8  servCanNetManageIsSignatureEqual(u8 index,u32 value);
/*********************************************************************************************
函 数 名:servCanNetManangeSetConfigState 
实现功能: 
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
void servCanNetManageSetConfigState(u8 enable);
void servCanNetManageSetConfigPreState(u8 enable);
/*********************************************************************************************
函 数 名:servCanNetManageGetConfigState
实现功能: 
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
u8 servCanNetManageGetConfigState();
u8 servCanNetManageGetConfigPreState();
/*********************************************************************************************
函 数 名:servCanNetManageInit
实现功能: 
输入参数: unique:唯一标识符字符串;len:字符串长度,不能大于128字节 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
void servCanNetManageInit(u8 * unique,int len);
#endif
