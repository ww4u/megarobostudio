/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servFpga.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  无;
完成日期:  无;
历史版本:  无;
*********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "project.h"
#include "servFPGA.h"
#include "servSoftTimer.h"
#include "bspSpi.h"
#include "bspSdio.h"
#include "bspGpio.h"
#include "bspTimer.h"
#include "bspFlash.h"
#include "comMemoryMap.h"
#include "servCanBuildFrame.h"

#if GELGOOG_SINANJU
#include <os.h>
#include "servCommIntfc.h"
#endif



/****************************************外部变量声明*****************************************/
extern MotionInfoStruct  g_motionInfo;

#if GELGOOG_SINANJU
extern OS_SEM          g_semCmdParseTask;
extern CommIntfcStruct g_commIntfc;
#endif



/*****************************************局部宏定义******************************************/
#define    FPGA_ADD_LEN_2BYTE      2       //配置寄存器时 数据前加的地址长度为 2字节
#define    FPGA_REG_LEN_BYTE       2       //寄存器长度

#define    FPGA_RREG_ADDR_MARK     0x1F    //读寄存器地址掩码 bit[0:4]
#define    FPGA_RCHAN_NUM_MARK     0x1E0   //读通道号掩码 bit[5:8]
#define    FPGA_RCHAN_NUM_LSHIFT   5       //读通道号掩码 bit[5:8]

#define    FPGA_WREG_ADDR_MARK     0x3F    //写寄存器地址掩码 bit[0:5]
#define    FPGA_WCHAN_NUM_MARK     0x0F    //写通道号掩码 bit[8:11]，第二个Byte

#define    FPGA_CFG_BUFF_SIZE      2048

#define    FPGA_ARM_COUNT_ENCODER  1       //编码器计数由ARM计数

#define    FPGA_DDR_WAVE_TABLE_POINT_LEN    4    //Bytes

#define    FPGA_SDIO_WRITE_USE_LOCAL    0    // 使用局部变量发送

#define    FPGA_PDM_MSTEP_COUNT_LEN      8
#define    FPGA_PDM_MSTEP_BUFFER_RESV    2

#define    FPGA_SDIO_RX_COUNT_MAX    5



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
FpgaWriteableRegValueStruct fpgaRegValue = {0};

#if !FPGA_SDIO_WRITE_USE_LOCAL
u8 sdioWriteBuffer[4] = {0};
#endif



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servFpgaDataLenSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaDataLenSet(FpgaActionEnum fpgaAction, u16 realLen, u16 dataLen)
{
    if (FPGAACT_WRITE == fpgaAction)
    {
        if (realLen != dataLen)
        {
            bspSdioCmdSend(CMD_FPGA_W_D_2_LEN, dataLen - 2, false);    //减2和减1都是FPGA接口要求
            bspSdioCmdSend(CMD_FPGA_W_D_3_LEN, realLen - 1, false);
        }
        else
        {
            bspSdioCmdSend(CMD_FPGA_W_D_LEN, dataLen - 2, false);
        }
    }
    else if (FPGAACT_READ == fpgaAction)
    {
        bspSdioCmdSend(CMD_FPGA_R_D_LEN, dataLen - 2, false);
    }   
}


/*********************************************************************************************
函 数 名: servFpgaReadAddrSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
static void servFpgaReadAddrSet(u16 readAddr)
{
    bspSdioCmdSend(CMD_FPGA_REG_ADDR, readAddr, true);
}


/*********************************************************************************************
函 数 名: servFpgaRegisterWrite
实现功能: 写Fpga寄存器
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
u8 servFpgaRegisterWrite(u8 chanNum, u16 regAddr, u16 regValue)
{
    u8 txState = 0;
    u8 regLen = FPGA_ADD_LEN_2BYTE + FPGA_REG_LEN_BYTE;
#if FPGA_SDIO_WRITE_USE_LOCAL
    u8 regData[4];
#endif
    

    //设置数据长度
    servFpgaDataLenSet(FPGAACT_WRITE, regLen, regLen);

#if FPGA_SDIO_WRITE_USE_LOCAL    
    regData[0] = regAddr & FPGA_WREG_ADDR_MARK;
    regData[1] = chanNum & FPGA_WCHAN_NUM_MARK;

    if (regData[1] > 0x0F)
    {
        regData[1] = chanNum;
    }
    
    regData[2] = (u8)(regValue & 0xFF);    //低8位
    regData[3] = (u8)((regValue & 0xFF00) >> 8);    //高8位

    //发送数据
    txState = bspSdioDataSend(regData, regLen, SDIO_BLK_4_Byte);
    
#else

    sdioWriteBuffer[0] = regAddr & FPGA_WREG_ADDR_MARK;
    sdioWriteBuffer[1] = chanNum & FPGA_WCHAN_NUM_MARK;

    if (sdioWriteBuffer[1] > 0x0F)
    {
        sdioWriteBuffer[1] = chanNum;
    }
    
    sdioWriteBuffer[2] = (u8)(regValue & 0xFF);    //低8位
    sdioWriteBuffer[3] = (u8)((regValue & 0xFF00) >> 8);    //高8位

    //发送数据
    txState = bspSdioDataSend(sdioWriteBuffer, regLen, SDIO_BLK_4_Byte);
#endif

    return txState;
}


/*********************************************************************************************
函 数 名: servFpgaRegisterRead
实现功能: 读Fpga寄存器
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
u8 servFpgaRegisterRead(u8 chanNum, u16 regAddr, u16 *pRegValue)
{
    u8  regLen = FPGA_ADD_LEN_2BYTE + FPGA_REG_LEN_BYTE;
    u8  regValue[4] = {0};
    u8  rxState = 0;
    u16 tempReg;
    u32 timeout = 33600;    //根据DELAY_COUNT_MS = 33600，此超时时间大约为1ms;

    u8   readChan;
    u16  readRegAddr;
    u16  readReg;
    u8   readCount = FPGA_SDIO_RX_COUNT_MAX;
    bool readRight = false;


    if (pRegValue != NULL)
    {
        //读得不对则多读几次
        while (!readRight && (readCount > 0))
        {
            //启动SDIO接收DMA
            bspSdioDataReceiveSet(regValue, regLen, SDIO_BLK_4_Byte);
            
            //设置数据长度
            servFpgaDataLenSet(FPGAACT_READ, regLen, regLen);

            //设置读取的逻辑寄存器地址
            tempReg = (regAddr & FPGA_RREG_ADDR_MARK) | ((chanNum << FPGA_RCHAN_NUM_LSHIFT) & FPGA_RCHAN_NUM_MARK);
            servFpgaReadAddrSet(tempReg);

            //做成超时等待，同时考虑释放CPU使用权
            while ((!bspSdioDataTxComplete()) && (timeout > 0))
            {
                timeout--;  
            }

            readCount--;
        
            //判断通道号和寄存器地址是否一致
            //readRegAddr = regValue[0] & FPGA_WREG_ADDR_MARK;
            //readChan    = regValue[1] & FPGA_WCHAN_NUM_MARK;
            readReg = ((u16)regValue[1] << 8) + regValue[0];
            readRegAddr = readReg & FPGA_RREG_ADDR_MARK;
            readChan    = (readReg & FPGA_RCHAN_NUM_MARK) >> FPGA_RCHAN_NUM_LSHIFT;

            if ((readChan == chanNum) && (readRegAddr == regAddr))
            {
                readRight = true;
        
                //前两个字节是地址
                *pRegValue  = regValue[2];         //低8位 
                *pRegValue |= regValue[3] << 8;    //高8位 

            }    //if ((readChan == chanNum) && (readRegAddr == regAddr))
        }    //while (!readRight && (readCount > 0))

        if (!readRight)
        {
            rxState = 1;    //Rx失败
        }
    }
  
    return rxState;
}


/*********************************************************************************************
函 数 名: servFpgaProgStart;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 配置启动是先由ARM在CONFIG管脚发出一个下降沿， 
          FPGA在收到下降沿后会将ANSWER管脚拉低作为应答，
          然后ARM可以进行数据下发, 在实际应用时可以不必
          等待PC3的应答信号,稍作延时后即可开始数据下发
*********************************************************************************************/
static bool servFpgaProgStart(void)
{
    u8 i = 0;

    
    //制造一个下降沿
    bspGpioFpgaProgConfigSet(Bit_SET);
    bspDelayUs(10);
    bspGpioFpgaProgConfigSet(Bit_RESET);
    bspDelayUs(10);    
    bspGpioFpgaProgConfigSet(Bit_SET);

    //等待FPGA回复
    while (bspGpioFpgaProgAnswerRead())
    {
        i++;
        bspDelayUs(50);
        
        if (i > 5)
        {
            return false;
        } 
    }
    
    return true;
}


