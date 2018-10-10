/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  comEnum.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
��������:  2017.01.10;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __COM_ENUM_H
#define __COM_ENUM_H



/******************************************�����ļ�*******************************************/
#include "comDatatype.h"



/*******************************************�궨��********************************************/
#define    U32_VALUE_MAX              4294967296u
#define    PVT_POINT_BUFFER_SIZE      256
#define    SMALL_PVT_BUFFER_SIZE      4
#define    PRESET_PVT_BUFFER_SIZE     32

#define    OUTPUT_DATA_BUFFER_SIZE    1023          //����Buffer�ĳ��ȣ�ֵҪ��2^n - 1������4095(16384���ֽ�)
#define    OUTPUT_EMPTY_BUFFER_SIZE   3             //�����յ���

#define    FPGA_PWM_CLOCK             20000000      //20MHz
#define    DEGREE_TO_RADIAN           0.0174533f    //��ת�������� = 3.1415926/180
#define    RADIAN_TO_DEGREE           57.295780f    //����ת������ = 180/3.1415926

#define    PI_VALUE                   3.1415926

#define    DEVICE_SN_LEN       19    //�ֽ���
#define    ERROR_CODE_LEN      6     
#define    SOFTWARE_VER_LEN    4
#define    FPGA_VER_LEN        4
#define    HARDWARE_VER_LEN    2
#define    BOOT_VER_LEN        2
#define    DRVBOARD_VER_LEN    3

#define    PERIOD_TASK_PERIOD   10      //����������������ʱ��(ms)

#define    NCYCLE_INFINITY                0xFFFFFFFF           //����λ��ѡ������ѭ��ģʽʱ ��λ�����͸��߼��Ĵ���   CP        

#define    APP_IMAGE_OK         0xA5    //�����������Ƿ�����
#define    VERIFY_SUCCESSFUL    0       //����У���ɹ�
#define    VERIFY_FAILED        -1      //����У��ʧ��

#define    ANGLE_CODE_TO_VALUE_FACTOR           0.005493164f    //�Ƕȴ������Ƕ���ֵ��ʵ��ֵ��ת��ϵ��(32768.0 * 180)
#define    VELOCITY_CODE_TO_VALUE_FACTOR        0.061035156f    //�Ƕȴ��������ٶ���ֵ��ʵ��ֵ��ת��ϵ��(32768.0 * 2000)
#define    ACCELERATION_CODE_TO_VALUE_FACTOR    0.004785156f    //�Ƕȴ��������ٶ���ֵ��ʵ��ֵ��ת��ϵ��(32768.0 * 16 * 9.8)

#define    ANALOG_IN_ADC_CH         ADC_Channel_10    //ģ��������Ӧ��ADCͨ��
#define    ADC_CONVERT_Value_MAX    4095
#define    ADC_CONVERT_Value_MIN    0

#define    FRACTION_FRAME_DATA_LEN_MAX    5    //����֡ÿ֡�������͵����ݳ���
#define    FRACTION(num, den)             (((num << 4) & 0xF0) + (den & 0x0F))

#define    EMERG_STOP_END_PERIOD    20000    //20000/20000000(FPGA_PWM_CLOCK)=0.001����Ӧ��ʱ����1ms

#define    GROUP_NUM          2
#define    GROUP_NUM_VALUE_MIN    129
#define    GROUP_NUM_VALUE_MAX    255

#define    UNIT_S_TO_MS    1000

#ifdef PROJECT_QUBELEY
#define    ANGLE_SENSOR_NUM       1
#define    DIST_SENSOR_NUM        1
#else
#define    ANGLE_SENSOR_NUM       4
#define    DIST_SENSOR_NUM        4

#define    ABS_ENCODER_NUM        4
#endif

#define    PVT_CALC_NEED_POINT_NUM    2    //PVT������Ҫ�ĵ���

#define    PVT_CALC_USE_FPGA_CLOCK_ERROR    1    //PVT���������м���ʱ������

#define    PVT_CALC_QUEUE_SIZE    20    //PVT�������г���



/***************************************��������������****************************************/
/*------------������������ö��------------*/
typedef enum
{
    OUTPDATA_FORWARD = 2,    //������������
    OUTPDATA_REVERSE,        //������������
    OUTPDATA_WAIT,           //�ȴ�ʱ��
    
}OutpDataTypeEnum;

/*------------Buffer��������ö��------------*/
typedef enum
{
    BUFFOPERT_WRITE = 0,    //��˳��д��
    BUFFOPERT_FORCESEND,    //ǿ�Ʒ���
    BUFFOPERT_MODIFYEND,    //�޸�ĩβ��
    
}BufferOperateEnum;



/***************************************��������������****************************************/
typedef void (*TimeOutFunc)(void *timeOutPara);
typedef void (*OutpBufferFillFunc)(uint8_t chanNum, 
                                   OutpDataTypeEnum datatype, 
                                   uint32_t outpData, 
                                   BufferOperateEnum bufferOperate,
                                   void *pContext);



/***************************************��������������****************************************/
/*-------------ͨ����ö��-----------*/
typedef enum
{
    CH_MIN = 0,     //ͨ������
    CH_SYS = 0,     //ϵͳͨ��
    CH1    = 0,     //ͨ��1

#ifdef PROJECT_QUBELEY  

    CH_MAX = CH1,   //ͨ������
    
#endif

#ifdef PROJECT_GELGOOG

    CH2,            //ͨ��2
    CH3,            //ͨ��3
    CH4,            //ͨ��4

#if GELGOOG_AXIS_4 

    CH_MAX = CH4,   //ͨ������
    
#elif GELGOOG_AXIS_8

    CH5,            //ͨ��5
    CH6,            //ͨ��6
    CH7,            //ͨ��7
    CH8,            //ͨ��8
    CH_MAX = CH8,   //ͨ������
    
#endif

#endif    //#ifdef PROJECT_GELGOOG

    CH_TOTAL,       //�ܵ�ͨ����
    CH_ALL = 128    //����ͨ��
    
}ChannelEnum;

/*-------------����ģʽö��-------------*/
typedef enum
{
    WORK_NORMAL = 0,    //��ͨģʽ
    WORK_UNIFORM,       //����ģʽ
    WORK_POSITION,      //λ�÷���ģʽ
    WORK_TORQUE         //���ط���ģʽ
    
}WorkModeEnum;

/*-------------�ϵ�״̬ö��-------------*/
typedef enum
{
    POWERON_DEFAULT = 0,    //ʹ��Ĭ��ֵ
    POWERON_LAST,           //ʹ���ϴ�ֵ
    
}PowerOnModeEnum;

/*-------------��Ʒ����ö��-------------*/
typedef enum
{
    MDMODEL_MRQ = 0,   //��������������ϵ��
    MDMODEL_MRV,       //���ŷ�ϵ��
    MDMODEL_MRX,       //������ϵ��
    
}MainDeviceModelEnum;

/*-------------��Ʒ����ö��-------------*/
typedef enum
{
    SDMODEL_C17D = 0,   //���ᣬ03/1.0�棬2660
    SDMODEL_C23D,       //���ᣬ03/1.0�棬262
    
    SDMODEL_M1703,      //3�ᣬ2660
    SDMODEL_M2303,      //3�ᣬ262
    
    SDMODEL_M1704,      //4�ᣬ2660
    SDMODEL_M2304,      //4�ᣬ262
    
    SDMODEL_M1706,      //6�ᣬ2660
    SDMODEL_M2306,      //6�ᣬ262
    
    SDMODEL_M1707,      //7�ᣬ2660
    SDMODEL_M2307,      //7�ᣬ262
    
    SDMODEL_M1708,      //8�ᣬ2660
    SDMODEL_M2308,      //8�ᣬ262
    
    SDMODEL_C17S,       //���ᣬ02/1.1�棬2660
    SDMODEL_C23S,       //���ᣬ02/1.1�棬262
    
    SDMODEL_ERROR,      //�������ͺ�
    
}SubDeviceModelEnum;

