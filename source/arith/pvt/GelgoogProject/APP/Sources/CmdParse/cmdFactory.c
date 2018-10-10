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
#include "servLed.h"
#include "servDriver.h"

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
extern DriverInfoStruct  g_driverInfo;
extern DebugInfoStruct   g_debugInfo;
extern DigitalOutStruct  g_digitalOut;

extern bool g_bCmdPostSemToFunc;

extern FpgaWriteableRegValueStruct fpgaRegValue;



/*****************************************局部宏定义******************************************/
#define    ANALOG_IN_ADC_THRESHOLD    1000
#define    EEPROM_CLEAN_CODE          0xA5     //165，清除EEPROM数据的验证码

#define    DIST_SENSOR_CMD_LEN        3



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
SubCmdProFunc pFactoryCmdFunc[FACTORYCMD_RESERVE];



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: cmdFactoryDeviceSnSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDeviceSnSet(u8 cmdDataLen, u8 *pCmdData)
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
    //
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


#if 0
#endif


/*********************************************************************************************
函 数 名: cmdFactoryReadRegQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryReadRegQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  dataLen;
    u8  data[6];
    u16 regValue;
    u8  chanNum = *pCmdData++;
    u8  regNum  = *pCmdData++;

    
    dataLen = sizeof(chanNum) + sizeof(regNum) + sizeof(regValue);
    if (chanNum <= CH_MAX)
    {
        servFpgaRegisterRead(chanNum, regNum, &regValue);
        
        data[0] = chanNum;
        data[1] = regNum;
        data[2] = (u8)((regValue & 0xFF00) >> 8);    //高位在前，低位在后
        data[3] = (u8)(regValue & 0x00FF);
        
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_FPGARREGQ, dataLen, data);
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryConfigRegQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryConfigRegQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  dataLen;
    u8  data[6];
    u16 regValue;
    u8  chanNum = *pCmdData++;
    u8  regNum  = *pCmdData++;
    bool bSendData = true;

    
    dataLen = sizeof(chanNum) + sizeof(regNum) + sizeof(regValue);
    if (chanNum <= CH_MAX)
    {
        switch (regNum)
        {
            case 16:
                regValue = fpgaRegValue.register16.regValue;
              break;

            case 17:
                regValue = fpgaRegValue.register17.regValue;
              break;

            case 18:
                regValue = fpgaRegValue.register18.regValue;
              break;
            
            case 23:
                regValue = fpgaRegValue.register23[chanNum].regValue;
              break;
            
            case 24:
                regValue = fpgaRegValue.register24.regValue;
              break;
            
            case 25:
                regValue = fpgaRegValue.register25.regValue;
              break;
            
            case 27:
                regValue = fpgaRegValue.register27.regValue;
              break;
            
            case 28:
                regValue = fpgaRegValue.register28.regValue;
              break;

            case 29:
                regValue = fpgaRegValue.register29.regValue;
              break;
            
            case 31:
                regValue = fpgaRegValue.register31[chanNum].regValue;
              break;
            
            case 32:
                regValue = fpgaRegValue.register32[chanNum].regValue;
              break;
            
            case 33:
                regValue = fpgaRegValue.register33[chanNum].regValue;
              break;
            
            case 34:
                regValue = fpgaRegValue.register34[chanNum].regValue;
              break;
            
            case 35:
                regValue = fpgaRegValue.register35[chanNum].regValue;
              break;
            
            case 36:
                regValue = fpgaRegValue.register36[chanNum].regValue;
              break;
            
            case 37:
                regValue = fpgaRegValue.register37[chanNum].regValue;
              break;
            
            case 38:
                regValue = fpgaRegValue.register38[chanNum].regValue;
              break;
            
            case 39:
                regValue = fpgaRegValue.register39[chanNum].regValue;
              break;
            
            case 45:
                regValue = fpgaRegValue.digitalOutputReg;
              break;
            
            case 46:
                regValue = fpgaRegValue.encandUartReg;
              break;
            
            case 49:
                regValue = fpgaRegValue.trigRelatRefCfgReg[chanNum];
              break;
            
            case 50:
                regValue = fpgaRegValue.register50[chanNum].regValue;
              break;
            
            case 51:
                regValue = fpgaRegValue.sensorUart2SelectReg;
              break;

            default:
                bSendData = false;
              break;
        }

        if (bSendData)
        {
            data[0] = chanNum;
            data[1] = regNum;
            data[2] = (u8)((regValue & 0xFF00) >> 8);    //高位在前，低位在后
            data[3] = (u8)(regValue & 0x00FF);
            
            cmdFrameSend(CMD_FACTORY, FACTORYCMD_FPGAWREGQ, dataLen, data);
        }
    }
}



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
函 数 名: cmdFactoryDistBaudSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDistBaudSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;
    u8 baud1[DIST_SENSOR_CMD_LEN]   = {0xA5, 0xAE, 0x53};    //9600
    u8 baud2[DIST_SENSOR_CMD_LEN]   = {0xA5, 0xAF, 0x54};    //115200
    u8 saveCmd[DIST_SENSOR_CMD_LEN] = {0xA5, 0x25, 0xCA};    //保存
    u8 *pData;


    if (1 == *pCmdData)
    {
        pData = baud2;
    }
    else
    {
        pData = baud1;
    }
    
    for (i = 0;i < SENSOR_RESERVE;i++)
    {
        servFpgaSensorUart1TxSelect((SensorNumEnum)i);
        bspDelayMs(1);
        servSensor1UartSend(pData,   DIST_SENSOR_CMD_LEN);
        bspDelayMs(5);
        servSensor1UartSend(saveCmd, DIST_SENSOR_CMD_LEN);
        bspDelayMs(5);

        //恢复DO的功能
        servFpgaDigitalOutSet((DoutNumEnum)i, g_digitalOut.output[i]);
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryDistModeSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDistModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;
    u8 mode1[DIST_SENSOR_CMD_LEN]   = {0xA5, 0x50, 0xF5};    //长距离模式
    u8 mode2[DIST_SENSOR_CMD_LEN]   = {0xA5, 0x51, 0xF6};    //快速模式
    u8 mode3[DIST_SENSOR_CMD_LEN]   = {0xA5, 0x52, 0xF7};    //高精度模式
    u8 mode4[DIST_SENSOR_CMD_LEN]   = {0xA5, 0x53, 0xF8};    //一般模式
    
    u8 saveCmd[DIST_SENSOR_CMD_LEN] = {0xA5, 0x25, 0xCA};    //保存
    u8 *pData;


    switch (*pCmdData)
    {
        case 0:
            pData = mode1;
          break;
          
        case 1:
            pData = mode2;
          break;
          
        case 2:
            pData = mode3;
          break;
          
        case 3:
            pData = mode4;
          break;

        default:
          break;
    }
    
    for (i = 0;i < SENSOR_RESERVE;i++)
    {
        servFpgaSensorUart1TxSelect((SensorNumEnum)i);
        bspDelayMs(1);
        servSensor1UartSend(pData,   DIST_SENSOR_CMD_LEN);
        bspDelayMs(5);
        servSensor1UartSend(saveCmd, DIST_SENSOR_CMD_LEN);
        bspDelayMs(5);

        //恢复DO的功能
        servFpgaDigitalOutSet((DoutNumEnum)i, g_digitalOut.output[i]);
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryWaveInfoQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryWaveInfoQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  i;
    u8  dataLen;
    u8  totalLen;
    u8  chanNum;
    u8 *pData;
    u8  frameNum;
    u8  tempData[6];
    WaveTableTypeEnum waveTable;


    if ((sizeof(chanNum) + sizeof(waveTable)) == cmdDataLen)
    {
        chanNum   = *pCmdData++;
        waveTable = (WaveTableTypeEnum)*pCmdData;

        if ((chanNum <= CH_MAX) && (waveTable < WTTYPE_RESERVE))
        {
            pData = (u8 *)&g_waveTable[chanNum][waveTable];
            totalLen = sizeof(WaveTableStruct);

            if ((totalLen % FRACTION_FRAME_DATA_LEN_MAX) != 0)
            {
                frameNum = totalLen / FRACTION_FRAME_DATA_LEN_MAX + 1;
            }
            else
            {
                frameNum = totalLen / FRACTION_FRAME_DATA_LEN_MAX;
            }

            for (i = 1;i <= frameNum;i++)
            {
                //高4bit代表分子，所以左移4bit，低4bit代表分母
                //总共可以发送16 x 5Bytes = 80Bytes(640bits)的数据，目前没有大数据，所以不做frameNum大小的判断了
                tempData[0] = FRACTION(i, frameNum);

                if (i == frameNum)
                {
                    dataLen = totalLen % FRACTION_FRAME_DATA_LEN_MAX;    //发送剩余的
                }
                else
                {
                    dataLen = FRACTION_FRAME_DATA_LEN_MAX;            
                }
                memcpy(&tempData[1], pData, dataLen);
                pData += dataLen;
                dataLen++;    //加1是为了算上tempData[0]
                
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_WAVEINFOQ, dataLen, tempData);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryDriverDebug;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryDriverDebug(u8 cmdDataLen, u8 *pCmdData)
{
#if GELGOOG_AXIS_10
    servDriverConfig(g_driverInfo);
#endif
}


/*********************************************************************************************
函 数 名: cmdFactoryPvtPosnCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryPvtPosnCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_debugInfo.pvtPosnCount[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_debugInfo.pvtPosnCount[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_debugInfo.pvtPosnCount[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_PVTPOSNQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_debugInfo.pvtPosnCount[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_debugInfo.pvtPosnCount[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_PVTPOSNQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryPvtVelcCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryPvtVelcCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_debugInfo.pvtVelcCount[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_debugInfo.pvtVelcCount[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_debugInfo.pvtVelcCount[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_PVTVELCQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_debugInfo.pvtVelcCount[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_debugInfo.pvtVelcCount[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_PVTVELCQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryPvtTimeCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryPvtTimeCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_debugInfo.pvtTimeCount[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_debugInfo.pvtTimeCount[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_debugInfo.pvtTimeCount[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_PVTTIMEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_debugInfo.pvtTimeCount[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_debugInfo.pvtTimeCount[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_PVTTIMEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryVefyPosnCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryVefyPosnCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_debugInfo.vefyPosnCount[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_debugInfo.vefyPosnCount[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_debugInfo.vefyPosnCount[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_VEFYPOSNQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_debugInfo.vefyPosnCount[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_debugInfo.vefyPosnCount[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_VEFYPOSNQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryVefyVelcCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryVefyVelcCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_debugInfo.vefyVelcCount[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_debugInfo.vefyVelcCount[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_debugInfo.vefyVelcCount[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_VEFYVELCQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_debugInfo.vefyVelcCount[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_debugInfo.vefyVelcCount[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_VEFYVELCQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryVefyTimeCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryVefyTimeCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_debugInfo.vefyTimeCount[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_debugInfo.vefyTimeCount[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_debugInfo.vefyTimeCount[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_VEFYTIMEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_debugInfo.vefyTimeCount[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_debugInfo.vefyTimeCount[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_VEFYTIMEQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryCalcPvtCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryCalcPvtCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 *pData;
    u8 data[6];
    u8 i, j;
    u8 chanNum = *pCmdData++;

    
    dataLen = sizeof(g_debugInfo.calcPvtCount[CH1]) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        pData = (u8 *)&g_debugInfo.calcPvtCount[chanNum];
        data[0] = chanNum;
        for (i = 0;i < sizeof(g_debugInfo.calcPvtCount[chanNum]);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_CALCPVTQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                pData = (u8 *)&g_debugInfo.calcPvtCount[i];
                data[0] = i;
                for (j = 0;j < sizeof(g_debugInfo.calcPvtCount[i]);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_CALCPVTQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryCalcTimeLCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryCalcTimeLCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  dataLen;
    u8  *pData;
    u8  data[6];
    u8  i, j;
    u32 timeCount;
    u8  chanNum = *pCmdData++;

    
    dataLen = sizeof(timeCount) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        timeCount = (u32)g_debugInfo.calcTime[chanNum];
        pData = (u8 *)&timeCount;
        data[0] = chanNum;
        for (i = 0;i < sizeof(timeCount);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_CALCTIMELQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                timeCount = (u32)g_debugInfo.calcTime[i];
                pData = (u8 *)&timeCount;
                data[0] = i;
                for (j = 0;j < sizeof(timeCount);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_CALCTIMELQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryCalcTimeHCountQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryCalcTimeHCountQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  dataLen;
    u8  *pData;
    u8  data[6];
    u8  i, j;
    u32 timeCount;
    u8  chanNum = *pCmdData++;

    
    dataLen = sizeof(timeCount) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        timeCount = (u32)(g_debugInfo.calcTime[chanNum] >> 32);
        pData = (u8 *)&timeCount;
        data[0] = chanNum;
        for (i = 0;i < sizeof(timeCount);i++)
        {
            data[1 + i] = *pData++;
        }
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_CALCTIMEHQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                timeCount = (u32)(g_debugInfo.calcTime[i] >> 32);
                pData = (u8 *)&timeCount;
                data[0] = i;
                for (j = 0;j < sizeof(timeCount);j++)
                {
                    data[1 + j] = *pData++;
                }
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_CALCTIMEHQ, dataLen, data);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryPeriodOverClear;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryPeriodOverClear(u8 cmdDataLen, u8 *pCmdData)
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
    //if (PARA_VERIFY_NO_ERROR == pvrfMotorTypeVerify(cmdDataLen, pCmdData, (void *)&motorType))
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if (bConfig[i])
            {
                servFpgaPeriodOverTxClear();
                servFpgaPeriodOverDdrClear(i);
            }
        }
    }
}


/*********************************************************************************************
函 数 名: cmdFactoryPeriodOverQuery;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void cmdFactoryPeriodOverQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  dataLen;
    u8  data[6];
    u8  i;
    u8  periodState;
    u8  chanNum = *pCmdData++;

    
    dataLen = sizeof(periodState) + sizeof(chanNum);
    if (chanNum <= CH_MAX)
    {
        periodState = servFpgaPeriodStateRead(chanNum);
        
        data[0] = chanNum;
        data[1] = periodState;
        
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_PERIODSTATEQ, dataLen, data);
    }
    else
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            if ((CH_ALL == chanNum) ||
                (chanNum == g_systemInfo.group[i][0]) ||
                (chanNum == g_systemInfo.group[i][1]))
            {
                periodState = servFpgaPeriodStateRead(i);
                
                data[0] = i;
                data[1] = periodState;
                
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_PERIODSTATEQ, dataLen, data);
            }
        }
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

    pFactoryCmdFunc[FACTORYCMD_DEVICESN] = cmdFactoryDeviceSnSet;
    pFactoryCmdFunc[FACTORYCMD_TYPE]     = cmdFactoryTypeSet;
    pFactoryCmdFunc[FACTORYCMD_SOFTVERQ] = cmdFactorySoftVerQuery; 
    pFactoryCmdFunc[FACTORYCMD_FPGAVERQ] = cmdFactoryFpgaVerQuery; 
    pFactoryCmdFunc[FACTORYCMD_RESET]    = cmdFactoryResetSet;


    pFactoryCmdFunc[FACTORYCMD_FPGARREGQ] = cmdFactoryReadRegQuery;
    pFactoryCmdFunc[FACTORYCMD_FPGAWREGQ] = cmdFactoryConfigRegQuery; 

    pFactoryCmdFunc[FACTORYCMD_EEPROMCLEAN]   = cmdFactoryEepromClean;
    pFactoryCmdFunc[FACTORYCMD_FPGAREGCONFIG] = cmdFactoryFpgaRegSet;

    pFactoryCmdFunc[FACTORYCMD_DISTBAUD] = cmdFactoryDistBaudSet;
    pFactoryCmdFunc[FACTORYCMD_DISTMODE] = cmdFactoryDistModeSet;

    pFactoryCmdFunc[FACTORYCMD_WAVEINFOQ] = cmdFactoryWaveInfoQuery;
    pFactoryCmdFunc[FACTORYCMD_DRVDEBUG]  = cmdFactoryDriverDebug;
    
    pFactoryCmdFunc[FACTORYCMD_PVTPOSNQ] = cmdFactoryPvtPosnCountQuery;
    pFactoryCmdFunc[FACTORYCMD_PVTVELCQ] = cmdFactoryPvtVelcCountQuery;
    pFactoryCmdFunc[FACTORYCMD_PVTTIMEQ] = cmdFactoryPvtTimeCountQuery;
    
    pFactoryCmdFunc[FACTORYCMD_VEFYPOSNQ] = cmdFactoryVefyPosnCountQuery;
    pFactoryCmdFunc[FACTORYCMD_VEFYVELCQ] = cmdFactoryVefyVelcCountQuery;
    pFactoryCmdFunc[FACTORYCMD_VEFYTIMEQ] = cmdFactoryVefyTimeCountQuery;
    
    pFactoryCmdFunc[FACTORYCMD_CALCPVTQ]   = cmdFactoryCalcPvtCountQuery;
    pFactoryCmdFunc[FACTORYCMD_CALCTIMELQ] = cmdFactoryCalcTimeLCountQuery;
    pFactoryCmdFunc[FACTORYCMD_CALCTIMEHQ] = cmdFactoryCalcTimeHCountQuery;

    pFactoryCmdFunc[FACTORYCMD_POVERCLEAR]   = cmdFactoryPeriodOverClear;
    pFactoryCmdFunc[FACTORYCMD_PERIODSTATEQ] = cmdFactoryPeriodOverQuery;
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