/*********************************************************************************************
函 数 名: servFpgaProgDone;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
static bool servFpgaProgDone(void)
{
    u8 i = 0;
    
    while (i < 50)
    {
        if (!bspGpioFpgaProgDoneRead())
        {
            bspDelayUs(200);
        }
        else
        {
            return true;
        }
        
        i++;
    }
    
    return false;
}


/*********************************************************************************************
函 数 名: servFpgaLoaclFileWrite
实现功能: 将本地文件写入到FPGA中
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
u8 servFpgaLoaclFileWrite(u32 writeAddr, char const* filePath)
{
    u8 *buff = NULL;
    s32 fileSize; 
    u32 writeLen;
    u8  sizeLen = sizeof(fileSize);

    
    FILE * pfile = fopen((char const*)filePath, "rb");
    bspDelayUs(20);
    if (pfile == NULL)
    {
        fclose(pfile);      //关闭文件
        
        return 1;
    }
    
    fseek(pfile, 0L, SEEK_END);    //定位到文件末尾
    fileSize = ftell(pfile);       //得到文件大小
    fseek(pfile, 0L, SEEK_SET);    //定位到文件头
    
    buff = (u8 *)malloc(FPGA_CFG_BUFF_SIZE);    //申请内存
    if (buff != NULL)
    {
#ifdef PROJECT_GELGOOG
        //擦除FPGA文件所占的空间
        bspFlashSectorErase(FLASH_FPGA_CODE_ADDR, FLASH_FPGA_CODE_ADDR + FLASH_FPGA_CODE_LEN);
#endif

        memcpy(buff, &fileSize, 4);    //前4个字节为逻辑文件长度
        fread(buff + sizeLen, 1, FPGA_CFG_BUFF_SIZE - sizeLen, pfile);    //读取数据,逻辑数据肯定大于2048字节
        
        bspFlashWrite(writeAddr, buff, FPGA_CFG_BUFF_SIZE);    
        fileSize -= FPGA_CFG_BUFF_SIZE - sizeLen;
        writeAddr += FPGA_CFG_BUFF_SIZE; 
            
        do
        {
            if (fileSize >= FPGA_CFG_BUFF_SIZE)
            {
                 writeLen = FPGA_CFG_BUFF_SIZE;   
            }
            else
            {
                 writeLen = fileSize;
            }
            fread(buff, 1, writeLen, pfile); //读取一块数据
            
            bspFlashWrite(writeAddr, buff, writeLen);
            bspDelayMs(10);
            
            fileSize  -= writeLen;
            writeAddr += writeLen;
            
        }while (fileSize > 0);

        free(buff);       //释放内存
        fclose(pfile);

        return 0;
    }
    else
    {
        return 4;
    }
}


/*********************************************************************************************
函 数 名: servFpgaLoaclFileLoad
实现功能: 从本地加载文件到FPGA中
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
u8 servFpgaLoaclFileLoad(char const* filePath)
{
    u8 tryCountMax = 3;    //最多试3次
    u8 tryCount = 0;
    u8 errorCode = 0;
    
    s32 fileSize;
    u8 *buff = NULL;
    u32 dataSize;
    u32 readover = 0;


    while (1)
    {
        if (!servFpgaProgStart())   //配置启动不成功
        {
            tryCount++;
            bspDelayMs(1);

            if (tryCount >= tryCountMax)
            {
                return 1;
            }
        }
        else
        {
            break;
        }
    }
    
    FILE * pfile = fopen(filePath, "rb");    //打开本地文件
    bspDelayUs(20);
    if (pfile == NULL)
    {
        fclose(pfile);
        return 2;
    }
    
    fseek(pfile, 0L, SEEK_END);    //定位到文件末尾
    fileSize = ftell(pfile);       //得到文件大小

    buff = (u8 *)malloc(FPGA_CFG_BUFF_SIZE);    //申请内存
    if (buff != NULL)
    {
        tryCount = 0;
        while (1)
        {
            for (readover = 0;readover < fileSize;)
            {
                fseek(pfile, readover, SEEK_SET);    //定位到上次读到的位置
                
                if ((fileSize - readover) >= FPGA_CFG_BUFF_SIZE)
                {
                    dataSize = FPGA_CFG_BUFF_SIZE;
                }
                else
                {
                    dataSize = fileSize - readover;
                }
                fread(buff, 1, dataSize, pfile);    //读取一块数据
                readover += dataSize;               //偏移
                
                bspFpSpiSend(buff, dataSize);
            }
        
            if (servFpgaProgDone())    //配置成功则退出循环
            {
                break;
            }
            else
            {
                tryCount++;

                if (tryCount >= tryCountMax)
                {
                    errorCode = 3;
                    break;
                }
            }
        }
        
        free(buff);       //释放内存
        fclose(pfile);    //关闭文件
        
        return errorCode;
    }
    else
    {
        return 4;
    }
}


/*********************************************************************************************
函 数 名: servFpgaFlashFileLoad
实现功能: 从本地加载文件到FPGA中
输入参数: 
输出参数:   
返 回 值: 
说    明:
*********************************************************************************************/
u8 servFpgaFlashFileLoad(u32 loadAddr)
{
    u8 tryCountMax = 3;    //最多试3次
    u8 tryCount = 0;
    u8 errorCode = 0;

    s32 dataLen = 0;
    u8 *buff = NULL;
    u32 readLen = 0;
    u32 flashAddr;
    u8  sizeLen = sizeof(dataLen);

    
    while (1)
    {
        if (!servFpgaProgStart())   //配置启动不成功
        {
            tryCount++;
            bspDelayMs(1);

            if (tryCount >= tryCountMax)
            {
                return 1;
            }
        }
        else
        {
            break;
        }
    }
    
    tryCount = 0;
    buff = (u8*)malloc(FPGA_CFG_BUFF_SIZE);
    if (buff != NULL)
    {
        while (1)
        {
            readLen = 0;
            flashAddr = loadAddr;
            bspFlashRead(flashAddr, (u8 *)&dataLen, sizeLen);    //读取数据长度
            flashAddr += sizeLen;

            //判断下数据长度，如果长度超出了FLASH分配的范围则直接退出
            if ((dataLen > FLASH_FPGA_CODE_LEN) || (dataLen < 0))
            {
                errorCode = 2;
                break;
            }
            
            do
            {
                if (dataLen > FPGA_CFG_BUFF_SIZE)
                {
                    readLen = FPGA_CFG_BUFF_SIZE;
                }
                else
                {
                    readLen = dataLen;
                }
                bspFlashRead(flashAddr,buff,readLen);
                
                bspFpSpiSend(buff,readLen);

                dataLen   -= readLen;
                flashAddr += readLen;
                
            }while (dataLen > 0);
         
            if (servFpgaProgDone())    //配置成功则退出循环
            {
                break;
            }
            else
            {
                tryCount++;

                if (tryCount >= tryCountMax)
                {
                    errorCode = 3;
                    break;
                }
            }
        }
        
        free(buff);
        
        return errorCode;
    }
    else
    {
        return 4;
    }
}


/*********************************************************************************************
函 数 名: servFpgaVersionGet
实现功能: 获取FPGA版本号
输入参数: 
输出参数:   
返 回 值: 
说    明:
*********************************************************************************************/
FpgaVersionStruct servFpgaVersionGet(void)
{
    u16 tmpReg = 0;
    FpgaVersionStruct fpgaVersion;

    
    //读逻辑版本号
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_VERSION_REG, &tmpReg);
    
    fpgaVersion.majorVer = (u8)((tmpReg & SERV_FPGA_VERSION_MAJOR_MASK) >> SERV_FPGA_VERSION_MAJOR_RSHIFT);
    fpgaVersion.minorVer = (u8)((tmpReg & SERV_FPGA_VERSION_MINOR_MASK) >> SERV_FPGA_VERSION_MINOR_RSHIFT);
    fpgaVersion.buildVer = (u8)(tmpReg & SERV_FPGA_VERSION_BUILD_MASK);


    //读产品型号
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_TYPE_REG, &tmpReg);
    
    fpgaVersion.majorType = (u8)((tmpReg & SERV_FPGA_TYPE_MAJOR_MASK) >> SERV_FPGA_TYPE_MAJOR_RSHIFT);
    fpgaVersion.minorType = (u8)((tmpReg & SERV_FPGA_TYPE_MINOR_MASK) >> SERV_FPGA_TYPE_MINOR_RSHIFT);
    fpgaVersion.hardVer   = (u8)(tmpReg & SERV_FPGA_TYPE_HARD_MASK);
    
    
    return fpgaVersion;
}


/*********************************************************************************************
函 数 名: servFPGA_State_If_WR_Allow
实现功能: 获取DDR状态是否可以进行DDR写入
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 就目前的应用而言,不会出现不允许写入的情况,若出现则报错
*********************************************************************************************/
s32 servFPGA_State_If_WR_Allow(u8 chanNum)
{
    u16 state = 0xFF;
    u8 ii = 0;
    do
    {
        servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_10, &state);
        ii++;
        bspDelayUs(3);
    }while(((state&0x03) != 0x03)&&(ii<3));


    if(ii < 3)
    {
        return 0;
    }
    else
    {
        /*
            ERROR!!
        */
        return -1;
    }
    
}


/*********************************************************************************************
函 数 名: servFpgaWaveDataStateRead
实现功能: 获取DDR状态是否可以开始执行 DDR 产生PWM
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
*********************************************************************************************/
s32 servFpgaWaveDataStateRead(u8 chanNum)
{
    FpgaResponseReg10Union responseReg;

    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_10, &responseReg.regValue);

    if (responseReg.regBitFiled.waveDataReady)
    {
        return 0;
    }
    else
    {
        return -1;
    }

}


/*********************************************************************************************
函 数 名: servFpgaDDRWriteSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDDRWriteSet(DDRWriteEnum ddrWrite)
{
    fpgaRegValue.register27.regBitFiled.wrDdrFinish = ddrWrite;
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_27, fpgaRegValue.register27.regValue);
}


/*********************************************************************************************
函 数 名: servFpgaDDRDataTypeSet
实现功能: 通过FPGA配置驱动时钟
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDDRDataTypeSet(u8 chanNum, WaveDataTypeEnum dateType)
{
    fpgaRegValue.register23[chanNum].regBitFiled.waveType = dateType;
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_23, fpgaRegValue.register23[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaRunStateGet
实现功能: 获取逻辑的运行状态
输入参数:     
              
输出参数:
            
返 回 值: 0:停止状态      FPGA_STOP= 0
          1:正常运行状态  FPGA_RUNNING = 1
          2:减速运行状态  FPGA_BREAK = 2
          3:错误状态      FPGA_ERROR = 3
说    明: 10号寄存器bit[10-7]值含义:
              0/3: FPGA空闲 没有SD信号输出
              1/4: 正在按照波表输出SD
              2: 急停减速状态    如果是速度保持模式 则也返回此状态
           
*********************************************************************************************/
RunStatenum servFpgaRunStateGet(u8 chanNum)
{
    FpgaResponseReg10Union responseReg;

    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_10, &responseReg.regValue);

    switch (responseReg.regBitFiled.waveState)
    {
        case WAVESTATE_IDLE1:
        case WAVESTATE_IDLE2:
            return RUNSTETE_NORUN;
          break;

        case WAVESTATE_SD:
            return RUNSTETE_SD;
          break;
          
        case WAVESTATE_OUTP1:
        case WAVESTATE_OUTP2:
            return RUNSTETE_RUN;
          break;

        case WAVESTATE_HOLD:
            return RUNSTETE_HOLD;
          break;

        default:
            return RUNSTETE_ERROR;
          break;
    }
}



/*********************************************************************************************
函 数 名: servFPGA_Cycle_Cnt_Get
实现功能: 获取逻辑当前执行的循环序数
输入参数:     
              
输出参数:
            
返 回 值: 0
说    明: 
         逻辑返回的数从零开始 因此需要加1
*********************************************************************************************/
void servFPGA_Cycle_Cnt_Get(u8 chanNum, u32 *times)
{
    u16 data_L = 0;
    u16 data_H = 0;
    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_22, &data_L);
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_23, &data_H);
    
    *times = (data_H<<16)|data_L + 1;
}


/*********************************************************************************************
函 数 名: servFpgaWaveWritableSizeRead
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 首先从逻辑获取逻辑还未输出的波表数据的个数,
          然后用FIFO总空间数-未输出个数 = 还可以写入的波表个数
*********************************************************************************************/
u32 servFpgaWaveWritableSizeRead(u8 chanNum, u32 totalSize)
{
    u16 regValue    = 0;
    u32 remainPoint = 0;
    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_03, &regValue);//FIFO遗留数据 低16bit
    remainPoint = regValue;

    regValue = 0;
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_04, &regValue);//FIFO遗留数据 高16bit
    remainPoint += (regValue << 16) & 0xFFFF0000;

    return (totalSize - remainPoint);
}


/*********************************************************************************************
函 数 名: servFpgaWaveDataStateRead
实现功能: 获取DDR状态是否可以开始执行 DDR 产生PWM
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
*********************************************************************************************/
u8 servFpgaSdioWrErrorRead(void)
{
    FpgaResponseReg06Union responseReg;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_06, &responseReg.regValue);

    return responseReg.regBitFiled.sdioWrError;
}


#if 0
#endif


/*********************************************************************************************
函 数 名: servFpgaDioStatusRead
实现功能: 读取数字端口电平
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.10 Add
*********************************************************************************************/
LevelStatusEnum servFpgaDioStatusRead(DigitalIONumEnum digitalIONum)
{
    u16 registerValue;
    LevelStatusEnum levelStatus;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_DIO_STATUS_REG, &registerValue);
    
    levelStatus = (LevelStatusEnum)((registerValue >> (u8)digitalIONum) & SERV_FPGA_DIO_STATUS_MASK); 

    return levelStatus;
}


/*********************************************************************************************
函 数 名: servFpgaDigitalOutLevelSet
实现功能: 设置数字端口电平
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.10 Add
*********************************************************************************************/
void servFpgaDigitalOutLevelSet(DoutNumEnum dIONum, LevelStatusEnum levelStatus)
{
    switch (dIONum)
    {
#ifdef PROJECT_QUBELEY
        case DIO_DO1:

            fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_DO1_CONFIG_MASK;
            fpgaRegValue.digitalOutputReg |= SERV_FPGA_DO1_SELECT_LEVEL;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.digitalOutputReg |= SERV_FPGA_DO1_LEVEL_HIGH;
            }
            else
            {
                fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_DO1_LEVEL_HIGH;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DIGITAL_OUTPUT_REG, fpgaRegValue.digitalOutputReg);
          break;

        case DIO_DO2:

            fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_DO2_CONFIG_MASK;
            fpgaRegValue.digitalOutputReg |= SERV_FPGA_DO2_SELECT_LEVEL;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.digitalOutputReg |= SERV_FPGA_DO2_LEVEL_HIGH;
            }
            else
            {
                fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_DO2_LEVEL_HIGH;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DIGITAL_OUTPUT_REG, fpgaRegValue.digitalOutputReg);
          break;