/*-------------�����������ͺ�ö��-------------*/
typedef enum
{
    DRIVER_2660 = 0,    // ������Ϊ  2660 
    DRIVER_262  = 1,    //
    DRIVER_UNKNOW
    
}DriverTypeEnum;    

/*-------------����΢��ö��-------------*/
typedef enum
{
    MICROSTEP_256 = 0,
    MICROSTEP_128,
    MICROSTEP_64,
    MICROSTEP_32,
    MICROSTEP_16,
    MICROSTEP_8,
    MICROSTEP_4,
    MICROSTEP_2,
    MICROSTEP_1
    
}MicroStepEnum;

/*-------------����������ö��-------------*/
typedef enum
{
    STEPANGLE_18 = 0,    //1.8��
    STEPANGLE_09,        //0.9��
    STEPANGLE_150,       //15��
    STEPANGLE_75         //7.5��
    
}StepAngleEnum;

/*-----------�����˶�����ö��-----------*/
typedef enum
{
    MOTOR_ROTARY = 0,    //��ת�˶�
    MOTOR_LINEAR         //ֱ���˶�
    
}MotorTypeEnum;

/*-------------�����ߴ�ö��-------------*/
typedef enum
{
    MOTOR_08 = 0,
    MOTOR_11,
    MOTOR_14,
    MOTOR_17, //42
    MOTOR_23, //57
    MOTOR_24
    
}MotorSizeEnum;

/*-------------������ͨ����ö��-------------*/
typedef enum
{
    ECCHAN_1 = 0,   //��ͨ��
    ECCHAN_3        //��ͨ��
    
}EncoderChanEnum;

/*-------------����������ö��-------------*/
typedef enum
{
    ENCODER_INC = 0,     //�����ͱ�����
    ENCODER_ABS          //�����ͱ�����
    
}EncoderTypeEnum;

/*-------------PVTģʽö��-------------*/
typedef enum
{
    EXECMODE_NCYCLE = 0,
    EXECMODE_FIFO,
    
}ExecModeEnum;

/*-------------���з���ö��-------------*/
typedef enum
{
    DIR_FORWARD = OUTPDATA_FORWARD,
    DIR_REVERSE = OUTPDATA_REVERSE
    
}DirectionEnum;

/*-----------PVT�滮ģʽö��-----------*/
typedef enum
{
    PLANMODE_CUBIC = 0,    //���ζ���ʽ��ֵ
    PLANMODE_LINEAR,       //���Բ�ֵ
    PLANMODE_UNIFORM,      //���ٹ滮
    PLANMODE_TRAPEZOID,    //���β�ֵ
    PLANMODE_QUINTIC,      //���ζ���ʽ��ֵ
    PLANMODE_SINE,         //����
    
}PlanModeEnum;

/*-------------�˶��л�ö��-------------*/
typedef enum
{
    MTSWITCH_RESET = 0,
    MTSWITCH_STOP,
    MTSWITCH_RUN,
    MTSWITCH_PREPARE,
    MTSWITCH_EMERGSTOP, //����ֹͣ����   ��ǿ������ֹͣ �������κμ��ٶ���  cp 20170516
    
}MotionSwitchEnum;

