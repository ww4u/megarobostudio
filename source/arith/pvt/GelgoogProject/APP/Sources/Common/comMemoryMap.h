/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  comMemoryMap.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.05.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __COM_MEMORY_MAP_H
#define __COM_MEMORY_MAP_H



/*******************************************�궨��********************************************/
/****************************************FLASH��ַ����****************************************/
//FLASH�Ĵ�СΪ768KBytes
#define    FLASH_BOOT_CODE_ADDR              0x08000000
#define    FLASH_BOOT_CODE_LEN               0x8000        //����32K��Bytes

#define    FLASH_MAIN_CODE_ADDR              (FLASH_BOOT_CODE_ADDR + FLASH_BOOT_CODE_LEN)        //0x08008000
#define    FLASH_MAIN_CODE_LEN               0x2F800       //����190K��Bytes      

#ifdef PROJECT_GELGOOG
#define    FLASH_FPGA_CODE_ADDR              0x08040000    //�̶�Ϊ1024KB�ĺ�768KB
#define    FLASH_FPGA_CODE_LEN               0xC0000       //����768K��Bytes

#else

#define    FLASH_FPGA_CODE_ADDR              0x08040000    //�̶�Ϊ768KB�ĺ�512KB
#define    FLASH_FPGA_CODE_LEN               0x80000       //����512K��Bytes
#endif


/***************************************EEPROM��ַ����****************************************/
//EEPROM�Ĵ�СΪ16KBytes
#define    EEPROM_START_ADDR                 0
#define    EEPROM_END_ADDR                   0x3FFF

/***********�������ֺ�BOOT���ã�����������ģ�������ɰ汾������ BEGIN***********/
#define    EEPROM_BOOT_INFO_ADDR             EEPROM_START_ADDR
#define    EEPROM_BOOT_INFO_LEN              64    //����64��Bytes

#define    EEPROM_INTEFACE_INFO_ADDR         (EEPROM_BOOT_INFO_ADDR + EEPROM_BOOT_INFO_LEN)
#define    EEPROM_INTEFACE_INFO_LEN          64    //����64��Bytes
/***********�������ֺ�BOOT���ã�����������ģ�������ɰ汾������ END*************/

/*�洢�������кŵ���Ϣ�����Ե�ַ���ܸ��ģ�����ᵼ�����кŶ�ʧ!!!!!!!!!!!!!!!!!!*/
#define    EEPROM_DEVICE_INFO_ADDR           (EEPROM_INTEFACE_INFO_ADDR + EEPROM_INTEFACE_INFO_LEN)
#define    EEPROM_DEVICE_INFO_LEN            64     //����64��Bytes
/*�洢�������кŵ���Ϣ�����Ե�ַ���ܸ��ģ�����ᵼ�����кŶ�ʧ!!!!!!!!!!!!!!!!!!*/

#define    EEPROM_SYS_INFO_ADDR              (EEPROM_DEVICE_INFO_ADDR + EEPROM_DEVICE_INFO_LEN)
#define    EEPROM_SYS_INFO_LEN               128    //����128��Bytes


#if GELGOOG_AXIS_4    //MRQ-M4

#define    EEPROM_MOTOR_INFO_ADDR            (EEPROM_SYS_INFO_ADDR + EEPROM_SYS_INFO_LEN) 
#define    EEPROM_MOTOR_INFO_LEN             384

#define    EEPROM_MOTION_INFO_ADDR           (EEPROM_MOTOR_INFO_ADDR + EEPROM_MOTOR_INFO_LEN)
#define    EEPROM_MOTION_INFO_LEN            512

#define    EEPROM_TRIT_IN_INFO_ADDR          (EEPROM_MOTION_INFO_ADDR + EEPROM_MOTION_INFO_LEN)
#define    EEPROM_TRIT_IN_INFO_LEN           256

#define    EEPROM_REPORT_INFO_ADDR           (EEPROM_TRIT_IN_INFO_ADDR + EEPROM_TRIT_IN_INFO_LEN)
#define    EEPROM_REPORT_INFO_LEN            1024

#define    EEPROM_TRIG_OUT_INFO_ADDR         (EEPROM_REPORT_INFO_ADDR + EEPROM_REPORT_INFO_LEN)
#define    EEPROM_TRIG_OUT_INFO_LEN          128

