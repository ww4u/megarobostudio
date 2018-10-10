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
#include "pvtTrapezSolution.h"
#include "pvtScurveSolution.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    PVT_FIFO_REPORT_CALC_NUM    5    //FIFO模式下开始上报计算结束状态的已计算点数



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
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
        梯形插值
        ***************************************************************/
        case PLANMODE_TRAPEZOID:
            errorCode = pvtTrapezCurveCalc(chanNum, pPvtCalcData);
          break;          
          
        /***************************************************************
        S曲线插值
        ***************************************************************/
        case PLANMODE_SCURVE:
            errorCode = pvtScurveCalc(chanNum, pPvtCalcData);
          break;

        default:
          break;
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
    u8   errorCode = PVT_CALC_NO_ERROR;
    OutpDataTypeEnum direction;    
    f32 motionStep;
    f32 posnConvertCoeff;
    
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
        startSpeed = pPvtCalcData->startPoint.speed;
        startTime  = pPvtCalcData->startPoint.time;
        
        endSpeed = pPvtCalcData->endPoint.speed;
        

        //判断方向
        if (pPvtCalcData->endPoint.position < pPvtCalcData->startPoint.position)
        {
            direction = OUTPDATA_REVERSE;
        }
        else
        {
            direction = OUTPDATA_FORWARD;
        }
        
        //反向了
        if (direction != pPvtCalcData->lastStepDir)
        {
            pPvtCalcData->lastStepDir = direction;
            
            //反向之后目标步数变为1，更新方向
            pPvtCalcData->targetStep = 1;
            pPvtCalcData->waitStepError = 0;

            //反向之后记录下反向的位置
            pPvtCalcData->invsePosition = pPvtCalcData->startPoint.position;
        }

        //基于反向位置计算相对起始位置
        startPosn = fabs(pPvtCalcData->startPoint.position - pPvtCalcData->invsePosition);
        endPosn   = fabs(pPvtCalcData->endPoint.position - pPvtCalcData->invsePosition);
        
        motionTime = pPvtCalcData->endPoint.time - startTime;

        posnConvertCoeff = pPvtCalcData->posnToStep;

        motionStep = (endPosn - startPosn) * posnConvertCoeff + pPvtCalcData->waitStepError;   //累加下不满一步的步数           

        //位置变化小于1步算为等待时间
        if (motionStep < 1.0f)
        {
            //保存第一次开始等待的位置
            if (0.0f == pPvtCalcData->waitStepError)
            {
                pPvtCalcData->waitPosition = startPosn;
            }
            pPvtCalcData->waitStepError += motionStep;

            //时间归一化
            motionTime = motionTime * pPvtCalcData->fpgaPwmClock + motionTime * pPvtCalcData->fpgaClockOffset;

            //作为等待时间存入波表Buffer
            motionTime += pPvtCalcData->errorTime;
            pPvtCalcData->errorTime = motionTime - (u32)motionTime;
            
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, (u32)motionTime, BUFFOPERT_WRITE, pPvtCalcData->pContext);

            //统计时间
            pPvtCalcData->timeCount += (u32)motionTime;
        }
        else
        {
            //起始位置从等待位置算起
            if (pPvtCalcData->waitStepError > 0.0f)
            {
                pPvtCalcData->waitStepError = 0;
                startPosn = pPvtCalcData->waitPosition;
            }
            
            //计算一段PVT
            pPvtCalcData->startPosn  = (u32)(startPosn * posnConvertCoeff + 0.5f);
            pPvtCalcData->startSpeed = fabs(startSpeed * posnConvertCoeff);
            pPvtCalcData->startTime  = startTime;
            pPvtCalcData->endPosn    = (u32)(endPosn * posnConvertCoeff + 0.5f);
            pPvtCalcData->endSpeed   = fabs(endSpeed * posnConvertCoeff);
            pPvtCalcData->motionTime = motionTime;

            //暂时采用不同的计算方式    NICK MARK
            if (MTNMODE_PVT == pPvtCalcData->motionMode)
            {
                pPvtCalcData->motionStep = (u32)((endPosn - pPvtCalcData->startPosn / posnConvertCoeff) * posnConvertCoeff + 0.5f);
            }
            else
            {
                pPvtCalcData->motionStep = (u32)((endPosn - startPosn) * posnConvertCoeff + 0.5f);
            }

            errorCode = pvtPointCalc(chanNum, pPvtCalcData);
        }

        //FIFO模式算完每一段都强制发送下数据，保存多个轴波表同步    CJ 2018.04.21
        if (EXECMODE_FIFO == pPvtCalcData->pvtExecMode)
        {
            //强制发送
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_FORWARD, 0, BUFFOPERT_SYNCSEND, pPvtCalcData->pContext);
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
