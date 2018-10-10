/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  servSysPara.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.08.15;
历史版本:  无;
*********************************************************************************************/
#include "project.h"
#include "servSystemPara.h"
#include "comMemoryMap.h"
#include "bspCrc32.h"
#include "bspEeprom.h"



/****************************************外部变量声明*****************************************/



/*****************************************局部宏定义******************************************/



/*************************************局部常量和类型定义**************************************/



/******************************************局部变量*******************************************/



/******************************************函数实现*******************************************/
/*********************************************************************************************
函 数 名: servBootInfoWrite
实现功能: 向EEPROM写入boot_info信息
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
void servBootInfoWrite(BootInfoStruct * info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info,sizeof(BootInfoStruct) - sizeof(info->crc));
    
    if (info->crc != crc)
    {
        info->crc = crc;
        
        bspAT24CxxWrite(EEPROM_BOOT_INFO_ADDR,(u8*)info,sizeof(BootInfoStruct));
    }
}


/*********************************************************************************************
函 数 名: servBootInfoRead
实现功能: 从EEPROM读取boot_info信息
输入参数: 
输出参数: 无;
返 回 值:
说    明: 
*********************************************************************************************/
int servBootInfoRead(BootInfoStruct * info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_BOOT_INFO_ADDR,(u8*)info,sizeof(BootInfoStruct));
    crc = bspCRC32((u8*)info,sizeof(BootInfoStruct) - sizeof(info->crc));
    
    return (crc == info->crc)?VERIFY_SUCCESSFUL:VERIFY_FAILED;    //CJ 2017.05.19 Modify
}


/*********************************************************************************************
函 数 名: servInterfaceInfoWrite
实现功能: 向EEPROM写入IntfaceInfo信息
输入参数: 
输出参数: 无;
返 回 值:
说    明: CJ 2017.05.19 Modify
*********************************************************************************************/
void servInterfaceInfoWrite(CommIntfcStruct * info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, sizeof(CommIntfcStruct) - sizeof(info->crc));
    
    if (info->crc != crc)
    { 
        info->crc = crc;

        bspAT24CxxWrite(EEPROM_INTEFACE_INFO_ADDR, (u8*)info, sizeof(CommIntfcStruct));
    }
}


