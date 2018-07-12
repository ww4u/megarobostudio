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



/****************************************外部变量声明*****************************************/
extern MotionInfoStruct  g_motionInfo;



/*****************************************局部宏定义******************************************/
#define    FPGA_ADD_LEN_2BYTE      2       //配置寄存器时 数据前加的地址长度为 2字节
#define    FPGA_REG_LEN_BYTE       2       //寄存器长度

#define    FPGA_RREG_ADDR_MARK     0x1F    //读寄存器地址掩码 bit[0:4]
#define    FPGA_RCHAN_NUM_MARK     0x1E0   //读通道号掩码 bit[5:8]
#define    FPGA_RCHAN_NUM_LSHIFT   5       //读通道号掩码 bit[5:8]

#define    FPGA_WREG_ADDR_MARK     0x3F    //写寄存器地址掩码 bit[0:5]
#define    FPGA_WCHAN_NUM_MARK     0x07    //写通道号掩码 bit[8:10]，第二个Byte

#define    FPGA_CFG_BUFF_SIZE      2048

#define    FPGA_ARM_COUNT_ENCODER  1       //编码器计数由ARM计数

#define    FPGA_DDR_WAVE_TABLE_POINT_LEN    4    //Bytes



/*************************************局部常量和类型定义**************************************/
typedef enum 
{
    FPGAACT_READ = 0,
    FPGAACT_WRITE
    
}FpgaActionEnum;

typedef struct
{
    u16 driverCtrlReg;                 //驱动控制寄存器 16#
    
    u16 ultrasonicCtrlReg;             //超声控制寄存器 19#

    u16 waveTableModeReg[CH_TOTAL];    //波表模式寄存器 31#，包括启动源和时钟校正
    u16 waveTableCtrlReg[CH_TOTAL];    //波表控制寄存器 32#

    u16 encMultandCountReg[CH_TOTAL];  //编码器倍乘和计数清零寄存器 39#
    
    u16 digitalOutputReg;              //DO配置寄存器 45#
    u16 encandUartReg;                 //编码器和UART配置寄存器 46#
    
    u16 trigRelatRefCfgReg[CH_TOTAL];  //触发逻辑关系和REF配置寄存器 49#
    
    u16 interruptSrcReg[CH_TOTAL];     //中断源寄存器 50#
    
    u16 sensorUart2SelectReg;          //Uart选择寄存器 51#
    
    
}FpgaWriteableRegValueStruct;



/******************************************局部变量*******************************************/
FpgaWriteableRegValueStruct fpgaRegValue = {0};



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servFpgaDataLenSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
static void servFpgaDataLenSet(FpgaActionEnum fpgaAction, u16 realLen, u16 dataLen)
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
    u8 regLen = FPGA_ADD_LEN_2BYTE + FPGA_REG_LEN_BYTE;
    u8 regData[32];
    

    //设置数据长度
    servFpgaDataLenSet(FPGAACT_WRITE, regLen, regLen);
    
    regData[0] = regAddr & FPGA_WREG_ADDR_MARK;
    regData[1] = chanNum & FPGA_WCHAN_NUM_MARK;
    regData[2] = (u8)(regValue & 0xFF);    //低8位
    regData[3] = (u8)((regValue & 0xFF00) >> 8);    //高8位

    //发送数据
    bspSdioDataSend(regData, regLen, SDIO_BLK_4_Byte);

    return 0;
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
    u16 tempReg;
    

    if (pRegValue != NULL)
    {
        //启动DMA
        bspSdioDataReceive(regValue, regLen, SDIO_BLK_4_Byte);
        
        //设置数据长度
        servFpgaDataLenSet(FPGAACT_READ, regLen, regLen);

        //设置读取的逻辑寄存器地址
        tempReg = (regAddr & FPGA_RREG_ADDR_MARK) | ((chanNum << FPGA_RCHAN_NUM_LSHIFT) & FPGA_RCHAN_NUM_MARK);
        servFpgaReadAddrSet(tempReg);

        //前两个字节是地址
        *pRegValue  = regValue[2];         //低8位 
        *pRegValue |= regValue[3] << 8;    //高8位 
    }
  
    return 0;
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

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_VERSION_REG, &tmpReg);
    
    fpgaVersion.hard  = (u8)((tmpReg & SERV_FPGA_VERSION_HARD_MASK)  >> SERV_FPGA_VERSION_HARD_RSHIFT);
    fpgaVersion.major = (u8)((tmpReg & SERV_FPGA_VERSION_MAJOR_MASK) >> SERV_FPGA_VERSION_MAJOR_RSHIFT);
    fpgaVersion.minor = (u8)((tmpReg & SERV_FPGA_VERSION_MINOR_MASK) >> SERV_FPGA_VERSION_MINOR_RSHIFT);
    fpgaVersion.build = (u8)(tmpReg & SERV_FPGA_VERSION_BUILD_MASK);
    
    return fpgaVersion;
}



