/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  cmdFactory.c；
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.06.23;
历史版本:  无;
*********************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "project.h"
#include "cmdFactory.h"
#include "cmdMainParse.h"
#include "servFpga.h"
#include "servCommIntfc.h"
#include "servSoftTimer.h"
#include "servSystemPara.h"
#include "systemInit.h"
#include "pvtAlgorithm.h"
#include "bspSdio.h"
#include "bspFlash.h"
#include "bspTimer.h"

#ifdef PROJECT_QUBELEY
#include "bspAdc.h"
#endif


/****************************************外部变量声明*****************************************/\
extern SystemInfoStruct  g_systemInfo;
extern DeviceInfoStruct  g_deviceInfo;
extern SystemStateStruct g_systemState;
extern BootInfoStruct    g_bootInfo;
extern MotorInfoStruct   g_motorInfo;
extern WaveTableStruct   g_waveTable[CH_TOTAL][WTTYPE_RESERVE];
extern OutputDataStruct  g_outputData[CH_TOTAL];
extern ChanCfgBmpStruct  g_chanCfgBmp[CH_TOTAL];

extern bool g_bCmdPostSemToFunc;



/*****************************************局部宏定义******************************************/
#if QUBELEY_HARDVER_2
#define    TRIGGER_IN_NUM    7     //数字输入的通道数
#elif QUBELEY_HARDVER_1
#define    TRIGGER_IN_NUM    2     //数字输入的通道数 
#else
#define    TRIGGER_IN_NUM    16    //Gelgoog数字输入的通道数 
#endif
#define    ISOLATOR_IN_NUM   4     //隔离输入的通道数

