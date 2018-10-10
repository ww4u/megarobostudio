/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdMotor.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2016.12.15;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include "project.h"
#include "cmdMotor.h"
#include "pvrfMotor.h"
#include "cmdMainParse.h"
#include "servSystemPara.h"
#include "servCommIntfc.h"
#include "bspTimer.h"
#include "servAlgorithm.h"
#include "servSoftTimer.h"



/****************************************外部变量声明*****************************************/
extern bool g_bCmdPostSemToFunc;
extern bool g_bCmdPostSemToEvent;

extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;
extern MotorInfoStruct   g_motorInfo;
extern ChanCfgBmpStruct  g_chanCfgBmp[CH_TOTAL];
extern TrigInInfoStruct  g_trigInInfo;

extern SensorUartStruct  g_sensorUart;
extern DriverInfoStruct  g_driverInfo;

extern EventSrcBmpStruct g_eventSrcBmp;

extern SoftTimerStruct g_paraSaveTimer;



/*****************************************局部宏定义******************************************/
#define    BACKLASH_TEST    1



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pMotroCmdFunc[MOTORCMD_RESERVE];
SubCmdProFunc pEncoderCmdFunc[ENCODERCMD_RESERVE];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdMotorGearRatioNumSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorGearRatioNumSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16 gearRatio;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorGearRatioVerify(cmdDataLen, pCmdData, (void *)&gearRatio))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].gearRatioNum = gearRatio;

#if !GELGOOG_AXIS_10                            
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                                      &g_systemState.posnConvertInfo[i]);
#else
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.microStep,
                                      &g_systemState.posnConvertInfo[i]);
#endif
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorGearRatioNumQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorGearRatioNumQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].gearRatioNum) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].gearRatioNum;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].gearRatioNum);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_GRATIONUMQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].gearRatioNum;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].gearRatioNum);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_GRATIONUMQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorGearRatioDenSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorGearRatioDenSet(u8 cmdDataLen, u8 *pCmdData)
{
    u16 gearRatio;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorGearRatioVerify(cmdDataLen, pCmdData, (void *)&gearRatio))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].gearRatioDen = gearRatio;

#if !GELGOOG_AXIS_10                            
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                                      &g_systemState.posnConvertInfo[i]);
#else
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.microStep,
                                      &g_systemState.posnConvertInfo[i]);
#endif
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorGearRatioDenQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorGearRatioDenQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].gearRatioDen) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].gearRatioDen;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].gearRatioDen);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_GRATIODENQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].gearRatioDen;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].gearRatioDen);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_GRATIODENQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorStepAngleSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorStepAngleSet(u8 cmdDataLen, u8 *pCmdData)
{
    StepAngleEnum stepAngel;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorStepAngleVerify(cmdDataLen, pCmdData, (void *)&stepAngel))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].stepAngel = stepAngel;

#if !GELGOOG_AXIS_10                            
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                                      &g_systemState.posnConvertInfo[i]);
#else
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.microStep,
                                      &g_systemState.posnConvertInfo[i]);