#endif

#ifdef PROJECT_GELGOOG          
        case DIO_DO1:
        
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN1;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN1_HIGH;
            }
            else
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN1_LOW;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
          break;

        case DIO_DO2:
        
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN2;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN2_HIGH;
            }
            else
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN2_LOW;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);

          break;

        case DIO_DO3:
        
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN3;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN3_HIGH;
            }
            else
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN3_LOW;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
          break;

        case DIO_DO4:
        
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN4;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN4_HIGH;
            }
            else
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN4_LOW;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
          break;
#endif

        default:
          break;
    }  
}


/*********************************************************************************************
函 数 名: servFpgaDigitalOutSet
实现功能: 设置数字输出
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaDigitalOutSet(DoutNumEnum dIONum, DigitalOutManageStruct digitalIO)
{
    LevelStatusEnum levelState;

    
    if (SENSOR_ON == digitalIO.state)
    {
        levelState = (LevelStatusEnum)digitalIO.polarity;
    }
    else    //关闭时配成高电平表示处于Uart的Tx模式
    {
        levelState = LEVEL_HIGH;
    }

    servFpgaDigitalOutLevelSet(dIONum, levelState);

    /*{
        if (DIO_DO1 == dIONum)
        {
            //TO ADD    NICK MARK
        }
        else if (DIO_DO2 == dIONum)
        {
            //drvUart3Init(digitalIO.baud);
        }
    }*/
}


/*********************************************************************************************
函 数 名: servFpgaIsolatorOutLevelSet
实现功能: 设置隔离端口电平
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.10 Add
*********************************************************************************************/
void servFpgaIsolatorOutLevelSet(YoutNumEnum youtNum, LevelStatusEnum levelStatus)
{
    switch (youtNum)
    {
        case YOUT_YO1:
            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.digitalOutputReg |= SERV_FPGA_YO1_LEVEL_HIGH;
            }
            else
            {
                fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_YO1_LEVEL_HIGH;
            }
          break;

        case YOUT_YO2:
            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.digitalOutputReg |= SERV_FPGA_YO2_LEVEL_HIGH;
            }
            else
            {
                fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_YO2_LEVEL_HIGH;
            }
          break;

        default:
          break;
    }
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DIGITAL_OUTPUT_REG, fpgaRegValue.digitalOutputReg);
}


/*********************************************************************************************
函 数 名: servFpgaIsolatorOutSet
实现功能: 设置数字输出
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaIsolatorOutSet(YoutNumEnum youtNum, IsolatorOutManageStruct isolOut)
{
    LevelStatusEnum levelState;

    
    if (SENSOR_ON == isolOut.state)
    {
        levelState = (LevelStatusEnum)isolOut.response;
    }
    else
    {
        levelState = (LevelStatusEnum)(DIOPOLARITY_P - isolOut.response);
    }

    servFpgaIsolatorOutLevelSet(youtNum, levelState);
}



/*********************************************************************************************
函 数 名: servFpgaTriggerFilterSet
实现功能: 设置触发输入滤波
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaTriggerFilterSet(u8 chanNum, TrigInPinEnum trigInPin, f32 sPeriod)
{
    u16 registerValue = 0;
    u16 sPeriodCode;

    
    //计算配置的寄存器值
    if (sPeriod < SERV_FPGA_SPERIOD_GEAR_VALUE)    //400us为分界
    {
        registerValue &= ~SERV_FPGA_SPERIOD_US_ENABLE;
        sPeriodCode = (u16)(sPeriod / SERV_FPGA_SPERIOD_NS_VALUE - SERV_FPGA_SPERIOD_OFFSET) 
                      & SERV_FPGA_FILTER_TIME_MASK;
        if (sPeriodCode < SERV_FPGA_SPERIOD_CODE_MIN)
        {
            sPeriodCode = SERV_FPGA_SPERIOD_CODE_MIN;
        }
        registerValue |= sPeriodCode;
    }
    else
    {
        registerValue |= SERV_FPGA_SPERIOD_US_ENABLE;
        sPeriodCode = (u16)(sPeriod / SERV_FPGA_SPERIOD_US_VALUE - SERV_FPGA_SPERIOD_OFFSET) 
                      & SERV_FPGA_FILTER_TIME_MASK;
        if (sPeriodCode < SERV_FPGA_SPERIOD_CODE_MIN)
        {
            sPeriodCode = SERV_FPGA_SPERIOD_CODE_MIN;
        }
        registerValue |= sPeriodCode;
    }

    switch (trigInPin)
    {
#ifdef PROJECT_QUBELEY

        case TRIGPIN_DI2_X1:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI2_X1_FILTER_REG, registerValue);
          break;

        case TRIGPIN_DI3_X2:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI3_X2_FILTER_REG, registerValue);
          break;

        case TRIGPIN_DI4_X3:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI4_X3_FILTER_REG, registerValue);
          break;

        case TRIGPIN_DI5_X4:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI5_X4_FILTER_REG, registerValue);
          break;

        case TRIGPIN_CHANZ:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CHANZ_FILTER_REG, registerValue);
          break;
          
#endif

#ifdef PROJECT_GELGOOG

        case TRIGPIN_DIL:
#if GELGOOG_AXIS_10
            //临时处理下 for debug 
            servFpgaRegisterWrite(chanNum, 52, registerValue);
#else

            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI4_X3_FILTER_REG, registerValue);
#endif
          break;

        case TRIGPIN_DIR:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI5_X4_FILTER_REG, registerValue);
          break;
          
#endif
        default:
          break;
    }
}


/*********************************************************************************************
函 数 名: servFpgaTriggerSet
实现功能: 设置触发输入码型及响应
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaTriggerSet(u8 chanNum, TrigInManageStruct trigInInfo, EncoderChanEnum encChan)
{
    u8 i;
    u8 bitValue[5] = {SERV_FPGA_TRIG_COND_NONE, 
                      SERV_FPGA_TRIG_COND_LOW, 
                      SERV_FPGA_TRIG_COND_RISE, 
                      SERV_FPGA_TRIG_COND_FALL, 
                      SERV_FPGA_TRIG_COND_HIGH};
    u8  trigState[2] = {SERV_FPGA_TRIG_DISABLE, SERV_FPGA_TRIG_ENABLE};
    u16 registerCode = 0;
    u16 alarmRegCode = 0;
    u16 registerValue;


    if (TRIGMODE_PATTERN == trigInInfo.trigMode)    //码型触发
    {
#if 0
        for (i = 0;i < TRIGPIN_RESERVE;i++)
        {
            registerCode |= (bitValue[(u8)trigInInfo.type[i]] & trigState[(u8)trigInInfo.pattState])
                            << (SERV_FPGA_TRIG_COND_LEN * i);

            //提前设置下采样周期
            servFpgaTriggerFilterSet((TrigInPinEnum)i, trigInInfo.pattSPeriod);
        }

        if ((0 == registerCode) ||    //触发条件全是NONE或者pattState是OFF，则49号寄存器要设置成0 
            (RESPONSE_NONE == trigInInfo.pattResponse))
        {
            //两个寄存器都失能
            trigRelRegValue = (~SERV_FPGA_STOP_OPR_ENABLE) & (~SERV_FPGA_ALARM_OPR_ENABLE);
            servFpgaRegisterWrite(0, SERV_FPGA_TRIG_REL_REG, trigRelRegValue);    //FOR MODIFY NICK
        }
        else
        {
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_DIO_ENABLE;    //使能数字触发
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
            
            switch (trigInInfo.pattResponse)
            {
                case RESPONSE_NONE:
                  break;

                case RESPONSE_ALARM:
                    //设置触发条件
                    registerValue = registerCode;
                    servFpgaRegisterWrite(0, SERV_FPGA_ALARM_CODE_REG, registerValue);    //FOR MODIFY NICK                    

                    //设置触发条件逻辑关系
                    trigRelRegValue = SERV_FPGA_ALARM_OPR_AND;
                    servFpgaRegisterWrite(0, SERV_FPGA_TRIG_REL_REG, trigRelRegValue);    //FOR MODIFY NICK
                
                    //设置中断
                    fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_ALARM_INT_ENABLE;
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.interruptSrcReg[chanNum]);
                  break;
                  
                case RESPONSE_STOP:
                    //设置触发条件
                    registerValue = registerCode;
                    servFpgaRegisterWrite(0, SERV_FPGA_STOP_SWITCH_REG, registerValue);    //FOR MODIFY NICK  

                    //设置触发条件逻辑关系
                    trigRelRegValue = SERV_FPGA_STOP_OPR_AND;
                    servFpgaRegisterWrite(0, SERV_FPGA_TRIG_REL_REG, trigRelRegValue);    //FOR MODIFY NICK
                    
                    //设置中断
                    fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_STOP_INT_ENABLE;
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.interruptSrcReg[chanNum]);
                  break;
                  
                case RESPONSE_ALARMSTOP:
                    //设置触发条件
                    registerValue = registerCode;
                    servFpgaRegisterWrite(0, SERV_FPGA_STOP_SWITCH_REG, registerValue);    //FOR MODIFY NICK
                    servFpgaRegisterWrite(0, SERV_FPGA_ALARM_CODE_REG,  registerValue);    //FOR MODIFY NICK  

                    //设置触发条件逻辑关系
                    trigRelRegValue = SERV_FPGA_STOP_OPR_AND | SERV_FPGA_ALARM_OPR_AND;
                    servFpgaRegisterWrite(0, SERV_FPGA_TRIG_REL_REG, trigRelRegValue);    //FOR MODIFY NICK
                    
                    //设置中断
                    fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_STOP_INT_ENABLE | SERV_FPGA_ALARM_INT_ENABLE;
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.interruptSrcReg[chanNum]);
                  break;
                  
                /*case RESPONSE_RUN:    //目前不支持RUN
                  break;*/

                default:
                  break;
            }
        }
#endif
    }
    else
    {
        for (i = 0;i < TRIGPIN_RESERVE;i++)
        {
            switch (trigInInfo.levelResponse[i])
            {
                case RESPONSE_NONE:
                  break;

                case RESPONSE_ALARM:
                    alarmRegCode |= (bitValue[(u8)trigInInfo.type[i]] & trigState[(u8)trigInInfo.levelState[i]]) 
                                    << (SERV_FPGA_TRIG_COND_LEN * (i + SERV_FPGA_TRIG_IN_OFFSET));
                  break;
                  
                case RESPONSE_STOP:
                    registerCode |= (bitValue[(u8)trigInInfo.type[i]] & trigState[(u8)trigInInfo.levelState[i]]) 
                                    << (SERV_FPGA_TRIG_COND_LEN * (i + SERV_FPGA_TRIG_IN_OFFSET));
                  break;
                  
                case RESPONSE_ALARMSTOP:
                    alarmRegCode |= (bitValue[(u8)trigInInfo.type[i]] & trigState[(u8)trigInInfo.levelState[i]]) 
                                    << (SERV_FPGA_TRIG_COND_LEN * (i + SERV_FPGA_TRIG_IN_OFFSET));
                    registerCode = alarmRegCode;
                  break;
                  
                /*case RESPONSE_RUN:    //目前不支持RUN
                  break;*/

                default:
                  break;
            }

            //提前设置下采样周期
            servFpgaTriggerFilterSet(chanNum, (TrigInPinEnum)i, trigInInfo.levelSPeriod[i]);
        }

        //先配置ALARM寄存器
        if (0 == alarmRegCode)
        {
            //失能
            fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_ALARM_OPR_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
        }
        else
        {
            //设置触发条件
            registerValue = registerCode;
            servFpgaRegisterWrite(0, SERV_FPGA_ALARM_CODE_REG, registerValue); 

            //设置触发条件逻辑关系
            fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_ALARM_OPR_OR;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
            
            //设置中断
            fpgaRegValue.register50[chanNum].regBitFiled.alarm = INT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
            
            fpgaRegValue.register32[chanNum].regBitFiled.sdSrcHard = BITOPT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
        }

        //再配置STOP寄存器
        if (0 == registerCode)
        {
            //失能
            fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_STOP_OPR_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
        }
        else
        {
            //设置触发条件
            registerValue = registerCode;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_STOP_SWITCH_REG, registerValue);   

            //设置触发条件逻辑关系
            fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_STOP_OPR_OR;

            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
            
            //设置中断
            fpgaRegValue.register50[chanNum].regBitFiled.trigger = INT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);

            //临时配置，后面修改成专门的函数来设置    NICK MARK
            fpgaRegValue.register32[chanNum].regBitFiled.sdSrcHard = BITOPT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
        }
    }
}