#define    EEPROM_ISOL_OUT_INFO_ADDR         (EEPROM_TRIG_OUT_INFO_ADDR + EEPROM_TRIG_OUT_INFO_LEN)
#define    EEPROM_ISOL_OUT_INFO_LEN          64

#define    EEPROM_SENSOR_UART_INFO_ADDR      (EEPROM_ISOL_OUT_INFO_ADDR + EEPROM_ISOL_OUT_INFO_LEN)
#define    EEPROM_SENSOR_UART_INFO_LEN       192

#define    EEPROM_DRIVER_INFO_ADDR           (EEPROM_SENSOR_UART_INFO_ADDR + EEPROM_SENSOR_UART_INFO_LEN)
#define    EEPROM_DRIVER_INFO_LEN            512

#define    EEPROM_ISOL_IN_INFO_ADDR          (EEPROM_DRIVER_INFO_ADDR + EEPROM_DRIVER_INFO_LEN)
#define    EEPROM_ISOL_IN_INFO_LEN           64

#define    EEPROM_MOTION_PLAY_INFO_ADDR      (EEPROM_ISOL_IN_INFO_ADDR + EEPROM_ISOL_IN_INFO_LEN)
#define    EEPROM_MOTION_PLAY_INFO_LEN       1664


#elif GELGOOG_SINANJU    //MRQ-MV

#define    EEPROM_MOTOR_INFO_ADDR            (EEPROM_SYS_INFO_ADDR + EEPROM_SYS_INFO_LEN) 
#define    EEPROM_MOTOR_INFO_LEN             384

#define    EEPROM_MOTION_INFO_ADDR           (EEPROM_MOTOR_INFO_ADDR + EEPROM_MOTOR_INFO_LEN)
#define    EEPROM_MOTION_INFO_LEN            512

#define    EEPROM_TRIT_IN_INFO_ADDR          (EEPROM_MOTION_INFO_ADDR + EEPROM_MOTION_INFO_LEN)
#define    EEPROM_TRIT_IN_INFO_LEN           256

#define    EEPROM_REPORT_INFO_ADDR           (EEPROM_TRIT_IN_INFO_ADDR + EEPROM_TRIT_IN_INFO_LEN)
#define    EEPROM_REPORT_INFO_LEN            1024

#define    EEPROM_TRIG_OUT_INFO_ADDR         (EEPROM_REPORT_INFO_ADDR + EEPROM_REPORT_INFO_LEN)
#define    EEPROM_TRIG_OUT_INFO_LEN          128

#define    EEPROM_ISOL_OUT_INFO_ADDR         (EEPROM_TRIG_OUT_INFO_ADDR + EEPROM_TRIG_OUT_INFO_LEN)
#define    EEPROM_ISOL_OUT_INFO_LEN          64

#define    EEPROM_SENSOR_UART_INFO_ADDR      (EEPROM_ISOL_OUT_INFO_ADDR + EEPROM_ISOL_OUT_INFO_LEN)
#define    EEPROM_SENSOR_UART_INFO_LEN       192

#define    EEPROM_DRIVER_INFO_ADDR           (EEPROM_SENSOR_UART_INFO_ADDR + EEPROM_SENSOR_UART_INFO_LEN)
#define    EEPROM_DRIVER_INFO_LEN            512

#define    EEPROM_SENSOR_ALARM_ADDR          (EEPROM_DRIVER_INFO_ADDR + EEPROM_DRIVER_INFO_LEN)
#define    EEPROM_SENSOR_ALARM_LEN           256    //����128��Bytes

#define    EEPROM_MOTION_PLAY_INFO_ADDR      (EEPROM_SENSOR_ALARM_ADDR + EEPROM_SENSOR_ALARM_LEN)
#define    EEPROM_MOTION_PLAY_INFO_LEN       2048


#elif GELGOOG_AXIS_10    //MRQ-MC

#define    EEPROM_MOTOR_INFO_ADDR            (EEPROM_SYS_INFO_ADDR + EEPROM_SYS_INFO_LEN) 
#define    EEPROM_MOTOR_INFO_LEN             768

#define    EEPROM_MOTION_INFO_ADDR           (EEPROM_MOTOR_INFO_ADDR + EEPROM_MOTOR_INFO_LEN)
#define    EEPROM_MOTION_INFO_LEN            1024

