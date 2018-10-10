/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  pvtProcTask.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.02.07;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include <os.h>
#include <math.h>
#include "project.h"
#include "pvtProcTask.h"
#include "pvtAlgorithm.h"
#include "servSoftTimer.h"
#include "servFPGA.h"
#include "cmdSensor.h"



/****************************************�ⲿ��������*****************************************/
extern OS_SEM g_semPvtCalcTask;
extern OS_SEM g_semFunctionTask;
extern OS_SEM g_semEventManageTask;

extern PlanInfoStruct    g_planInfo;

extern OutputDataStruct  g_outputData[CH_TOTAL];
extern MotorInfoStruct   g_motorInfo; 
extern SystemInfoStruct  g_systemInfo;
extern SystemStateStruct g_systemState;
extern WaveTableStruct   g_waveTable[CH_TOTAL][WTTYPE_RESERVE];
extern ReportInfoStruct  g_reportInfo;
extern DebugInfoStruct   g_debugInfo;

extern EventSrcBmpStruct  g_eventSrcBmp;
extern SystemCfgBmpStruct g_systemCfgBmp;
extern ChanCfgBmpStruct   g_chanCfgBmp[CH_TOTAL];

extern SoftTimerStruct  g_reportTimer[CH_TOTAL][REPTTYPE_RESERVE];

extern DebugInfoStruct  g_debugInfo;



/*****************************************�ֲ��궨��******************************************/
#define    DATA_TYPE_ACTIVE_FLAG          0x80000000           //���������Ч��ʶ
#define    DATA_TYPE_LEFT_SHIFT           27                   //�������������������λ��
#define    WAIT_TIME_HIGH_RIGHT_SHIFT     24                   //�ȴ�ʱ���24λ����λ��
#define    DATA_WAIT_TIME_MASK            0x00FFFFFF           //�ȴ�ʱ����������
#define    DATA_WAIT_TIME_HIGH_MASK       0xA8000000           //�ȴ�ʱ���24bit����
#define    DATA_TYPE_WAIT_LOW             4                    //�������͵ȴ�ʱ���24bit
#define    DATA_TYPE_WAIT_HIGH            5                    //�������͵ȴ�ʱ���24bit
#define    SEND_DATA_LEN_MIN              4                    //�������ݳ��ȵ���Сֵ��128bit����ӦΪ4��32bit

#define    OUTPUT_DATA_BYTES_SIZE         8192    //����Buffer���ֽڳ��ȣ�������һ��u32�ķ��͵�ַ���ֽ�����(1023 + 1) * 4
#define    OUTPUT_DATA_BLOCK_SIZE         208     //����Buffer�Ŀ鳤�ȣ���log2(OUTPUT_DATA_BYTES_SIZE)���������������4λ
#define    OUTPUT_EMPTY_BYTES_SIZE        16      //��������ݵ��ֽڳ��ȣ�������һ��u32�ķ��͵�ַ���ֽ�����(3 + 1) * 4
#define    OUTPUT_EMPTY_BLOCK_SIZE        64      //��������ݵĿ鳤�ȣ���log2(OUTPUT_EMPTY_BYTES_SIZE)���������������4λ
#define    OUTPUT_BLOCK_SIZE_LEFT_SHIFT   4       //����4λ����Ϊ���ݿ�������SDIO_DCTRL�Ĵ�����[4:7]bit