/*********************************************************************************************
函 数 名: servFPGA_STAT_Get
实现功能: 读取FPGA的状态
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
*********************************************************************************************/
s32 servFPGA_STAT_Get(u16* STAT)
{
    servFpgaRegisterRead(0, ADDR_FPGA_STAT, STAT);    //通道号固定为0

    return 0;
}

/*********************************************************************************************
函 数 名: servFPGA_Sonic_Reset
实现功能: 复位逻辑的超声测距功能
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 为了保证每次测距的准确性,必须执行该操作
*********************************************************************************************/
static void servFPGA_Sonic_Reset(void)
{
    fpgaRegValue.ultrasonicCtrlReg &= ~0x01;  //测距启动信号清空
    fpgaRegValue.ultrasonicCtrlReg |= 0x01<<1; //设置测距复位
    
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_CTRL, fpgaRegValue.ultrasonicCtrlReg);    //FOR MODIFY NICK
    
    fpgaRegValue.ultrasonicCtrlReg &= ~(0x01<<1); //清除测距复位  必须操作
}


/*********************************************************************************************
函 数 名: servFPGA_Sonic_Start
实现功能: 控制逻辑进行一次超声测距
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
*********************************************************************************************/
static void servFPGA_Sonic_Start(void)
{
    fpgaRegValue.ultrasonicCtrlReg &= ~(0x01<<1); //清除测距复位  必须操作
    fpgaRegValue.ultrasonicCtrlReg |= ~0x01;      //设置测距启动信号
    
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_CTRL, fpgaRegValue.ultrasonicCtrlReg);    //FOR MODIFY NICK
    
    fpgaRegValue.ultrasonicCtrlReg &= ~(0x01); //清除测距启动
}

/*********************************************************************************************
函 数 名: servFPGA_Sonic_Protect_Mode
实现功能: 配置逻辑在检测到碰撞危险时的操作
输入参数: 
          euSonicProtMode  mode:
                        SONIC_ARM_PROTECT = 0,    //逻辑检测到碰撞危险后不做任何动作
                        SONIC_FPGA_PROTECT = 1    //逻辑检测到碰撞危险后主动进行保护动作
输出参数:
            
返 回 值: 0
说    明: 
*********************************************************************************************/
void servFPGA_Sonic_Protect_Mode(euSonicProtMode mode)
{
    if(mode == SONIC_ARM_PROTECT)
    {
        fpgaRegValue.ultrasonicCtrlReg &= ~(0x01<<2);  //bit2 = 0 ARM保护
    }
    else
    {
        fpgaRegValue.ultrasonicCtrlReg |= (0x01<<2);  //bit2 = 1 FPGA保护
    }
        
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_CTRL, fpgaRegValue.ultrasonicCtrlReg);    //FOR MODIFY NICK
}


/*********************************************************************************************
函 数 名: servFPGA_Sonic_Pulse_Get
实现功能: 获取超声波模块的回响信号脉冲数
输入参数: 
          
输出参数:u32 * u32pulse:脉冲计数的返回地址
            
返 回 值: 0
说    明: 脉冲数是由逻辑返回的 低16和高4位 拼接得到
*********************************************************************************************/
static s32 servFPGA_Sonic_Pulse_Get(u8 chanNum, u32 * u32pulse)
{
    u16 pulse_L = 0;
    u16 pulse_H = 0;

    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_USONIC_DIS_L, &pulse_L);
    servFpgaRegisterRead(chanNum, ADDR_FPGA_USONIC_DIS_H, &pulse_H);
    
    *u32pulse = (pulse_H<<16)|pulse_L;
    
    return 0;
}


/*********************************************************************************************
函 数 名: servFPGA_Sonic_Distance_Get
实现功能: 获得超声波测距的结果
输入参数: float  f32Temperature:环境温度
          
输出参数:
            
返 回 值: > 0 :超胜测速得到的距离
          < 0 :错误
说    明: 
        ARM按照下式计算脉宽PW和间距S:
        PW=PCNT * TCNT= 2S/(331.4 + 0.607T)
        其中TCNT为FPGA内部计数时钟周期=1.28*10^6S.
        以S表示间距(单位:m),
        T为环境温度(单位:OC)
        
        经换算得到公式为: S = 0.00064*P/(331400+607T) 
*********************************************************************************************/
float servFPGA_Sonic_Distance_Get(float  f32Temperature)
{
    u32 u32pulse = 0;
    float f32tmp_1 = 0;
    float f32tmp_2 = 0;
    float f32res = 0;
    servFPGA_Sonic_Reset();

    servFPGA_Sonic_Start();
    bspDelayMs(15); //等待15ms为逻辑要求
    
    servFPGA_Sonic_Pulse_Get(0, &u32pulse);    //FOR MODIFY NICK
    f32tmp_1 = 331.400 + 0.607*f32Temperature;
    f32tmp_2 = 0.0008*u32pulse;
    f32res = f32tmp_2*f32tmp_1;

    return f32res;
}