#endif
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorStepAngleQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorStepAngleQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].stepAngel) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].stepAngel;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].stepAngel);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_STEPSQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].stepAngel;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].stepAngel);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_STEPSQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    MotorTypeEnum motorType;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorTypeVerify(cmdDataLen, pCmdData, (void *)&motorType))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].motorType = motorType;
                
                if (MOTOR_ROTARY == g_motorInfo.motor[i].motorType)
                {
                    g_motorInfo.motor[i].posnUnit = POSTUNIT_ANGLE;
                }
                else if (MOTOR_LINEAR == g_motorInfo.motor[i].motorType)
                {
                    g_motorInfo.motor[i].posnUnit = POSTUNIT_MMETER;    
                }
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].motorType) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].motorType;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].motorType);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_TYPEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].motorType;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].motorType);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_TYPEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorPosnUnitSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorPosnUnitSet(u8 cmdDataLen, u8 *pCmdData)
{
    PostUnitEnum  posnUnit;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorPosnUnitVerify(cmdDataLen, pCmdData, (void *)&posnUnit))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].posnUnit = posnUnit;
            }
        }

        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorPosnUnitQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorPosnUnitQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].posnUnit) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].posnUnit;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].posnUnit);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_POSNUNITQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].posnUnit;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].posnUnit);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_POSNUNITQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorLeadSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorLeadSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 lead;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorLeadVerify(cmdDataLen, pCmdData, (void *)&lead))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].lead = lead;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorLeadQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorLeadQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].lead) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].lead;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].lead);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_LEADQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].lead;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].lead);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_LEADQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorPeakSpeedSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorPeakSpeedSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 peakSpeed;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorPeakSpeedVerify(cmdDataLen, pCmdData, (void *)&peakSpeed))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].peakSpeed = peakSpeed;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorPeakSpeedQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorPeakSpeedQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].peakSpeed) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].peakSpeed;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].peakSpeed);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_PEAKSPEEDQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].peakSpeed;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].peakSpeed);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_PEAKSPEEDQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorPeakAccSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorPeakAccSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 peakAcc;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorPeakAccVerify(cmdDataLen, pCmdData, (void *)&peakAcc))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].peakAcc = peakAcc;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorPeakAccQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorPeakAccQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].peakAcc) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].peakAcc;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].peakAcc);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_PEAKACCQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].peakAcc;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].peakAcc);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_PEAKACCQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorSizeSet(u8 cmdDataLen, u8 *pCmdData)
{
    MotorSizeEnum motorSize;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorSizeVerify(cmdDataLen, pCmdData, (void *)&motorSize))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].motorSize = motorSize;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorSizeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorSizeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].motorSize) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].motorSize;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].motorSize);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_SIZEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].motorSize;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].motorSize);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_SIZEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorVoltSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorVoltSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 motorVolt;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorVoltVerify(cmdDataLen, pCmdData, (void *)&motorVolt))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].motorVolt = motorVolt;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorVoltQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorVoltQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].motorVolt) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].motorVolt;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].motorVolt);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_VOLTQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].motorVolt;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].motorVolt);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_VOLTQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorCurrSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorCurrSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 motorCurr;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorCurrVerify(cmdDataLen, pCmdData, (void *)&motorCurr))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].motorCurr = motorCurr;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorCurrQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorCurrQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].motorCurr) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].motorCurr;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].motorCurr);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_CURRQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].motorCurr;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].motorCurr);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_CURRQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorBacklashSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorBacklashSet(u8 cmdDataLen, u8 *pCmdData)
{
    f32 backlash;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfMotorBacklashVerify(cmdDataLen, pCmdData, (void *)&backlash))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].backlash = backlash;
            }
        }
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdMotorBacklashQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorBacklashQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].backlash) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].backlash;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].backlash);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_MOTOR, MOTORCMD_BACKLASHQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].backlash;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].backlash);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_MOTOR, MOTORCMD_BACKLASHQ, dataLen, data);
            }
        }
    }
}


