/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  cmdFactory.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.06.23;
��ʷ�汾:  ��;
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


/****************************************�ⲿ��������*****************************************/\
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



/*****************************************�ֲ��궨��******************************************/
#define    ANALOG_IN_ADC_THRESHOLD    1000
#define    EEPROM_CLEAN_CODE          0xA5     //165�����EEPROM���ݵ���֤��

#define    DIST_SENSOR_CMD_LEN        3



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pFactoryCmdFunc[FACTORYCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdFactoryDeviceSnSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryDeviceSnSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 dataLen;
    u8 dataOffset;
    u8 frameNum;
    u8 totalNum;
    u8 fraction;
    static u8 deviceSn[DEVICE_SN_LEN];


    if ((cmdDataLen >= 2) && (cmdDataLen <= 6))    //����Ҫ��2-6֮��(����������1�����ݣ�����5������)
    {
        fraction = *pCmdData++;
        frameNum = (fraction & 0xF0) >> 4;    //��4bit������ӣ���������4bit����4bit�����ĸ
        totalNum = (fraction & 0x0F);

        //����ƫ�ƺ����ݳ���
        dataLen    = cmdDataLen - 1;    //�����Ƿ���ռ��1���ֽ�
        dataOffset = (frameNum - 1) * FRACTION_FRAME_DATA_LEN_MAX;
        memcpy((deviceSn + dataOffset), pCmdData, dataLen);
        
        if (frameNum == totalNum)
        {
            //�洢��EEPROM��FLASH��    //Ŀǰ��δ���ܴ洢��������Ҫ��Ϊ���ܴ洢    NICK MARK
            memcpy(g_deviceInfo.storageInfo.deviceSn, deviceSn, DEVICE_SN_LEN);
            servDeviceInfoStore(&g_deviceInfo.storageInfo);

            //ǰ��дEEprom��ʱ���Ѿ�������CRC�������ټ�����    FOR MODIFY NICK
            //bspFlashWrite(FLASH_SN_ADDR, (u8 *)&g_deviceInfo, sizeof(SnEepromStruct));
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdFactoryTypeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryTypeSet(u8 cmdDataLen, u8 *pCmdData)
{
    //
}


/*********************************************************************************************
�� �� ��: cmdFactorySoftVerQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryFpgaVerQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryResetSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryResetSet(u8 cmdDataLen, u8 *pCmdData)
{
    bspSoftReset();
}


#if 0
#endif


/*********************************************************************************************
�� �� ��: cmdFactoryReadRegQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
        data[2] = (u8)((regValue & 0xFF00) >> 8);    //��λ��ǰ����λ�ں�
        data[3] = (u8)(regValue & 0x00FF);
        
        cmdFrameSend(CMD_FACTORY, FACTORYCMD_FPGARREGQ, dataLen, data);
    }
}


/*********************************************************************************************
�� �� ��: cmdFactoryConfigRegQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
            data[2] = (u8)((regValue & 0xFF00) >> 8);    //��λ��ǰ����λ�ں�
            data[3] = (u8)(regValue & 0x00FF);
            
            cmdFrameSend(CMD_FACTORY, FACTORYCMD_FPGAWREGQ, dataLen, data);
        }
    }
}



/*********************************************************************************************
�� �� ��: cmdFactoryEepromClean;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryEepromClean(u8 cmdDataLen, u8 *pCmdData)
{
    u8  cleanCode = *pCmdData;


    if (EEPROM_CLEAN_CODE == cleanCode)    //��ֵ��ȷ�Ͱ�����ֵд��Ĭ��ֵ
    {
        SystemParaReset();
    }
}


/*********************************************************************************************
�� �� ��: cmdFactoryFpgaRegSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryDistBaudSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryDistBaudSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;
    u8 baud1[DIST_SENSOR_CMD_LEN]   = {0xA5, 0xAE, 0x53};    //9600
    u8 baud2[DIST_SENSOR_CMD_LEN]   = {0xA5, 0xAF, 0x54};    //115200
    u8 saveCmd[DIST_SENSOR_CMD_LEN] = {0xA5, 0x25, 0xCA};    //����
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

        //�ָ�DO�Ĺ���
        servFpgaDigitalOutSet((DoutNumEnum)i, g_digitalOut.output[i]);
    }
}


/*********************************************************************************************
�� �� ��: cmdFactoryDistModeSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryDistModeSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;
    u8 mode1[DIST_SENSOR_CMD_LEN]   = {0xA5, 0x50, 0xF5};    //������ģʽ
    u8 mode2[DIST_SENSOR_CMD_LEN]   = {0xA5, 0x51, 0xF6};    //����ģʽ
    u8 mode3[DIST_SENSOR_CMD_LEN]   = {0xA5, 0x52, 0xF7};    //�߾���ģʽ
    u8 mode4[DIST_SENSOR_CMD_LEN]   = {0xA5, 0x53, 0xF8};    //һ��ģʽ
    
    u8 saveCmd[DIST_SENSOR_CMD_LEN] = {0xA5, 0x25, 0xCA};    //����
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

        //�ָ�DO�Ĺ���
        servFpgaDigitalOutSet((DoutNumEnum)i, g_digitalOut.output[i]);
    }
}


/*********************************************************************************************
�� �� ��: cmdFactoryWaveInfoQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
                //��4bit������ӣ���������4bit����4bit�����ĸ
                //�ܹ����Է���16 x 5Bytes = 80Bytes(640bits)�����ݣ�Ŀǰû�д����ݣ����Բ���frameNum��С���ж���
                tempData[0] = FRACTION(i, frameNum);

                if (i == frameNum)
                {
                    dataLen = totalLen % FRACTION_FRAME_DATA_LEN_MAX;    //����ʣ���
                }
                else
                {
                    dataLen = FRACTION_FRAME_DATA_LEN_MAX;            
                }
                memcpy(&tempData[1], pData, dataLen);
                pData += dataLen;
                dataLen++;    //��1��Ϊ������tempData[0]
                
                cmdFrameSend(CMD_FACTORY, FACTORYCMD_WAVEINFOQ, dataLen, tempData);
            }
        }
    }
}


/*********************************************************************************************
�� �� ��: cmdFactoryDriverDebug;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryDriverDebug(u8 cmdDataLen, u8 *pCmdData)
{
#if GELGOOG_AXIS_10
    servDriverConfig(g_driverInfo);
#endif
}


/*********************************************************************************************
�� �� ��: cmdFactoryPvtPosnCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryPvtVelcCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryPvtTimeCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryVefyPosnCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryVefyVelcCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryVefyTimeCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryCalcPvtCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryCalcTimeLCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryCalcTimeHCountQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryPeriodOverClear;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
    
    //���в�����֤
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
�� �� ��: cmdFactoryPeriodOverQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryCmdInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryCmdProc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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



/*******************************************�ļ�β********************************************/