/*********************************************************************************************
函 数 名: servFPGA_Sonic_Threshold_Set
实现功能: 设置超声波碰撞保护阈值
输入参数: 
          
输出参数:u32 u32ThresDis:安全距离阈值
            
返 回 值: 0
说    明: 
          发送给逻辑的安全距离数共20bit,第一次发送低16bit,第二次发送高4bit
          下发数据的计算公式为
          TH=(2*Sth/(331.4 + 0.607*T)*1000000)/ (Tsys*128) -1
          其中TH为阈值,Sth为安全距离(单位为m),T为环境温度(单位摄氏度),Tsys为逻辑内部计数时钟周期,单位为us
          目前该数值为80*10^-3us.
*********************************************************************************************/

s32 servFPGA_Sonic_Threshold_Set(float f32ThresDis,float f32tmeperature)
{
    float f32tmp = 0;
    u32 res = 0;
    u16 data = 0;

    f32tmp = (0.607*f32tmeperature + 331.4)*5.12;
    res = (u32)((f32ThresDis*1000000)/f32tmp);

    data = res&0xFF; //低16bit
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_THRES_L, data);    //FOR MODIFY NICK

    data = (res>>16)&0x0F;//高4bit
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_THRES_H, data);
    
    //printf("dis is %d\n",res);
    return 0;
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
        servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_STAT, &state);
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
函 数 名: servFPGA_State_If_Run_Allow
实现功能: 获取DDR状态是否可以开始执行 DDR 产生PWM
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
*********************************************************************************************/
s32 servFPGA_State_If_Run_Allow(u8 chanNum)
{
    u16 state = 0xFF;

    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_STAT, &state);

    if((state>>2)&0x01 == 0x01)
    {
        return 0;
    }
    else
    {
        return -1;
    }

}


/*********************************************************************************************
函 数 名: servFPGA_State_If_DDR_Init
实现功能: 获取DDR状态是否已经初始化
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
*********************************************************************************************/
s32 servFPGA_State_If_DDR_Init(u8 chanNum)
{
    u16 state = 0xFF;

    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_STAT, &state);

    if((state)&0x01 == 0x01)
    {
        return 0;
    }
    else
    {
        return -1;
    }

}


/*********************************************************************************************
函 数 名: servFPGA_PWM_Len_Set
实现功能: 配置逻辑要执行的波表的长度
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
          
*********************************************************************************************/
void servFPGA_PWM_Len_Set(u8 chanNum, u32 len)
{
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_LENGTH_L_REG, (u16)len);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_LENGTH_H_REG, (u16)(len >> 16));
}


/*********************************************************************************************
函 数 名: servFPGA_PWM_Cycle_Set
实现功能: 配置逻辑要执行的波表的循环次数
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
          下发的数据等于波表长度*2
*********************************************************************************************/
void servFPGA_PWM_Cycle_Set(u8 chanNum, u32 times)
{
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CYCLE_NUM_L_REG, (u16)times);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CYCLE_NUM_H_REG, (u16)(times >> 16));
}


/*********************************************************************************************
函 数 名: servFPGA_DDR_WR_Start_Flag
实现功能: 向逻辑寄存器写入标志 告知开始写入
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
         bit0 写之前清零,写完成后置1
*********************************************************************************************/
void servFPGA_DDR_WR_Start_Flag(void)
{
    servFpgaRegisterWrite(0, ADDR_FPGA_DDR_WAVE_WRITE_CTRL, 0);    //FOR MODIFY NICK
}

/*********************************************************************************************
函 数 名: servFPGA_DDR_WR_End_Flag
实现功能: 向逻辑寄存器写入标志 告知写入完成
输入参数: 
          
输出参数:
            
返 回 值: 0
说    明: 
         bit0 写之前清零,写完成后置1
*********************************************************************************************/
void servFPGA_DDR_WR_End_Flag(void)
{
    servFpgaRegisterWrite(0, ADDR_FPGA_DDR_WAVE_WRITE_CTRL, 1);    //FOR MODIFY NICK
}

