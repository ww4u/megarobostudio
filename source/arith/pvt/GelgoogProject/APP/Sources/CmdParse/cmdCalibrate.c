/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdCalibrate.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include "cmdCalibrate.h"
#include "cmdMainParse.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pCalibrateCmdFunc[CALBRCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdCalibrateBackLash;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCalibrateBackLash(u8 cmdDataLen, u8 *pCmdData)
{    

}

/*********************************************************************************************
�� �� ��: cmdCalibrateBackLashEnd;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCalibrateBackLashEnd(u8 cmdDataLen, u8 *pCmdData)
{    

}


/*********************************************************************************************
�� �� ��: cmdCalibrateMechError;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCalibrateMechError(u8 cmdDataLen, u8 *pCmdData)
{    

}

/*********************************************************************************************
�� �� ��: cmdCalibrateMechErrorEnd;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCalibrateMechErrorEnd(u8 cmdDataLen, u8 *pCmdData)
{    

}


/*********************************************************************************************
�� �� ��: cmdCalibrateCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCalibrateCmdInit(void)
{
    memset(pCalibrateCmdFunc, 0, sizeof(pCalibrateCmdFunc));

    pCalibrateCmdFunc[CALBRCMD_BACKLASH]  = cmdCalibrateBackLash;
    pCalibrateCmdFunc[CALBRCMD_BLEND]     = cmdCalibrateBackLashEnd;
    pCalibrateCmdFunc[CALBRCMD_MECHERROR] = cmdCalibrateMechError;
    pCalibrateCmdFunc[CALBRCMD_MEEND]     = cmdCalibrateMechErrorEnd;
}

            
/*********************************************************************************************
�� �� ��: cmdCalibrateCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdCalibrateCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < CALBRCMD_RESERVE) && (pCalibrateCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pCalibrateCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************�ļ�β********************************************/