#define    INCHING_SPEED_LINE      20.0f     //˿���˶�  �ٶ�Ϊ 30mm/s
#define    INCHING_SPEED_ROTARY    5.0f      //��ת�˶�  �ٶ�Ϊ  5��/s  



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
PvtCalcStruct pvtCalcData[CH_TOTAL];
bool bPvtCalc[CH_TOTAL];                //״̬�л�



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: OutpBufferFill;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: �������������4Bytes��������SDIO���ʹ�2^n��Bytes;
*********************************************************************************************/
void OutpBufferFill(u8 chanNum, OutpDataTypeEnum datatype, u32 outpData, BufferOperateEnum bufferOperate, void *pContext)
{
    OS_ERR  os_err;
    bool bRewind = false;
    u32 data;
    u32 i;
    u8  remainder;
    u16 toSendPoints;
#if 0
    u16 threshold;
#endif
    u32 tailSize;
    WaveTableStruct *pcalcWaveTable = &g_waveTable[chanNum][g_systemState.calcWaveTable[chanNum]];
 

    if (BUFFOPERT_SYNCSEND == bufferOperate)    //�޸�ĩβ��
    {
        //����ǰ�ж��²���ʣ��ռ��Ƿ�
        if (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints)
        {
            if (WTWORKMODE_FIFO == pcalcWaveTable->waveWorkMode)
            {
                do
                {
                    //��ѯʣ��ռ䣬��ʵ�ǲ�ѯ���ж��ٲ������û�������Ȼ�����ܵĲ����С��ȥû������ĵ���
                    pcalcWaveTable->writableSize = servFpgaWaveWritableSizeRead(chanNum, pcalcWaveTable->totalSize);

                    //����ʱ���ͷ������CPUʹ��Ȩ
                    servSoftDelayMs(1);
                    
                }while (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints);    //ֱ����д�ռ���ڵ���Ҫ���͵ĵ���
            }
            else
            {
                //ֹͣ���㣬����ERROR״̬    TO ADD NICK MARK
                pcalcWaveTable->waveState = MTSTATE_ERROR;

                return;
            }
        }

        tailSize = pcalcWaveTable->totalSize + pcalcWaveTable->startAddr - pcalcWaveTable->writableAddr;
        if (tailSize < g_outputData[chanNum].noSendPoints)    //�ռ乻����������ͷβ��������ɵģ���Ҫ��һ�����ݷִη���
        {
            //����ֱ�Ӱ�һ֡���ͣ�������Ч����ֻ��tailSize��ô�࣬ʣ�������������֡����
            bRewind = true;
            g_outputData[chanNum].validPoints = tailSize;
            g_outputData[chanNum].validBytes = tailSize * sizeof(g_outputData[chanNum].data[0]) 
                                               + sizeof(g_outputData[chanNum].writeAddr);
        }
        else
        {
            g_outputData[chanNum].validPoints = g_outputData[chanNum].noSendPoints;
            g_outputData[chanNum].validBytes = g_outputData[chanNum].noSendPoints * sizeof(g_outputData[chanNum].data[0]) 
                                               + sizeof(g_outputData[chanNum].writeAddr);
        }
        
        g_outputData[chanNum].totalPoints += g_outputData[chanNum].validPoints;
    
        g_outputData[chanNum].toSendBlkSize = OUTPUT_DATA_BLOCK_SIZE;
        g_outputData[chanNum].toSendPoints  = OUTPUT_DATA_BUFFER_SIZE;
        g_outputData[chanNum].toSendBytes   = OUTPUT_DATA_BYTES_SIZE;
        
        g_outputData[chanNum].noSendPoints -= g_outputData[chanNum].validPoints;
        
        g_chanCfgBmp[chanNum].bWaveTableData = true;
        OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);

        if (bRewind)
        {
            //��Ϊ֮ǰ�в������ݱ�������Ч���ݷ����ˣ�������Ҫ���ⲿ���������·�����
            //���·��͵ķ�ʽ���ǰ��ⲿ�����ݷ���Bufferͷ���������һ���µ�����
            for (i = 0;i < g_outputData[chanNum].noSendPoints;i++)
            {
                g_outputData[chanNum].data[i] = g_outputData[chanNum].data[g_outputData[chanNum].validPoints + i];
            }

            g_outputData[chanNum].validPoints = g_outputData[chanNum].noSendPoints;
            g_outputData[chanNum].validBytes = g_outputData[chanNum].noSendPoints * sizeof(g_outputData[chanNum].data[0]) 
                                               + sizeof(g_outputData[chanNum].writeAddr);
            
            g_outputData[chanNum].totalPoints += g_outputData[chanNum].validPoints;
        
            g_outputData[chanNum].toSendBlkSize = OUTPUT_DATA_BLOCK_SIZE;
            g_outputData[chanNum].toSendPoints  = OUTPUT_DATA_BUFFER_SIZE;
            g_outputData[chanNum].toSendBytes   = OUTPUT_DATA_BYTES_SIZE;
            
            g_outputData[chanNum].noSendPoints  = 0;
            
            g_chanCfgBmp[chanNum].bWaveTableData = true;
            OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);
        }
    }
    else if (BUFFOPERT_FORCESEND == bufferOperate)    //ǿ�Ʒ��͵Ļ��϶������һ֡
    {       
        g_outputData[chanNum].totalPoints += g_outputData[chanNum].noSendPoints;
        toSendPoints = g_outputData[chanNum].noSendPoints;
        remainder = g_outputData[chanNum].totalPoints % SEND_DATA_LEN_MIN;

        //��һ����ͨ�������һ�η��Ͳ����ݣ��Ѳ����ܵĵ�������SEND_DATA_LEN_MIN(4)��������
        if (remainder != 0)
        {
            toSendPoints += SEND_DATA_LEN_MIN - remainder; 
        }

        //���겹�����ݳ��Ⱥ����жϲ���ռ��Ƿ��㹻
        if (pcalcWaveTable->writableSize < toSendPoints)
        {
            if (WTWORKMODE_FIFO == pcalcWaveTable->waveWorkMode)
            {
                do
                {
                    //��ѯʣ��ռ䣬��ʵ�ǲ�ѯ���ж��ٲ������û�������Ȼ�����ܵĲ����С��ȥû������ĵ���
                    pcalcWaveTable->writableSize = servFpgaWaveWritableSizeRead(chanNum, pcalcWaveTable->totalSize);
                    
                    //����ʱ���ͷ������CPUʹ��Ȩ
                    servSoftDelayMs(1);
                    
                }while (pcalcWaveTable->writableSize < toSendPoints);    //ֱ����д�ռ���ڵ���Ҫ���͵ĵ���
            }
            else
            {
                //ֹͣ���㣬����ERROR״̬    TO ADD NICK MARK
                pcalcWaveTable->waveState = MTSTATE_ERROR;

                return;
            }
        }

        tailSize = pcalcWaveTable->totalSize + pcalcWaveTable->startAddr - pcalcWaveTable->writableAddr;
        if (tailSize < toSendPoints)    //�ռ乻����������ͷβ��������ɵģ���Ҫ��һ�����ݷִη���
        {
            //�ȷ�һ֡����Ч������β���ռ��С
            for (i = 0;i < (OUTPUT_DATA_BUFFER_SIZE - g_outputData[chanNum].noSendPoints);i++)
            {
                g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints + i] = 0;    //0����Ч����
            }

            //����ֱ�Ӱ�һ֡���ͣ�������Ч����ֻ��tailSize��ô�࣬ʣ�������������֡����
            g_outputData[chanNum].validPoints = tailSize;
            g_outputData[chanNum].validBytes  = tailSize * sizeof(g_outputData[chanNum].data[0]) 
                                                + sizeof(g_outputData[chanNum].writeAddr);            
            g_outputData[chanNum].toSendPoints  = OUTPUT_DATA_BUFFER_SIZE;
            g_outputData[chanNum].toSendBytes   = OUTPUT_DATA_BYTES_SIZE;
            g_outputData[chanNum].toSendBlkSize = OUTPUT_DATA_BLOCK_SIZE;
            
            g_outputData[chanNum].totalPoints += g_outputData[chanNum].validPoints;
            
            g_chanCfgBmp[chanNum].bWaveTableData = true;
            OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);

            if (tailSize < g_outputData[chanNum].noSendPoints)    //��һ������Ч����δ����
            {
                g_outputData[chanNum].noSendPoints -= tailSize;
                for (i = 0;i < g_outputData[chanNum].noSendPoints;i++)
                {
                    g_outputData[chanNum].data[i] = g_outputData[chanNum].data[tailSize + i];
                }
            }
            toSendPoints -= tailSize;  
        }

        //��һ�������������ĳ����������Buffer Size��Ҫ�����η�
        if (toSendPoints > OUTPUT_DATA_BUFFER_SIZE)
        {
            //һ�β����ֻ��Ҫ��3���������ݣ���ô��һ֡���ʱ��Ҳ����2���������
            //�����µ�һֻ֡��Ҫ����3�������2^n

            g_outputData[chanNum].bSendEmpty = true;
            g_outputData[chanNum].validEmpPoints = toSendPoints - OUTPUT_DATA_BUFFER_SIZE;
            g_outputData[chanNum].validEmpBytes = g_outputData[chanNum].validEmpPoints * sizeof(g_outputData[chanNum].data[0]) 
                                                  + sizeof(g_outputData[chanNum].emptyDataAddr);
            g_outputData[chanNum].emptyPoints  = OUTPUT_EMPTY_BUFFER_SIZE;
            g_outputData[chanNum].emptyBlkSize = OUTPUT_EMPTY_BLOCK_SIZE;
            g_outputData[chanNum].emptyBytes   = OUTPUT_EMPTY_BYTES_SIZE;

            memset(g_outputData[chanNum].emptyData, 0, sizeof(g_outputData[chanNum].emptyData));
            
            toSendPoints = OUTPUT_DATA_BUFFER_SIZE;
        }

        g_outputData[chanNum].validPoints = toSendPoints;
        g_outputData[chanNum].validBytes = toSendPoints * sizeof(g_outputData[chanNum].data[0]) 
                                           + sizeof(g_outputData[chanNum].writeAddr);

        //�ڶ����ǰ����һ�η��͵����ݵ�������2^n