/*********************************************************************************************
函 数 名: servFPGA_DDR_Data_Type_Set
实现功能: 配置即将写入的数据类型
输入参数: 
          euWaveDataType euType:
                                    WAVE_DATA = 0,      //写入的是波表
                                    ENCODER_DATA = 1    //写入的是编码器校准系数
          
输出参数:
            
返 回 值: 0
说    明: 
         
*********************************************************************************************/
void servFPGA_DDR_Data_Type_Set(u8 chanNum, euWaveDataType euType)
{
    u16 data = 3 ;

    
    if (euType == ENCODER_DATA)
    {
        data = 2;
    }
    else
    {
        data = 1;
    }

    servFpgaRegisterWrite(chanNum, ADDR_FPGA_DDR_WAVE_TYPE, data);
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
    u16 tmpReg = 0;

    
    servFpgaRegisterRead(chanNum, SERV_FPGA_DDR_MOTOR_STATE_REG, &tmpReg);

    tmpReg &= SERV_FPGA_RUN_STATE_MASK;

    if ((SERV_FPGA_RUN_STATE_NORUN1 == tmpReg) || (SERV_FPGA_RUN_STATE_NORUN2 == tmpReg))
    {
        return RUNSTETE_NORUN;
    }
    else if ((SERV_FPGA_RUN_STATE_RUN1 == tmpReg) || (SERV_FPGA_RUN_STATE_RUN2 == tmpReg))
    {
        return RUNSTETE_RUN;

    }
    else if (SERV_FPGA_RUN_STATE_SD == tmpReg)
    {
        return RUNSTETE_SD;
    }
    else if (SERV_FPGA_RUN_STATE_HOLD == tmpReg)
    {
        return RUNSTETE_HOLD;
    }
    else
    {
        return RUNSTETE_ERROR;
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
    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_CYC_CNT_L, &data_L);
    servFpgaRegisterRead(chanNum, ADDR_FPGA_CYC_CNT_H, &data_H);
    
    *times = (data_H<<16)|data_L + 1;
}


/*********************************************************************************************
函 数 名: servFPGA_PWM_Stop
实现功能: 通知逻辑停止产生PWM
输入参数:     
              
输出参数:
            
返 回 值: 0
说    明: 在运行过程中需要停止  或者  开始预取数据之前  都要发送停止命令
*********************************************************************************************/
void servFPGA_PWM_Reset(u8 chanNum)
{
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_STOP;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
    
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_STOP;
}


/*********************************************************************************************
函 数 名: servFPGA_PWM_FIFO_Left
实现功能: 获取FIFO剩余空间
输入参数: 
          u32      u32Total:FIFO总大小
输出参数:
            
返 回 值: 
说    明: 
         首先从逻辑获取逻辑还未输出的波表数据的个数,
         然后用FIFO总空间数-未输出个数 = 还可以写入的波表个数
*********************************************************************************************/
u32 servFPGA_PWM_FIFO_Left(u8 chanNum, u32 u32Total)
{
    u16 tmp = 0;
    u32 u32Occupy = 0;
    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_WAVE_FIFO_OCCUPY_L, &tmp);//FIFO遗留数据 低16bit
    u32Occupy = tmp;

    tmp = 0;
    servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_WAVE_FIFO_OCCUPY_H, &tmp);//FIFO遗留数据 高16bit
    u32Occupy |= (tmp<<16)&0xFFFF0000;

    return u32Total-u32Occupy;
}


/*********************************************************************************************
函 数 名: servSetFpgaMultiSyncFunc
实现功能: 启动源设置：开始输出波表（S/D信号）的条件/触发源，(位域[6:4]在逻辑上没有限制关系，可同时有效,但在上层需要互斥操作)，只要满足其中1项条件，就输出S/D。
          多机同步设置
输入参数: trigSource:触发源选择(0:软件触发,1:硬件触发,2:CAN总线触发)      
输出参数:   
返 回 值: 
说    明: xyzheng add 2017-04-01
*********************************************************************************************/
void servSetFpgaMultiSyncFunc(MultiSyncFuncEnum enFunc)
{
#if 0  //只用于测试,没有互斥操作,可同时使能多个触发源 
    switch(trigSource)
    {
    case MULTI_SYNC_TRIG_SRC_SW:
        fpgaRegValue.waveTableModeReg &= ~0x01<<4;
        fpgaRegValue.waveTableModeReg |=  0x01<<4;
        break;
    case MULTI_SYNC_TRIG_SRC_HW:
        fpgaRegValue.waveTableModeReg &= ~0x01<<5;
        fpgaRegValue.waveTableModeReg |=  0x01<<5;
        break;
    case MULTI_SYNC_TRIG_SRC_CAN:
        fpgaRegValue.waveTableModeReg &= ~0x01<<6;
        fpgaRegValue.waveTableModeReg |=  0x01<<6;
        break;
    default:
        return;
    }
#else //加入互斥,只能选择一个功能
    switch(enFunc)
    {
    case MULTI_SYNC_TRIG_SRC_OFF:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       break;
    case MULTI_SYNC_TRIG_SRC_SW:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<4;
       break;
    case MULTI_SYNC_TRIG_SRC_HW:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<5;
       break;
    case MULTI_SYNC_TRIG_SRC_CAN:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<6;
       break;
    case MULTI_SYNC_START:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<9;
       break;
    case MULTI_SYNC_END:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<10;
       break;
    case MULTI_SYNC_RST:
       fpgaRegValue.waveTableModeReg[0] &= ~0x01<<8;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<8;
       break;
    default:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<4; //默认为软件触发
       return;
    }
#endif

    servFpgaRegisterWrite(0, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[0]);    //FOR MODIFY NICK
}


