/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  systemInit.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.01.11;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <string.h>
#include <os.h>
#include "project.h"
#include "systemInit.h"
#include "pvtProcTask.h"
#include "cmdParseTask.h"
#include "functionTask.h"
#include "periodicTask.h"
#include "eventManageTask.h"
#include "paraSaveTask.h"
#include "servAlgorithm.h"
#include "bspCan.h"
#include "bspAdc.h"
#include "bspSpi.h"
#include "bspUart.h"
#include "bspGpio.h"
#include "bspSdio.h"
#include "bspTimer.h"
#include "bspCrc32.h"
#include "bspEeprom.h"
#include "servSystemPara.h"
#include "pvrfMainVerify.h"
#include "cmdMainParse.h"
#include "servSoftTimer.h"
#include "servCommIntfc.h"
#include "servLed.h"
#include "servFpga.h"
#include "servDriver.h"
#include "servDeviceInfo.h"

#if MRV_SUPPORT
#include "servValve.h"
#endif

#ifdef PROJECT_QUBELEY
#include "servPT100.h"
#endif



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/
#if QUBELEY_HARDVER_1
#define FPGA_DATA_PATH "D:\\Work\\FPGA_VERSION\\fpga_top_v0003.bin"
#else
#define FPGA_DATA_PATH "E:\\Daily Backup\\Version\\gelgoog_fpga_top_wrapper.bin"
#endif

#define    SYSTEM_SOFTWARE_BRANCH_VER_INDEX    0 
#define    SYSTEM_SOFTWARE_MAJOR_VER_INDEX     1
#define    SYSTEM_SOFTWARE_MINOR_VER_INDEX     2
#define    SYSTEM_SOFTWARE_BUILD_VER_INDEX     3

#define    SYSTEM_PDM_ENC_DIV_DEFAULT          8



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
//�߳��ñ���
static OS_TCB  SystemInitTaskTCB;
static CPU_STK SystemInitTaskStk[APP_TASK_STK_SIZE_SYSTEM_INIT];

static OS_TCB  CmdParseTaskTCB;
static CPU_STK CmdParseTaskStk[APP_TASK_STK_SIZE_CMD_PARSE];

static OS_TCB  PvtProcTaskTCB;
static CPU_STK PvtProcTaskStk[APP_TASK_STK_SIZE_PVT_CALC];

static OS_TCB  FunctionTaskTCB;
static CPU_STK FunctionTaskStk[APP_TASK_STK_SIZE_FUNCTION];

static OS_TCB  PeriodicTaskTCB;
static CPU_STK PeriodicTaskStk[APP_TASK_STK_SIZE_PERIODIC];

static OS_TCB  EventManageTaskTCB;
static CPU_STK EventManageTaskStk[APP_TASK_STK_SIZE_EVENT_MANAGE];

static OS_TCB  ParaSaveTaskTCB;
static CPU_STK ParaSaveTaskStk[APP_TASK_STK_SIZE_PARA_SAVE];

OS_SEM    g_semCmdParseTask;
OS_SEM    g_semPvtCalcTask;
OS_SEM    g_semFunctionTask;
OS_SEM    g_semPeriodicTask;
OS_SEM    g_semEventManageTask;
OS_SEM    g_semParaSaveTask;

OS_MUTEX  g_mutexSdioInterface;



/*ϵͳ�����ñ���**************************************************************/
//1. �Ǵ洢�����
SystemStateStruct  g_systemState;
OutputDataStruct   g_outputData[CH_TOTAL];    //PVTCALC�̺߳�FUNC�̹߳��ô˱���
CanIntfcStruct     g_ciCanInterface;          //�ӿ����ò��������ú�Ӧ��֮��������ʱ����޸ĺ�Ĳ���ֵ   
UartIntfcStruct    g_ciUartInterface;
UartIntfcStruct    g_sensorUartIntfc[UARTNUM_RESERVE];
SensorDataStruct   g_sensorData;

EventSrcBmpStruct  g_eventSrcBmp;
SystemCfgBmpStruct g_systemCfgBmp;
ChanCfgBmpStruct   g_chanCfgBmp[CH_TOTAL];
WaveTableStruct    g_waveTable[CH_TOTAL][WTTYPE_RESERVE];    //�����ַ

//2. �洢�����
/*�������Ǻ�BOOT���õģ����Դ洢��ַ������ĳ�Ա������������޸�!!!*/
BootInfoStruct    g_bootInfo;
CommIntfcStruct   g_commIntfc;
/*�������Ǻ�BOOT���õģ����Դ洢��ַ������ĳ�Ա������������޸�!!!*/

/*PVT������ݷ�Ϊ������************************/
//21. �滮��ز���(�����˲���Ľ��㣬����ֹͣ�����Լ�ʧ����)��
PlanInfoStruct    g_planInfo;

//22. PVT��(��Ϊռ���ڴ�Ƚϴ󣬶���ʱ������CCRAM��)
#ifdef PROJECT_GELGOOG
#pragma location = "PVTPOINT_SECTION"
#endif

PvtInfoStruct  pvtPointBuffer[CH_TOTAL];

//23. PVT�������(��Ҫ�ǽ��������ʹ�õ��м���������Է�����g_systemState��)
/*PVT������ݷ�Ϊ������************************/

DeviceInfoStruct  g_deviceInfo;
SystemInfoStruct  g_systemInfo;
MotorInfoStruct   g_motorInfo; 
MotionInfoStruct  g_motionInfo;
ReportInfoStruct  g_reportInfo;
TrigInInfoStruct  g_trigInInfo;
DigitalOutStruct  g_digitalOut;
IsolatorOutStruct g_isolatorOut;
SensorUartStruct  g_sensorUart;
DriverInfoStruct  g_driverInfo;

#ifdef PROJECT_QUBELEY
AnalogInfoStruct g_analogInfo;
#endif

#ifdef PROJECT_GELGOOG

#if GELGOOG_SINANJU
SensorAlarmStruct g_sensorAlarm;

#else

IsolatorInStruct  g_isolatorIn;
#endif

#endif
/*ϵͳ�����ñ���**************************************************************/


/*SINANJU��ŷ��ñ���*********************************************************/
#if MRV_SUPPORT
//��ŷ����ò���
MrvCtrlParaStruct g_mrvCtrlPara;

//��ŷ��滮����
MrvPlanInfoStruct g_mrvPlanInfo;

#pragma location = "PVTPOINT_SECTION"
PvtInfoStruct  mrvPvtPointBuffer[MRV_CH_TOTAL_NUM];

//��ŷ�����
MrvWaveTableStruct g_mrvWaveTable[WTTYPE_RESERVE];

MrvChanCfgBmpStruct g_mrvChanCfgBmp;
#endif
/*SINANJU��ŷ��ñ���*********************************************************/


/*�����ñ���******************************************************************/
DebugInfoStruct g_debugInfo;

#if GELGOOG_SINANJU
u8 pdmMstepDataBuffer[CH_TOTAL][PDM_MSTEP_BUFFER_SIZE];
#endif
/*�����ñ���******************************************************************/


/*Ĭ�����ò���****************************************************************/
SystemInfoStruct  systemInfoDefault = 
{
    .workMode = WORK_NORMAL,
    .powerOn  = POWERON_DEFAULT,
    
    .revMotion = SENSOR_OFF,
    
    /*.group = {GROUP_NUM_VALUE_MIN, 
              GROUP_NUM_VALUE_MIN},*/
              
#if GELGOOG_SINANJU
    .fanDuty = 50,       //50%ռ�ձ�
    .armLedDuty = {50, 50, 50, 50},
            
    .fanFrequency = 1000,    //1KHzƵ��
    .armLedFreq = {100, 100, 100, 100},
#endif

    .otpInfo = 
    {
        .state     = SENSOR_ON,
        .response  = RESPONSE_ALARM,
        .threshold = 10000    //��λ0.01��
    },
    
    .fpgaClockOffset = 0,
};

//�ӿ�Ĭ������
/*
CAN ID����Ĭ�Ϲ���(���鵫��ǿ��):
    �㲥ID:000000 ~7E //������ȼ�,����Ϊ0x7F,ID���7λ����ȫ'1',���ܵ����ٲ�ʧ��.
    ��ID  :0x80~0xFD   //��125��,�θ����ȼ�
    ����ID:0x100~0x17D   //��125��,can���ߵ�����������CAN�ڵ����100���
    ����ID:0x180~0x1FB //��124��
*/
CommIntfcStruct commIntfcDefault =
{
    .linkType = LINK_CAN,
    
    .canIntfc = 
    {
        .idType    = CANTYPE_B,
        .group     = 0,
        .baud      = CANBAUD_1000,
        .sendId    = 0x00000200,    /*384~509*/
        .receiveId = 0x00000180,    /*256~381*/
        .groupId1  = 0x00000080,    /*128~253*/
        .groupId2  = 0x00000100, 
        .radioId   = 0x00000001,    /*0~126*/ 
    },
    
    .uartIntfc = 
    {
        .baud    = UARTBAUD_115200,
        .wordLen = WORDLEN_8,
        .stopBit = STOPBIT_10,
        .parity  = PARITY_NONE,
        .flowCtl = FLOWCTL_NONE,
    },
    
    .crc = 0
};

