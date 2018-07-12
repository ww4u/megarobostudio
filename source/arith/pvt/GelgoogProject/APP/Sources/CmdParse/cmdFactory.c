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

extern bool g_bCmdPostSemToFunc;



/*****************************************�ֲ��궨��******************************************/
#if QUBELEY_HARDVER_2
#define    TRIGGER_IN_NUM    7     //���������ͨ����
#elif QUBELEY_HARDVER_1
#define    TRIGGER_IN_NUM    2     //���������ͨ���� 
#else
#define    TRIGGER_IN_NUM    16    //Gelgoog���������ͨ���� 
#endif
#define    ISOLATOR_IN_NUM   4     //���������ͨ����

#define    ANALOG_IN_ADC_THRESHOLD    1000
#define    EEPROM_CLEAN_CODE          0xA5     //165�����EEPROM���ݵ���֤��



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
SubCmdProFunc pFactoryCmdFunc[FACTORYCMD_RESERVE];



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: cmdFactorySnSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactorySnSet(u8 cmdDataLen, u8 *pCmdData)
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


/*********************************************************************************************
�� �� ��: cmdFactorySignatureQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactorySignatureQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8  data[FRACTION_FRAME_DATA_LEN_MAX + 1];    //���ݳ����ڼ��Ϸ�����һ���ֽ��ܹ�6���ֽ�
    u8  i;
    u32 address = 0x1FFFF7E8; //The 96-bit unique device identifier


    //96λ��12���ֽڣ���Ҫ�����η���
    //��һ��
    data[0] = FRACTION(1, 3);    //1/3
    for (i = 0;i < FRACTION_FRAME_DATA_LEN_MAX;i++)
    {
        data[i + 1] = *(__IO u8*) address++;
    }
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_SIGNATUREQ, 6, data);
    
    //�ڶ���
    data[0] = FRACTION(2, 3);    //2/3
    for (i = 0;i < FRACTION_FRAME_DATA_LEN_MAX;i++)
    {
        data[i] = *(__IO u8*) address++;
    }
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_SIGNATUREQ, 6, data);

    //���һ��
    data[0] = FRACTION(3, 3);    //3/3
    for (i = 0;i < 2;i++)    //�ٶ��������
    {
        data[i] = *(__IO u8*) address++;
    }
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_SIGNATUREQ, 3, data);
}


/*********************************************************************************************
�� �� ��: cmdFactoryDDRTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryDDRTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    //����DDR����ģʽ
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_DDRTEST;
}


/*********************************************************************************************
�� �� ��: cmdFactoryDDRTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryDDRTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_DDRTESTQ, 2, g_systemState.errorCode);
}


/*********************************************************************************************
�� �� ��: cmdFactoryDriverTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryDriverTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u32 i;
    u8  chanNum;
    WaveTableStruct *pCurrUseWaveTable;


    chanNum = *pCmdData;
    if (chanNum < g_systemState.chanNum)
    {
        //������������ģʽ
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

        //��ʼ��Ҫд��Ĳ���(תһȦ���ⲿ�̶��Ӽ���10:1�ĵ����ϸ��64��һȦ����128000΢����ѭ����126)
        servFpgaWaveReset(pCurrUseWaveTable, WTWORKMODE_CYCLE);
        
        for (i = 0;i < OUTPUT_DATA_BUFFER_SIZE;i++)
        {
            //2500 * 50ns = 125us
            g_outputData[chanNum].data[i] = (0x02 << 27) | (2500 & 0x01FFFFFF) | 0x80000000;    //�����������ݣ�������Ч
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
�� �� ��: cmdFactoryDriverTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryDriverTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_DRIVERTESTQ, 3, g_systemState.errorCode);
}


/*********************************************************************************************
�� �� ��: cmdFactoryTrigInTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryTrigInTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 data;

    
    //���봥���������ģʽ
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_TRIGINTEST;

    //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽţ���صĹܽ��ɲ��԰�ȫ������
    data = (u8)LEVEL_HIGH;
    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
    servTxFrameProcess();    //ǿ�Ʒ���
    
    bspDelayMs(10);    //������ʱ���ѯ��ƽ
    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
�� �� ��: cmdFactoryTrigInTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryTrigInTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINTESTQ, 5, g_systemState.errorCode);
}


/*********************************************************************************************
�� �� ��: cmdFactoryTrigOutTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryTrigOutTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;

    
    //���봥���������ģʽ
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_TRIGOUTTEST;

    //ͨ��FPGA���ùܽ����Ϊ��
    for (i = 0;i < DIO_RESERVE;i++)
    {
        servFpgaDigitalOutLevelSet((DoutNumEnum)i, LEVEL_HIGH);
    }
    g_systemState.levelStatus = LEVEL_HIGH;
    
    bspDelayMs(10);    //������ʱ���ѯ��ƽ
    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGOUTENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
�� �� ��: cmdFactoryTrigOutTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryTrigOutTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_TRIGOUTTESTQ, 5, g_systemState.errorCode);
}


/*********************************************************************************************
�� �� ��: cmdFactoryEncoderTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryEncoderTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    //���봥���������ģʽ
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_ENCODERTEST;

    //�������������
    servFpgaEncoderCountReset(CH1);

    //���ñ���Ϊ4
    servFpgaEncoderSet(CH1, INTFC_ON, ENCMULT_QUADR, ECCHAN_3);

    servFrameSend(CMD_FACTORY, FACTORYCMD_ENCODERSTART, 0, NULL, LINK_CAN);
    servTxFrameProcess();    //ǿ�Ʒ���
    
    bspDelayMs(1000);    //������ʱ���ѯ��ƽ
}


/*********************************************************************************************
�� �� ��: cmdFactoryEncoderTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryEncoderTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_ENCODERTESTQ, 3, g_systemState.errorCode);
}


/*********************************************************************************************
�� �� ��: cmdFactoryIsolatorInTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryIsolatorInTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 data;

    
    //���봥���������ģʽ
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_ISOLINTEST;

    //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽţ���صĹܽ��ɲ��԰�ȫ������
    data = (u8)LEVEL_HIGH;
    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
    servTxFrameProcess();    //ǿ�Ʒ���
    
    bspDelayMs(10);    //������ʱ���ѯ��ƽ
    servFrameSend(CMD_FACTORY, FACTORYCMD_ISOLINENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
�� �� ��: cmdFactoryIsolatorInTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryIsolatorInTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_ISOLINTESTQ, 5, g_systemState.errorCode);
}


/*********************************************************************************************
�� �� ��: cmdFactoryIsolatorOutTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryIsolatorOutTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 i;

    
    //���봥���������ģʽ
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_ISOLOUTTEST;

    //ͨ��FPGA���ùܽ����Ϊ��
    for (i = 0;i < DIO_RESERVE;i++)
    {
        servFpgaIsolatorOutLevelSet((YoutNumEnum)i, LEVEL_LOW);
    }
    g_systemState.levelStatus = LEVEL_HIGH;
    
    bspDelayMs(10);    //������ʱ���ѯ��ƽ
    servFrameSend(CMD_FACTORY, FACTORYCMD_ISOLOUTENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
�� �� ��: cmdFactoryIsolatorOutTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryIsolatorOutTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_ISOLOUTTESTQ, 5, g_systemState.errorCode);
}


#ifdef PROJECT_QUBELEY
/*********************************************************************************************
�� �� ��: cmdFactoryAnalogInTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryAnalogInTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    u8 data;

    
    //����ģ���������ģʽ
    memset(g_systemState.errorCode, 0, ERROR_CODE_LEN);
    g_systemState.errorCode[0] = 0xFF;
    g_systemState.EnginMode = ENGINMODE_ANALOGINTEST;

    //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽ�
    data = (u8)LEVEL_HIGH;

    servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
    servTxFrameProcess();    //ǿ�Ʒ���

    bspDelayMs(10);    //������ʱ���ѯ��ƽ
    servFrameSend(CMD_FACTORY, FACTORYCMD_ANALOGINENDQ, 0, NULL, LINK_CAN);
}


/*********************************************************************************************
�� �� ��: cmdFactoryAnalogInTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryAnalogInTestQuery(u8 cmdDataLen, u8 *pCmdData)
{
    cmdFrameSend(CMD_FACTORY, FACTORYCMD_ANALOGINTESTQ, 2, g_systemState.errorCode);
}
#endif


/*********************************************************************************************
�� �� ��: cmdFactorySensorUartTestSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactorySensorUartTestSet(u8 cmdDataLen, u8 *pCmdData)
{
    
}


/*********************************************************************************************
�� �� ��: cmdFactorySensorUartTestQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactorySensorUartTestQuery(u8 cmdDataLen, u8 *pCmdData)
{

}


#if 0
#endif
/*********************************************************************************************
�� �� ��: cmdFactoryDriverEndQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: cmdFactoryTrigInEndQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //�ȴ�FPGA��ȡ״̬
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

        //ÿ��������ռ����bit��bit1�����Ƿ��д˲����bit0������Խ��: 1-���Գ���; 0-��������
        errorCode |= (2 << (i * 2));    //�д˲�����
        if (readStatus != levelStatus)
        {
            sameState  = false;
            errorCode |= (1 << (i * 2));    //���Գ���
        }
    }

    if (sameState)    //״̬һ��������͵�ƽ���ԣ���һ�����¼����ֹͣ����
    {
        switch (levelStatus)
        {
            case LEVEL_LOW:
                testResult = (u8 *)&errorCode;
                for (i = 0;i < sizeof(errorCode);i++)
                {
                    g_systemState.errorCode[1 + i] = *testResult++;
                }
                g_systemState.EnginMode = ENGINMODE_NONE;    //��������
                g_systemState.errorCode[0] = 0;
              break;

            case LEVEL_HIGH:
                //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽ�
                data = (u8)LEVEL_LOW;
                
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
                servTxFrameProcess();    //ǿ�Ʒ���
                
                bspDelayMs(10);    //������ʱ���ѯ��ƽ
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINENDQ, 0, NULL, LINK_CAN);
              break;

            default:
              break;
        }
    }
    else
    {
        //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽ�
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
�� �� ��: cmdFactoryTrigOutEndQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryTrigOutEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   i;
    u8   allPinState;
    u8  *testResult;
    u32  errorCode = 0;
    bool sameState = true;
    LevelStatusEnum readStatus;     //��FPGA��ȡ��״̬
    LevelStatusEnum levelStatus;    //�Ӳ����豸�ض���״̬


    //�ȴ�FPGA��ȡ״̬
    allPinState = *pCmdData;
    for (i = 0;i < DIO_RESERVE;i++)
    {
        //readStatus = servFpgaDioStatusRead((DigitalIONumEnum)((u8)DIGITALIO_DO1 + i));
        levelStatus = (LevelStatusEnum)((allPinState >> i) & 0x01);
        readStatus  = levelStatus;

        //ÿ��������ռ����bit��bit1�����Ƿ��д˲����bit0������Խ��: 1-���Գ���; 0-��������
        errorCode |= (2 << (i * 2));    //�д˲�����
        if ((readStatus != levelStatus) || (readStatus != g_systemState.levelStatus))
        {
            sameState  = false;
            errorCode |= (1 << (i * 2));    //���Գ���
        }
    }

    //����״̬(���ú�ʵ�����������ͻض�)��һ��������͵�ƽ���ԣ���һ�����¼����ֹͣ����
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
                //ͨ��FPGA���ùܽ����Ϊ��
                for (i = 0;i < DIO_RESERVE;i++)
                {
                    servFpgaDigitalOutLevelSet((DoutNumEnum)i, LEVEL_LOW);
                }
                g_systemState.levelStatus = LEVEL_LOW;
                
                bspDelayMs(10);    //������ʱ���ѯ��ƽ
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGOUTENDQ, 0, NULL, LINK_CAN);
              break;

            default:
              break;
        }
    }
    else
    {
        //ͨ��FPGA���ùܽ����Ϊ��
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
�� �� ��: cmdFactoryEncoderEndQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryEncoderEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    s16 channelZCount;
    s32 channelABCount;


    //��������������DEMO�巢��200�����壬����������4
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
�� �� ��: cmdFactoryIsolatorInEndQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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

    
    //�ȴ�FPGA��ȡ״̬
    levelStatus = (LevelStatusEnum)*pCmdData;
    for (i = DIGITALIO_XI1;i < (DIGITALIO_XI1 + ISOLATOR_IN_NUM);i++)
    {
        readStatus = (LevelStatusEnum)(LEVEL_HIGH - servFpgaDioStatusRead((DigitalIONumEnum)i));

        //ÿ��������ռ����bit��bit1�����Ƿ��д˲����bit0������Խ��: 1-���Գ���; 0-��������
        errorCode |= (2 << ((i - DIGITALIO_XI1) * 2));    //�д˲�����
        if (readStatus != levelStatus)
        {
            sameState  = false;
            errorCode |= (1 << ((i - DIGITALIO_XI1) * 2));    //���Գ���
        }
    }

    if (sameState)    //״̬һ��������͵�ƽ���ԣ���һ�����¼����ֹͣ����
    {
        switch (levelStatus)
        {
            case LEVEL_LOW:
                testResult = (u8 *)&errorCode;
                for (i = 0;i < sizeof(errorCode);i++)
                {
                    g_systemState.errorCode[1 + i] = *testResult++;
                }
                g_systemState.EnginMode = ENGINMODE_NONE;    //��������
                g_systemState.errorCode[0] = 0;
              break;

            case LEVEL_HIGH:
                //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽ�
                data = (u8)LEVEL_LOW;
                
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
                servTxFrameProcess();    //ǿ�Ʒ���
                
                bspDelayMs(10);    //������ʱ���ѯ��ƽ
                servFrameSend(CMD_FACTORY, FACTORYCMD_ISOLINENDQ, 0, NULL, LINK_CAN);
              break;

            default:
              break;
        }
    }
    else
    {
        //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽ�
        data = (u8)LEVEL_LOW;
                
        servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
        
        testResult = (u8 *)&errorCode;
        for (i = 0;i < sizeof(errorCode);i++)
        {
            g_systemState.errorCode[1 + i] = *testResult++;
        }
        g_systemState.EnginMode = ENGINMODE_NONE;    //��������
        g_systemState.errorCode[0] = 0;
    }
}


/*********************************************************************************************
�� �� ��: cmdFactoryIsolatorOutEndQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryIsolatorOutEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8   i;
    u8   allPinState;
    u8  *testResult;
    u32  errorCode = 0;
    bool sameState = true;
    LevelStatusEnum readStatus;     //��FPGA��ȡ��״̬
    LevelStatusEnum levelStatus;    //�Ӳ����豸�ض���״̬


    //�ȴ�FPGA��ȡ״̬
    allPinState = *pCmdData;
    for (i = 0;i < YOUT_RESERVE;i++)
    {
        //readStatus = servFpgaDioStatusRead((DigitalIONumEnum)((u8)DIGITALIO_YO1 + i));
        levelStatus = (LevelStatusEnum)((allPinState >> (i + 2)) & 0x01);
        readStatus  = levelStatus;

        //ÿ��������ռ����bit��bit1�����Ƿ��д˲����bit0������Խ��: 1-���Գ���; 0-��������
        errorCode |= (2 << (i * 2));    //�д˲�����
        if ((readStatus != levelStatus) || (readStatus != g_systemState.levelStatus))
        {
            sameState  = false;
            errorCode |= (1 << (i * 2));    //���Գ���
        }
    }

    //����״̬(���ú�ʵ�����������ͻض�)��һ��������͵�ƽ���ԣ���һ�����¼����ֹͣ����
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
                g_systemState.EnginMode = ENGINMODE_NONE;    //��������
                g_systemState.errorCode[0] = 0;
              break;

            case LEVEL_HIGH:
                //ͨ��FPGA���ùܽ����Ϊ��
                for (i = 0;i < YOUT_RESERVE;i++)
                {
                    servFpgaIsolatorOutLevelSet((YoutNumEnum)i, LEVEL_HIGH);
                }
                g_systemState.levelStatus = LEVEL_LOW;
                
                bspDelayMs(10);    //������ʱ���ѯ��ƽ
                servFrameSend(CMD_FACTORY, FACTORYCMD_ISOLOUTENDQ, 0, NULL, LINK_CAN);
              break;

            default:
              break;
        }
    }
    else
    {
        //ͨ��FPGA���ùܽ����Ϊ��
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
        g_systemState.EnginMode = ENGINMODE_NONE;    //��������
        g_systemState.errorCode[0] = 0;
    }
}


#ifdef PROJECT_QUBELEY
/*********************************************************************************************
�� �� ��: cmdFactoryAnalogInEndQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactoryAnalogInEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    u8 data;
    u16 dacValue;
    LevelStatusEnum levelStatus;


    //�ȴ�ADC��ȡ����ֵ
    dacValue = drvAdcGetAverage(ANALOG_IN_ADC_CH, 1);

    levelStatus = *(LevelStatusEnum *)pCmdData;
    switch (levelStatus)
    {
        case LEVEL_LOW:
            if (dacValue < ANALOG_IN_ADC_THRESHOLD)
            {
                g_systemState.EnginMode = ENGINMODE_NONE;    //��������
                g_systemState.errorCode[0] = 0;
            }
            else
            {
                //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽ�
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
                //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽ�
                data = (u8)LEVEL_LOW;    //1����ߵ�ƽ��0����͵�ƽ
                
                servFrameSend(CMD_FACTORY, FACTORYCMD_TRIGINSTART, 1, &data, LINK_CAN);
                servTxFrameProcess();    //ǿ�Ʒ���
                
                bspDelayMs(10);    //������ʱ���ѯ��ƽ
                servFrameSend(CMD_FACTORY, FACTORYCMD_ANALOGINENDQ, 0, NULL, LINK_CAN);
            }
            else
            {
                //ͨ��CAN����֪ͨ�����豸������Ӧ�ܽ�
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
�� �� ��: cmdFactorySensorUartEndQuery;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void cmdFactorySensorUartEndQuery(u8 cmdDataLen, u8 *pCmdData)
{
    
}


#if 0
#endif
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

    //�����ⲿ�������Ǵ����豸�Ͳ����豸֮�����ͨ���õģ�������λ������ͨ��
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
    //�����ⲿ�������Ǵ����豸�Ͳ����豸֮�����ͨ���õģ�������λ������ͨ��

    pFactoryCmdFunc[FACTORYCMD_EEPROMCLEAN]  = cmdFactoryEepromClean;
    pFactoryCmdFunc[FACTORYCMD_FPGAREG]      = cmdFactoryFpgaRegSet;
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