#define    EEPROM_TRIT_IN_INFO_ADDR          (EEPROM_MOTION_INFO_ADDR + EEPROM_MOTION_INFO_LEN)
#define    EEPROM_TRIT_IN_INFO_LEN           512

#define    EEPROM_REPORT_INFO_ADDR           (EEPROM_TRIT_IN_INFO_ADDR + EEPROM_TRIT_IN_INFO_LEN)
#define    EEPROM_REPORT_INFO_LEN            2048

#define    EEPROM_TRIG_OUT_INFO_ADDR         (EEPROM_REPORT_INFO_ADDR + EEPROM_REPORT_INFO_LEN)
#define    EEPROM_TRIG_OUT_INFO_LEN          128

#define    EEPROM_ISOL_OUT_INFO_ADDR         (EEPROM_TRIG_OUT_INFO_ADDR + EEPROM_TRIG_OUT_INFO_LEN)
#define    EEPROM_ISOL_OUT_INFO_LEN          64

#define    EEPROM_SENSOR_UART_INFO_ADDR      (EEPROM_ISOL_OUT_INFO_ADDR + EEPROM_ISOL_OUT_INFO_LEN)
#define    EEPROM_SENSOR_UART_INFO_LEN       192

#define    EEPROM_DRIVER_INFO_ADDR           (EEPROM_SENSOR_UART_INFO_ADDR + EEPROM_SENSOR_UART_INFO_LEN)
#define    EEPROM_DRIVER_INFO_LEN            512

#define    EEPROM_ISOL_IN_INFO_ADDR          (EEPROM_DRIVER_INFO_ADDR + EEPROM_DRIVER_INFO_LEN)
#define    EEPROM_ISOL_IN_INFO_LEN           64

#define    EEPROM_MOTION_PLAY_INFO_ADDR      (EEPROM_ISOL_IN_INFO_ADDR + EEPROM_ISOL_IN_INFO_LEN)
#define    EEPROM_MOTION_PLAY_INFO_LEN       4096


#elif QUBELEY_HARDVER_1    //MRQ-C

#define    EEPROM_MOTOR_INFO_ADDR            (EEPROM_SYS_INFO_ADDR + EEPROM_SYS_INFO_LEN) 
#define    EEPROM_MOTOR_INFO_LEN             128

#define    EEPROM_MOTION_INFO_ADDR           (EEPROM_MOTOR_INFO_ADDR + EEPROM_MOTOR_INFO_LEN)
#define    EEPROM_MOTION_INFO_LEN            128

#define    EEPROM_TRIT_IN_INFO_ADDR          (EEPROM_MOTION_INFO_ADDR + EEPROM_MOTION_INFO_LEN)
#define    EEPROM_TRIT_IN_INFO_LEN           128

#define    EEPROM_REPORT_INFO_ADDR           (EEPROM_TRIT_IN_INFO_ADDR + EEPROM_TRIT_IN_INFO_LEN)
#define    EEPROM_REPORT_INFO_LEN            256

#define    EEPROM_TRIG_OUT_INFO_ADDR         (EEPROM_REPORT_INFO_ADDR + EEPROM_REPORT_INFO_LEN)
#define    EEPROM_TRIG_OUT_INFO_LEN          64

#define    EEPROM_ISOL_OUT_INFO_ADDR         (EEPROM_TRIG_OUT_INFO_ADDR + EEPROM_TRIG_OUT_INFO_LEN)
#define    EEPROM_ISOL_OUT_INFO_LEN          64

#define    EEPROM_SENSOR_UART_INFO_ADDR      (EEPROM_ISOL_OUT_INFO_ADDR + EEPROM_ISOL_OUT_INFO_LEN)
#define    EEPROM_SENSOR_UART_INFO_LEN       64

#define    EEPROM_DRIVER_INFO_ADDR           (EEPROM_SENSOR_UART_INFO_ADDR + EEPROM_SENSOR_UART_INFO_LEN)
#define    EEPROM_DRIVER_INFO_LEN            128

#define    EEPROM_ANALOG_IN_INFO_ADDR        (EEPROM_DRIVER_INFO_ADDR + EEPROM_DRIVER_INFO_LEN)
#define    EEPROM_ANALOG_IN_INFO_LEN         64