/*********************************************************************************************
函 数 名: servGetFpgaMultiSyncFunc
实现功能: 多机同步配置
输入参数: 
输出参数:   
返 回 值: 
说    明: xyzheng add 2017-04-01
*********************************************************************************************/
int servGetFpgaMultiSyncFunc(void)
{
    if((fpgaRegValue.waveTableModeReg[0]>>0x06) & 0x01)
    {
         return MULTI_SYNC_TRIG_SRC_CAN;
    }
    else if((fpgaRegValue.waveTableModeReg[0]>>0x04) & 0x01)
    {
         return MULTI_SYNC_TRIG_SRC_SW;
    }
    else if((fpgaRegValue.waveTableModeReg[0]>>0x05) & 0x01)
    {
        return MULTI_SYNC_TRIG_SRC_HW;
    }
    else if((fpgaRegValue.waveTableModeReg[0]>>0x09) & 0x01)
    {
        return MULTI_SYNC_START;
    }
    else if((fpgaRegValue.waveTableModeReg[0]>>0x0a) & 0x01)
    {
        return MULTI_SYNC_END;
    }
    return -1;
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
    else
    {
        levelState = (LevelStatusEnum)(DIOPOLARITY_P - digitalIO.polarity);
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
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI4_X3_FILTER_REG, registerValue);
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
            servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
            
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
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
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
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
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
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
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
            fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_ALARM_INT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
            
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_DIO_ENABLE;    //使能数字触发
            servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
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
        
#ifdef PROJECT_GELGOOG
            /*if (SENSOR_ON == trigInInfo.levelState[TRIGPIN_DIR])
            {
                //根据编码器选择模式
                if (ECCHAN_3 == encChan)  
                {
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_REF_SELECT_MASK;
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH4_ENC_TRIPLE;
                }
                else
                {
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_REF_SELECT_MASK;
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH4_ENC_SINGLE;
                }
            }*/
#endif
            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
            
            //设置中断
            fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_STOP_INT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);

            //临时配置，后面修改成专门的函数来设置    NICK MARK
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_DIO_ENABLE;    //使能数字触发
            servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
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
        fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_WAVE_END_INT_ENABLE;
    }
    else
    {
        fpgaRegValue.interruptSrcReg[chanNum] &= ~SERV_FPGA_WAVE_END_INT_ENABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
}


