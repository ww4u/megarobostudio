/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��: servDeviceInfo.c��
��������:     ;
�����б�:     ;
��ǰ�汾:     1.0.0;
�汾�Ķ�:     ;
��     ��:  CJ;
�������:     2017.10.30;
��ʷ�汾:     ��;
*********************************************************************************************/
#include <string.h>
#include "project.h"
#include "servDeviceInfo.h"
#include "servSystemPara.h"
#include "servFPGA.h"
#include "bspGpio.h"
#include "bspAdc.h"



/****************************************�ⲿ��������*****************************************/
extern SystemStateStruct  g_systemState;
extern DriverInfoStruct   g_driverInfo;



/*****************************************�ֲ��궨��******************************************/
#define    DIGITAL_HARD_WERSION_2      330     //mV
#define    DIGITAL_HARD_WERSION_3      660     //mV
#define    DIGITAL_HARD_WERSION_4      990     //mV
#define    DIGITAL_HARD_WERSION_5      1320    //mV
#define    DIGITAL_HARD_WERSION_6      1650    //mV
#define    DIGITAL_HARD_WERSION_7      1980    //mV
#define    DIGITAL_HARD_WERSION_8      2310    //mV
#define    DIGITAL_HARD_WERSION_9      2640    //mV
#define    DIGITAL_HARD_WERSION_10     2970    //mV

#define    DRIVER_HARD_WER_GEAR_H_1    3200    //mV
#define    DRIVER_HARD_WER_GEAR_H_2    2400    //mV
#define    DRIVER_HARD_WER_GEAR_H_3    1600    //mV
#define    DRIVER_HARD_WER_GEAR_H_4     800    //mV

#define    DRIVER_HARD_WER_GEAR_L_2    700     //mV
#define    DRIVER_HARD_WER_GEAR_L_3    600     //mV
#define    DRIVER_HARD_WER_GEAR_L_4    500     //mV
#define    DRIVER_HARD_WER_GEAR_L_5    400     //mV
#define    DRIVER_HARD_WER_GEAR_L_6    300     //mV
#define    DRIVER_HARD_WER_GEAR_L_7    200     //mV
#define    DRIVER_HARD_WER_GEAR_L_8    100     //mV



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
/*����汾�ţ������µ�����汾ʱӦ���޸Ĵ˴�������汾�ţ�����Ϊ:
��֧�汾�� + ��汾�� + С�汾�� + ����汾�ţ�����汾�Ų�����*/  
#if QUBELEY_HARDVER_1

u8 softwareVersion[SOFTWARE_VER_LEN] = {5, 0, 1, 14};    //����汾��    ÿ�η����°汾ʱ��Ҫ�޸Ĵ˱���

#elif QUBELEY_HARDVER_2

u8 softwareVersion[SOFTWARE_VER_LEN] = {2, 0, 1, 13};    //Qubeley�ڶ�������汾��    ÿ�η����°汾ʱ��Ҫ�޸Ĵ˱���

#else

u8 softwareVersion[SOFTWARE_VER_LEN] = {5, 0, 1, 14};    //Gelgoog����汾��    ÿ�η����°汾ʱ��Ҫ�޸Ĵ˱���

#endif

#ifdef PROJECT_GELGOOG

#if GELGOOG_AXIS_4
SubDeviceModelEnum devcModel = SDMODEL_M2304;
char deviceSn[DEVICE_SN_LEN] = "MRQM10040018220001";
#elif GELGOOG_AXIS_8
SubDeviceModelEnum devcModel = SDMODEL_M2308;
char deviceSn[DEVICE_SN_LEN] = "MRQM10080018220001";
#endif

#else

SubDeviceModelEnum devcModel = SDMODEL_C23D;
char deviceSn[DEVICE_SN_LEN] = "MRQC01010000000000";

