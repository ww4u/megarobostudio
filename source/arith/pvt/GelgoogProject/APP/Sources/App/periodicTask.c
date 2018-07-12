/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  periodtask.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.02.07;
历史版本:  无;
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



/****************************************外部变量声明*****************************************/
extern OS_SEM g_semPeriodicTask;
extern OS_SEM g_semFunctionTask;
extern OS_SEM g_semCmdParseTask;
extern OS_SEM g_semEventManageTask;

extern SoftTimerStruct g_ciUartDmaRecTimer;
extern SoftTimerStruct g_senUartDmaRecTimer[UARTNUM_RESERVE];
extern SoftTimerStruct g_senUartSwitchTimer[UARTNUM_RESERVE];
extern SoftTimerStruct g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];
extern SoftTimerStruct g_motionStateTimer[CH_TOTAL];
extern SoftTimerStruct g_motionMonitorTimer[CH_TOTAL];

#ifdef PROJECT_QUBELEY
extern SoftTimerStruct g_analogInTimer;
#endif

#ifdef PROJECT_GELGOOG
extern SoftTimerStruct g_isolatorInTimer;
#endif

extern SoftTimerStruct g_ledTimer;

extern SensorDataStruct  g_sensorData;
extern SystemStateStruct g_systemState; 

#if DRIVE_FROM_ARM
extern GPIO_TypeDef* stepGpio;
extern u16           stepPin;
extern GPIO_TypeDef* dirGpio;
extern u16           dirNum;
#endif



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
bool g_bPerdPostSemToFunc  = false;
bool g_bPerdPostSemToCmd   = false;
bool g_bPerdPostSemToEvent = false;



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: PeriodicTask;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
    u32 tmcState;
    BitAction BitVal = Bit_SET;
#endif
    
    while (1)
    {
        OSSemPend(&g_semPeriodicTask, PERIOD_TASK_PERIOD, OS_OPT_PEND_BLOCKING, NULL, &os_err);
        
        
        servStimerExamine(&g_ledTimer, NULL);

        if (ENGINMODE_NONE != g_systemState.EnginMode)
        {
            if (ENGINMODE_DDRTEST == g_systemState.EnginMode)
            {
                //每1s(10次)查询下寄存器D10，获取DDR状态
                timeCount++;
                if (timeCount >= (UNIT_S_TO_MS / PERIOD_TASK_PERIOD))
                {
                    timeCount = 0;
                    ddrState = servFpgaDDRStateGet();
                    readCount++;

                    if (!bHaveClear)
                    {
                        //第一次读回来的数据先清除掉，同时写寄存器ADDR_FPGA_DDR_WAVE_READ_DATA，清除标识
                        ddrState = DDR_NOERROR;
                        bHaveClear = true;
                        servFpgaDDRErrorClear();
                        
                        servLedSet(LED_ERROR);

                        bspDelayMs(1000);
                        
                        servLedSet(LED_IDEL);
                    }

                    if (DDR_NOINIT == ddrState)    //初始化未完成
                    {
                        g_systemState.errorCode[1] = FPGA_DDR_VERIFY_ERROR;    //第一个元素放测试状态: 0xFF-测试未结束; 0-测试完成
                        
                        servLedSet(LED_ERROR);
                    }
                    else if (DDR_TESTERROR == ddrState)    //自测有错
                    {
                        g_systemState.errorCode[1] = FPGA_DDR_TEST_ERROR;    //第一个元素放测试状态: 0xFF-测试未结束; 0-测试完成
                        
                        servLedSet(LED_ERROR1);
                    }
                }

                if (readCount >= (90 * UNIT_S_TO_MS / PERIOD_TASK_PERIOD))    //测90s就可以结束了
                {
                    readCount = 0;
                    g_systemState.EnginMode = ENGINMODE_NONE;
                    g_systemState.errorCode[0] = 0;
                }
            }
            else if (ENGINMODE_DRIVETEST == g_systemState.EnginMode)
            {
                runState = servFpgaRunStateGet(CH1);    //FOR MODIFY NICK
                if (RUNSTETE_NORUN == runState)
                {
                    //通过CAN总线查询测试结果
                    servFrameSend(CMD_FACTORY, FACTORYCMD_DRIVERENDQ, 0, NULL, LINK_CAN);
                    g_bPerdPostSemToCmd = true;
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
            //UART DMA接收定时器
            servStimerExamine(&g_ciUartDmaRecTimer, NULL);

            for (i = 0;i < UARTNUM_RESERVE;i++)
            {
                servStimerExamine(&g_senUartDmaRecTimer[i], (void *)&i);
                servStimerExamine(&g_senUartSwitchTimer[i], (void *)&i);
            }
            
            if (SENSOR_ON == g_systemState.reportSwitch)
            {
                for (i = 0;i < g_systemState.chanNum;i++)
                {
                    //运动状态
                    servStimerExamine(&g_motionStateTimer[i], (void *)&i);

                    //运动监控
                    servStimerExamine(&g_motionMonitorTimer[i], (void *)&i);

                    //上报数据
                    for (j = 0;j < REPTTYPE_RESERVE;j++)    //REPTTYPE_ANGLESEN数量和通道数量不等同
                    {
                        servStimerExamine(&g_reportTimer[i][j], (void *)&i);
                    }
                }
                
                for (i = 0;i < ANGLE_SENSOR_NUM;i++)
                {
                    servStimerExamine(&g_reportTimer[i][REPTTYPE_ANGLESEN], (void *)&i);
                }
            }
            
#ifdef PROJECT_QUBELEY
            //模拟输入
            servStimerExamine(&g_analogInTimer, NULL);
#endif

#ifdef PROJECT_GELGOOG
            //隔离输入
            servStimerExamine(&g_isolatorInTimer, NULL);
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



        
#if DRIVE_FROM_ARM
        GPIO_WriteBit(dirGpio, dirNum, BitVal);
        
        GPIO_WriteBit(stepGpio, stepPin, Bit_SET);
        bspDelayUs(500);
        GPIO_WriteBit(stepGpio, stepPin, Bit_RESET);
        bspDelayUs(500);
        GPIO_WriteBit(stepGpio, stepPin, Bit_SET);
        bspDelayUs(500);
        GPIO_WriteBit(stepGpio, stepPin, Bit_RESET);
        bspDelayUs(500);
        GPIO_WriteBit(stepGpio, stepPin, Bit_SET);
        bspDelayUs(500);
        GPIO_WriteBit(stepGpio, stepPin, Bit_RESET);
        bspDelayUs(500);
        GPIO_WriteBit(stepGpio, stepPin, Bit_SET);
        bspDelayUs(500);
        GPIO_WriteBit(stepGpio, stepPin, Bit_RESET);
        bspDelayUs(500);

        //servTMC_Reg_Read(RDSEL_MICRO_POS, &tmcState);
#endif
    }
}



/*******************************************文件尾********************************************/