/*********************************************************************************************
函 数 名: servFpgaIntrSourceGet
实现功能: 获取FPGA给ARM中断的源头
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.06.26 Add
*********************************************************************************************/
u16 servFpgaIntrSourceGet(u8 chanNum)
{
    u16 intrSrcValue;
    
    
    servFpgaRegisterRead(chanNum, SERV_FPGA_INTERRUPT_SRC_REG, &intrSrcValue);

    return intrSrcValue;
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
    //StopConfigStruct holdStopConfigInfo;


    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_STOP_DECEL_MASK;    //域复位


    //默认使能停止动作的触发源为波表结束
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_WAVE_END_ENABLE;

    switch (stopConfigInfo.stopMode)
    {
        case STOPMODE_IMMED: 
            fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_SD_SRC_WAVE_END_ENABLE;    //禁止急停减速，直接停止   
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_IMMED_ENABLE;            //减速方式立即停止
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
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SLOPE_ENABLE;

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

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
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
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_HOLD_MASK;    //域复位

    if (waveHold)
    {
        fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WH_SRC_WAVE_END_ENABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
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
    fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_WT_MODE_MASK; //清除工作模式
    if (WTWORKMODE_FIFO == waveWorkMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MODE_FIFO_ENABLE; 
    }
    else if (WTWORKMODE_CYCLE == waveWorkMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MODE_CYCLE_ENABLE; 
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
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
    //清除调整占比
    fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_WT_LVT_TIME_MASK;    
    
    if (MDUTY_1000 == modifyDuty)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_LVT_TIME_1000; 
    }
    else if (MDUTY_500 == modifyDuty)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_LVT_TIME_500; 
    }
    else if (MDUTY_250 == modifyDuty)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_LVT_TIME_250; 
    }
    else if (MDUTY_125 == modifyDuty)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_LVT_TIME_125; 
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
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
    //配置编码器线数倍乘
    fpgaRegValue.encMultandCountReg[chanNum] &= ~SERV_FPGA_ENC_LINE_MULT_MASK;    //清除编码器线数的倍乘
    fpgaRegValue.encMultandCountReg[chanNum] |= (lineMult - SERV_FPGA_ENC_LINE_MULT_OFFSET) 
                                                 << SERV_FPGA_ENC_LINE_MULT_LSHIFT
                                                 & SERV_FPGA_ENC_LINE_MULT_MASK;

    servFpgaRegisterWrite(chanNum, SERV_FPGA_ENCODER_RESET_REG, 
                          fpgaRegValue.encMultandCountReg[chanNum]);
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
    fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_WT_MOTION_MODE_MASK;    //清除运动模式
    if (MTNMODE_PVT == motionMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MTN_MODE_PVT; 
    }
    else if (MTNMODE_LVT_CORR == motionMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MTN_MODE_LVT_VEL; 
    }
    else if (MTNMODE_LVT_NOCORR == motionMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MTN_MODE_LVT_NOVEL; 
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
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
函 数 名: servFpgaWaveReset
实现功能: 复位波表
输入参数: 
输出参数:   
返 回 值: 
说    明: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveReset(WaveTableStruct *pWaveTable, WaveWorkModeEnum waveWorkMode)
{
    pWaveTable->writableAddr = pWaveTable->startAddr;
    pWaveTable->writableSize = pWaveTable->totalSize;
    
    pWaveTable->waveState = MTSTATE_IDLE;
    
    pWaveTable->waveWorkMode = waveWorkMode;
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
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_FIFO_RESET;
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_STOP;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
    
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_FIFO_RESET;
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_STOP;
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
    u16 addrL;
    u16 addrH;


    //传入的是波表地址，和DDR地址的对应关系是ddrAddr = waveAddr * 4
    ddrAddr = waveAddr * FPGA_DDR_WAVE_TABLE_POINT_LEN;
    addrL = (u16)(ddrAddr & 0xFFFF);
    addrH = (u16)((ddrAddr >> 16) & 0x0FFF);

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_WT_WRITE_ADDR_L_REG, addrL);
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_WT_WRITE_ADDR_H_REG, addrH);
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
    u16 addrL;
    u16 addrH;


    //传入的是波表地址，和DDR地址的对应关系是ddrAddr = waveAddr * 4
    ddrAddr = waveAddr * FPGA_DDR_WAVE_TABLE_POINT_LEN;
    addrL = (u16)(ddrAddr & 0xFFFF);
    addrH = (u16)((ddrAddr >> 16) & 0x0FFF);

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_START_ADDR_L_REG, addrL);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_START_ADDR_H_REG, addrH);
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
    s32 writeResult = 0;


    //波表未配置
    if (MTSTATE_IDLE == pWaveTable->waveState)
    {    
        //修改波表状态
        pWaveTable->waveState = MTSTATE_CALCING;
        
        //配置数据类型
        servFPGA_DDR_Data_Type_Set(chanNum, WAVE_DATA);

        //初始化波表
        servFpgaWaveInit(chanNum, *pWaveTable);

        //FIFO模式需要Reset
        if (WTWORKMODE_FIFO == pWaveTable->waveWorkMode)
        {
            servFpgaWaveFifoReset(chanNum);

            //这一步理论上在预取的时候设置也是可行的，目前先按旧的流程设置，后续测试后再修改    NICK MARK
            servFPGA_PWM_Len_Set(chanNum, pWaveTable->totalSize);
        }
    }
    
    //通道发生切换
    if (lastChan != chanNum)
    {
        lastChan = chanNum;

        //配置数据类型
        servFPGA_DDR_Data_Type_Set(chanNum, WAVE_DATA);
    }

    //读取逻辑状态，检测是否可以进行波表下发
    writeResult = servFPGA_State_If_WR_Allow(chanNum);    
    if (writeResult != 0)
    {
        return writeResult;
    }
        
    //配置开始标志
    servFPGA_DDR_WR_Start_Flag();  
        
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
        bspSdioDataSend((u8 *)&pOutputData->writeAddr, pOutputData->toSendBytes, pOutputData->toSendBlkSize);

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
            bspSdioDataSend((u8 *)&pOutputData->emptyDataAddr, pOutputData->emptyBytes, pOutputData->emptyBlkSize);

            //更新波表可写地址和大小
            pWaveTable->writableAddr += pOutputData->validEmpPoints;
            pWaveTable->writableSize -= pOutputData->validEmpPoints;

            pOutputData->bSendEmpty  = false;
        }
    }
    
    //配置结束标志
    servFPGA_DDR_WR_End_Flag();

    //最后一帧数据
    if (pOutputData->bLastFrame)
    {
        if (WTWORKMODE_FIFO != pWaveTable->waveWorkMode)
        {
            pWaveTable->waveState = MTSTATE_CALCEND;
        }
    }

    return writeResult;
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
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_PREFETCH;

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);  
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
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~(SERV_FPGA_WAVE_PREFETCH      |
                                                SERV_FPGA_WAVE_OUTPUT_ENABLE |
                                                SERV_FPGA_WAVE_STOP); 
                                          
    if (outputEnable)
    {
        fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_OUTPUT_ENABLE;
    }
    else
    {
        fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_OUTPUT_ENABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
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
    fpgaRegValue.waveTableCtrlReg[0] |= SERV_FPGA_WAVE_ALL_RUN;

    servFpgaRegisterWrite(0, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[0]);
    
    fpgaRegValue.waveTableCtrlReg[0] &= ~SERV_FPGA_WAVE_ALL_RUN;
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
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_SOFT_ENABLE;
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_OUTPUT_ENABLE;    //清除启动位
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
    
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_SD_SRC_SOFT_ENABLE;    //停止命令发送后本地状态必须复位  防止下次误配
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
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_SOFT_ENABLE;
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_OUTPUT_ENABLE;    //清除启动位
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
    
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_SD_SRC_SOFT_ENABLE;    //停止命令发送后本地状态必须复位  防止下次误配    
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
    servFPGA_PWM_Reset(chanNum);

    //配置波表起始地址和长度
    if (waveTableAddr.waveWorkMode != WTWORKMODE_FIFO)    
    {
        servFPGA_PWM_Len_Set(chanNum, waveTableAddr.totalSize - waveTableAddr.writableSize);
    }
    /*else    //理论上FIFO模式也可以在这里才设置长度，目前先按旧流程，等测试过了后再修改    NICK MARK
    {
        servFPGA_PWM_Len_Set(waveTableAddr.totalSize);
    }*/

    //设置波表起始地址
    servFpgaWaveStartDdrAddrSet(chanNum, waveTableAddr.startAddr);
    
    //配置逻辑开始预取数据
    servFpgaWavePrefetch(chanNum);
    
    //查询是否可以开始
    do
    {
        state = servFPGA_State_If_Run_Allow(chanNum); 
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
                
#if FPGA_ARM_COUNT_ENCODER
                bspEncoderTimerInit(chanNum, encMult, ENABLE);
#endif

#else   //#ifdef PROJECT_GELGOOG
              
#if QUBELEY_HARDVER_2
                fpgaRegValue.encandUartReg &= ~SERV_FPGA_ENC_SELECT_MASK;
                fpgaRegValue.encandUartReg |= SERV_FPGA_ENC_SELECT_DI145;
                servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
#endif

#if FPGA_ARM_COUNT_ENCODER
                bspEncoderTimerInit(chanNum, encMult, ENABLE);
#endif
              
#endif    //#ifdef PROJECT_GELGOOG

              break;

            default:
              break;
        }
        servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);


        if (bEncMult)
        {
            //设置编码器倍乘
            fpgaRegValue.encMultandCountReg[chanNum] &= ~SERV_FPGA_ENC_MULT_MASK;

            switch (encMult)
            {
                case ENCMULT_SINGLE:
                    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_MULT_SINGLE;
                  break;

                case ENCMULT_DOUBLE:
                    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_MULT_DOUBLE;
                  break;

                case ENCMULT_QUADR:
                    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_MULT_QUADR;
                  break;

                default:
                  break;
            }
            
            servFpgaRegisterWrite(chanNum, SERV_FPGA_ENCODER_RESET_REG, fpgaRegValue.encMultandCountReg[chanNum]);

            //设置通道数
            switch (encChan)
            {
                case ECCHAN_3:
                    fpgaRegValue.encMultandCountReg[chanNum] &= ~SERV_FPGA_ENC_CHAN_SINGLE;
                  break;
            
                case ECCHAN_1:
                    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_CHAN_SINGLE;
                  break;

                default:
                  break;
            }
            servFpgaRegisterWrite(chanNum, SERV_FPGA_ENCODER_RESET_REG, fpgaRegValue.encMultandCountReg[chanNum]);
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
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_ENCODER_RESET_REG, fpgaRegValue.encMultandCountReg[chanNum]);
#endif
}