#endif



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: servDevicePortModelandVerRead;
ʵ�ֹ���: ��ȡ�˿ڵ��豸�ͺź�Ӳ���汾��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servDevicePortModelandVerRead(DeviceInfoStruct *deviceInfo)
{
    u16 versionVolt;
    
#ifdef PROJECT_GELGOOG
    u16 voltDiff;
    DriverTypeEnum driveTypeDn;
    DriverTypeEnum driveTypeUp;
    
#else

    u8  model;
    DriverTypeEnum driveType;
#endif


#ifdef PROJECT_GELGOOG 

#if 0    //����Ŀǰ��ѹֵ���ӵĲ��ԣ���������
    //modelֵ��MRQ-M��MRQ-D������

    //���°��ѹ(CH1-4)
    versionVolt = bspDrvDnHardVerAverVoltGet(5);    //5�ζ�ȡ��ƽ��ֵ

    //�жϵ���ϸ�������MRQӲ���ͺż��汾����
    if (versionVolt <= DRIVER_HARD_WER_GEAR_H_1)
    {
        if (versionVolt > DRIVER_HARD_WER_GEAR_H_2)    //2400mV��3200mV֮��
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_2;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //3100mV��3200mV֮��
            {
                driveTypeDn = DRIVER_262;
                
                deviceInfo->driverBoardDn[0] = 4;    //4�ᣬ1.0��
                deviceInfo->driverBoardDn[1] = 1;
                deviceInfo->driverBoardDn[2] = 0;
            }
            else
            {
                driveTypeDn = DRIVER_UNKNOW;
                
                deviceInfo->driverBoardDn[0] = 0;
                deviceInfo->driverBoardDn[1] = 0;
                deviceInfo->driverBoardDn[2] = 0;
            }
            /*else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //3000mV��3100mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //2900mV��3000mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //2800mV��2900mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //2700mV��2800mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //2600mV��2700mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //2500mV��2600mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else    //2400mV��2500mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }*/
        }
        /*else if (versionVolt > DRIVER_HARD_WER_GEAR_H_3)    //1600mV��2400mV֮��
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_3;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //2300mV��2400mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else
            {
                driveType = DRIVER_UNKNOW;
                
                deviceInfo->driverBoardDn[0] = 0;
                deviceInfo->driverBoardDn[1] = 0;
                deviceInfo->driverBoardDn[2] = 0;
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //2200mV��2300mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //2100mV��2200mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //2000mV��2100mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //1900mV��2000mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //1800mV��1900mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //1700mV��1800mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else    //1600mV��1700mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
        }
        else if (versionVolt > DRIVER_HARD_WER_GEAR_H_4)    //800mV��1600mV֮��
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_4;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //1500mV��1600mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else
            {
                driveType = DRIVER_UNKNOW;
                
                deviceInfo->driverBoardDn[0] = 0;
                deviceInfo->driverBoardDn[1] = 0;
                deviceInfo->driverBoardDn[2] = 0;
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //1400mV��1500mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //1300mV��1400mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //1200mV��1300mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //1100mV��1200mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //1000mV��1100mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //900mV��1000mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else    //800mV��900mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
        }
        else    //0mV��800mV֮��
        {
            voltDiff = versionVolt;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //700mV��800mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else
            {
                driveType = DRIVER_UNKNOW;
                
                deviceInfo->driverBoardDn[0] = 0;
                deviceInfo->driverBoardDn[1] = 0;
                deviceInfo->driverBoardDn[2] = 0;
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //600mV��700mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //500mV��600mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //400mV��500mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //300mV��400mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //200mV��300mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //100mV��200mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else    //0mV��100mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
        }*/
    }
    else    //δ�����°�
    {
        //����
        deviceInfo->drvBoardTypeUp = DRVBOARD_ERROR;
        
        deviceInfo->driverBoardDn[0] = 0;
        deviceInfo->driverBoardDn[1] = 0;
        deviceInfo->driverBoardDn[2] = 0;
    }

    //���ϰ��ѹ(CH5-8)
    versionVolt = bspDrvUpHardVerAverVoltGet(5);    //5�ζ�ȡ��ƽ��ֵ

    //�жϵ���ϸ�������MRQӲ���ͺż��汾����
    if (versionVolt <= DRIVER_HARD_WER_GEAR_H_1)
    {
        if (versionVolt > DRIVER_HARD_WER_GEAR_H_2)    //2400mV��3200mV֮��
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_2;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //3100mV��3200mV֮��
            {
                driveTypeUp = DRIVER_262;
                
                deviceInfo->driverBoardUp[0] = 4;    //4�ᣬ1.0��
                deviceInfo->driverBoardUp[1] = 1;
                deviceInfo->driverBoardUp[2] = 0;
            }
            else
            {
                driveTypeUp = DRIVER_UNKNOW;
                
                deviceInfo->driverBoardUp[0] = 0;
                deviceInfo->driverBoardUp[1] = 0;
                deviceInfo->driverBoardUp[2] = 0;
            }
            /*else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //3000mV��3100mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //2900mV��3000mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //2800mV��2900mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //2700mV��2800mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //2600mV��2700mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //2500mV��2600mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else    //2400mV��2500mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }*/
        }
        /*else if (versionVolt > DRIVER_HARD_WER_GEAR_H_3)    //1600mV��2400mV֮��
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_3;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //2300mV��2400mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //2200mV��2300mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //2100mV��2200mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //2000mV��2100mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //1900mV��2000mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //1800mV��1900mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //1700mV��1800mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else    //1600mV��1700mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
        }
        else if (versionVolt > DRIVER_HARD_WER_GEAR_H_4)    //800mV��1600mV֮��
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_4;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //1500mV��1600mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //1400mV��1500mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //1300mV��1400mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //1200mV��1300mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //1100mV��1200mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //1000mV��1100mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //900mV��1000mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else    //800mV��900mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
        }
        else    //0mV��800mV֮��
        {
            voltDiff = versionVolt;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //700mV��800mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //600mV��700mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //500mV��600mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //400mV��500mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //300mV��400mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //200mV��300mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //100mV��200mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
            else    //0mV��100mV֮��
            {
                //Ŀǰ��1.0�汾�����������
            }
        }*/
    }
    else    //δ�����ϰ�
    {
        //����Ǵ���4��Ĳ�Ʒ�򱨴�
        deviceInfo->drvBoardTypeUp = DRVBOARD_ERROR;
        
        deviceInfo->driverBoardUp[0] = 0;
        deviceInfo->driverBoardUp[1] = 0;
        deviceInfo->driverBoardUp[2] = 0;
    }

    //ȷ����Ʒ�ͺ�
    if (deviceInfo->drvBoardTypeDn != DRVBOARD_ERROR)
    {
        if (deviceInfo->drvBoardTypeUp != DRVBOARD_ERROR)
        {
            //8��
            if (driveTypeUp == driveTypeDn)
            {
                if (DRIVER_262 == driveTypeDn)
                {
                    deviceInfo->sDevcModel = SDMODEL_M2308;
                }
                else
                {
                    deviceInfo->sDevcModel = SDMODEL_M1708;
                }
            }
            else    //Ŀǰ��֧�����°��������Ͳ�һ��
            {
                deviceInfo->sDevcModel = SDMODEL_ERROR;
            }
        }
        else
        {
            //4��
            if (DRIVER_262 == driveTypeDn)
            {
                deviceInfo->sDevcModel = SDMODEL_M2304;
            }
            else
            {
                deviceInfo->sDevcModel = SDMODEL_M1704;
            }
        }
    }
    else
    {
        deviceInfo->sDevcModel = SDMODEL_ERROR;
    }
    

    //�����ְ�
    versionVolt = bspDigitalHardVerAverVoltGet(5);    //5�ζ�ȡ��ƽ��ֵ

    //�жϵ���ϸ�������MRQӲ���ͺż��汾����
    if (versionVolt < DIGITAL_HARD_WERSION_2)
    {
        //Ŀǰ��1.0�汾
        deviceInfo->hardware[0] = 1;
        deviceInfo->hardware[1] = 0;
    }
    /*else if (DIGITAL_HARD_WERSION_2 <= versionVolt < DIGITAL_HARD_WERSION_3)
    {
        //Ŀǰ��1.0�汾�����������
    }*/
    else
    {
        //����汾��
        deviceInfo->hardware[0] = 0;
        deviceInfo->hardware[1] = 0;
    }
    
