/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtalgorithm.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.08.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arm_math.h"
#include "pvtAlgorithm.h"
#include "pvtCubicSolution.h"
#include "pvtLinearSolution.h"
#include "pvtTrapezSolution.h"
#include "pvtUniformSolution.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    PVT_FIFO_REPORT_CALC_NUM    5    //FIFOģʽ�¿�ʼ�ϱ��������״̬���Ѽ������
#define    INVERSE_DBL_RADIAN          0.1591549f    //2�з�֮һ����ȡֵ3.1415926
#define    DEGREES_OF_A_CIRCLE         360           //ԲһȦ�Ķ���
#define    SHORT_INT_VALUE_MIN         -32768
#define    SHORT_INT_VALUE_MAX         32767



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
bool bCalibrate = true;    //��ʱ���У׼���� NICK MARK



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvtPosnConvCoeffCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void pvtPosnConvCoeffCalc(MotorManageStruct motorInfo, u8 drvMres, PosnConvertInfoStruct *pPosnConvertInfo)
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
    if (ENCODER_INC == motorInfo.encoderType)
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
            posnToLine = (f32)lineNum * INVERSE_DBL_RADIAN;
        }
        else if (POSTUNIT_ANGLE == motorInfo.posnUnit)
        {

            //ÿ���ϵ�����
            posnToLine = (f32)lineNum / DEGREES_OF_A_CIRCLE;            
        }
        else if (POSTUNIT_MMETER == motorInfo.posnUnit)
        {
            //ÿ�����ϵ�����
            posnToLine = (f32)lineNum / motorInfo.lead;
        }
    }
    else
    {
        posnToLine = 0;
        lineNum    = 1;
    }
    
    pPosnConvertInfo->posnToLine = posnToLine;
    /*---------------------------����λ�õ�������������ת��ϵ��------------------------------------*/

    //�߼䲽
#if 1
    pPosnConvertInfo->lineSteps = (f32)steps * (motorInfo.feedbackRatio + 1) / lineNum;    //��1����ΪlineMult�����Ǽ���1��
    pPosnConvertInfo->lineMult = motorInfo.feedbackRatio;
    
#else

    if (steps > lineNum)
    {
        pPosnConvertInfo->lineSteps = steps / lineNum;
        pPosnConvertInfo->lineMult = 1;
    }
    else
    {
        pPosnConvertInfo->lineSteps = steps;
        pPosnConvertInfo->lineMult = lineNum;
        
        //����steps��lineNum�����Լ��
        while ((steps != 0) && (lineNum != 0))
        {
            if (steps > lineNum)
            {
                steps = steps % lineNum;
            }
            else
            {
                lineNum = lineNum % steps;
            }
        }

        if (0 == steps)
        {
            Multiple = lineNum;
        }
        else
        {
            Multiple = steps;
        }

        //����Ĵ�����ֵ
        pPosnConvertInfo->lineSteps /= Multiple;
        pPosnConvertInfo->lineMult /= Multiple;

        if (pPosnConvertInfo->lineMult > 256)
        {
            //TO ADD NICK MARK
            //��������Multiple����֤lineMult <= 256����lineStepsReg > 1;
        }
    }
#endif
}