/*-------------�˶�״̬ö��-------------*/
/*�˶�״̬������: ��һ����ϵͳ������Ҫ����״̬�Ƶ���˸������
                                  �г�ʼ��״̬(POWERON)������״̬(IDLE)��
                                  ����״̬(CALCEND��CALCEND��STANDBY��RUNNING)�ʹ���״̬(ERROR);

                  �ڶ�����ͨ�������г�ʼ��״̬(POWERON)����ֹ״̬(IDLE��CALCEND��STANDBY)��
                                    ����״̬(CALCING)������״̬(OUTPUTING)�ʹ���״̬(ERROR);
                                    
                  �������ǲ���������δ����״̬(IDLE)��д����״̬(CALCEND)��
                                    д������״̬(CALCEND)��Ԥȡ״̬(STANDBY)��
                                    ����״̬(RUNNING)�ʹ���״̬(ERROR);
*/
typedef enum
{
    MTSTATE_POWERON = 0, //�ϵ��У�              ���ڲ�����˵�����ڴ�״̬
    MTSTATE_IDLE,        //�˶�ϵͳ���ڿ���״̬�����ڲ�����˵��ʾδ����
    MTSTATE_CALCING,     //PVT�����У�           ���ڲ�����˵��ʾд����
    MTSTATE_CALCEND,     //PVT�������ɣ�         ���ڲ�����˵��ʾд������
    MTSTATE_STANDBY,     //�˶�׼�����ɣ�        ���ڲ�����˵��ʾԤȡ����
    MTSTATE_OUTPUTING,   //�����˶���            ���ڲ�����˵��ʾ������
    MTSTATE_ERROR,       //�˶�ϵͳ�������̳��ִ���   ����������Ҫ�Ӵ�������ѯ
    
}MotionStateEnum;

/*-------------ͨ��״̬ö��--------------*/
typedef enum
{
    CHSTATE_POWERON = 0,
    CHSTATE_IDLE,
    CHSTATE_CALCING,
    CHSTATE_OUTPUTING,
    CHSTATE_ERROR     
    
}ChanStateEnum;

/*-------------ϵͳ״̬ö��--------------*/
typedef enum
{
    SYSTATE_POWERON = 0,
    SYSTATE_IDLE,
    SYSTATE_WORKING,
    SYSTATE_ERROR     
    
}SysStateEnum;

/*-----------����ֹͣ����ö��-----------*/
typedef enum
{
    STOPMODE_IMMED = 0,    //ֱ��ֹͣ
    //STOPMODE_HOLD,         //���ֵ�ǰ����
    STOPMODE_DIST,         //������ֹͣ
    STOPMODE_TIME,         //��ʱ��ֹͣ
    STOPMODE_DIST_TIME,    //��������ʱ��ֹͣ
    
}StopModeEnum;

/*-----------����״̬ö��-----------*/
typedef enum
{
    ENDSTATE_STOP = 0,    //ֱ��ֹͣ
    ENDSTATE_HOLD,        //��������һ�����ٶ�
    
}EndStateEnum;

/*-------------λ�õ�λö��-------------*/
typedef enum
{
    POSTUNIT_ANGLE = 0,    //�Ƕ�
    POSTUNIT_RADIAN,       //����
    POSTUNIT_MMETER,       //����
    POSTUNIT_IOSIGNAL      //IO�ź�
    
}PostUnitEnum;

/*-------------΢��ģʽö��-------------*/
typedef enum
{
    INCHIMODE_RELTANGLE = 0,    //�����ԽǶȽ���΢��
    INCHIMODE_RELTRADIAN,       //�����Ի��Ƚ���΢��
    INCHIMODE_RELTMMETER,       //�����Ժ��׽���΢��
    INCHIMODE_ABSANGLE,         //�Ծ��ԽǶȽ���΢��
    INCHIMODE_ABSRADIAN,        //�Ծ��Ի��Ƚ���΢��
    INCHIMODE_ABSMMETER         //�Ծ��Ժ��׽���΢��
    
}InchingModeEnum;