#else

#if GELGOOG_AXIS_4
    //4��
    deviceInfo->sDevcModel = SDMODEL_M2304;

    deviceInfo->driverBoardDn[0] = 4;    //4�ᣬ1.0��
    deviceInfo->driverBoardDn[1] = 1;
    deviceInfo->driverBoardDn[2] = 0;

    deviceInfo->hardware[0] = 1;
    deviceInfo->hardware[1] = 0;
    
#elif GELGOOG_AXIS_8

    //8��
    deviceInfo->sDevcModel = SDMODEL_M2308;

    deviceInfo->driverBoardDn[0] = 8;    //8�ᣬ1.0��
    deviceInfo->driverBoardDn[1] = 1;
    deviceInfo->driverBoardDn[2] = 0;
    
    deviceInfo->driverBoardUp[0] = 8;
    deviceInfo->driverBoardUp[1] = 1;
    deviceInfo->driverBoardUp[2] = 0;

    deviceInfo->hardware[0] = 1;
    deviceInfo->hardware[1] = 0;
#endif

#endif

#else    //PROJECT_QUBELEY


#if QUBELEY_HARDVER_2

#if DRIVE_BOARD_TYPE_CONFIG_262
    deviceInfo->sDevcModel = SDMODEL_C23S;
#else
    deviceInfo->sDevcModel = SDMODEL_C17S;
