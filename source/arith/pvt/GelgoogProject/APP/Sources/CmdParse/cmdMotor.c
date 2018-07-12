/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdMotor.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "project.h"
#include "cmdMotor.h"
#include "pvrfMotor.h"
#include "cmdMainParse.h"
#include "servSystemPara.h"
#include "servCommIntfc.h"
#include "bspTimer.h"



/****************************************�ⲿ��������*****************************************/
extern bool g_bCmdPostSemToFunc;

extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;
extern MotorInfoStruct   g_motorInfo;
extern ChanCfgBmpStruct  g_chanCfgBmp[CH_TOTAL];
extern TrigInInfoStruct  g_trigInInfo;

extern SensorUartStruct  g_sensorUart;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pMotroCmdFunc[MOTORCMD_RESERVE];
SubCmdProFunc pEncoderCmdFunc[ENCODERCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdMotorGearRatioNumSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorGearRatioVerify(cmdDataLen, pCmdData, (void *)&gearRatio))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].gearRatioNum = gearRatio;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorGearRatioNumQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorGearRatioDenSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorGearRatioVerify(cmdDataLen, pCmdData, (void *)&gearRatio))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].gearRatioDen = gearRatio;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorGearRatioDenQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorStepAngleSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorStepAngleVerify(cmdDataLen, pCmdData, (void *)&stepAngel))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].stepAngel = stepAngel;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorStepAngleQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorTypeVerify(cmdDataLen, pCmdData, (void *)&motorType))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
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
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorPosnUnitSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorPosnUnitVerify(cmdDataLen, pCmdData, (void *)&posnUnit))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].posnUnit = posnUnit;
            }
        }

        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorPosnUnitQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorLeadSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorLeadVerify(cmdDataLen, pCmdData, (void *)&lead))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].lead = lead;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorLeadQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorPeakSpeedSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorPeakSpeedVerify(cmdDataLen, pCmdData, (void *)&peakSpeed))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].peakSpeed = peakSpeed;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorPeakSpeedQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorPeakAccSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorPeakAccVerify(cmdDataLen, pCmdData, (void *)&peakAcc))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].peakAcc = peakAcc;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorPeakAccQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorSizeVerify(cmdDataLen, pCmdData, (void *)&motorSize))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].motorSize = motorSize;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorSizeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorVoltSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorVoltVerify(cmdDataLen, pCmdData, (void *)&motorVolt))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].motorVolt = motorVolt;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorVoltQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorCurrSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorCurrVerify(cmdDataLen, pCmdData, (void *)&motorCurr))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].motorCurr = motorCurr;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorCurrQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdMotorBacklashSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfMotorBacklashVerify(cmdDataLen, pCmdData, (void *)&backlash))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].backlash = backlash;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdMotorBacklashQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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