/*********************************************************************************************
�� �� ��: pvtStepToCircleCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
s16 pvtStepToCircleCalc(s32 totalSteps, f32 convertCoeff, PostUnitEnum  posnUnit)
{
    s32 stepsToCircle;
    s16 circle;
    
    
    //����PVTλ�õĵ�λ����һȦ�Ĳ���
    if (POSTUNIT_RADIAN == posnUnit)
    {
        //����΢����������Ǻͼ��ٱȼ��㣬��ת�˶���ÿ�����ϵĲ���
        stepsToCircle = (s32)((f32)totalSteps * INVERSE_DBL_RADIAN / convertCoeff);
    }
    else if (POSTUNIT_ANGLE == posnUnit)
    {

        //����΢����������Ǻͼ��ٱȼ��㣬��ת�˶���ÿ���ϵĲ���
        stepsToCircle = (s32)((f32)totalSteps / (convertCoeff * DEGREES_OF_A_CIRCLE));
    }
    else if (POSTUNIT_MMETER == posnUnit)
    {
        //����΢����������Ǻ͵��̼��㣬ֱ���˶���ÿ�����ϵĲ���
        stepsToCircle = (s32)((f32)totalSteps / convertCoeff);
    }

    //��������
    if (stepsToCircle > SHORT_INT_VALUE_MAX)
    {
        circle = SHORT_INT_VALUE_MAX;
    }
    else if (stepsToCircle < SHORT_INT_VALUE_MIN)
    {
        circle = SHORT_INT_VALUE_MIN;
    }
    else
    {
       circle = stepsToCircle;
    }

    return circle;
}


/*********************************************************************************************
�� �� ��: pvtPointCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvtPointCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;


    //���ݹ滮��ʽ����Ԥ����
    switch (pPvtCalcData->pvtPlanMode)
    {
        /***************************************************************
        ���ζ���ʽ��ֵ
        ***************************************************************/
        case PLANMODE_CUBIC:
            errorCode = pvtCubicCurveCalc(chanNum, pPvtCalcData);
          break;
          
          
        /***************************************************************
        ���Բ�ֵ
        ***************************************************************/
        case PLANMODE_LINEAR:
            errorCode = pvtLinearCurveCalc(chanNum, pPvtCalcData);
          break;

        /***************************************************************
        ���β�ֵ
        ***************************************************************/
        case PLANMODE_TRAPEZOID:
            errorCode = pvtTrapezCurveCalc(chanNum, pPvtCalcData);
          break;
          
          
        /***************************************************************
        ��ζ���ʽ��ֵ
        ***************************************************************/
        case PLANMODE_QUINTIC:
          break;

          
        /***************************************************************
        ����
        ***************************************************************/
        case PLANMODE_SINE:
          break;

        /***************************************************************
        ����
        ***************************************************************/
        case PLANMODE_UNIFORM:
            errorCode = pvtUniformCurveCalc(chanNum, pPvtCalcData);
          break;

        default:
          break;
    }

    return errorCode;
}


