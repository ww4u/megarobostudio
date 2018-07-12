/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvrfMotion.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __PARA_VERIFY_MOTION_H
#define __PARA_VERIFY_MOTION_H



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
u8 pvrfMotionStateReportVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionStartSrcVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionStartTypeVerify(u8 dataLen, u8 *pData, void *pParaValue);

u8 pvrfMotionMaxSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMinSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMaxPosnVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMinPosnVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMaxTorqueVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMinTorqueVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMaxAccVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionMinAccVerify(u8 dataLen, u8 *pData, void *pParaValue);

u8 pvrfMotionOffsetStateVerify(u8 dataLen, u8 *pData, void *pParaValue);
u8 pvrfMotionReverseMotionVerify(u8 dataLen, u8 *pData, void *pParaValue);



/*****************************************头文件保护******************************************/
#endif
/*******************************************文件尾********************************************/
