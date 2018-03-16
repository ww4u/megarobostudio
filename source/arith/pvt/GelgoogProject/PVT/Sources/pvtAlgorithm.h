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
#include "comStruct.h"
#include "comEnum.h"



/****************************************�ⲿ��������*****************************************/



/*******************************************�궨��********************************************/
#define    OUTPUT_PERIOD_MIN              7    //���������Сֵ

#define    SQUARE(num)                    (num * num)
#define    CUBE(num)                      (num * num * num)
#define    TARGET_REAL_OFFSET_P           0.02f                        //����PVTÿ���ٶ�ʱ��ʵ�ʲ�����Ŀ�경��֮��������
#define    TARGET_REAL_OFFSET_N           TARGET_REAL_OFFSET_P * -1    //����PVTÿ���ٶ�ʱ��ʵ�ʲ�����Ŀ�경��֮��������

#define    PVT_ONE_POINT_CALC_TIMES_MAX    1000    //����һ��������������

//PVT���������
#define    PVT_CALC_NO_ERROR               0
#define    PVT_CALC_ERROR_TYPE             1
#define    PVT_CALC_ERROR_LEN              2
#define    PVT_CALC_ERROR_INDEX            3
#define    PVT_CALC_LESS_LOWER_LIMIT       4
#define    PVT_CALC_GREAT_UPER_LIMIT       5
#define    PVT_CALC_TOO_MANY_CALC_TIMES    6
#define    PVT_CALC_WRONG_ACC_PLAN         7



/***************************************��������������****************************************/
typedef struct
{
#if 0
    bool           bQueryReady;    //�Ƿ��ѯFPGA Ready״̬
    bool           bReportCalcEnd; //�Ƿ��ϱ��˼������״̬��FIFOģʽ��ʹ�� CJ 2017.03.28 Add
    
    WaveTableTypeEnum calcWaveTable;    //��ǰʹ�õĲ���
    
    PlanModeEnum   pvtPlanMode;   //PVT�滮ģʽ
    ExecModeEnum   pvtExecMode;   //PVTִ��ģʽ
    
    MotionModeEnum motionMode;     //�˶�ģʽ

    u16 lastPoint;      //�ϴμ���ĵ�ı��
    
    u32 fpgaPwmClock;   //FPGA PWMʱ��

    s32 pvtSteps;
    
    PvtPointStruct startPoint;     //���μ��������PVT��
    PvtPointStruct endPoint;       //���μ��������PVT��

    PosnConvertInfoStruct posnConvertInfo;

    OutpBufferFillFunc  outpBufferFill;
    
#else

    bool           bQueryReady;    //�Ƿ��ѯFPGA Ready״̬
    bool           bReportCalcEnd; //�Ƿ��ϱ��˼������״̬��FIFOģʽ��ʹ�� CJ 2017.03.28 Add
    
    WaveTableTypeEnum calcWaveTable;    //��ǰʹ�õĲ���
    
    PlanModeEnum   pvtPlanMode;   //PVT�滮ģʽ
    ExecModeEnum   pvtExecMode;   //PVTִ��ģʽ
    
    MotionModeEnum  motionMode;     //�˶�ģʽ

    DirectionEnum   lastStepDir;    //��һ���ķ���

    u16 lastPoint;      //�ϴμ���ĵ�ı��

    u16 accScale;       //S���߼��ٶ�ʱ��ռ��
    u16 decScale;       //S���߼��ٶ�ʱ��ռ��

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
    f32 fpgaPwmClock;   //FPGA PWMʱ��ƫ��
    
#else

    u32 fpgaPwmClock;   //FPGA PWMʱ��
#endif

    s32 pvtSteps;

    f32 startPosn;      //һ��PVT����ʼλ��
    f32 startSpeed;     //һ��PVT����ʼ�ٶ�
    f32 startTime;      //һ��PVT����ʼʱ��
    f32 endPosn;        //һ��PVT�Ľ���λ��
    f32 endSpeed;       //һ��PVT�Ľ����ٶ�
    f32 motionTime;     //һ��PVT������ʱ��
        
    f32 initPosition;   //PVT��ʼ���λ��
    f32 invsePosition;  //�ϴη���ʱ��λ�� 
    f32 waitPosition;   //�ϴεȴ�ʱ��λ��  
    f32 waitStepError;  //��Ϊ�ƶ����벻��һ������Ϊ�ȴ�ʱ��Ĳ������
    
    u32 targetStep;     //Ŀ�경��
    f32 lastStepSpeed;  //��һ�����ٶȣ�ʵ�ʼ�¼����ʱ��
    f32 lastStepTime;   //��һ�����������PVT��0ʱ�̵�ʱ��(PVT��ĵ�һ����ʱ������Ϊ0)
    
    u32 targetLine;     //Ŀ������
    f32 lastLineSpeed;  //��һ�ߵ��ٶȣ�ʵ�ʼ�¼����ʱ��
    f32 lastLineTime;   //��һ�����������PVT��0ʱ�̵�ʱ��(PVT��ĵ�һ����ʱ������Ϊ0)
    f32 remainLine;     //��ǰ�߼��㲽����ʣ�µ�����
    f32 lineErrorP;     //Ŀ��ֵ��ʵ��ֵ�������
    f32 lineErrorN;     //Ŀ��ֵ��ʵ��ֵ�ĸ����
    
    f32 errorTime;      //�������뵼�µ��ۼ����
    
    PvtPointStruct startPoint;     //���μ��������PVT��
    PvtPointStruct endPoint;       //���μ��������PVT��

    PosnConvertInfoStruct posnConvertInfo;

    OutpBufferFillFunc  outpBufferFill;
    
    void *pContext;    //������ָ�룬��λ��ʹ��
    
#endif

}PvtCalcStruct;

/*-------------���β�ֵ��ģʽö��-------------*/
typedef enum
{
    TRAPEZOID_ERROR = 0,    //����ģʽ
    TRAPEZOID_INTERP,       //��ֵģʽ
    TRAPEZOID_UNIFORM       //����ģʽ
    
}TrapezoidModeEnum;



/******************************************��������*******************************************/



/******************************************��������*******************************************/
u8   pvtSegmentCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum);
void pvtPosnConvCoeffCalc(MotorManageStruct motorInfo, u8 drvMres, PosnConvertInfoStruct *pPosnConvertInfo);
s16  pvtStepToCircleCalc(s32 totalSteps, f32 convertCoeff, PostUnitEnum  posnUnit);



/*****************************************ͷ�ļ�����******************************************/
#endif
/*******************************************�ļ�β********************************************/
