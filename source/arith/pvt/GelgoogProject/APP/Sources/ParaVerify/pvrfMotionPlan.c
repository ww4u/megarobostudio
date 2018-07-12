/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvrfMotionPlan.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "pvrfMotionPlan.h"



/****************************************�ⲿ��������*****************************************/
extern ParaLimitStruct  g_paraLimit;
extern SystemStateStruct g_systemState;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: pvrfPvtExecModeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtExecModeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(ExecModeEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *pData;
            
            switch (tempValue)
            {
                case 0:
                    *(ExecModeEnum *)pParaValue = EXECMODE_NCYCLE;
                  break;

                case 1:
                    *(ExecModeEnum *)pParaValue = EXECMODE_FIFO;
                  break;      

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtPlanModeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtPlanModeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(PlanModeEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(PlanModeEnum *)pParaValue = PLANMODE_CUBIC;
                  break;

                case 1:
                    *(PlanModeEnum *)pParaValue = PLANMODE_LINEAR;
                  break; 

                case 2:
                    *(PlanModeEnum *)pParaValue = PLANMODE_UNIFORM;
                  break; 

                case 3:
                    *(PlanModeEnum *)pParaValue = PLANMODE_TRAPEZOID;
                  break;  

                case 4:
                    *(PlanModeEnum *)pParaValue = PLANMODE_QUINTIC;
                  break; 

                case 5:
                    *(PlanModeEnum *)pParaValue = PLANMODE_SINE;
                  break;     

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtMotionModeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtMotionModeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(MotionModeEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(MotionModeEnum *)pParaValue = MTNMODE_PVT;
                  break;

                case 1:
                    *(MotionModeEnum *)pParaValue = MTNMODE_LVT_CORR;
                  break; 

                case 2:
                    *(MotionModeEnum *)pParaValue = MTNMODE_LVT_NOCORR;
                  break;     

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtModifyDutyVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtModifyDutyVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(ModifyDutyEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(ModifyDutyEnum *)pParaValue = MDUTY_1000;
                  break;

                case 1:
                    *(ModifyDutyEnum *)pParaValue = MDUTY_500;
                  break; 

                case 2:
                    *(ModifyDutyEnum *)pParaValue = MDUTY_250;
                  break; 

                case 3:
                    *(ModifyDutyEnum *)pParaValue = MDUTY_125;
                  break;    

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtNcyclesVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtNcyclesVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(u32)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *(u32 *)pData;
            if (tempValue <= g_paraLimit.upLimit.pvtNcycles)
            {
                if (tempValue >= g_paraLimit.downLimit.pvtNcycles)
                {
                    *(u32 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtWarnPointVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtWarnPointVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u16 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(u16)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *(u16 *)pData;
            if (tempValue <= g_paraLimit.upLimit.pvtWarnPoint)
            {
                if (tempValue >= g_paraLimit.downLimit.pvtWarnPoint)
                {
                    *(u16 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtEndStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtEndStateVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(EndStateEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(EndStateEnum *)pParaValue = ENDSTATE_STOP;
                  break;
                  
                case 1:
                    *(EndStateEnum *)pParaValue = ENDSTATE_HOLD;
                  break;

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtNcyclesVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtSpeedScaleVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u16 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(u16)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *(u16 *)pData;
            if (tempValue <= g_paraLimit.upLimit.speedScale)
            {
                if (tempValue >= g_paraLimit.downLimit.speedScale)
                {
                    *(u16 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtStopDecelModeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtStopDecelModeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(StopModeEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(StopModeEnum *)pParaValue = STOPMODE_IMMED;
                  break;
                  
                case 1:
                    *(StopModeEnum *)pParaValue = STOPMODE_DIST;
                  break;
                  
                case 2:
                    *(StopModeEnum *)pParaValue = STOPMODE_TIME;
                  break;
                  
                case 3:
                    *(StopModeEnum *)pParaValue = STOPMODE_DIST_TIME;
                  break;

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtStopDecelTimeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtStopDecelTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(f32)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *(f32 *)pData;
            if (tempValue <= g_paraLimit.upLimit.stopTime)
            {
                if (tempValue >= g_paraLimit.downLimit.stopTime)
                {
                    *(f32 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtStopDecelDistanceVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtStopDecelDistanceVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(f32)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *(f32 *)pData;
            if (tempValue <= g_paraLimit.upLimit.stopDistance)
            {
                if (tempValue >= g_paraLimit.downLimit.stopDistance)
                {
                    *(f32 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                }
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfOutOfStepStateVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtOOSStateVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(SensorStateEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(SensorStateEnum *)pParaValue = SENSOR_OFF;
                  break; 

                case 1:
                    *(SensorStateEnum *)pParaValue = SENSOR_ON;
                  break;      

                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtOOSLineOutNumVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtOOSLineOutNumVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(u16)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *(u16 *)pData;
            if (tempValue <= g_systemState.posnConvertInfo[CH1].lineSteps)
            {
                *(u16 *)pParaValue = tempValue;
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtOOSTotalOutNumVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtOOSTotalOutNumVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(u32)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *(u32 *)pData;
            if (tempValue <= g_paraLimit.upLimit.outOfStepNum)
            {
                if (tempValue >= g_paraLimit.downLimit.outOfStepNum)
                {
                    *(u32 *)pParaValue = tempValue;
                }
                else
                {
                    verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                } 
            }
            else
            {
                verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}


/*********************************************************************************************
�� �� ��: pvrfPvtOOSResponseVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtOOSResponseVerify(u8 dataLen, u8 *pData, void *pParaValue, WaveTableTypeEnum *pWaveIndex)
{
    u8 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if ((sizeof(WaveTableTypeEnum) + sizeof(ResponseTypeEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempValue = *pData;
            switch (tempValue)
            {
                case 0:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_NONE;
                  break;
        
                case 1:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_ALARM;
                  break;  
        
                case 2:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_STOP;
                  break; 
        
                case 3:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_ALARMSTOP;
                  break;  
        
                case 4:
                    *(ResponseTypeEnum *)pParaValue = RESPONSE_RUN;
                  break;      
        
                default:
                    verifyResult = PARA_VERIFY_ERROR_TYPE;
                  break;
            }
        }
        else
        {
            verifyResult = PARA_VERIFY_ERROR_INDEX;
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}

#if 0
#endif

/*********************************************************************************************
�� �� ��: pvrfPvtPositionVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtPositionVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex, WaveTableTypeEnum *pWaveIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) + sizeof(u8) + (sizeof(WaveTableTypeEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempIndex = *(u8 *)pData++;
            //if (tempIndex < PVT_POINT_BUFFER_SIZE)
            {
                *pIndex = tempIndex;
                tempValue = *(f32 *)pData;
                if (tempValue <= g_paraLimit.upLimit.pvtPosn)
                {
                    if (tempValue >= g_paraLimit.downLimit.pvtPosn)
                    {
                        *(f32 *)pParaValue = tempValue;
                    }
                    else
                    {
                        verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                    }
                }
                else
                {
                    verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
                }
            }
            /*else
            {
                verifyResult = PARA_VERIFY_ERROR_INDEX;
            }*/
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}



/*********************************************************************************************
�� �� ��: pvrfPvtSpeedVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtSpeedVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex, WaveTableTypeEnum *pWaveIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) + sizeof(u8) + (sizeof(WaveTableTypeEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempIndex = *(u8 *)pData++;
            //if (tempIndex < PVT_POINT_BUFFER_SIZE)
            {
                *pIndex = tempIndex;
                tempValue = *(f32 *)pData;
                if (tempValue <= g_paraLimit.upLimit.pvtSpeed)
                {
                    if (tempValue >= g_paraLimit.downLimit.pvtSpeed)
                    {
                        *(f32 *)pParaValue = tempValue;
                    }
                    else
                    {
                        verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                    }
                }
                else
                {
                    verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
                }
            }
            /*else
            {
                verifyResult = PARA_VERIFY_ERROR_INDEX;
            }*/
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}



/*********************************************************************************************
�� �� ��: pvrfPvtTimeVerify;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 pvrfPvtTimeVerify(u8 dataLen, u8 *pData, void *pParaValue, u8 *pIndex, WaveTableTypeEnum *pWaveIndex)
{
    u8  tempIndex;
    f32 tempValue;
    u8 verifyResult = PARA_VERIFY_NO_ERROR;

    
    if (sizeof(f32) + sizeof(u8) + (sizeof(WaveTableTypeEnum)) == dataLen)    //������Ҫ��ȷ
    {
        tempValue = *pData++;
        if (tempValue < WTTYPE_RESERVE)
        {
            *pWaveIndex = (WaveTableTypeEnum)tempValue;
            
            tempIndex = *(u8 *)pData++;
            //if (tempIndex < PVT_POINT_BUFFER_SIZE)
            {
                *pIndex = tempIndex;
                tempValue = *(f32 *)pData;
                if (tempValue <= g_paraLimit.upLimit.pvtTime)
                {
                    if (tempValue >= g_paraLimit.downLimit.pvtTime)
                    {
                        *(f32 *)pParaValue = tempValue;
                    }
                    else
                    {
                        verifyResult = PARA_VERIFY_LESS_LOWER_LIMIT;
                    }
                }
                else
                {
                    verifyResult = PARA_VERIFY_GREAT_UPER_LIMIT;
                }
            }
            /*else
            {
                verifyResult = PARA_VERIFY_ERROR_INDEX;
            }*/
        }
    }
    else
    {
        verifyResult = PARA_VERIFY_ERROR_LEN;
    }

    g_systemState.errorCode[ERROR_CODE_INDEX_PARA_VERIFY] = verifyResult;

    return verifyResult;
}



/*******************************************�ļ�β********************************************/
