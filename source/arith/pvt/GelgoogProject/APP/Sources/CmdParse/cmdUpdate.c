/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdUpdate.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.12.15;
��ʷ�汾:  ��;
      �������̣�
     boot , app �� fpga����
1. ��λ������start�����λ����������������ж��Ƿ�Ҫ��������boot,��Ӧ��
2. ��λ�����͵�ַ���������λ���жϵ�ַ�Ϸ��ԣ��������ַ����Ӧ��
3. ���ǻ����Ϳ�ʼ����(start)����,��λ�����仺��,����������ǰ��׼����,��Ӧ��.
4. ��λ�����Ͳ�����erase�����������Ӧ��flash,��Ӧ��
5. ��λ��ѭ�������������ݣ���λ���������ݣ���Ӧ��
6. ��λ�������������ݽ��������λ������������������Ӧ��
7. ��λ��������ת�����boot��ת��app. ��Ӧ��
*********************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "cmdUpdate.h"
#include "cmdMainParse.h"
#include "servSystemPara.h"
#include "servSoftTimer.h"
#include "bspFlash.h"
#include "bspTimer.h"
#include "comMemoryMap.h"



/****************************************�ⲿ��������*****************************************/
extern SystemStateStruct g_systemState;
extern BootInfoStruct    g_bootInfo;



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pUpdateCmdFunc[UPDATECMD_RESERVE];

u8 * g_update_buf = NULL;
u32  g_flash_position = 0;
u16  g_buf_offset = 0;



/******************************************����ʵ��*******************************************/
//��Flash��д������
static int updateWriteFlash(u8* data,u16 len)
{
    u32 u32left = 0;
    if(g_update_buf == NULL)
    {
      return -2;
    }
    if(g_buf_offset + len >= FLASH_PAGE_SIZE)//��������������д��flash
    {
        u32left = FLASH_PAGE_SIZE - g_buf_offset;
        memcpy(&g_update_buf[g_buf_offset],data,u32left);
        //ֻ����һҳ�Ż�д��
        bspFlashWrite(g_flash_position,g_update_buf,FLASH_PAGE_SIZE);//д��Flash
        bspDelayMs(20);
       
        memcpy(&g_update_buf[0],&data[u32left],len - u32left);//��data��ʣ������ݻ�������
        g_buf_offset = len - u32left;//�����������ݳ���
        g_flash_position += FLASH_PAGE_SIZE;//�ƶ�λ��
    }
    else //�������һ���
    {
        memcpy(&g_update_buf[g_buf_offset],data,len);
        g_buf_offset += len;
    }
    return 0;
}
//����������������,ȫ��д��Flash
static void updateFlushData()
{
    if(g_buf_offset == 0 )
    {
      return;
    }
    if((g_buf_offset&0x01))//��˫�ֽڶ��룬��0
    {
      g_update_buf[g_buf_offset] = 0;
      g_buf_offset += 1;
    }
    bspFlashWrite(g_flash_position,g_update_buf,g_buf_offset);

    g_flash_position += g_buf_offset;//�ƶ�λ��
    bspDelayMs(20);//�����ʱ�Ǳ�Ҫ��,��Flashд��ʱ��
}


/*********************************************************************************************
�� �� ��: cmdUpdateStart;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateStart(u8 cmdDataLen, u8 *pCmdData)
{   
    if(pCmdData[0] != UPDATE_TYPE_BOOT)
    {
        g_bootInfo.u8BootFlag = 0x01;//��λ������־
        servBootInfoWrite(&g_bootInfo);//������EEPROM��
        //����ݲ�����
        bspSoftReset();
    }
    else//���������boot������ת
    {
        g_flash_position = 0x08000000;
        g_buf_offset = 0;
        if(g_update_buf == NULL)
        {
            g_update_buf = (u8*)malloc(2048);
            if(g_update_buf == NULL)
            {
                return;
            }
        }
    }
    cmdFrameSend(CMD_UPDATE, UPDATECMD_START, 0, NULL);

}


/*********************************************************************************************
�� �� ��: cmdUpdateErase
ʵ�ֹ���: ����flash
�������: �ܹ�5���ֽ�,��һ���ֽڱ�ʾ����,��������4���ֽ��ǲ����ĳ���
�������: ��;
�� �� ֵ: ��;
˵    ��: 
*********************************************************************************************/
void cmdUpdateErase(u8 cmdDataLen, u8 *pCmdData)
{    
    u8  type = 0;
    
    
    type = pCmdData[0];

    if(type != UPDATE_TYPE_BOOT)//��APP��,��ʱֻ������boot
    {
        return;
    }
    //memcpy(&len,&pCmdData[1],4);
#ifdef PROJECT_GELGOOG
    bspFlashSectorErase(FLASH_BOOT_CODE_ADDR, 0x08007FFF);
#endif
    cmdFrameSend(CMD_UPDATE, UPDATECMD_ERASE, 0, NULL);
}

