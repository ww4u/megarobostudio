/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  comDatatype.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.08.22;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __COM_DATA_TYPE_H
#define __COM_DATA_TYPE_H



/******************************************�����ļ�*******************************************/
#include <stdio.h>
#include "project.h"

#if defined PROJECT_QUBELEY
#include "stm32f10x.h"
#endif

#if defined PROJECT_GELGOOG
#include "stm32f4xx.h"
#endif



/****************************************�ⲿ��������*****************************************/



/***************************************��������������****************************************/
/*typedef unsigned char         u8;     //�޷����ַ���
typedef char                  s8;     //�з����ַ���
typedef unsigned short        u16;    //�޷��Ŷ�����
typedef short                 s16;    //�з��Ŷ�����
typedef unsigned long         u32;    //�޷�������
typedef int                   s32;    //�з�������*/
typedef long long             s64;    //�з��ų�����
typedef unsigned long long    u64;    //�޷��ų�����
typedef float                 f32;    //�����ȸ���
typedef long double           f64;    //˫���ȸ���

#ifndef __cplusplus

typedef unsigned char         bool;

/*******************************************�궨��********************************************/

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



/******************************************��������*******************************************/



/*****************************************ͷ�ļ�����******************************************/
#endif



/*****************************************end of file****************************************/