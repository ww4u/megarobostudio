/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  comStruct.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.01.10;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __COM_STRUCT_H
#define __COM_STRUCT_H



/******************************************�����ļ�*******************************************/
#include "project.h"
#include "comEnum.h"
#include "bspTimer.h"
#include "comInterface.h"



/***************************************��������������****************************************/
/*-------------��Buffer�ṹ��-------------*/
typedef struct 
{
    bool bNotEmpty    :1;      //�Ƿ�������
    bool bRewind      :1;      //�Ƿ����
    bool bAlmostFull  :1;      //�Ƿ������
    u8   reverse      :5;      //����

    u8   frameCount;
    
    u16  bufferSize;
    u16  headOffset;
    u16  tailOffset;
    u16  dataTailOffset;       //��Ч���ݵ�β��
    u16  reserveMemorySize;    
    u8   *pBuffer;
    
}StreamBufferStruct;

/*-------------��ʱ���ṹ��-------------*/
typedef struct
{
  bool         bUsed;            //�Ƿ�����
  u16          period;           //��ʱʱ��
  u64          timeout;          //��ʱʱ��
  TimeOutFunc  pTimerOutFunc;    //��ʱ�ص�����
  
}SoftTimerStruct;

/*-------------PVT��ṹ��-------------*/
typedef struct
{
    f32 position;    //λ��
    f32 speed;       //�ٶ�
    f32 time;        //ʱ��
      
}PvtPointStruct;

/*-------------��ͣ���ٽṹ��-------------*/
typedef struct
{
    StopModeEnum stopMode;        //����ֹͣģʽ
    
    f32  distance;    //ָ��������ֹͣ��ʵ��ʹ�õ�ʱ��Ҫ����ɲ���
    f32  time;        //ָ��ʱ����ֹͣ
      
}StopDecelerationStruct;

/*-------------��ͣ���ٽṹ��-------------*/
typedef struct
{
    SensorStateEnum  lineState;
    SensorStateEnum  totalState;
    
    ResponseTypeEnum lineResp;    //�߼�ʧ����Ӧ
    ResponseTypeEnum totalResp;   //�г���ʧ����Ӧ
    
    u16  lineOutNum;     //�߼�ʧ��������
    u32  totalOutNum;    //�г���ʧ��������
      
}OutOfStepInfoStruct;

/*-----PVT��Ϣ�ṹ��------*/
typedef struct
{
    bool bClear;      //�����Ƿ������
    bool bRewind;     //PVT��Buffer�Ƿ����
    
    u8 pointIndex;    //Buffer����һ�����д��λ��
    u8 tailPoint;     //Buffer��δ����ĵ���
    
    u16 pvtBufferSize;    //PVT��BUFFER�Ĵ�С���ϵ��ʼ���󲻿��޸�
    
    PvtPointStruct *pvtPoint;    //PVT������

    //�ܹ�12���ֽ�
      
}PvtManageStruct;

/*-----PVT��Ϣ�ṹ��------*/
typedef struct
{
    PvtPointStruct mainPvtPoint[PVT_POINT_BUFFER_SIZE];    //PVT������
    
    PvtPointStruct smallPvtPoint[SMALL_PVT_BUFFER_SIZE];    //PVT������
    
    PvtPointStruct presetPvtPoint[PRESET_RESERVE][PRESET_PVT_BUFFER_SIZE];    //PVT������

    u32 crc;

    //�ܹ�12*256+12*4+12*8*32+4 = 6196���ֽ�
      
}PvtInfoStruct;

/*-----�滮����ṹ��------*/
typedef struct
{
    ExecModeEnum    execMode;       //ִ��ģʽ
    PlanModeEnum    planMode;       //�滮ģʽ
    MotionModeEnum  motionMode;     //�˶�ģʽ
    ModifyDutyEnum  modifyDuty;     //LVTģʽ��ʱ��ĵ���ռ��
    
    EndStateEnum endState;    //���н���״̬
    u8  validPoint;     //Buffer����Ч����
    u8  warnPoint;      //FIFOģʽ�������

    u16 accScale;       //S���߼��ٶ�ʱ��ռ��
    u16 decScale;       //S���߼��ٶ�ʱ��ռ��
    
    u32 cycleNum;       //ѭ��ģʽ��ѭ����
    
    StopDecelerationStruct stopDecelInfo;    //ֹͣ����
    OutOfStepInfoStruct    outOfStepInfo;    //ʧ����Ϣ

    //�ܹ�40���ֽ�
      
}PlanManageStruct;

/*--------�滮��Ϣ�ṹ��---------*/
typedef struct
{
    PlanManageStruct plan[CH_TOTAL][WTTYPE_RESERVE];

    u32 crc;    //40*CH_TOTAL*WTTYPE_RESERVE + 4���ֽ�(����Ϊ404����8��Ϊ3204)
      
}PlanInfoStruct;