//���Ĭ�ϲ���
MotorManageStruct motorInfoDefault =
{
    .motorType  = MOTOR_ROTARY,       //����
    .stepAngel  = STEPANGLE_18,       //�����
    .microSteps = MICROSTEP_32,        //΢����
    .motorSize  = MOTOR_23,            //����ߴ�

    .encoderState   = INTFC_ON,        //״̬
    .encoderType    = ENCODER_INC,     //����
    .encoderChanNum = ECCHAN_3,        //ͨ����
    .encoderMult    = ENCMULT_SINGLE,  //����������
    
    .posnUnit  = POSTUNIT_ANGLE,       //ϵͳ���й�����ʹ�õĵ�λ
    .motorVolt = 24,                   //�����ѹ
    .motorCurr = 10,                   //�������
    .feedbackRatio = 1,               //�������������� 
    
    .gearRatioNum = 1,                  //���ٱȷ���
    .gearRatioDen = 1,                  //���ٱȷ�ĸ
    .lead = 10,                         //ֱ���˶�ʱ�ĵ��̣���λmm/cycle
    .peakSpeed = 5,                     //��������ٶ�
    .peakAcc = 5,                       //�����ٶ�    CJ 2017.05.24 Add

    .backlash = 0.0,

#if GELGOOG_SINANJU
    .encoderLineNum = 8192,      //����
#else
    .encoderLineNum = 2000,      //����
#endif
};

MotionManageStruct motionInfoDefault = 
{
    .stateReport  = STATEQRY_ACTIVE,
    .initPostUnit = POSTUNIT_ANGLE, 
#ifdef PROJECT_QUBELEY
    .initIOPin = TRIGPIN_CHANZ,
#endif
    
#ifdef PROJECT_GELGOOG
    .initIOPin = TRIGPIN_DIL,
#endif
    .offsetState = SENSOR_OFF,

    .startSrc  = SSRC_SOFT,
    .startType = RECEIVE_RADIOID,
    
    .maxSpeed = 0,
    .minSpeed = 0,
    .maxPosn = 0,
    .minPosn = 0,
    .maxTorque = 0,
    .minTorque = 0,
    .maxAcc = 0,
    .minAcc = 0,
    .origin = 0
};

ReportManageStruct reportInfoDefault = 
{
    //״̬
    .state = 
    {
        SENSOR_OFF,
        SENSOR_OFF,
        SENSOR_OFF,
        SENSOR_OFF,
        SENSOR_OFF,
        SENSOR_OFF
    },
    
    //����
    .period = 
    {
        1000,
        1000,
        1000,
        1000,
        1000,
        1000
    },
};

PlanManageStruct planInfoDefault =
{
    .execMode   = EXECMODE_NCYCLE,
    .planMode   = PLANMODE_CUBIC,
    .motionMode = MTNMODE_PVT,
    .modifyDuty = MDUTY_125,
    
    .endState   = ENDSTATE_STOP,
    .validPoint = 0,
    .warnPoint  = 0,
    
    .cycleNum = 1,
    
    .stopDecelInfo =
    {
        .stopMode = STOPMODE_IMMED,
    
        .distance = 0.0,
        .time = 0.0,
    },
    
    .outOfStepInfo = 
    {
        .lineState  = SENSOR_OFF,
        .totalState = SENSOR_OFF,
        
        .lineResp  = RESPONSE_NONE,
        .totalResp = RESPONSE_NONE,
        
        .lineOutNum  = 32,
        .totalOutNum = 32,
    }      
};

TrigInManageStruct trigInInfoDefault = 
{
    .trigMode = TRIGMODE_LEVEL,
    
    .pattState    = SENSOR_OFF,
    .pattResponse = RESPONSE_NONE,
    .pattSMode    = SMODE_CONTINUED,
    .pattSPeriod  = 0.01,
    
    .levelState= 
    {
        SENSOR_OFF,
        SENSOR_OFF,

#ifdef PROJECT_QUBELEY
        SENSOR_OFF,
        SENSOR_OFF,
        SENSOR_OFF,
#endif
    },
    .levelResponse= 
    {
        RESPONSE_NONE,
        RESPONSE_NONE,

#ifdef PROJECT_QUBELEY
        RESPONSE_NONE,
        RESPONSE_NONE,
        RESPONSE_NONE,
#endif
    },
    
    .levelSMode = 
    {
        SMODE_CONTINUED,
        SMODE_CONTINUED,

#ifdef PROJECT_QUBELEY
        SMODE_CONTINUED,
        SMODE_CONTINUED,
        SMODE_CONTINUED,
#endif
    },
    
    .type = 
    {
        TRIGINTYPE_RISE,
        TRIGINTYPE_RISE,

#ifdef PROJECT_QUBELEY
        TRIGINTYPE_RISE,
        TRIGINTYPE_RISE,
        TRIGINTYPE_RISE,
#endif
    },
    
    .levelSPeriod = 
    {
        0.01,
        0.01,

#ifdef PROJECT_QUBELEY
        0.01,
        0.01,
        0.01,
#endif
    }
};

DigitalOutManageStruct digitalIODefault = 
{
    .state  = SENSOR_OFF,
    .source = CH1,
    
    .condition = DIOCONDT_AUTO,
    .signal    = DIOSIGNAL_LEVEL,
    .polarity  = DIOPOLARITY_P,
    
    .duty   = 50,
    .period = 1000 
};

IsolatorOutManageStruct isolatorOutDefault = 
{
    .state     = SENSOR_OFF,
    .source    = CH1,
    .condition = DIOCONDT_AUTO,
    .response  = DIOPOLARITY_N
};

#if GELGOOG_SINANJU
SensorManageStruct angleSensorDefault = 
{
    .state    = SENSOR_OFF,
    .SOF      = 0x1A,
    .frameLen = 12,
    .recvNum  = 1,
    .swTime   = 30,      //ms
};

#else

SensorManageStruct angleSensorDefault = 
{
    .state    = SENSOR_OFF,
    .SOF      = 0x55,
    .frameLen = 11,
    .recvNum  = 3,
    .swTime   = 30,      //ms
};
#endif

SensorManageStruct distSensorDefault = 
{
    .state    = SENSOR_OFF,
    .SOF      = 0x5A,
    .frameLen = 8,
    .recvNum  = 1,
    .swTime   = 110,     //ms
};

#ifdef PROJECT_QUBELEY
AnalogInfoStruct asensorInfoDefault =
{
    .state     = SENSOR_OFF,
    .responseH = RESPONSE_NONE,
    .responseL = RESPONSE_NONE,
    .thresholdH = 3.5,
    .thresholdL = 1.5,

    .crc = 0
};
#endif

#ifdef PROJECT_GELGOOG
IsolatorInStruct isolatorInDefault = 
{
    .state    = INTFC_OFF,
    .response = RESPONSE_NONE,
    .respChan = CH1,
    .sMode    = SMODE_CONTINUED,  
    .type     = TRIGINTYPE_LOW,
    .sPeriod  = 1000,

    .crc = 0
};
#endif

#if !GELGOOG_AXIS_10
DriverManageStruct driverManageDefault = 
{
    .state = SENSOR_ON,
    
    .type  = DRIVER_262,
    
    .curr  = 2,    //Ĭ��ֵ���͵�0.2A    CJ 2017.11.30 Modify
    
    .idleCurr = 2,

    .tuningState = SENSOR_ON,
    .currRatio   = CURRRATIO_QUARTER,
    
    .currIncre   = CURRINCRE_8,
    .currDecre   = CURRDECRE_32,
    
    .sgThreshold = 5,
    
    .energEfficMax    = 720,
    .energEfficOffset = 400,

    .switchTime = 500,

    .DRVCTRL = 
    {
        .stepDirMode =
        {
            .MRES          = MICROSTEP_64,
            
            .resv_bit4_7   = 0,
            
            .DEDGE         = 0,
            .INTPOL        = 0,
            .resv_bit10_17 = 0,
            
            .addr          = ADDR_TMC_DRVCTRL,
            
            .resv_bit20_31 = 0
        }
    },

    .CHOPCONF = 
    {
        .regBitFiled =
        {
            .TOFF  = 1,
            .HSTRT = 1,
            .HEND  = 0,
            .HDEC  = 3,
            .RNDTF = 0,
            
            .CHM   = 0,
            .TBL   = 3,
            
            .addr  = ADDR_TMC_CHOPCONF,
    
            .resv_bit20_31 = 0
        }
    },

    .SMARTEN = 
    {
        .regBitFiled =
        {
            .SEMIN      = 8,
            .resv_bit4  = 0,
            
            .SEUP       = 3,
            .resv_bit7  = 0,
            
            .SEMAX      = 12,
            .resv_bit12 = 0,
            
            .SEDN       = 0,
            .SEIMIN     = 1,
            .resv_bit16 = 0,
            
            .addr       = ADDR_TMC_SMARTEN,
            
            .resv_bit20_31 = 0
        }
    },

    .SGCSCONF = 
    {
        .regBitFiled =
        {
            .CS          = 10,
            .resv_bit5_7 = 0,
            
            .SGT        = 4,
            .resv_bit15 = 0,
            
            .SFILT = 1,
            
            .addr  = ADDR_TMC_SGCSCONF,
            
            .resv_bit20_31 = 0
        }
    },

    .DRVCONF = 
    {
        .regBitFiled =
        {
            .resv_bit0_3 = 0,
            
            .RDSEL  = RDSEL_SG_CS_LVL,
            .VSENSE = 1,
            .SDOFF  = 0,
            .TS2G   = 0,
            .DISS2G = 0,
            
            .resv_bit11 = 0,
            
            .SLPL   = 3,
            .SLPH   = 3,
            .TST    = 0,
            
            .addr   = ADDR_TMC_DRVCONF,
            
            .resv_bit20_31 = 0
        }
    }
};

#else

DriverInfoStruct driverInfoDefault = 
{
    .state = 
    {
        SENSOR_ON,
        SENSOR_ON,
        SENSOR_ON,
        SENSOR_ON,
        SENSOR_ON,
        SENSOR_ON,
        SENSOR_ON,
        SENSOR_ON,
        SENSOR_ON,
        SENSOR_ON,
    },
    
    .type = 
    {
        DRIVER_820,
        DRIVER_820,
        DRIVER_820,
        DRIVER_820,
        DRIVER_820,
        DRIVER_820,
        DRIVER_820,
        DRIVER_820,
        DRIVER_820,
        DRIVER_820
    },
    
    .microStep = MICROSTEP_32,
    .curr = 2,

    .crc = 0
};
#endif