/*********************************************************************************************
�� �� ��: cmdUpdateData;
ʵ�ֹ���: ������λ�����͵���������,�ں��ʵ�ʱ��д��flash
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateData(u8 cmdDataLen, u8 *pCmdData)
{    
    if(updateWriteFlash(pCmdData,cmdDataLen) != 0)
    {
        return;
    }
    cmdFrameSend(CMD_UPDATE, UPDATECMD_DATA, 0, NULL);
}


/*********************************************************************************************
�� �� ��: cmdUpdateJump;
ʵ�ֹ���: ��ת,ʵ��������ϵͳ 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateJump(u8 cmdDataLen, u8 *pCmdData)
{    
    //bspSoftReset();//����
}


/*********************************************************************************************
�� �� ��: cmdUpdateAppAddrSet;
ʵ�ֹ���: ����App��flash�е�λ��
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ����û�в�����֤
*********************************************************************************************/
void cmdUpdateAppAddrSet(u8 cmdDataLen, u8 *pCmdData)
{    
    if(cmdDataLen != 4 )
    {
        return;
    }
    memcpy((void*)&g_bootInfo.u32AppBaseAddr,pCmdData,4);
    servBootInfoWrite(&g_bootInfo);
    cmdFrameSend(CMD_UPDATE, UPDATECMD_APPADDR, 0, NULL);
}
/*********************************************************************************************
�� �� ��: cmdUpdateAppAddrQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateAppAddrQuery(u8 cmdDataLen, u8 *pCmdData)
{    
    u8 dataLen;
    u8 *pData;

    dataLen = sizeof(g_bootInfo.u32AppBaseAddr);
    pData = (u8 *)&g_bootInfo.u32AppBaseAddr;
    cmdFrameSend(CMD_UPDATE, UPDATECMD_APPADDRQ, dataLen, pData);
}

/*********************************************************************************************
�� �� ��: cmdUpdateEnd;
ʵ�ֹ���: ��������,���������е�����д��flash
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateEnd(u8 cmdDataLen, u8 *pCmdData)
{    
    updateFlushData();
    if(g_update_buf)
    {
        free(g_update_buf);
    }
    cmdFrameSend(CMD_UPDATE, UPDATECMD_END, 0, NULL);
}


/*********************************************************************************************
�� �� ��: cmdUpdateFpgaAddrSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateFpgaAddrSet(u8 cmdDataLen, u8 *pCmdData)
{    
    if(cmdDataLen != 4 )
    {
        return;
    }
    memcpy((void*)&g_bootInfo.u32FpgaBaseAddr,pCmdData,4);
    servBootInfoWrite(&g_bootInfo);
    cmdFrameSend(CMD_UPDATE, UPDATECMD_FPGAADDR, 0, NULL);
    
}

/*********************************************************************************************
�� �� ��: cmdUpdateFpgaAddrQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateFpgaAddrQuery(u8 cmdDataLen, u8 *pCmdData)
{    
    u8 dataLen;
    u8 *pData;

    dataLen = sizeof(g_bootInfo.u32FpgaBaseAddr);
    pData = (u8 *)&g_bootInfo.u32FpgaBaseAddr;
    cmdFrameSend(CMD_UPDATE, UPDATECMD_FPGAADDRQ, dataLen, pData);
}
/*********************************************************************************************
�� �� ��: cmdUpdateCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateCmdInit(void)
{
    memset(pUpdateCmdFunc, 0, sizeof(pUpdateCmdFunc));

    pUpdateCmdFunc[UPDATECMD_START]       = cmdUpdateStart;
    pUpdateCmdFunc[UPDATECMD_ERASE]       = cmdUpdateErase;
    pUpdateCmdFunc[UPDATECMD_DATA]        = cmdUpdateData;
    pUpdateCmdFunc[UPDATECMD_END]         = cmdUpdateEnd;
    pUpdateCmdFunc[UPDATECMD_JUMP]        = cmdUpdateJump;
    pUpdateCmdFunc[UPDATECMD_APPADDR]     = cmdUpdateAppAddrSet;
    pUpdateCmdFunc[UPDATECMD_FPGAADDR]    = cmdUpdateFpgaAddrSet;
    pUpdateCmdFunc[UPDATECMD_APPADDRQ]     = cmdUpdateAppAddrQuery;
    pUpdateCmdFunc[UPDATECMD_FPGAADDRQ]    = cmdUpdateFpgaAddrQuery;
}

            
/*********************************************************************************************
�� �� ��: cmdUpdateCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdUpdateCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < UPDATECMD_RESERVE) && (pUpdateCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pUpdateCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************�ļ�β********************************************/