#define    EEPROM_MOTION_PLAY_INFO_ADDR      (EEPROM_ANALOG_IN_INFO_ADDR + EEPROM_ANALOG_IN_INFO_LEN)
#define    EEPROM_MOTION_PLAY_INFO_LEN       480

#endif



/****************************************DDR��ַ����****************************************/
//DDR�Ĵ�СΪ2Gbits(256MBytes)��һ������ռ4Bytes���ܹ����Է���64M���������
#define    DDR_START_ADDR                    0
#define    DDR_END_ADDR                      0x0FFFFFFF
#define    DDR_WAVE_TABLE_PRESET1_NUM        4
#define    DDR_WAVE_TABLE_PRESET5_NUM        4


//(��ͨ��)1����������45M��1��΢��������5M��8��Ԥ�貨������4��Ϊ2.5M(�������)��4��Ϊ1M
#define    DDR_WAVE_TABLE_CHAN_OFFSET1       0

#define    DDR_MAIN_WAVE_TABLE_ADDR1         DDR_START_ADDR
#define    DDR_MAIN_WAVE_TABLE_SIZE1         0x02D00000        //����45M

#define    DDR_INCHING_WAVE_TABLE_ADDR1      DDR_MAIN_WAVE_TABLE_ADDR1 + DDR_MAIN_WAVE_TABLE_SIZE1
#define    DDR_INCHING_WAVE_TABLE_SIZE1      0x00500000        //����5M

#define    DDR_PRESET1_WAVE_TABLE_ADDR1      DDR_INCHING_WAVE_TABLE_ADDR1 + DDR_INCHING_WAVE_TABLE_SIZE1
#define    DDR_PRESET1_WAVE_TABLE_SIZE1      0x00280000        //����2.5M

#define    DDR_PRESET5_WAVE_TABLE_ADDR1      DDR_PRESET1_WAVE_TABLE_ADDR1 + DDR_PRESET1_WAVE_TABLE_SIZE1 * DDR_WAVE_TABLE_PRESET1_NUM
#define    DDR_PRESET5_WAVE_TABLE_SIZE1      0x00100000        //����1M


//(3��4ͨ��)1����������11.25M��1��΢��������1.25M��8��Ԥ�貨������4��Ϊ0.625M(�������)��4��Ϊ0.25M
//��PRESETȫ���޸�Ϊ0.625M(655360)������64ϸ�֡�50�ٱ���(200*64*50 = 640000)�����Ա���1Ȧ���ڵ����
//��Ӧ��INCHING���ٵ�0.625M    CJ 2018.01.10 Modify
#define    DDR_WAVE_TABLE_CHAN_OFFSET4       0x01000000    //16M

#define    DDR_MAIN_WAVE_TABLE_ADDR4         DDR_START_ADDR
#define    DDR_MAIN_WAVE_TABLE_SIZE4         0x00A60000        //����10.375M

#define    DDR_INCHING_WAVE_TABLE_ADDR4      DDR_MAIN_WAVE_TABLE_ADDR4 + DDR_MAIN_WAVE_TABLE_SIZE4
#define    DDR_INCHING_WAVE_TABLE_SIZE4      0x000A0000        //����0.625M

#define    DDR_PRESET1_WAVE_TABLE_ADDR4      DDR_INCHING_WAVE_TABLE_ADDR4 + DDR_INCHING_WAVE_TABLE_SIZE4
#define    DDR_PRESET1_WAVE_TABLE_SIZE4      0x000A0000        //����0.625M

#define    DDR_PRESET5_WAVE_TABLE_ADDR4      DDR_PRESET1_WAVE_TABLE_ADDR4 + DDR_PRESET1_WAVE_TABLE_SIZE4 * DDR_WAVE_TABLE_PRESET1_NUM
#define    DDR_PRESET5_WAVE_TABLE_SIZE4      0x000A0000        //����0.625M


//(5ͨ��(��е��))1����������7.175M��1��΢��������0.625M��8��Ԥ�貨������4��Ϊ0.625M(�������)��4��Ϊ0.625M
//��PRESETȫ���޸�Ϊ0.625M(655360)������64ϸ�֡�50�ٱ���(200*64*50 = 640000)�����Ա���1Ȧ���ڵ����
#define    DDR_WAVE_TABLE_CHAN_OFFSET5       0x00CCCCCC    //12.8M