/*-------------�ϱ�����ö��-------------*/
typedef enum
{
    REPTTYPE_TORQUE = 0,    //TMCоƬ��SGֵ
    REPTTYPE_CYCLE,         //ѭ������ѭ�������ϱ��������ú�����Ч�ģ�ѭ�����ĸ���������ֵ�仯��    CJ 2017.06.07 Add

    REPTTYPE_XANGLE,    //�Ƕȴ�����X���Ƕ�      //2
    REPTTYPE_YANGLE,    //�Ƕȴ�����Y���Ƕ�
    REPTTYPE_ZANGLE,    //�Ƕȴ�����Z���Ƕ�
    
    REPTTYPE_XVELOC,    //�Ƕȴ�����X�����ٶ�      //5
    REPTTYPE_YVELOC,    //�Ƕȴ�����Y�����ٶ�
    REPTTYPE_ZVELOC,    //�Ƕȴ�����Z�����ٶ�
    
    REPTTYPE_XACC,      //�Ƕȴ�����X���Ǽ��ٶ�      //8
    REPTTYPE_YACC,      //�Ƕȴ�����Y���Ǽ��ٶ�
    REPTTYPE_ZACC,      //�Ƕȴ�����Z���Ǽ��ٶ�

    REPTTYPE_DRVSGALL,  //SGALL��ȡ
    REPTTYPE_DRVSGSE,   //SGSE��ȡ
    
    REPTTYPE_DISTANCE,  //���ഫ����ֵ
    REPTTYPE_ABSENC,    //����ֵ������
    
    REPTTYPE_OUTNUM,        //ʧ����             //11
    REPTTYPE_STEPS,         //�����Ĳ���
    REPTTYPE_VELOCITY,      //�ٶ�
    REPTTYPE_ANGLESEN,      //�Ƕȴ�����
    REPTTYPE_DRVMONT,       //��������
    
    REPTTYPE_RESERVE
    
}ReportTypeEnum;

/*-------------�ϱ�״̬ö��-------------*///���ϱ���Ҳ������һ�ִ���������SensorStateEnumö��һ��    CJ 2017.05.24 Delete

/*-----------������������ö��-----------*/
typedef enum
{
    TRIGINTYPE_NONE = 0,    //������
    TRIGINTYPE_LOW,
    TRIGINTYPE_RISE,
    TRIGINTYPE_FALL,
    TRIGINTYPE_HIGH,
    
}TrigInTypeEnum;

/*-----------���������ܽ�ö��-----------*/
typedef enum
{
#ifdef PROJECT_QUBELEY

    TRIGPIN_DI2_X1 = 0,
    TRIGPIN_DI3_X2,
    TRIGPIN_DI4_X3,
    TRIGPIN_DI5_X4,
    TRIGPIN_CHANZ,

#else

    TRIGPIN_DIL = 0,
    TRIGPIN_DIR,
    
#endif

    TRIGPIN_RESERVE
    
}TrigInPinEnum;

/*-----------���������ܽ�ö��-----------*/
typedef enum
{
    SMODE_CONTINUED = 0,    //��������
    SMODE_INTERVAL,         //��������
    SMODE_SINGLE            //���β���
    
}SamplingModeEnum;

/*-----------����IO�ź�����ö��-----------*/
typedef enum
{
    DIOSIGNAL_LEVEL = 0,
    DIOSIGNAL_SQUARE,
    DIOSIGNAL_PULSE
    
}DIOSignalEnum;

/*-----------����IO����ö��-----------*/
typedef enum
{
    DIOCONDT_WTSTART = 0,    //����������ʼ
    DIOCONDT_WTEND,          //������������
    DIOCONDT_AUTO            //�Զ���ʹ��֮����������ƽ
    
}DIOCondtEnum;

/*-----------����IO�źż���ö��-----------*/
typedef enum
{
    DIOPOLARITY_N = 0,    //������
    DIOPOLARITY_P
    
}DIOPolarityEnum;

/*-------------������״̬ö��-------------*/
typedef enum
{
    SENSOR_OFF = 0,
    SENSOR_ON
    
}SensorStateEnum;

/*-------------ʱ��ͬ���Ĵ���ö��-----------*/
typedef enum
{
    CLOCKREG_OFF = 0,
    CLOCKREG_ON 
    
}ClockSyncRegEnum;

/*-------------ʱ��ͬ��״̬ö��-----------*/    //CJ 2017.04.11 Add
typedef enum
{
    CLOCKSYNC_END = 0,
    CLOCKSYNC_START 
    
}ClockSyncStateEnum;

