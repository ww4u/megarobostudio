/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servAlgorithm.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2018.05.31;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "servAlgorithm.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    INVERSE_DBL_RADIAN          0.1591549f    //2�з�֮һ����ȡֵ3.1415926
#define    DEGREES_OF_A_CIRCLE         360           //ԲһȦ�Ķ���



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: servPosnConvCoeffCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servPosnConvCoeffCalc(MotorManageStruct motorInfo, u8 drvMres, PosnConvertInfoStruct *pPosnConvertInfo)
{
    u8  i;
    u32 Multiple;
    u16 microSteps = 1;
    u32 steps;
    u32 lineNum;
    f32 posnToStep;
    f32 posnToLine;

    
    /*----------------------------����λ�õ�΢������ת��ϵ��---------------------------------------*/
    //����΢����
    if (drvMres <= MICROSTEP_1)
    {
        for (i = 0;i <= (u8)drvMres;i++)
        {
            microSteps = 256 >> i;
        }
    }
    else
    {
        microSteps = 1;
    }

    //���ݲ���Ǽ��㲽��
    switch (motorInfo.stepAngel)
    {
        case STEPANGLE_18:
            steps = 200;    //360��/1.8��
          break;
          
        case STEPANGLE_09:
            steps = 400;    //360��/0.9��
          break;
          
        case STEPANGLE_150:
            steps = 24;    //360��/15��
          break;
          
        case STEPANGLE_75:
            steps = 48;    //360��/7.5��
          break;

        default:
          break;
    }
    steps *= microSteps;
    
    //����PVTλ�õĵ�λ����һȦ�Ĳ���
    if (POSTUNIT_RADIAN == motorInfo.posnUnit)
    {
        steps = steps * motorInfo.gearRatioNum / motorInfo.gearRatioDen;
        
        //����΢����������Ǻͼ��ٱȼ��㣬��ת�˶���ÿ�����ϵĲ���
        posnToStep = (f32)steps * INVERSE_DBL_RADIAN;
    }
    else if (POSTUNIT_ANGLE == motorInfo.posnUnit)
    {
        steps = steps * motorInfo.gearRatioNum / motorInfo.gearRatioDen;
        
        //����΢����������Ǻͼ��ٱȼ��㣬��ת�˶���ÿ���ϵĲ���
        posnToStep = (f32)steps / DEGREES_OF_A_CIRCLE;            
    }
    else if (POSTUNIT_MMETER == motorInfo.posnUnit)
    {
        //����΢����������Ǻ͵��̼��㣬ֱ���˶���ÿ�����ϵĲ���
        posnToStep = (f32)steps / motorInfo.lead;
    }
    
    pPosnConvertInfo->posnToStep = posnToStep;
    /*----------------------------����λ�õ�΢������ת��ϵ��---------------------------------------*/

    
    
    /*---------------------------����λ�õ�������������ת��ϵ��------------------------------------*/
    //�����ͱ���������Ҫ����
    if ((ENCODER_INC == motorInfo.encoderType) && (INTFC_NONE != motorInfo.encoderState))
    {
        //���㱶��
        switch (motorInfo.encoderMult)
        {
            case ENCMULT_SINGLE:
                Multiple = 1;
              break;
              
            case ENCMULT_DOUBLE:
                Multiple = 2;
              break;

            case ENCMULT_QUADR:
                Multiple = 4;
              break;

            default:
              break;
        }

        //��������
        lineNum = Multiple * motorInfo.encoderLineNum;

        if (POSTUNIT_RADIAN == motorInfo.posnUnit)
        {
            //ÿ�����ϵ�����
            posnToLine = (f32)lineNum * INVERSE_DBL_RADIAN / motorInfo.feedbackRatio;
        }
        else if (POSTUNIT_ANGLE == motorInfo.posnUnit)
        {

            //ÿ���ϵ�����
            posnToLine = (f32)lineNum / (DEGREES_OF_A_CIRCLE * motorInfo.feedbackRatio);            
        }
        else if (POSTUNIT_MMETER == motorInfo.posnUnit)
        {
            //ÿ�����ϵ�����
            posnToLine = (f32)lineNum / (motorInfo.lead * motorInfo.feedbackRatio);
        }

        //�߼䲽
        pPosnConvertInfo->lineSteps = (f32)steps * (motorInfo.feedbackRatio) / lineNum;

        //FPGAҪ����СֵΪ2
        if (pPosnConvertInfo->lineSteps < 2.0f)
        {
            pPosnConvertInfo->lineSteps = 2.0f;
        }

        pPosnConvertInfo->lineMult = motorInfo.feedbackRatio;
    }
    else
    {
        posnToLine = 1;
    }
    
    pPosnConvertInfo->posnToLine = posnToLine;
    /*---------------------------����λ�õ�������������ת��ϵ��------------------------------------*/
}



/*******************************************�ļ�β********************************************/