/*-------------���������ݽṹ��-------------*/
typedef struct
{
    bool bLastFrame;   //�Ƿ������һ֡����
    bool bSendEmpty;   //�Ƿ��Ϳ����ݣ�true--���Ϳ����ݣ����ݳ��ȹ̶�Ϊ3

    u8  toSendBlkSize;   //��Ч���ݵĿ鳤�ȣ�SDIO�����ã������2^14=16384
    u8  emptyBlkSize;    //�����ݵĿ鳤�ȣ�SDIO������
    
    u16 lastSendBytes;   //�ϴη��͵��ֽڳ���
    u16 noSendPoints;    //δ���͵����ݳ��ȣ���ʾ���ǲ������
    
    u16 validPoints;     //���η�������Ч�������
    u16 validBytes;      //��Ч���ֽڳ��ȣ�����writeAddr�ĳ���
    
    u16 validEmpPoints;  //���η�������Ч�ղ������
    u16 validEmpBytes;   //��Ч�Ŀ��ֽڳ��ȣ�����writeAddr�ĳ���

    u16 toSendPoints;    //Ҫ���͵Ĳ������
    u16 toSendBytes;     //Ҫ���͵��ֽڳ��ȣ�����writeAddr�ĳ���
    
    u16 emptyPoints;     //Ҫ���͵Ŀղ������
    u16 emptyBytes;      //Ҫ���͵Ŀ��ֽڳ��ȣ�����emptyDataAddr�ĳ���
    
    u32 totalPoints;     //�ܷ��͵����ݳ��ȣ���ʾ���ǲ������
    
    u32 minPeriod;       //��С������ֵ��Ҳ���������ٶ�
    
    u32 writeAddr;                        //�����д��ַ����FUNC��ֵ��ʹ��
    u32 data[OUTPUT_DATA_BUFFER_SIZE];    //����Buffer

    u32 emptyDataAddr;                          //�����ݵĵ�ַ����FUNC��ֵ��ʹ��
    u32 emptyData[OUTPUT_EMPTY_BUFFER_SIZE];    //������Buffer����������Ϊ0
      
}OutputDataStruct;

/*-------------�����Ϣ�ṹ��-------------*/
typedef struct
{
    MotorTypeEnum motorType;          //����
    StepAngleEnum stepAngel;          //�����
    MicroStepEnum microSteps;         //΢����
    MotorSizeEnum motorSize;          //����ߴ�
    
    IntfcStateEnum   encoderState;      //������״̬
    EncoderTypeEnum  encoderType;       //����������
    EncoderChanEnum  encoderChanNum;    //������ͨ����
    EncoderMultiEnum encoderMult;       //�������źű���
    
    PostUnitEnum  posnUnit;    //ϵͳ���й�����ʹ�õĵ�λ��PVT��StopDecelʹ��    CJ 2017.06.29 Add
    u8  motorVolt;             //�����ѹ
    u8  motorCurr;             //�����������λ0.1mA��ֻ������ʾ
    u8  feedbackRatio;         //��������������
    
    u16 gearRatioNum;       //���ٱȷ���
    u16 gearRatioDen;       //���ٱȷ�ĸ
    
    f32 lead;               //ֱ���˶�ʱ�ĵ��̣���λmm/cycle
    
    f32 peakSpeed;          //��������ٶ�
    f32 peakAcc;            //������м��ٶ�    CJ 2017.05.24 Add
    
    f32 backlash;           //�����϶
    
    u32 encoderLineNum;     //����������
    //�ܹ�36���ֽ�
    
}MotorManageStruct;

/*-------------�����Ϣ�ṹ��-------------*/
typedef struct
{
    MotorManageStruct motor[CH_TOTAL];

    u32 crc;    //36*CH_TOTAL + 4���ֽ�(����Ϊ40����8��Ϊ292)
    
}MotorInfoStruct;

/*-------------��ͣ���ýṹ��-------------*/
typedef struct
{
    StopModeEnum stopMode;    //����ֹͣģʽ
    
    u32  stopSteps;    //ָ��������ֹͣ
    f32  stopTime;     //ָ��ʱ����ֹͣ
    
    u32  emergPeriod;    //ֹͣ���ڣ�����ֵ���ڴ�ֵ�����Ϊ����ֱ��ֹͣ��
    u32  minPeriod;      //��С����ֵ����ʵ��������ٶ�
      
}StopConfigStruct;

/*-------------������Ϣ�ṹ��-------------*/
typedef struct
{
    StateReportEnum  stateReport;      //״̬�ϱ���ʽ
    PostUnitEnum     initPostUnit;     //��ʼλ��ʹ�õĵ�λ���ɴ��������ֵ�ĵ�λ����
    TrigInPinEnum    initIOPin;        //��Ϊ��ʼλ��IO�ܽ�
    SensorStateEnum  offsetState;

    SensorStateEnum  revMotion;    //��������
    
    StartSourceEnum  startSrc;         //����Դ
    ReceiveTypeEnum  startType;        //����ID����
    
    f32  maxSpeed;
    f32  minSpeed;
    f32  maxPosn;
    f32  minPosn;
    f32  maxTorque;
    f32  minTorque;
    f32  maxAcc;       //�����С���ٶ�    CJ 2017.05.24 Add
    f32  minAcc;
    u32  origin;
    
    //�ܹ�18*4=72���ֽ�
      
}MotionManageStruct;

/*-------------������Ϣ�ṹ��-------------*/
typedef struct
{
    MotionManageStruct motion[CH_TOTAL];
    
    u32 crc;    //72*CH_TOTAL + 4���ֽ�(����Ϊ76����8��Ϊ580)
      
}MotionInfoStruct;

/*-------------�ϱ�����Ϣ�ṹ��-------------*/
typedef struct
{
    SensorStateEnum state[REPTTYPE_RESERVE];
    u32             period[REPTTYPE_RESERVE];    //�ϱ�����

    //�ܹ�72�ֽ�
    
}ReportManageStruct;

/*-------------�ϱ�����Ϣ�ṹ��-------------*/
typedef struct
{
    ReportManageStruct report[CH_TOTAL];
    
    u32 crc;    //72*CH_TOTAL + 4���ֽ�(����Ϊ76����8��Ϊ580)
    
}ReportInfoStruct;