/*-------------�Ĵ�������ö��-------------*/    //CJ 2017.04.11 Add
typedef enum
{
    REGISTER_NONE = 0,     //�ر�
    REGISTER_SOFTWARE,     //��������
    REGISTER_DIOSTART,     //����IO����
    REGISTER_CANSTART,     //CAN��������
    REGISTER_SYNCSTART,    //��ʼʱ��ͬ��
    REGISTER_SYNCEND       //����ʱ��ͬ��
    
}RegisterFuncEnum;

/*-------------��������ö��-------------*/    //CJ 2017.04.11 Add
typedef enum
{
    RECEIVE_RADIOID = 0,     //�������ǹ㲥ID
    RECEIVE_RECEIVEID,       //�������ǽ���ID
    RECEIVE_GROUPID1,          //����������ID
    RECEIVE_GROUPID2,          //����������ID
    
}ReceiveTypeEnum;

/*-------------��������Ӧö��-------------*/
typedef enum
{
    RESPONSE_NONE = 0,
    RESPONSE_ALARM,
    RESPONSE_STOP,
    RESPONSE_ALARMSTOP,
    RESPONSE_RUN
    
}ResponseTypeEnum;

/*-----------FPGA��������Դö��(���⼸��ö��,���������߼���ͬһ���Ĵ���,���л�����ϵ,���Է���һ��)---------------*/
typedef enum
{
    MULTI_SYNC_TRIG_SRC_OFF,
    MULTI_SYNC_TRIG_SRC_SW,
    MULTI_SYNC_TRIG_SRC_HW,
    MULTI_SYNC_TRIG_SRC_CAN,
    MULTI_SYNC_RST,
    MULTI_SYNC_START,
    MULTI_SYNC_END,
}MultiSyncFuncEnum;

typedef enum
{
  NMT_CONF_IDEL,
  NMT_C0NF_HASH,
  NMT_CONF_SIGNATURE,
  
}NMTConfigState;

/*-------------����ģʽö��-------------*/
typedef enum
{
    ENGINMODE_NONE = 0,        //�ǹ���ģʽ
    ENGINMODE_DDRTEST,         //DDR����ģʽ
    ENGINMODE_DRIVETEST,       //��������ģʽ
    ENGINMODE_TRIGINTEST,      //������������ģʽ
    ENGINMODE_TRIGOUTTEST,     //������������ģʽ
    ENGINMODE_ENCODERTEST,     //����������ģʽ
    ENGINMODE_ISOLINTEST,      //������������ģʽ
    ENGINMODE_ISOLOUTTEST,     //������������ģʽ
    ENGINMODE_ANALOGINTEST,    //ģ������ģʽ
    ENGINMODE_SUARTTEST,       //������UART����ģʽ
    
}EnginModeEnum;

/*-------------��ƽ״̬ö��-------------*/
typedef enum
{
    LEVEL_LOW = 0,      //�͵�ƽ
    LEVEL_HIGH,         //�ߵ�ƽ
    LEVEL_ERROR = 5,    //������ƽ
    
}LevelStatusEnum;

/*-----------����IO����ö��-------------*/
typedef enum
{
    DIGITALIO_DI1 = 0,
    DIGITALIO_DI2,
    DIGITALIO_XI1,
    
    DIGITALIO_SEN1RX = 0,
    DIGITALIO_SEN2RX,
    DIGITALIO_SEN3RX,
    DIGITALIO_SEN4RX,

    DIGITALIO_DI3 = 3,
    DIGITALIO_DI4,
    DIGITALIO_DI5,
    DIGITALIO_DI6,
    DIGITALIO_DI7,

    DIGITALIO_XI2 = 3,
    DIGITALIO_XI3,
    DIGITALIO_XI4,
    DIGITALIO_AI1,



    DIGITALIO_DO1 = 8,
    DIGITALIO_DO2,

    DIGITALIO_XIN = 9,
    
    DIGITALIO_YO1,
    DIGITALIO_YO2,
    
}DigitalIONumEnum;