/*********************************************************************************************
�� �� ��: pvtLineCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvtLineCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum)
{
    bool bHaveRemain = false;
    u8   errorCode = PVT_CALC_NO_ERROR;
    u32  i;

    f32 calcOutput;
    f64 curTime = 0;    //��ʱ�ĳ�64λ    NICK MARK
    f32 errorTime;

    u16 calcCount = 0;
    f64 lastTime;       //��ʱ�ĳ�64λ    NICK MARK
    f64 maxTime;        //��ʱ�ĳ�64λ    NICK MARK
    f64 minTime;        //��ʱ�ĳ�64λ    NICK MARK

    f32 cubeMotionTime;
    f32 squareMotionTime;
    f32 curPost;   
    f32 threeTermCoef;
    f32 twoTermCoef;

    f32 posnError;


    //ת�����٣���ȡ��ʼ��ֹ��λ�ú��ٶ�
    u32 startPost = (u32)(pPvtCalcData->startPosn * pPvtCalcData->posnConvertInfo.posnToLine + 0.5f);
    u32 endPost   = (u32)(pPvtCalcData->endPosn   * pPvtCalcData->posnConvertInfo.posnToLine + 0.5f);

    
    f32 startSpeed = fabs(pPvtCalcData->startSpeed * pPvtCalcData->posnConvertInfo.posnToLine);
    f32 endSpeed   = fabs(pPvtCalcData->endSpeed * pPvtCalcData->posnConvertInfo.posnToLine); 
    
#if 1    //Ŀǰ��ֻ֧���˶�����������    NICK MARK
    u32 poseOffset = abs(endPost - startPost);
#else
    f32 poseOffset = fabs(pPvtCalcData->endPosn - pPvtCalcData->startPosn) * pPvtCalcData->convertCoeff;

    //���Ҫ���е��������������Ļ�����¼����ʣ��Ĳ���1�ߵ�ֵ
    u32 lineNum = (u32)poseOffset;
    if (lineNum != poseOffset)
    {
        bHaveRemain = true;
        pPvtCalcData->.remainLine = poseOffset - lineNum;
    }
#endif

    if (DIR_FORWARD == pPvtCalcData->lastStepDir)
    {
        pPvtCalcData->pvtSteps += poseOffset;
    }
    else
    {
        pPvtCalcData->pvtSteps -= poseOffset;
    }

    //��ʼ��ʱ��
    minTime = 0;
    if ((1 == pPvtCalcData->targetLine) ||   //��һ����ʱ����Ҫ��ȫʱ�䷶Χ�ڲ���
        (0.0f == startSpeed))                  //���߳�ʼ�ٶ�Ϊ0����Ϊ����֮���ٶȱ仯�ϴ���������ȫʱ�䷶Χ�ڲ���
    {
        if (poseOffset != 0)
        {
            curTime = pPvtCalcData->motionTime / poseOffset;
            maxTime = pPvtCalcData->motionTime;
        }

        pPvtCalcData->lastLineTime = pPvtCalcData->startTime;
        pPvtCalcData->lastLineSpeed = 0;
        lastTime = 0;
    }
    else
    {
        lastTime = pPvtCalcData->lastLineTime - pPvtCalcData->startTime;
        curTime = lastTime;
        maxTime = curTime + pPvtCalcData->lastLineSpeed * 3.0f;
    }
    
    //��ǰ���������Ͷ������ϵ��
    cubeMotionTime = CUBE(pPvtCalcData->motionTime);
    squareMotionTime = SQUARE(pPvtCalcData->motionTime);
    threeTermCoef = ((startSpeed + endSpeed) / squareMotionTime) - (poseOffset * 2.0f / cubeMotionTime);
    twoTermCoef = (poseOffset * 3.0f / squareMotionTime) - (startSpeed * 2.0f + endSpeed) / pPvtCalcData->motionTime;

    pPvtCalcData->targetLine -= startPost;
    for (i = 0;i < poseOffset;i++)
    {
        calcCount = 0;       
        curTime += pPvtCalcData->lastLineSpeed;    //����һ����ʱ�����ϼ�����һ�����ٶ���Ϊ��һ���ļ����ʼʱ��
        if (curTime > pPvtCalcData->motionTime)
        {
            curTime = pPvtCalcData->motionTime;
        }

        curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + startSpeed) * curTime;
        
        posnError = curPost - pPvtCalcData->targetLine;
        while ((posnError > pPvtCalcData->lineErrorP) ||    //���С��1΢������    NICK MARK
               (posnError < pPvtCalcData->lineErrorN))
        {
            calcCount++;
            if (calcCount > PVT_ONE_POINT_CALC_TIMES_MAX)
            {
                //�������������������㣬����
                errorCode = PVT_CALC_TOO_MANY_CALC_TIMES;
                break;
            }
            
            if (posnError > pPvtCalcData->lineErrorP)
            {
                maxTime = curTime;
                curTime = (curTime + minTime) * 0.5f;
            }
            else
            {
                minTime = curTime;
                curTime = (curTime + maxTime) * 0.5f;
            }
            curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + startSpeed) * curTime;
            
            posnError = curPost - pPvtCalcData->targetLine;
        }

        //������������������������㣬����
        if (PVT_CALC_TOO_MANY_CALC_TIMES == errorCode)
        {
            break;
        }
        
        pPvtCalcData->targetLine++;    //��ʼ������һ��Ŀ��
        
        pPvtCalcData->lastLineSpeed = curTime - lastTime;    //��¼��һ������һ���ļ��ʱ�䣬Ҳ�����ٶ�

        minTime = curTime;
        lastTime = curTime;
        maxTime = curTime + pPvtCalcData->lastLineSpeed * 3.0f;
        if (maxTime > pPvtCalcData->motionTime)
        {
            maxTime = pPvtCalcData->motionTime;
        }

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
        //��ʱ�䰴���߼䲽ƽ��
        calcOutput = (pPvtCalcData->lastLineSpeed * FPGA_PWM_CLOCK + 
                      pPvtCalcData->lastLineSpeed * pPvtCalcData->fpgaPwmClock + 
                      pPvtCalcData->errorTime) 
                      * (pPvtCalcData->posnConvertInfo.lineMult + 1)    //��1����ΪlineMult�����Ǽ���1��
                      / pPvtCalcData->posnConvertInfo.lineSteps;

        //���������ޱ���
        if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
        {
            calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_GREAT_UPER_LIMIT;
        }
        
#else

        //��ʱ�䰴���߼䲽ƽ��
        calcOutput = (pPvtCalcData->lastLineSpeed * pPvtCalcData->fpgaPwmClock + pPvtCalcData->errorTime) 
                      * (pPvtCalcData->posnConvertInfo.lineMult + 1)    //��1����ΪlineMult�����Ǽ���1��
                      / pPvtCalcData->posnConvertInfo.lineSteps;

        //�����ٶȱ���

        if (calcOutput > pPvtCalcData->fpgaPwmClock)
        {
            calcOutput = pPvtCalcData->fpgaPwmClock;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_GREAT_UPER_LIMIT;
        } 
#endif
        else if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_LESS_LOWER_LIMIT;
        }

        //ͳ���¹�һ��������������ɵ����
        errorTime = calcOutput - (u32)calcOutput;
        pPvtCalcData->errorTime = errorTime * pPvtCalcData->posnConvertInfo.lineSteps;

        pPvtCalcData->outpBufferFill(chanNum, (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              pPvtCalcData->pContext);
    }

    pPvtCalcData->targetLine += startPost;
    pPvtCalcData->lastLineTime = curTime + pPvtCalcData->startTime;
    
    //�������һ��
    if (bHaveRemain)
    {
#if PVT_CALC_USE_FPGA_CLOCK_ERROR
        //��ʱ�䰴���߼䲽ƽ��
        calcOutput = ((pPvtCalcData->motionTime - curTime) * FPGA_PWM_CLOCK + 
                      (pPvtCalcData->motionTime - curTime) * pPvtCalcData->fpgaPwmClock) 
                     * (pPvtCalcData->posnConvertInfo.lineMult + 1)    //��1����ΪlineMult�����Ǽ���1��
                     / (pPvtCalcData->remainLine * pPvtCalcData->posnConvertInfo.lineSteps);

        //�����ٶȱ���
        if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_LESS_LOWER_LIMIT;
        }
        
        
#else

        calcOutput = (pPvtCalcData->motionTime - curTime) * pPvtCalcData->fpgaPwmClock 
                     * (pPvtCalcData->posnConvertInfo.lineMult + 1)    //��1����ΪlineMult�����Ǽ���1��
                     / (pPvtCalcData->remainLine * pPvtCalcData->posnConvertInfo.lineSteps);

        //�����ٶȱ���
        if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_LESS_LOWER_LIMIT;
        }
#endif
        //ͳ���¹�һ��������������ɵ����
        errorTime = calcOutput - (u32)calcOutput;
        pPvtCalcData->errorTime = errorTime * pPvtCalcData->remainLine * pPvtCalcData->posnConvertInfo.lineSteps;

        pPvtCalcData->outpBufferFill(chanNum, (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              pPvtCalcData->pContext);
    }

    return errorCode;
}


/*********************************************************************************************
�� �� ��: pvtSegmentCalc
ʵ�ֹ���: ����һ��PVT(����PVT��֮�������΢����)���˺���������Ҫ�ǶԼ������Ԥ����֮����
          pvtPointCalc�������PVT��֮������΢�����ļ���
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
u8 pvtSegmentCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum)
{
    u8  errorCode = PVT_CALC_NO_ERROR;
    DirectionEnum direction;
    bool bCalcPvt = false;    
    
    f32 motionStep;
    
    f32 startPosn;
    f32 startSpeed;
    f32 endPosn;
    f32 endSpeed;
    f32 startTime;
    f32 motionTime;
    

    //�������һ����ż���
    if (pPvtCalcData->endPoint.time != -1.0f)
    {
        //��ȡ��ǰҪ�������һ��PVT���������
        startPosn  = pPvtCalcData->startPoint.position;
        startSpeed = pPvtCalcData->startPoint.speed;
        startTime  = pPvtCalcData->startPoint.time;
        
        endPosn  = pPvtCalcData->endPoint.position;
        endSpeed = pPvtCalcData->endPoint.speed;
        
        motionTime = pPvtCalcData->endPoint.time - startTime;

        //�жϷ���
        if (endPosn < startPosn)
        {
            direction = DIR_REVERSE;
        }
        else
        {
            direction = DIR_FORWARD;
        }
        
        if (direction != pPvtCalcData->lastStepDir)    //�����˶�
        {
            //����֮��Ŀ�경����Ϊ1�����·���
            pPvtCalcData->targetStep = 1;
            pPvtCalcData->targetLine = 1;
            pPvtCalcData->lastStepDir = direction;

            pPvtCalcData->waitStepError = 0;

            //����֮���¼�·����λ��
            pPvtCalcData->invsePosition = pPvtCalcData->startPoint.position;
        }

        //���ڷ���λ�ü��������ʼλ��
        startPosn = fabs(pPvtCalcData->startPoint.position - pPvtCalcData->invsePosition);
        endPosn   = fabs(pPvtCalcData->endPoint.position - pPvtCalcData->invsePosition);

        if (MTNMODE_PVT == pPvtCalcData->motionMode)
        {
            motionStep  = (endPosn - startPosn) * pPvtCalcData->posnConvertInfo.posnToStep;
        }
        else
        {
            motionStep  = (endPosn - startPosn) * pPvtCalcData->posnConvertInfo.posnToLine;

            pPvtCalcData->lineErrorP = 0.0003f;
            pPvtCalcData->lineErrorN = -0.0003f;
        }
        motionStep += pPvtCalcData->waitStepError;    //�ۼ��²���һ���Ĳ���   

        //λ�ñ仯����1���ż��㣬������Ϊ�ȴ�ʱ��
        if (motionStep >= 1.0f)
        {                        
            //��Ϊ�ȴ��Ĳ�������1���˾�Ҫ����һ���߳���
            if (pPvtCalcData->waitStepError > 0.0f)
            {
                pPvtCalcData->waitStepError = 0;
                startPosn = pPvtCalcData->waitPosition;
            }
            
            //����һ��PVT
            pPvtCalcData->startPosn   = startPosn;
            pPvtCalcData->startSpeed  = startSpeed;
            pPvtCalcData->startTime   = startTime;
            pPvtCalcData->endPosn     = endPosn;
            pPvtCalcData->endSpeed    = endSpeed;
            pPvtCalcData->motionTime  = motionTime;
            
            //���У׼�ˣ�Ҳ�ͱ�Ȼ�б�����
            if (bCalibrate)
            {
                //LVTģʽ
                if (MTNMODE_PVT != pPvtCalcData->motionMode)
                {
                    pvtLineCalc(pPvtCalcData, chanNum);
                }
                else
                {
                    bCalcPvt = true;
                }
            }
            else    //û��У׼�Ļ�����Ҫ���ձ�����ÿ��֮��Ļ��Ƚ���ʼPVT���зֶ�
            {
                bCalcPvt = true;
            }
    
            if (bCalcPvt)
            {
                pvtPointCalc(chanNum, pPvtCalcData);
            }
        }
        else
        {
            //�����һ�ο�ʼ�ȴ���λ��
            if (0.0f == pPvtCalcData->waitStepError)
            {
                pPvtCalcData->waitPosition = startPosn;
            }
            pPvtCalcData->waitStepError += motionStep;

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
            motionTime = motionTime * FPGA_PWM_CLOCK + motionTime * pPvtCalcData->fpgaPwmClock;
#else

            motionTime *= pPvtCalcData->fpgaPwmClock;
#endif
            motionTime += 0.5f;    //��������
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, (u32)motionTime, BUFFOPERT_WRITE, pPvtCalcData->pContext);
        }
    }
    
    //���һ������ǿ�Ʒ������������ݣ������FIFOģʽ�㹻�˵�����ֻ֪ͨFUNC�ϱ��������״̬
    if ((-1.0f == pPvtCalcData->endPoint.time) ||
        ((EXECMODE_FIFO == pPvtCalcData->pvtExecMode) &&        //FIFOģʽ���㹻�˵����һ�δ�ϱ�    CJ 2017.03.28 Add
         (false == pPvtCalcData->bReportCalcEnd) &&
         (pPvtCalcData->lastPoint >= PVT_FIFO_REPORT_CALC_NUM)))
    {
        //֪ͨFUNC��ѯFPGA��Ready״̬
        if ((EXECMODE_FIFO != pPvtCalcData->pvtExecMode) ||    //FIFOģʽ���Ѿ��ϱ�������Ҫ�ϱ�    CJ 2017.03.28 Modify
            (false == pPvtCalcData->bReportCalcEnd))   
        {
            pPvtCalcData->bQueryReady = true;
            pPvtCalcData->bReportCalcEnd = true;
        }

        //���һ�������΢��ģʽ����Ҫǿ�Ʒ���Buffer�е����ݺ���չ�������
        if (-1.0f == pPvtCalcData->endPoint.time)    //CJ 2017.03.28 Modify
        {
            //ǿ�Ʒ���Buffer�е����ݣ��ܵĲ������ݵ���m_stProcessData.targetStep - 1
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_FORWARD, 0, BUFFOPERT_FORCESEND, pPvtCalcData->pContext);
        }
    }

    return errorCode;
}



/*******************************************�ļ�β********************************************/
