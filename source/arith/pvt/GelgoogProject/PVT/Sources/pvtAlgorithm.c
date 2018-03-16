/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtalgorithm.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.08.15;
历史版本:  无;
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



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    PVT_FIFO_REPORT_CALC_NUM    5    //FIFO模式下开始上报计算结束状态的已计算点数
#define    INVERSE_DBL_RADIAN          0.1591549f    //2π分之一，π取值3.1415926
#define    DEGREES_OF_A_CIRCLE         360           //圆一圈的度数
#define    SHORT_INT_VALUE_MIN         -32768
#define    SHORT_INT_VALUE_MAX         32767



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
bool bCalibrate = true;    //暂时替代校准数据 NICK MARK



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvtPosnConvCoeffCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
    if (ENCODER_INC == motorInfo.encoderType)
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
            posnToLine = (f32)lineNum * INVERSE_DBL_RADIAN;
        }
        else if (POSTUNIT_ANGLE == motorInfo.posnUnit)
        {

            //每度上的线数
            posnToLine = (f32)lineNum / DEGREES_OF_A_CIRCLE;            
        }
        else if (POSTUNIT_MMETER == motorInfo.posnUnit)
        {
            //每毫米上的线数
            posnToLine = (f32)lineNum / motorInfo.lead;
        }
    }
    else
    {
        posnToLine = 0;
        lineNum    = 1;
    }
    
    pPosnConvertInfo->posnToLine = posnToLine;
    /*---------------------------计算位置到编码器线数的转换系数------------------------------------*/

    //线间步
#if 1
    pPosnConvertInfo->lineSteps = (f32)steps * (motorInfo.feedbackRatio + 1) / lineNum;    //加1是因为lineMult本身是减了1的
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
        
        //计算steps和lineNum的最大公约数
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

        //配给寄存器的值
        pPosnConvertInfo->lineSteps /= Multiple;
        pPosnConvertInfo->lineMult /= Multiple;

        if (pPosnConvertInfo->lineMult > 256)
        {
            //TO ADD NICK MARK
            //重新设置Multiple，保证lineMult <= 256，且lineStepsReg > 1;
        }
    }
#endif
}


