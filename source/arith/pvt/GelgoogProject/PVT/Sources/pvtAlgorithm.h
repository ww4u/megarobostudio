/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtalgorithm.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.08.22;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __PVT_ALGORITHM_H
#define __PVT_ALGORITHM_H



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"



/****************************************�ⲿ��������*****************************************/



/*******************************************�궨��********************************************/
#define    OUTPUT_PERIOD_MIN              7    //���������Сֵ

#define    SQUARE(num)                    (num * num)
#define    CUBE(num)                      (num * num * num)
#define    PVT_TARGET_REAL_OFFSET_P       0.02f                            //����PVTÿ���ٶ�ʱ��ʵ�ʲ�����Ŀ�경��֮��������
#define    PVT_TARGET_REAL_OFFSET_N       PVT_TARGET_REAL_OFFSET_P * -1    //����PVTÿ���ٶ�ʱ��ʵ�ʲ�����Ŀ�경��֮��������
#define    LVT_TARGET_REAL_OFFSET_P       0.0003f                          //����LVTÿ���ٶ�ʱ��ʵ��������Ŀ������֮��������
#define    LVT_TARGET_REAL_OFFSET_N       LVT_TARGET_REAL_OFFSET_P * -1    //����LVTÿ���ٶ�ʱ��ʵ��������Ŀ������֮��������

#define    PVT_ONE_POINT_CALC_TIMES_MAX    1000    //����һ��������������

#define    SCALE_MAGNIFICATION             1000

//PVT���������
#define    PVT_CALC_NO_ERROR               0
#define    PVT_CALC_ERROR_TYPE             1
#define    PVT_CALC_ERROR_LEN              2
#define    PVT_CALC_ERROR_INDEX            3

#define    PVT_CALC_SECTION_1              0
#define    PVT_CALC_SECTION_2              16
#define    PVT_CALC_SECTION_3              32

#define    PVT_CALC_LESS_LOWER_LIMIT       1
#define    PVT_CALC_GREAT_UPER_LIMIT       2
#define    PVT_CALC_TOO_MANY_CALC_TIMES    3
#define    PVT_CALC_SPEED_EQUAL_ZERO       4



/***************************************��������������****************************************/
/*******************ö��*********************/
/*-----------PVT�滮ģʽö��-----------*/
typedef enum
{
    PLANMODE_CUBIC = 0,    //���ζ���ʽ��ֵ
    PLANMODE_TRAPEZOID,    //���β�ֵ
    PLANMODE_SCURVE,       //����
    
}PlanModeEnum;

/*-------------PVTģʽö��-------------*/
typedef enum
{
    EXECMODE_NCYCLE = 0,
    EXECMODE_FIFO,
    
}ExecModeEnum;

/*-------------�˶���ʽö��--------------*/
typedef enum
{
    MTNMODE_PVT  = 0,     //PVTģʽ
    MTNMODE_LVT_CORR,     //LVT������ģʽ
    MTNMODE_LVT_NOCORR,   //LVT��������ģʽ
    
}MotionModeEnum;

/*------------�����������ö��------------*/
typedef enum
{
    OUTPDATA_FORWARD = 2,    //������������
    OUTPDATA_REVERSE,        //������������
    OUTPDATA_WAIT,           //�ȴ�ʱ��
    
}OutpDataTypeEnum;

/*------------Buffer��������ö��------------*/
typedef enum
{
    BUFFOPERT_WRITE = 0,    //��˳��д��
    BUFFOPERT_FORCESEND,    //ǿ�Ʒ���
    BUFFOPERT_SYNCSEND,     //Ϊ��ͬ�����еķ���
    
}BufferOperateEnum;
/*******************ö��*********************/


typedef void (*OutpBufferFillFunc)(u8 chanNum, 
                                   OutpDataTypeEnum datatype, 
                                   u32 outpData, 
                                   BufferOperateEnum bufferOperate,
                                   void *pContext);


/******************�ṹ��********************/
/*-------------PVT��ṹ��-------------*/
typedef struct
{
    f32 position;    //λ��
    f32 speed;       //�ٶ�
    f32 time;        //ʱ��
    u16 accScale;    //����ռ��
    u16 decScale;    //����ռ��
      
}PvtPointStruct;

/*-------------λ��ת����Ϣ�ṹ��-----------*/
typedef struct
{
    f32 posnToStep;      //λ�õ�������ת��ϵ��
    f32 posnToLine;      //λ�õ�������������ת��ϵ�� 
    f32 lineSteps;       //�߼䲽
    u32 lineMult;        //�����ı���
    
}PosnConvertInfoStruct;

typedef struct
{
    bool           bQueryReady;    //�Ƿ��ѯFPGA Ready״̬
    bool           bReportCalcEnd; //�Ƿ��ϱ��˼������״̬��FIFOģʽ��ʹ�� CJ 2017.03.28 Add
    
    PlanModeEnum   pvtPlanMode;   //PVT�滮ģʽ
    ExecModeEnum   pvtExecMode;   //PVTִ��ģʽ
    
    MotionModeEnum   motionMode;     //�˶�ģʽ

    OutpDataTypeEnum lastStepDir;    //��һ���ķ���

    u16 lastPoint;      //�ϴν����PVT��ı��

    u32 fpgaPwmClock;      //FPGA PWMʱ��
    f32 fpgaClockOffset;   //FPGA PWMʱ��ƫ��

    u32 startPosn;      //һ��PVT����ʼλ��
    f32 startSpeed;     //һ��PVT����ʼ�ٶ�
    f32 startTime;      //һ��PVT����ʼʱ��
    u32 endPosn;        //һ��PVT�Ľ���λ��
    f32 endSpeed;       //һ��PVT�Ľ����ٶ�
    f32 motionTime;     //һ��PVT������ʱ��
    u32 motionStep;     //һ��PVT�����о���(΢����/����)
        
    f32 invsePosition;  //�ϴη���ʱ��λ�� 
    f32 waitPosition;   //�ϴεȴ�ʱ��λ��  
    f32 waitStepError;  //��Ϊ�ƶ����벻��һ������Ϊ�ȴ�ʱ��Ĳ������
    
    u32 targetStep;     //Ŀ�경��
    f32 lastStepSpeed;  //��һ�����ٶȣ�ʵ�ʼ�¼����ʱ��
    f32 maxOffset;      //��Ŀ�경����������ֵ
    f32 minOffset;      //��Ŀ�경����������ֵ

    f32 posnToStep;     //λ�õ�������ת��ϵ��
    f32 lineSteps;      //�߼䲽
    f32 lineStepsInv;   //�߼䲽����

    f32 errorTime;      //�������뵼�µ��ۼ����
    
    PvtPointStruct startPoint;     //���μ��������PVT��
    PvtPointStruct endPoint;       //���μ��������PVT��

    OutpBufferFillFunc  outpBufferFill;
    
    void *pContext;    //������ָ�룬��λ��ʹ��

    u64 timeCount;     //ͳ����

}PvtCalcStruct;
/******************�ṹ��********************/



/******************************************��������*******************************************/



/******************************************��������*******************************************/
//u8 pvtSegmentCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
