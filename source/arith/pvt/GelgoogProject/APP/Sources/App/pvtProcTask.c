/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  pvtProcTask.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.02.07;
历史版本:  无;
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



/****************************************外部变量声明*****************************************/
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



/*****************************************局部宏定义******************************************/
#define    DATA_TYPE_ACTIVE_FLAG          0x80000000           //输出数据有效标识
#define    DATA_TYPE_LEFT_SHIFT           27                   //输出数据数据类型左移位数
#define    WAIT_TIME_HIGH_RIGHT_SHIFT     24                   //等待时间高24位右移位数
#define    DATA_WAIT_TIME_MASK            0x00FFFFFF           //等待时间数据掩码
#define    DATA_WAIT_TIME_HIGH_MASK       0xA8000000           //等待时间高24bit掩码
#define    DATA_TYPE_WAIT_LOW             4                    //数据类型等待时间低24bit
#define    DATA_TYPE_WAIT_HIGH            5                    //数据类型等待时间高24bit
#define    SEND_DATA_LEN_MIN              4                    //发送数据长度的最小值，128bit，对应为4个32bit

#define    OUTPUT_DATA_BYTES_SIZE         8192    //波表Buffer的字节长度，包含了一个u32的发送地址的字节数，(1023 + 1) * 4
#define    OUTPUT_DATA_BLOCK_SIZE         208     //波表Buffer的块长度，由log2(OUTPUT_DATA_BYTES_SIZE)计算出来，再左移4位
#define    OUTPUT_EMPTY_BYTES_SIZE        16      //波表空数据的字节长度，包含了一个u32的发送地址的字节数，(3 + 1) * 4
#define    OUTPUT_EMPTY_BLOCK_SIZE        64      //波表空数据的块长度，由log2(OUTPUT_EMPTY_BYTES_SIZE)计算出来，再左移4位
#define    OUTPUT_BLOCK_SIZE_LEFT_SHIFT   4       //左移4位是因为数据块设置在SDIO_DCTRL寄存器的[4:7]bit

