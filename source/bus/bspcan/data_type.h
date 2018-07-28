/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  data_type.h
功能描述:  
函数列表:  
当前版本:  1.0.0
版本改动:  
作    者:  xyzheng
完成日期:  Nov 8, 2017  4:24:00 PM
历史版本:  无
*********************************************************************************************/

#ifndef DATA_TYPE_H_
#define DATA_TYPE_H_

/* there is no 'bool' in C programe,but it exist in C++ programe*/
//typedef unsigned char                   bool;
//#define   bool                          u8

#define  S64_UP_LIMIT            0x7FFFFFFFFFFFFFFF
#define  S32_UP_LIMIT            0x7FFFFFFF
#define  S16_UP_LIMIT            0x7FFF
#define  S8_UP_LIMIT             0x7F
#define  U64_UP_LIMIT            0xFFFFFFFFFFFFFFFF
#define  U32_UP_LIMIT            0xFFFFFFFF
#define  U16_UP_LIMIT            0xFFFF
#define  U8_UP_LIMIT             0xFF


#define  INVALID_ID   0xFFFFFFFF

#ifdef __linux__
#include <linux/types.h>
#include <stddef.h>
#include <linux/kernel.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>

#define UNUSED(x) (void)x

typedef char int8;
typedef char sbyte;
typedef char CHAR;
typedef char s8;

typedef unsigned char uint8;
typedef unsigned char byte;
typedef unsigned char BYTE;
typedef unsigned char UCHAR;
typedef unsigned char u8;
typedef unsigned char UINT8;

typedef unsigned int UInt32;
typedef unsigned int u32;
typedef unsigned int UINT32;

typedef int int32;
typedef int Int32;
typedef int INT;
typedef int s32;
typedef int INT32;

typedef unsigned short UINT16;
typedef unsigned short UInt16;
typedef unsigned short u16;
typedef unsigned short uint16;
typedef unsigned short USHORT;

typedef short int16;
typedef short Int16;
typedef short s16;

typedef float f32;
typedef double f64;

typedef unsigned long long uint64;
typedef unsigned long long u64;

typedef long long int64;
typedef long long Int64;
typedef long long s64;

typedef void *  PVOID;
typedef void    VOID;

#ifndef NULL
#define NULL              0L
#endif

/* 定义bool类型的取值 */
#ifndef TRUE
#define TRUE   1
#define true   1
#endif
#ifndef FALSE
#define false  0
#define FALSE  0
#endif

typedef  sem_t                       OS_SEM;
typedef  pthread_mutex_t             OS_MUTEX;
#define SEM4_ERR   		((sem_t *) 0)
#define UIP_MALLOC 		malloc
#define UIP_FREE   		free
/* 屏蔽中断,关闭调度 */
#define UIP_PUSH_CRITICAL_REGION
/* 使能中断,使能调度 */
#define UIP_POP_CRITICAL_REGION
/* 不屏蔽中断,关闭调度 */
#define UIP_PUSH_UNSCHEDULE_REGION
/* 使能调度 */
#define UIP_POP_UNSCHEDULE_REGION

#endif /* __LINUX__ */

typedef enum
{
    /* 0 */
    UIP_OK = 0,                     /* 成功 */
	UIP_NULL_PTR,                   /* 检测到空指针 */
	UIP_INVALID_PARA,               /* 输入参数不合法 */
	UIP_MEM_ALLOC_FAILED,           /* 内存分配失败 */
	UIP_MEM_FREE_FAILED,            /* 内存释放失败 */

	/* 5 */
	UIP_CREATE_THREAD_FAILED,       /* 创建线程失败 */
	UIP_CREATE_SEM4_FAILED,         /* 创建信号量失败 */
	UIP_CREATE_MUTEX_FAILED,        /* 创建mutex失败 */
	UIP_CREATE_MSG_FAILED,          /* 创建消息失败 */
	UIP_CREATE_DEV_FLAG_FAILED,     /* 创建device flag失败 */

	/* 10 */
    UIP_NULL_CALLBACK_PTR,          /* 回调函数指针为空 */
    UIP_INTERFACE_TYPE,             /*接口类型错误*/
	UIP_ERR_CODE_BUTT               /* 错误码定义结束 */

}UIP_ERR_CODE_EN;

/* UIP各模块ID定义 */
typedef enum
{
    UIP_SYS,          /* UIP系统自身 */
    UIP_PB,           /* PictBridge模块 */
    UIP_SCP,          /* SCPI命令解析模块 */
    UIP_HELP,         /* 通用帮助模块 */
    UIP_LXI,          /* LXI模块 */
    UIP_USB,          /* USB模块 */
    UIP_GPIB,         /* GPIB模块 */
    UIP_USB2GPIB,     /* USB转GPIB模块 */
    UIP_USB2RS232,    /* USB转RS232模块 */
    UIP_UPGRADE,      /* 软件升级模块 */
    UIP_DEBUG,        /* 通用调试模块 */
    UIP_MODULE_BUTT
}UIP_MODULE_EN;

/* UIP各模块返回的细分错误码范围分配 */
#define UIP_ERR_CODE_RANGE 0x00010000
/* 举例: UIP_PB * UIP_ERR_CODE_RANGE 得到 PictBridge模块错误码起始值 */

#define PB_ERR_CODE_BASE    (UIP_PB * UIP_ERR_CODE_RANGE)
#define SCP_ERR_CODE_BASE   (UIP_SCP * UIP_ERR_CODE_RANGE)
#define HELP_ERR_CODE_BASE  (UIP_HELP * UIP_ERR_CODE_RANGE)
#define LXI_ERR_CODE_BASE   (UIP_LXI * UIP_ERR_CODE_RANGE)
#define USB_ERR_CODE_BASE   (UIP_USB * UIP_ERR_CODE_RANGE)
#define GPIB_ERR_CODE_BASE  (UIP_GPIB * UIP_ERR_CODE_RANGE)
#define USB2GPIB_ERR_CODE_BASE   (UIP_USB2GPIB * UIP_ERR_CODE_RANGE)
#define USB2RS232_ERR_CODE_BASE  (UIP_USB2RS232 * UIP_ERR_CODE_RANGE)
#define UPGRADE_ERR_CODE_BASE    (UIP_UPGRADE * UIP_ERR_CODE_RANGE)
#define DEBUG_ERR_CODE_BASE      (UIP_DEBUG * UIP_ERR_CODE_RANGE)
#define UIP_ERR_CODE_BUTT        (UIP_MODULE_BUTT * UIP_ERR_CODE_RANGE)


/**
 * \brief   升级错误
 */
#define NO_USB_DISK        0xF0000001
#define NO_UPGRADE_FILE    0xF0000002

#endif /* DATA_TYPE_H_ */
