#ifndef __SERV_PT100_H_
#define __SERV_PT100_H_
#include "bspAdc.h"
#include "comStruct.h"

/*********************************************************************************************
�� �� ��:servGetPT100Value
ʵ�ֹ���:��ȡPT100���¶�ֵ  
�������: ��;
�������: ��;
�� �� ֵ: ��;

*********************************************************************************************/
u32 servGetPT100Value();

void servAnalogInSet(AnalogInfoStruct asensorInfo);
void servAdc1IntTimerCB(void *timeOutPara);
void irqAdc1AWD(void);



#endif