/*********************************************************************************************
函 数 名: servInterfaceInfoRead
实现功能: 从EEPROM读取IntfaceInfo信息
输入参数: 
输出参数: 无;
返 回 值:
说    明: CJ 2017.05.19 Modify
*********************************************************************************************/
int servInterfaceInfoRead(CommIntfcStruct * info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_INTEFACE_INFO_ADDR, (u8*)info, sizeof(CommIntfcStruct));
    crc = bspCRC32((u8*)info, sizeof(CommIntfcStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servSystemInfoWrite
实现功能: 向EEPROM写入系统信息
输入参数: 
输出参数: 无;
返 回 值:
说    明: CJ 2017.05.19 Modify
*********************************************************************************************/
void servSystemInfoWrite(SystemInfoStruct * info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info,sizeof(SystemInfoStruct) - sizeof(info->crc));
    
    if (info->crc != crc)
    { 
        info->crc = crc;
    
        bspAT24CxxWrite(EEPROM_SYS_INFO_ADDR, (u8*)info, sizeof(SystemInfoStruct));
    }
}


/*********************************************************************************************
函 数 名: servSystemInfoRead
实现功能: 从EEPROM读取系统信息
输入参数: 
输出参数: 无;
返 回 值:
说    明: CJ 2017.05.19 Modify
*********************************************************************************************/
int servSystemInfoRead(SystemInfoStruct * info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_SYS_INFO_ADDR, (u8*)info, sizeof(SystemInfoStruct));
    crc = bspCRC32((u8*)info, sizeof(SystemInfoStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}



/*********************************************************************************************
函 数 名: servMotorInfoWrite
实现功能: 向EEPROM的地址上写入电机信息
输入参数: 
输出参数: 无;
返 回 值:
说    明: CJ 2017.05.19 Modify
*********************************************************************************************/
void servMotorInfoWrite(MotorInfoStruct * info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info,sizeof(MotorInfoStruct) - sizeof(info->crc));
    
    if (info->crc != crc)
    {
        info->crc = crc;
    
        bspAT24CxxWrite(EEPROM_MOTOR_INFO_ADDR, (u8*)info, sizeof(MotorInfoStruct));
    }
}


/*********************************************************************************************
函 数 名: servMotorInfoRead
实现功能: 从EEPROM的地址上读取电机信息
输入参数: 
输出参数: 无;
返 回 值:
说    明: CJ 2017.05.19 Modify
*********************************************************************************************/
int servMotorInfoRead(MotorInfoStruct * info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_MOTOR_INFO_ADDR, (u8*)info, sizeof(MotorInfoStruct));
    crc = bspCRC32((u8*)info, sizeof(MotorInfoStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servMotionInfoWrite
实现功能: 向EEPROM的地址上写入运行信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servMotionInfoWrite(MotionInfoStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(MotionInfoStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_MOTION_INFO_ADDR, (u8*)info, sizeof(MotionInfoStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servMotionInfoRead
实现功能: 从EEPROM的地址上读取运行信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servMotionInfoRead(MotionInfoStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_MOTION_INFO_ADDR, (u8*)info, sizeof(MotionInfoStruct));
    crc = bspCRC32((u8*)info, sizeof(MotionInfoStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servReportInfoWrite
实现功能: 向EEPROM的地址上写入上报器信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servReportInfoWrite(ReportInfoStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(ReportInfoStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    { 
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_REPORT_INFO_ADDR, (u8*)info, sizeof(ReportInfoStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servReportInfoRead
实现功能: 从EEPROM的地址上读取上报器信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servReportInfoRead(ReportInfoStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_REPORT_INFO_ADDR, (u8*)info, sizeof(ReportInfoStruct));
    crc = bspCRC32((u8*)info, sizeof(ReportInfoStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servTrigInInfoWrite
实现功能: 向EEPROM的地址上写入触发输入信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servTrigInInfoWrite(TrigInInfoStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(TrigInInfoStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {   
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_TRIT_IN_INFO_ADDR, (u8*)info, sizeof(TrigInInfoStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servTrigInInfoRead
实现功能: 从EEPROM的地址上读取触发输入信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servTrigInInfoRead(TrigInInfoStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_TRIT_IN_INFO_ADDR, (u8*)info, sizeof(TrigInInfoStruct));
    crc = bspCRC32((u8*)info, sizeof(TrigInInfoStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servTrigOutInfoWrite
实现功能: 向EEPROM的地址上写入触发输入信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servTrigOutInfoWrite(DigitalOutStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(DigitalOutStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {  
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_TRIG_OUT_INFO_ADDR, (u8*)info, sizeof(DigitalOutStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servTrigOutInfoRead
实现功能: 从EEPROM的地址上读取触发输入信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servTrigOutInfoRead(DigitalOutStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_TRIG_OUT_INFO_ADDR, (u8*)info, sizeof(DigitalOutStruct));
    crc = bspCRC32((u8*)info, sizeof(DigitalOutStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servIsolatorOutInfoStore
实现功能: 向EEPROM的地址上写入隔离输出信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servIsolatorOutInfoStore(IsolatorOutStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(IsolatorOutStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    { 
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_ISOL_OUT_INFO_ADDR, (u8*)info, sizeof(IsolatorOutStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servIsolatorOutInfoRead
实现功能: 从EEPROM的地址上读取隔离输出信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servIsolatorOutInfoRead(IsolatorOutStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_ISOL_OUT_INFO_ADDR, (u8*)info, sizeof(IsolatorOutStruct));
    crc = bspCRC32((u8*)info, sizeof(IsolatorOutStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servSensorUartInfoStore
实现功能: 向EEPROM的地址上写入传感器UART信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servSensorUartInfoStore(SensorUartStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(SensorUartStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {   
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_SENSOR_UART_INFO_ADDR, (u8*)info, sizeof(SensorUartStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servSensorUartInfoRead
实现功能: 从EEPROM的地址上读取传感器UART信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servSensorUartInfoRead(SensorUartStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_SENSOR_UART_INFO_ADDR, (u8*)info, sizeof(SensorUartStruct));
    crc = bspCRC32((u8*)info, sizeof(SensorUartStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


#ifdef PROJECT_QUBELEY
/*********************************************************************************************
函 数 名: servAnalogInInfoWrite
实现功能: 向EEPROM的地址上写入模拟输入信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servAnalogInInfoWrite(AnalogInfoStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(AnalogInfoStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {   
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_ANALOG_IN_INFO_ADDR, (u8*)info, sizeof(AnalogInfoStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servAnalogInInfoRead
实现功能: 从EEPROM的地址上读取模拟输入信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servAnalogInInfoRead(AnalogInfoStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_ANALOG_IN_INFO_ADDR, (u8*)info, sizeof(AnalogInfoStruct));
    crc = bspCRC32((u8*)info, sizeof(AnalogInfoStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}
#endif


/*********************************************************************************************
函 数 名: servPlanInfoRead
实现功能: 从EEPROM的地址上读取PVT信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servPlanInfoRead(PlanInfoStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_MOTION_PLAY_INFO_ADDR, (u8*)info, sizeof(PlanInfoStruct));
    crc = bspCRC32((u8*)info, sizeof(PlanInfoStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servPlanInfoStore
实现功能: 向EEPROM的地址上写入PVT信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servPlanInfoStore(PlanInfoStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(PlanInfoStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {   
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_MOTION_PLAY_INFO_ADDR, (u8*)info, sizeof(PlanInfoStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servDeviceInfoWrite
实现功能: 向EEPROM的地址上写入设备信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.06.19 Add
*********************************************************************************************/
int servDeviceInfoStore(DeviceInfoStorageStruct *info)
{
    u32 crc = 0;

    
    crc = bspCRC32((u8*)info, (sizeof(DeviceInfoStorageStruct) - sizeof(info->crc)));

    if (info->crc != crc)
    {  
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_DEVICE_INFO_ADDR, (u8*)info, sizeof(DeviceInfoStorageStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servDeviceInfoRead
实现功能: 从EEPROM的地址上读取序列号信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.06.19 Add
*********************************************************************************************/
int servDeviceInfoRead(DeviceInfoStorageStruct *info)
{
    u32 crc = 0;

    
    bspAT24CxxRead(EEPROM_DEVICE_INFO_ADDR, (u8*)info, sizeof(DeviceInfoStorageStruct));
    crc = bspCRC32((u8*)info, sizeof(DeviceInfoStorageStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servDriverInfoStore
实现功能: 向EEPROM的地址上写入驱动信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.10.24 Add
*********************************************************************************************/
int servDriverInfoStore(DriverInfoStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(DriverInfoStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {   
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_DRIVER_INFO_ADDR, (u8*)info, sizeof(DriverInfoStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servDriverInfoRead
实现功能: 从EEPROM的地址上读取驱动信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.10.24 Add
*********************************************************************************************/
int servDriverInfoRead(DriverInfoStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_DRIVER_INFO_ADDR, (u8*)info, sizeof(DriverInfoStruct));
    crc = bspCRC32((u8*)info, sizeof(DriverInfoStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


#ifdef PROJECT_GELGOOG
#if GELGOOG_SINANJU
/*********************************************************************************************
函 数 名: servSensorAlarmInfoRead
实现功能: 从EEPROM的地址上读取传感器告警信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2018.03.22 Add
*********************************************************************************************/
int servSensorAlarmInfoRead(SensorAlarmStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_SENSOR_ALARM_ADDR, (u8*)info, sizeof(SensorAlarmStruct));
    crc = bspCRC32((u8*)info, sizeof(SensorAlarmStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}


/*********************************************************************************************
函 数 名: servSensorAlarmInfoStore
实现功能: 向EEPROM的地址上写入传感器告警信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2018.03.22 Add
*********************************************************************************************/
int servSensorAlarmInfoStore(SensorAlarmStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(SensorAlarmStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {   
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_SENSOR_ALARM_ADDR, (u8*)info, sizeof(SensorAlarmStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


#else


/*********************************************************************************************
函 数 名: servIsolatorInInfoWrite
实现功能: 向EEPROM的地址上写入隔离输入信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servIsolatorInInfoStore(IsolatorInStruct *info)
{
    u32 crc;

    
    crc = bspCRC32((u8*)info, (sizeof(IsolatorInStruct) - sizeof(info->crc)));
    
    if (info->crc != crc)
    {  
        info->crc = crc;
    
        return bspAT24CxxWrite(EEPROM_ISOL_IN_INFO_ADDR, (u8*)info, sizeof(IsolatorInStruct));
    }
    else
    {
        return VERIFY_SUCCESSFUL;
    }
}


/*********************************************************************************************
函 数 名: servIsolatorInInfoRead
实现功能: 从EEPROM的地址上读取隔离输入信息
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: CJ 2017.05.24 Add
*********************************************************************************************/
int servIsolatorInInfoRead(IsolatorInStruct *info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_ISOL_IN_INFO_ADDR, (u8*)info, sizeof(IsolatorInStruct));
    crc = bspCRC32((u8*)info, sizeof(IsolatorInStruct) - sizeof(info->crc));
    
    if (crc == info->crc)
    {
        return VERIFY_SUCCESSFUL;
    }
    else
    {
        return VERIFY_FAILED;
    }
}
#endif
#endif



/*******************************************文件尾********************************************/