#if GELGOOG_SINANJU
AbsEncoderAlarmStruct absEncAlarmDefault = 
{
    .state    = SENSOR_OFF,
    .zeroPost = SENSOR_OFF,

    .zeroValue = 0xFFFFFFFF,    //���ֵ����δУ׼
    .upLimit = 262143,
    .dnLimit = 0
};

DistSensorAlarmStruct distSensorAlarmDefault = 
{
    .state    = SENSOR_OFF,
    
    .alarm1Dist = 500,
    .alarm2Dist = 200,
    .alarm3Dist = 100,
};
#endif
/*Ĭ�����ò���****************************************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: WaveTableInfoInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void WaveTableInfoInit(void)
{
    u8 i, j;
    u32 wtMainAddr;
    u32 wtMainSize;
    u32 wtInchingAddr;
    u32 wtInchingSize;
    u32 wtPreset1Addr;
    u32 wtPreset1Size;
    u32 wtPreset5Addr;
    u32 wtPreset5Size;
    u32 wtChanOffset;
    

    //����ͨ���ż��㲨���С
    if (1 == CH_TOTAL)
    {
        wtMainAddr    = DDR_MAIN_WAVE_TABLE_ADDR1;
        wtMainSize    = DDR_MAIN_WAVE_TABLE_SIZE1;
        wtInchingAddr = DDR_INCHING_WAVE_TABLE_ADDR1;
        wtInchingSize = DDR_INCHING_WAVE_TABLE_SIZE1;
        wtPreset1Addr = DDR_PRESET1_WAVE_TABLE_ADDR1;
        wtPreset1Size = DDR_PRESET1_WAVE_TABLE_SIZE1;
        wtPreset5Addr = DDR_PRESET5_WAVE_TABLE_ADDR1;
        wtPreset5Size = DDR_PRESET5_WAVE_TABLE_SIZE1;
        wtChanOffset  = DDR_WAVE_TABLE_CHAN_OFFSET1;
    }
    else if (CH_TOTAL <= 4)
    {
        wtMainAddr    = DDR_MAIN_WAVE_TABLE_ADDR4;
        wtMainSize    = DDR_MAIN_WAVE_TABLE_SIZE4;
        wtInchingAddr = DDR_INCHING_WAVE_TABLE_ADDR4;
        wtInchingSize = DDR_INCHING_WAVE_TABLE_SIZE4;
        wtPreset1Addr = DDR_PRESET1_WAVE_TABLE_ADDR4;
        wtPreset1Size = DDR_PRESET1_WAVE_TABLE_SIZE4;
        wtPreset5Addr = DDR_PRESET5_WAVE_TABLE_ADDR4;
        wtPreset5Size = DDR_PRESET5_WAVE_TABLE_SIZE4;
        wtChanOffset  = DDR_WAVE_TABLE_CHAN_OFFSET4;
    }
    else if (CH_TOTAL <= 5)    //��е�����·���
    {
        wtMainAddr    = DDR_MAIN_WAVE_TABLE_ADDR5;
        wtMainSize    = DDR_MAIN_WAVE_TABLE_SIZE5;
        wtInchingAddr = DDR_INCHING_WAVE_TABLE_ADDR5;
        wtInchingSize = DDR_INCHING_WAVE_TABLE_SIZE5;
        wtPreset1Addr = DDR_PRESET1_WAVE_TABLE_ADDR5;
        wtPreset1Size = DDR_PRESET1_WAVE_TABLE_SIZE5;
        wtPreset5Addr = DDR_PRESET5_WAVE_TABLE_ADDR5;
        wtPreset5Size = DDR_PRESET5_WAVE_TABLE_SIZE5;
        wtChanOffset  = DDR_WAVE_TABLE_CHAN_OFFSET5;
    }
    else if (CH_TOTAL <= 8)
    {
        wtMainAddr    = DDR_MAIN_WAVE_TABLE_ADDR8;
        wtMainSize    = DDR_MAIN_WAVE_TABLE_SIZE8;
        wtInchingAddr = DDR_INCHING_WAVE_TABLE_ADDR8;
        wtInchingSize = DDR_INCHING_WAVE_TABLE_SIZE8;
        wtPreset1Addr = DDR_PRESET1_WAVE_TABLE_ADDR8;
        wtPreset1Size = DDR_PRESET1_WAVE_TABLE_SIZE8;
        wtPreset5Addr = DDR_PRESET5_WAVE_TABLE_ADDR8;
        wtPreset5Size = DDR_PRESET5_WAVE_TABLE_SIZE8;
        wtChanOffset  = DDR_WAVE_TABLE_CHAN_OFFSET8;
    }
    else if (10 == CH_TOTAL)
    {
        wtMainAddr    = DDR_MAIN_WAVE_TABLE_ADDR10;
        wtMainSize    = DDR_MAIN_WAVE_TABLE_SIZE10;
        wtInchingAddr = DDR_INCHING_WAVE_TABLE_ADDR10;
        wtInchingSize = DDR_INCHING_WAVE_TABLE_SIZE10;
        wtPreset1Addr = DDR_PRESET1_WAVE_TABLE_ADDR10;
        wtPreset1Size = DDR_PRESET1_WAVE_TABLE_SIZE10;
        wtPreset5Addr = DDR_PRESET5_WAVE_TABLE_ADDR10;
        wtPreset5Size = DDR_PRESET5_WAVE_TABLE_SIZE10;
        wtChanOffset  = DDR_WAVE_TABLE_CHAN_OFFSET10;
    }

    for (i = 0;i < CH_TOTAL;i++)
    {
        //��ʼ��״̬
        for (j = 0;j < WTTYPE_RESERVE;j++)
        { 
            g_waveTable[i][j].bEndIntEnable = true;
            g_waveTable[i][j].bCycleQuery   = false;
            
            g_waveTable[i][j].waveWorkMode = WTWORKMODE_CYCLE;
            
            g_waveTable[i][j].waveState = MTSTATE_IDLE;
            g_waveTable[i][j].lastState = MTSTATE_IDLE;
        }

        
        //��ʼ����ַ
        g_waveTable[i][WTTYPE_MAIN].startAddr    = wtMainAddr + wtChanOffset * i;
        g_waveTable[i][WTTYPE_MAIN].totalSize    = wtMainSize;
        g_waveTable[i][WTTYPE_MAIN].writableAddr = wtMainAddr + wtChanOffset * i;
        g_waveTable[i][WTTYPE_MAIN].writableSize = wtMainSize;
        
        g_waveTable[i][WTTYPE_SMALL].startAddr    = wtInchingAddr + wtChanOffset * i;
        g_waveTable[i][WTTYPE_SMALL].totalSize    = wtInchingSize;
        g_waveTable[i][WTTYPE_SMALL].writableAddr = wtInchingAddr + wtChanOffset * i;
        g_waveTable[i][WTTYPE_SMALL].writableSize = wtInchingSize;

        for (j = 0;j <= (WTTYPE_PRESET4 - WTTYPE_PRESET1);j++)
        {
            g_waveTable[i][WTTYPE_PRESET1 + j].startAddr    = wtPreset1Addr + wtPreset1Size * j + wtChanOffset * i;
            g_waveTable[i][WTTYPE_PRESET1 + j].totalSize    = wtPreset1Size;
            g_waveTable[i][WTTYPE_PRESET1 + j].writableAddr = wtPreset1Addr + wtPreset1Size * j + wtChanOffset * i;
            g_waveTable[i][WTTYPE_PRESET1 + j].writableSize = wtPreset1Size;
        }

        for (j = 0;j <= (WTTYPE_PRESET8 - WTTYPE_PRESET5);j++)
        {
            g_waveTable[i][WTTYPE_PRESET5 + j].startAddr    = wtPreset5Addr + wtPreset5Size * j + wtChanOffset * i;
            g_waveTable[i][WTTYPE_PRESET5 + j].totalSize    = wtPreset5Size;
            g_waveTable[i][WTTYPE_PRESET5 + j].writableAddr = wtPreset5Addr + wtPreset5Size * j + wtChanOffset * i;
            g_waveTable[i][WTTYPE_PRESET5 + j].writableSize = wtPreset5Size;
        }
    }
}


/*********************************************************************************************
�� �� ��: SystemParaReset;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void SystemParaReset(void)
{
    u8 i, j;
    

    //����ϵͳ��Ϣ����
    memcpy(&g_systemInfo, &systemInfoDefault, sizeof(SystemInfoStruct));
    for (i = 0;i < CH_TOTAL;i++)
    {
        for (j = 0;j < GROUP_NUM;j++)
        {
            g_systemInfo.group[i][j] = GROUP_NUM_VALUE_MIN;
        }
    }

#if 0
    //���ýӿ����ò���
    memcpy(&g_commIntfc, &commIntfcDefault, sizeof(CommIntfcStruct));
    servInterfaceInfoWrite(&g_commIntfc);  
#endif

    for (i = 0;i < CH_TOTAL;i++)
    {
        //���õ����Ϣ
        memcpy(&g_motorInfo.motor[i], &motorInfoDefault, sizeof(MotorManageStruct));

#ifdef PROJECT_GELGOOG 
#if GELGOOG_AXIS_8 || GELGOOG_AXIS_10

        g_motorInfo.motor[i].encoderState = INTFC_NONE;
        
#elif GELGOOG_SINANJU

        //5���5�����ϵĲ�֧�ֱ�����
        if (i >= ENCODER_SUPPORT_NUM)
        {
            g_motorInfo.motor[i].encoderState = INTFC_NONE;
        }
#endif
#endif

        //�������в���
        memcpy(&g_motionInfo.motion[i], &motionInfoDefault, sizeof(MotionManageStruct));
        
        //�����ϱ�������
        for (j = 0;j < REPTTYPE_RESERVE;j++)
        {
            g_reportInfo.report[i].state[j]  = SENSOR_OFF;
            g_reportInfo.report[i].period[j] = 1000;
        }

        //���ô����������
        memcpy(&g_trigInInfo.trigIn[i], &trigInInfoDefault, sizeof(TrigInManageStruct));

        //���ù滮����
        memcpy(&g_planInfo.plan[i][WTTYPE_MAIN], &planInfoDefault, sizeof(PlanManageStruct));
        memcpy(&g_planInfo.plan[i][WTTYPE_SMALL], &planInfoDefault, sizeof(PlanManageStruct));
        for (j = WTTYPE_PRESET1;j < WTTYPE_RESERVE;j++)
        {
            memcpy(&g_planInfo.plan[i][j], &planInfoDefault, sizeof(PlanManageStruct));
        }

#if !GELGOOG_AXIS_10
        //������Ϣ
        memcpy(&g_driverInfo.driver[i], &driverManageDefault, sizeof(DriverManageStruct));
#endif
    }

#if GELGOOG_AXIS_10
    memcpy(&g_driverInfo, &driverInfoDefault, sizeof(DriverInfoStruct));
#endif

    //���ô����������
    for (i = 0;i < DIO_RESERVE;i++)
    {
        memcpy(&g_digitalOut.output[i], &digitalIODefault, sizeof(DigitalOutManageStruct));
    }

    //���ø����������
    for (i = 0;i < YOUT_RESERVE;i++)
    {
        memcpy(&g_isolatorOut.output[i], &isolatorOutDefault, sizeof(IsolatorOutManageStruct));
    }

    //���ô�����UART
    for (i = 0;i < UARTNUM_RESERVE;i++)
    {
        memcpy(&g_sensorUart.uartIntfc[i], &commIntfcDefault.uartIntfc, sizeof(UartIntfcStruct));
    }
    
#if GELGOOG_SINANJU
    //��е��Ĭ�ϲ�����2.5M
    g_sensorUart.uartIntfc[UARTNUM_U2].baud = UARTBAUD_2500000;
#endif

    for (i = 0;i < SENSOR_RESERVE;i++)
    {
        memcpy(&g_sensorUart.sensor[UARTNUM_U1][i], &distSensorDefault, sizeof(SensorManageStruct));

#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4���10��ֻ֧��1·
        memcpy(&g_sensorUart.sensor[UARTNUM_U2][i], &angleSensorDefault, sizeof(SensorManageStruct));
#endif
    }

#ifdef PROJECT_QUBELEY
    if (SDMODEL_C23D == g_deviceInfo.sDevcModel)    //-D����AI��OTP
    {
        //ģ������
        memcpy(&g_analogInfo, &asensorInfoDefault, sizeof(AnalogInfoStruct));
        servAnalogInInfoWrite(&g_analogInfo);
    }
#endif

#ifdef PROJECT_GELGOOG

#if GELGOOG_SINANJU
    //�洢����λ��Ϣ��������
    //����������
    /*for (i = 0;i < SENSOR_RESERVE;i++)
    {
        memcpy(&g_sensorAlarm.encAlarm[i],  &absEncAlarmDefault,     sizeof(AbsEncoderAlarmStruct));
        memcpy(&g_sensorAlarm.distAlarm[i], &distSensorAlarmDefault, sizeof(DistSensorAlarmStruct));
    }
    g_sensorAlarm.encAlarmResponse = RESPONSE_ALARMSTOP;
    g_sensorAlarm.crc = 0;
    servSensorAlarmInfoStore(&g_sensorAlarm);*/

