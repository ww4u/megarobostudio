/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  periodtask.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.02.07;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <os.h>
#include "project.h"
#include "periodicTask.h"
#include "servFpga.h"
#include "servDriver.h"
#include "comCmd.h"
#include "servSoftTimer.h"
#include "servLed.h"
#include "servCommIntfc.h"



/****************************************�ⲿ��������*****************************************/
extern OS_SEM g_semPeriodicTask;
extern OS_SEM g_semFunctionTask;
extern OS_SEM g_semCmdParseTask;
extern OS_SEM g_semEventManageTask;
extern OS_SEM g_semParaSaveTask;

extern SoftTimerStruct g_ciUartDmaRecTimer;
extern SoftTimerStruct g_senUartDmaRecTimer[UARTNUM_RESERVE];
extern SoftTimerStruct g_senUartSwitchTimer[UARTNUM_RESERVE];
extern SoftTimerStruct g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];
extern SoftTimerStruct g_motionStateTimer[CH_TOTAL];
extern SoftTimerStruct g_motionMonitorTimer[CH_TOTAL];
extern SoftTimerStruct g_driverCurrTimer[CH_TOTAL];

extern SoftTimerStruct g_runTimeCountTimer[CH_TOTAL];

extern SoftTimerStruct g_paraSaveTimer;

#if GELGOOG_AXIS_10
extern SoftTimerStruct g_driverFaultTimer;
#else
extern SoftTimerStruct g_drvStateReadTimer[CH_TOTAL];
#endif


#ifdef PROJECT_QUBELEY
extern SoftTimerStruct g_analogInTimer;
#endif


#ifdef PROJECT_GELGOOG

#if !GELGOOG_SINANJU
extern SoftTimerStruct g_isolatorInTimer;

#else
extern SoftTimerStruct g_distAlarmLedTimer;
#endif

#endif


extern SoftTimerStruct   g_ledTimer;
extern SensorDataStruct  g_sensorData;
extern SystemStateStruct g_systemState; 

