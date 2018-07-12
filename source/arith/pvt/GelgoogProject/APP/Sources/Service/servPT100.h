#ifndef __SERV_PT100_H_
#define __SERV_PT100_H_
#include "bspAdc.h"
#include "comStruct.h"

/*********************************************************************************************
函 数 名:servGetPT100Value
实现功能:获取PT100的温度值  
输入参数: 无;
输出参数: 无;
返 回 值: 无;

*********************************************************************************************/
u32 servGetPT100Value();

void servAnalogInSet(AnalogInfoStruct asensorInfo);
void servAdc1IntTimerCB(void *timeOutPara);
void irqAdc1AWD(void);



#endif