/*********************************************************************************************
函 数 名: servFpgaWaveEndIntSet
实现功能: 使能或失能波表结束时的中断
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaWaveEndIntSet(u8 chanNum, bool bEnable)
{
    if (bEnable)
    {
        fpgaRegValue.register50[chanNum].regBitFiled.waveEnd = INT_ENABLE;
    }
    else
    {
        fpgaRegValue.register50[chanNum].regBitFiled.waveEnd = INT_DISABLE;
    }
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaIntrSourceGet
实现功能: 获取FPGA给ARM中断的源头
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
u8 servFpgaIntrSourceGet(u8 chanNum, u16 *intrSrcValue)
{
    return servFpgaRegisterRead(chanNum, SERV_FPGA_INTERRUPT_SRC_REG, intrSrcValue);
}


/*********************************************************************************************
函 数 名: servFpgaDistStopModeSet
实现功能: 按距离停止模式设置
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.09 Add
*********************************************************************************************/
void servFpgaDistStopModeSet(u8 chanNum, StopConfigStruct stopConfigInfo)
{
    u16 tempValue = 0;
    u32 periodInt = 0;    //整数部分
    u32 periodDec = 0;    //小数部分
    f32 emergDecelSpeed;


    //配置减速阈值
    tempValue = (u16)stopConfigInfo.emergPeriod;  //低16bit
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_PERIOD_L_REG, tempValue);

    tempValue = (stopConfigInfo.emergPeriod >> SERV_FPGA_STOPDECEL_PERIOD_H_RSHIFT) & 
                SERV_FPGA_STOPDECEL_PERIOD_H_MASK;  //高9bit
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_PERIOD_H_REG, tempValue);

    //计算减速斜率
    emergDecelSpeed = (f32)(stopConfigInfo.emergPeriod - stopConfigInfo.minPeriod) / stopConfigInfo.stopSteps;

    //配置减速斜率          
    periodInt = (u32)emergDecelSpeed;
    periodDec = (u32)((emergDecelSpeed - periodInt) * SERV_FPGA_STOPDECEL_SLOPE_DEC_VAILD_BITS);
    
    //小数的低16位
    tempValue = (u16)periodDec;    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_SLOPE_L_REG, tempValue);

    //整数的低9位 + 小数的高7位
    tempValue = ((u16)(periodDec >> SERV_FPGA_STOPDECEL_SLOPE_RSHIFT) & SERV_FPGA_STOPDECEL_SLOPE_M_DEC_MASK) | 
                ((periodInt & SERV_FPGA_STOPDECEL_SLOPE_M_INT_MASK) << SERV_FPGA_STOPDECEL_SLOPE_M_INT_LSHIFT);    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_SLOPE_M_REG, tempValue);

    //整数的高16位
    tempValue = (u16)(periodInt >> SERV_FPGA_STOPDECEL_SLOPE_RSHIFT); 
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_SLOPE_H_REG, tempValue);
}