/*-------------OTP��Ϣ�ṹ��-------------*/
typedef struct
{
    SensorStateEnum   state;
    ResponseTypeEnum  response;
    u16               threshold;
    u32               period;
    
}OtpInfoStruct;

/*------------���ִ������ṹ��-----------*/
typedef struct
{
    TriggerModeEnum   trigMode;
    
    SensorStateEnum   pattState;    //���ʹ���ʱ��״̬
    ResponseTypeEnum  pattResponse;
    SamplingModeEnum  pattSMode;
    
    f32               pattSPeriod;
    
    SensorStateEnum   levelState[TRIGPIN_RESERVE];    //��ƽ����ʱ��״̬
    ResponseTypeEnum  levelResponse[TRIGPIN_RESERVE];
    SamplingModeEnum  levelSMode[TRIGPIN_RESERVE];
    
    TrigInTypeEnum    type[TRIGPIN_RESERVE];    //���ִ���ģʽ����
    
    f32               levelSPeriod[TRIGPIN_RESERVE]; 

    //�ܹ�48(����)/24(����)���ֽ�
    
}TrigInManageStruct;

/*------------���ִ������ṹ��-----------*/
typedef struct
{
    TrigInManageStruct trigIn[CH_TOTAL];
    
    u32 crc;    //����Ϊ48+4=52����8��Ϊ24*8+4=196
    
}TrigInInfoStruct;

/*------------���ִ������ṹ��-----------*/
typedef struct
{
    SensorStateEnum  state;
    DIOSignalEnum    signal;
    DIOPolarityEnum  polarity;
    ChannelEnum      source;
    DIOCondtEnum     condition;
    u16              duty;
    u32              period;    

    //�ܹ�12�ֽ�
    
}DigitalOutManageStruct;

/*------------���ִ������ṹ��-----------*/
typedef struct
{
    DigitalOutManageStruct output[DIO_RESERVE]; 
    
    u32 crc;    //����Ϊ2*12+4=28����8��Ϊ4*12+4=52
    
}DigitalOutStruct;

#ifdef PROJECT_QUBELEY
/*-----------ģ�⴫�����ṹ��------------*/
typedef struct
{
    SensorStateEnum   state;
    ResponseTypeEnum  responseH;    //CJ 2017.05.18 Modify
    ResponseTypeEnum  responseL;    //CJ 2017.05.18 Modify
    
    u16 adcCodeH;
    u16 adcCodeL;
    
    f32 thresholdH;
    f32 thresholdL;   
    
    u32 crc;//ǰ����16���ֽڣ���CRC�ܹ�20���ֽ�
    
}AnalogInfoStruct;
#endif

/*-----------�ռ�����ṹ��------------*/
typedef struct
{
    float xAxis;
    float yAxis;
    float zAxis;
    
}CoordinateStruct;

/*-----------�Ƕȴ�������ֵ�ṹ��------------*/
typedef struct
{
    u16  xCode;
    u16  yCode;
    u16  zCode;
    
}SensorCodeStruct;

/*----------���������ݽṹ��---------*///�������ϱ����ͽǶȴ��������ϱ����ݣ������ݺ����ò������룬ͳһ��������
typedef struct
{
    u32 reporterData[CH_TOTAL][REPTTYPE_RESERVE];    //�ϱ�������

    u32 temperature;
    
}SensorDataStruct;


/*-------------�������޽ṹ��-------------*/
typedef struct
{
    u8  snUartFrameLen;
    u8  snUartRecvNum;
    u8  feedbackRatio;
    
    s16 otpThr;
    s16 canGroup;
    s32 canSendId;
    s32 canReceiveId;
    s32 canGroupId;    
    s32 canRadioId;

    s32 pvtStartPoint;
    s32 pvtEndPoint;
    u32 pvtNcycles;
    s32 pvtWarnPoint;
    
    s16 motorGearRatio;
    s16 motorVolt;
    s16 motorCurr;
    f32 motorPeakSpeed;
    f32 motorPeakAcc;
    f32 motorLead;
    f32 motorBacklash;
    u32 encoderLineNum;
    
    f32 pvtPosn;
    f32 pvtSpeed;
    f32 pvtTime;

    f32 stopTime;
    f32 stopDistance;
    f32 endSpeed;
    
    f32 initPostValue;
    f32 inchingTimeValue;
    f32 inchingDisValue;
    
    f32 motionMaxSpeed;
    f32 motionMaxPosn;
    f32 motionMaxTorque;
    f32 motionMaxAcc;

    s32 outOfStepNum;

    u32 reportPeriod;
    f32 trigInPeriod;
    u32 trigOutPeriod;
    u16 trigOutDuty;
    
    f32 snUartSwTime;
    
    f32 asensorThr;

    u32 fpgaPwmClock;    //CJ 2017.04.12 Add

    u32 sgLimit;
    
    u16 speedScale;
    
}UpLimitStruct;

