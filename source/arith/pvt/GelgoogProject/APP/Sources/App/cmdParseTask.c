/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdparsetask.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.02.07;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <os.h>
#include "cmdparsetask.h"
#include "cmdMainParse.h"
#include "servCommIntfc.h"
#include "comInterface.h"



/****************************************�ⲿ��������*****************************************/
extern OS_SEM g_semCmdParseTask;
extern OS_SEM g_semPvtCalcTask;
extern OS_SEM g_semFunctionTask;
extern OS_SEM g_semEventManageTask;

extern CommIntfcStruct g_systemIntfc; 



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
bool g_bCmdPostSemToPvt = false;
bool g_bCmdPostSemToFunc = false;
bool g_bCmdPostSemToEvent = false;



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: CmdParseTask;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void CmdParseTask(void *p_arg)
{
    OS_ERR  os_err;

    (void)p_arg;

    while (1)
    {
        //�ȴ��ź���
        OSSemPend(&g_semCmdParseTask, 0, OS_OPT_PEND_BLOCKING, NULL, &os_err);
        
        //�������Buffer�е�����
        servRxFrameProcess();

        //�������
        cmdFrameProcess();
        
        //������Buffer�е�����
        servTxFrameProcess();

        if (g_bCmdPostSemToEvent)
        {
            OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, &os_err);

            g_bCmdPostSemToEvent = false;
        }

        if (g_bCmdPostSemToFunc)
        {
            OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);

            g_bCmdPostSemToFunc = false;
        }

        if (g_bCmdPostSemToPvt)
        {
            OSSemPost(&g_semPvtCalcTask, OS_OPT_POST_ALL, &os_err);

            g_bCmdPostSemToPvt = false;
        }
        
        /*OSTimeDlyHMSM(0, 0, 0, 1, OS_OPT_TIME_HMSM_STRICT, &os_err);*/
    }
}



/*******************************************�ļ�β********************************************/