/*-----------���������˿ڱ���ö��-------------*/
typedef enum
{
    DIO_DO1 = 0,
    DIO_DO2,

#ifdef PROJECT_GELGOOG
    DIO_DO3,
    DIO_DO4,
#endif

    DIO_RESERVE
    
}DoutNumEnum;

typedef enum
{
    YOUT_YO1 = 0,
    YOUT_YO2,
    YOUT_RESERVE
    
}YoutNumEnum;

/*---------------��������ö��-----------------*/
typedef enum
{
    WTTYPE_MAIN = 0,
    WTTYPE_SMALL,
    WTTYPE_PRESET1,
    WTTYPE_PRESET2,
    WTTYPE_PRESET3,
    WTTYPE_PRESET4,
    WTTYPE_PRESET5,
    WTTYPE_PRESET6,
    WTTYPE_PRESET7,
    WTTYPE_PRESET8,
    WTTYPE_RESERVE
    
}WaveTableTypeEnum;

/*-------------����ʹ�÷�ʽö��--------------*/
typedef enum
{
    WTWORKMODE_FIFO  = 0,    //������FIFOģʽʹ��
    WTWORKMODE_CYCLE,        //������ѭ��ģʽʹ��
    
}WaveWorkModeEnum;

/*-------------�˶���ʽö��--------------*/
typedef enum
{
    MTNMODE_PVT  = 0,     //PVTģʽ
    MTNMODE_LVT_CORR,     //LVT������ģʽ
    MTNMODE_LVT_NOCORR,   //LVT��������ģʽ
    
}MotionModeEnum;

/*-------------Preset�û�ö��---------------*/
typedef enum
{
    PRESET_USER1 = 0,
    PRESET_USER2,
    PRESET_USER3,
    PRESET_USER4,
    PRESET_USER5,
    PRESET_USER6,
    PRESET_USER7,
    PRESET_USER8,
    PRESET_RESERVE
    
}PresetUserEnum;

/*-------------�¼�Դö��---------------*/
typedef enum
{
    EVENTSRC_ENCCHANZ = 0,    //������Zͨ��
    EVENTSRC_FPGA,            //FPGA�ж�
    EVENTSRC_STATSWIT,        //״̬�л�
    EVENTSRC_ANALOGIN         //ģ������
    
}EventSourceEnum;

/*-------------TRIGGER��ʽö��--------------*/
typedef enum
{
    TRIGMODE_PATTERN = 0,    //������ʽΪ���ʹ���
    TRIGMODE_LEVEL,          //��ƽ����
    
}TriggerModeEnum;

/*-----------״̬�ϱ���ʽö��-----------*/
typedef enum
{
    STATEQRY_ACTIVE = 0,    //�����ϱ�
    STATEQRY_QUERY          //��ѯ�ϱ�
    
}StateReportEnum;

/*-----------LED��˸��ʽö��-----------*/
typedef enum
{
    LED_POWERON = 0,    //�ϵ���
    LED_IDEL,           //����
    LED_WORKING,        //�����˶�
    LED_ERROR,          //�˶�ϵͳ�������̳��ִ���
    LED_ERROR1,         //�˶�ϵͳ�������̳��ִ���
    LED_DEVICEID,       //�豸ʶ��
    
}LedStatusEnum;

/*-------------ʶ��״̬ö��-----------*/    //CJ 2017.08.11 Add
typedef enum
{
    DISTING_OFF = 0,
    DISTING_ON 
    
}DistingStateEnum;

/*-------------����ռ��ö��-----------*/    //CJ 2017.08.11 Add
typedef enum
{
    MDUTY_1000 = 0,    //1
    MDUTY_500,         //0.5
    MDUTY_250,         //0.25
    MDUTY_125,         //0.125
    
}ModifyDutyEnum;

/*-------------����״̬ö��-----------*/    //CJ 2017.08.11 Add
typedef enum
{
    CONFIG_END = 0,
    CONFIG_CLEAR
    
}ConfigStateEnum;