#if 1
        //ֱ�Ӱ�����������
        toSendPoints = OUTPUT_DATA_BUFFER_SIZE;
        g_outputData[chanNum].toSendBlkSize = (OUTPUT_DATA_BLOCK_SIZE >> OUTPUT_BLOCK_SIZE_LEFT_SHIFT);
#else
        //����2^n�ķ���:���ж��Ƿ���2^n��ֻ��Ҫ�ж����䣬����512��ֱ�Ӳ���1024������256����512����������
        threshold = OUTPUT_DATA_BUFFER_SIZE + 1;
        g_outputData.toSendBlkSize = (OUTPUT_DATA_BLOCK_SIZE >> OUTPUT_BLOCK_SIZE_LEFT_SHIFT);
        while (threshold > 1)
        {
            //�Ƿ������ֵ��һ�룬����һ��Ͳ���ȫ��
            if ((toSendPoints + 1) > (threshold / 2))    //Ҫ����writeAddr
            {
                toSendPoints = threshold - 1;
                break;
            }
            else
            {
                //��ʣ�µ�һ���
                threshold /= 2;
                g_outputData.toSendBlkSize--;
            }
        }
#endif

        //��������ļ���������Ч����
        for (i = 0;i < (toSendPoints - g_outputData[chanNum].noSendPoints);i++)
        {
            g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints + i] = 0;    //0����Ч����
        }