#endif

#else

    bspModelGpioInit();

    model = bspModelGpioRead();
    
    driveType = servFpgaDrvBoardTypeGet(CH1);
    switch (model)    //ת������ϸ�������MRQӲ���ͺż��汾����
    {
        //MRQ-C-D
        case 0:
            if (DRIVER_2660 == driveType)
            {
                deviceInfo->sDevcModel = SDMODEL_C17D;
            }
            else if (DRIVER_262 == driveType)
            {
                deviceInfo->sDevcModel = SDMODEL_C23D;
            }
            else
            {
                deviceInfo->sDevcModel = SDMODEL_ERROR;
            }
          break;

        //MRQ-C-S
        case 1:
            if (DRIVER_2660 == driveType)
            {
                deviceInfo->sDevcModel = SDMODEL_C17S;
            }
            else if (DRIVER_262 == driveType)
            {
                deviceInfo->sDevcModel = SDMODEL_C23S;
            }
            else
            {
                deviceInfo->sDevcModel = SDMODEL_ERROR;
            }
          break;

        default:
          break;
    }

    versionVolt = bspHardVerAverVoltGet(5);    //5�ζ�ȡ��ƽ��ֵ

    //�жϵ���ϸ�������MRQӲ���ͺż��汾����
    if (versionVolt < DIGITAL_HARD_WERSION_2)
    {
        //Ŀǰ��1.0�汾
        deviceInfo->hardware[0] = 1;
        deviceInfo->hardware[1] = 0;
    }
    /*else if (DIGITAL_HARD_WERSION_2 <= versionVolt < DIGITAL_HARD_WERSION_3)
    {
        //Ŀǰ��1.0�汾�����������
    }*/
    else if (DIGITAL_HARD_WERSION_10 <= versionVolt)
    {
        //Ŀǰ����0.3�汾ʹ�ã�����0.3�治��ά���������Ϊ���ʹ��
        deviceInfo->hardware[0] = 0;
        deviceInfo->hardware[1] = 3;        
    }
    else
    {
        //����汾��
        deviceInfo->hardware[0] = 0;
        deviceInfo->hardware[1] = 0;
    }
    
#endif    //#if QUBELEY_HARDVER_2
#endif    //#ifdef PROJECT_GELGOOG
}