#else

    //���ø�������
    memcpy(&g_isolatorIn, &isolatorInDefault, sizeof(IsolatorInStruct));
    servIsolatorInInfoStore(&g_isolatorIn);
#endif

#endif
    
    g_systemInfo.crc = 0;
    servSystemInfoWrite(&g_systemInfo);
    
    g_motorInfo.crc = 0;  
    servMotorInfoWrite(&g_motorInfo);
    
    g_motionInfo.crc = 0;
    servMotionInfoWrite(&g_motionInfo);
    
    g_reportInfo.crc = 0;
    servReportInfoWrite(&g_reportInfo);
    
    g_trigInInfo.crc = 0;
    servTrigInInfoWrite(&g_trigInInfo);
    
    g_digitalOut.crc = 0;
    servTrigOutInfoWrite(&g_digitalOut);
    
    g_driverInfo.crc = 0;
    servDriverInfoStore(&g_driverInfo);

    g_isolatorOut.crc = 0;
    servIsolatorOutInfoStore(&g_isolatorOut);
    
    g_sensorUart.crc = 0;
    servSensorUartInfoStore(&g_sensorUart);

    g_planInfo.crc = 0;
    servPlanInfoStore(&g_planInfo);
}


/*********************************************************************************************
�� �� ��: SystemParaInit;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void SystemParaInit(void)
{ 
    u16 i, j;
#if 0
    u32 flashCrc;
    u32 eepromCrc;
    SnEepromStruct snEeprom;
    SnEepromStruct snFlash;
#endif

    //�ȶ�ȡEEprom�е�BOOT��Ϣ
    if (servBootInfoRead(&g_bootInfo) != VERIFY_SUCCESSFUL)//��ȡEEprom�б����boot��Ϣ����
    {
        //�������,��Ҫ����ϵͳ
        bspSoftReset();
    }
    else
    {
        if (g_bootInfo.u8AppFlag != APP_IMAGE_OK)
        {
            g_bootInfo.u8AppFlag  = APP_IMAGE_OK;
            g_bootInfo.u8TryCount = 0;
            servBootInfoWrite(&g_bootInfo);
        }
    }

    //ϵͳ״̬������ʼ��
    memset(&g_systemState, 0, sizeof(g_systemState));
    for (i = 0;i < CH_TOTAL;i++)
    {
        for (j = 0;j < PVT_CALC_QUEUE_SIZE;j++)
        {
            g_systemState.calcQueue[i][j] = WTTYPE_RESERVE;
        }

#if GELGOOG_SINANJU
        g_systemState.pdmInfo[i].sampleState = SENSOR_OFF;
        g_systemState.pdmInfo[i].chanNum     = CH1;
        g_systemState.pdmInfo[i].encDiv      = SYSTEM_PDM_ENC_DIV_DEFAULT;
        
        g_systemState.pdmInfo[i].readOffset = 0;
        g_systemState.pdmInfo[i].readLen    = 0;
        g_systemState.pdmInfo[i].mstepCount = 0;
        
        g_systemState.pdmInfo[i].mstepData   = (void *)&pdmMstepDataBuffer[i];
#endif
    }
#if GELGOOG_SINANJU
    g_systemState.ledFlickerFreq = LED_FLICKER_FREQ_MIN;
#endif

    //��ȡ�洢�Ĳ���ϵͳ��Ϣ����
    if (servSystemInfoRead(&g_systemInfo) != VERIFY_SUCCESSFUL)
    {
        memcpy(&g_systemInfo, &systemInfoDefault, sizeof(SystemInfoStruct));
        for (i = 0;i < CH_TOTAL;i++)
        {
            for (j = 0;j < GROUP_NUM;j++)
            {
                g_systemInfo.group[i][j] = GROUP_NUM_VALUE_MIN;
            }
        }

        g_systemInfo.crc = 0;
        servSystemInfoWrite(&g_systemInfo);
    }
    //��ΪrevMotion�Ƿ����˽ṹ���м䣬֮ǰ����group��Ĭ��ֵ�ģ�����������Ҫ���¸�ֵ    CJ 2018.08.09 Add
    if ((u8)g_systemInfo.revMotion > (u8)SENSOR_ON)
    {
        memcpy(&g_systemInfo, &systemInfoDefault, sizeof(SystemInfoStruct));
        for (i = 0;i < CH_TOTAL;i++)
        {
            for (j = 0;j < GROUP_NUM;j++)
            {
                g_systemInfo.group[i][j] = GROUP_NUM_VALUE_MIN;
            }
        }

        g_systemInfo.crc = 0;
        servSystemInfoWrite(&g_systemInfo);
    }
#if 0    //For Modify NICK
    //��ȡSN���ȴ�EEprom��Flash�ж���������Ȼ��ֱ����CRC    //����SN������м��ܴ洢    NICK MARK
    drvAT24CxxRead(EEPROM_SN_ADDR, (u8*)&snEeprom, sizeof(SnEepromStruct));
    bspFlashRead(FLASH_SN_ADDR, (u8 *)&snFlash, sizeof(SnEepromStruct));
    eepromCrc = servCRC32((u8*)&snEeprom, sizeof(SnEepromStruct) - sizeof(snEeprom.crc));
    flashCrc  = servCRC32((u8*)&snFlash,  sizeof(SnEepromStruct) - sizeof(snFlash.crc));
    if ((eepromCrc == snEeprom.crc) && (flashCrc == snFlash.crc))
    {
        //��FlashΪ׼��FLASH��EEPROM��һ��������дEEPROM
        memcpy(g_systemInfo.deviceSn, snFlash.deviceSn, DEVICE_SN_LEN);
        if (strcmp((char *)snFlash.deviceSn, (char *)snEeprom.deviceSn))
        {
            //����дEEPROM
            drvAT24CxxWrite(EEPROM_SN_ADDR, (u8*)&snFlash, sizeof(SnEepromStruct));
        }
    }
    else if ((eepromCrc != snEeprom.crc) && (flashCrc == snFlash.crc))
    {
        memcpy(g_systemInfo.deviceSn, snFlash.deviceSn, DEVICE_SN_LEN);

        //����дEEPROM
        drvAT24CxxWrite(EEPROM_SN_ADDR, (u8*)&snFlash, sizeof(SnEepromStruct));
    }
    else if ((eepromCrc == snEeprom.crc) && (flashCrc != snFlash.crc))
    {
        memcpy(g_systemInfo.deviceSn, snEeprom.deviceSn, DEVICE_SN_LEN);

        //����дFlash
        bspFlashWrite(FLASH_SN_ADDR, (u8 *)&snEeprom, sizeof(SnEepromStruct));
    }
    else
    {
        memcpy(snEeprom.deviceSn, systemInfoDefault.deviceSn, DEVICE_SN_LEN);
        memcpy(g_systemInfo.deviceSn, snEeprom.deviceSn, DEVICE_SN_LEN);

        //Ĭ��ֵд��Flash��EEPROM
        servSnWrite(&snEeprom);
        bspFlashWrite(FLASH_SN_ADDR, (u8 *)&snEeprom, sizeof(SnEepromStruct));
    }
#endif
    
    //��ȡ�豸��Ϣ��Ϣ
    servDeviceInfoInit(&g_deviceInfo, g_bootInfo.au8BootVer);

    servSystemInfoInit(g_deviceInfo.sDevcModel);

    //��ȡ�ӿ����ò���
    if (servInterfaceInfoRead(&g_commIntfc) != VERIFY_SUCCESSFUL)
    {
        //����У�����,��ָ�Ĭ��
        memcpy(&g_commIntfc, &commIntfcDefault, sizeof(CommIntfcStruct));
        servInterfaceInfoWrite(&g_commIntfc);
    }
    memcpy(&g_ciCanInterface,  &g_commIntfc.canIntfc,  sizeof(CanIntfcStruct));
    memcpy(&g_ciUartInterface, &g_commIntfc.uartIntfc, sizeof(UartIntfcStruct));

    //���������Ϣ
    if (servMotorInfoRead(&g_motorInfo) != VERIFY_SUCCESSFUL)
    {   
        for (i = 0;i < CH_TOTAL;i++)
        {
            memcpy(&g_motorInfo.motor[i], &motorInfoDefault, sizeof(MotorManageStruct)); 

#ifdef PROJECT_GELGOOG 

#if GELGOOG_AXIS_8 || GELGOOG_AXIS_10    //8�ᣬ10��Ŀǰ��֧�ֱ�����

            g_motorInfo.motor[i].encoderState = INTFC_NONE;
            
#elif GELGOOG_SINANJU

            //��е��Ŀǰֻ֧��4·������
            if (i >= ENCODER_SUPPORT_NUM)
            {
                g_motorInfo.motor[i].encoderState = INTFC_NONE; 
            }
#endif

#endif
        }
        
        g_motorInfo.crc = 0;
        servMotorInfoWrite(&g_motorInfo);
    }

    //�������в���
    if (servMotionInfoRead(&g_motionInfo) != VERIFY_SUCCESSFUL)
    { 
        for (i = 0;i < CH_TOTAL;i++)
        {
            memcpy(&g_motionInfo.motion[i], &motionInfoDefault, sizeof(MotionManageStruct));
        }
        g_motionInfo.crc = 0;
        servMotionInfoWrite(&g_motionInfo);
    }
    for (i = 0;i < CH_TOTAL;i++)
    {
        g_systemState.softTimer.motionState[i] = SENSOR_ON;
    }
    
    //�����ϱ�������
    if (servReportInfoRead(&g_reportInfo) != VERIFY_SUCCESSFUL)
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            for (j = 0;j < REPTTYPE_RESERVE;j++)
            {
                g_reportInfo.report[i].state[j]  = SENSOR_OFF;
                g_reportInfo.report[i].period[j] = 1000;
            }
        }
        g_reportInfo.crc = 0;
        servReportInfoWrite(&g_reportInfo);
    }

    //�����������
    if (servTrigInInfoRead(&g_trigInInfo) != VERIFY_SUCCESSFUL)
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            memcpy(&g_trigInInfo.trigIn[i], &trigInInfoDefault, sizeof(TrigInInfoStruct));
        }
        g_trigInInfo.crc = 0;
        servTrigInInfoWrite(&g_trigInInfo);
    }
    
    //�����������
    if (servTrigOutInfoRead(&g_digitalOut) != VERIFY_SUCCESSFUL)
    {
        for (i = 0;i < DIO_RESERVE;i++)
        {
            memcpy(&g_digitalOut.output[i], &digitalIODefault, sizeof(DigitalOutManageStruct));
        }
        g_digitalOut.crc = 0;
        servTrigOutInfoWrite(&g_digitalOut);
    }

    //�����������
    if (servIsolatorOutInfoRead(&g_isolatorOut) != VERIFY_SUCCESSFUL)
    {
        for (i = 0;i < YOUT_RESERVE;i++)
        {
            memcpy(&g_isolatorOut.output[i], &isolatorOutDefault, sizeof(IsolatorOutManageStruct));
        }
        g_isolatorOut.crc = 0;
        servIsolatorOutInfoStore(&g_isolatorOut);
    }

    //������UART
    if (servSensorUartInfoRead(&g_sensorUart) != VERIFY_SUCCESSFUL)
    {
        for (i = 0;i < UARTNUM_RESERVE;i++)
        {
            memcpy(&g_sensorUart.uartIntfc[i], &commIntfcDefault.uartIntfc, sizeof(UartIntfcStruct));
        }

#if GELGOOG_SINANJU
        //��е��Ĭ�ϲ�����2.5M
        g_sensorUart.uartIntfc[UARTNUM_U2].baud = UARTBAUD_2500000;
#endif

        for (i = 0;i < SENSOR_RESERVE;i++)
        {
            memcpy(&g_sensorUart.sensor[UARTNUM_U1][i], &distSensorDefault,  sizeof(SensorManageStruct));

#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4���10��ֻ֧��1·
            memcpy(&g_sensorUart.sensor[UARTNUM_U2][i], &angleSensorDefault, sizeof(SensorManageStruct));
#endif
        }

        g_sensorUart.crc = 0;
        servSensorUartInfoStore(&g_sensorUart);
    }
    memcpy(&g_sensorUartIntfc[UARTNUM_U1], &g_sensorUart.uartIntfc[UARTNUM_U1], sizeof(UartIntfcStruct));
#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4���10��ֻ֧��1·
    memcpy(&g_sensorUartIntfc[UARTNUM_U2], &g_sensorUart.uartIntfc[UARTNUM_U2], sizeof(UartIntfcStruct));
#endif


#ifdef PROJECT_QUBELEY
    if (SDMODEL_C23D == g_deviceInfo.sDevcModel)    //-D����AI��OTP
    {
        //ģ������
        if (servAnalogInInfoRead(&g_analogInfo) != VERIFY_SUCCESSFUL)
        {
            memcpy(&g_analogInfo, &asensorInfoDefault, sizeof(AnalogInfoStruct));
            servAnalogInInfoWrite(&g_analogInfo);
        }
    }
#endif


#ifdef PROJECT_GELGOOG

#if GELGOOG_SINANJU
    //����������
    if (servSensorAlarmInfoRead(&g_sensorAlarm) != VERIFY_SUCCESSFUL)
    {
        for (i = 0;i < SENSOR_RESERVE;i++)
        {
            memcpy(&g_sensorAlarm.encAlarm[i],  &absEncAlarmDefault,     sizeof(AbsEncoderAlarmStruct));
            memcpy(&g_sensorAlarm.distAlarm[i], &distSensorAlarmDefault, sizeof(DistSensorAlarmStruct));
        }
        g_sensorAlarm.encAlarmResponse = RESPONSE_ALARMSTOP;
        g_sensorAlarm.crc = 0;
        servSensorAlarmInfoStore(&g_sensorAlarm);
    }

#else

    //��������
    if (servIsolatorInInfoRead(&g_isolatorIn) != VERIFY_SUCCESSFUL)
    {
        memcpy(&g_isolatorIn, &isolatorInDefault, sizeof(IsolatorInStruct));
        servIsolatorInInfoStore(&g_isolatorIn);
    }
#endif

#endif

    //PVT���ݽṹ���ʼ��
    if (servPlanInfoRead(&g_planInfo) != VERIFY_SUCCESSFUL)
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            memcpy(&g_planInfo.plan[i][WTTYPE_MAIN], &planInfoDefault, sizeof(PlanManageStruct));
            memcpy(&g_planInfo.plan[i][WTTYPE_SMALL], &planInfoDefault, sizeof(PlanManageStruct));

            for (j = WTTYPE_PRESET1;j < WTTYPE_RESERVE;j++)
            {
                memcpy(&g_planInfo.plan[i][j], &planInfoDefault, sizeof(PlanManageStruct));
            }
        }

        g_planInfo.crc = 0;
        servPlanInfoStore(&g_planInfo);
    }
    
    for (i = 0;i < CH_TOTAL;i++)
    {
        memset(&g_systemState.pvtManage[i][WTTYPE_MAIN], 0, sizeof(PvtManageStruct));
        g_systemState.pvtManage[i][WTTYPE_MAIN].pvtBufferSize = PVT_POINT_BUFFER_SIZE;
        g_systemState.pvtManage[i][WTTYPE_MAIN].pvtPoint = pvtPointBuffer[i].mainPvtPoint;
        
        
        memset(&g_systemState.pvtManage[i][WTTYPE_SMALL], 0, sizeof(PvtManageStruct));
        g_systemState.pvtManage[i][WTTYPE_SMALL].pvtBufferSize = PVT_POINT_BUFFER_SIZE;
        g_systemState.pvtManage[i][WTTYPE_SMALL].pvtPoint = pvtPointBuffer[i].smallPvtPoint;

        for (j = WTTYPE_PRESET1;j < WTTYPE_RESERVE;j++)
        {
            memset(&g_systemState.pvtManage[i][j], 0, sizeof(PvtManageStruct));
            g_systemState.pvtManage[i][j].pvtBufferSize = PVT_POINT_BUFFER_SIZE;
            g_systemState.pvtManage[i][j].pvtPoint = pvtPointBuffer[i].presetPvtPoint[j - WTTYPE_PRESET1];
        }
    }

    //������Ϣ�ṹ���ʼ��
    if (servDriverInfoRead(&g_driverInfo) != VERIFY_SUCCESSFUL)
    {
#if !GELGOOG_AXIS_10
        for (i = 0;i < CH_TOTAL;i++)
        {
            //������Ϣ
            memcpy(&g_driverInfo.driver[i], &driverManageDefault, sizeof(DriverManageStruct));
        }
#else
        memcpy(&g_driverInfo, &driverInfoDefault, sizeof(DriverInfoStruct));
#endif
        
        g_driverInfo.crc = 0;
        servDriverInfoStore(&g_driverInfo);
    }
#if !GELGOOG_AXIS_10
    //��ΪtuningState֮ǰ��λ������sgUpLimit��Ĭ��ֵ�ģ�����������Ҫ���¸�ֵ������ֻ���ж�һ��ͨ������    CJ 2018.09.01 Add
    if ((u8)g_driverInfo.driver[0].tuningState > (u8)SENSOR_ON)
    {
        for (i = 0;i < CH_TOTAL;i++)
        {
            //������Ϣ
            memcpy(&g_driverInfo.driver[i], &driverManageDefault, sizeof(DriverManageStruct));
        }
        
        g_driverInfo.crc = 0;
        servDriverInfoStore(&g_driverInfo);
    }
#endif
    
    //������λ�õ�΢���ͱ������ߵ�ת��ϵ��
    for (i = 0;i < CH_TOTAL;i++)
    {
#if !GELGOOG_AXIS_10                            
        //���¼�����λ�õ�΢���ͱ������ߵ�ת��ϵ��
        servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                              g_driverInfo.driver[i].DRVCTRL.stepDirMode.MRES,
                              &g_systemState.posnConvertInfo[i]);
#else
        //���¼�����λ�õ�΢���ͱ������ߵ�ת��ϵ��
        servPosnConvCoeffCalc(g_motorInfo.motor[i], 
                              g_driverInfo.microStep,
                              &g_systemState.posnConvertInfo[i]);
#endif
    }

    //�������ϱ����ݳ�ʼ��
    memset(&g_sensorData, 0, sizeof(g_sensorData));

    //λͼ��ʼ��
    memset(&g_chanCfgBmp,   0, sizeof(g_chanCfgBmp));
    memset(&g_systemCfgBmp, 0, sizeof(g_systemCfgBmp));
    memset(&g_eventSrcBmp,  0, sizeof(g_eventSrcBmp));

    //��ʼ���������в����״̬��ʼ��Ϊδ����״̬
    WaveTableInfoInit();

    if (0 == g_systemState.pvtCalcTaskSem)    //û��PRESET��Ҫ�����ֱ�ӽ���IDLE״̬
    {
#if MRV_SUPPORT 
        for (i = 0;i < CH_TOTAL + MRV_CH_TOTAL_NUM;i++)
#else
        for (i = 0;i < CH_TOTAL;i++)
#endif
        {
            //g_systemState.bStateSwitch[i][WTTYPE_MAIN] = true;

            //����״̬����¼�
            g_eventSrcBmp.bStateMonitor[i] = true;
        }

        //״̬�ı䣬֪ͨevent�߳�
        OSSemPost(&g_semEventManageTask, OS_OPT_POST_ALL, NULL);
    }
#if 0    //��ʱ��֧���ϵ����PVT
    else
    {
        //��PVTCALC�̷߳��ź���
        for (i = 0;i < g_systemState.pvtCalcTaskSem;i++)
        {
            OSSemPost(&g_semPvtCalcTask, OS_OPT_POST_ALL, NULL);
        }
    }
#endif
}


/*********************************************************************************************
�� �� ��: SystemSemCreate;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
static void SystemSemCreate(void)
{
    OS_ERR err;
    
    
    OSSemCreate((OS_SEM   *)&g_semCmdParseTask,
                (CPU_CHAR *)"Cmd Parse Task Sem",
                (OS_SEM_CTR)0,
                (OS_ERR   *)&err);

    OSSemCreate((OS_SEM   *)&g_semPvtCalcTask,
                (CPU_CHAR *)"PVT Calc Task Sem",
                (OS_SEM_CTR)0,
                (OS_ERR   *)&err);
    
    OSSemCreate((OS_SEM   *)&g_semFunctionTask,
                (CPU_CHAR *)"Function Task Sem",
                (OS_SEM_CTR)0,
                (OS_ERR   *)&err);
    
    
    OSSemCreate((OS_SEM   *)&g_semPeriodicTask,
                (CPU_CHAR *)"Periodic Task Sem",
                (OS_SEM_CTR)0,
                (OS_ERR   *)&err);
    
    
    OSSemCreate((OS_SEM   *)&g_semEventManageTask,
                (CPU_CHAR *)"Event Manage Task Sem",
                (OS_SEM_CTR)0,
                (OS_ERR   *)&err);
    
    
    OSSemCreate((OS_SEM   *)&g_semParaSaveTask,
                (CPU_CHAR *)"Para Save Task Sem",
                (OS_SEM_CTR)0,
                (OS_ERR   *)&err);
}


/*********************************************************************************************
�� �� ��: SystemTaskCreate;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
static void SystemTaskCreate(void)
{
    OS_ERR err;
    
                 
    //�����������
    OSTaskCreate((OS_TCB    *)&CmdParseTaskTCB,
                 (CPU_CHAR  *)"Cmd Parse Task",
                 (OS_TASK_PTR)CmdParseTask,
                 (void      *)0,
                 (OS_PRIO    )APP_TASK_PRIO_CMD_PARSE,
                 (CPU_STK   *)&CmdParseTaskStk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_CMD_PARSE / 10,    //(CPU_STK_SIZE)CmdParseTaskObjStk[APP_TASK_STK_SIZE_CMD_PARSE / 10],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_CMD_PARSE,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
                 
    //PVT��������
    OSTaskCreate((OS_TCB    *)&PvtProcTaskTCB,
                 (CPU_CHAR  *)"Pvt Proc Task",
                 (OS_TASK_PTR)PvtProcTask,
                 (void      *)0,
                 (OS_PRIO    )APP_TASK_PRIO_PVT_CALC,
                 (CPU_STK   *)&PvtProcTaskStk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_PVT_CALC / 10,    //(CPU_STK_SIZE)PvtCalcTaskObjStk[APP_TASK_STK_SIZE_PVT_CALC / 10],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_PVT_CALC,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
    
    
    //Function����
    OSTaskCreate((OS_TCB    *)&FunctionTaskTCB,
                 (CPU_CHAR  *)"Function Task",
                 (OS_TASK_PTR)FunctionTask,
                 (void      *)0,
                 (OS_PRIO    )APP_TASK_PRIO_FUNCTION,
                 (CPU_STK   *)&FunctionTaskStk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_FUNCTION / 10,    //(CPU_STK_SIZE)FunctionTaskStk[APP_TASK_STK_SIZE_FUNCTION / 10],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_FUNCTION,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
    
    
    //����������
    OSTaskCreate((OS_TCB    *)&PeriodicTaskTCB,
                 (CPU_CHAR  *)"Periodic Task",
                 (OS_TASK_PTR)PeriodicTask,
                 (void      *)0,
                 (OS_PRIO    )APP_TASK_PRIO_PERIODIC,
                 (CPU_STK   *)&PeriodicTaskStk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_PERIODIC / 10,    //(CPU_STK_SIZE)PeriodTaskStk[APP_TASK_STK_SIZE_PERIODIC / 10],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_PERIODIC,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
    
    
    //�¼���������
    OSTaskCreate((OS_TCB    *)&EventManageTaskTCB,
                 (CPU_CHAR  *)"Event Manage Task",
                 (OS_TASK_PTR)EventManageTask,
                 (void      *)0,
                 (OS_PRIO    )APP_TASK_PRIO_EVENT_MANAGE,
                 (CPU_STK   *)&EventManageTaskStk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_EVENT_MANAGE / 10,    //(CPU_STK_SIZE)EventManageTaskStk[APP_TASK_STK_SIZE_EVENT_MANAGE / 10],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_EVENT_MANAGE,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
    
    
    //�����洢����
    OSTaskCreate((OS_TCB    *)&ParaSaveTaskTCB,
                 (CPU_CHAR  *)"Para Save Task",
                 (OS_TASK_PTR)ParaSaveTask,
                 (void      *)0,
                 (OS_PRIO    )APP_TASK_PRIO_PARA_SAVE,
                 (CPU_STK   *)&ParaSaveTaskStk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_PARA_SAVE / 10,    //(CPU_STK_SIZE)ParaSaveTaskStk[APP_TASK_STK_SIZE_PARA_SAVE / 10],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_PARA_SAVE,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);

    //SDIO�ӿڻ���
    OSMutexCreate((OS_MUTEX  *)&g_mutexSdioInterface,
                  (CPU_CHAR  *)"Sdio Interface Mutex",
                  (OS_ERR    *)&err);
}


#if SDIO_TX_RX_TEST
/*********************************************************************************************
�� �� ��: SystemSdioTxRxTest;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void SystemSdioTxRxTest(void)
{
    bool bRemainder = false;
    bool bRxError = false;
    u8  cmdData[8];
    u8 *pTestCount;
    u8  readCount;
    u8  respData[1024];
    u16 i;
    u16 j;
    u16 readLen = 510;
    u16 dataBlock;
    u16 txErrorCount;
    u16 rxErrorCount;
    u16 *pTxData;
    u16 *pRxData;
    u32 dataLen;
    u32 respDataLen;
    u64 testCount;

    
    //���Կ�ʼ����CAN���߷��Ͳ��Կ�ʼָ��
    for (i = 0;i < 8;i++)
    {
        cmdData[i] = 1;
    }
    servFrameSend(cmdData[0], cmdData[1], 6, &cmdData[2], g_commIntfc.linkType);
    servTxFrameProcess();

    //����һ������
    g_outputData[CH1].writeAddr = SERV_FPGA_CONFIG_REG_26;
    for (i = 0;i < OUTPUT_DATA_BUFFER_SIZE;i++)
    {
        g_outputData[CH1].data[i] = 0x20001 + 0x20002 * i;
    }

    //ѭ����FPGA CACHE��д�����ɵ����ݣ�Ȼ���ȡд����״̬
    //�ٴ�FPGA CACHE�ж���д������ݺ�д��֮ǰ�����ݽ��жԱ�
    testCount    = 0;
    txErrorCount = 0;
    rxErrorCount = 0;
    dataLen   = 8;    //��һ�η���4���ֽ�
    dataBlock = SDIO_DataBlockSize_8b;
    do
    {
        //��λ����ʼ��������������
        servFpgaPdmCacheTestSet();
        servFpgaPdmSampleReset(SAMPLECH_CH1);
        servFpgaPdmSampleStart(SAMPLECH_CH1);
        servFpgaDDRDataTypeSet(CH1, DATATYPE_CACHE);

        //д����
        servFpgaDataLenSet(FPGAACT_WRITE, dataLen, dataLen);
        bspSdioDataSend((u8 *)&g_outputData[CH1].writeAddr, dataLen, dataBlock);

        //����
        servFpgaPdmSampleEnd(SAMPLECH_CH1);

        //��ȡд��ĳ���(�������ĳ�����16bit�ģ�����Ҫ��2)
        respDataLen = servFpgaPdmMstepCountRead(SAMPLECH_CH1) * 2;
        
        //��ȡд����״̬
        if ((!servFpgaSdioWrErrorRead()) && (respDataLen == (dataLen - sizeof(u32))))
        {
            bRemainder = false;
            bRxError = false;

            readCount = respDataLen / readLen;
            if ((respDataLen % readLen) != 0)
            {
                bRemainder = true;
            }

            pTxData = (u16 *)g_outputData[CH1].data;
            for (i = 0;i < readCount;i++)
            {
                pRxData = (u16 *)respData + 1;    //+1�ǼĴ�����ַƫ��
                servFpgaPdmMstepDataRead(SAMPLECH_CH1, respData, readLen);

                for (j = 0;j < readLen / sizeof(u16);j++)
                {
                    if (*pTxData != *pRxData)
                    {
                        rxErrorCount++;
                        bRxError = true;
                        break;
                    }
                    pTxData++;
                    pRxData++;
                }

                if (bRxError)
                {
                    break;
                }

                respDataLen -= readLen;
            }

            //����ʣ������
            if (bRemainder && !bRxError)
            {
                pRxData = (u16 *)respData + 1;    //+1�ǼĴ�����ַƫ��
                servFpgaPdmMstepDataRead(SAMPLECH_CH1, respData, respDataLen);

                for (j = 0;j < respDataLen / sizeof(u16);j++)
                {
                    if (*pTxData != *pRxData)
                    {
                        rxErrorCount++;
                        bRxError = true;
                        break;
                    }
                    pTxData++;
                    pRxData++;
                }
            }
        }
        else
        {
            //��¼���������д���˾�û��Ҫ����
            txErrorCount++;
        }

        dataLen   *= 2;
        dataBlock += SDIO_DataBlockSize_2b;
        if (dataLen > 4096)
        {
            dataLen   = 8;
            dataBlock = SDIO_DataBlockSize_8b;
        }

        testCount++;
    }
    while ((txErrorCount < 1000) && ((rxErrorCount < 1000)));
    
    //���Խ�������CAN���߷��Ͳ��Խ���ָ��
    pTestCount = (u8 *)&testCount;
    for (i = 0;i < 8;i++)
    {
        cmdData[i] = *pTestCount++;
    }
    servFrameSend(cmdData[0], cmdData[1], 6, &cmdData[2], g_commIntfc.linkType);

    for (i = 0;i < 6;i++)
    {
        cmdData[i] = 0;
    }
    cmdData[6] = (u8)(txErrorCount >> 8);
    cmdData[7] = (u8)txErrorCount;
    servFrameSend(cmdData[0], cmdData[1], 6, &cmdData[2], g_commIntfc.linkType);

    for (i = 0;i < 6;i++)
    {
        cmdData[i] = 0;
    }
    cmdData[6] = (u8)(rxErrorCount >> 8);
    cmdData[7] = (u8)rxErrorCount;
    servFrameSend(cmdData[0], cmdData[1], 6, &cmdData[2], g_commIntfc.linkType);
    
    servTxFrameProcess();
}
#endif


/*********************************************************************************************
�� �� ��: SystemInitTask;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
static void SystemInitTask(void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
    u16         i;
    

    //(void)p_arg;

    //BSP_Init();
    //Mem_Init();
    //Math_Init(); 

    cpu_clk_freq = *(CPU_INT32U *)p_arg;
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;
    OS_CPU_SysTickInit(cnts);

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    //�����ź���
    SystemSemCreate();
    
    //�����߳�
    SystemTaskCreate();


    
    /*------------------------------ϵͳ��ʼ��������Ӳ�����裬FPGA�Լ����-----------------------------------*/
    /************************Ӳ����ʼ����ʼ***********************************/
    //��ʼ��GPIO
    bspLedGpioInit(); 
    bspFpgaITGpioInit();
    bspDriverGpioInit();

    bspSdioInit();

    bspFpSpiInit();

    //��ʼ������
    bspEepromInit();
    bspCRC32Init();

    bspCiCanInit();

    bspCiUartInit();
    bspSensor1UartInit();
    bspSensor2UartInit();