#define    INCHING_SPEED_LINE      20.0f     //丝杠运动  速度为 30mm/s
#define    INCHING_SPEED_ROTARY    5.0f      //旋转运动  速度为  5度/s  



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
PvtCalcStruct pvtCalcData[CH_TOTAL];
bool bPvtCalc[CH_TOTAL];                //状态切换



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: OutpBufferFill;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 波表点数总数凑4Bytes整数倍，SDIO发送凑2^n个Bytes;
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
 

    if (BUFFOPERT_SYNCSEND == bufferOperate)    //修改末尾点
    {
        //发送前判断下波表剩余空间是否够
        if (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints)
        {
            if (WTWORKMODE_FIFO == pcalcWaveTable->waveWorkMode)
            {
                do
                {
                    //查询剩余空间，其实是查询还有多少波表点数没有输出，然后用总的波表大小减去没有输出的点数
                    pcalcWaveTable->writableSize = servFpgaWaveWritableSizeRead(chanNum, pcalcWaveTable->totalSize);

                    //软延时，释放任务的CPU使用权
                    servSoftDelayMs(1);
                    
                }while (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints);    //直到可写空间大于等于要发送的点数
            }
            else
            {
                //停止计算，进入ERROR状态    TO ADD NICK MARK
                pcalcWaveTable->waveState = MTSTATE_ERROR;

                return;
            }
        }

        tailSize = pcalcWaveTable->totalSize + pcalcWaveTable->startAddr - pcalcWaveTable->writableAddr;
        if (tailSize < g_outputData[chanNum].noSendPoints)    //空间够，但是是由头尾两部分组成的，需要把一包数据分次发送
        {
            //数据直接按一帧发送，但是有效数据只有tailSize这么多，剩余的数据重新组帧发送
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
            //因为之前有部分数据被当成无效数据发送了，所以需要把这部分数据重新发送下
            //重新发送的方式就是把这部分数据放在Buffer头部，再组成一次新的数据
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
    else if (BUFFOPERT_FORCESEND == bufferOperate)    //强制发送的话肯定是最后一帧
    {       
        g_outputData[chanNum].totalPoints += g_outputData[chanNum].noSendPoints;
        toSendPoints = g_outputData[chanNum].noSendPoints;
        remainder = g_outputData[chanNum].totalPoints % SEND_DATA_LEN_MIN;

        //第一步是通过给最后一次发送补数据，把波表总的点数补成SEND_DATA_LEN_MIN(4)的整数倍
        if (remainder != 0)
        {
            toSendPoints += SEND_DATA_LEN_MIN - remainder; 
        }

        //算完补的数据长度后，先判断波表空间是否足够
        if (pcalcWaveTable->writableSize < toSendPoints)
        {
            if (WTWORKMODE_FIFO == pcalcWaveTable->waveWorkMode)
            {
                do
                {
                    //查询剩余空间，其实是查询还有多少波表点数没有输出，然后用总的波表大小减去没有输出的点数
                    pcalcWaveTable->writableSize = servFpgaWaveWritableSizeRead(chanNum, pcalcWaveTable->totalSize);
                    
                    //软延时，释放任务的CPU使用权
                    servSoftDelayMs(1);
                    
                }while (pcalcWaveTable->writableSize < toSendPoints);    //直到可写空间大于等于要发送的点数
            }
            else
            {
                //停止计算，进入ERROR状态    TO ADD NICK MARK
                pcalcWaveTable->waveState = MTSTATE_ERROR;

                return;
            }
        }

        tailSize = pcalcWaveTable->totalSize + pcalcWaveTable->startAddr - pcalcWaveTable->writableAddr;
        if (tailSize < toSendPoints)    //空间够，但是是由头尾两部分组成的，需要把一包数据分次发送
        {
            //先发一帧，有效长度是尾部空间大小
            for (i = 0;i < (OUTPUT_DATA_BUFFER_SIZE - g_outputData[chanNum].noSendPoints);i++)
            {
                g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints + i] = 0;    //0是无效数据
            }

            //数据直接按一帧发送，但是有效数据只有tailSize这么多，剩余的数据重新组帧发送
            g_outputData[chanNum].validPoints = tailSize;
            g_outputData[chanNum].validBytes  = tailSize * sizeof(g_outputData[chanNum].data[0]) 
                                                + sizeof(g_outputData[chanNum].writeAddr);            
            g_outputData[chanNum].toSendPoints  = OUTPUT_DATA_BUFFER_SIZE;
            g_outputData[chanNum].toSendBytes   = OUTPUT_DATA_BYTES_SIZE;
            g_outputData[chanNum].toSendBlkSize = OUTPUT_DATA_BLOCK_SIZE;
            
            g_outputData[chanNum].totalPoints += g_outputData[chanNum].validPoints;
            
            g_chanCfgBmp[chanNum].bWaveTableData = true;
            OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);

            if (tailSize < g_outputData[chanNum].noSendPoints)    //有一部分有效数据未发送
            {
                g_outputData[chanNum].noSendPoints -= tailSize;
                for (i = 0;i < g_outputData[chanNum].noSendPoints;i++)
                {
                    g_outputData[chanNum].data[i] = g_outputData[chanNum].data[tailSize + i];
                }
            }
            toSendPoints -= tailSize;  
        }

        //上一步补完整数倍的长度如果超了Buffer Size就要分两次发
        if (toSendPoints > OUTPUT_DATA_BUFFER_SIZE)
        {
            //一次补最多只需要补3个波表数据，那么比一帧多的时候也最多多2个波表点数
            //所以新的一帧只需要凑满3个点就是2^n

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

        //第二步是把最后一次发送的数据点数补成2^n
#if 1
        //直接按最大点数发送
        toSendPoints = OUTPUT_DATA_BUFFER_SIZE;
        g_outputData[chanNum].toSendBlkSize = (OUTPUT_DATA_BLOCK_SIZE >> OUTPUT_BLOCK_SIZE_LEFT_SHIFT);
#else
        //补成2^n的方法:不判断是否是2^n，只需要判断区间，大于512则直接补成1024，大于256补成512，依次类推
        threshold = OUTPUT_DATA_BUFFER_SIZE + 1;
        g_outputData.toSendBlkSize = (OUTPUT_DATA_BLOCK_SIZE >> OUTPUT_BLOCK_SIZE_LEFT_SHIFT);
        while (threshold > 1)
        {
            //是否大于阈值的一半，大于一半就补成全部
            if ((toSendPoints + 1) > (threshold / 2))    //要算上writeAddr
            {
                toSendPoints = threshold - 1;
                break;
            }
            else
            {
                //和剩下的一半比
                threshold /= 2;
                g_outputData.toSendBlkSize--;
            }
        }
#endif

        //将多出来的几个补成无效数据
        for (i = 0;i < (toSendPoints - g_outputData[chanNum].noSendPoints);i++)
        {
            g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints + i] = 0;    //0是无效数据
        }

#if 1   //为了减速停止和速度保存能执行，要求波表最后一个点不能是无效点
        //所以再补了无效点后需要把最后一个点和最后一个有效点调换位置
        //但是如果最后的点是等待时间时，因为需要保证等待时间的低24bit和高24bit在一起
        //就不能进行位置调换
        data  = g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1];
        data &= ~DATA_WAIT_TIME_MASK;
        if ((remainder != 0) && (DATA_WAIT_TIME_HIGH_MASK != data))
