/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtTrapezSolution.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.11.23;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arm_math.h"
#include "pvtTrapezSolution.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#define    SCALE_MAGNIFICATION    1000



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvtTrapezCurveCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    //һԪ���η�����ر���
    f32 delta;
    f32 sqrtDelta;
    f32 squarecoeff2B;
    f32 coeff4A;
    f32 coeff2AInv;
    //һԪ���η�����ر���
    
    u32 posnOffset;
    u32 accPosn;
    u32 decPosn;
    u32 startPosn = (u32)(pPvtCalcData->startPosn * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);//����������(��ʱ) NICK MARK
    u32 endPosn   = (u32)(pPvtCalcData->endPosn * pPvtCalcData->posnConvertInfo.posnToStep + 0.5f);  //����������(��ʱ) NICK MARK
    

    //ת������
    startSpeed = fabs(pPvtCalcData->startSpeed * pPvtCalcData->posnConvertInfo.posnToStep);
    endSpeed   = fabs(pPvtCalcData->endSpeed * pPvtCalcData->posnConvertInfo.posnToStep);
    posnOffset = abs(endPosn - startPosn);

    //ͳ����΢����������ӣ������
    if (DIR_FORWARD == pPvtCalcData->lastStepDir)
    {
        pPvtCalcData->pvtSteps += posnOffset;
    }
    else
    {
        pPvtCalcData->pvtSteps -= posnOffset;
    }
        
    //��ʼ��ʱ��
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

    if (posnOffset > 1)    //С�ڵ���һ��ʱֱ�ӽ������һ������
    {
        //�������߼�һ�ε�S���ߣ���Ϊ���Σ�I��II��III�Σ��򵥵��Ϊ���١����ٺͼ��ٶ�
        //ʵ����I��III�ο��ܵ�����Ǽ��ټ��١����ټ��١����ټ��١����ټ���
        
        //�ȸ���PVT0��PVT1��Sacc(����(I)��ռ��)��Sdec(����(II)��ռ��)���������(II)�ε��ٶ�Vunf
        accTime = pPvtCalcData->motionTime * pPvtCalcData->accScale / SCALE_MAGNIFICATION;
        decTime = pPvtCalcData->motionTime * pPvtCalcData->decScale / SCALE_MAGNIFICATION;
        accTimeHalf = accTime * 0.5f;
        decTimeHalf = decTime * 0.5f;
        uniformSpeed = (posnOffset - (startSpeed * accTimeHalf + endSpeed * decTimeHalf)) / 
                       (pPvtCalcData->motionTime - accTimeHalf - decTimeHalf);

        //����I�εĽ���λ�ú�III�ε���ʼλ��
        accPosn = startPosn + (u32)(accTimeHalf * (uniformSpeed + startSpeed));
        decPosn = endPosn   - (u32)(decTimeHalf * (uniformSpeed + endSpeed));


        //���ٶ�(I)��ʼ���PVT�ֱ�Ϊ:   startPosn��startSpeed��  0
        //���ٶ�(I)�������PVT�ֱ�Ϊ:   accPosn��  uniformSpeed��accTime
        //���ٶ�(III)��ʼ���PVT�ֱ�Ϊ: decPosn��  uniformSpeed��motionTime - decTime
        //���ٶ�(III)�������PVT�ֱ�Ϊ: endPosn��  endSpeed��    motionTime

        /*************************************�������(I)��************************************************************/
        //�Ӽ��ٶ�P��T�Ĺ�ϵ��һ��һԪ���η��̣�P��y��T��x�������Ǹ���P��ȡT
        //һԪ���η��������ʽ���������ϵ���ֱ���: a -- (uniformSpeed - startSpeed)/(2 * accTime)
        //һԪ���η��������ʽ���������ϵ���ֱ���: b -- startSpeed
        //һԪ���η��������ʽ���������ϵ���ֱ���: c -- pPvtCalcData->targetStep

        //pPvtCalcData->targetStep -= startPosn;

        squarecoeff2B = startSpeed * startSpeed;
        coeff2AInv = accTime / (uniformSpeed - startSpeed);
        coeff4A = 2.0f / coeff2AInv;
        
        for (i = 1;i <= (accPosn - startPosn);i++)
        {
            //��= b^2 - 4ac
            delta = squarecoeff2B + coeff4A * i;
            arm_sqrt_f32(delta, &sqrtDelta);
            curTime = (sqrtDelta - startSpeed) * coeff2AInv;

            pPvtCalcData->targetStep++;

            pPvtCalcData->lastStepSpeed = curTime - lastTime;
            //lastTime = curTime;

            //ʱ��Ĳ�������:�����Ͻ���ʱ����Ϊ�ٶ���û�����ģ��������Դ�ڹ�һ�������������
            calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                         pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

            //���������ޱ���
            if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
            {
                calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                
                //ϵͳ��������λ
                errorCode = PVT_CALC_GREAT_UPER_LIMIT;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //ϵͳ��������λ
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
        /*************************************�������(I)��************************************************************/
        
        
        /*************************************��������(II)��***********************************************************/
        pPvtCalcData->lastStepSpeed = 1.0f / uniformSpeed;
        for (i = 0;i < (decPosn - accPosn);i++)
        {
            pPvtCalcData->targetStep++;    //��ʼ������һ��Ŀ��

            curTime += pPvtCalcData->lastStepSpeed;

            //ʱ��Ĳ�������:�����Ͻ���ʱ����Ϊ�ٶ���û�����ģ��������Դ�ڹ�һ�������������
            calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                         pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

            //���������ޱ���
            if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
            {
                calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                
                //ϵͳ��������λ
                errorCode = PVT_CALC_GREAT_UPER_LIMIT;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //ϵͳ��������λ
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
        /*************************************��������(II)��***********************************************************/

        
        /*************************************�������(III)��**********************************************************/
        //�Ӽ��ٶ�P��T�Ĺ�ϵ��һ��һԪ���η��̣�P��y��T��x�������Ǹ���P��ȡT
        //һԪ���η��������ʽ���������ϵ���ֱ���: a -- (endSpeed - uniformSpeed)/(2 * decTime)
        //һԪ���η��������ʽ���������ϵ���ֱ���: b -- uniformSpeed
        //һԪ���η��������ʽ���������ϵ���ֱ���: c -- pPvtCalcData->targetStep

        //pPvtCalcData->targetStep -= (decPosn - startPosn);
        lastTime = 0.0;

        squarecoeff2B = uniformSpeed * uniformSpeed;
        coeff2AInv = decTime / (endSpeed - uniformSpeed);
        coeff4A = 2.0f / coeff2AInv;
        
        for (i = 1;i < (endPosn - decPosn);i++)    //����һ�������һ������������
        {
            //��= b^2 - 4ac
            delta = squarecoeff2B + coeff4A * i;
            arm_sqrt_f32(delta, &sqrtDelta);
            curTime = (sqrtDelta - uniformSpeed) * coeff2AInv;

            pPvtCalcData->targetStep++;

            pPvtCalcData->lastStepSpeed = curTime - lastTime;
            lastTime = curTime;

            //ʱ��Ĳ�������:�����Ͻ���ʱ����Ϊ�ٶ���û�����ģ��������Դ�ڹ�һ�������������
            calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                         pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

            //���������ޱ���
            if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
            {
                calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
                
                //ϵͳ��������λ
                errorCode = PVT_CALC_GREAT_UPER_LIMIT;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //ϵͳ��������λ
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
        /*************************************�������(III)��**********************************************************/
    }

    /****************************************���һ��**************************************************************/
    //���һ����ʱ��ֱ����motionTime��ȥ��һ����ʱ��λ��
    //pPvtCalcData->lastStepSpeed = pPvtCalcData->motionTime - curTime);
    pPvtCalcData->lastStepSpeed = decTime - curTime;
    
    //ʱ��Ĳ�������:�����Ͻ���ʱ����Ϊ�ٶ���û�����ģ��������Դ�ڹ�һ�������������
    calcOutput = pPvtCalcData->lastStepSpeed * FPGA_PWM_CLOCK + 
                 pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock;

    //�������ޱ���
    if (calcOutput > (FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock))
    {
        calcOutput = FPGA_PWM_CLOCK + pPvtCalcData->fpgaPwmClock;
        
        //ϵͳ��������λ
        errorCode = PVT_CALC_GREAT_UPER_LIMIT;
    }
    else if (calcOutput < OUTPUT_PERIOD_MIN)
    {
        calcOutput = OUTPUT_PERIOD_MIN;
        
        //ϵͳ��������λ
        errorCode = PVT_CALC_LESS_LOWER_LIMIT;
    }

    calcOutput += pPvtCalcData->errorTime;
    pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;

    pPvtCalcData->outpBufferFill(chanNum, 
                                 (OutpDataTypeEnum)pPvtCalcData->lastStepDir, 
                                 (u32)calcOutput, 
                                 BUFFOPERT_WRITE, 
                                 pPvtCalcData->pContext);
    /****************************************���һ��**************************************************************/

    pPvtCalcData->targetStep++;
    //pPvtCalcData->targetStep += startPosn + decPosn;
    
    pPvtCalcData->lastStepTime = pPvtCalcData->motionTime + pPvtCalcData->startTime;

    return errorCode;
}



/*******************************************�ļ�β********************************************/