/*********************************************************************************************
�� �� ��: servDeviceInfoInit;
ʵ�ֹ���: �ͺţ����кţ�Ӳ���汾�ŵ�Ӳ����Ϣ��ʼ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void servDeviceInfoInit(DeviceInfoStruct *deviceInfo, u8 *bootVer)
{
    DeviceInfoStorageStruct storageInfo;

    
    //�ͺź����к�������Flash����EEPROM�б���һ��
    //�ͺŻ�����ͨ��Ӳ���˿ڵĵ�ƽ����ʶ��
    //�ϵ��������ͺ�(�����к���Ҳ���Խ������ͺ�)���жԱȣ���һ���򱨴��޷����й���

    if (deviceInfo != NULL)
    {
        //�洢���ͺź�SN
        if (servDeviceInfoRead(&storageInfo)!= VERIFY_SUCCESSFUL)    //EEPROM�е����ݴ���
        {
            //��FLASH�е�����
            if (0)
            {
                //TO ADD NICK
            }
            else    //ʹ��Ĭ��ֵ
            {
                deviceInfo->storageInfo.sDevcModel = devcModel;
                memcpy(deviceInfo->storageInfo.deviceSn, deviceSn, DEVICE_SN_LEN);
            }
        }
        else
        {
            deviceInfo->storageInfo = storageInfo;
        }
        
        //Ӳ���˿����õ��ͺź�Ӳ���汾��
        deviceInfo->mDevcModel = MDMODEL_MRQ;
        servDevicePortModelandVerRead(deviceInfo);
        
        //�߼��汾��
        deviceInfo->fpga.verStruct = servFpgaVersionGet();

        //����汾��
        memcpy(deviceInfo->software, softwareVersion, SOFTWARE_VER_LEN);

        //BOOT�汾��
        memcpy(deviceInfo->boot, bootVer, BOOT_VER_LEN);
    }
}


#ifdef PROJECT_GELGOOG
/*********************************************************************************************
�� �� ��: servFpgaTypeRead;
ʵ�ֹ���: ��ȡFPGA���ͺ�; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
FpgaTypeEnum servFpgaTypeRead(void)
{
    u8 model;
    FpgaTypeEnum fpgaType;


    bspModelGpioInit();

    model = bspModelGpioRead();
    
    switch (model)    //ת������ϸ�������MRQӲ���ͺż��汾����
    {
        case 0:
            fpgaType = FPGATYPE_SMALLM;
          break;

        case 1:
            fpgaType = FPGATYPE_BIGONE;
          break;

        case 2:
            fpgaType = FPGATYPE_BIGTWO;
          break;

        case 3:
            fpgaType = FPGATYPE_SMALLD;
          break;

        default:
          break;
    }

    return fpgaType;
}
#endif


/*********************************************************************************************
�� �� ��: servSystemInfoInit;
ʵ�ֹ���: �ͺţ����кţ�Ӳ���汾�ŵ�Ӳ����Ϣ��ʼ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void servSystemInfoInit(SubDeviceModelEnum sDevcModel)
{
    DriverTypeEnum type;
    u8 i;

    
    //�����ͺ��趨ͨ���ţ������ӿ�����g_deviceInfo.sDevcModel
    switch (sDevcModel)
    {
        case SDMODEL_C17D:
        case SDMODEL_C17S:
            type = DRIVER_2660;
            g_systemState.chanNum = 1;
          break;
            
        case SDMODEL_C23D:
        case SDMODEL_C23S:
            type = DRIVER_262;
            g_systemState.chanNum = 1;
          break;

        case SDMODEL_M1703:
            type = DRIVER_2660;
            g_systemState.chanNum = 3;
          break;
          
        case SDMODEL_M2303:
            type = DRIVER_262;
            g_systemState.chanNum = 3;
          break;

        case SDMODEL_M1704:
            type = DRIVER_2660;
            g_systemState.chanNum = 4;
          break;
          
        case SDMODEL_M2304:
            type = DRIVER_262;
            g_systemState.chanNum = 4;
          break;

        case SDMODEL_M1706:
            type = DRIVER_2660;
            g_systemState.chanNum = 6;
          break;
          
        case SDMODEL_M2306:
            type = DRIVER_262;
            g_systemState.chanNum = 6;
          break;

        case SDMODEL_M1707:
            type = DRIVER_2660;
            g_systemState.chanNum = 7;
          break;
          
        case SDMODEL_M2307:
            type = DRIVER_262;
            g_systemState.chanNum = 7;
          break;

        case SDMODEL_M1708:
            type = DRIVER_2660;
            g_systemState.chanNum = 8;
          break;
          
        case SDMODEL_M2308:
            type = DRIVER_262;
            g_systemState.chanNum = 8;
          break;

        case SDMODEL_ERROR:
        default:
            //����
          break;
    }

    for (i = 0;i < g_systemState.chanNum;i++)
    {
        g_driverInfo.driver[i].type = type;
    }
}



/*******************************************�ļ�β********************************************/