/*********************************************************************************************
函 数 名: servFpgaEncoderCountReset
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
void servFpgaLineOutOfStepSet(u8 chanNum, SensorStateEnum  state, f32 lineSteps, u16 threshold)
{
    u16 thresholdL;
    u16 thresholdH;


    if (SENSOR_ON == state)
    {
        fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_OOSTEP_INT_ENABLE;

        thresholdL = (u16)(lineSteps - threshold);
        thresholdH = (u16)(lineSteps + threshold);
        
        servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_OUT_OF_STEP_L_REG, thresholdL);
        servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_OUT_OF_STEP_H_REG, thresholdH);
    }
    else
    {
        fpgaRegValue.interruptSrcReg[chanNum] &= ~SERV_FPGA_OOSTEP_INT_ENABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
}


#if QUBELEY_HARDVER_2
/*********************************************************************************************
函 数 名: servFpgaDriverStateCofing
实现功能: 通过FPGA配置驱动
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
static void servFpgaDriverStateCofing(DriverTypeEnum deviceType, DriverStateEnum driverState)
{
    fpgaRegValue.driverCtrlReg &= SERV_FPGA_DRIVER_SELC_MASK;
    fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_STATE_MASK;    //关闭所有驱动

    if (DRIVER_2660 == deviceType)
    {
        fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_SELC_2660;

        if (DRVSTATE_ON == driverState)
        {
            fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_OFF_2660;
        }
    }
    else
    {
        fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_SELC_262;

        if (DRVSTATE_ON == driverState)
        {
            fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_OFF_262;
        }
    }
        
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_REG, fpgaRegValue.driverCtrlReg);
}
#endif


/*********************************************************************************************
函 数 名: servFpgaDriverClkCofing
实现功能: 通过FPGA配置驱动时钟
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
static void servFpgaDriverClkCofing(u8 chanNum, DriverClkStateEnum driverClkState)
{
    if (CH1 == chanNum)
    {
        fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_CLK_MASK;

        if (DRVCLK_OPEN == driverClkState)
        {
            fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_CLK_ENABLE;
        }
    }
#ifdef PROJECT_GELGOOG

    else
    {
        fpgaRegValue.driverCtrlReg &= ~(SERV_FPGA_DRIVER_CLK2_MASK << (chanNum - CH2));

        if (DRVCLK_OPEN == driverClkState)
        {
            fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_CLK2_ENABLE << (chanNum - CH2);
        }
    }
    
#endif

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_REG, fpgaRegValue.driverCtrlReg);
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
    fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_SRC_MASK;
    fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_SRC_CPU;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_REG, fpgaRegValue.driverCtrlReg);    //通道号固定为0
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
#ifdef PROJECT_GELGOOG 
    fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_CHAN_MASK;
    fpgaRegValue.driverCtrlReg |= chanNum << SERV_FPGA_DRIVER_CHAN_LSHIFT;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_REG, fpgaRegValue.driverCtrlReg);
#endif
}


/*********************************************************************************************
函 数 名: servFpgaDriverCofing
实现功能: 通过FPGA配置驱动
输入参数: 
输出参数:   
返 回 值: 
说    明: 
*********************************************************************************************/
s32 servFpgaDriverCofing(u8 chanNum, DriverTypeEnum deviceType, DriverStateEnum driverState, DriverClkStateEnum driverClkState)
{
    //fpgaRegValue.driverCtrlReg = 0;

#if QUBELEY_HARDVER_2    //Qubeley 03版以后及Gelgoog都不需要配置 
    servFpgaDriverStateCofing(deviceType, driverState);
#endif

    servFpgaDriverClkCofing(chanNum, driverClkState);
    
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
    fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_START_SOURCE_MASK;

    switch (startSource)
    {
        case SSRC_SOFT:
        default:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_SOFT;
          break;

        case SSRC_HARD:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_HARD;
          break;

        case SSRC_CAN:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_CAN;
          break;

        case SSRC_ALL:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_SOFT;
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_HARD;
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_CAN;
          break;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
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


    fpgaRegValue.waveTableModeReg[CH1] &= ~SERV_FPGA_CLOCK_SYNC_MASK;

    switch (ClockSyncReg)
    {
        case CLOCKREG_ON:
          fpgaRegValue.waveTableModeReg[CH1] |= SERV_FPGA_CLOCK_SYNC_RESET;
          fpgaRegValue.waveTableModeReg[CH1] |= SERV_FPGA_CLOCK_SYNC_START;
          break;

        case CLOCKREG_OFF:
          fpgaRegValue.waveTableModeReg[CH1] |= SERV_FPGA_CLOCK_SYNC_END;
          break;
          
        default:
          break;
    }


    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[CH1]);

    if(receiveType == RECEIVE_RADIOID)
    {
        canId = canIntfc.radioId;
    }
    else if(receiveType == RECEIVE_GROUPID1)
    {
        canId = canIntfc.groupId1;
    }
    else if(receiveType == RECEIVE_GROUPID2)
    {
        canId = canIntfc.groupId2;
    }
    else if(receiveType == RECEIVE_RECEIVEID)
    {
        canId = canIntfc.receiveId;
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
    u16 tmpReg = 0;
    DdrStateEnum ddrState;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_DDR_MOTOR_STATE_REG, &tmpReg);

    if ((tmpReg & SERV_FPGA_DDR_INIT_MASK) == SERV_FPGA_DDR_INIT_SUCCESS)
    {
        if ((tmpReg & SERV_FPGA_DDR_TEST_ERROR) == SERV_FPGA_DDR_TEST_ERROR)
        {
            ddrState = DDR_TESTERROR;
        }
        else
        {
            ddrState = DDR_NOERROR;
        }
    }
    else
    {
        ddrState = DDR_NOINIT;
    }

    return ddrState;
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
void servFpgaReverseMotionSet(u8 chanNum, SensorStateEnum  revMotion)
{
    switch (revMotion)
    {
        case SENSOR_OFF:
        default:
          fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_REVERSE_MOTION_ENABLE;
          break;

        case SENSOR_ON:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_REVERSE_MOTION_ENABLE;
          break;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
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
    //复位编码器计数
    servFpgaEncoderCountReset(chanNum);

    //更新下急停设置
    servFpgaWaveStopConfig(&(pOutpWaveTable->stopConfigInfo), wavePlanInfo.stopDecelInfo, posnConvertInfo.posnToStep);

    //配置结束状态
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
        servFPGA_PWM_Cycle_Set(chanNum, wavePlanInfo.cycleNum);
    }
}


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
#if GELGOOG_SINANJU
    u16 dataID = 0x11A;
    
    servFpgaRegisterWrite(sensorNum, SERV_FPGA_ABS_ENC_REDA_REG, dataID);
#endif
}



/*******************************************文件尾********************************************/