#ifdef PROJECT_QUBELEY
    bspAdc1Init();    //����ģ������
#endif


#ifdef PROJECT_GELGOOG
    
#if GELGOOG_SINANJU
    bspFanTimerInit();       //���Ʒ���
    bspArmLedTimerInit();    //����LED

#elif GELGOOG_AXIS_10
    bspDriverTimerInit();    //��������оƬVref���Ӷ����Ƶ���
#endif
    
#endif

    bspHardVerAdcInit();
    /************************Ӳ����ʼ������***********************************/
    

    //Gelgoog��Ҫ�Ȼ�ȡ���ְ��ͺź�Ӳ���汾�Ų��ܾ���FPGA�ļ��ļ��ؾ���
    //servFpgaTypeRead();��FPGA���ͣ��������ͼ��ؾ���

    /************************��ʼ����FPGA***********************************/
#if !FOR_FPGA_DEBUG

    bspFpgaProgGpioInit();
   
#if 0    //������FPGA����д��Flash
    servFpgaLoaclFileWrite(FLASH_FPGA_CODE_ADDR, FPGA_DATA_PATH);    
#endif

    //��Flash���߱����ļ���ȡFPGA���벢����
#if 1
    servFpgaFlashFileLoad(FLASH_FPGA_CODE_ADDR);    //��Flash�ж�ȡFPGA����
#else
    servFpgaLoaclFileLoad(FPGA_DATA_PATH);    //�ӱ����ļ��ж�ȡFPGA����
#endif


#else //#if !FOR_FPGA_DEBUG    


    //�ȴ�FPGA������ɣ�������: д01�Ĵ�����Ȼ�����ֱ������������д����һ��
    bool fpgaReady = false;
    do
    {
        fpgaReady = servFpgaReadyStateQuery();
        
    }while (false == fpgaReady);
        
#endif    //#if !FOR_FPGA_DEBUG
    /************************��ɼ���FPGA***********************************/


    //״̬����Ϊ�ϵ�״̬
    servLedInit(LED_POWERON); 

    //��ʼ��ϵͳ�������Ӵ洢�л�ȡ����������õ����ò���
    SystemParaInit();
    
    //��ʼ��Э��ջ����ʱ��
    servCommStreamBufferInit();
    servStimerAllInit();

    //���ö���ӿ�:CAN��232
    servCiUartConfig(g_commIntfc.uartIntfc);
    servCiCanConfig(g_commIntfc.canIntfc);

    //���ô���������
    servSensor1UartConfig(g_sensorUart.uartIntfc[UARTNUM_U1]);
#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4���10��ֻ֧��1·
    servSensor2UartConfig(g_sensorUart.uartIntfc[UARTNUM_U2]);