#define    ANALOG_IN_ADC_THRESHOLD    1000
#define    EEPROM_CLEAN_CODE          0xA5     //165，清除EEPROM数据的验证码



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pFactoryCmdFunc[FACTORYCMD_RESERVE];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdFactorySnSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactorySnSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 dataOffset;
    u8 frameNum;
    u8 totalNum;
    u8 fraction;
    static u8 deviceSn[DEVICE_SN_LEN];


    if ((cmdDataLen >= 2) && (cmdDataLen <= 6))    //长度要在2-6之间(分数加至少1个数据，至多5个数据)
    {
        fraction = *pCmdData++;
        frameNum = (fraction & 0xF0) >> 4;    //高4bit代表分子，所以左移4bit，低4bit代表分母
        totalNum = (fraction & 0x0F);

        //计算偏移和数据长度
        dataLen    = cmdDataLen - 1;    //减的是分数占得1个字节
        dataOffset = (frameNum - 1) * FRACTION_FRAME_DATA_LEN_MAX;
        memcpy((deviceSn + dataOffset), pCmdData, dataLen);
        
        if (frameNum == totalNum)
        {
            //存储到EEPROM和FLASH中    //目前是未加密存储，后续需要改为加密存储    NICK MARK
            memcpy(g_deviceInfo.storageInfo.deviceSn, deviceSn, DEVICE_SN_LEN);
            servDeviceInfoStore(&g_deviceInfo.storageInfo);

            //前面写EEprom的时候已经计算了CRC，不用再计算了    FOR MODIFY NICK
            //bspFlashWrite(FLASH_SN_ADDR, (u8 *)&g_deviceInfo, sizeof(SnEepromStruct));
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryTypeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
}


/*********************************************************************************************
函 数 名: cmdFactorySoftVerQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactorySoftVerQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = SOFTWARE_VER_LEN;
    pData = g_deviceInfo.software;
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_SOFTVERQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdFactoryFpgaVerQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryFpgaVerQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;

    
    dataLen = FPGA_VER_LEN;
    pData = g_deviceInfo.fpga.verArray;
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_FPGAVERQ, dataLen, pData);
}


/*********************************************************************************************
函 数 名: cmdFactoryResetSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryResetSet(u8 cmdDataLen, u8 *pCmdData)
{
    bspSoftReset();
}


/*********************************************************************************************
函 数 名: cmdFactorySignatureQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactorySignatureQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  data[FRACTION_FRAME_DATA_LEN_MAX + 1];    //数据长度在加上分数的一个字节总共6个字节
    u8  i;
    u32 address = 0x1FFFF7E8; //The 96-bit unique device identifier


    //96位，12个字节，需要分三次发送
    //第一次
    data[0] = FRACTION(1, 3);    //1/3
    for (i = 0;i < FRACTION_FRAME_DATA_LEN_MAX;i++)
    {
        data[i + 1] = *(__IO u8*) address++;
    }
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_SIGNATUREQ, 6, data);
    
    //第二次
    data[0] = FRACTION(2, 3);    //2/3
    for (i = 0;i < FRACTION_FRAME_DATA_LEN_MAX;i++)
    {
        data[i] = *(__IO u8*) address++;
    }
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_SIGNATUREQ, 6, data);

    //最后一次
    data[0] = FRACTION(3, 3);    //3/3
    for (i = 0;i < 2;i++)    //再读最后两个
    {
        data[i] = *(__IO u8*) address++;
    }
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_SIGNATUREQ, 3, data);
}


/*********************************************************************************************
函 数 名: cmdFactoryDDRTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDDRTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    //进入DDR测试模式
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_DDRTEST;
}


/*********************************************************************************************
函 数 名: cmdFactoryDDRTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDDRTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_DDRTESTQ, 2, g_systemState.errorCode);
}


/*********************************************************************************************
函 数 名: cmdFactoryDriverTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDriverTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 i;
    u8  chanNum;
    WaveTableStruct *pCurrUseWaveTable;


    chanNum = *pCmdData;
    if (chanNum < g_systemState.chanNum)
    {
        //进入驱动测试模式
        memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
        g_systemState.errorCode[0] = 0xFF;
        g_systemState.errorCode[1] = chanNum;
        g_systemState.EnginMode = ENGINMODE_DRIVETEST;
    
        g_systemState.calcWaveTable[chanNum] = WTTYPE_SMALL;
        pCurrUseWaveTable = &g_waveTable[chanNum][WTTYPE_SMALL];

        servFrameSend(CMD_FACTORY, FACTORYCMD_DRIVERSTART, 0, NULL, LINK_CAN);

        memset(&g_outputData[chanNum], 0, sizeof(g_outputData[chanNum]));
        g_outputData[chanNum].writeAddr     = ADDR_FPGA_DDR_WAVE_WRITE_DATA;
        g_outputData[chanNum].emptyDataAddr = ADDR_FPGA_DDR_WAVE_WRITE_DATA;

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
        g_outputData[chanNum].minPeriod = FPGA_PWM_CLOCK;  
        
#else

        g_outputData[chanNum].minPeriod     = g_systemInfo.fpgaPwmClock;   
#endif

        //初始化要写入的波表(转一圈，外部固定接减速10:1的电机，细分64，一圈就是128000微步，循环数126)
        servFpgaWaveReset(pCurrUseWaveTable, WTWORKMODE_CYCLE);
        
        for (i = 0;i < OUTPUT_DATA_BUFFER_SIZE;i++)
        {
            //2500 * 50ns = 125us
            g_outputData[chanNum].data[i] = (0x02 << 27) | (2500 & 0x01FFFFFF) | 0x80000000;    //正向，周期数据，数据有效
        }
        g_outputData[chanNum].toSendBlkSize = 192;
        g_outputData[chanNum].toSendPoints  = 1023;
        g_outputData[chanNum].toSendBytes   = 4096;
        g_outputData[chanNum].validPoints   = g_outputData[chanNum].toSendPoints;
        g_outputData[chanNum].validBytes    = g_outputData[chanNum].toSendBytes;
        
        servFpgaWaveDataWrite(chanNum, &g_outputData[chanNum], pCurrUseWaveTable);

        pCurrUseWaveTable->stopConfigInfo.stopMode = STOPMODE_IMMED;    //FOR MODIFY NICK

        g_chanCfgBmp[chanNum].bQryReady = true;
        g_bCmdPostSemToFunc = true;    
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryDriverTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDriverTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_DRIVERTESTQ, 3, g_systemState.errorCode);
}


/*********************************************************************************************
函 数 名: cmdFactoryTrigInTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryTrigInTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 data;

    
    //进入触发输入测试模式
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_TRIGINTEST;

    //通过CAN总线通知测试设备拉高相应管脚，相关的管脚由测试板全部拉高
    data = (u8)LEVEL_HIGH;
    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
    servTxFrameProcess();    //强制发送
    
    bspDelayMs(10);    //短暂延时后查询电平
    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
函 数 名: cmdFactoryTrigInTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryTrigInTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINTESTQ, 5, g_systemState.errorCode);
}


/*********************************************************************************************
函 数 名: cmdFactoryTrigOutTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryTrigOutTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;

    
    //进入触发输出测试模式
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_TRIGOUTTEST;

    //通过FPGA设置管脚输出为高
    for (i = 0;i < DIO_RESERVE;i++)
    {
        servFpgaDigitalOutLevelSet((DoutNumEnum)i, LEVEL_HIGH);
    }
    g_systemState.levelStatus = LEVEL_HIGH;
    
    bspDelayMs(10);    //短暂延时后查询电平
    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGOUTENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
函 数 名: cmdFactoryTrigOutTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryTrigOutTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_TRIGOUTTESTQ, 5, g_systemState.errorCode);
}


/*********************************************************************************************
函 数 名: cmdFactoryEncoderTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryEncoderTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    //进入触发输出测试模式
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_ENCODERTEST;

    //清除编码器计数
    servFpgaEncoderCountReset(CH1);

    //设置倍乘为4
    servFpgaEncoderSet(CH1, INTFC_ON, ENCMULT_QUADR, ECCHAN_3);

    servFrameSend(CMD_FACTORY, FACTORYCMD_ENCODERSTART, 0, NULL, LINK_CAN);
    servTxFrameProcess();    //强制发送
    
    bspDelayMs(1000);    //短暂延时后查询电平
}


/*********************************************************************************************
函 数 名: cmdFactoryEncoderTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryEncoderTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_ENCODERTESTQ, 3, g_systemState.errorCode);
}


/*********************************************************************************************
函 数 名: cmdFactoryIsolatorInTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryIsolatorInTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 data;

    
    //进入触发输入测试模式
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_ISOLINTEST;

    //通过CAN总线通知测试设备拉高相应管脚，相关的管脚由测试板全部拉高
    data = (u8)LEVEL_HIGH;
    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
    servTxFrameProcess();    //强制发送
    
    bspDelayMs(10);    //短暂延时后查询电平
    servFrameSend(CMD_FACTORY, FACTORYCMD_ISOLINENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
函 数 名: cmdFactoryIsolatorInTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryIsolatorInTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_ISOLINTESTQ, 5, g_systemState.errorCode);
}


/*********************************************************************************************
函 数 名: cmdFactoryIsolatorOutTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryIsolatorOutTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;

    
    //进入触发输出测试模式
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_ISOLOUTTEST;

    //通过FPGA设置管脚输出为高
    for (i = 0;i < DIO_RESERVE;i++)
    {
        servFpgaIsolatorOutLevelSet((YoutNumEnum)i, LEVEL_LOW);
    }
    g_systemState.levelStatus = LEVEL_HIGH;
    
    bspDelayMs(10);    //短暂延时后查询电平
    servFrameSend(CMD_FACTORY, FACTORYCMD_ISOLOUTENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
函 数 名: cmdFactoryIsolatorOutTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryIsolatorOutTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_ISOLOUTTESTQ, 5, g_systemState.errorCode);
}


#ifdef PROJECT_QUBELEY
/*********************************************************************************************
函 数 名: cmdFactoryAnalogInTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryAnalogInTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 data;

    
    //进入模拟输入测试模式
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_ANALOGINTEST;

    //通过CAN总线通知测试设备拉高相应管脚
    data = (u8)LEVEL_HIGH;

    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
    servTxFrameProcess();    //强制发送

    bspDelayMs(10);    //短暂延时后查询电平
    servFrameSend(CMD_FACTORY, FACTORYCMD_ANALOGINENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
函 数 名: cmdFactoryAnalogInTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryAnalogInTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_ANALOGINTESTQ, 2, g_systemState.errorCode);
}
#endif


/*********************************************************************************************
函 数 名: cmdFactorySensorUartTestSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactorySensorUartTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    
}


/*********************************************************************************************
函 数 名: cmdFactorySensorUartTestQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactorySensorUartTestQuery(u8 cmdDataLen, u8 *pCmdData)
{

}


#if 0
#endif
/*********************************************************************************************
函 数 名: cmdFactoryDriverEndQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDriverEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 channelACount;
    u8 channelBCount;
    u8 channelZCount;


    channelACount  = *(u16 *)pCmdData++;
    channelBCount  = *(u16 *)pCmdData++;
    channelZCount  = *(u16 *)pCmdData;
    if ((channelACount <= 0) || (channelBCount <= 0) || (channelZCount <= 0))
    {
        g_systemState.errorCode[2] = ENGINMODE_TEST_ERROR;
    }
    g_systemState.EnginMode = ENGINMODE_NONE;
    g_systemState.errorCode[0] = 0;
}


/*********************************************************************************************
函 数 名: cmdFactoryTrigInEndQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryTrigInEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   data;
    u8   i;
    u8  *testResult;
    u32  errorCode = 0;
    bool sameState = true;
    LevelStatusEnum readStatus;
    LevelStatusEnum levelStatus;

    
    //先从FPGA读取状态
    levelStatus = (LevelStatusEnum)*pCmdData;
    for (i = 0;i < TRIGGER_IN_NUM;i++)
    {
        if (i <= DIGITALIO_DI2)
        {
            readStatus = servFpgaDioStatusRead((DigitalIONumEnum)i);
        }
        else
        {
            
            readStatus = servFpgaDioStatusRead((DigitalIONumEnum)(i + 1));
        }

        //每个测试项占两个bit，bit1代表是否有此测试项，bit0代表测试结果: 1-测试出错; 0-测试正常
        errorCode |= (2 << (i * 2));    //有此测试项
        if (readStatus != levelStatus)
        {
            sameState  = false;
            errorCode |= (1 << (i * 2));    //测试出错
        }
    }

    if (sameState)    //状态一致则继续低电平测试，不一致则记录错误并停止测试
    {
        switch (levelStatus)
        {
            case LEVEL_LOW:
                testResult = (u8 *)&errorCode;
                for (i = 0;i < sizeof(errorCode);i++)
                {
                    g_systemState.errorCode[1 + i] = *testResult++;
                }
                g_systemState.EnginMode = ENGINMODE_NONE;    //结束测试
                g_systemState.errorCode[0] = 0;
              break;

            case LEVEL_HIGH:
                //通过CAN总线通知测试设备拉高相应管脚
                data = (u8)LEVEL_LOW;
                
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
                servTxFrameProcess();    //强制发送
                
                bspDelayMs(10);    //短暂延时后查询电平
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINENDQ, 0, NULL, LINK_CAN);
              break;

            default:
              break;
        }
    }
    else
    {
        //通过CAN总线通知测试设备拉高相应管脚
        data = (u8)LEVEL_LOW;
                
        servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);

        testResult = (u8 *)&errorCode;
        for (i = 0;i < sizeof(errorCode);i++)
        {
            g_systemState.errorCode[1 + i] = *testResult++;
        }
        
        g_systemState.EnginMode = ENGINMODE_NONE;
        g_systemState.errorCode[0] = 0;
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryTrigOutEndQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryTrigOutEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   i;
    u8   allPinState;
    u8  *testResult;
    u32  errorCode = 0;
    bool sameState = true;
    LevelStatusEnum readStatus;     //从FPGA读取的状态
    LevelStatusEnum levelStatus;    //从测试设备回读的状态


    //先从FPGA读取状态
    allPinState = *pCmdData;
    for (i = 0;i < DIO_RESERVE;i++)
    {
        //readStatus = servFpgaDioStatusRead((DigitalIONumEnum)((u8)DIGITALIO_DO1 + i));
        levelStatus = (LevelStatusEnum)((allPinState >> i) & 0x01);
        readStatus  = levelStatus;

        //每个测试项占两个bit，bit1代表是否有此测试项，bit0代表测试结果: 1-测试出错; 0-测试正常
        errorCode |= (2 << (i * 2));    //有此测试项
        if ((readStatus != levelStatus) || (readStatus != g_systemState.levelStatus))
        {
            sameState  = false;
            errorCode |= (1 << (i * 2));    //测试出错
        }
    }

    //三个状态(设置和实际输出，输出和回读)都一致则继续低电平测试，不一致则记录错误并停止测试
    if (sameState)   
    {
        switch (levelStatus)
        {
            case LEVEL_LOW:
                testResult = (u8 *)&errorCode;
                for (i = 0;i < sizeof(errorCode);i++)
                {
                    g_systemState.errorCode[1 + i] = *testResult++;
                }
                g_systemState.EnginMode = ENGINMODE_NONE;
                g_systemState.errorCode[0] = 0;
              break;

            case LEVEL_HIGH:
                //通过FPGA设置管脚输出为高
                for (i = 0;i < DIO_RESERVE;i++)
                {
                    servFpgaDigitalOutLevelSet((DoutNumEnum)i, LEVEL_LOW);
                }
                g_systemState.levelStatus = LEVEL_LOW;
                
                bspDelayMs(10);    //短暂延时后查询电平
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGOUTENDQ, 0, NULL, LINK_CAN);
              break;

            default:
              break;
        }
    }
    else
    {
        //通过FPGA设置管脚输出为高
        for (i = 0;i < DIO_RESERVE;i++)
        {
            servFpgaDigitalOutLevelSet((DoutNumEnum)i, LEVEL_LOW);
        }
        g_systemState.levelStatus = LEVEL_LOW;

        testResult = (u8 *)&errorCode;
        for (i = 0;i < sizeof(errorCode);i++)
        {
            g_systemState.errorCode[1 + i] = *testResult++;
        }
        g_systemState.EnginMode = ENGINMODE_NONE;
        g_systemState.errorCode[0] = 0;
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryEncoderEndQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryEncoderEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    s16 channelZCount;
    s32 channelABCount;


    //读编码器计数，DEMO板发出200个脉冲，编码器倍乘4
    servFpgaEncoderCountRead(CH1, &channelZCount, &channelABCount, NULL);

    if (0 == channelZCount)
    {
        g_systemState.errorCode[1] = ENGINMODE_TEST_ERROR;
    }

    if (channelABCount <= 400)
    {
        g_systemState.errorCode[2] = ENGINMODE_TEST_ERROR;
    }

    g_systemState.EnginMode = ENGINMODE_NONE;
    g_systemState.errorCode[0] = 0;
}


/*********************************************************************************************
函 数 名: cmdFactoryIsolatorInEndQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryIsolatorInEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   data;
    u8   i;
    u8  *testResult;
    u32  errorCode = 0;
    bool sameState = true;
    LevelStatusEnum readStatus;
    LevelStatusEnum levelStatus;

    
    //先从FPGA读取状态
    levelStatus = (LevelStatusEnum)*pCmdData;
    for (i = DIGITALIO_XI1;i < (DIGITALIO_XI1 + ISOLATOR_IN_NUM);i++)
    {
        readStatus = (LevelStatusEnum)(LEVEL_HIGH - servFpgaDioStatusRead((DigitalIONumEnum)i));

        //每个测试项占两个bit，bit1代表是否有此测试项，bit0代表测试结果: 1-测试出错; 0-测试正常
        errorCode |= (2 << ((i - DIGITALIO_XI1) * 2));    //有此测试项
        if (readStatus != levelStatus)
        {
            sameState  = false;
            errorCode |= (1 << ((i - DIGITALIO_XI1) * 2));    //测试出错
        }
    }

    if (sameState)    //状态一致则继续低电平测试，不一致则记录错误并停止测试
    {
        switch (levelStatus)
        {
            case LEVEL_LOW:
                testResult = (u8 *)&errorCode;
                for (i = 0;i < sizeof(errorCode);i++)
                {
                    g_systemState.errorCode[1 + i] = *testResult++;
                }
                g_systemState.EnginMode = ENGINMODE_NONE;    //结束测试
                g_systemState.errorCode[0] = 0;
              break;

            case LEVEL_HIGH:
                //通过CAN总线通知测试设备拉高相应管脚
                data = (u8)LEVEL_LOW;
                
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
                servTxFrameProcess();    //强制发送
                
                bspDelayMs(10);    //短暂延时后查询电平
                servFrameSend(CMD_FACTORY, FACTORYCMD_ISOLINENDQ, 0, NULL, LINK_CAN);
              break;

            default:
              break;
        }
    }
    else
    {
        //通过CAN总线通知测试设备拉高相应管脚
        data = (u8)LEVEL_LOW;
                
        servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
        
        testResult = (u8 *)&errorCode;
        for (i = 0;i < sizeof(errorCode);i++)
        {
            g_systemState.errorCode[1 + i] = *testResult++;
        }
        g_systemState.EnginMode = ENGINMODE_NONE;    //结束测试
        g_systemState.errorCode[0] = 0;
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryIsolatorOutEndQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryIsolatorOutEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   i;
    u8   allPinState;
    u8  *testResult;
    u32  errorCode = 0;
    bool sameState = true;
    LevelStatusEnum readStatus;     //从FPGA读取的状态
    LevelStatusEnum levelStatus;    //从测试设备回读的状态


    //先从FPGA读取状态
    allPinState = *pCmdData;
    for (i = 0;i < YOUT_RESERVE;i++)
    {
        //readStatus = servFpgaDioStatusRead((DigitalIONumEnum)((u8)DIGITALIO_YO1 + i));
        levelStatus = (LevelStatusEnum)((allPinState >> (i + 2)) & 0x01);
        readStatus  = levelStatus;

        //每个测试项占两个bit，bit1代表是否有此测试项，bit0代表测试结果: 1-测试出错; 0-测试正常
        errorCode |= (2 << (i * 2));    //有此测试项
        if ((readStatus != levelStatus) || (readStatus != g_systemState.levelStatus))
        {
            sameState  = false;
            errorCode |= (1 << (i * 2));    //测试出错
        }
    }

    //三个状态(设置和实际输出，输出和回读)都一致则继续低电平测试，不一致则记录错误并停止测试
    if (sameState)   
    {
        switch (levelStatus)
        {
            case LEVEL_LOW:
                testResult = (u8 *)&errorCode;
                for (i = 0;i < sizeof(errorCode);i++)
                {
                    g_systemState.errorCode[1 + i] = *testResult++;
                }
                g_systemState.EnginMode = ENGINMODE_NONE;    //结束测试
                g_systemState.errorCode[0] = 0;
              break;

            case LEVEL_HIGH:
                //通过FPGA设置管脚输出为高
                for (i = 0;i < YOUT_RESERVE;i++)
                {
                    servFpgaIsolatorOutLevelSet((YoutNumEnum)i, LEVEL_HIGH);
                }
                g_systemState.levelStatus = LEVEL_LOW;
                
                bspDelayMs(10);    //短暂延时后查询电平
                servFrameSend(CMD_FACTORY, FACTORYCMD_ISOLOUTENDQ, 0, NULL, LINK_CAN);
              break;

            default:
              break;
        }
    }
    else
    {
        //通过FPGA设置管脚输出为高
        for (i = 0;i < YOUT_RESERVE;i++)
        {
            servFpgaIsolatorOutLevelSet((YoutNumEnum)i, LEVEL_HIGH);
        }
        g_systemState.levelStatus = LEVEL_HIGH;
        
        testResult = (u8 *)&errorCode;
        for (i = 0;i < sizeof(errorCode);i++)
        {
            g_systemState.errorCode[1 + i] = *testResult++;
        }
        g_systemState.EnginMode = ENGINMODE_NONE;    //结束测试
        g_systemState.errorCode[0] = 0;
    }
}


#ifdef PROJECT_QUBELEY
/*********************************************************************************************
函 数 名: cmdFactoryAnalogInEndQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryAnalogInEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 data;
    u16 dacValue;
    LevelStatusEnum levelStatus;


    //先从ADC读取采样值
    dacValue = drvAdcGetAverage(ANALOG_IN_ADC_CH, 1);

    levelStatus = *(LevelStatusEnum *)pCmdData;
    switch (levelStatus)
    {
        case LEVEL_LOW:
            if (dacValue < ANALOG_IN_ADC_THRESHOLD)
            {
                g_systemState.EnginMode = ENGINMODE_NONE;    //结束测试
                g_systemState.errorCode[0] = 0;
            }
            else
            {
                //通过CAN总线通知测试设备拉低相应管脚
                data = (u8)LEVEL_LOW;
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
                
                g_systemState.errorCode[1] = ENGINMODE_TEST_ERROR;
                g_systemState.EnginMode = ENGINMODE_NONE;
                g_systemState.errorCode[0] = 0;
            }
          break;

        case LEVEL_HIGH:
            if (dacValue > ANALOG_IN_ADC_THRESHOLD)
            {
                //通过CAN总线通知测试设备拉低相应管脚
                data = (u8)LEVEL_LOW;    //1代表高电平，0代表低电平
                
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
                servTxFrameProcess();    //强制发送
                
                bspDelayMs(10);    //短暂延时后查询电平
                servFrameSend(CMD_FACTORY, FACTORYCMD_ANALOGINENDQ, 0, NULL, LINK_CAN);
            }
            else
            {
                //通过CAN总线通知测试设备拉低相应管脚
                data = (u8)LEVEL_LOW;
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
                
                g_systemState.errorCode[1] = ENGINMODE_TEST_ERROR;
                g_systemState.EnginMode = ENGINMODE_NONE;
                g_systemState.errorCode[0] = 0;
            }
          break;

        default:
          break;
    }
}
#endif


/*********************************************************************************************
函 数 名: cmdFactorySensorUartEndQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactorySensorUartEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    
}


#if 0
#endif
/*********************************************************************************************
函 数 名: cmdFactoryEepromClean;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryEepromClean(u8 cmdDataLen, u8 *pCmdData)
{
    u8  cleanCode = *pCmdData;


    if (EEPROM_CLEAN_CODE == cleanCode)    //码值正确就把所有值写成默认值
    {
        SystemParaReset();
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryFpgaRegSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryFpgaRegSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8  chanNum;
    u16 regNum;
    u16 regValue;


    if ((sizeof(chanNum) + sizeof(regNum) + sizeof(regValue)) == cmdDataLen)
    {
        chanNum  = *pCmdData++;
        regNum   = *(u16 *)pCmdData;
        regValue = *(((u16 *)pCmdData) + 1);
        servFpgaRegisterWrite(chanNum, regNum, regValue);
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryCmdInit;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryCmdInit(void)
{
    memset(pFactoryCmdFunc, 0, sizeof(pFactoryCmdFunc));

    pFactoryCmdFunc[FACTORYCMD_SN]       = cmdFactorySnSet;
    pFactoryCmdFunc[FACTORYCMD_TYPE]     = cmdFactoryTypeSet;
    pFactoryCmdFunc[FACTORYCMD_SOFTVERQ] = cmdFactorySoftVerQuery; 
    pFactoryCmdFunc[FACTORYCMD_FPGAVERQ] = cmdFactoryFpgaVerQuery; 
    pFactoryCmdFunc[FACTORYCMD_RESET]    = cmdFactoryResetSet;
    
    pFactoryCmdFunc[FACTORYCMD_SIGNATUREQ]    = cmdFactorySignatureQuery; 
    pFactoryCmdFunc[FACTORYCMD_DDRTEST]       = cmdFactoryDDRTestSet;
    pFactoryCmdFunc[FACTORYCMD_DDRTESTQ]      = cmdFactoryDDRTestQuery;
    pFactoryCmdFunc[FACTORYCMD_DRIVERTEST]    = cmdFactoryDriverTestSet;
    pFactoryCmdFunc[FACTORYCMD_DRIVERTESTQ]   = cmdFactoryDriverTestQuery;
    pFactoryCmdFunc[FACTORYCMD_TRIGINTEST]    = cmdFactoryTrigInTestSet;
    pFactoryCmdFunc[FACTORYCMD_TRIGINTESTQ]   = cmdFactoryTrigInTestQuery;
    pFactoryCmdFunc[FACTORYCMD_TRIGOUTTEST]   = cmdFactoryTrigOutTestSet;
    pFactoryCmdFunc[FACTORYCMD_TRIGOUTTESTQ]  = cmdFactoryTrigOutTestQuery;
    pFactoryCmdFunc[FACTORYCMD_ENCODERTEST]   = cmdFactoryEncoderTestSet;
    pFactoryCmdFunc[FACTORYCMD_ENCODERTESTQ]  = cmdFactoryEncoderTestQuery;
    pFactoryCmdFunc[FACTORYCMD_ISOLINTEST]    = cmdFactoryIsolatorInTestSet;
    pFactoryCmdFunc[FACTORYCMD_ISOLINTESTQ]   = cmdFactoryIsolatorInTestQuery;
    pFactoryCmdFunc[FACTORYCMD_ISOLOUTTEST]   = cmdFactoryIsolatorOutTestSet;
    pFactoryCmdFunc[FACTORYCMD_ISOLOUTTESTQ]  = cmdFactoryIsolatorOutTestQuery; 

#ifdef PROJECT_QUBELEY
    pFactoryCmdFunc[FACTORYCMD_ANALOGINTEST]  = cmdFactoryAnalogInTestSet;
    pFactoryCmdFunc[FACTORYCMD_ANALOGINTESTQ] = cmdFactoryAnalogInTestQuery;
#endif

    pFactoryCmdFunc[FACTORYCMD_SUARTTEST]     = cmdFactorySensorUartTestSet;
    pFactoryCmdFunc[FACTORYCMD_SUARTTESTQ]    = cmdFactorySensorUartTestQuery;

    //下面这部分命令是待测设备和测试设备之间进行通信用的，不和上位机进行通信
    pFactoryCmdFunc[FACTORYCMD_DRIVERENDQ]    = cmdFactoryDriverEndQuery;
    pFactoryCmdFunc[FACTORYCMD_TRIGINENDQ]    = cmdFactoryTrigInEndQuery;
    pFactoryCmdFunc[FACTORYCMD_TRIGOUTENDQ]   = cmdFactoryTrigOutEndQuery;
    pFactoryCmdFunc[FACTORYCMD_ENCODERENDQ]   = cmdFactoryEncoderEndQuery;
    pFactoryCmdFunc[FACTORYCMD_ISOLINENDQ]    = cmdFactoryIsolatorInEndQuery;
    pFactoryCmdFunc[FACTORYCMD_ISOLOUTENDQ]   = cmdFactoryIsolatorOutEndQuery;

#ifdef PROJECT_QUBELEY
    pFactoryCmdFunc[FACTORYCMD_ANALOGINENDQ]  = cmdFactoryAnalogInEndQuery;
#endif

    pFactoryCmdFunc[FACTORYCMD_SUARTENDQ]     = cmdFactorySensorUartEndQuery;
    //上面这部分命令是待测设备和测试设备之间进行通信用的，不和上位机进行通信

    pFactoryCmdFunc[FACTORYCMD_EEPROMCLEAN]  = cmdFactoryEepromClean;
    pFactoryCmdFunc[FACTORYCMD_FPGAREG]      = cmdFactoryFpgaRegSet;
}

            
/*********************************************************************************************
函 数 名: cmdFactoryCmdProc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryCmdProc(CmdParseFrameStruct *pCmdStackFrame)
{    
    u8 dataLen = pCmdStackFrame->dataLen;
    u8 *pData = pCmdStackFrame->payload;

    
    if ((pCmdStackFrame->subType < FACTORYCMD_RESERVE) && (pFactoryCmdFunc[pCmdStackFrame->subType] != NULL))
    {    
        pFactoryCmdFunc[pCmdStackFrame->subType](dataLen, pData);
    }
}



/*******************************************文件尾********************************************/