#if BACKLASH_TEST
/*********************************************************************************************
函 数 名: cmdMotorBacklashTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorBacklashTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    //if (PARA_VERIFY_NO_ERROR == pvrfMotorBacklashVerify(cmdDataLen, pCmdData, (void *)&backlash))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_chanCfgBmp[i].bBlReset = true;
                g_bCmdPostSemToFunc = true;
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdMotorBacklashTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorBacklashTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;
    
    //进行参数验证
    //if (PARA_VERIFY_NO_ERROR == pvrfMotorBacklashVerify(cmdDataLen, pCmdData, (void *)&backlash))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_eventSrcBmp.backLashRead[i] = true;
                g_bCmdPostSemToEvent = true;
            }
        }
    }
}
#endif
 

/*********************************************************************************************
函 数 名: cmdMotorCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorCmdInit(void)
{
    memset(pMotroCmdFunc, 0, sizeof(pMotroCmdFunc));

    pMotroCmdFunc[MOTORCMD_STEPS]       = cmdMotorStepAngleSet;
    pMotroCmdFunc[MOTORCMD_STEPSQ]      = cmdMotorStepAngleQuery;  
    pMotroCmdFunc[MOTORCMD_TYPE]        = cmdMotorTypeSet;
    pMotroCmdFunc[MOTORCMD_TYPEQ]       = cmdMotorTypeQuery;
    
    //CJ 2017.06.29 Add
    pMotroCmdFunc[MOTORCMD_POSNUNIT]   = cmdMotorPosnUnitSet; 
    pMotroCmdFunc[MOTORCMD_POSNUNITQ]  = cmdMotorPosnUnitQuery;
    
    pMotroCmdFunc[MOTORCMD_GRATIONUM]   = cmdMotorGearRatioNumSet;
    pMotroCmdFunc[MOTORCMD_GRATIONUMQ]  = cmdMotorGearRatioNumQuery;
    pMotroCmdFunc[MOTORCMD_GRATIODEN]   = cmdMotorGearRatioDenSet;
    pMotroCmdFunc[MOTORCMD_GRATIODENQ]  = cmdMotorGearRatioDenQuery;
    pMotroCmdFunc[MOTORCMD_LEAD]        = cmdMotorLeadSet;
    pMotroCmdFunc[MOTORCMD_LEADQ]       = cmdMotorLeadQuery;
    pMotroCmdFunc[MOTORCMD_PEAKSPEED]   = cmdMotorPeakSpeedSet;
    pMotroCmdFunc[MOTORCMD_PEAKSPEEDQ]  = cmdMotorPeakSpeedQuery;
    pMotroCmdFunc[MOTORCMD_PEAKACC]     = cmdMotorPeakAccSet;
    pMotroCmdFunc[MOTORCMD_PEAKACCQ]    = cmdMotorPeakAccQuery;
    pMotroCmdFunc[MOTORCMD_SIZE]        = cmdMotorSizeSet;
    pMotroCmdFunc[MOTORCMD_SIZEQ]       = cmdMotorSizeQuery;    
    pMotroCmdFunc[MOTORCMD_VOLT]        = cmdMotorVoltSet;
    pMotroCmdFunc[MOTORCMD_VOLTQ]       = cmdMotorVoltQuery;
    pMotroCmdFunc[MOTORCMD_CURR]        = cmdMotorCurrSet;
    pMotroCmdFunc[MOTORCMD_CURRQ]       = cmdMotorCurrQuery;
    pMotroCmdFunc[MOTORCMD_BACKLASH]    = cmdMotorBacklashSet;
    pMotroCmdFunc[MOTORCMD_BACKLASHQ]   = cmdMotorBacklashQuery;

    #if BACKLASH_TEST
    pMotroCmdFunc[MOTORCMD_BACKLASHTEST]  = cmdMotorBacklashTestSet;
    pMotroCmdFunc[MOTORCMD_BACKLASHTESTQ] = cmdMotorBacklashTestQuery;
    #endif
}

            
/*********************************************************************************************
函 数 名: cmdMotorCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdMotorCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < MOTORCMD_RESERVE) && (pMotroCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pMotroCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}


#if 0
#endif
/*********************************************************************************************
函 数 名: cmdEncoderLineNumSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderLineNumSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 lineNum;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderLineNumVerify(cmdDataLen, pCmdData, (void *)&lineNum))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].encoderLineNum = lineNum;

#if !GELGOOG_AXIS_10                            
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                                      &g_systemState.posnConvertInfo[i]);
#else
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.microStep,
                                      &g_systemState.posnConvertInfo[i]);
#endif
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderLineNumQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderLineNumQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].encoderLineNum) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].encoderLineNum;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].encoderLineNum);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCODER, ENCODERCMD_LINENUMQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].encoderLineNum;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].encoderLineNum);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_ENCODER, ENCODERCMD_LINENUMQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderChanNumSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderChanNumSet(u8 cmdDataLen, u8 *pCmdData)
{
    EncoderChanEnum encoderChanNum;
    u8 i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderChanNumVerify(cmdDataLen, pCmdData, (void *)&encoderChanNum))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].encoderChanNum = encoderChanNum;

                //单通道编码器不支持4倍乘
                if ((ECCHAN_1 == encoderChanNum) && (ENCMULT_QUADR == g_motorInfo.motor[i].encoderMult))
                {
                    //默认恢复成单倍乘
                    g_motorInfo.motor[i].encoderMult = ENCMULT_SINGLE;
                }

                g_chanCfgBmp[i].bEncoder = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderChanNumQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderChanNumQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].encoderChanNum) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].encoderChanNum;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].encoderChanNum);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCODER, ENCODERCMD_CHANNUMQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].encoderChanNum;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].encoderChanNum);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_ENCODER, ENCODERCMD_CHANNUMQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    EncoderTypeEnum type;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderTypeVerify(cmdDataLen, pCmdData, (void *)&type))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].encoderType = type;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderTypeQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderTypeQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].encoderType) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].encoderType;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].encoderType);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCODER, ENCODERCMD_TYPEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].encoderType;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].encoderType);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_ENCODER, ENCODERCMD_TYPEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderMultipleSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderMultipleSet(u8 cmdDataLen, u8 *pCmdData)
{
    EncoderMultiEnum encoderMult;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderMultipleVerify(cmdDataLen, pCmdData, (void *)&encoderMult))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                //单通道编码器不支持4倍乘
                if ((ECCHAN_1 == g_motorInfo.motor[i].encoderChanNum) && (ENCMULT_QUADR == encoderMult))
                {
                    g_systemState.eventCode[ERROR_CODE_INDEX_PARA_VERIFY] = PARA_VERIFY_ERROR_TYPE;
                }
                else
                {
                    g_motorInfo.motor[i].encoderMult = encoderMult;
                    
#if !GELGOOG_AXIS_10                            
                    //重新计算下位置到微步和编码器线的转换系数
                    servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                          g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                                          &g_systemState.posnConvertInfo[i]);
#else
                    //重新计算下位置到微步和编码器线的转换系数
                    servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                          g_driverInfo.microStep,
                                          &g_systemState.posnConvertInfo[i]);
#endif

                    g_chanCfgBmp[i].bEncoder = true;
                    g_bCmdPostSemToFunc = true;
                }
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderMultipleQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderMultipleQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].encoderMult) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].encoderMult;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].encoderMult);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCODER, ENCODERCMD_MULTIPLEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].encoderMult;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].encoderMult);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_ENCODER, ENCODERCMD_MULTIPLEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderStateSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    IntfcStateEnum encoderState;
    u8 i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderStateVerify(cmdDataLen, pCmdData, (void *)&encoderState))
    {
        for (i = 0;i < ENCODER_SUPPORT_NUM;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].encoderState = encoderState;
                
                g_chanCfgBmp[i].bEncoder = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderStateQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderStateQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].encoderState) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].encoderState;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].encoderState);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCODER, ENCODERCMD_STATEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].encoderState;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].encoderState);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_ENCODER, ENCODERCMD_STATEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderFeedbackRatioSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderFeedbackRatioSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8   feedbackRatio;
    u8   i;
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                bConfig[i] = true;
            }
        }
    }
    cmdDataLen -= 1;

    
    //进行参数验证
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderFeedbackRatioVerify(cmdDataLen, pCmdData, (void *)&feedbackRatio))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].feedbackRatio = feedbackRatio;

#if !GELGOOG_AXIS_10                            
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                                      &g_systemState.posnConvertInfo[i]);
#else
                //重新计算下位置到微步和编码器线的转换系数
                servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                                      g_driverInfo.microStep,
                                      &g_systemState.posnConvertInfo[i]);
#endif
            }
        }
        
        servStimerAdd(&g_paraSaveTimer);
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderFeedbackRatioQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderFeedbackRatioQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_motorInfo.motor[CH1].feedbackRatio) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_motorInfo.motor[chanNum].feedbackRatio;
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_motorInfo.motor[chanNum].feedbackRatio);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_ENCODER, ENCODERCMD_FEEDBACKRATIOQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_motorInfo.motor[i].feedbackRatio;
                data[0] = i;
                for (j = 0;j < sizeof(g_motorInfo.motor[i].feedbackRatio);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_ENCODER, ENCODERCMD_FEEDBACKRATIOQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdEncoderCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderCmdInit(void)
{
    memset(pEncoderCmdFunc, 0, sizeof(pEncoderCmdFunc));

    pEncoderCmdFunc[ENCODERCMD_LINENUM]   = cmdEncoderLineNumSet;
    pEncoderCmdFunc[ENCODERCMD_LINENUMQ]  = cmdEncoderLineNumQuery;
    pEncoderCmdFunc[ENCODERCMD_CHANNUM]   = cmdEncoderChanNumSet;
    pEncoderCmdFunc[ENCODERCMD_CHANNUMQ]  = cmdEncoderChanNumQuery;
    pEncoderCmdFunc[ENCODERCMD_TYPE]      = cmdEncoderTypeSet;
    pEncoderCmdFunc[ENCODERCMD_TYPEQ]     = cmdEncoderTypeQuery;
    pEncoderCmdFunc[ENCODERCMD_MULTIPLE]  = cmdEncoderMultipleSet;
    pEncoderCmdFunc[ENCODERCMD_MULTIPLEQ] = cmdEncoderMultipleQuery;
    pEncoderCmdFunc[ENCODERCMD_STATE]     = cmdEncoderStateSet;
    pEncoderCmdFunc[ENCODERCMD_STATEQ]    = cmdEncoderStateQuery;
    
    pEncoderCmdFunc[ENCODERCMD_FEEDBACKRATIO]  = cmdEncoderFeedbackRatioSet;
    pEncoderCmdFunc[ENCODERCMD_FEEDBACKRATIOQ] = cmdEncoderFeedbackRatioQuery;
}

            
/*********************************************************************************************
函 数 名: cmdEncoderCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdEncoderCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < ENCODERCMD_RESERVE) && (pEncoderCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pEncoderCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************文件尾********************************************/