/*-------------�������޽ṹ��-------------*/
typedef struct
{
    u8  snUartFrameLen;
    u8  snUartRecvNum;
    u8  feedbackRatio;
    
    s16 otpThr;
    s16 canGroup;
    s32 canSendId;
    s32 canReceiveId;
    s32 canGroupId;    
    s32 canRadioId;

    s32 pvtStartPoint;
    s32 pvtEndPoint;
    u32 pvtNcycles;
    s32 pvtWarnPoint;
    
    s16 motorGearRatio;
    s16 motorVolt;
    s16 motorCurr;
    f32 motorPeakSpeed;
    f32 motorPeakAcc;
    f32 motorLead;
    f32 motorBacklash;
    u32 encoderLineNum;
    
    f32 pvtPosn;
    f32 pvtSpeed;
    f32 pvtTime;

    f32 stopTime;
    f32 stopDistance;
    f32 endSpeed;
    
    f32 initPostValue;
    f32 inchingTimeValue;
    f32 inchingDisValue;
    
    f32 motionMinSpeed;
    f32 motionMinPosn;
    f32 motionMinTorque;
    f32 motionMinAcc;

    s32 outOfStepNum;

    u32 reportPeriod;
    f32 trigInPeriod;
    u32 trigOutPeriod;
    u16 trigOutDuty;
    
    f32 snUartSwTime;
    
    f32 asensorThr;

    u32 fpgaPwmClock;    //CJ 2017.04.12 Add

    u32 sgLimit;
    
    u16 speedScale;
    
}DownLimitStruct;

/*-----------���������޽ṹ��-------------*/
typedef struct
{
    UpLimitStruct   upLimit;
    DownLimitStruct downLimit;
    
}ParaLimitStruct;

typedef struct
{
    u8  hard;
    u8  major;
    u8  minor;
    u8  build;
    
}FpgaVersionStruct;

typedef union
{
    u8  verArray[FPGA_VER_LEN];

    FpgaVersionStruct verStruct;
    
}FpgaVersionUnion;

/*-------------�豸��Ϣ�洢�ṹ��-----------*/
typedef struct
{
    SubDeviceModelEnum sDevcModel;    //���ͺ�
    
    u8 deviceSn[DEVICE_SN_LEN];       //���к�

    u32 crc;    //�ܹ�1+20+4=24���ֽ�
    
}DeviceInfoStorageStruct;

/*-------------�豸��Ϣ�ṹ��-----------*/
typedef struct
{
    DeviceInfoStorageStruct storageInfo;    //���ͺź����кţ��洢��FLASH����EEPROM��

    MainDeviceModelEnum mDevcModel;    //���ͺţ��̶�ΪMDMODEL_MRQ
    SubDeviceModelEnum  sDevcModel;    //���ͺţ��˿ڶ�����ֵ

    DriverBoardTypeEnum drvBoardTypeDn;    //�°��ʹ������
    DriverBoardTypeEnum drvBoardTypeUp;    //�ϰ��ʹ������

    FpgaVersionUnion fpga;

    u8 software[SOFTWARE_VER_LEN];    //��֧�汾��+��汾��+С�汾��+����汾��, ����汾�Ų�����
    u8 hardware[HARDWARE_VER_LEN];
    u8 boot[BOOT_VER_LEN];
    
    u8 driverBoardDn[DRVBOARD_VER_LEN];    //�°��Ӳ���汾��
    u8 driverBoardUp[DRVBOARD_VER_LEN];    //�ϰ��Ӳ���汾��
    
}DeviceInfoStruct;

/*-------------ϵͳ��Ϣ�ṹ��-----------*/
typedef struct
{    
    WorkModeEnum     workMode; 
    PowerOnModeEnum  powerOn;

    u8 group[CH_TOTAL][GROUP_NUM];    //Ĭ��129���û������ã���Χ130-255

    MainLabelEnum mainLabel[CH_TOTAL];    //����ǩ
    SubLabelEnum  subLabel[CH_TOTAL];     //�ӱ�ǩ

    OtpInfoStruct otpInfo;    //OTP��Ϣ��8�ֽ�

#if PVT_CALC_USE_FPGA_CLOCK_ERROR
    f32 fpgaPwmClock;         //FPGA���PWM��ʱ�Ӻͱ�׼ʱ�ӵ����
    
#else

    u32 fpgaPwmClock;         //FPGA���PWM��ʱ��
#endif

    u32 crc;    //�ܹ�(����24���ֽڣ�����36���ֽ�)
    
}SystemInfoStruct;

/*-------------λ��ת����Ϣ�ṹ��-----------*/
typedef struct
{
    f32 posnToStep;      //λ�õ�������ת��ϵ��
    f32 posnToLine;      //λ�õ�������������ת��ϵ�� 
    f32 lineSteps;       //�߼䲽
    u32 lineMult;        //�����ı���
    
}PosnConvertInfoStruct;

/*-------------ϵͳ״̬�ṹ��-----------*/
typedef struct
{
    EnginModeEnum    EnginMode;       //����ģʽ
    u8               chanNum;         //ϵͳ֧�ֵ�ͨ����
    SysStateEnum     systemState;     //��ǰϵͳģʽ
    DistingStateEnum distState;       //ʶ��״̬
    
    ClockSyncStateEnum clockSyncState;   //ʱ��ͬ����״̬
    ClockSyncRegEnum   clockSyncReg;     //ʱ��ͬ���Ĵ�����״̬
    ReceiveTypeEnum    clockSyncType;    //ʱ��ͬ��ʱ����ID����
    
    LevelStatusEnum    levelStatus;

    SensorStateEnum    reportSwitch;      //�Զ��ϱ��ܿ���
        
    ChanStateEnum      chanState[CH_TOTAL];        //ͨ��״̬
    ChanStateEnum      lastChanState[CH_TOTAL];    //�ϴ�ͨ��״̬
    
    WaveTableTypeEnum  calcWaveTable[CH_TOTAL];    //��ǰ����Ĳ���
    WaveTableTypeEnum  outpWaveTable[CH_TOTAL];    //��ǰ����Ĳ���
    
    WaveTableTypeEnum  calcQueue[CH_TOTAL][PVT_CALC_QUEUE_SIZE];    //�������
    u8                 calcIndex[CH_TOTAL];                         //������е�ǰ��������
    u8                 tailIndex[CH_TOTAL];                         //������ж�β����
        
    
    u8                 errorCode[ERROR_CODE_LEN];    //�������
    
    DriverTypeEnum     drvType[CH_TOTAL];     //��������
    u8                 drvState[CH_TOTAL];    //����״̬
    
    SensorStateEnum    revMotion[CH_TOTAL];    //�������еĿ���״̬
    
    u16  eventValue;

    s16  circleNum[CH_TOTAL];
    s32  steps[CH_TOTAL];
    s32  abCount[CH_TOTAL];
    s32  lastAbCount[CH_TOTAL];
    s32  pvtSteps[CH_TOTAL];
    u32  cycleNumCount[CH_TOTAL];
    
    u32  clockCount;        //ʱ��ͬ������
    u32  currSteps;         //�����ǰ����λ�ã��������0λ�Ĳ�����¼

    u32  pvtCalcTaskSem;        //��ʼ��ʱ��PVTCALC������ź�������

    PosnConvertInfoStruct posnConvertInfo[CH_TOTAL];
    
    ConfigStateEnum configState[CH_TOTAL][WTTYPE_RESERVE];   //PVT����������״̬
    PvtManageStruct pvtManage[CH_TOTAL][WTTYPE_RESERVE];
    
}SystemStateStruct;


