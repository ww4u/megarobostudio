/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  bspCan.h
功能描述:  
函数列表:  
当前版本:  1.0.0
版本改动:  
作    者:  xyzheng
完成日期:  Oct 11, 2017  3:20:24 PM
历史版本:  无
*********************************************************************************************/

#ifndef MR_BSP_CAN_H_
#define MR_BSP_CAN_H_
#include "data_type.h"

#define DEBUG_BSP_CAN  0

typedef struct
{
    int fd;
}CAN_INFO;

extern CAN_INFO g_stCanInfo;

#include "canapi.h"


#endif /* CAN_H_ */