#endif

    //��������(10��ֱ�����ã�������Ʒ������ʱ�Ӻ�����ʹ��)
#if GELGOOG_AXIS_10
    servDriverConfig(g_driverInfo);
    
#else

    for (i = 0;i < CH_TOTAL;i++)
    {
        servFpgaDriverClkCofing(i, DRVCLK_OPEN);    //��ʼ���߼���TMC���� 
    }
    
    //ʹ������
    servDriverCtrlEnable(true);

#endif

    //����ͨ����صĲ���
    for (i = 0;i < CH_TOTAL;i++)
    {
#if !GELGOOG_AXIS_10
        //��������
        servDriverConfig(i, g_driverInfo.driver[i]);

        //���ñ�����(10��û�б�����)
        servFpgaEncoderSet(i, g_motorInfo.motor[i].encoderState,
                              g_motorInfo.motor[i].encoderMult,
                              g_motorInfo.motor[i].encoderChanNum);
#endif

        //��������Դ
        servFpgaStartSourceSet(i, g_motionInfo.motion[i].startSrc);

        //���ô�������
        servFpgaTriggerSet(i, g_trigInInfo.trigIn[i], g_motorInfo.motor[i].encoderChanNum);
    }

    //�����ϱ���
    //TO ADD NICK

    //�����������
    for (i = 0;i < g_systemState.doutNum;i++)
    {
        servFpgaDigitalOutSet((DoutNumEnum)i, g_digitalOut.output[i]);
    }

    //���ø������
    for (i = 0;i < g_systemState.youtNum;i++)
    {
        servFpgaIsolatorOutSet((YoutNumEnum)i, g_isolatorOut.output[i]);
    }

    //����UART������
    for (i = 0;i < SENSOR_RESERVE;i++)
    {
        if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U1][i].state)
        {
            servSensor1UartReciveOn(g_sensorUart.sensor[UARTNUM_U1][i], (SensorNumEnum)i);
        }