#define    DDR_MAIN_WAVE_TABLE_ADDR5         DDR_START_ADDR
#define    DDR_MAIN_WAVE_TABLE_SIZE5         0x0072CCCC       //����7.175M

#define    DDR_INCHING_WAVE_TABLE_ADDR5      DDR_MAIN_WAVE_TABLE_ADDR5 + DDR_MAIN_WAVE_TABLE_SIZE5
#define    DDR_INCHING_WAVE_TABLE_SIZE5      0x000A0000        //����0.625M

#define    DDR_PRESET1_WAVE_TABLE_ADDR5      DDR_INCHING_WAVE_TABLE_ADDR5 + DDR_INCHING_WAVE_TABLE_SIZE5
#define    DDR_PRESET1_WAVE_TABLE_SIZE5      0x000A0000        //����0.625M

#define    DDR_PRESET5_WAVE_TABLE_ADDR5      DDR_PRESET1_WAVE_TABLE_ADDR5 + DDR_PRESET1_WAVE_TABLE_SIZE5 * DDR_WAVE_TABLE_PRESET1_NUM
#define    DDR_PRESET5_WAVE_TABLE_SIZE5      0x000A0000        //����0.625M


//(6��7��8��ͨ��)1����������5.625M��1��΢��������0.625M��8��Ԥ�貨������4��Ϊ0.3125M(�������)��4��Ϊ0.125M
#define    DDR_WAVE_TABLE_CHAN_OFFSET8       0x00800000    //8M

#define    DDR_MAIN_WAVE_TABLE_ADDR8         DDR_START_ADDR
#define    DDR_MAIN_WAVE_TABLE_SIZE8         0x005A0000        //����5.625M

#define    DDR_INCHING_WAVE_TABLE_ADDR8      DDR_MAIN_WAVE_TABLE_ADDR8 + DDR_MAIN_WAVE_TABLE_SIZE8
#define    DDR_INCHING_WAVE_TABLE_SIZE8      0x000A0000        //����0.625M

#define    DDR_PRESET1_WAVE_TABLE_ADDR8      DDR_INCHING_WAVE_TABLE_ADDR8 + DDR_INCHING_WAVE_TABLE_SIZE8
#define    DDR_PRESET1_WAVE_TABLE_SIZE8      0x00050000        //����0.3125M

#define    DDR_PRESET5_WAVE_TABLE_ADDR8      DDR_PRESET1_WAVE_TABLE_ADDR8 + DDR_PRESET1_WAVE_TABLE_SIZE8 * DDR_WAVE_TABLE_PRESET1_NUM
#define    DDR_PRESET5_WAVE_TABLE_SIZE8      0x00020000        //����0.125M


//10ͨ��
#define    DDR_WAVE_TABLE_CHAN_OFFSET10       0x00666666    //6.4M

#define    DDR_MAIN_WAVE_TABLE_ADDR10         DDR_START_ADDR
#define    DDR_MAIN_WAVE_TABLE_SIZE10         0x00396666       //����7.175M

#define    DDR_INCHING_WAVE_TABLE_ADDR10      DDR_MAIN_WAVE_TABLE_ADDR10 + DDR_MAIN_WAVE_TABLE_SIZE10
#define    DDR_INCHING_WAVE_TABLE_SIZE10      0x00050000        //����0.3125M

#define    DDR_PRESET1_WAVE_TABLE_ADDR10      DDR_INCHING_WAVE_TABLE_ADDR10 + DDR_INCHING_WAVE_TABLE_SIZE10
#define    DDR_PRESET1_WAVE_TABLE_SIZE10      0x00050000        //����0.3125M

#define    DDR_PRESET5_WAVE_TABLE_ADDR10      DDR_PRESET1_WAVE_TABLE_ADDR10 + DDR_PRESET1_WAVE_TABLE_SIZE10 * DDR_WAVE_TABLE_PRESET1_NUM
#define    DDR_PRESET5_WAVE_TABLE_SIZE10      0x00050000        //����0.3125M


#endif
/*****************************************END OF FILE****************************************/
