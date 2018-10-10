/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servAlgorithm.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2018.05.31;
历史版本:  无;
*********************************************************************************************/
#include "servAlgorithm.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    INVERSE_DBL_RADIAN          0.1591549f    //2π分之一，π取值3.1415926
#define    DEGREES_OF_A_CIRCLE         360           //圆一圈的度数



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servPosnConvCoeffCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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

    
    /*----------------------------计算位置到微步数的转换系数---------------------------------------*/
    //计算微步数
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

    //根据步距角计算步数
    switch (motorInfo.stepAngel)
    {
        case STEPANGLE_18:
            steps = 200;    //360°/1.8°
          break;
          
        case STEPANGLE_09:
            steps = 400;    //360°/0.9°
          break;
          
        case STEPANGLE_150:
            steps = 24;    //360°/15°
          break;
          
        case STEPANGLE_75:
            steps = 48;    //360°/7.5°
          break;

        default:
          break;
    }
    steps *= microSteps;
    
    //根据PVT位置的单位计算一圈的步数
    if (POSTUNIT_RADIAN == motorInfo.posnUnit)
    {
        steps = steps * motorInfo.gearRatioNum / motorInfo.gearRatioDen;
        
        //根据微步数、步距角和减速比计算，旋转运动是每弧度上的步数
        posnToStep = (f32)steps * INVERSE_DBL_RADIAN;
    }
    else if (POSTUNIT_ANGLE == motorInfo.posnUnit)
    {
        steps = steps * motorInfo.gearRatioNum / motorInfo.gearRatioDen;
        
        //根据微步数、步距角和减速比计算，旋转运动是每度上的步数
        posnToStep = (f32)steps / DEGREES_OF_A_CIRCLE;            
    }
    else if (POSTUNIT_MMETER == motorInfo.posnUnit)
    {
        //根据微步数、步距角和导程计算，直线运动是每毫米上的步数
        posnToStep = (f32)steps / motorInfo.lead;
    }
    
    pPosnConvertInfo->posnToStep = posnToStep;
    /*----------------------------计算位置到微步数的转换系数---------------------------------------*/

    
    
    /*---------------------------计算位置到编码器线数的转换系数------------------------------------*/
    //增量型编码器才需要计算
    if ((ENCODER_INC == motorInfo.encoderType) && (INTFC_NONE != motorInfo.encoderState))
    {
        //计算倍数
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

        //计算线数
        lineNum = Multiple * motorInfo.encoderLineNum;

        if (POSTUNIT_RADIAN == motorInfo.posnUnit)
        {
            //每弧度上的线数
            posnToLine = (f32)lineNum * INVERSE_DBL_RADIAN / motorInfo.feedbackRatio;
        }
        else if (POSTUNIT_ANGLE == motorInfo.posnUnit)
        {

            //每度上的线数
            posnToLine = (f32)lineNum / (DEGREES_OF_A_CIRCLE * motorInfo.feedbackRatio);            
        }
        else if (POSTUNIT_MMETER == motorInfo.posnUnit)
        {
            //每毫米上的线数
            posnToLine = (f32)lineNum / (motorInfo.lead * motorInfo.feedbackRatio);
        }

        //线间步
        pPosnConvertInfo->lineSteps = (f32)steps * (motorInfo.feedbackRatio) / lineNum;

        //FPGA要求最小值为2
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
    /*---------------------------计算位置到编码器线数的转换系数------------------------------------*/
}



/*******************************************文件尾********************************************/