/*-------------�ӿ�״̬ö��-------------*/
typedef enum
{
    INTFC_NONE = 0,
    INTFC_OFF,
    INTFC_ON
    
}IntfcStateEnum;

/*-------------������Uart�ӿ�����ö��-------------*///ָ����ARM�ṩ���������õĽӿ�����
typedef enum
{
    UARTNUM_U1 = 0,
    UARTNUM_U2,
    UARTNUM_RESERVE
    
}SenUartNumEnum;

/*-------------Sensor����ö��-------------*/
typedef enum
{
    SENSOR_S1 = 0,

#ifdef PROJECT_GELGOOG
    SENSOR_S2,
    SENSOR_S3,
    SENSOR_S4,
#endif

    SENSOR_RESERVE
    
}SensorNumEnum;

typedef enum
{
    SSRC_SOFT = 0,
    SSRC_HARD,
    SSRC_CAN,
    SSRC_ALL
    
}StartSourceEnum;    //��ʼ���������Ĵ���Դ

typedef enum
{
    UPDATE_TYPE_BOOT,
    UPDATE_TYPE_APP,
    UPDATE_TYPE_FPGA,
    
}UpdateTypeEnum;

/*-------------Driver�ض��Ĵ���ö��-------------*/
typedef enum
{
    READREG_R0 = 0,
    READREG_R1,
    READREG_R2,
    READREG_RESERVE
    
}DriverReadRespEnum;

/*-------------Driver�Ĵ���ö��-------------*/
typedef enum
{
    DRVREG_DRVCTRL = 0,
    DRVREG_CHOPCONF,
    DRVREG_SMARTEN,
    DRVREG_SGCSCONF,
    DRVREG_DRVCONF,
    DRVREG_RESERVE
    
}DriverRegisterEnum;

/*-------------Driver�Ĵ���ö��-------------*/
typedef enum
{
    LIMIT_UP = 0,
    LIMIT_DOWN,
    LIMIT_RESERVE
    
}LimitEnum;

/*-------------SG����ö��-------------*/
typedef enum
{
    SGPARA_SG0 = 0,
    SGPARA_SGT,
    SGPARA_SEMAX,
    SGPARA_SEMIN,
    SGPARA_RESERVE
    
}SGParaEnum;

/*-------------FPGA�ͺ�ö��-------------*/
typedef enum
{
    FPGATYPE_SMALLM = 0,    //MRQ-Mʹ�õ�СFPGA
    FPGATYPE_BIGONE,
    FPGATYPE_BIGTWO,
    FPGATYPE_SMALLD,        //MRQ-Dʹ�õ�СFPGA
    FPGATYPE_RESERVE
    
}FpgaTypeEnum;

/*-------------�������ͺ�ö��-------------*/
typedef enum
{
    DRVBOARD_CTRL = 0,    //������ʹ��
    DRVBOARD_ARM,         //
    DRVBOARD_ERROR
    
}DriverBoardTypeEnum;

/*-------------�Ƕ���������ö��-------------*/
typedef enum
{
    MPUFRAME_ACC   = 0x51,
    MPUFRAME_VEL   = 0x52,
    MPUFRAME_ANGLE = 0x53
    
}AngleSensorDataTypeEnum;

/*-------------PRESET״̬ö��-----------*/    //CJ 2017.08.11 Add
typedef enum
{
    PRESET_OFF = 0,
    PRESET_ON,
    PRESET_CALCABLE
    
}PresetStateEnum;

/*-------------LABELö��-----------*/
typedef enum
{
    MLABEL_SINANJU = 0,
    MLABEL_MEGATRON,
    MLABEL_GOUF,
    MLABEL_RESERVE
    
}MainLabelEnum;

/*-------------LABELö��-----------*/
typedef enum
{
    SLABEL_S1 = 0,
    SLABEL_S2,
    SLABEL_S3,
    SLABEL_S4,
    SLABEL_RESERVE
    
}SubLabelEnum;



#endif
/*****************************************END OF FILE****************************************/