/*********************************************************************************************
�� �� ��: cmdMotorCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
}

            
/*********************************************************************************************
�� �� ��: cmdMotorCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdEncoderLineNumSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderLineNumVerify(cmdDataLen, pCmdData, (void *)&lineNum))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].encoderLineNum = lineNum;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdEncoderLineNumQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdEncoderChanNumSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdEncoderChanNumSet(u8 cmdDataLen, u8 *pCmdData)
{
    EncoderChanEnum encoderChanNum;
    u8 i;
#ifdef PROJECT_GELGOOG
    u8 j;
    u8 dataLen;
    u8 *pData;
    u8 data[6];
#endif
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderChanNumVerify(cmdDataLen, pCmdData, (void *)&encoderChanNum))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].encoderChanNum = encoderChanNum;

                //��ͨ����������֧��4����
                if ((ECCHAN_1 == encoderChanNum) && (ENCMULT_QUADR == g_motorInfo.motor[i].encoderMult))
                {
                    //Ĭ�ϻָ��ɵ�����
                    g_motorInfo.motor[i].encoderMult = ENCMULT_SINGLE;
                }
                
#ifdef PROJECT_GELGOOG
                if (INTFC_ON == g_motorInfo.motor[i].encoderState)
                {
                    //���߱�������TRIG_DIR���ö˿��ˣ�������Ҫ�ر�
                    if ((SENSOR_ON == g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR]) &&
                        (ECCHAN_3 == g_motorInfo.motor[i].encoderChanNum))
                    {
                        g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR] = SENSOR_OFF;
                        servTrigInInfoWrite(&g_trigInInfo);
                        
                        //֪ͨ��λ��IO״̬�仯��
                        dataLen = sizeof(g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR]) + sizeof(chanNum) + sizeof(TRIGPIN_DIR);
                        pData = (u8 *)&g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR];
                        data[0] = chanNum;        //ͨ����
                        data[1] = TRIGPIN_DIR;    //Trig���
                        for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR]);j++)
                        {
                            data[2 + j] = *pData++;
                        }
                        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELSTATEQ, dataLen, data);
                
                        g_chanCfgBmp[i].bTrigIn = true;
                    }                    
                }
#endif

                g_chanCfgBmp[i].bEncoder = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdEncoderChanNumQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdEncoderTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderTypeVerify(cmdDataLen, pCmdData, (void *)&type))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].encoderType = type;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdEncoderTypeQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdEncoderMultipleSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderMultipleVerify(cmdDataLen, pCmdData, (void *)&encoderMult))
    {
        for (i = 0;i < g_systemState.chanNum;i++)
        {
            if (bConfig[i])
            {
                //��ͨ����������֧��4����
                if ((ECCHAN_1 == g_motorInfo.motor[i].encoderChanNum) && (ENCMULT_QUADR == encoderMult))
                {
                    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = PARA_VERIFY_ERROR_TYPE;
                }
                else
                {
                    g_motorInfo.motor[i].encoderMult = encoderMult;

                    g_chanCfgBmp[i].bEncoder = true;
                    g_bCmdPostSemToFunc = true;
                }
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdEncoderMultipleQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdEncoderStateSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdEncoderStateSet(u8 cmdDataLen, u8 *pCmdData)
{
    IntfcStateEnum encoderState;
    u8 i;
#ifdef PROJECT_GELGOOG
    u8 j;
    u8 dataLen;
    u8 data[6];
#endif
    bool bConfig[CH_TOTAL] = {0};
    u8   chanNum = *pCmdData++;

    
    if (chanNum <= CH_MAX)
    {
        bConfig[chanNum] = true;
    }
    else
    {
        for (i = 0;i < g_systemState.chanNum;i++)
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

    
    //���в�����֤
    if (PARA_VERIFY_NO_ERROR == pvrfEncoderStateVerify(cmdDataLen, pCmdData, (void *)&encoderState))
    {
        for (i = 0;i < ENCODER_SUPPORT_NUM;i++)
        {
            if (bConfig[i])
            {
                g_motorInfo.motor[i].encoderState = encoderState;

#ifdef PROJECT_GELGOOG
#if 1
                //�����޸�ΪCH4��������UartSensor2����
                if ((INTFC_ON == encoderState) && (CH4 == i))
                {
#if 1
                    //��ͨ����������U2_S3��4����
                    //��ͨ����������U2_S3����
                    if (ECCHAN_3 == g_motorInfo.motor[i].encoderChanNum)
                    {
                        for (j = SENSOR_S3;j < SENSOR_RESERVE;j++)
                        {
                            if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][j].state)
                            {
                                g_sensorUart.sensor[UARTNUM_U2][j].state = SENSOR_OFF;

                                //֪ͨ��λ��IO״̬�仯��
                                dataLen = sizeof(g_sensorUart.sensor[UARTNUM_U2][j].state) + sizeof(SenUartNumEnum) + sizeof(SensorNumEnum);
                                data[0] = UARTNUM_U2;    //UARTNUM_U2
                                data[1] = j;             //SENSOR_S1
                                data[2] = g_sensorUart.sensor[UARTNUM_U2][j].state;
                                cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STATEQ, dataLen, data);
                        
                                servSensor2UartReciveOff();    //��ʱ���������ã������ĳ���FUNC�߳��йر�    NICK MARK
                            }
                        }
                    }
                    else
                    {
                        if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][SENSOR_S3].state)
                        {
                            g_sensorUart.sensor[UARTNUM_U2][SENSOR_S3].state = SENSOR_OFF;

                            //֪ͨ��λ��IO״̬�仯��
                            dataLen = sizeof(g_sensorUart.sensor[UARTNUM_U2][SENSOR_S3].state) + sizeof(SenUartNumEnum) + sizeof(SensorNumEnum);
                            data[0] = UARTNUM_U2;    //UARTNUM_U2
                            data[1] = SENSOR_S3;     //SENSOR_S3
                            data[2] = g_sensorUart.sensor[UARTNUM_U2][SENSOR_S3].state;
                            cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STATEQ, dataLen, data);
                    
                            servSensor2UartReciveOff();    //��ʱ���������ã������ĳ���FUNC�߳��йر�    NICK MARK
                        }
                    }
#else
                    for (j = 0;j < SENSOR_RESERVE;j++)
                    {
                        if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][j].state)
                        {
                            g_sensorUart.sensor[UARTNUM_U2][j].state = SENSOR_OFF;

                            //֪ͨ��λ��IO״̬�仯��
                            dataLen = sizeof(g_sensorUart.sensor[UARTNUM_U2][j].state) + sizeof(SenUartNumEnum) + sizeof(SensorNumEnum);
                            data[0] = UARTNUM_U2;    //UARTNUM_U2
                            data[1] = j;             //SENSOR_S1
                            data[2] = g_sensorUart.sensor[UARTNUM_U2][j].state;
                            cmdFrameSend(CMD_SENSORUART, SNUARTCMD_STATEQ, dataLen, data);
                    
                            servSensor2UartReciveOff();    //��ʱ���������ã������ĳ���FUNC�߳��йر�    NICK MARK
                        }
                    }
#endif
                    servSensorUartInfoStore(&g_sensorUart);
                }
                
#else

                if (INTFC_ON == encoderState)
                {
                    //���߱�������TRIG_DIR���ö˿��ˣ�������Ҫ�ر�
                    if ((SENSOR_ON == g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR]) &&
                        (ECCHAN_3 == g_motorInfo.motor[i].encoderChanNum))
                    {
                        g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR] = SENSOR_OFF;
                        servTrigInInfoWrite(&g_trigInInfo);
                        
                        //֪ͨ��λ��IO״̬�仯��
                        dataLen = sizeof(g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR]) + sizeof(chanNum) + sizeof(TRIGPIN_DIR);
                        pData = (u8 *)&g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR];
                        data[0] = chanNum;        //ͨ����
                        data[1] = TRIGPIN_DIR;    //Trig���
                        for (j = 0;j < sizeof(g_trigInInfo.trigIn[i].levelState[TRIGPIN_DIR]);j++)
                        {
                            data[2 + j] = *pData++;
                        }
                        cmdFrameSend(CMD_TRIGIN, TRIGINCMD_LEVELSTATEQ, dataLen, data);
                
                        g_chanCfgBmp[i].bTrigIn = true;
                    }                    
                }
#endif
#endif
                
                g_chanCfgBmp[i].bEncoder = true;
                g_bCmdPostSemToFunc = true;
            }
        }
        
        servMotorInfoWrite(&g_motorInfo);
    }
}


/*********************************************************************************************
�� �� ��: cmdEncoderStateQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        for (i = 0;i < g_systemState.chanNum;i++)
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
�� �� ��: cmdEncoderCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
}

            
/*********************************************************************************************
�� �� ��: cmdEncoderCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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



/*******************************************�ļ�β********************************************/
