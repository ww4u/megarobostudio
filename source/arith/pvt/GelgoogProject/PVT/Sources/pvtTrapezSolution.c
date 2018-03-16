/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtTrapezSolution.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.11.23;
历史版本:  无;
*********************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arm_math.h"
#include "pvtTrapezSolution.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/
#define    SCALE_MAGNIFICATION    1000



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: pvtTrapezCurveCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 pvtTrapezCurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;

    f32 calcOutput;
    u32 i;
    f64 curTime = 0;
    f64 lastTime;
    
    f32 startSpeed;
    f32 endSpeed;
    f32 uniformSpeed;
    f32 accTime;
    f32 decTime;
    f32 accTimeHalf;
    f32 decTimeHalf;

    //一元二次方程相关变量
    f32 delta;
    f32 sqrtDelta;
    f32 squarecoeff2B;
    f32 coeff4A;
    f32 coeff2AInv;
    //一元二次方程相关变量
    
    u32 posnOffset;
    u32 accPosn;
    u32 decPosn;
    u32 startPosn = (u32)(pPvtCalcData->startPosn * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);//四舍五入下(临时) NICK MARK
    u32 endPosn   = (u32)(pPvtCalcData->endPosn * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);  //四舍五入下(临时) NICK MARK
    

    //转换量纲
    startSpeed = fabs(pPvtCalcData->startSpeed * pPvtCalcData->posnConvertInfo.posnToStep);
    endSpeed   = fabs(pPvtCalcData->endSpeed * pPvtCalcData->posnConvertInfo.posnToStep);
    posnOffset = abs(endPosn - startPosn);

    //统计下微步数，正向加，反向减
    if (DIR_FORWARD == pPvtCalcData->lastStepDir)
    {
        pPvtCalcData->pvtSteps += posnOffset;
    }
    else
    {
        pPvtCalcData->pvtSteps -= posnOffset;
    }
        
    //初始化时间
    if ((1 == pPvtCalcData->targetStep) ||
        (0.0f == startSpeed))
    {
        pPvtCalcData->lastStepTime = pPvtCalcData->startTime;
        pPvtCalcData->lastStepSpeed = 0;
        lastTime = 0;
    }
    else
    {
        lastTime = pPvtCalcData->lastStepTime - pPvtCalcData->startTime;
    }

    if (posnOffset > 1)    //小于等于一步时直接进入最后一步计算
    {
        //梯形曲线即一次的S曲线，分为三段，I、II和III段，简单点称为加速、匀速和减速段
        //实际上I和III段可能的情况是加速减速、加速加速、减速加速、减速减速
        
        //先根据PVT0、PVT1、Sacc(加速(I)段占比)、Sdec(减速(II)段占比)计算出匀速(II)段的速度Vunf
        accTime = pPvtCalcData->motionTime * pPvtCalcData->accScale / SCALE_MAGNIFICATION;
        decTime = pPvtCalcData->motionTime * pPvtCalcData->decScale / SCALE_MAGNIFICATION;
        accTimeHalf = accTime * 0.5f;
        decTimeHalf = decTime * 0.5f;
        uniformSpeed = (posnOffset - (startSpeed * accTimeHalf + endSpeed * decTimeHalf)) / 
                       (pPvtCalcData->motionTime - accTimeHalf - decTimeHalf);

        //计算I段的结束位置和III段的起始位置
        accPosn = startPosn + (u32)(accTimeHalf * (uniformSpeed + startSpeed));
        decPosn = endPosn   - (u32)(decTimeHalf * (uniformSpeed + endSpeed));


        //加速段(I)开始点的PVT分别为:   startPosn、startSpeed、  0
        //加速段(I)结束点的PVT分别为:   accPosn、  uniformSpeed、accTime
        //减速段(III)开始点的PVT分别为: decPosn、  uniformSpeed、motionTime - decTime
        //减速段(III)结束点的PVT分别为: endPosn、  endSpeed、    motionTime

        /*************************************解算加速(I)段************************************************************/
        //加减速段P和T的关系是一个一元二次方程，P是y，T是x，最终是根据P获取T
        //一元二次方程求根公式里面的三个系数分别是: a -- (uniformSpeed - startSpeed)/(2 * accTime)
        //一元二次方程求根公式里面的三个系数分别是: b -- startSpeed
        //一元二次方程求根公式里面的三个系数分别是: c -- pPvtCalcData->targetStep

        //pPvtCalcData->targetStep -= startPosn;

        squarecoeff2B = startSpeed * startSpeed;
        coeff2AInv = accTime / (uniformSpeed - startSpeed);
        coeff4A = 2.0f / coeff2AInv;
        
        for (i = 1;i <= (accPosn - startPosn);i++)
        {
            //△= b^2 - 4ac
            delta = squarecoeff2B + coeff4A * i;
            arm_sqrt_f32(delta, &sqrtDelta);
            curTime = (sqrtDelta - startSpeed) * coeff2AInv;

            pPvtCalcData->targetStep++;

            pPvtCalcData->lastStepSpeed = curTime - lastTime;
            //lastTime = curTime;

            //时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
            calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                         pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

            //做个上下限保护
            if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
            {
                calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                
                //系统错误码置位
                errorCode = PVT_CALC_GREAT_UPER_LIMIT;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //系统错误码置位
                errorCode = PVT_CALC_LESS_LOWER_LIMIT;
            }
            lastTime = curTime;
            
            calcOutput += pPvtCalcData->errorTime;
            pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

            pPvtCalcData->outpBufferFill(chanNum, 
                                         (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                         (u32)calcOutput, 
                                         BUFFOPERT_WRITE, 
                                         pPvtCalcData->pContext);
        }
        /*************************************解算加速(I)段************************************************************/
        
        
        /*************************************解算匀速(II)段***********************************************************/
        pPvtCalcData->lastStepSpeed = 1.0f / uniformSpeed;
        for (i = 0;i < (decPosn - accPosn);i++)
        {
            pPvtCalcData->targetStep++;    //开始计算下一个目标

            curTime += pPvtCalcData->lastStepSpeed;

            //时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
            calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                         pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

            //做个上下限保护
            if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
            {
                calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                
                //系统错误码置位
                errorCode = PVT_CALC_GREAT_UPER_LIMIT;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //系统错误码置位
                errorCode = PVT_CALC_LESS_LOWER_LIMIT;
            }
            
            calcOutput += pPvtCalcData->errorTime;
            pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

            pPvtCalcData->outpBufferFill(chanNum, 
                                         (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                         (u32)calcOutput, 
                                         BUFFOPERT_WRITE, 
                                         pPvtCalcData->pContext);
        }
        lastTime = curTime;
        /*************************************解算匀速(II)段***********************************************************/

        
        /*************************************解算减速(III)段**********************************************************/
        //加减速段P和T的关系是一个一元二次方程，P是y，T是x，最终是根据P获取T
        //一元二次方程求根公式里面的三个系数分别是: a -- (endSpeed - uniformSpeed)/(2 * decTime)
        //一元二次方程求根公式里面的三个系数分别是: b -- uniformSpeed
        //一元二次方程求根公式里面的三个系数分别是: c -- pPvtCalcData->targetStep

        //pPvtCalcData->targetStep -= (decPosn - startPosn);
        lastTime = 0.0;

        squarecoeff2B = uniformSpeed * uniformSpeed;
        coeff2AInv = decTime / (endSpeed - uniformSpeed);
        coeff4A = 2.0f / coeff2AInv;
        
        for (i = 1;i < (endPosn - decPosn);i++)    //少算一步，最后一步不在这里算
        {
            //△= b^2 - 4ac
            delta = squarecoeff2B + coeff4A * i;
            arm_sqrt_f32(delta, &sqrtDelta);
            curTime = (sqrtDelta - uniformSpeed) * coeff2AInv;

            pPvtCalcData->targetStep++;

            pPvtCalcData->lastStepSpeed = curTime - lastTime;
            lastTime = curTime;

            //时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
            calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                         pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

            //做个上下限保护
            if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
            {
                calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                
                //系统错误码置位
                errorCode = PVT_CALC_GREAT_UPER_LIMIT;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //系统错误码置位
                errorCode = PVT_CALC_LESS_LOWER_LIMIT;
            }
            
            calcOutput += pPvtCalcData->errorTime;
            pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

            pPvtCalcData->outpBufferFill(chanNum, 
                                         (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                         (u32)calcOutput, 
                                         BUFFOPERT_WRITE, 
                                         pPvtCalcData->pContext);
        }
        /*************************************解算减速(III)段**********************************************************/
    }

    /****************************************最后一步**************************************************************/
    //最后一步的时间直接用motionTime减去上一步的时间位置
    //pPvtCalcData->lastStepSpeed = pPvtCalcData->motionTime - curTime);
    pPvtCalcData->lastStepSpeed = decTime - curTime;
    
    //时间的补偿问题:理论上讲用时间作为速度是没有误差的，而误差来源于归一化后的四舍五入
    calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                 pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

    //做个上限保护
    if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
    {
        calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
        
        //系统错误码置位
        errorCode = PVT_CALC_GREAT_UPER_LIMIT;
    }
    else if (calcOutput < OUTPUT_PERIOD_MIN)
    {
        calcOutput = OUTPUT_PERIOD_MIN;
        
        //系统错误码置位
        errorCode = PVT_CALC_LESS_LOWER_LIMIT;
    }

    calcOutput += pPvtCalcData->errorTime;
    pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

    pPvtCalcData->outpBufferFill(chanNum, 
                                 (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                 (u32)calcOutput, 
                                 BUFFOPERT_WRITE, 
                                 pPvtCalcData->pContext);
    /****************************************最后一步**************************************************************/

    pPvtCalcData->targetStep++;
    //pPvtCalcData->targetStep += startPosn + decPosn;
    
    pPvtCalcData->lastStepTime = pPvtCalcData->motionTime + pPvtCalcData->startTime;

    return errorCode;
}



/*******************************************文件尾********************************************/
