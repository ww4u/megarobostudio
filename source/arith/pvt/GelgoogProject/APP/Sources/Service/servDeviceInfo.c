/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名: servDeviceInfo.c；
功能描述:     ;
函数列表:     ;
当前版本:     1.0.0;
版本改动:     ;
作     者:  CJ;
完成日期:     2017.10.30;
历史版本:     无;
*********************************************************************************************/
#include <string.h>
#include "project.h"
#include "servDeviceInfo.h"
#include "servSystemPara.h"
#include "servFPGA.h"
#include "bspGpio.h"
#include "bspAdc.h"



/****************************************外部变量声明*****************************************/
extern SystemStateStruct  g_systemState;
extern DriverInfoStruct   g_driverInfo;



/*****************************************局部宏定义******************************************/
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



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/
/*软件版本号，制作新的软件版本时应该修改此处的软件版本号，依次为:
分支版本号 + 大版本号 + 小版本号 + 编译版本号，编译版本号不对外*/  
#if QUBELEY_HARDVER_1

u8 softwareVersion[SOFTWARE_VER_LEN] = {5, 0, 1, 14};    //软件版本号    每次发布新版本时需要修改此变量

#elif QUBELEY_HARDVER_2

u8 softwareVersion[SOFTWARE_VER_LEN] = {2, 0, 1, 13};    //Qubeley第二版软件版本号    每次发布新版本时需要修改此变量

#else

u8 softwareVersion[SOFTWARE_VER_LEN] = {5, 0, 1, 14};    //Gelgoog软件版本号    每次发布新版本时需要修改此变量

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



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servDevicePortModelandVerRead;
实现功能: 读取端口的设备型号和硬件版本号; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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