/*--------ͨ������λͼ�ṹ��-------*/
typedef struct
{
    u8 bWaveTableData :1;    //��������
    u8 bPrepare       :1;    //�����˶�׼��
    u8 bStop          :1;    //ֹͣ
    u8 bEmergStop     :1;    //����ֹͣ 
    u8 bQryReady      :1;    //��ѯFPGA��Ready״̬
    u8 bCycleQry      :1;    //��ѯѭ������ 
    u8 bEncoder       :1;    //������
    u8 bOutOfStep     :1;    //����ʧ��
    
    u8 bTrigIn        :1;    //���ô�������
    u8 bStartSrc      :1;    //��������Դ
    u8 bRevMotion     :1;    //���÷�������״̬
    u8 bReportTorque  :1;    //xyzheng �ϱ����� 
    u8 bMicroStep     :1;    //���ϸ������
    u8 bCurrent       :1;    //��������
    u8 bDriverState   :1;    //����״̬����
    u8 bDriverQuery   :1;    //��ѯ����״̬
    
    u8 bDrvCtrl       :1;    //����DRVCTRL�Ĵ���
    u8 bChopConfig    :1;    //����CHOPCONF�Ĵ���
    u8 bSmarten       :1;    //����SMARTEN�Ĵ���
    u8 bSgcsConfig    :1;    //����SGCSCONF�Ĵ���
    u8 bDrvConfig     :1;    //����DRVCONF�Ĵ���
    u8 bMotnMode      :1;    //
    u8 bModifyDuty    :1;    //
    u8 bLineMult      :1;    //
    
    u8 reserve        :8;    //Ԥ��
    
}ChanCfgBmpStruct;


/*--------ϵͳ����λͼ�ṹ��-------*/
typedef struct
{
    u8 bRun           :1;    //��ʼ���
    u8 bDebugMotor    :1;    //�������  xyzheng
    u8 bClockReg      :1;    //����ʱ��ͬ���Ĵ���
    u8 bClockCount    :1;    //��ȡʱ��ͬ������ֵ    CJ 2017.04.11 Add  
    u8 bAnalogIn      :1;    //ģ������ 
    u8 bQryTemp       :1;    //��ѯ�¶�
    u8 bDigitalOut1   :1;    //�����������1
    u8 bDigitalOut2   :1;    //�����������2
    
    u8 bDigitalIO3    :1;    //��������IO3
    u8 bDigitalIO4    :1;    //��������IO4
    u8 bIsolatorOut1  :1;    //���ø������1
    u8 bIsolatorOut2  :1;    //���ø������2
    u8 bSensorUart1   :1;    //���ô�����UART1
    u8 bSensorUart2   :1;    //���ô�����UART2
    u8 bIsolatorIn    :1;    //���ø�������
    
    u8 reserve        :1;    //Ԥ��
    
}SystemCfgBmpStruct;


typedef struct 
{
    u8   u8BootFlag;    //�Ƿ���Ҫ��������ģʽ  1Byte
    u8   u8AppFlag;     //Ӧ�ó����Ƿ�����OK  1Byte  APP_IMAGE_OK--0xA5����APP�ǿ������е�
    u8   u8TryCount;    //APP���Խ��ж�γ���(�ݶ�3��)��ֻ��������γ��Զ��������к����ΪAPP�޷�ʹ�� CJ 2017.05.19 Add
    
    u8   au8BootVer[BOOT_VER_LEN];/*����汾��-> ��8λ�����汾��;��8λ���ΰ汾��.  2Bytes */
    u32  u32AppBaseAddr;   //Ӧ�ó�����flash�еĵ�ַ   4Bytes
    u32  u32FpgaBaseAddr;  //FPGA��flash�еĵ�ַ  4bytes
    
    u8   resv[20];    //ǰ����16���ֽڣ�Ԥ��20���ֽڣ���CRC�ܹ�40���ֽ�
    u32  crc;
    
}BootInfoStruct;