#if !(GELGOOG_AXIS_4 || GELGOOG_AXIS_10)    //4���10��ֻ֧��1·        
        if (SENSOR_ON == g_sensorUart.sensor[UARTNUM_U2][i].state)
        {
            servSensor2UartReciveOn(g_sensorUart.sensor[UARTNUM_U2][i], (SensorNumEnum)i);
        }
#endif
    }

#ifdef PROJECT_QUBELEY
    if (SDMODEL_C23D == g_deviceInfo.sDevcModel)    //-D����AI��OTP
    {
        //����ģ������
        servAnalogInSet(g_analogInfo);
    }
#endif

#ifdef PROJECT_GELGOOG

#if GELGOOG_SINANJU
    //���÷���ת��
    servFanConfig(g_systemInfo.fanFrequency, g_systemInfo.fanDuty);

    for (i = 0;i < ARMLED_RESERVE;i++)
    {
        servArmLedConfig((ArmLedEnum)i, g_systemInfo.armLedFreq[i], g_systemInfo.armLedDuty[i]);
    }

#else

    //��������
    //TO ADD NICK
    
#endif

#endif

    //��ʼ���������ģ��
    cmdMainParseInit();

    //��ʼ��������֤ģ��
    pvrfParaLimitInit();
    /*------------------------------ϵͳ��ʼ��������Ӳ�����裬FPGA�Լ����-----------------------------------*/

#if SDIO_CRC_ERROR_ENABLE
    servFpgaSdioCrcErrorIntSet(SENSOR_ON);
#endif


#if SDIO_TX_RX_TEST
    SystemSdioTxRxTest();
#endif

    
    OS_TaskSuspend((OS_TCB*)&SystemInitTaskTCB, &err);    //����ʼ����
    
    while (DEF_TRUE) 
    {
    }
}


/*********************************************************************************************
�� �� ��: SystemOSCreate;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void SystemOSCreate(void * pHCLKFrequency)
{
    OS_ERR  err;

    
    /*********************����ϵͳ����*********************/
    CPU_IntDis();
    
    CPU_Init(); 
    
    //��ʼ������ϵͳ
    OSInit(&err);

    //������������
    OSTaskCreate((OS_TCB    *)&SystemInitTaskTCB,
                 (CPU_CHAR  *)"System Init Start",
                 (OS_TASK_PTR)SystemInitTask,
                 (void      *)pHCLKFrequency,
                 (OS_PRIO    )APP_TASK_PRIO_SYSTEM_INIT,
                 (CPU_STK   *)&SystemInitTaskStk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_SYSTEM_INIT / 10,    //(CPU_STK_SIZE)AppTaskStartStk[APP_TASK_STK_SIZE_SYSTEM_INIT / 10],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE_SYSTEM_INIT,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
    
    //��������ϵͳ
    OSStart(&err);
    /*********************����ϵͳ����*********************/

}


/*********************************************************************************************
�� �� ��: sysTaskCreate;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void sysTaskCreate(SystemTaskEnum sysTask)
{
    if (SYSTASK_CMDPARSE == sysTask)
    {
        //�����������
        OSTaskCreate((OS_TCB    *)&CmdParseTaskTCB,
                     (CPU_CHAR  *)"Cmd Parse Task",
                     (OS_TASK_PTR)CmdParseTask,
                     (void      *)0,
                     (OS_PRIO    )APP_TASK_PRIO_CMD_PARSE,
                     (CPU_STK   *)&CmdParseTaskStk[0],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_CMD_PARSE / 10,    //(CPU_STK_SIZE)CmdParseTaskObjStk[APP_TASK_STK_SIZE_CMD_PARSE / 10],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_CMD_PARSE,
                     (OS_MSG_QTY )0,
                     (OS_TICK    )0,
                     (void      *)0,
                     (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                     (OS_ERR    *)NULL);
    }
    else if (SYSTASK_PVTCALC == sysTask)
    {             
        //PVT��������
        OSTaskCreate((OS_TCB    *)&PvtProcTaskTCB,
                     (CPU_CHAR  *)"Pvt Proc Task",
                     (OS_TASK_PTR)PvtProcTask,
                     (void      *)0,
                     (OS_PRIO    )APP_TASK_PRIO_PVT_CALC,
                     (CPU_STK   *)&PvtProcTaskStk[0],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_PVT_CALC / 10,    //(CPU_STK_SIZE)PvtCalcTaskObjStk[APP_TASK_STK_SIZE_PVT_CALC / 10],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_PVT_CALC,
                     (OS_MSG_QTY )0,
                     (OS_TICK    )0,
                     (void      *)0,
                     (OS_OPT     )(OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                     (OS_ERR    *)NULL);
    }
    else if (SYSTASK_FUNCTION == sysTask)
    {
        //Function����
        OSTaskCreate((OS_TCB    *)&FunctionTaskTCB,
                     (CPU_CHAR  *)"Function Task",
                     (OS_TASK_PTR)FunctionTask,
                     (void      *)0,
                     (OS_PRIO    )APP_TASK_PRIO_FUNCTION,
                     (CPU_STK   *)&FunctionTaskStk[0],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_FUNCTION / 10,    //(CPU_STK_SIZE)FunctionTaskStk[APP_TASK_STK_SIZE_FUNCTION / 10],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_FUNCTION,
                     (OS_MSG_QTY )0,
                     (OS_TICK    )0,
                     (void      *)0,
                     (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                     (OS_ERR    *)NULL);
    }
    else if (SYSTASK_PERIODIC == sysTask)
    {
        //����������
        OSTaskCreate((OS_TCB    *)&PeriodicTaskTCB,
                     (CPU_CHAR  *)"Periodic Task",
                     (OS_TASK_PTR)PeriodicTask,
                     (void      *)0,
                     (OS_PRIO    )APP_TASK_PRIO_PERIODIC,
                     (CPU_STK   *)&PeriodicTaskStk[0],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_PERIODIC / 10,    //(CPU_STK_SIZE)PeriodTaskStk[APP_TASK_STK_SIZE_PERIODIC / 10],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_PERIODIC,
                     (OS_MSG_QTY )0,
                     (OS_TICK    )0,
                     (void      *)0,
                     (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                     (OS_ERR    *)NULL);
    }
    else if (SYSTASK_EVENMANG == sysTask)
    {
        //�¼���������
        OSTaskCreate((OS_TCB    *)&EventManageTaskTCB,
                     (CPU_CHAR  *)"Event Manage Task",
                     (OS_TASK_PTR)EventManageTask,
                     (void      *)0,
                     (OS_PRIO    )APP_TASK_PRIO_EVENT_MANAGE,
                     (CPU_STK   *)&EventManageTaskStk[0],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_EVENT_MANAGE / 10,    //(CPU_STK_SIZE)EventManageTaskStk[APP_TASK_STK_SIZE_EVENT_MANAGE / 10],
                     (CPU_STK_SIZE)APP_TASK_STK_SIZE_EVENT_MANAGE,
                     (OS_MSG_QTY )0,
                     (OS_TICK    )0,
                     (void      *)0,
                     (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                     (OS_ERR    *)NULL);
     }            
}


/*********************************************************************************************
�� �� ��: sysTaskDelete;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void sysTaskDelete(SystemTaskEnum sysTask)
{
    if (SYSTASK_CMDPARSE == sysTask)
    {
        //�����������
        OSTaskDel((OS_TCB *)&CmdParseTaskTCB, (OS_ERR *)NULL);
    }
    else if (SYSTASK_PVTCALC == sysTask)
    {             
        //PVT��������
        OSTaskDel((OS_TCB *)&PvtProcTaskTCB, (OS_ERR *)NULL);
    }
    else if (SYSTASK_FUNCTION == sysTask)
    {
        //Function����
        OSTaskDel((OS_TCB *)&FunctionTaskTCB, (OS_ERR *)NULL);
    }
    else if (SYSTASK_PERIODIC == sysTask)
    {
        //����������
        OSTaskDel((OS_TCB *)&PeriodicTaskTCB, (OS_ERR *)NULL);
    }
    else if (SYSTASK_EVENMANG == sysTask)
    {
        //�¼���������
        OSTaskDel((OS_TCB *)&EventManageTaskTCB, (OS_ERR *)NULL);
    }            
}


/*********************************************************************************************
�� �� ��: sysTaskClear;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void sysTaskClear(SystemTaskEnum sysTask)
{
    u8 i;
    u8 j;

    
    if (SYSTASK_CMDPARSE == sysTask)
    {
    }
    else if (SYSTASK_PVTCALC == sysTask)
    {             
        g_semPvtCalcTask.Ctr = 0;

        //ɾ���ٴ���    CJ 2017.11.23 Add
        sysTaskDelete(SYSTASK_PVTCALC);
        sysTaskCreate(SYSTASK_PVTCALC);

        //��ս������
        for (i = 0;i < CH_TOTAL;i++)
        {
            for (j = 0;j < PVT_CALC_QUEUE_SIZE;j++)
            {
                g_systemState.calcQueue[i][j] = WTTYPE_RESERVE;
            }
            g_systemState.calcIndex[i] = 0;
            g_systemState.tailIndex[i] = 0;
        }
    }
    else if (SYSTASK_FUNCTION == sysTask)
    {
    }
    else if (SYSTASK_PERIODIC == sysTask)
    {
    }
    else if (SYSTASK_EVENMANG == sysTask)
    {
    } 
}



/*******************************************�ļ�β********************************************/
