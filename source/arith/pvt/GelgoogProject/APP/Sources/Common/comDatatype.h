/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  comDatatype.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.08.22;
历史版本:  无;
*********************************************************************************************/
#ifndef __COM_DATA_TYPE_H
#define __COM_DATA_TYPE_H



/******************************************包含文件*******************************************/
#include <stdio.h>
#include "project.h"

#if defined PROJECT_QUBELEY
#include "stm32f10x.h"
#endif

#if defined PROJECT_GELGOOG
#include "stm32f4xx.h"
#endif



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/
/*typedef unsigned char         u8;     //无符号字符型
typedef char                  s8;     //有符号字符型
typedef unsigned short        u16;    //无符号短整型
typedef short                 s16;    //有符号短整型
typedef unsigned long         u32;    //无符号整型
typedef int                   s32;    //有符号整型*/
typedef long long             s64;    //有符号长整型
typedef unsigned long long    u64;    //无符号长整型
typedef float                 f32;    //单精度浮点
typedef long double           f64;    //双精度浮点

#ifndef __cplusplus

typedef unsigned char         bool;

/*******************************************宏定义********************************************/

#ifndef NULL
#define    NULL     0
#endif

#ifndef true
#define    true     1
#endif

#ifndef false
#define    false    0
#endif

#else

#endif

#define    COUNTOF(a)          (sizeof(a) / sizeof(*(a)))



/******************************************变量声明*******************************************/



/*****************************************头文件保护******************************************/
#endif



/*****************************************end of file****************************************/