/*-------------����ṹ��-------------*/
typedef struct
{
    bool bEndIntEnable;    //�����жϵ�ʹ��
    bool bCycleQuery;      //��ѯѭ����
    
    MotionStateEnum     lastState;       //�����ϴ�״̬
    MotionStateEnum     waveState;       //����ǰ״̬
    WaveWorkModeEnum    waveWorkMode;    //������ģʽ

    u32 startAddr;       //������ʼ��ַ
    u32 totalSize;       //�����С����4Bytes������

    u32 writableAddr;    //��д��ַ����4Bytes������
    u32 writableSize;    //��д�Ĳ������

    f32 lineSteps;       //�߼䲽������Ĵ�����ֵ
    u32 lineMult;        //�����ı���

    StopConfigStruct stopConfigInfo;    //����ļ�ͣ������Ϣ
      
}WaveTableStruct;

/*------------��������ṹ��-----------*/
typedef struct
{
    SensorStateEnum  state;
    ChannelEnum      source;
    DIOCondtEnum     condition;  
    DIOPolarityEnum  response;

    //�ܹ�4�ֽ�
    
}IsolatorOutManageStruct;

/*------------��������ṹ��-----------*/
typedef struct
{
    IsolatorOutManageStruct output[YOUT_RESERVE]; 
    
    u32 crc;    //2*4+4=12��
    
}IsolatorOutStruct;

/*------------�������ṹ��-----------*/
typedef struct
{
    SensorStateEnum  state;       //״̬
    u8               SOF;         //֡ͷ
    u8               frameLen;    //֡����
    u8               recvNum;     //һ�������ڽ���֡��
    u32              swTime;      //�л�ʱ��(Qubeley��ʵ������)

    //�ܹ�8�ֽ�
    
}SensorManageStruct;

/*-----------������UART�ڽṹ��-------------*/
typedef struct
{
    UartIntfcStruct    uartIntfc[UARTNUM_RESERVE];   //8*2=16���ֽ�
    SensorManageStruct sensor[UARTNUM_RESERVE][SENSOR_RESERVE];      //����8*2=16���ֽڣ�����8*2*4=64���ֽ�
    
    u32 crc;    //����16+16+4=36���ֽڣ�����16+64+4=84���ֽ�
    
}SensorUartStruct;

/*------------��������ṹ��-----------*/
typedef struct
{
    IntfcStateEnum    state;       //��ƽ����ʱ��״̬
    ResponseTypeEnum  response;
    ChannelEnum       respChan;
    SamplingModeEnum  sMode;
    
    TrigInTypeEnum    type;
    
    u32 sPeriod; 

    u32 crc;    //�ܹ�16���ֽ�
  
}IsolatorInStruct;


/*-------------FPGA�ж�Դ�ṹ��-------------*/
typedef struct
{
    u16 stopSwitch      :1;    //ֹͣ����״̬
    u16 di2_x1StopState :1;    //
    u16 di3_x2StopState :1;    //
    u16 di4_x3StopState :1;    //
    u16 di5_x4StopState :1;    //
    u16 chanZStopState  :1;    //
    
    u16 resv1 :2;
    
    u16 alarmSwitch      :1;    //�澯����״̬
    u16 di2_x1AlarmState :1;    //
    u16 di3_x2AlarmState :1;    //
    u16 di4_x3AlarmState :1;    //
    u16 di5_x4AlarmState :1;    //
    u16 chanZAlarmState  :1;    //
    
    u16 outOfStep     :1;    //ʧ��������ֵ
    u16 waveEndState  :1;    //������������ж�
    
}IntrSourceStruct;


/*--------�¼�Դλͼ�ṹ��-------*/
typedef struct
{
    u8 bStateSwitch[CH_TOTAL];         //״̬�л�
    u8 bSenUart[UARTNUM_RESERVE][SENSOR_RESERVE];       //������Uart�������
    
    u8 bDistSen[DIST_SENSOR_NUM];      //��ഫ����

#if GELGOOG_SINANJU
    u8 bAbsEncoder[ABS_ENCODER_NUM];   //����ֵ������
#else
    u8 bAngleSen[ANGLE_SENSOR_NUM];    //�Ƕȴ�����
#endif

    u8 bFpgaInt          :1;    //��FPGA�����ж�
    u8 bAnalogInInt      :1;    //ģ�������ж�
    u8 bDeviceId         :1;    //�豸ʶ��
    u8 bXInMatch         :1;    //XINƥ��
    
    u8 reserve           :4;    //Ԥ��
    
}EventSrcBmpStruct;

typedef  union
{
    IntrSourceStruct intrSrc;

    u16 intrSrcValue;
    
}IntrSourceUnion;


#if 0
#endif


/*------------TMC2660��TMC262�ı���״̬-----------*/
typedef struct
{
    u8 bStall  :1;    //stallguard״̬, Ϊ1��ʾ�ﵽ��ת��ֵ SG_TST�ᱻ����
    u8 bOTDown :1;    //1��ʾ �¶ȴﵽ150  ����оƬ�رձ���
    u8 bOTWarn :1;    //1��ʾ �¶ȴﵽ100  �������±���
    u8 bS2GA   :1;    //1��ʾ A·�߶�mosfet�����ӵض�·
    u8 bS2GB   :1;    //1��ʾ B·�߶�mosfet�����ӵض�·
    u8 bOLA    :1;    //1��ʾ A·���ؿ�·  Ӧ�ǵ��û�ӻ�����Ȧ����
    u8 bOLB    :1;    //1��ʾ B·���ؿ�·  Ӧ�ǵ��û�ӻ�����Ȧ����
    u8 bSTST   :1;    //1��ʾ ����ֹͣ״̬,�ڹ�ȥ��һ��ʱ����û�н��յ����������ź�
    
}DriverWarnStruct;

