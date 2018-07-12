/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfMotor.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_MOTOR_H
#define __PARA_VERIFY_MOTOR_H



/******************************************包含文件*******************************************/
#include "comDatatype.h"
#include "comStruct.h"
#include "comErrorcode.h"
#include "comDebug.h"



/****************************************外部变量声明*****************************************/



/***************************************常数和类型声明****************************************/



/*******************************************宏定义********************************************/ 



/******************************************变量声明*******************************************/



/******************************************函数声明*******************************************/
u8 pvrfMotorGearRatioVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorStepAngleVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorPeakSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorPeakAccVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorPosnUnitVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorSizeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorLeadVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorVoltVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorCurrVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotorBacklashVerify(u8 dataLen, u8 *pData, void *pParaValue);

u8 pvrfEncoderLineNumVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfEncoderChanNumVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfEncoderTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfEncoderMultipleVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfEncoderStateVerify(u8 dataLen, u8 *pData, void *pParaValue);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/