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

#define    EEPROM_SYS_INFO_ADDR              (EEPROM_INTEFACE_INFO_ADDR + EEPROM_INTEFACE_INFO_LEN)
#define    EEPROM_SYS_INFO_LEN               64    //����64��Bytes

#define    EEPROM_MOTOR_INFO_ADDR            (EEPROM_SYS_INFO_ADDR + EEPROM_SYS_INFO_LEN) 
#ifdef PROJECT_QUBELEY
#define    EEPROM_MOTOR_INFO_LEN             64    //����64��Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_MOTOR_INFO_LEN             320    //����320��Bytes
#endif

#define    EEPROM_MOTION_INFO_ADDR           (EEPROM_MOTOR_INFO_ADDR + EEPROM_MOTOR_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_MOTION_INFO_LEN            128    //����128��Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_MOTION_INFO_LEN            640    //����640��Bytes
#endif

#define    EEPROM_TRIT_IN_INFO_ADDR          (EEPROM_MOTION_INFO_ADDR + EEPROM_MOTION_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_TRIT_IN_INFO_LEN           96     //����96��Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_TRIT_IN_INFO_LEN           256    //����256��Bytes
#endif

#define    EEPROM_REPORT_INFO_ADDR           (EEPROM_TRIT_IN_INFO_ADDR + EEPROM_TRIT_IN_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_REPORT_INFO_LEN            128    //����128��Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_REPORT_INFO_LEN            640    //����640��Bytes
#endif

#define    EEPROM_TRIG_OUT_INFO_ADDR         (EEPROM_REPORT_INFO_ADDR + EEPROM_REPORT_INFO_LEN)
#define    EEPROM_TRIG_OUT_INFO_LEN          96    //����96��Bytes

#define    EEPROM_ISOL_OUT_INFO_ADDR         (EEPROM_TRIG_OUT_INFO_ADDR + EEPROM_TRIG_OUT_INFO_LEN)
#define    EEPROM_ISOL_OUT_INFO_LEN          64    //����64��Bytes

#define    EEPROM_SENSOR_UART_INFO_ADDR      (EEPROM_ISOL_OUT_INFO_ADDR + EEPROM_ISOL_OUT_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_SENSOR_UART_INFO_LEN       64     //����64��Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_SENSOR_UART_INFO_LEN       128    //����128��Bytes
#endif

#define    EEPROM_DRIVER_INFO_ADDR           (EEPROM_SENSOR_UART_INFO_ADDR + EEPROM_SENSOR_UART_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_DRIVER_INFO_LEN            64     //����64��Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_DRIVER_INFO_LEN            320    //����320��Bytes
#endif

#define    EEPROM_DEVICE_INFO_ADDR           (EEPROM_DRIVER_INFO_ADDR + EEPROM_DRIVER_INFO_LEN)
#define    EEPROM_DEVICE_INFO_LEN            64     //����64��Bytes

#define    EEPROM_PRESET_INFO_ADDR           (EEPROM_DEVICE_INFO_ADDR + EEPROM_DEVICE_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_PRESET_INFO_LEN            1792    //����1792��Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_PRESET_INFO_LEN            6944    //����6944��Bytes
#endif

#ifdef PROJECT_QUBELEY
#define    EEPROM_ANALOG_IN_INFO_ADDR        (EEPROM_PRESET_INFO_ADDR + EEPROM_PRESET_INFO_LEN)
#define    EEPROM_ANALOG_IN_INFO_LEN         32    //����32��Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_ISOL_IN_INFO_ADDR          (EEPROM_PRESET_INFO_ADDR + EEPROM_PRESET_INFO_LEN)
#define    EEPROM_ISOL_IN_INFO_LEN           32    //����32��Bytes
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


#endif
/*****************************************END OF FILE****************************************/
