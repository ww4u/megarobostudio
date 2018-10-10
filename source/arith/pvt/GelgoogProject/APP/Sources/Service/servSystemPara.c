/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servSysPara.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.08.15;
��ʷ�汾:  ��;
*********************************************************************************************/
#include "project.h"
#include "servSystemPara.h"
#include "comMemoryMap.h"
#include "bspCrc32.h"
#include "bspEeprom.h"



/****************************************�ⲿ��������*****************************************/



/*****************************************�ֲ��궨��******************************************/



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: servBootInfoWrite
ʵ�ֹ���: ��EEPROMд��boot_info��Ϣ
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
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
�� �� ��: servBootInfoRead
ʵ�ֹ���: ��EEPROM��ȡboot_info��Ϣ
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
int servBootInfoRead(BootInfoStruct * info)
{
    u32 crc;

    
    bspAT24CxxRead(EEPROM_BOOT_INFO_ADDR,(u8*)info,sizeof(BootInfoStruct));
    crc = bspCRC32((u8*)info,sizeof(BootInfoStruct) - sizeof(info->crc));
    
    return (crc == info->crc)?VERIFY_SUCCESSFUL:VERIFY_FAILED;    //CJ 2017.05.19 Modify
}


/*********************************************************************************************
�� �� ��: servInterfaceInfoWrite
ʵ�ֹ���: ��EEPROMд��IntfaceInfo��Ϣ
�������: 
�������: ��;
�� �� ֵ:
˵    ��: CJ 2017.05.19 Modify
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
�� �� ��: servInterfaceInfoRead
ʵ�ֹ���: ��EEPROM��ȡIntfaceInfo��Ϣ
�������: 
�������: ��;
�� �� ֵ:
˵    ��: CJ 2017.05.19 Modify
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
�� �� ��: servSystemInfoWrite
ʵ�ֹ���: ��EEPROMд��ϵͳ��Ϣ
�������: 
�������: ��;
�� �� ֵ:
˵    ��: CJ 2017.05.19 Modify
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
�� �� ��: servSystemInfoRead
ʵ�ֹ���: ��EEPROM��ȡϵͳ��Ϣ
�������: 
�������: ��;
�� �� ֵ:
˵    ��: CJ 2017.05.19 Modify
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
�� �� ��: servMotorInfoWrite
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д������Ϣ
�������: 
�������: ��;
�� �� ֵ:
˵    ��: CJ 2017.05.19 Modify
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
�� �� ��: servMotorInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ�����Ϣ
�������: 
�������: ��;
�� �� ֵ:
˵    ��: CJ 2017.05.19 Modify
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
�� �� ��: servMotionInfoWrite
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д��������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servMotionInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servReportInfoWrite
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д���ϱ�����Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servReportInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ�ϱ�����Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servTrigInInfoWrite
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д�봥��������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servTrigInInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ����������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servTrigOutInfoWrite
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д�봥��������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servTrigOutInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ����������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servIsolatorOutInfoStore
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д����������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servIsolatorOutInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ���������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servSensorUartInfoStore
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д�봫����UART��Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servSensorUartInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ������UART��Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servAnalogInInfoWrite
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д��ģ��������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servAnalogInInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡģ��������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servPlanInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡPVT��Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servPlanInfoStore
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д��PVT��Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servDeviceInfoWrite
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д���豸��Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.06.19 Add
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
�� �� ��: servDeviceInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ���к���Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.06.19 Add
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
�� �� ��: servDriverInfoStore
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д��������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.10.24 Add
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
�� �� ��: servDriverInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.10.24 Add
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
�� �� ��: servSensorAlarmInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ�������澯��Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2018.03.22 Add
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
�� �� ��: servSensorAlarmInfoStore
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д�봫�����澯��Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2018.03.22 Add
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
�� �� ��: servIsolatorInInfoWrite
ʵ�ֹ���: ��EEPROM�ĵ�ַ��д�����������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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
�� �� ��: servIsolatorInInfoRead
ʵ�ֹ���: ��EEPROM�ĵ�ַ�϶�ȡ����������Ϣ
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: CJ 2017.05.24 Add
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



/*******************************************�ļ�β********************************************/