#if 0    //多轴目前电压值焊接的不对，先屏蔽了
    //model值有MRQ-M和MRQ-D的区分

    //读下板电压(CH1-4)
    versionVolt = bspDrvDnHardVerAverVoltGet(5);    //5次读取求平均值

    //判断的详细规则见《MRQ硬件型号及版本规则》
    if (versionVolt <= DRIVER_HARD_WER_GEAR_H_1)
    {
        if (versionVolt > DRIVER_HARD_WER_GEAR_H_2)    //2400mV到3200mV之间
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_2;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //3100mV到3200mV之间
            {
                driveTypeDn = DRIVER_262;
                
                deviceInfo->driverBoardDn[0] = 4;    //4轴，1.0版
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
            /*else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //3000mV到3100mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //2900mV到3000mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //2800mV到2900mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //2700mV到2800mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //2600mV到2700mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //2500mV到2600mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else    //2400mV到2500mV之间
            {
                //目前就1.0版本，后续再添加
            }*/
        }
        /*else if (versionVolt > DRIVER_HARD_WER_GEAR_H_3)    //1600mV到2400mV之间
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_3;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //2300mV到2400mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else
            {
                driveType = DRIVER_UNKNOW;
                
                deviceInfo->driverBoardDn[0] = 0;
                deviceInfo->driverBoardDn[1] = 0;
                deviceInfo->driverBoardDn[2] = 0;
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //2200mV到2300mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //2100mV到2200mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //2000mV到2100mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //1900mV到2000mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //1800mV到1900mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //1700mV到1800mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else    //1600mV到1700mV之间
            {
                //目前就1.0版本，后续再添加
            }
        }
        else if (versionVolt > DRIVER_HARD_WER_GEAR_H_4)    //800mV到1600mV之间
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_4;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //1500mV到1600mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else
            {
                driveType = DRIVER_UNKNOW;
                
                deviceInfo->driverBoardDn[0] = 0;
                deviceInfo->driverBoardDn[1] = 0;
                deviceInfo->driverBoardDn[2] = 0;
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //1400mV到1500mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //1300mV到1400mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //1200mV到1300mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //1100mV到1200mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //1000mV到1100mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //900mV到1000mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else    //800mV到900mV之间
            {
                //目前就1.0版本，后续再添加
            }
        }
        else    //0mV到800mV之间
        {
            voltDiff = versionVolt;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //700mV到800mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else
            {
                driveType = DRIVER_UNKNOW;
                
                deviceInfo->driverBoardDn[0] = 0;
                deviceInfo->driverBoardDn[1] = 0;
                deviceInfo->driverBoardDn[2] = 0;
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //600mV到700mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //500mV到600mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //400mV到500mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //300mV到400mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //200mV到300mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //100mV到200mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else    //0mV到100mV之间
            {
                //目前就1.0版本，后续再添加
            }
        }*/
    }
    else    //未连接下板
    {
        //报错
        deviceInfo->drvBoardTypeUp = DRVBOARD_ERROR;
        
        deviceInfo->driverBoardDn[0] = 0;
        deviceInfo->driverBoardDn[1] = 0;
        deviceInfo->driverBoardDn[2] = 0;
    }

    //读上板电压(CH5-8)
    versionVolt = bspDrvUpHardVerAverVoltGet(5);    //5次读取求平均值

    //判断的详细规则见《MRQ硬件型号及版本规则》
    if (versionVolt <= DRIVER_HARD_WER_GEAR_H_1)
    {
        if (versionVolt > DRIVER_HARD_WER_GEAR_H_2)    //2400mV到3200mV之间
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_2;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //3100mV到3200mV之间
            {
                driveTypeUp = DRIVER_262;
                
                deviceInfo->driverBoardUp[0] = 4;    //4轴，1.0版
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
            /*else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //3000mV到3100mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //2900mV到3000mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //2800mV到2900mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //2700mV到2800mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //2600mV到2700mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //2500mV到2600mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else    //2400mV到2500mV之间
            {
                //目前就1.0版本，后续再添加
            }*/
        }
        /*else if (versionVolt > DRIVER_HARD_WER_GEAR_H_3)    //1600mV到2400mV之间
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_3;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //2300mV到2400mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //2200mV到2300mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //2100mV到2200mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //2000mV到2100mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //1900mV到2000mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //1800mV到1900mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //1700mV到1800mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else    //1600mV到1700mV之间
            {
                //目前就1.0版本，后续再添加
            }
        }
        else if (versionVolt > DRIVER_HARD_WER_GEAR_H_4)    //800mV到1600mV之间
        {
            voltDiff = versionVolt - DRIVER_HARD_WER_GEAR_H_4;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //1500mV到1600mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //1400mV到1500mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //1300mV到1400mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //1200mV到1300mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //1100mV到1200mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //1000mV到1100mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //900mV到1000mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else    //800mV到900mV之间
            {
                //目前就1.0版本，后续再添加
            }
        }
        else    //0mV到800mV之间
        {
            voltDiff = versionVolt;

            if (voltDiff > DRIVER_HARD_WER_GEAR_L_2)    //700mV到800mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_3)    //600mV到700mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_4)    //500mV到600mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_5)    //400mV到500mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_6)    //300mV到400mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_7)    //200mV到300mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else if (voltDiff > DRIVER_HARD_WER_GEAR_L_8)    //100mV到200mV之间
            {
                //目前就1.0版本，后续再添加
            }
            else    //0mV到100mV之间
            {
                //目前就1.0版本，后续再添加
            }
        }*/
    }
    else    //未连接上板
    {
        //如果是大于4轴的产品则报错
        deviceInfo->drvBoardTypeUp = DRVBOARD_ERROR;
        
        deviceInfo->driverBoardUp[0] = 0;
        deviceInfo->driverBoardUp[1] = 0;
        deviceInfo->driverBoardUp[2] = 0;
    }

    //确定产品型号
    if (deviceInfo->drvBoardTypeDn != DRVBOARD_ERROR)
    {
        if (deviceInfo->drvBoardTypeUp != DRVBOARD_ERROR)
        {
            //8轴
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
            else    //目前不支持上下板驱动类型不一致
            {
                deviceInfo->sDevcModel = SDMODEL_ERROR;
            }
        }
        else
        {
            //4轴
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
    

    //读数字板
    versionVolt = bspDigitalHardVerAverVoltGet(5);    //5次读取求平均值

    //判断的详细规则见《MRQ硬件型号及版本规则》
    if (versionVolt < DIGITAL_HARD_WERSION_2)
    {
        //目前就1.0版本
        deviceInfo->hardware[0] = 1;
        deviceInfo->hardware[1] = 0;
    }
    /*else if (DIGITAL_HARD_WERSION_2 <= versionVolt < DIGITAL_HARD_WERSION_3)
    {
        //目前就1.0版本，后续再添加
    }*/
    else
    {
        //错误版本号
        deviceInfo->hardware[0] = 0;
        deviceInfo->hardware[1] = 0;
    }
    
#else

#if GELGOOG_AXIS_4
    //4轴
    deviceInfo->sDevcModel = SDMODEL_M2304;

    deviceInfo->driverBoardDn[0] = 4;    //4轴，1.0版
    deviceInfo->driverBoardDn[1] = 1;
    deviceInfo->driverBoardDn[2] = 0;

    deviceInfo->hardware[0] = 1;
    deviceInfo->hardware[1] = 0;
    
#elif GELGOOG_AXIS_8

    //8轴
    deviceInfo->sDevcModel = SDMODEL_M2308;

    deviceInfo->driverBoardDn[0] = 8;    //8轴，1.0版
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
    switch (model)    //转换的详细规则见《MRQ硬件型号及版本规则》
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

    versionVolt = bspHardVerAverVoltGet(5);    //5次读取求平均值

    //判断的详细规则见《MRQ硬件型号及版本规则》
    if (versionVolt < DIGITAL_HARD_WERSION_2)
    {
        //目前就1.0版本
        deviceInfo->hardware[0] = 1;
        deviceInfo->hardware[1] = 0;
    }
    /*else if (DIGITAL_HARD_WERSION_2 <= versionVolt < DIGITAL_HARD_WERSION_3)
    {
        //目前就1.0版本，后续再添加
    }*/
    else if (DIGITAL_HARD_WERSION_10 <= versionVolt)
    {
        //目前当成0.3版本使用，后续0.3版不再维护后可以作为别的使用
        deviceInfo->hardware[0] = 0;
        deviceInfo->hardware[1] = 3;        
    }
    else
    {
        //错误版本号
        deviceInfo->hardware[0] = 0;
        deviceInfo->hardware[1] = 0;
    }
    
#endif    //#if QUBELEY_HARDVER_2
#endif    //#ifdef PROJECT_GELGOOG
}


/*********************************************************************************************
函 数 名: servDeviceInfoInit;
实现功能: 型号，序列号，硬件版本号等硬件信息初始化; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void servDeviceInfoInit(DeviceInfoStruct *deviceInfo, u8 *bootVer)
{
    DeviceInfoStorageStruct storageInfo;

    
    //型号和序列号首先在Flash或者EEPROM中保存一份
    //型号还可以通过硬件端口的电平进行识别
    //上电后对两个型号(从序列号中也可以解析出型号)进行对比，不一致则报错，无法进行工作

    if (deviceInfo != NULL)
    {
        //存储的型号和SN
        if (servDeviceInfoRead(&storageInfo)!= VERIFY_SUCCESSFUL)    //EEPROM中的数据错误
        {
            //读FLASH中的数据
            if (0)
            {
                //TO ADD NICK
            }
            else    //使用默认值
            {
                deviceInfo->storageInfo.sDevcModel = devcModel;
                memcpy(deviceInfo->storageInfo.deviceSn, deviceSn, DEVICE_SN_LEN);
            }
        }
        else
        {
            deviceInfo->storageInfo = storageInfo;
        }
        
        //硬件端口设置的型号和硬件版本号
        deviceInfo->mDevcModel = MDMODEL_MRQ;
        servDevicePortModelandVerRead(deviceInfo);
        
        //逻辑版本号
        deviceInfo->fpga.verStruct = servFpgaVersionGet();

        //软件版本号
        memcpy(deviceInfo->software, softwareVersion, SOFTWARE_VER_LEN);

        //BOOT版本号
        memcpy(deviceInfo->boot, bootVer, BOOT_VER_LEN);
    }
}


#ifdef PROJECT_GELGOOG
/*********************************************************************************************
函 数 名: servFpgaTypeRead;
实现功能: 读取FPGA的型号; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
FpgaTypeEnum servFpgaTypeRead(void)
{
    u8 model;
    FpgaTypeEnum fpgaType;


    bspModelGpioInit();

    model = bspModelGpioRead();
    
    switch (model)    //转换的详细规则见《MRQ硬件型号及版本规则》
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
函 数 名: servSystemInfoInit;
实现功能: 型号，序列号，硬件版本号等硬件信息初始化; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void servSystemInfoInit(SubDeviceModelEnum sDevcModel)
{
    DriverTypeEnum type;
    u8 i;

    
    //根据型号设定通道号，各个接口数量g_deviceInfo.sDevcModel
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
            //报错
          break;
    }

    for (i = 0;i < g_systemState.chanNum;i++)
    {
        g_driverInfo.driver[i].type = type;
    }
}



/*******************************************文件尾********************************************/