/*------------TMC2660/262 DRVCTRL�Ĵ���SPIģʽ-----------*/
typedef struct
{
    u32 CB   :8;    //B�����ֵ����
    u32 PHB  :1;    //B���������
    u32 CA   :8;    //A�����ֵ����
    u32 PHA  :1;    //A���������
    
    u32 addr :2;    //�Ĵ�����ַ
    
    u32 resv_bit20_31 :12;
    
}DriverControlSpiStruct;

/*------------TMC2660/262 DRVCTRL�Ĵ���STEP/DIRģʽ-----------*/
typedef struct 
{
    u32 MRES          :4;    //ϸ������
    u32 resv_bit4_7   :4;
    
    u32 DEDGE         :1;    //����ģʽ
    u32 INTPOL        :1;    //�ڲ���ֵ���� 0:�����ڲ���ֵ,1:ʹ���ڲ���ֵ
    u32 resv_bit10_17 :8;
    
    u32 addr          :2;    //�Ĵ�����ַ
    
    u32 resv_bit20_31 :12;
    
}DriverControlStepDirStruct;

/*------------TMC2660/262 DRVCTRL�Ĵ���������-----------*/
typedef union
{
    DriverControlStepDirStruct stepDirMode;
    DriverControlSpiStruct     spiMode;
    
    u32 regValue;
    
}DriverControlUnion;

/*------------TMC2660/262 CHOPCONF�Ĵ���-----------*/
typedef struct
{
    u32 TOFF          :4;
    u32 HSTRT         :3;
    u32 HEND          :4;
    u32 HDEC          :2;
    u32 RNDTF         :1;
    u32 CHM           :1;    //ն��ģʽ����,0:��չ����ģʽ;1:��ͳ�ĺ㶨����˥��ʱ��
    u32 TBL           :2;    //����ʱ��
    
    u32 addr          :3;    //�Ĵ�����ַ
    
    u32 resv_bit20_31 :12;
    
}ChopperConfigStruct;

/*------------TMC2660/262 CHOPCONF�Ĵ���������-----------*/
typedef union
{
   ChopperConfigStruct regBitFiled;
    
    u32 regValue;
    
}ChopConfigUnion;

/*------------TMC2660/262 SMARTEN�Ĵ���-----------*/
typedef struct
{
    u32 SEMIN         :4;    //��SGС��SEMINʱ,��������
    u32 resv_bit4     :1;
    u32 SEUP          :2;    //�������ӱ�������
    u32 resv_bit7     :1;
    u32 SEMAX         :4;    //��SG����SEMAXʱ,��������
    u32 resv_bit12    :1;
    u32 SEDN          :2;    //������С��������
    u32 SEIMIN        :1;    //��͵������ű�������
    u32 resv_bit16    :1;
    
    u32 addr          :3;    //�Ĵ�����ַ
    
    u32 resv_bit20_31 :12;

}CoolStepCofingStruct;

/*------------TMC2660/262 SMARTEN�Ĵ���������-----------*/
typedef union
{
    CoolStepCofingStruct regBitFiled;
    
    u32 regValue;
    
}CoolStepCofingUnion;

/*------------TMC2660/262 SGCSCONF�Ĵ���-----------*/
typedef struct
{
    u32 CS            :5;    //�������ű���
    u32 resv_bit5_7   :3;
    u32 SGT           :7;    //���ʧ��ָʾ��ֵ stall guard threshold
    u32 resv_bit15    :1;
    u32 SFILT         :1;    //�Ƿ�����˲�
    
    u32 addr          :3;    //�Ĵ�����ַ
    
    u32 resv_bit20_31 :12;
    
}StallGuard2ConfigStruct;

/*------------TMC2660/262 SGCSCONF�Ĵ���������-----------*/
typedef union
{
    StallGuard2ConfigStruct regBitFiled;
    
    u32 regValue;
    
}StallGuard2ConfigUnion;

/*------------TMC2660/262 DRVCONF�Ĵ���-----------*/
typedef struct
{
    u32 resv_bit0_3   :4;
    u32 RDSEL         :2;    //�ض��Ĵ���ѡ��
    u32 VSENSE        :1;
    u32 SDOFF         :1;    //�ر�STEP/DIR�ӿ�
    u32 TS2G          :2;    //�ӵض�·���ʱ��
    u32 DISS2G        :1;    //�ӵض�·����ʹ��
    u32 resv_bit11    :1;
    u32 SLPL          :2;
    u32 SLPH          :2;
    u32 TST           :1;    //����
    
    u32 addr          :3;    //�Ĵ�����ַ
    
    u32 resv_bit20_31 :12;

}DriverConfigStruct;

/*------------TMC2660/262 DRVCONF�Ĵ���������-----------*/
typedef union
{
    DriverConfigStruct regBitFiled;
    
    u32 regValue;
    
}DriverConfigUnion;

/*------------TMC2660��TMC262�ı���״̬-----------*/
typedef struct 
{
    SensorStateEnum state;
    u8              warning;
    u32             period;
    
    /*
    1.SG��������          ����  bit0 = 1
    2.S2GA S2GB��Ϊ0    ����  bit1 = 1
    3.OTPW OT ��Ϊ0     ����  bit2 = 1
    4.΢����������          ����  bit3 = 1
    */

}TMCMonitorParaStruct;

/*------------SG��Ч���߱�����ֵ-----------*/
typedef struct 
{
    f32 UpperLimit;
    f32 LowerLimit;

}TMCSGLimitStruct;

/*------------SG��صļ�������  ׼���Ժ�ֻ���û������⼸��-----------*/
typedef struct
{
    u32 SGT;
    u32 SEMIN;
    u32 SEMAX;
    u32 SG0;
    
}TMCSGParaStruct;