#else
        if (remainder != 0)
#endif
        {
            //将最后一个实际有效值和补的最后一个无效数据换下位置
            if (g_outputData[chanNum].bSendEmpty)
            {
                //下标要减一
                g_outputData[chanNum].emptyData[g_outputData[chanNum].validEmpPoints - 1] = g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1];
                g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1] = 0;
            }
            else
            {
                //下标要减一
                g_outputData[chanNum].data[g_outputData[chanNum].validPoints - 1] = g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1];    
                g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints - 1] = 0;
            }
        }
        
        g_outputData[chanNum].toSendBlkSize <<= OUTPUT_BLOCK_SIZE_LEFT_SHIFT;    //左移4位是因为数据块设置在SDIO_DCTRL寄存器的[4:7]bit
        g_outputData[chanNum].toSendPoints = toSendPoints;
        g_outputData[chanNum].toSendBytes = toSendPoints * sizeof(g_outputData[chanNum].data[0]) 
                                            + sizeof(g_outputData[chanNum].writeAddr);    //要算上writeAddr
        
        g_outputData[chanNum].noSendPoints = 0;

        g_outputData[chanNum].bLastFrame = true;
        
        g_chanCfgBmp[chanNum].bWaveTableData = true;
        OSSemPost(&g_semFunctionTask, OS_OPT_POST_ALL, &os_err);
    }
    //非强制发送时，一次发送2^n - 1个32bits，再加上FUNC那边加了一个32bits的地址值，就变成了在SDIO上一次发送2^n个32bits
    else
    {
        //向Buffer中写数据
        if ((OUTPDATA_FORWARD == datatype) || (OUTPDATA_REVERSE == datatype))
        {
            data = (datatype << DATA_TYPE_LEFT_SHIFT) | (outpData & DATA_PERIOD_MASK) | DATA_TYPE_ACTIVE_FLAG;

            //统计下最小周期值
            if (g_outputData[chanNum].minPeriod > outpData)
            {
                g_outputData[chanNum].minPeriod = outpData;
            }
        }
        else    //等待时间为48bit
        {
            //低24bit
            data = (DATA_TYPE_WAIT_LOW  << DATA_TYPE_LEFT_SHIFT) | 
                   (outpData & DATA_WAIT_TIME_MASK) | 
                   DATA_TYPE_ACTIVE_FLAG;
            g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints] = data;
            g_outputData[chanNum].noSendPoints++;

            //判断下Buffer是否还有空间    CJ 2018.01.18 Add
            if (g_outputData[chanNum].noSendPoints >= OUTPUT_DATA_BUFFER_SIZE)
            {
                //发送前判断下波表剩余空间是否够
                if (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints)
                {
                    if (WTWORKMODE_FIFO == pcalcWaveTable->waveWorkMode)
                    {
                        do
                        {
                            //查询剩余空间，其实是查询还有多少波表点数没有输出，然后用总的波表大小减去没有输出的点数
                            pcalcWaveTable->writableSize = servFpgaWaveWritableSizeRead(chanNum, pcalcWaveTable->totalSize);

                            //软延时，释放任务的CPU使用权
                            servSoftDelayMs(1);
                            
                        }while (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints);    //直到可写空间大于等于要发送的点数
                    }
                    else
                    {
                        //停止计算，进入ERROR状态    TO ADD NICK MARK
                        pcalcWaveTable->waveState = MTSTATE_ERROR;

                        return;
                    }
                }

                tailSize = pcalcWaveTable->totalSize + pcalcWaveTable->startAddr - pcalcWaveTable->writableAddr;
                if (tailSize < g_outputData[chanNum].noSendPoints)    //空间够，但是是由头尾两部分组成的，需要把一包数据分次发送
                {
                    //数据直接按一帧发送，但是有效数据只有tailSize这么多，剩余的数据重新组帧发送
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
                    //因为之前有部分数据被当成无效数据发送了，所以需要把这部分数据重新发送下
                    //重新发送的方式就是把这部分数据放在Buffer头部，再组成一次新的数据
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
            
            //高24bit
            data = (DATA_TYPE_WAIT_HIGH << DATA_TYPE_LEFT_SHIFT) | 
                   ((outpData >> WAIT_TIME_HIGH_RIGHT_SHIFT) & DATA_WAIT_TIME_MASK) | 
                   DATA_TYPE_ACTIVE_FLAG;
        }
        g_outputData[chanNum].data[g_outputData[chanNum].noSendPoints] = data;
        g_outputData[chanNum].noSendPoints++;

        //判断下是否需要通知FUNC任务发送给FPGA
        if (g_outputData[chanNum].noSendPoints >= OUTPUT_DATA_BUFFER_SIZE)
        {
            //发送前判断下波表剩余空间是否够
            if (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints)
            {
                if (WTWORKMODE_FIFO == pcalcWaveTable->waveWorkMode)
                {
                    do
                    {
                        //查询剩余空间，其实是查询还有多少波表点数没有输出，然后用总的波表大小减去没有输出的点数
                        pcalcWaveTable->writableSize = servFpgaWaveWritableSizeRead(chanNum, pcalcWaveTable->totalSize);

                        //软延时，释放任务的CPU使用权
                        servSoftDelayMs(1);
                        
                    }while (pcalcWaveTable->writableSize < g_outputData[chanNum].noSendPoints);    //直到可写空间大于等于要发送的点数
                }
                else
                {
                    //停止计算，进入ERROR状态    TO ADD NICK MARK
                    pcalcWaveTable->waveState = MTSTATE_ERROR;
                }
            }

            tailSize = pcalcWaveTable->totalSize + pcalcWaveTable->startAddr - pcalcWaveTable->writableAddr;
            if (tailSize < g_outputData[chanNum].noSendPoints)    //空间够，但是是由头尾两部分组成的，需要把一包数据分次发送
            {
                //数据直接按一帧发送，但是有效数据只有tailSize这么多，剩余的数据重新组帧发送
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
                //因为之前有部分数据被当成无效数据发送了，所以需要把这部分数据重新发送下
                //重新发送的方式就是把这部分数据放在Buffer头部，再组成一次新的数据
                for (i = 0;i < g_outputData[chanNum].noSendPoints;i++)
                {
                    g_outputData[chanNum].data[i] = g_outputData[chanNum].data[g_outputData[chanNum].validPoints + i];
                }
            }
        }
    }
}