#if DRIVE_FROM_ARM
extern GPIO_TypeDef* stepGpio;
extern u16           stepPin;
extern GPIO_TypeDef* dirGpio;
extern u16           dirNum;
#endif



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
bool g_bPerdPostSemToFunc  = false;
bool g_bPerdPostSemToCmd   = false;
bool g_bPerdPostSemToEvent = false;
bool g_bPerdPostSemToPara  = false;



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: PeriodicTask;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void PeriodicTask(void *p_arg)
{
    OS_ERR  os_err;
    u16 i, j;
    (void)p_arg;


    u16 timeCount = 0;
    u16 readCount = 0;
    DdrStateEnum ddrState;
    RunStatenum  runState;
    bool bHaveClear = false;

#if DRIVE_FROM_ARM
    GPIO_InitTypeDef GPIO_InitStructure;

  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //GPIO_SetBits(GPIOA, GPIO_Pin_15);
#endif
    
    while (1)
    {
        OSSemPend(&g_semPeriodicTask, PERIOD_TASK_PERIOD, OS_OPT_PEND_BLOCKING, NULL, &os_err);
        
        
        servStimerExamine(&g_ledTimer, NULL);

        if (ENGINMODE_NONE != g_systemState.EnginMode)
        {
            if (ENGINMODE_DDRTEST == g_systemState.EnginMode)
            {
                //ÿ1s(10��)��ѯ�¼Ĵ���D10����ȡDDR״̬
                timeCount++;
                if (timeCount >= (UNIT_S_TO_MS / PERIOD_TASK_PERIOD))
                {
                    timeCount = 0;
                    ddrState = servFpgaDDRStateGet();
                    readCount++;

                    if (!bHaveClear)
                    {
                        //��һ�ζ��������������������ͬʱд�Ĵ���ADDR_FPGA_DDR_WAVE_READ_DATA�������ʶ
                        ddrState = DDR_NOERROR;
                        bHaveClear = true;
                        servFpgaDDRErrorClear();
                        
                        servLedSet(LED_ERROR);

                        bspDelayMs(1000);
                        
                        servLedSet(LED_IDEL);
                    }

                    if (DDR_NOINIT == ddrState)    //��ʼ��δ���
                    {
                        g_systemState.eventCode[1] = FPGA_DDR_VERIFY_ERROR;    //��һ��Ԫ�طŲ���״̬: 0xFF-����δ����; 0-�������
                        
                        servLedSet(LED_ERROR);
                    }
                    else if (DDR_TESTERROR == ddrState)    //�Բ��д�
                    {
                        g_systemState.eventCode[1] = FPGA_DDR_TEST_ERROR;    //��һ��Ԫ�طŲ���״̬: 0xFF-����δ����; 0-�������
                        
                        servLedSet(LED_ERROR1);
                    }
                }

                if (readCount >= (90 * UNIT_S_TO_MS / PERIOD_TASK_PERIOD))    //��90s�Ϳ��Խ�����
                {
                    readCount = 0;
                    g_systemState.EnginMode = ENGINMODE_NONE;
                    g_systemState.eventCode[0] = 0;
                }
            }
            else if (ENGINMODE_DRIVETEST == g_systemState.EnginMode)
            {
                runState = servFpgaRunStateGet(CH1);    //FOR MODIFY NICK
                if (RUNSTETE_NORUN == runState)
                {
                    //
                }
            }
            else if (ENGINMODE_ENCODERTEST == g_systemState.EnginMode)
            {
                
            }
            else if (ENGINMODE_ISOLINTEST == g_systemState.EnginMode)
            {
                
            }
            else if (ENGINMODE_ISOLOUTTEST == g_systemState.EnginMode)
            {
                
            }
            else if (ENGINMODE_ANALOGINTEST == g_systemState.EnginMode)
            {
                
            }
            else if (ENGINMODE_SUARTTEST == g_systemState.EnginMode)
            {
                
            }
        }
        else
        {
            //UART DMA���ն�ʱ��
            servStimerExamine(&g_ciUartDmaRecTimer, NULL);

            servStimerExamine(&g_paraSaveTimer, NULL);

            for (i = 0;i < UARTNUM_RESERVE;i++)
            {
                servStimerExamine(&g_senUartDmaRecTimer[i], (void *)&i);
                servStimerExamine(&g_senUartSwitchTimer[i], (void *)&i);
            }
            
            for (i = 0;i < CH_TOTAL;i++)
            {
                //�˶�״̬
                servStimerExamine(&g_motionStateTimer[i], (void *)&i);

                //�˶����
                servStimerExamine(&g_motionMonitorTimer[i], (void *)&i);

#if !GELGOOG_AXIS_10
#if TUNING_SUPPORT
                //����״̬
                servStimerExamine(&g_drvStateReadTimer[i], (void *)&i);
#endif
#endif

                servStimerExamine(&g_runTimeCountTimer[i], (void *)&i);

                //�����л�
                servStimerExamine(&g_driverCurrTimer[i], (void *)&i);

                //�ϱ�����
                for (j = 0;j < REPTTYPE_RESERVE;j++)    //REPTTYPE_ANGLESEN������ͨ����������ͬ
                {
                    servStimerExamine(&g_reportTimer[i][j], (void *)&i);
                }
            }
            
#ifdef PROJECT_QUBELEY
            //ģ������
            servStimerExamine(&g_analogInTimer, NULL);
#endif

#ifdef PROJECT_GELGOOG

#if !GELGOOG_SINANJU
            //��������
            servStimerExamine(&g_isolatorInTimer, NULL);

#else

            servStimerExamine(&g_distAlarmLedTimer, NULL);
#endif

#if GELGOOG_AXIS_10
            servStimerExamine(&g_driverFaultTimer, NULL);
#endif

#endif
        }

        if (g_bPerdPostSemToCmd)
        {
            g_bPerdPostSemToCmd = false;

            OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, &os_err);
        }

        if (g_bPerdPostSemToFunc)
        {
            g_bPerdPostSemToFunc = false; 

            OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);
        }

        if (g_bPerdPostSemToEvent)
        {
            g_bPerdPostSemToEvent = false;

            OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, &os_err);
        }

        if (g_bPerdPostSemToPara)
        {
            g_bPerdPostSemToPara = false;
            
            OSSemPost(&g_semParaSaveTask, OS_OPT_POST_ALL, NULL);
        }



        
#if DRIVE_FROM_ARM        
        GPIO_SetBits(GPIOA, GPIO_Pin_15);
        bspDelayUs(500);
        GPIO_ResetBits(GPIOA, GPIO_Pin_15);
        bspDelayUs(500);
#endif
    }
}



/*******************************************�ļ�β********************************************/