/*********************************************************************************************
函 数 名: servFpgaStopDecelSet
实现功能: 设置急停减速信息
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.09 Add
*********************************************************************************************/
void servFpgaStopDecelSet(u8 chanNum, StopConfigStruct stopConfigInfo)
{
    fpgaRegValue.register32[chanNum].regBitFiled.stopDecType = SDTYPE_NONE;
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcEnd    = BITOPT_ENABLE;

    switch (stopConfigInfo.stopMode)
    {
        case STOPMODE_IMMED: 
            fpgaRegValue.register32[chanNum].regBitFiled.stopDecType = SDTYPE_IMMED;      //减速方式立即停止
            fpgaRegValue.register32[chanNum].regBitFiled.sdSrcEnd    = BITOPT_DISABLE;    //禁止急停减速，直接停止
          break;
          
        /*case STOPMODE_HOLD:
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SLOPE_ENABLE;           //减速方式按减速度减

            //先按减速度的方式设置，后续等FPGA修改好了再改    NICK MARK
            holdStopConfigInfo.emergPeriod = EMERG_STOP_END_PERIOD;
            holdStopConfigInfo.minPeriod   = EMERG_STOP_END_PERIOD;    //两个周期一样斜率就为0了
            holdStopConfigInfo.stopSteps   = 1;
            servFpgaDistStopModeSet(chanNum, holdStopConfigInfo);
          break;*/

        case STOPMODE_DIST:
            //目前按距离减速的方式是在波表计算过程中，统计周期最小值，
            //然后在这里安装最小值和阈值的差值以及要求的距离计算周期增量
            //后续等FPGA修改好了之后再改    NICK MARK
            fpgaRegValue.register32[chanNum].regBitFiled.stopDecType = SDTYPE_SPEED;

            servFpgaDistStopModeSet(chanNum, stopConfigInfo);
          break;

        case STOPMODE_TIME:
            //目前不支持
            //emergTime = stopDecelInfo.emergTime;
          break;

        case STOPMODE_DIST_TIME:
            //目前不支持
            //emergTime = stopDecelInfo.emergTime;
          break;

        default:
          break;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveHoldSet
实现功能: 设置波形保持信息
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.09 Add
*********************************************************************************************/
void servFpgaWaveHoldSet(u8 chanNum, bool waveHold)
{
    fpgaRegValue.register32[chanNum].regBitFiled.waveHoldSrc = WHSRC_NONE;

    if (waveHold)
    {
        fpgaRegValue.register32[chanNum].regBitFiled.waveHoldSrc = WHSRC_END;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaEndStateSet
实现功能: 设置波形结束的状态
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.09 Add
*********************************************************************************************/
void servFpgaEndStateSet(u8 chanNum, EndStateEnum endState, StopConfigStruct stopConfigInfo)
{
    servFpgaStopDecelSet(chanNum, stopConfigInfo);
            
    switch (endState)
    {
        case ENDSTATE_STOP: 
            servFpgaWaveHoldSet(chanNum, false);
          break;
          
        case ENDSTATE_HOLD: 
            servFpgaWaveHoldSet(chanNum, true);
          break;

        default:
          break;
    }
}


/*********************************************************************************************
函 数 名: servFpgaWaveWorkModeSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWaveWorkModeSet(u8 chanNum, WaveWorkModeEnum waveWorkMode)
{
    if (WTWORKMODE_FIFO == waveWorkMode)
    {
        fpgaRegValue.register31[chanNum].regBitFiled.waveStopMode = WSTOPMODE_FIFO; 
    }
    else if (WTWORKMODE_CYCLE == waveWorkMode)
    {
        fpgaRegValue.register31[chanNum].regBitFiled.waveStopMode = WSTOPMODE_CYCLE; 
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveModifyDutySet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWaveModifyDutySet(u8 chanNum, ModifyDutyEnum modifyDuty)
{  
    fpgaRegValue.register31[chanNum].regBitFiled.lvtModify = modifyDuty; 

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveLineStepsSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWaveLineStepsSet(u8 chanNum, f32 lineSteps)
{
    u64  lineStepsInverse;


    //配置线间步，配置的是倒数
    lineStepsInverse = (u64)(SERV_FPGA_LINE_STEPS_MAX / (f64)lineSteps);

    servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_STEPS_INVS_L_REG, 
                          (u16)lineStepsInverse);
                         
    servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_STEPS_INVS_M_REG, 
                          (u16)(lineStepsInverse >> 16));
                         
    servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_STEPS_INVS_H_REG, 
                          (u16)(lineStepsInverse >> 32));

    servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_STEPS_VAILD_BIT_REG, 
                          SERV_FPGA_LINE_STEPS_VAILD_BIT_VALUE);
}


/*********************************************************************************************
函 数 名: servFpgaWaveLineMultSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWaveLineMultSet(u8 chanNum, u32 lineMult)
{
    //配置编码器反馈比
    fpgaRegValue.register39[chanNum].regBitFiled.encDivSelect = lineMult - SERV_FPGA_ENC_LINE_MULT_OFFSET;

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_39, fpgaRegValue.register39[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveCalcModeSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWaveCalcModeSet(u8 chanNum, MotionModeEnum motionMode)
{
    switch (motionMode)
    {
        case MTNMODE_PVT:
            fpgaRegValue.register31[chanNum].regBitFiled.motionMode = WMMODE_PVT;
          break;
          
        case MTNMODE_LVT_CORR:
            fpgaRegValue.register31[chanNum].regBitFiled.motionMode = WMMODE_LVT_CORR;
          break;
          
        case MTNMODE_LVT_NOCORR:
            fpgaRegValue.register31[chanNum].regBitFiled.motionMode = WMMODE_LVT_NOCORR;
          break;

        default:
          break;
          
    }
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveMotionModeSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWaveMotionModeSet(u8 chanNum, MotionModeEnum motionMode, f32 lineSteps, u32 lineMult, ModifyDutyEnum modifyDuty)
{
    servFpgaWaveCalcModeSet(chanNum, motionMode);    

    if ((MTNMODE_PVT != motionMode) && (lineSteps >= 2.0))    //CJ 2017.11.25 Modify
    //if ((MTNMODE_PVT != motionMode) && (lineSteps != 0) && (lineMult != 0))
    {
        servFpgaWaveModifyDutySet(chanNum, modifyDuty);
        servFpgaWaveLineStepsSet(chanNum, lineSteps);
        servFpgaWaveLineMultSet(chanNum, lineMult); 
    }
}


/*********************************************************************************************
函 数 名: servFpgaWaveInit
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWaveInit(u8 chanNum, WaveTableStruct waveTable)
{
    servFpgaWaveWorkModeSet(chanNum, waveTable.waveWorkMode);
    //servFpgaWaveMotionModeSet(chanNum, waveTable.motionMode);    
}


/*********************************************************************************************
函 数 名: servFpgaWaveTableInit
实现功能: 复位波表
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveTableInit(WaveTableStruct *pWaveTable, WaveWorkModeEnum waveWorkMode)
{
    pWaveTable->writableAddr = pWaveTable->startAddr;
    pWaveTable->writableSize = pWaveTable->totalSize;
    
    pWaveTable->waveState = MTSTATE_IDLE;
    
    pWaveTable->waveWorkMode = waveWorkMode;
}


/*********************************************************************************************
函 数 名: servFpgaWaveBufferReset
实现功能: 预取前复位缓存
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaWaveBufferReset(u8 chanNum)
{
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop = BITOPT_ENABLE;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
    
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop = BITOPT_DISABLE;
}


/*********************************************************************************************
函 数 名: servFpgaWaveFifoReset
实现功能: 在FIFO模式下进行配置波表前对FIFO复位
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.12.24 Add
*********************************************************************************************/
void servFpgaWaveFifoReset(u8 chanNum)
{
    fpgaRegValue.register32[chanNum].regBitFiled.waveReset = BITOPT_ENABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop  = BITOPT_ENABLE;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);

    fpgaRegValue.register32[chanNum].regBitFiled.waveReset = BITOPT_DISABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop  = BITOPT_DISABLE;
}


/*********************************************************************************************
函 数 名: servFpgaWaveWriteDdrAddrSet
实现功能: 设置波表写入地址
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2018.02.01 Add
*********************************************************************************************/
void servFpgaWaveWriteDdrAddrSet(u32 waveAddr)
{
    u32 ddrAddr;


    //传入的是波表地址，和DDR地址的对应关系是ddrAddr = waveAddr * 4
    ddrAddr = waveAddr * FPGA_DDR_WAVE_TABLE_POINT_LEN;
    
    fpgaRegValue.register24.regBitFiled.regData = (u16)(ddrAddr & 0xFFFF);
    fpgaRegValue.register25.regBitFiled.regData = (u16)((ddrAddr >> 16) & 0x0FFF);

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_24, fpgaRegValue.register24.regValue);
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_25, fpgaRegValue.register25.regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveStartDdrAddrSet
实现功能: 设置波表输出起始地址
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2018.02.01 Add
*********************************************************************************************/
void servFpgaWaveStartDdrAddrSet(u8 chanNum, u32 waveAddr)
{
    u32 ddrAddr;


    //传入的是波表地址，和DDR地址的对应关系是ddrAddr = waveAddr * 4
    ddrAddr = waveAddr * FPGA_DDR_WAVE_TABLE_POINT_LEN;
    
    fpgaRegValue.register33[chanNum].regBitFiled.regData = (u16)(ddrAddr & 0xFFFF);
    fpgaRegValue.register34[chanNum].regBitFiled.regData = (u16)((ddrAddr >> 16) & 0x0FFF);

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_33, fpgaRegValue.register33[chanNum].regValue);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_34, fpgaRegValue.register34[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveDateLenSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaWaveDateLenSet(u8 chanNum, u32 len)
{
    fpgaRegValue.register35[chanNum].regBitFiled.regData = (u16)(len & 0xFFFF);
    fpgaRegValue.register36[chanNum].regBitFiled.regData = (u16)(len >> 16);
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_35, fpgaRegValue.register35[chanNum].regValue);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_36, fpgaRegValue.register36[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveDateCycleSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaWaveDateCycleSet(u8 chanNum, u32 cycleNum)
{
    fpgaRegValue.register37[chanNum].regBitFiled.regData = (u16)(cycleNum & 0xFFFF);
    fpgaRegValue.register38[chanNum].regBitFiled.regData = (u16)(cycleNum >> 16);
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_37, fpgaRegValue.register37[chanNum].regValue);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_38, fpgaRegValue.register38[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveDataWrite
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
s32 servFpgaWaveDataWrite(u8 chanNum, OutputDataStruct *pOutputData, WaveTableStruct *pWaveTable)
{
    static u8 lastChan = 0;
    u8  txState = 0;
    s32 writeResult = 0;


    //波表未配置
    if (MTSTATE_IDLE == pWaveTable->waveState)
    {    
        //修改波表状态
        pWaveTable->waveState = MTSTATE_CALCING;
        
        //配置数据类型
        servFpgaDDRDataTypeSet(chanNum, DATATYPE_WAVE);

        //初始化波表
        servFpgaWaveInit(chanNum, *pWaveTable);

        //FIFO模式需要Reset
        if (WTWORKMODE_FIFO == pWaveTable->waveWorkMode)
        {
            servFpgaWaveFifoReset(chanNum);

            //这一步理论上在预取的时候设置也是可行的，目前先按旧的流程设置，后续测试后再修改    NICK MARK
            servFpgaWaveDateLenSet(chanNum, pWaveTable->totalSize);
        }
    }
    
    //通道发生切换
    if (lastChan != chanNum)
    {
        lastChan = chanNum;

        //配置数据类型
        servFpgaDDRDataTypeSet(chanNum, DATATYPE_WAVE);
    }

    //读取逻辑状态，检测是否可以进行波表下发
    writeResult = servFPGA_State_If_WR_Allow(chanNum);    
    if (writeResult != 0)
    {
        return writeResult;
    }
        
    //配置开始标志
    servFpgaDDRWriteSet(DDRWR_START);
        
    //配置写入地址
    servFpgaWaveWriteDdrAddrSet(pWaveTable->writableAddr);

    //有要发送的数据才发送
    if (pOutputData->toSendBytes > 0)
    {
        //PVTCALC线程已经将数据补成了2^n，这里只需要发送就行

        //发送数据长度变了才重新设置长度
        //if (pOutputData->lastSendBytes != pOutputData->toSendBytes)    //先不做长度判断，会出差    NICK MARK
        {
            //FPGA_Set_W_R_Data_Length(FPGA_WRITE_ACTION, pOutputData->toSendBytes - 2);    //-2为逻辑要求 
            servFpgaDataLenSet(FPGAACT_WRITE, pOutputData->validBytes, pOutputData->toSendBytes);
            pOutputData->lastSendBytes = pOutputData->toSendBytes;
        }
        txState = bspSdioDataSend((u8 *)&pOutputData->writeAddr, pOutputData->toSendBytes, pOutputData->toSendBlkSize);

        //更新波表可写地址和大小
        pWaveTable->writableAddr += pOutputData->validPoints;
        pWaveTable->writableSize -= pOutputData->validPoints;

        //是否需要发送空数据
        if (pOutputData->bSendEmpty)
        {
            //发送数据长度变了才重新设置长度
            //if (pOutputData->lastSendBytes != pOutputData->toSendBytes)    //先不做长度判断，会出差    NICK MARK
            {
                //FPGA_Set_W_R_Data_Length(FPGA_WRITE_ACTION, pOutputData->emptyBytes - 2);    //-2为逻辑要求 
                servFpgaDataLenSet(FPGAACT_WRITE, pOutputData->validEmpBytes, pOutputData->emptyBytes);
                pOutputData->lastSendBytes = pOutputData->toSendBytes;
            }
            txState = bspSdioDataSend((u8 *)&pOutputData->emptyDataAddr, pOutputData->emptyBytes, pOutputData->emptyBlkSize);

            //更新波表可写地址和大小
            pWaveTable->writableAddr += pOutputData->validEmpPoints;
            pWaveTable->writableSize -= pOutputData->validEmpPoints;

            pOutputData->bSendEmpty  = false;
        }
    }
    
    //配置结束标志
    servFpgaDDRWriteSet(DDRWR_FINISH);

    //最后一帧数据
    /*if (pOutputData->bLastFrame)
    {
        if (WTWORKMODE_FIFO != pWaveTable->waveWorkMode)
        {
            pWaveTable->waveState = MTSTATE_CALCEND;
        }
    }*/

    return (writeResult | txState);
}


/*********************************************************************************************
函 数 名: servFpgaWavePrefetch
实现功能: 预取波表
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWavePrefetch(u8 chanNum)
{
    fpgaRegValue.register32[chanNum].regBitFiled.wavePrepare = BITOPT_ENABLE;

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);  
}


/*********************************************************************************************
函 数 名: servFpgaWaveEnable
实现功能: 使能输出波表
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveEnable(u8 chanNum, bool outputEnable)
{                                                
    fpgaRegValue.register32[chanNum].regBitFiled.wavePrepare = BITOPT_DISABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveOutput  = BITOPT_DISABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop    = BITOPT_DISABLE;
    
    if (outputEnable)
    {
        fpgaRegValue.register32[chanNum].regBitFiled.waveOutput = BITOPT_ENABLE;
    }
    else
    {
        fpgaRegValue.register32[chanNum].regBitFiled.waveOutput = BITOPT_DISABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaWaveStart
实现功能: 开始输出波表
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveStart(void)
{
    fpgaRegValue.register32[CH_SYS].regBitFiled.waveRunAll = BITOPT_ENABLE;

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[CH_SYS].regValue);
    
    fpgaRegValue.register32[CH_SYS].regBitFiled.waveRunAll = BITOPT_DISABLE;
}


/*********************************************************************************************
函 数 名: servFpgaWaveStop
实现功能: 设置停止输出波表
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveStop(u8 chanNum, StopConfigStruct stopConfigInfo)
{
    servFpgaStopDecelSet(chanNum, stopConfigInfo);    //临时设计，等FPGA保持设计好后再修改    NICK MARK

    //配置FPGA停止输出波表
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcSoft  = BITOPT_ENABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveOutput = BITOPT_DISABLE;    //清除启动位
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
    
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcSoft = BITOPT_DISABLE;    //停止命令发送后本地状态必须复位  防止下次误配
}


/*********************************************************************************************
函 数 名: servFpgaWaveEmergStop
实现功能: 设置紧急停止输出波表
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveEmergStop(u8 chanNum)
{
    StopConfigStruct stopConfigInfo = {.stopMode = STOPMODE_IMMED};

    
    //配置成立即停止
    servFpgaStopDecelSet(chanNum, stopConfigInfo);
    
    //配置FPGA停止输出波表
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcSoft  = BITOPT_ENABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveOutput = BITOPT_DISABLE;    //清除启动位
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
    
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcSoft = BITOPT_DISABLE;    //停止命令发送后本地状态必须复位  防止下次误配    
}


/*********************************************************************************************
函 数 名: servFpgaWaveStopConfig
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWaveStopConfig(StopConfigStruct *pWaveStopCofing, StopDecelerationStruct stopDecel, f32 posnToStep)
{
    u32 stopSteps;

    
    pWaveStopCofing->stopMode  = stopDecel.stopMode;
    if (STOPMODE_DIST == stopDecel.stopMode)
    {
        stopSteps = (u32)(stopDecel.distance * posnToStep);
        if (0 == stopSteps)
        {
            pWaveStopCofing->stopMode = STOPMODE_IMMED;
        }
        else
        {
            pWaveStopCofing->stopSteps = stopSteps;
        }
    }
    pWaveStopCofing->stopTime  = stopDecel.time;
    pWaveStopCofing->emergPeriod = EMERG_STOP_END_PERIOD;
}


/*********************************************************************************************
函 数 名: servFpgaWavePrepareConfig
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaWavePrepareConfig(u8 chanNum, WaveTableStruct *pOutpWaveTable, PlanManageStruct wavePlanInfo, PosnConvertInfoStruct posnConvertInfo)
{
    //更新下减速停止的设置参数(对外接口和FPGA接口的数据格式不一样，需要转换下)
    servFpgaWaveStopConfig(&(pOutpWaveTable->stopConfigInfo), wavePlanInfo.stopDecelInfo, posnConvertInfo.posnToStep);

    //配置结束状态和减速停止
    servFpgaEndStateSet(chanNum, wavePlanInfo.endState, pOutpWaveTable->stopConfigInfo);
    
    //配置波表结束中断
    servFpgaWaveEndIntSet(chanNum, pOutpWaveTable->bEndIntEnable);
    
    //LVT相关设置
    servFpgaWaveMotionModeSet(chanNum, 
                              wavePlanInfo.motionMode,
                              posnConvertInfo.lineSteps,
                              posnConvertInfo.lineMult,
                              wavePlanInfo.modifyDuty);

    //丢步告警
    servFpgaLineOutOfStepSet(chanNum, wavePlanInfo.outOfStepInfo.lineState,
                                      posnConvertInfo.lineSteps,
                                      wavePlanInfo.outOfStepInfo.lineOutNum);

    //预取之前将波表的循环数配置下去
    if (pOutpWaveTable->waveWorkMode != WTWORKMODE_FIFO)
    {
        servFpgaWaveDateCycleSet(chanNum, wavePlanInfo.cycleNum);
    }
}


/*********************************************************************************************
函 数 名: servFpgaReadyQuery
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
bool servFpgaReadyQuery(u8 chanNum, WaveTableStruct waveTableAddr)
{
    s16 state = 0;
    u32 times = 0;


    //通知逻辑停止输出波表
    servFpgaWaveBufferReset(chanNum);

    //配置波表起始地址和长度
    if (waveTableAddr.waveWorkMode != WTWORKMODE_FIFO)    
    {
        servFpgaWaveDateLenSet(chanNum, waveTableAddr.totalSize - waveTableAddr.writableSize);
    }
    /*else    //理论上FIFO模式也可以在这里才设置长度，目前先按旧流程，等测试过了后再修改    NICK MARK
    {
        servFpgaWaveDateLenSet(waveTableAddr.totalSize);
    }*/

    //设置波表起始地址
    servFpgaWaveStartDdrAddrSet(chanNum, waveTableAddr.startAddr);
    
    //配置逻辑开始预取数据
    servFpgaWavePrefetch(chanNum);
    
    //查询是否可以开始
    do
    {
        state = servFpgaWaveDataStateRead(chanNum); 
        times++;

        bspDelayMs(2);
        
    }while ((state != 0) && (times < FPGA_REG_QUERY_COUNT_MAX));
    
    if (state == 0)
    {
        //配置逻辑开始预取数据
        servFpgaWaveEnable(chanNum, true);
        
        return true;
    }
    else
    {
        return false;  
    }
}


#if QUBELEY_HARDVER_1
/*********************************************************************************************
函 数 名: servFpgaDrvBoardTypeGet
实现功能: 获取驱动板型号
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
DriverTypeEnum servFpgaDrvBoardTypeGet(u8 chanNum)
{

    u16 registerValue = 0;
    DriverTypeEnum drvType;

    servFpgaRegisterRead(CH_SYS, SERV_FPGA_STATE_REG, &registerValue);
    
    if ((registerValue & SERV_FPGA_DRIVE_TYPE_MASK) == SERV_FPGA_DRIVE_TYPE_262)
    {
        drvType = DRIVER_262; 
    }
    else if ((registerValue & SERV_FPGA_DRIVE_TYPE_MASK) == SERV_FPGA_DRIVE_TYPE_2660)
    {
        drvType =  DRIVER_2660; 
    }

    return drvType;      
}
#endif


/*********************************************************************************************
函 数 名: servFpgaReadyStateQuery
实现功能: 查询Fpga Ready状态
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
bool servFpgaReadyStateQuery(void)
{
    u16 writeValue = 0xA5;
    u16 readValue;


    servFpgaRegisterWrite(0, SERV_FPGA_DEBUG_REG, writeValue);    //通道号固定为0
    bspDelayUs(10);    //短暂延时下
    servFpgaRegisterRead(0, SERV_FPGA_DEBUG_REG, &readValue);    //通道号固定为0

    if (writeValue == readValue)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*********************************************************************************************
函 数 名: servFpgaEncoderEnable
实现功能: 使能编码器
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaEncoderSet(u8 chanNum, IntfcStateEnum encoderState, EncoderMultiEnum encMult, EncoderChanEnum  encChan)
{
    bool bEncMult = true;
    

    if (chanNum < ENCODER_SUPPORT_NUM)
    {
        switch (encoderState)
        {
            case INTFC_NONE:
                fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_REF_SELECT_MASK;
                fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH8_NO_ENC;

                bEncMult = false;
              break;

            case INTFC_OFF:
                bspEncoderTimerInit(chanNum, encMult, DISABLE);
                bEncMult = false;
              break;
              
            case INTFC_ON:

#ifdef PROJECT_GELGOOG

                fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_REF_SELECT_MASK;
                if (ECCHAN_3 == encChan)
                {
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH4_ENC_TRIPLE;
                }
                else
                {
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH4_ENC_SINGLE;
                }            
              
#endif
                bspEncoderTimerInit(chanNum, encMult, ENABLE);

              break;

            default:
              break;
        }
        servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);


        if (bEncMult)
        {
            //设置编码器倍乘
            switch (encMult)
            {
                case ENCMULT_SINGLE:
                    fpgaRegValue.register39[chanNum].regBitFiled.encMultSelect = EMSEL_NONE;
                  break;

                case ENCMULT_DOUBLE:
                    fpgaRegValue.register39[chanNum].regBitFiled.encMultSelect = EMSEL_DOUBLE;
                  break;

                case ENCMULT_QUADR:
                    fpgaRegValue.register39[chanNum].regBitFiled.encMultSelect = EMSEL_QUADR;
                  break;

                default:
                  break;
            }

            //设置通道数
            switch (encChan)
            {
                case ECCHAN_3:
                    fpgaRegValue.register39[chanNum].regBitFiled.encSingleSel = BITOPT_DISABLE;
                  break;
            
                case ECCHAN_1:
                    fpgaRegValue.register39[chanNum].regBitFiled.encSingleSel = BITOPT_ENABLE;
                  break;

                default:
                  break;
            }
            
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_39, fpgaRegValue.register39[chanNum].regValue);
        }
    }
}


/*********************************************************************************************
函 数 名: servFpgaEncoderCountReset
实现功能: 复位编码器计数
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaEncoderCountReset(u8 chanNum)
{
#if FPGA_ARM_COUNT_ENCODER
    bspEncoderCountSet(chanNum, 0);
    
#else

    fpgaRegValue.encMultandCountReg[chanNum] &= ~SERV_FPGA_ENC_COUNT_MASK;
    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_COUNT_RESET;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_39, fpgaRegValue.register39[chanNum].regValue);
#endif
}


/*********************************************************************************************
函 数 名: servFpgaEncoderCountRead
实现功能: 复位编码器计数
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaEncoderCountRead(u8 chanNum, s16 *pCircleNum, s32 *pABCount, s32 *pSteps)
{
#if FPGA_ARM_COUNT_ENCODER
    //目前只支持abCount和circleNum
    if (pCircleNum != NULL)
    {
        *pCircleNum = bspEncoderCircleCountGet(chanNum);
    }
    
    if (pABCount != NULL)
    {
        *pABCount = bspEncoderABCountGet(chanNum);
    }
    
#else

    u16 abCntValue;
    u16 stepsValue;
    u32 abCount;
    u32 steps;

    //为了锁存下面两个寄存器的计数值，先读下SERV_FPGA_ENC_STEPS_L_REG
    servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_STEPS_L_REG, &stepsValue);
    
    if (pCircleNum != NULL)
    {
        servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_CIRCLE_NUM_REG, (u16 *)pCircleNum);
    }

    if (pABCount != NULL)
    {        
        servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_ABCNT_L_REG, &abCntValue);
        abCount = abCntValue;
        servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_ABCNT_H_REG, &abCntValue);

#if 1
        if (abCntValue > 0)    //负数
        {
            abCount |= (u32)(abCntValue & SERV_FPGA_ENC_AB_CNT_H_MASK) << SERV_FPGA_ENC_COUNT_H_LSHIFT;
            *pABCount = (s32)abCount - SERV_FPGA_ABCNT_RCOUNT_MIN;
        }
        else
        {
            *pABCount = abCount;
        }
#else
        abCount |= (u32)(abCntValue & SERV_FPGA_ENC_AB_CNT_H_MASK) << SERV_FPGA_ENC_COUNT_H_LSHIFT;
        
        if (abCount > SERV_FPGA_ABCNT_FCOUNT_MAX)
        {
            *pABCount = (s32)abCount - SERV_FPGA_ABCNT_RCOUNT_MIN;
        }
        else
        {
            *pABCount = abCount;
        }
#endif
    }

    if (pSteps != NULL)
    {
        steps = stepsValue;
        servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_STEPS_H_REG, &stepsValue);
        steps |= (u32)stepsValue << SERV_FPGA_ENC_COUNT_H_LSHIFT;
        if (steps > SERV_FPGA_STEPS_FCOUNT_MAX)
        {
            *pSteps = (s32)steps - SERV_FPGA_STEPS_RCOUNT_MIN;
        }
        else
        {
            *pSteps = steps;
        }
    }
#endif
}


/*********************************************************************************************
函 数 名: servFpgaLineOutOfStepSet
实现功能: 设置线间失步阈值
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.08.12 Add
*********************************************************************************************/
void servFpgaLineOutOfStepSet(u8 chanNum, SensorStateEnum state, f32 lineSteps, u16 threshold)
{
    //u16 thresholdL;
    u16 thresholdH;


    if (SENSOR_ON == state)
    {
        fpgaRegValue.register50[chanNum].regBitFiled.outOfStep = INT_ENABLE;

        //thresholdL = (u16)(lineSteps - threshold);
        thresholdH = (u16)(lineSteps + threshold);
        
        //目前不处理超步，下限先配成0
        //servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_57, thresholdL);
        servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_57, 0);
        
        servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_58, thresholdH);
    }
    else
    {
        fpgaRegValue.register50[chanNum].regBitFiled.outOfStep = INT_DISABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaSdioCrcErrorIntSet
实现功能: 设置SDIO CRC错误中断
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2018.05.17 Add
*********************************************************************************************/
void servFpgaSdioCrcErrorIntSet(SensorStateEnum state)
{
    if (SENSOR_ON == state)
    {
        fpgaRegValue.register50[CH_SYS].regBitFiled.sdioCrcErr = INT_ENABLE;
    }
    else
    {
        fpgaRegValue.register50[CH_SYS].regBitFiled.sdioCrcErr = INT_DISABLE;
    }

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[CH_SYS].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaDriverFromArm
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDriverFromArm(u8 chanNum)
{
    fpgaRegValue.register16.regBitFiled.drvSrc = DRIVER_ARM;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);    //通道号固定为0
}


/*********************************************************************************************
函 数 名: servFpgaDriverSelect
实现功能: 选择要配置的驱动
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDriverSelect(u8 chanNum)
{
#if GELGOOG_AXIS_4 || GELGOOG_SINANJU
    fpgaRegValue.register16.regBitFiled.drvSpiSelect = chanNum;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
#endif
}


/*********************************************************************************************
函 数 名: servFpgaDriverConfig;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 servFpgaDriverConfig(u8 regNum, u32 regVaule, DrvConfActionEnum drvConfAct)
{
    fpgaRegValue.register17.regValue = (u16)regVaule;    //低16bit
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_17, fpgaRegValue.register17.regValue);
    
    fpgaRegValue.register18.regBitFiled.drvData = (u16)(regVaule >> 16);    //高4bit
    fpgaRegValue.register18.regBitFiled.drvRead = drvConfAct;
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_18, fpgaRegValue.register18.regValue);
    
    return 0;
}


/*********************************************************************************************
函 数 名: servFpgaStartSourceSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaStartSourceSet(u8 chanNum, StartSourceEnum startSource)
{
    fpgaRegValue.register31[chanNum].regBitFiled.startSrcSoft = BITOPT_DISABLE;
    fpgaRegValue.register31[chanNum].regBitFiled.startSrcHard = BITOPT_DISABLE;
    fpgaRegValue.register31[chanNum].regBitFiled.startSrcCan  = BITOPT_DISABLE;

    switch (startSource)
    {
        case SSRC_SOFT:
        default:
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcSoft = BITOPT_ENABLE;
          break;

        case SSRC_HARD:
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcHard = BITOPT_ENABLE;
          break;

        case SSRC_CAN:
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcCan = BITOPT_ENABLE;
          break;

        case SSRC_ALL:
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcSoft = BITOPT_ENABLE;
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcHard = BITOPT_ENABLE;
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcCan  = BITOPT_ENABLE;
          break;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[chanNum].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaClockSyncSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaClockSyncSet(ClockSyncRegEnum ClockSyncReg, ReceiveTypeEnum receiveType, CanIntfcStruct canIntfc)
{
    u32 canId = 0;


    fpgaRegValue.register31[CH_SYS].regBitFiled.startSrcCan = BITOPT_DISABLE;

    switch (ClockSyncReg)
    {
        case CLOCKREG_ON:
            fpgaRegValue.register31[CH_SYS].regBitFiled.clkSynState = CSSTATE_RESET;
            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[CH_SYS].regValue);

            fpgaRegValue.register31[CH_SYS].regBitFiled.clkSynState = CSSTATE_ON;
            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[CH_SYS].regValue);
          break;

        case CLOCKREG_OFF:
            fpgaRegValue.register31[CH_SYS].regBitFiled.clkSynState = CSSTATE_OFF;
            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[CH_SYS].regValue);
          break;
          
        default:
          break;
    }

    switch (receiveType)
    {
        case RECEIVE_RADIOID:
            canId = canIntfc.radioId;
          break;
          
        case RECEIVE_GROUPID1:
            canId = canIntfc.groupId1;
          break;
          
        case RECEIVE_GROUPID2:
            canId = canIntfc.groupId2;
          break;
          
        case RECEIVE_RECEIVEID:
            canId = canIntfc.receiveId;
          break;

        default:
          break;
    }

    if (CLOCKREG_ON == ClockSyncReg)
    {
        servCanBuildFrameSendClockSyncStartFrame2Fpga(canId, canIntfc.idType, canIntfc.baud);
    }
    else
    {
        servCanBuildFrameSendClockSyncEndFrame2Fpga(canId, canIntfc.idType, canIntfc.baud);
    }
}


/*********************************************************************************************
函 数 名: servFpgaClockSyncCountRead
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
u32 servFpgaClockSyncCountRead(void)
{
    u32 count = 0;
    u16 state = 0;


    servFpgaRegisterRead(CH_SYS, SERV_FPGA_CLOCK_SYNC_COUNT_H_REG, &state);

    if (state & SERV_FPGA_CLOCK_SYNC_COUNT_INVAILD)
    {
        count = 0;
    }
    else
    {
        //只取32位先

        //中16位
        servFpgaRegisterRead(CH_SYS, SERV_FPGA_CLOCK_SYNC_COUNT_M_REG, &state);
        count = ((u32)state) << 16;

        //低16位
        servFpgaRegisterRead(CH_SYS, SERV_FPGA_CLOCK_SYNC_COUNT_L_REG, &state);
        count += state;
    }

    return count;
}


/*********************************************************************************************
函 数 名: servFpgaMotionRegSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaMotionRegSet(RegisterFuncEnum regFunc, ReceiveTypeEnum receiveType, CanIntfcStruct canIntfc)
{
    switch (regFunc)
    {
        case REGISTER_NONE:
          break;

        case REGISTER_SOFTWARE:
        case REGISTER_DIOSTART:
        case REGISTER_CANSTART:
          break;

        case REGISTER_SYNCSTART:
        case REGISTER_SYNCEND:
          break;

        default:
          break;
    }
}


/*********************************************************************************************
函 数 名: servFpgaDDRStateGet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
DdrStateEnum servFpgaDDRStateGet(void)
{
    FpgaResponseReg10Union responseReg;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_10, &responseReg.regValue);

    if (responseReg.regBitFiled.ddrInitDone)
    {
        if (responseReg.regBitFiled.ddrCalibError)
        {
            return DDR_TESTERROR;
        }
        else
        {
            return DDR_NOERROR;
        }
    }
    else
    {
        return DDR_NOINIT;
    }
}


/*********************************************************************************************
函 数 名: servFpgaDDRErrorClear
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaDDRErrorClear(void)
{
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DDR_TEST_REG, SERV_FPGA_DDR_TEST_CLEAR);
}


/*********************************************************************************************
函 数 名: servFpgaUartSelect
实现功能: 使能编码器
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaSensorUart1RxSelect(SensorNumEnum sensorNum)
{
#ifdef PROJECT_GELGOOG
    fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_SELECT_MASK;

    
    switch (sensorNum)
    {
        case SENSOR_S1:
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_SELECT_SEN1;
          break;

        case SENSOR_S2:
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_SELECT_SEN2;
          break;

        case SENSOR_S3:
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_SELECT_SEN3;
          break;

        case SENSOR_S4:
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_SELECT_SEN4;
          break;

        default:
          break;
    }
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
#endif
}


/*********************************************************************************************
函 数 名: servFpgaSensorUart1TxSelect
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaSensorUart1TxSelect(SensorNumEnum sensorNum)
{
#ifdef PROJECT_GELGOOG

    switch (sensorNum)
    {
        case SENSOR_S1:
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN1;
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN1_TX;
          break;

        case SENSOR_S2:
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN2;
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN2_TX;
          break;

        case SENSOR_S3:
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN3;
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN3_TX;
          break;

        case SENSOR_S4:
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN4;
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN4_TX;
          break;

        default:
          break;
    }
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
    
#endif
}


/*********************************************************************************************
函 数 名: servFpgaUart4RxEnable
实现功能: 使能编码器
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.11.02 Add
*********************************************************************************************/
void servFpgaSensorUart2RxEnable(bool enable)
{
#ifdef PROJECT_GELGOOG
    static u16 regValue = 0;
    static bool record = false;


    if (enable)
    {
        regValue = fpgaRegValue.trigRelatRefCfgReg[CH4];
        record   = true;
        
        fpgaRegValue.trigRelatRefCfgReg[CH4] &= ~SERV_FPGA_REF_SELECT_MASK;
        fpgaRegValue.trigRelatRefCfgReg[CH4] |= SERV_FPGA_REF_CH4_ENC_SENSOR;
    }
    else
    {
        if (record)
        {
            fpgaRegValue.trigRelatRefCfgReg[CH4] = regValue;
        }
    }
    
    
    servFpgaRegisterWrite(CH4, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[CH4]);
#endif
}


/*********************************************************************************************
函 数 名: servFpgaUart4RxSelect
实现功能: 使能编码器
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.11.02 Add
*********************************************************************************************/
void servFpgaSensorUart2RxSelect(SensorNumEnum sensorNum)
{
#ifdef PROJECT_GELGOOG

    fpgaRegValue.sensorUart2SelectReg &= ~SERV_FPGA_UART4_SELECT_MASK;
    
    //4轴驱控器中，FPGA在定义通道时，定义的对应关系是POS13、14、15、16分别对应通道CH4、CH1、CH2、CH3
    switch (sensorNum)
    {
        case SENSOR_S1:
            fpgaRegValue.sensorUart2SelectReg |= SERV_FPGA_UART4_SELECT_SEN1;
          break;

        case SENSOR_S2:
            fpgaRegValue.sensorUart2SelectReg |= SERV_FPGA_UART4_SELECT_SEN2;
          break;

        case SENSOR_S3:
            fpgaRegValue.sensorUart2SelectReg |= SERV_FPGA_UART4_SELECT_SEN3;
          break;

        case SENSOR_S4:
            fpgaRegValue.sensorUart2SelectReg |= SERV_FPGA_UART4_SELECT_SEN4;
          break;

        default:
          break;
    }
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_UART4_RX_REG, fpgaRegValue.sensorUart2SelectReg);
#endif
}


/*********************************************************************************************
函 数 名: servFpgaReverseMotionSet
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servFpgaReverseMotionSet(SensorStateEnum  revMotion)
{
    switch (revMotion)
    {
        case SENSOR_OFF:
        default:          
            fpgaRegValue.register31[CH_SYS].regBitFiled.dirReverse = BITOPT_DISABLE;
          break;

        case SENSOR_ON:          
            fpgaRegValue.register31[CH_SYS].regBitFiled.dirReverse = BITOPT_ENABLE;
          break;
    }

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[CH_SYS].regValue);
}


/*********************************************************************************************
函 数 名: servFpgaDioRefStatusRead
实现功能: 读取限位开关状态
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2018.03.10 Add
*********************************************************************************************/
u16 servFpgaDioRefStatusRead(void)
{
    u16 registerValue;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_07, &registerValue);

    return registerValue;
}


/*********************************************************************************************
函 数 名: servFpgaDioPortStatusRead
实现功能: 读取数字端口状态
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2018.03.10 Add
*********************************************************************************************/
u16 servFpgaDioPortStatusRead(void)
{
    u16 registerValue;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_DIO_STATUS_REG, &registerValue);

    return registerValue;
}


#if GELGOOG_AXIS_10

/*********************************************************************************************
函 数 名: servFpgaDriverResetSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDriverResetSet(DriverResetEnum drvReset)
{
    fpgaRegValue.register16.regBitFiled.drvReset = drvReset;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}

/*********************************************************************************************
函 数 名: servFpgaDriverModeSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDriverModeSet(MicroStepEnum microStep)
{
    DriverModeEnum drvMode;
    

    switch (microStep)
    {
        case MICROSTEP_256:
            drvMode = DRIVER_MODE256;
          break;

        case MICROSTEP_128:
            drvMode = DRIVER_MODE128;
          break;

        case MICROSTEP_32:
            drvMode = DRIVER_MODE32;
          break;

        case MICROSTEP_16:
            drvMode = DRIVER_MODE16;
          break;

        case MICROSTEP_8:
            drvMode = DRIVER_MODE8;
          break;

        case MICROSTEP_4:
            drvMode = DRIVER_MODE4;
          break;

        case MICROSTEP_2:
            drvMode = DRIVER_MODE2;
          break;

        case MICROSTEP_1:
            drvMode = DRIVER_MODE1;
          break;

        default:
          break;
    }
    
    fpgaRegValue.register16.regBitFiled.drvMode = drvMode;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}

/*********************************************************************************************
函 数 名: servFpgaDriverDecaySet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDriverDecaySet(DriverDecayEnum drvDecay)
{
    fpgaRegValue.register16.regBitFiled.drvDecay = drvDecay;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}


/*********************************************************************************************
函 数 名: servFpgaDriverSourceSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDriverSourceSet(DriverSourceEnum drvSrc)
{
    fpgaRegValue.register16.regBitFiled.drvSrc = drvSrc;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}


/*********************************************************************************************
函 数 名: servFpgaDriverEnable
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDriverEnable(u8 chanNum, SensorStateEnum  state)
{
    bool bConfigReg = true;

    
    switch (chanNum)
    {
        case 0:
            fpgaRegValue.register17.regBitFiled.drvEnable0 = state;
          break;

        case 1:
            fpgaRegValue.register17.regBitFiled.drvEnable1 = state;
          break;

        case 2:
            fpgaRegValue.register17.regBitFiled.drvEnable2 = state;
          break;

        case 3:
            fpgaRegValue.register17.regBitFiled.drvEnable3 = state;
          break;

        case 4:
            fpgaRegValue.register17.regBitFiled.drvEnable4 = state;
          break;

        case 5:
            fpgaRegValue.register17.regBitFiled.drvEnable5 = state;
          break;

        case 6:
            fpgaRegValue.register17.regBitFiled.drvEnable6 = state;
          break;

        case 7:
            fpgaRegValue.register17.regBitFiled.drvEnable7 = state;
          break;

        case 8:
            fpgaRegValue.register17.regBitFiled.drvEnable8 = state;
          break;

        case 9:
            fpgaRegValue.register17.regBitFiled.drvEnable9 = state;
          break;

        default:
            bConfigReg = false;
          break;
    }

    if (bConfigReg)
    {
        servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_17, fpgaRegValue.register17.regValue);
    }
}


/*********************************************************************************************
函 数 名: servFpgaDriverFaultRead
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
u16 servFpgaDriverFaultRead(void)
{
    u16 regValue;
    

    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_12, &regValue);

    return regValue;
}


#else


/*********************************************************************************************
函 数 名: servFpgaDriverClkCofing
实现功能: 通过FPGA配置驱动时钟
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaDriverClkCofing(u8 chanNum, DriverClkStateEnum driverClkState)
{
    if (CH1 == chanNum)
    {
        fpgaRegValue.register16.regBitFiled.drvSysClk1 = driverClkState;
    }
#ifdef PROJECT_GELGOOG

    else
    {
        switch (chanNum)
        {
            case CH2:
                fpgaRegValue.register16.regBitFiled.drvSysClk2 = driverClkState;
              break;
              
            case CH3:
                fpgaRegValue.register16.regBitFiled.drvSysClk3 = driverClkState;
              break;
              
            case CH4:
                fpgaRegValue.register16.regBitFiled.drvSysClk4 = driverClkState;
              break;

#if GELGOOG_SINANJU             
            case CH5:
                fpgaRegValue.register16.regBitFiled.drvSysClk5 = driverClkState;
              break;
#endif

#if GELGOOG_AXIS_8              
            case CH6:
                fpgaRegValue.register16.regBitFiled.drvSysClk6 = driverClkState;
              break;
              
            case CH7:
                fpgaRegValue.register16.regBitFiled.drvSysClk7 = driverClkState;
              break;
              
            case CH8:
                fpgaRegValue.register16.regBitFiled.drvSysClk8 = driverClkState;
              break;
#endif

            default:
              break;
        }
    }
    
#endif

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}
#endif


/*********************************************************************************************
函 数 名: servFpgaBackLashReset
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaBackLashReset(BackLashChanEnum blChan)
{
    fpgaRegValue.register28.regBitFiled.chSelect = blChan;
    fpgaRegValue.register28.regBitFiled.blReset  = BLRESET_ON;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_28, fpgaRegValue.register28.regValue);
}

/*********************************************************************************************
函 数 名: servFpgaBackLashReset
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
u16 servFpgaBackLashRead(BackLashChanEnum blChan)
{
    u16 backLash;

    
    servFpgaRegisterRead(blChan, SERV_FPGA_RESPONSE_REG_08, &backLash);

    return backLash;
}


#if  GELGOOG_SINANJU
/*********************************************************************************************
函 数 名: servFpgaAbsEncReadTrig
实现功能: 触发绝对值编码器的回读
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2018.02.28 Add
*********************************************************************************************/
void servFpgaAbsEncReadTrig(SensorNumEnum sensorNum)
{  
    servFpgaRegisterWrite(sensorNum, SERV_FPGA_ABS_ENC_REDA_REG, SERV_FPGA_ABS_ENC_REDA_CMD);
}


/*********************************************************************************************
函 数 名: servFpgaPdmSampleReset
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaPdmSampleReset(SampleChanEnum sampleChan)
{
    fpgaRegValue.register29.regBitFiled.chSelect    = sampleChan;
    fpgaRegValue.register29.regBitFiled.sampleState = SAMPLESTATE_RESET;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
}

/*********************************************************************************************
函 数 名: servFpgaPdmSampleStart
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaPdmSampleStart(SampleChanEnum sampleChan)
{
    fpgaRegValue.register29.regBitFiled.chSelect    = sampleChan;
    fpgaRegValue.register29.regBitFiled.sampleState = SAMPLESTATE_START;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
}

/*********************************************************************************************
函 数 名: servFpgaPdmSampleEnd
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaPdmSampleEnd(SampleChanEnum sampleChan)
{
    fpgaRegValue.register29.regBitFiled.chSelect    = sampleChan;
    fpgaRegValue.register29.regBitFiled.sampleState = SAMPLESTATE_END;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
}

/*********************************************************************************************
函 数 名: servFpgaPdmEncDivSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaPdmEncDivSet(SampleChanEnum sampleChan, u8 encDiv)
{
    fpgaRegValue.register29.regBitFiled.chSelect = sampleChan;
    fpgaRegValue.register29.regBitFiled.encDiv   = encDiv - 1;    //FPGA要求减1
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
}

/*********************************************************************************************
函 数 名: servFpgaPdmSampleStateSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaPdmSampleStateSet(SampleChanEnum sampleChan, PdmInfoStruct pdmInfo)
{
    if (SENSOR_ON == pdmInfo.sampleState)
    {
        servFpgaPdmSampleReset(sampleChan);

        servFpgaPdmEncDivSet(sampleChan, pdmInfo.encDiv);

        servFpgaPdmSampleStart(sampleChan);
    }
    else
    {
        servFpgaPdmSampleEnd(sampleChan);
    }
}


/*********************************************************************************************
函 数 名: servFpgaPdmMstepCountRead
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
u16 servFpgaPdmMstepCountRead(SampleChanEnum sampleChan)
{
    u16 regValue;
        
    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_09, &regValue);

    return (regValue + SERV_FPGA_MSTEP_COUNT_OFFSET);
}


/*********************************************************************************************
函 数 名: servFpgaPdmMstepDataRead
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
u8 servFpgaPdmMstepDataRead(SampleChanEnum sampleChan, u8 *pRegValue, u16 bufferSize)
{
    u8  rxState = 0;
    u16 regLen = FPGA_ADD_LEN_2BYTE + bufferSize;
    u16 tempReg;
    u32 timeout = 33600;    //根据DELAY_COUNT_MS = 33600，此超时时间大约为1ms;


    if (pRegValue != NULL)
    {
        //启动SDIO接收
        bspSdioDataReceiveSet(pRegValue, regLen, SDIO_BLK_512_Byte);
        
        //设置数据长度
        servFpgaDataLenSet(FPGAACT_READ, regLen, regLen);

        //设置读取的逻辑寄存器地址
        tempReg = (SERV_FPGA_RESPONSE_REG_11 & FPGA_RREG_ADDR_MARK) | 
                  ((sampleChan << FPGA_RCHAN_NUM_LSHIFT) & FPGA_RCHAN_NUM_MARK);
        servFpgaReadAddrSet(tempReg);

        //做成超时等待，同时考虑释放CPU使用权
        while ((!bspSdioDataTxComplete()) && (timeout > 0))
        {
            timeout--;  
        }
    }
  
    return rxState;
}


/*********************************************************************************************
函 数 名: servFpgaPdmMstepDataProcess
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaPdmMstepDataProcess(SampleChanEnum sampleChan, PdmInfoStruct pdmInfo)
{
    u8  data[FPGA_PDM_MSTEP_COUNT_LEN];
    bool bRemainder = false;
    bool bRemSend   = false;
    u16 i, j, k;
    u16 readCount;
    u16 sendCount;
    u8 *dataBuffer;
    u16 mstepCount = pdmInfo.readLen * sizeof(u16);                     //长度指的是U16的长度
    u16 dataLen = PDM_MSTEP_BUFFER_SIZE - FPGA_ADD_LEN_2BYTE - FPGA_PDM_MSTEP_BUFFER_RESV;    //数据不包含寄存器地址和保留部分


    //需要读取的次数
    readCount = mstepCount / dataLen;
    if ((mstepCount % dataLen) != 0)
    {
        bRemainder = true;
    }

    //一次读取后需要发送的次数
    sendCount = dataLen / FPGA_PDM_MSTEP_COUNT_LEN;
    if ((dataLen % FPGA_PDM_MSTEP_COUNT_LEN) != 0)
    {
        sendCount++;
    }

    //开始读取并发送
    for (i = 0;i < readCount;i++)
    {
        //数据读出来
        servFpgaPdmMstepDataRead(sampleChan, pdmInfo.mstepData, dataLen);

        //发送，其实是压入发送Buffer中
        dataBuffer = (u8 *)pdmInfo.mstepData + FPGA_ADD_LEN_2BYTE;      //跳过寄存器地址
        for (j = 0;j < sendCount;j++)
        {
            for (k = 0;k < FPGA_PDM_MSTEP_COUNT_LEN;k++)
            {
                data[k] = *dataBuffer++;
            }
            //cmdFrameSend(CMD_PDM, PDMCMD_MSTEPDATAQ, FPGA_PDM_MSTEP_COUNT_LEN, data);
            servFrameSend(data[0], data[1], FPGA_PDM_MSTEP_COUNT_LEN - 2, &data[2], g_commIntfc.linkType);
        }

        //给CMDPARSE线程发信号量，完成发送
        OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, NULL);

        mstepCount -= dataLen;
    }
    
    //处理剩余部分
    if (bRemainder)
    {
        //一次读取后需要发送的次数
        sendCount = mstepCount / FPGA_PDM_MSTEP_COUNT_LEN;
        if ((mstepCount % FPGA_PDM_MSTEP_COUNT_LEN) != 0)
        {
            bRemSend = true;
        }
        
        //数据读出来，然后一点一点发送上去
        servFpgaPdmMstepDataRead(sampleChan, pdmInfo.mstepData, mstepCount);

        //发送，其实是压入发送Buffer中
        dataBuffer = (u8 *)pdmInfo.mstepData + FPGA_ADD_LEN_2BYTE;      //跳过寄存器地址
        for (j = 0;j < sendCount;j++)
        {
            for (k = 0;k < FPGA_PDM_MSTEP_COUNT_LEN;k++)
            {
                data[k] = *dataBuffer++;
            }
            //cmdFrameSend(CMD_PDM, PDMCMD_MSTEPDATAQ, FPGA_PDM_MSTEP_COUNT_LEN, data);
            servFrameSend(data[0], data[1], FPGA_PDM_MSTEP_COUNT_LEN - 2, &data[2], g_commIntfc.linkType);

            mstepCount -= FPGA_PDM_MSTEP_COUNT_LEN;
        }

        //发送剩余的
        if (bRemSend)
        {
            //最短长度
            if (mstepCount < 2)
            {
                mstepCount = 2;
            }
            
            for (k = 0;k < mstepCount;k++)
            {
                data[k] = *dataBuffer++;
            }
            //cmdFrameSend(CMD_PDM, PDMCMD_MSTEPDATAQ, FPGA_PDM_MSTEP_COUNT_LEN, data);
            servFrameSend(data[0], data[1], mstepCount - 2, &data[2], g_commIntfc.linkType);
        }

        //给CMDPARSE线程发信号量，完成发送
        OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, NULL);
    }
    
}


/*********************************************************************************************
函 数 名: servFpgaPdmCacheTestSet
实现功能: 
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
void servFpgaPdmCacheTestSet(void)
{
    fpgaRegValue.register29.regBitFiled.cacheMode = BITOPT_ENABLE;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
    
    //fpgaRegValue.register29.regBitFiled.cacheMode = BITOPT_DISABLE;
}
#endif


/*********************************************************************************************
函 数 名: servFpgaWaveEndIntClear
实现功能: 清除波表结束标记
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaWaveEndIntClear(u8 chanNum)
{
    fpgaRegValue.register50[chanNum].regBitFiled.waveEndClear = INT_ENABLE;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
    
    fpgaRegValue.register50[chanNum].regBitFiled.waveEndClear = INT_DISABLE;
}

/*********************************************************************************************
函 数 名: servFpgaPeriodOverTxClear
实现功能: 清除传输导致的微步周期超时标记
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaPeriodOverTxClear(void)
{
    fpgaRegValue.register50[CH_SYS].regBitFiled.pOvrTxClear  = INT_ENABLE;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[CH_SYS].regValue);
    
    fpgaRegValue.register50[CH_SYS].regBitFiled.pOvrTxClear  = INT_DISABLE;
}

/*********************************************************************************************
函 数 名: servFpgaPeriodOverDdrClear
实现功能: 清除DDR读取导致的微步周期超时标记
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaPeriodOverDdrClear(u8 chanNum)
{
    fpgaRegValue.register50[chanNum].regBitFiled.pOvrDdrClear = INT_ENABLE;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
    
    fpgaRegValue.register50[chanNum].regBitFiled.pOvrDdrClear = INT_DISABLE;
}


/*********************************************************************************************
函 数 名: servFpgaPeriodStateRead
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
u8 servFpgaPeriodStateRead(u8 chanNum)
{
    u8 periodState;
    FpgaResponseReg10Union responseReg;

    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_10, &responseReg.regValue);


    periodState = responseReg.regBitFiled.periodOverTx       |
                  responseReg.regBitFiled.periodUnderTx  * 2 |
                  responseReg.regBitFiled.periodOverDdr  * 4 |
                  responseReg.regBitFiled.periodUnderDdr * 8;

    return periodState;
}



/*******************************************文件尾********************************************/