/*********************************************************************************************
函 数 名: PvtInfoGet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
        
            if (g_systemState.pvtManage[i][waveTable].tailPoint >= PVT_CALC_NEED_POINT_NUM)    //至少两个点才计算
            {
                //PVT数据更新后第一次获取
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

                    //初始化要写入的波表(工作模式、运动模式、循环数，是否立即输出)
                    if (EXECMODE_FIFO != pvtCalcData[i].pvtExecMode)
                    {
                        waveWorkMode = WTWORKMODE_CYCLE;
                    }
                    else
                    {
                        waveWorkMode = WTWORKMODE_FIFO;
                    }

                    //初始化相应波表
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
函 数 名: PvtCalcTask;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void PvtCalcTask(void)
{
    OS_ERR  os_err;
    u32  motionTime;
    u8   i;
    u8   errorCode;
    

    //关中断
    CPU_IntDis();
    
    //获取数据
    PvtInfoGet();

    //开中断
    CPU_IntEn();


    for (i = 0;i < CH_TOTAL;i++)
    {
        if (bPvtCalc[i])
        {
            /*//进入计算中状态
            if ((MTSTATE_IDLE    == g_waveTable[i][g_systemState.calcWaveTable[i]].waveState) &&
                (MTSTATE_POWERON != g_waveTable[i][g_systemState.calcWaveTable[i]].waveState))
            {
                g_systemState.bStateSwitch[i][g_systemState.calcWaveTable[i]] = true;
            }*/

            if (1)    //临时设计    NICK MARK
            {
                //计算PVT
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

                    //将最小周期值和统计的微步数记录下来
                    g_waveTable[i][g_systemState.calcWaveTable[i]].stopConfigInfo.minPeriod = g_outputData[i].minPeriod;

                    if (WTWORKMODE_CYCLE == g_waveTable[i][g_systemState.calcWaveTable[i]].waveWorkMode)
                    {
                        g_systemState.calcQueue[i][g_systemState.calcIndex[i]] = WTTYPE_RESERVE;

                        //算完队列中的一个，切换到下一个
                        g_systemState.calcIndex[i]++;
                        if (g_systemState.calcIndex[i] >= PVT_CALC_QUEUE_SIZE)
                        {
                            g_systemState.calcIndex[i] = 0;
                        }

                        //如果下一个是无效的
                        if (WTTYPE_RESERVE == g_systemState.calcQueue[i][g_systemState.calcIndex[i]])
                        {
                            //防止清零时有新的加入到队列中
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

                    //不是因为初始化的时候的信号量引起的计算则查询Ready状态
                    if (0 == g_systemState.pvtCalcTaskSem)
                    {  
                        //循环模式且循环数上报开启，则需要查询循环数
                        if ((WTWORKMODE_CYCLE == g_waveTable[i][g_systemState.calcWaveTable[i]].waveWorkMode) &&
                            (SENSOR_ON == g_reportInfo.report[i].state[REPTTYPE_CYCLE]))
                        {
                            g_waveTable[i][g_systemState.calcWaveTable[i]].bCycleQuery = true;
                            
                            motionTime = (u32)(pvtCalcData[i].startPoint.time * UNIT_S_TO_MS / 5);    //取一次循环时间的5分之1作为定时器查询时间
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
                //开启定时器，定时从FPGA读取数据并上报
            }

            bPvtCalc[i] = false;
        }
    }
                
    /*if (g_systemState.pvtCalcTaskSem > 0)
    {
        g_systemState.pvtCalcTaskSem--;
        if (0 == g_systemState.pvtCalcTaskSem)    //计算完初始化过程中的PVT后将系统状态置为IDLE
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
函 数 名: PvtProcTask;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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



/*******************************************文件尾********************************************/
