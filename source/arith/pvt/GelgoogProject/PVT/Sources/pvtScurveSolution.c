/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtScurveSolution.c��
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
//#include <math.h>
#include "arm_math.h"
#include "pvtScurveSolution.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvtScurveCalc
ʵ�ֹ���: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*********************************************************************************************/
u8 pvtScurveCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData)
{
    u8  errorCode = PVT_CALC_NO_ERROR;
    u16 calcCount = 0;

    f32 calcOutput;
    u32 i;

    //ʱ��ʹ��64λ����Ϊ�õ��ǲ��ҷ�(��ֵ��)�������Ȳ���ʱ���ܵ��²��Ҳ����ڷ�Χ�ڵ�Ŀ��ֵ
    //���ʹ�ý�����ķ�ʽ����Ҫ
    f64 curTime = 0;
    f64 lastTime;
    f64 maxTime;
    f64 minTime;
    
    f32 cubeMotionTime;
    f32 squareMotionTime;
    f32 curPost;   
    f32 fourTermCoef;
    f32 threeTermCoef;
    f32 posnOffset;
    
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
    f32 targetStep;

    f32 squareSpeed;
    

    //��һ�������������һ��ʱֱ�ӽ������һ������
    if (pPvtCalcData->motionStep > 1)    
    {         
        //S���ߣ���Ϊ���Σ�I��II��III�Σ��򵥵��Ϊ���١����ٺͼ��ٶ�
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
        if (accPosn > 0)
        {
            //��ǰ���������Ͷ������ϵ��
            cubeMotionTime   = CUBE(accTime);
            squareMotionTime = SQUARE(accTime);
            
            //(v0 - vu) / (2 * Tacc^3)
            fourTermCoef = (pPvtCalcData->startSpeed - uniformSpeed) / (2.0f * cubeMotionTime);

            //(vu - v0) / Tacc^2
            threeTermCoef = (uniformSpeed - pPvtCalcData->startSpeed) / squareMotionTime;

            //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t + p0
            //P(t) = pPvtCalcData->targetStep
            //P(t) - p0 = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
            //P(t) - p0 = pPvtCalcData->targetStep - p0  
            
            //��ʼ��ʱ��
            minTime  = 0;
            lastTime = 0;
            if ((1 == pPvtCalcData->targetStep) ||     //��һ����ʱ����Ҫ��motionTime��Χ�ڲ���
                (0.0f == pPvtCalcData->startSpeed))    //���߳�ʼ�ٶ�Ϊ0����Ϊ����֮���ٶȱ仯�ϴ���������motionTime��Χ�ڲ���
            {
                curTime = accTime / accPosn;
                maxTime = accTime;

                pPvtCalcData->lastStepSpeed = 0;
            }
            else
            {
                curTime = lastTime;
                maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
            }
            realStep = realAccPosn / accPosn;
            
            for (i = 1;i < accPosn;i++)
            {
                calcCount  = 0;
                targetStep = realStep * i;
                
                //����һ����ʱ�����ϼ�����һ�����ٶ���Ϊ��һ���ļ����ʼʱ��
                curTime += pPvtCalcData->lastStepSpeed;    
                if (curTime > accTime)
                {
                    curTime = accTime;
                }

                //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
                curPost = ((fourTermCoef * curTime  + threeTermCoef) * curTime * curTime + pPvtCalcData->startSpeed) * curTime;
                
                //��Ŀ��ֵ�����̫������Ҫ��������
                posnOffset = curPost - targetStep;
                while ((posnOffset > pPvtCalcData->maxOffset) ||    //��Ŀ��ֵ�����̫���������
                       (posnOffset < pPvtCalcData->minOffset))
                {
                    calcCount++;
                    
                    if (calcCount >= PVT_ONE_POINT_CALC_TIMES_MAX)
                    {
                        //�������������������㣬����
                        errorCode = PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_1;
                        break;
                    }
                    else if (PVT_ONE_POINT_CALC_TIMES_MAX / 2 == calcCount)  //����һ�����ƺ��ȼӴ�maxTime
                    {
                        maxTime = accTime;
                    }

                    //���ֲ��ң��ı�ʱ���������
                    if (posnOffset > pPvtCalcData->maxOffset)
                    {
                        maxTime = curTime;
                        curTime = (curTime + minTime) * 0.5f;
                    }
                    else
                    {
                        minTime = curTime;
                        curTime = (curTime + maxTime) * 0.5f;
                    }

                    //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
                    curPost = ((fourTermCoef * curTime  + threeTermCoef) * curTime * curTime + pPvtCalcData->startSpeed) * curTime;
                    
                    posnOffset = curPost - targetStep;
                }

                //������������������������㣬����
                if ((PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_1) == errorCode)
                {
                    break;
                }
                
                //��¼��һ������һ���ļ��ʱ�䣬Ҳ�����ٶ�
                pPvtCalcData->lastStepSpeed = curTime - lastTime;    
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_1;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                
                //����ʱ�䣬׼����һ���ļ���
                pPvtCalcData->targetStep++;
                minTime = curTime;
                lastTime = curTime;
                maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
                if (maxTime > accTime)
                {
                    maxTime = accTime;
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

            //I�����һ��
            pPvtCalcData->targetStep++;
            
            pPvtCalcData->lastStepSpeed = accTime - lastTime;    
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
                
                //������Ĳ�����Ϊ�ȴ�ʱ��
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
        decStep = pPvtCalcData->motionStep - decPosn;
        if (decStep > 1)
        {
            //��ǰ���������Ͷ������ϵ��
            cubeMotionTime   = CUBE(decTime);
            squareMotionTime = SQUARE(decTime);
            
            //(v0 - vu) / (2 * Tacc^3)
            fourTermCoef = (uniformSpeed - pPvtCalcData->endSpeed) / (2.0f * cubeMotionTime);

            //(vu - v0) / Tacc^2
            threeTermCoef = (pPvtCalcData->endSpeed - uniformSpeed) / squareMotionTime;
            
            //��ʼ��ʱ��
            minTime  = 0;
            lastTime = 0;
            curTime  = decTime / decStep;
            maxTime  = decTime;
            pPvtCalcData->lastStepSpeed = 0;

            realStep = realDecPosn / decStep;
            
            //����һ�������һ������������
            for (i = 1;i < decStep;i++)    
            {
                calcCount = 0; 
                targetStep = realStep * i;

                //����һ����ʱ�����ϼ�����һ�����ٶ���Ϊ��һ���ļ����ʼʱ��
                curTime += pPvtCalcData->lastStepSpeed;    
                if (curTime > decTime)
                {
                    curTime = decTime;
                }

                //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
                curPost = ((fourTermCoef * curTime  + threeTermCoef) * curTime * curTime + uniformSpeed) * curTime;
                
                //��Ŀ��ֵ�����̫������Ҫ��������
                posnOffset = curPost - targetStep;
                while ((posnOffset > pPvtCalcData->maxOffset) ||    //��Ŀ��ֵ�����̫���������
                       (posnOffset < pPvtCalcData->minOffset))
                {
                    calcCount++;
                    
                    if (calcCount >= PVT_ONE_POINT_CALC_TIMES_MAX)
                    {
                        //�������������������㣬����
                        errorCode = PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_3;
                        break;
                    }
                    else if (PVT_ONE_POINT_CALC_TIMES_MAX / 2 == calcCount)  //����һ�����ƺ��ȼӴ�maxTime
                    {
                        maxTime = decTime;
                    }

                    //���ֲ��ң��ı�ʱ���������
                    if (posnOffset > pPvtCalcData->maxOffset)
                    {
                        maxTime = curTime;
                        curTime = (curTime + minTime) * 0.5f;
                    }
                    else
                    {
                        minTime = curTime;
                        curTime = (curTime + maxTime) * 0.5f;
                    }

                    //P(t) = fourTermCoef * t^4 + threeTermCoef * t^3 + v0 * t
                    curPost = ((fourTermCoef * curTime  + threeTermCoef) * curTime * curTime + uniformSpeed) * curTime;
                    
                    posnOffset = curPost - targetStep;
                }

                //������������������������㣬����
                if ((PVT_CALC_TOO_MANY_CALC_TIMES | PVT_CALC_SECTION_3) == errorCode)
                {
                    break;
                }
                
                //��¼��һ������һ���ļ��ʱ�䣬Ҳ�����ٶ�
                pPvtCalcData->lastStepSpeed = curTime - lastTime;    
                if (pPvtCalcData->lastStepSpeed <= 0.0f)
                {
                    errorCode = PVT_CALC_SPEED_EQUAL_ZERO | PVT_CALC_SECTION_3;
                    pPvtCalcData->lastStepSpeed = 0;
                }
                
                //����ʱ�䣬׼����һ���ļ���
                pPvtCalcData->targetStep++;
                minTime = curTime;
                lastTime = curTime;
                maxTime = curTime + pPvtCalcData->lastStepSpeed * 3.0f;
                if (maxTime > decTime)
                {
                    maxTime = decTime;
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
    //���һ����ʱ��ֱ����motionTime��ȥ��һ����ʱ��λ��
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