/*------------��������ṹ��-----------*/
typedef struct
{
    SensorStateEnum  state; 
    DriverTypeEnum   type;
    u8               curr;         //������������λ0.1mA

    s8  sgUpLimit;    //SG�ֱ���Ϊ32/1024=0.03125��ʵ�ʴ洢���ǷŴ�100��������ֵ
    s8  sgDnLimit;
    
    s16 sgZero;
    s16 sgThreshold;
    s16 seMax;
    s16 seMin;
        
    //DRVCTRL : Driver Control Register
    DriverControlUnion DRVCTRL;
    
    //CHOPCONF: Chopper Configuration Register
    ChopConfigUnion CHOPCONF;
    
    //SMARTEN : coolStep Configuration Register
    CoolStepCofingUnion SMARTEN;
    
    //SGCSCONF: stallGuard2 Configuration Register
    StallGuard2ConfigUnion SGCSCONF;
    
    //DRVCONF : Driver Configuration Register
    DriverConfigUnion DRVCONF;

    //�ܹ�36�ֽ�
    
}DriverManageStruct;

/*------------������Ϣ�ṹ��-----------*/
typedef struct
{
    DriverManageStruct driver[CH_TOTAL]; 
    
    u32 crc;    //����Ϊ28+4=32����8��Ϊ8*28+4=228
    
}DriverInfoStruct;

/*------------TMC2660/262 �ض��Ĵ���-----------*/
typedef struct
{
    u32 SG   :1;
    u32 OT   :1;
    u32 OTPW :1;
    u32 S2GA :1;
    u32 S2GB :1;
    u32 OLA  :1;
    u32 OLB  :1;
    u32 STST :1;
    
    u32 resv_bit8_9 :2;
    
    u32 MSTEP :10;
    
    u32 resv_bit20_31 :12;

}DriverReadRespOneStruct;

/*------------TMC2660/262 �ض��Ĵ���-----------*/
typedef struct
{
    u32 SG   :1;
    u32 OT   :1;
    u32 OTPW :1;
    u32 S2GA :1;
    u32 S2GB :1;
    u32 OLA  :1;
    u32 OLB  :1;
    u32 STST :1;
    
    u32 resv_bit8_9 :2;
    
    u32 SGALL :10;
    
    u32 resv_bit20_31 :12;

}DriverReadRespTwoStruct;

/*------------TMC2660/262 �ض��Ĵ���-----------*/
typedef struct
{
    u32 SG   :1;
    u32 OT   :1;
    u32 OTPW :1;
    u32 S2GA :1;
    u32 S2GB :1;
    u32 OLA  :1;
    u32 OLB  :1;
    u32 STST :1;
    
    u32 resv_bit8_9 :2;
    
    u32 SE  :5;
    u32 SGH :5;
    
    u32 resv_bit20_31 :12;

}DriverReadRespThreeStruct;


/*------------�����ض��Ĵ���������-----------*/
typedef union
{
    DriverReadRespOneStruct   readRespOne; 
    DriverReadRespTwoStruct   readRespTwo; 
    DriverReadRespThreeStruct readRespThree;
    
    u32 regValue;
    
}DriverReadRespUnion;


/*------------������ؽṹ��-----------*/
typedef struct
{
    SensorStateEnum energyState; 
    SensorStateEnum sgAllState; 
    SensorStateEnum sgseState; 

    u8  csValue;

    s8  sgUpLimit;
    s8  sgDnLimit;
    
    u16 sgZero;

    u32 * energyValue;
    u32 * sgAllValue; 
    u32 * sgseValue; 
    
}DriverMonitorStruct;

/*------------�Ƕȴ��������ݽṹ��-----------*/
typedef struct 
{
    u8  head;    //��ͷ��ʶ�� �̶�Ϊ0x55
    
    AngleSensorDataTypeEnum dataType;    //���������� ȡֵ:0x51,0x52,0x53
    
    u16 xAxis;       //���������ݺ��¶�
    u16 yAxis;
    u16 zAxis;
    u16 temp;
    
    u8  sum;
    
}AngleSensorFrameStruct;

/*------------��ഫ�������ݽṹ��-----------*/
typedef struct 
{
    u8  SOF1;        //֡ͷ��           �̶�Ϊ0x5A
    u8  SOF2;        //֡ͷ��           �̶�Ϊ0x5A
    u8  dataType;    //��֡�������ͣ��̶�Ϊ0x15
    u8  dataNum;     //��������          �̶�Ϊ0x03
    
    u8  dataH;
    u8  dataL;

    u8  model;    //0x03-һ�����;0x02-�߾��Ȳ���;0x01-���ٲ���;0x00-���������
    
    u8  sum;      //У���
    
}DistanceSensorFrameStruct;

/*------------����ֵ���������ݽṹ��-----------*/
typedef struct 
{
    u8  SOF1;     //֡ͷ���̶�Ϊ0x1A
    u8  SOF2;     //֡ͷ���̶�Ϊ0x1A
    
    u8  state;    //��֡����״̬
    
    u8  abs0;     //��Ȧλ����Ϣ��8λ
    u8  abs1;     //��Ȧλ����Ϣ��8λ
    u8  abs2;     //��Ȧλ����Ϣ��8λ
    
    u8  enid;     //�̶�Ϊ0x11
    
    u8  abm0;     //��Ȧ��Ϣ��8λ
    u8  abm1;     //��Ȧ��Ϣ��8λ
    u8  abm2;     //��Ȧ��Ϣ��8λ

    u8  almc;     //����λ
    
    u8  crc;      //У���
    
}AbsEncoderFrameStruct;



#endif
/*****************************************END OF FILE****************************************/
