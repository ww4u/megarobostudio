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
    
    f32 curTime = 0;
    f32 lastTime;
    
    f32 uniformSpeed;
    f32 accTime;
    f32 decTime;
    f32 accTimeHalf;
    f32 decTimeHalf;
    f32 uniformTime;
     
    u32 accPosn;
    u32 decPosn;
    u32 uniformStep;
    u32 decStep;
     
    f32 realAccPosn;
    f32 realDecPosn;
    f32 realStep;

    //һԪ���η�����ر���
    f32 delta;
    f32 sqrtDelta;
    f32 squarecoeff2B;
    f32 coeff4A;
    f32 coeff2AInv;
    //һԪ���η�����ر���

    f32 squareSpeed;
    

    //�е���һ�������������һ��ʱֱ�ӽ������һ������
    if (pPvtCalcData->motionStep > 1)    
    {         
        //�������߼�һ�ε�S���ߣ���Ϊ���Σ�I��II��III�Σ��򵥵��Ϊ���١����ٺͼ��ٶ�
        //ʵ����I��III�ο��ܵ�����Ǽ��ټ��١����ټ��١����ټ��١����ټ���
        
        //�ȸ���PVT0��PVT1��Sacc(����(I)��ռ��)��Sdec(����(II)��ռ��)���������(II)�ε��ٶ�Vunf
        accTime = pPvtCalcData->motionTime * pPvtCalcData->endPoint.accScale / SCALE_MAGNIFICATION;
        decTime = pPvtCalcData->motionTime * pPvtCalcData->endPoint.decScale / SCALE_MAGNIFICATION;
        accTimeHalf = accTime * 0.5f;
        decTimeHalf = decTime * 0.5f;

        //vu = ((p1 - p0) - (v0 * sacc / 2 + v1 * sdec / 2) * T) / T * (1 - sacc / 2 - sdec / 2)
        //vu = ((p1 - p0) - (v0 * Tacc / 2 + v1 * Tdec / 2)) / (T - Tacc / 2 - Tdec / 2)
        uniformSpeed = (pPvtCalcData->motionStep 
                        - (pPvtCalcData->startSpeed * accTimeHalf + pPvtCalcData->endSpeed * decTimeHalf)) / 
                       (pPvtCalcData->motionTime - accTimeHalf - decTimeHalf);

        //����ƽ���ٶ��ж��Ƿ���Ҫ���¼���Ӽ���ʱ��
        if (0.0f == uniformSpeed)
        {
            uniformSpeed = 0;

            squareSpeed = 2.0f / (pPvtCalcData->startSpeed * pPvtCalcData->startSpeed + 
                                  pPvtCalcData->endSpeed   * pPvtCalcData->endSpeed);

            accTime = pPvtCalcData->motionStep * pPvtCalcData->startSpeed * squareSpeed;
            decTime = pPvtCalcData->motionStep * pPvtCalcData->endSpeed * squareSpeed;
            
            accTimeHalf = accTime * 0.5f;
            decTimeHalf = decTime * 0.5f;
        }
        
        //����I�εĽ���λ�ú�III�ε���ʼλ��
        realAccPosn = accTimeHalf * (uniformSpeed + pPvtCalcData->startSpeed);
        realDecPosn = decTimeHalf * (uniformSpeed + pPvtCalcData->endSpeed);
        accPosn = (u32)realAccPosn;
        decPosn = pPvtCalcData->motionStep - (u32)realDecPosn;


        //���ٶ�(I)��ʼ���PVT�ֱ�Ϊ:   startPosn��startSpeed��  0
        //���ٶ�(I)�������PVT�ֱ�Ϊ:   accPosn��  uniformSpeed��accTime
        //���ٶ�(III)��ʼ���PVT�ֱ�Ϊ: decPosn��  uniformSpeed��motionTime - decTime
        //���ٶ�(III)�������PVT�ֱ�Ϊ: endPosn��  endSpeed��    motionTime

        /*************************************�������(I)��************************************************************/
        //�Ӽ��ٶ�P��T�Ĺ�ϵ��һ��һԪ���η��̣�P��y��T��x�������Ǹ���P��ȡT
        //һԪ���η��������ʽ���������ϵ���ֱ���: a -- (uniformSpeed - startSpeed)/(2 * accTime)
        //һԪ���η��������ʽ���������ϵ���ֱ���: b -- startSpeed
        //һԪ���η��������ʽ���������ϵ���ֱ���: c -- pPvtCalcData->targetStep
        if (accPosn > 0)
        {
            squarecoeff2B = pPvtCalcData->startSpeed * pPvtCalcData->startSpeed;
            coeff2AInv = accTime / (uniformSpeed - pPvtCalcData->startSpeed);
            coeff4A = 2.0f / coeff2AInv;
            
            //��ʼ��ʱ��
            lastTime = 0;
            realStep = realAccPosn / accPosn;
            
            //����������һ����I�����һ��ֱ���ò�ֵ�ķ�ʽ����
            for (i = 1;i < accPosn;i++)
            {
                //��Ϊb = v0, a = (vu - v0) / (2 * Tacc), c = -p(t) = -i
                //��= b^2 - 4ac
                //���Ա����delta = squarecoeff2B + coeff4A * i
                delta = squarecoeff2B + coeff4A * i * realStep;
                arm_sqrt_f32(delta, &sqrtDelta);
                curTime = (sqrtDelta - pPvtCalcData->startSpeed) * coeff2AInv;

                //����ͳ����
                pPvtCalcData->targetStep++;

                pPvtCalcData->lastStepSpeed = curTime - lastTime;    
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                lastTime = curTime;
                
                //��ʱ�䰴���߼䲽ƽ��
                calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                              pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                              pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

                //���������ޱ���
                if (calcOutput > pPvtCalcData->fpgaPwmClock)
                {
                    calcOutput -= pPvtCalcData->fpgaPwmClock;
                    
                    pPvtCalcData->errorTime = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                    
                    //������Ĳ�����Ϊ�ȴ�ʱ��
                    pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                          (u32)calcOutput, 
                                                          BUFFOPERT_WRITE, 
                                                          pPvtCalcData->pContext);
                                                 
                    //ͳ����ʱ��
                    pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
                    
                    calcOutput = pPvtCalcData->fpgaPwmClock;
                    
                    //ϵͳ��������λ
                    errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_1;
                }
                else if (calcOutput < OUTPUT_PERIOD_MIN)
                {
                    calcOutput = OUTPUT_PERIOD_MIN;
                    
                    //ϵͳ��������λ
                    errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_1;
                }
                
                //ͳ��������ʱ��
                pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

                pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                                      (u32)calcOutput, 
                                                      BUFFOPERT_WRITE, 
                                                      pPvtCalcData->pContext);
            }

            //I�����һ��
            pPvtCalcData->targetStep++;

            pPvtCalcData->lastStepSpeed = accTime - curTime;    
            if (pPvtCalcData->lastStepSpeed <= 0.0f)
            {
                errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
                pPvtCalcData->lastStepSpeed = 0;
            }
            
            //��ʱ�䰴���߼䲽ƽ��
            calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                          pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                          pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

            //���������ޱ���
            if (calcOutput > pPvtCalcData->fpgaPwmClock)
            {
                calcOutput -= pPvtCalcData->fpgaPwmClock;
                
                pPvtCalcData->errorTime = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                
                //������Ĳ�����Ϊ�ȴ�ʱ��
                pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                      (u32)calcOutput, 
                                                      BUFFOPERT_WRITE, 
                                                      pPvtCalcData->pContext);
                                             
                //ͳ����ʱ��
                pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
                
                calcOutput = pPvtCalcData->fpgaPwmClock;
                
                //ϵͳ��������λ
                errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_1;
            }
            else if (calcOutput < OUTPUT_PERIOD_MIN)
            {
                calcOutput = OUTPUT_PERIOD_MIN;
                
                //ϵͳ��������λ
                errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_1;
            }
            
            //ͳ��������ʱ��
            pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
            pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

            pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                                  (u32)calcOutput, 
                                                  BUFFOPERT_WRITE, 
                                                  pPvtCalcData->pContext);
        }
        /*************************************�������(I)��************************************************************/
        
        
        /*************************************��������(II)��***********************************************************/
        uniformTime = pPvtCalcData->motionTime - accTime - decTime;
        uniformStep = decPosn - accPosn;

        //���ٶ���ʱ��Ŵ���
        if (uniformTime > 0.0f)
        {
            //��ʱ��û������ǵȴ�ʱ��
            if (0 == uniformStep)
            {
                calcOutput = pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                             pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                             pPvtCalcData->errorTime;

                pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;
                
                pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                      (u32)calcOutput, 
                                                      BUFFOPERT_WRITE, 
                                                      pPvtCalcData->pContext);
                                             
                //ͳ����ʱ��
                pPvtCalcData->timeCount += (u32)calcOutput;
            }
            else
            {
                pPvtCalcData->lastStepSpeed = uniformTime / uniformStep;    
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_2;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                for (i = 0;i < (decPosn - accPosn);i++)
                {
                    //����ͳ����
                    pPvtCalcData->targetStep++;
                    
                    //��ʱ�䰴���߼䲽ƽ��
                    calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                                  pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                                  pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

                    //���������ޱ���
                    if (calcOutput > pPvtCalcData->fpgaPwmClock)
                    {
                        /*calcOutput -= pPvtCalcData->fpgaPwmClock;
                        
                        calcOutput += pPvtCalcData->errorTime;
                        pPvtCalcData->errorTime = calcOutput - (u32)calcOutput;
                        
                        //������Ĳ�����Ϊ�ȴ�ʱ��
                        pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                              (u32)calcOutput, 
                                                              BUFFOPERT_WRITE, 
                                                              pPvtCalcData->pContext);
                                                     
                        //ͳ����ʱ��
                        pPvtCalcData->timeCount += (u32)calcOutput;
                        
                        calcOutput = pPvtCalcData->fpgaPwmClock;*/
                        
                        //ϵͳ��������λ
                        errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_2;

                        //����ģʽ�����ٶβ�Ӧ���ֵȴ�ʱ�䣬�����˳�����
                        return errorCode;
                    }
                    else if (calcOutput < OUTPUT_PERIOD_MIN)
                    {
                        calcOutput = OUTPUT_PERIOD_MIN;
                        
                        //ϵͳ��������λ
                        errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_2;
                    }
                    
                    //ͳ��������ʱ��
                    pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                    pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

                    pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                                          (u32)calcOutput, 
                                                          BUFFOPERT_WRITE, 
                                                          pPvtCalcData->pContext);
                }
            }
        }
        /*************************************��������(II)��***********************************************************/

        
        /*************************************�������(III)��**********************************************************/
        //�Ӽ��ٶ�P��T�Ĺ�ϵ��һ��һԪ���η��̣�P��y��T��x�������Ǹ���P��ȡT
        //һԪ���η��������ʽ���������ϵ���ֱ���: a -- (endSpeed - uniformSpeed)/(2 * decTime)
        //һԪ���η��������ʽ���������ϵ���ֱ���: b -- uniformSpeed
        //һԪ���η��������ʽ���������ϵ���ֱ���: c -- pPvtCalcData->targetStep
        decStep = pPvtCalcData->motionStep - decPosn;
        if (decStep > 1)
        {
            squarecoeff2B = uniformSpeed * uniformSpeed;
            coeff2AInv = decTime / (pPvtCalcData->endSpeed - uniformSpeed);
            coeff4A = 2.0f / coeff2AInv;
            
            lastTime = 0.0;
            realStep = realDecPosn / decStep;
            
            //����һ�������һ������������
            for (i = 1;i < decStep;i++)    
            {
                //��Ϊb = v0, a = (vu - v0) / (2 * Tacc), c = -p(t) = -i
                //��= b^2 - 4ac
                //���Ա����delta = squarecoeff2B + coeff4A * i
                delta = squarecoeff2B + coeff4A * i * realStep;
                arm_sqrt_f32(delta, &sqrtDelta);
                curTime = (sqrtDelta - uniformSpeed) * coeff2AInv;

                //����ͳ����
                pPvtCalcData->targetStep++;

                pPvtCalcData->lastStepSpeed = curTime - lastTime;    
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_3;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                lastTime = curTime;
                
                //��ʱ�䰴���߼䲽ƽ��
                calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                              pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                              pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

                //���������ޱ���
                if (calcOutput > pPvtCalcData->fpgaPwmClock)
                {
                    calcOutput -= pPvtCalcData->fpgaPwmClock;
                    
                    pPvtCalcData->errorTime = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                    
                    //������Ĳ�����Ϊ�ȴ�ʱ��
                    pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                          (u32)calcOutput, 
                                                          BUFFOPERT_WRITE, 
                                                          pPvtCalcData->pContext);
                                                 
                    //ͳ����ʱ��
                    pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
                    
                    calcOutput = pPvtCalcData->fpgaPwmClock;
                    
                    //ϵͳ��������λ
                    errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_3;
                }
                else if (calcOutput < OUTPUT_PERIOD_MIN)
                {
                    calcOutput = OUTPUT_PERIOD_MIN;
                    
                    //ϵͳ��������λ
                    errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_3;
                }
                
                //ͳ��������ʱ��
                pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
                pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

                pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                                      (u32)calcOutput, 
                                                      BUFFOPERT_WRITE, 
                                                      pPvtCalcData->pContext);
            }
        }
        else
        {
            curTime = 0;
        }
        /*************************************�������(III)��**********************************************************/
    }
    else
    {
        decTime = pPvtCalcData->motionTime;
    }

    /****************************************���һ��**************************************************************/
    //III�����һ��
    //pPvtCalcData->lastStepSpeed = pPvtCalcData->motionTime - curTime);
    pPvtCalcData->lastStepSpeed = decTime - curTime;    
    if (pPvtCalcData->lastStepSpeed <= 0.0f)
    {
        errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_3;
        pPvtCalcData->lastStepSpeed = 0;
    }
    else
    {
        //��ʱ�䰴���߼䲽ƽ��
        calcOutput = (pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaPwmClock + 
                      pPvtCalcData->lastStepSpeed * pPvtCalcData->fpgaClockOffset + 
                      pPvtCalcData->errorTime) * pPvtCalcData->lineStepsInv;

        //�������ޱ���
        if (calcOutput > pPvtCalcData->fpgaPwmClock)
        {
            calcOutput -= pPvtCalcData->fpgaPwmClock;
            
            pPvtCalcData->errorTime = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
            
            //������Ĳ�����Ϊ�ȴ�ʱ��
            pPvtCalcData->outpBufferFill(chanNum, OUTPDATA_WAIT, 
                                                  (u32)calcOutput, 
                                                  BUFFOPERT_WRITE, 
                                                  pPvtCalcData->pContext);
                                         
            //ͳ����ʱ��
            pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;
            
            calcOutput = pPvtCalcData->fpgaPwmClock;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_GREAT_UPER_LIMIT | PVT_CALC_SECTION_3;
        }
        else if (calcOutput < OUTPUT_PERIOD_MIN)
        {
            calcOutput = OUTPUT_PERIOD_MIN;
            
            //ϵͳ��������λ
            errorCode = PVT_CALC_LESS_LOWER_LIMIT | PVT_CALC_SECTION_3;
        }
        
        //ͳ��������ʱ��
        pPvtCalcData->errorTime  = (calcOutput - (u32)calcOutput) * pPvtCalcData->lineSteps;
        pPvtCalcData->timeCount += (u32)calcOutput * pPvtCalcData->lineSteps;

        pPvtCalcData->outpBufferFill(chanNum, pPvtCalcData->lastStepDir, 
                                              (u32)calcOutput, 
                                              BUFFOPERT_WRITE, 
                                              pPvtCalcData->pContext);
        /****************************************���һ��**************************************************************/

        //����ͳ����
        pPvtCalcData->targetStep++;
    }
    
    return errorCode;
}



/*******************************************�ļ�β********************************************/