#if 1   //Ϊ�˼���ֹͣ���ٶȱ�����ִ�У�Ҫ�󲨱����һ���㲻������Ч��
        //�����ٲ�����Ч�����Ҫ�����һ��������һ����Ч�����λ��
        //����������ĵ��ǵȴ�ʱ��ʱ����Ϊ��Ҫ��֤�ȴ�ʱ��ĵ�24bit�͸�24bit��һ��
        //�Ͳ��ܽ���λ�õ���
        data  = g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1];
        data &= ~DATA_WAIT_TIME_MASK;
        if ((remainder != 0) && (DATA_WAIT_TIME_HIGH_MASK != data))
#else
        if (remainder != 0)
#endif
        {
            //�����һ��ʵ����Чֵ�Ͳ������һ����Ч���ݻ���λ��
            if (g_outputData[chanNum].bSendEmpty)
            {
                //�±�Ҫ��һ
                g_outputData[chanNum].emptyData[g_outputData[chanNum].validEmpPoints - 1] = g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1];
                g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1] = 0;
            }
            else
            {
                //�±�Ҫ��һ
                g_outputData[chanNum].data[g_outputData[chanNum].validPoints - 1] = g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1];    
                g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1] = 0;
            }
        }
        
        g_outputData[chanNum].toSendBlkSize <<= OUTPUT_BLOCK_SIZE_LEFT_SHIFT;    //����4λ����Ϊ���ݿ�������SDIO_DCTRL�Ĵ�����[4:7]bit
        g_outputData[chanNum].toSendPoints = toSendPoints;
        g_outputData[chanNum].toSendBytes = toSendPoints * sizeof(g_outputData[chanNum].data[0]) 
                                            + sizeof(g_outputData[chanNum].writeAddr);    //Ҫ����writeAddr
        
        g_outputData[chanNum].noSendPoints = 0;

        g_outputData[chanNum].bLastFrame = true;
        
        g_chanCfgBmp[chanNum].bWaveTableData = true;
        OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);
    }
    //��ǿ�Ʒ���ʱ��һ�η���2^n - 1��32bits���ټ���FUNC�Ǳ߼���һ��32bits�ĵ�ֵַ���ͱ������SDIO��һ�η���2^n��32bits
    else
    {
        //��Buffer��д����
        if ((OUTPDATA_FORWARD == datatype) || (OUTPDATA_REVERSE == datatype))
        {
            data = (datatype << DATA_TYPE_LEFT_SHIFT) | (outpData & DATA_PERIOD_MASK) | DATA_TYPE_ACTIVE_FLAG;

            //ͳ������С����ֵ
            if (g_outputData[chanNum].minPeriod > outpData)
            {
                g_outputData[chanNum].minPeriod = outpData;
            }
        }
        else    //�ȴ�ʱ��Ϊ48bit
        {
            //��24bit
            data = (DATA_TYPE_WAIT_LOW  << DATA_TYPE_LEFT_SHIFT) | 
                   (outpData & DATA_WAIT_TIME_MASK) | 
                   DATA_TYPE_ACTIVE_FLAG;
            g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints] = data;
            g_outputData[chanNum].noSendPoints++;

            //�ж���Buffer�Ƿ��пռ�    CJ 2018.01.18 Add
            if (g_outputData[chanNum].noSendPoints >= OUTPUT_DATA_BUFFER_SIZE)
            {
                //����ǰ�ж��²���ʣ��ռ��Ƿ�
                if (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints)
                {
                    if (WTWORKMODE_FIFO == pcalcWaveTable->waveWorkMode)
                    {
                        do
                        {
                            //��ѯʣ��ռ䣬��ʵ�ǲ�ѯ���ж��ٲ������û�������Ȼ�����ܵĲ����С��ȥû������ĵ���
                            pcalcWaveTable->writableSize = servFpgaWaveWritableSizeRead(chanNum, pcalcWaveTable->totalSize);

                            //����ʱ���ͷ������CPUʹ��Ȩ
                            servSoftDelayMs(1);
                            
                        }while (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints);    //ֱ����д�ռ���ڵ���Ҫ���͵ĵ���
                    }
                    else
                    {
                        //ֹͣ���㣬����ERROR״̬    TO ADD NICK MARK
                        pcalcWaveTable->waveState = MTSTATE_ERROR;

                        return;
                    }
                }

                tailSize = pcalcWaveTable->totalSize + pcalcWaveTable->startAddr - pcalcWaveTable->writableAddr;
                if (tailSize < g_outputData[chanNum].noSendPoints)    //�ռ乻����������ͷβ��������ɵģ���Ҫ��һ�����ݷִη���
                {
                    //����ֱ�Ӱ�һ֡���ͣ�������Ч����ֻ��tailSize��ô�࣬ʣ�������������֡����
                    bRewind = true;
                    g_outputData[chanNum].validPoints = tailSize;
                    g_outputData[chanNum].validBytes = tailSize * sizeof(g_outputData[chanNum].data[0]) 
                                                       + sizeof(g_outputData[chanNum].writeAddr);
                }
                else
                {
                    g_outputData[chanNum].validPoints = g_outputData[chanNum].noSendPoints;
                    g_outputData[chanNum].validBytes = OUTPUT_DATA_BYTES_SIZE;
                }
                
                g_outputData[chanNum].totalPoints += g_outputData[chanNum].validPoints;
            
                g_outputData[chanNum].toSendBlkSize = OUTPUT_DATA_BLOCK_SIZE;
                g_outputData[chanNum].toSendPoints  = g_outputData[chanNum].noSendPoints;
                g_outputData[chanNum].toSendBytes   = OUTPUT_DATA_BYTES_SIZE;
                
                g_outputData[chanNum].noSendPoints  = g_outputData[chanNum].toSendPoints - g_outputData[chanNum].validPoints;
                
                g_chanCfgBmp[chanNum].bWaveTableData = true;
                OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);

                if (bRewind)
                {
                    //��Ϊ֮ǰ�в������ݱ�������Ч���ݷ����ˣ�������Ҫ���ⲿ���������·�����
                    //���·��͵ķ�ʽ���ǰ��ⲿ�����ݷ���Bufferͷ���������һ���µ�����
                    for (i = 0;i < g_outputData[chanNum].noSendPoints;i++)
                    {
                        g_outputData[chanNum].data[i] = g_outputData[chanNum].data[g_outputData[chanNum].validPoints + i];
                    }

                    /*g_outputData[chanNum].validPoints = g_outputData[chanNum].noSendPoints;
                    g_outputData[chanNum].validBytes = OUTPUT_DATA_BYTES_SIZE;
                    
                    g_outputData[chanNum].totalPoints += g_outputData[chanNum].validPoints;
                
                    g_outputData[chanNum].toSendBlkSize = OUTPUT_DATA_BLOCK_SIZE;
                    g_outputData[chanNum].toSendPoints  = OUTPUT_DATA_BUFFER_SIZE;
                    g_outputData[chanNum].toSendBytes   = OUTPUT_DATA_BYTES_SIZE;
                    
                    g_outputData[chanNum].noSendPoints  = g_outputData[chanNum].toSendPoints - g_outputData[chanNum].validPoints;
                    
                    g_chanCfgBmp[chanNum].bWaveTableData = true;
                    OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);*/
                }
            }    //if (g_outputData[chanNum].noSendPoints >= OUTPUT_DATA_BUFFER_SIZE)
            
            //��24bit
            data = (DATA_TYPE_WAIT_HIGH << DATA_TYPE_LEFT_SHIFT) | 
                   ((outpData >> WAIT_TIME_HIGH_RIGHT_SHIFT) & DATA_WAIT_TIME_MASK) | 
                   DATA_TYPE_ACTIVE_FLAG;
        }
        g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints] = data;
        g_outputData[chanNum].noSendPoints++;

        //�ж����Ƿ���Ҫ֪ͨFUNC�����͸�FPGA
        if (g_outputData[chanNum].noSendPoints >= OUTPUT_DATA_BUFFER_SIZE)
        {
            //����ǰ�ж��²���ʣ��ռ��Ƿ�
            if (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints)
            {
                if (WTWORKMODE_FIFO == pcalcWaveTable->waveWorkMode)
                {
                    do
                    {
                        //��ѯʣ��ռ䣬��ʵ�ǲ�ѯ���ж��ٲ������û�������Ȼ�����ܵĲ����С��ȥû������ĵ���
                        pcalcWaveTable->writableSize = servFpgaWaveWritableSizeRead(chanNum, pcalcWaveTable->totalSize);

                        //����ʱ���ͷ������CPUʹ��Ȩ
                        servSoftDelayMs(1);
                        
                    }while (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints);    //ֱ����д�ռ���ڵ���Ҫ���͵ĵ���
                }
                else
                {
                    //ֹͣ���㣬����ERROR״̬    TO ADD NICK MARK
                    pcalcWaveTable->waveState = MTSTATE_ERROR;
                }
            }

            tailSize = pcalcWaveTable->totalSize + pcalcWaveTable->startAddr - pcalcWaveTable->writableAddr;
            if (tailSize < g_outputData[chanNum].noSendPoints)    //�ռ乻����������ͷβ��������ɵģ���Ҫ��һ�����ݷִη���
            {
                //����ֱ�Ӱ�һ֡���ͣ�������Ч����ֻ��tailSize��ô�࣬ʣ�������������֡����
                bRewind = true;
                g_outputData[chanNum].validPoints = tailSize;
                g_outputData[chanNum].validBytes = tailSize * sizeof(g_outputData[chanNum].data[0]) 
                                                   + sizeof(g_outputData[chanNum].writeAddr);
            }
            else
            {
                g_outputData[chanNum].validPoints = g_outputData[chanNum].noSendPoints;
                g_outputData[chanNum].validBytes = OUTPUT_DATA_BYTES_SIZE;
            }
            
            g_outputData[chanNum].totalPoints += g_outputData[chanNum].validPoints;
        
            g_outputData[chanNum].toSendBlkSize = OUTPUT_DATA_BLOCK_SIZE;
            g_outputData[chanNum].toSendPoints  = g_outputData[chanNum].noSendPoints;
            g_outputData[chanNum].toSendBytes   = OUTPUT_DATA_BYTES_SIZE;
            
            g_outputData[chanNum].noSendPoints  = g_outputData[chanNum].toSendPoints - g_outputData[chanNum].validPoints;
            
            g_chanCfgBmp[chanNum].bWaveTableData = true;
            OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);

            if (bRewind)
            {
                //��Ϊ֮ǰ�в������ݱ�������Ч���ݷ����ˣ�������Ҫ���ⲿ���������·�����
                //���·��͵ķ�ʽ���ǰ��ⲿ�����ݷ���Bufferͷ���������һ���µ�����
                for (i = 0;i < g_outputData[chanNum].noSendPoints;i++)
                {
                    g_outputData[chanNum].data[i] = g_outputData[chanNum].data[g_outputData[chanNum].validPoints + i];
                }
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: PvtInfoGet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void PvtInfoGet(void)
{
    u8 i;
    WaveTableTypeEnum  waveTable;
    WaveWorkModeEnum   waveWorkMode;
    WaveTableStruct   *pcalcWaveTable;


    for (i = 0;i < CH_TOTAL;i++)
    {
        waveTable = g_systemState.calcQueue[i][g_systemState.calcIndex[i]];

        if (waveTable < WTTYPE_RESERVE)
        {
            g_systemState.calcWaveTable[i] = waveTable;
            pcalcWaveTable = &g_waveTable[i][waveTable];
        
            if (g_systemState.pvtManage[i][waveTable].tailPoint >= PVT_CALC_NEED_POINT_NUM)    //����������ż���
            {
                //PVT���ݸ��º��һ�λ�ȡ
                if (g_systemState.pvtManage[i][waveTable].bClear)
                {
                    g_systemState.pvtManage[i][waveTable].bClear = false;
                    
                    memset(&g_outputData[i], 0, sizeof(g_outputData[i]));
                    g_outputData[i].writeAddr     = SERV_FPGA_CONFIG_REG_26;
                    g_outputData[i].emptyDataAddr = SERV_FPGA_CONFIG_REG_26;
                    g_outputData[i].minPeriod = FPGA_PWM_CLOCK;

                    memset(&pvtCalcData[i], 0, sizeof(pvtCalcData[i]));
                
                    pvtCalcData[i].fpgaClockOffset = g_systemInfo.fpgaClockOffset;
                    
                    pvtCalcData[i].fpgaPwmClock = FPGA_PWM_CLOCK;
                    pvtCalcData[i].pvtPlanMode  = g_planInfo.plan[i][waveTable].planMode;
                    pvtCalcData[i].motionMode   = g_planInfo.plan[i][waveTable].motionMode;
                    pvtCalcData[i].pvtExecMode  = g_planInfo.plan[i][waveTable].execMode;
                    
                    pvtCalcData[i].outpBufferFill  = OutpBufferFill;
                    
                    if (MTNMODE_PVT == pvtCalcData[i].motionMode)
                    {
                        pvtCalcData[i].posnToStep = g_systemState.posnConvertInfo[i].posnToStep;
                        
                        pvtCalcData[i].maxOffset = PVT_TARGET_REAL_OFFSET_P;
                        pvtCalcData[i].minOffset = PVT_TARGET_REAL_OFFSET_N; 
                        
                        pvtCalcData[i].lineStepsInv = 1.0f; 
                        pvtCalcData[i].lineSteps    = 1.0f;
                    }
                    else
                    {
                        pvtCalcData[i].posnToStep = g_systemState.posnConvertInfo[i].posnToLine;
                        
                        pvtCalcData[i].maxOffset = LVT_TARGET_REAL_OFFSET_P;
                        pvtCalcData[i].minOffset = LVT_TARGET_REAL_OFFSET_N;  
                        
                        pvtCalcData[i].lineStepsInv = 1.0f / g_systemState.posnConvertInfo[i].lineSteps;
                        pvtCalcData[i].lineSteps    = g_systemState.posnConvertInfo[i].lineSteps;
                    }

                    //��ʼ��Ҫд��Ĳ���(����ģʽ���˶�ģʽ��ѭ�������Ƿ��������)
                    if (EXECMODE_FIFO != pvtCalcData[i].pvtExecMode)
                    {
                        waveWorkMode = WTWORKMODE_CYCLE;
                    }
                    else
                    {
                        waveWorkMode = WTWORKMODE_FIFO;
                    }

                    //��ʼ����Ӧ����
                    servFpgaWaveTableInit(pcalcWaveTable, waveWorkMode);
                }
                
                pvtCalcData[i].startPoint = g_systemState.pvtManage[i][waveTable].pvtPoint[pvtCalcData[i].lastPoint];
                if ((pvtCalcData[i].lastPoint + 1) >= PVT_POINT_BUFFER_SIZE)
                {
                    pvtCalcData[i].endPoint = g_systemState.pvtManage[i][waveTable].pvtPoint[0];
                }
                else
                {
                    pvtCalcData[i].endPoint = g_systemState.pvtManage[i][waveTable].pvtPoint[pvtCalcData[i].lastPoint + 1];
                }
            
                pvtCalcData[i].lastPoint++;
                if (pvtCalcData[i].lastPoint >= PVT_POINT_BUFFER_SIZE)
                {
                    pvtCalcData[i].lastPoint = 0;
                }
                
                g_systemState.pvtManage[i][waveTable].tailPoint--;

                bPvtCalc[i] = true;
                
            }    //if (g_planInfo.pvtInfo[i].tailPoint >= PVT_CALC_NEED_POINT_NUM)
        }    //if (g_systemState.calcWaveTable[i] < WTTYPE_RESERVE)
    }
}


/*********************************************************************************************
�� �� ��: PvtCalcTask;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void PvtCalcTask(void)
{
    OS_ERR  os_err;
    u32  motionTime;
    u8   i;
    u8   errorCode;
    

    //���ж�
    CPU_IntDis();
    
    //��ȡ����
    PvtInfoGet();

    //���ж�
    CPU_IntEn();


    for (i = 0;i < CH_TOTAL;i++)
    {
        if (bPvtCalc[i])
        {
            /*//���������״̬
            if ((MTSTATE_IDLE    == g_waveTable[i][g_systemState.calcWaveTable[i]].waveState) &&
                (MTSTATE_POWERON != g_waveTable[i][g_systemState.calcWaveTable[i]].waveState))
            {
                g_systemState.bStateSwitch[i][g_systemState.calcWaveTable[i]] = true;
            }*/

            if (1)    //��ʱ���    NICK MARK
            {
                //����PVT
                errorCode = pvtSegmentCalc(&pvtCalcData[i], i);
                if (errorCode)
                {
                    g_systemState.eventCode[0] = i;
                    g_systemState.eventCode[1] = ERROR_CODE_INDEX_PVT_CALC;
                    g_systemState.eventCode[2] = errorCode;                    
                }

                //For PVT Debug
                g_debugInfo.calcPvtCount[i]++;

                if (pvtCalcData[i].bQueryReady)
                {  
                    pvtCalcData[i].bQueryReady = false;

                    //����С����ֵ��ͳ�Ƶ�΢������¼����
                    g_waveTable[i][g_systemState.calcWaveTable[i]].stopConfigInfo.minPeriod = g_outputData[i].minPeriod;

                    if (WTWORKMODE_CYCLE == g_waveTable[i][g_systemState.calcWaveTable[i]].waveWorkMode)
                    {
                        g_systemState.calcQueue[i][g_systemState.calcIndex[i]] = WTTYPE_RESERVE;

                        //��������е�һ�����л�����һ��
                        g_systemState.calcIndex[i]++;
                        if (g_systemState.calcIndex[i] >= PVT_CALC_QUEUE_SIZE)
                        {
                            g_systemState.calcIndex[i] = 0;
                        }

                        //�����һ������Ч��
                        if (WTTYPE_RESERVE == g_systemState.calcQueue[i][g_systemState.calcIndex[i]])
                        {
                            //��ֹ����ʱ���µļ��뵽������
                            CPU_IntDis();
                            g_systemState.calcIndex[i] = 0;
                            g_systemState.tailIndex[i] = 0;
                            CPU_IntEn();
                        }
                        else
                        {
                            g_systemState.calcWaveTable[i] = g_systemState.calcQueue[i][g_systemState.calcIndex[i]];
                            g_systemState.pvtManage[i][g_systemState.calcWaveTable[i]].bClear = true;
                        }
                    }

                    //������Ϊ��ʼ����ʱ����ź�������ļ������ѯReady״̬
                    if (0 == g_systemState.pvtCalcTaskSem)
                    {  
                        //ѭ��ģʽ��ѭ�����ϱ�����������Ҫ��ѯѭ����
                        if ((WTWORKMODE_CYCLE == g_waveTable[i][g_systemState.calcWaveTable[i]].waveWorkMode) &&
                            (SENSOR_ON == g_reportInfo.report[i].state[REPTTYPE_CYCLE]))
                        {
                            g_waveTable[i][g_systemState.calcWaveTable[i]].bCycleQuery = true;
                            
                            motionTime = (u32)(pvtCalcData[i].startPoint.time * UNIT_S_TO_MS / 5);    //ȡһ��ѭ��ʱ���5��֮1��Ϊ��ʱ����ѯʱ��
                            servStimerInit(&g_reportTimer[i][REPTTYPE_CYCLE], motionTime, cmdReportCycleTimerCB);
                        }
                        else
                        {
                            g_waveTable[i][g_systemState.calcWaveTable[i]].bCycleQuery = false;
                        }
                        
                        g_chanCfgBmp[i].bQryReady = true;
                        OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);

                        //For PVT Debug
                        g_debugInfo.calcTime[i] = pvtCalcData[i].timeCount;
                    }
                }
            }
            else
            {
                //������ʱ������ʱ��FPGA��ȡ���ݲ��ϱ�
            }

            bPvtCalc[i] = false;
        }
    }
                
    /*if (g_systemState.pvtCalcTaskSem > 0)
    {
        g_systemState.pvtCalcTaskSem--;
        if (0 == g_systemState.pvtCalcTaskSem)    //�������ʼ�������е�PVT��ϵͳ״̬��ΪIDLE
        {
            for (i = 0;i < CH_TOTAL;i++)
            {
                g_systemState.calcIndex[i] = 0;
                g_systemState.tailIndex[i] = 0;
            }
            OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, &os_err);
        }
    }*/
}


/*********************************************************************************************
�� �� ��: PvtProcTask;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void PvtProcTask(void *p_arg)
{
    OS_ERR  os_err;

    (void)p_arg;

    while (1)
    {
        OSSemPend(&g_semPvtCalcTask, 0, OS_OPT_PEND_BLOCKING, NULL, &os_err);

#if 1    //FOR DEBUG
        //servFpgaDigitalOutLevelSet(DIO_DO1, LEVEL_HIGH);
#endif

        PvtCalcTask();

#if 1    //FOR DEBUG
        //servFpgaDigitalOutLevelSet(DIO_DO1, LEVEL_LOW);
#endif

    }
}



/*******************************************�ļ�β********************************************/