/*********************************************************************************************
函 数 名: pvtStepToCircleCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
s16 pvtStepToCircleCalc(s32 totalSteps, f32 convertCoeff, PostUnitEnum  posnUnit)
{
    s32 stepsToCircle;
    s16 circle;
    
    
    //根据PVT位置的单位计算一圈的步数
    if (POSTUNIT_RADIAN == posnUnit)
    {
        //根据微步数、步距角和减速比计算，旋转运动是每弧度上的步数
        stepsToCircle = (s32)((f32)totalSteps * INVERSE_DBL_RADIAN / convertCoeff);
    }
    else if (POSTUNIT_ANGLE == posnUnit)
    {

        //根据微步数、步距角和减速比计算，旋转运动是每度上的步数
        stepsToCircle = (s32)((f32)totalSteps / (convertCoeff * DEGREES_OF_A_CIRCLE));
    }
    else if (POSTUNIT_MMETER == posnUnit)
    {
        //根据微步数、步距角和导程计算，直线运动是每毫米上的步数
        stepsToCircle = (s32)((f32)totalSteps / convertCoeff);
    }

    //做个限制
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
函 数 名: pvtPointCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvtPointCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;


    //根据规划方式进行预计算
    switch (pPvtCalcData->pvtPlanMode)
    {
        /***************************************************************
        三次多项式插值
        ***************************************************************/
        case PLANMODE_CUBIC:
            errorCode = pvtCubicCurveCalc(chanNum, pPvtCalcData);
          break;
          
          
        /***************************************************************
        线性插值
        ***************************************************************/
        case PLANMODE_LINEAR:
            errorCode = pvtLinearCurveCalc(chanNum, pPvtCalcData);
          break;

        /***************************************************************
        梯形插值
        ***************************************************************/
        case PLANMODE_TRAPEZOID:
            errorCode = pvtTrapezCurveCalc(chanNum, pPvtCalcData);
          break;
          
          
        /***************************************************************
        五次多项式插值
        ***************************************************************/
        case PLANMODE_QUINTIC:
          break;

          
        /***************************************************************
        正弦
        ***************************************************************/
        case PLANMODE_SINE:
          break;

        /***************************************************************
        匀速
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
函 数 名: pvtLineCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvtLineCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum)
{
    bool bHaveRemain = false;
    u8   errorCode = PVT_CALC_NO_ERROR;
    u32  i;

    f32 calcOutput;
    f64 curTime = 0;    //临时改成64位    NICK MARK
    f32 errorTime;

    u16 calcCount = 0;
    f64 lastTime;       //临时改成64位    NICK MARK
    f64 maxTime;        //临时改成64位    NICK MARK
    f64 minTime;        //临时改成64位    NICK MARK

    f32 cubeMotionTime;
    f32 squareMotionTime;
    f32 curPost;   
    f32 threeTermCoef;
    f32 twoTermCoef;

    f32 posnError;


    //转换量纲，获取起始终止的位置和速度
    u32 startPost = (u32)(pPvtCalcData->startPosn * pPvtCalcData->posnConvertInfo.posnToLine + 0.5f);
    u32 endPost   = (u32)(pPvtCalcData->endPosn   * pPvtCalcData->posnConvertInfo.posnToLine + 0.5f);

    
    f32 startSpeed = fabs(pPvtCalcData->startSpeed * pPvtCalcData->posnConvertInfo.posnToLine);
    f32 endSpeed   = fabs(pPvtCalcData->endSpeed * pPvtCalcData->posnConvertInfo.posnToLine); 
    
#if 1    //目前先只支持运动整数个线数    NICK MARK
    u32 poseOffset = abs(endPost - startPost);
#else
    f32 poseOffset = fabs(pPvtCalcData->endPosn - pPvtCalcData->startPosn) * pPvtCalcData->convertCoeff;

    //如果要运行的线数不是整数的话，记录下来剩余的不满1线的值
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

    //初始化时间
    minTime = 0;
    if ((1 == pPvtCalcData->targetLine) ||   //第一步的时候需要在全时间范围内查找
        (0.0f == startSpeed))                  //或者初始速度为0则认为两段之间速度变化较大则重新在全时间范围内查找
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
    
    //提前算好三次项和二次项的系数
    cubeMotionTime = CUBE(pPvtCalcData->motionTime);
    squareMotionTime = SQUARE(pPvtCalcData->motionTime);
    threeTermCoef = ((startSpeed + endSpeed) / squareMotionTime) - (poseOffset * 2.0f / cubeMotionTime);
    twoTermCoef = (poseOffset * 3.0f / squareMotionTime) - (startSpeed * 2.0f + endSpeed) / pPvtCalcData->motionTime;

    pPvtCalcData->targetLine -= startPost;
    for (i = 0;i < poseOffset;i++)
    {
        calcCount = 0;       
        curTime += pPvtCalcData->lastLineSpeed;    //在上一步的时间轴上加上上一步的速度作为这一步的计算初始时间
        if (curTime > pPvtCalcData->motionTime)
        {
            curTime = pPvtCalcData->motionTime;
        }

        curPost = ((threeTermCoef * curTime  + twoTermCoef) * curTime + startSpeed) * curTime;
        
        posnError = curPost - pPvtCalcData->targetLine;
        while ((posnError > pPvtCalcData->lineErrorP) ||    //误差小于1微步才行    NICK MARK
               (posnError < pPvtCalcData->lineErrorN))
        {
            calcCount++;
            if (calcCount > PVT_ONE_POINT_CALC_TIMES_MAX)
            {
                //计算次数过多则结束计算，报错
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

        //发生计算次数过多错误结束计算，报错
        if (PVT_CALC_TOO_MANY_CALC_TIMES == errorCode)
        {
            break;
        }
        
        pPvtCalcData->targetLine++;    //开始计算下一个目标
        
        pPvtCalcData->lastLineSpeed = curTime - lastTime;    //记录这一步和上一步的间隔时间，也就是速度

        minTime = curTime;
        lastTime = curTime;
        maxTime = curTime + pPvtCalcData->lastLineSpeed * 3.0f;
        if (maxTime > pPvtCalcData->motionTime)
        {
            maxTime = pPvtCalcData->motionTime;
        }

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
        //将时间按照线间步平分
        calcOutput = (pPvtCalcData->lastLineSpeed * FPGA_PWM_CLOCK + 
                      pPvtCalcData->lastLineSpeed * pPvtCalcData->fpgaPwmClock + 
                      pPvtCalcData->errorTime) 
                      * (pPvtCalcData->posnConvertInfo.lineMult + 1)    //加1是因为lineMult本身是减了1的
                      / pPvtCalcData->posnConvertInfo.lineSteps;

        //做个上下限保护
        if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
        {
            calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
            
            //系统错误码置位
            errorCode = PVT_CALC_GREAT_UPER_LIMIT;
        }
        
#else

        //将时间按照线间步平分
        calcOutput = (pPvtCalcData->lastLineSpeed * pPvtCalcData->fpgaPwmClock + pPvtCalcData->errorTime) 
                      * (pPvtCalcData->posnConvertInfo.lineMult + 1)    //加1是因为lineMult本身是减了1的
                      / pPvtCalcData->posnConvertInfo.lineSteps;

        //做个速度保护

        if (calcOutput > pPvtCalcData->fpgaPwmClock)
        {
            calcOutput = pPvtCalcData->fpgaPwmClock;
            
            //系统错误码置位
            errorCode = PVT_CALC_GREAT_UPER_LIMIT;
        } 
#endif
        else if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //系统错误码置位
            errorCode = PVT_CALC_LESS_LOWER_LIMIT;
        }

        //统计下归一化后四舍五入造成的误差
        errorTime = calcOutput - (u32)calcOutput;
        pPvtCalcData->errorTime = errorTime * pPvtCalcData->posnConvertInfo.lineSteps;

        pPvtCalcData->outpBufferFill(chanNum, (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              pPvtCalcData->pContext);
    }

    pPvtCalcData->targetLine += startPost;
    pPvtCalcData->lastLineTime = curTime + pPvtCalcData->startTime;
    
    //计算最后一段
    if (bHaveRemain)
    {
#if PVT_CALC_USE_FPGA_CLOCK_ERROR
        //将时间按照线间步平分
        calcOutput = ((pPvtCalcData->motionTime - curTime) * FPGA_PWM_CLOCK + 
                      (pPvtCalcData->motionTime - curTime) * pPvtCalcData->fpgaPwmClock) 
                     * (pPvtCalcData->posnConvertInfo.lineMult + 1)    //加1是因为lineMult本身是减了1的
                     / (pPvtCalcData->remainLine * pPvtCalcData->posnConvertInfo.lineSteps);

        //做个速度保护
        if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //系统错误码置位
            errorCode = PVT_CALC_LESS_LOWER_LIMIT;
        }
        
        
#else

        calcOutput = (pPvtCalcData->motionTime - curTime) * pPvtCalcData->fpgaPwmClock 
                     * (pPvtCalcData->posnConvertInfo.lineMult + 1)    //加1是因为lineMult本身是减了1的
                     / (pPvtCalcData->remainLine * pPvtCalcData->posnConvertInfo.lineSteps);

        //做个速度保护
        if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //系统错误码置位
            errorCode = PVT_CALC_LESS_LOWER_LIMIT;
        }
#endif
        //统计下归一化后四舍五入造成的误差
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
函 数 名: pvtSegmentCalc
实现功能: 计算一段PVT(两个PVT点之间的所有微步数)，此函数作用主要是对计算进行预处理，之后由
          pvtPointCalc完成两个PVT点之间所有微步数的计算
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
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
    

    //不是最后一个点才计算
    if (pPvtCalcData->endPoint.time != -1.0f)
    {
        //获取当前要计算的这一段PVT的相关数据
        startPosn  = pPvtCalcData->startPoint.position;
        startSpeed = pPvtCalcData->startPoint.speed;
        startTime  = pPvtCalcData->startPoint.time;
        
        endPosn  = pPvtCalcData->endPoint.position;
        endSpeed = pPvtCalcData->endPoint.speed;
        
        motionTime = pPvtCalcData->endPoint.time - startTime;

        //判断方向
        if (endPosn < startPosn)
        {
            direction = DIR_REVERSE;
        }
        else
        {
            direction = DIR_FORWARD;
        }
        
        if (direction != pPvtCalcData->lastStepDir)    //反向运动
        {
            //反向之后目标步数变为1，更新方向
            pPvtCalcData->targetStep = 1;
            pPvtCalcData->targetLine = 1;
            pPvtCalcData->lastStepDir = direction;

            pPvtCalcData->waitStepError = 0;

            //反向之后记录下反向的位置
            pPvtCalcData->invsePosition = pPvtCalcData->startPoint.position;
        }

        //基于反向位置计算相对起始位置
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
        motionStep += pPvtCalcData->waitStepError;    //累加下不满一步的步数   

        //位置变化大于1步才计算，否则算为等待时间
        if (motionStep >= 1.0f)
        {                        
            //因为等待的步数超过1步了就要把这一步走出来
            if (pPvtCalcData->waitStepError > 0.0f)
            {
                pPvtCalcData->waitStepError = 0;
                startPosn = pPvtCalcData->waitPosition;
            }
            
            //计算一段PVT
            pPvtCalcData->startPosn   = startPosn;
            pPvtCalcData->startSpeed  = startSpeed;
            pPvtCalcData->startTime   = startTime;
            pPvtCalcData->endPosn     = endPosn;
            pPvtCalcData->endSpeed    = endSpeed;
            pPvtCalcData->motionTime  = motionTime;
            
            //如果校准了，也就必然有编码器
            if (bCalibrate)
            {
                //LVT模式
                if (MTNMODE_PVT != pPvtCalcData->motionMode)
                {
                    pvtLineCalc(pPvtCalcData, chanNum);
                }
                else
                {
                    bCalcPvt = true;
                }
            }
            else    //没有校准的话不需要按照编码器每线之间的弧度将初始PVT进行分段
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
            //保存第一次开始等待的位置
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
            motionTime += 0.5f;    //四舍五入
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, (u32)motionTime, BUFFOPERT_WRITE, pPvtCalcData->pContext);
        }
    }
    
    //最后一个点则强制发送下最后的数据，如果是FIFO模式算够了点数则只通知FUNC上报计算结束状态
    if ((-1.0f == pPvtCalcData->endPoint.time) ||
        ((EXECMODE_FIFO == pPvtCalcData->pvtExecMode) &&        //FIFO模式，算够了点数且还未上报    CJ 2017.03.28 Add
         (false == pPvtCalcData->bReportCalcEnd) &&
         (pPvtCalcData->lastPoint >= PVT_FIFO_REPORT_CALC_NUM)))
    {
        //通知FUNC查询FPGA的Ready状态
        if ((EXECMODE_FIFO != pPvtCalcData->pvtExecMode) ||    //FIFO模式下已经上报了则不需要上报    CJ 2017.03.28 Modify
            (false == pPvtCalcData->bReportCalcEnd))   
        {
            pPvtCalcData->bQueryReady = true;
            pPvtCalcData->bReportCalcEnd = true;
        }

        //最后一个点或者微调模式才需要强制发送Buffer中的数据和清空过程数据
        if (-1.0f == pPvtCalcData->endPoint.time)    //CJ 2017.03.28 Modify
        {
            //强制发送Buffer中的数据，总的波表数据等于m_stProcessData.targetStep - 1
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_FORWARD, 0, BUFFOPERT_FORCESEND, pPvtCalcData->pContext);
        }
    }

    return errorCode;
}



/*******************************************文件尾********************************************/
