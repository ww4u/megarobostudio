/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdparsetask.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.02.07;
历史版本:  无;
*********************************************************************************************/
#include <os.h>
#include "cmdparsetask.h"
#include "cmdMainParse.h"
#include "servCommIntfc.h"
#include "comInterface.h"



/****************************************外部变量声明*****************************************/
extern OS_SEM g_semCmdParseTask;
extern OS_SEM g_semPvtCalcTask;
extern OS_SEM g_semFunctionTask;
extern OS_SEM g_semEventManageTask;

extern CommIntfcStruct g_systemIntfc; 



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
bool g_bCmdPostSemToPvt = false;
bool g_bCmdPostSemToFunc = false;
bool g_bCmdPostSemToEvent = false;



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: CmdParseTask;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void CmdParseTask(void *p_arg)
{
    OS_ERR  os_err;

    (void)p_arg;

    while (1)
    {
        //等待信号量
        OSSemPend(&g_semCmdParseTask, 0, OS_OPT_PEND_BLOCKING, NULL, &os_err);
        
        //处理接收Buffer中的数据
        servRxFrameProcess();

        //处理命令集
        cmdFrameProcess();
        
        //处理发送Buffer中的数据
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



/*******************************************文件尾********************************************/
