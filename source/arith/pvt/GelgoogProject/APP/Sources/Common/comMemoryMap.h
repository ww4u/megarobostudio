/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  comMemoryMap.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.05.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __COM_MEMORY_MAP_H
#define __COM_MEMORY_MAP_H



/*******************************************宏定义********************************************/
/****************************************FLASH地址分配****************************************/
//FLASH的大小为768KBytes
#define    FLASH_BOOT_CODE_ADDR              0x08000000
#define    FLASH_BOOT_CODE_LEN               0x8000        //分配32K个Bytes

#define    FLASH_MAIN_CODE_ADDR              (FLASH_BOOT_CODE_ADDR + FLASH_BOOT_CODE_LEN)        //0x08008000
#define    FLASH_MAIN_CODE_LEN               0x2F800       //分配190K个Bytes      

#ifdef PROJECT_GELGOOG
#define    FLASH_FPGA_CODE_ADDR              0x08040000    //固定为1024KB的后768KB
#define    FLASH_FPGA_CODE_LEN               0xC0000       //分配768K个Bytes

#else

#define    FLASH_FPGA_CODE_ADDR              0x08040000    //固定为768KB的后512KB
#define    FLASH_FPGA_CODE_LEN               0x80000       //分配512K个Bytes
#endif


/***************************************EEPROM地址分配****************************************/
//EEPROM的大小为16KBytes
#define    EEPROM_START_ADDR                 0
#define    EEPROM_END_ADDR                   0x3FFF

/***********这两部分和BOOT共用，不可随意更改，容易造成版本不兼容 BEGIN***********/
#define    EEPROM_BOOT_INFO_ADDR             EEPROM_START_ADDR
#define    EEPROM_BOOT_INFO_LEN              64    //分配64个Bytes

#define    EEPROM_INTEFACE_INFO_ADDR         (EEPROM_BOOT_INFO_ADDR + EEPROM_BOOT_INFO_LEN)
#define    EEPROM_INTEFACE_INFO_LEN          64    //分配64个Bytes
/***********这两部分和BOOT共用，不可随意更改，容易造成版本不兼容 END*************/

#define    EEPROM_SYS_INFO_ADDR              (EEPROM_INTEFACE_INFO_ADDR + EEPROM_INTEFACE_INFO_LEN)
#define    EEPROM_SYS_INFO_LEN               64    //分配64个Bytes

#define    EEPROM_MOTOR_INFO_ADDR            (EEPROM_SYS_INFO_ADDR + EEPROM_SYS_INFO_LEN) 
#ifdef PROJECT_QUBELEY
#define    EEPROM_MOTOR_INFO_LEN             64    //分配64个Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_MOTOR_INFO_LEN             320    //分配320个Bytes
#endif

#define    EEPROM_MOTION_INFO_ADDR           (EEPROM_MOTOR_INFO_ADDR + EEPROM_MOTOR_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_MOTION_INFO_LEN            128    //分配128个Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_MOTION_INFO_LEN            640    //分配640个Bytes
#endif

#define    EEPROM_TRIT_IN_INFO_ADDR          (EEPROM_MOTION_INFO_ADDR + EEPROM_MOTION_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_TRIT_IN_INFO_LEN           96     //分配96个Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_TRIT_IN_INFO_LEN           256    //分配256个Bytes
#endif

#define    EEPROM_REPORT_INFO_ADDR           (EEPROM_TRIT_IN_INFO_ADDR + EEPROM_TRIT_IN_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_REPORT_INFO_LEN            128    //分配128个Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_REPORT_INFO_LEN            640    //分配640个Bytes
#endif

#define    EEPROM_TRIG_OUT_INFO_ADDR         (EEPROM_REPORT_INFO_ADDR + EEPROM_REPORT_INFO_LEN)
#define    EEPROM_TRIG_OUT_INFO_LEN          96    //分配96个Bytes

#define    EEPROM_ISOL_OUT_INFO_ADDR         (EEPROM_TRIG_OUT_INFO_ADDR + EEPROM_TRIG_OUT_INFO_LEN)
#define    EEPROM_ISOL_OUT_INFO_LEN          64    //分配64个Bytes

#define    EEPROM_SENSOR_UART_INFO_ADDR      (EEPROM_ISOL_OUT_INFO_ADDR + EEPROM_ISOL_OUT_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_SENSOR_UART_INFO_LEN       64     //分配64个Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_SENSOR_UART_INFO_LEN       128    //分配128个Bytes
#endif

#define    EEPROM_DRIVER_INFO_ADDR           (EEPROM_SENSOR_UART_INFO_ADDR + EEPROM_SENSOR_UART_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_DRIVER_INFO_LEN            64     //分配64个Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_DRIVER_INFO_LEN            320    //分配320个Bytes
#endif

#define    EEPROM_DEVICE_INFO_ADDR           (EEPROM_DRIVER_INFO_ADDR + EEPROM_DRIVER_INFO_LEN)
#define    EEPROM_DEVICE_INFO_LEN            64     //分配64个Bytes

#define    EEPROM_PRESET_INFO_ADDR           (EEPROM_DEVICE_INFO_ADDR + EEPROM_DEVICE_INFO_LEN)
#ifdef PROJECT_QUBELEY
#define    EEPROM_PRESET_INFO_LEN            1792    //分配1792个Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_PRESET_INFO_LEN            6944    //分配6944个Bytes
#endif

#ifdef PROJECT_QUBELEY
#define    EEPROM_ANALOG_IN_INFO_ADDR        (EEPROM_PRESET_INFO_ADDR + EEPROM_PRESET_INFO_LEN)
#define    EEPROM_ANALOG_IN_INFO_LEN         32    //分配32个Bytes
#endif
#ifdef PROJECT_GELGOOG
#define    EEPROM_ISOL_IN_INFO_ADDR          (EEPROM_PRESET_INFO_ADDR + EEPROM_PRESET_INFO_LEN)
#define    EEPROM_ISOL_IN_INFO_LEN           32    //分配32个Bytes
#endif



/****************************************DDR地址分配****************************************/
//DDR的大小为2Gbits(256MBytes)，一个波表占4Bytes，总共可以放入64M个波表点数
#define    DDR_START_ADDR                    0
#define    DDR_END_ADDR                      0x0FFFFFFF
#define    DDR_WAVE_TABLE_PRESET1_NUM        4
#define    DDR_WAVE_TABLE_PRESET5_NUM        4


//(单通道)1个主波表区45M，1个微调波表区5M，8个预设波表区，4个为2.5M(波表点数)，4个为1M
#define    DDR_WAVE_TABLE_CHAN_OFFSET1       0

#define    DDR_MAIN_WAVE_TABLE_ADDR1         DDR_START_ADDR
#define    DDR_MAIN_WAVE_TABLE_SIZE1         0x02D00000        //分配45M

#define    DDR_INCHING_WAVE_TABLE_ADDR1      DDR_MAIN_WAVE_TABLE_ADDR1 + DDR_MAIN_WAVE_TABLE_SIZE1
#define    DDR_INCHING_WAVE_TABLE_SIZE1      0x00500000        //分配5M

#define    DDR_PRESET1_WAVE_TABLE_ADDR1      DDR_INCHING_WAVE_TABLE_ADDR1 + DDR_INCHING_WAVE_TABLE_SIZE1
#define    DDR_PRESET1_WAVE_TABLE_SIZE1      0x00280000        //分配2.5M

#define    DDR_PRESET5_WAVE_TABLE_ADDR1      DDR_PRESET1_WAVE_TABLE_ADDR1 + DDR_PRESET1_WAVE_TABLE_SIZE1 * DDR_WAVE_TABLE_PRESET1_NUM
#define    DDR_PRESET5_WAVE_TABLE_SIZE1      0x00100000        //分配1M


//(3、4通道)1个主波表区11.25M，1个微调波表区1.25M，8个预设波表区，4个为0.625M(波表点数)，4个为0.25M
//将PRESET全部修改为0.625M(655360)，满足64细分、50速比下(200*64*50 = 640000)，可以保存1圈周期的情况
//相应的INCHING减少到0.625M    CJ 2018.01.10 Modify
#define    DDR_WAVE_TABLE_CHAN_OFFSET4       0x01000000    //16M

#define    DDR_MAIN_WAVE_TABLE_ADDR4         DDR_START_ADDR
#define    DDR_MAIN_WAVE_TABLE_SIZE4         0x00A60000        //分配10.375M

#define    DDR_INCHING_WAVE_TABLE_ADDR4      DDR_MAIN_WAVE_TABLE_ADDR4 + DDR_MAIN_WAVE_TABLE_SIZE4
#define    DDR_INCHING_WAVE_TABLE_SIZE4      0x000A0000        //分配0.625M

#define    DDR_PRESET1_WAVE_TABLE_ADDR4      DDR_INCHING_WAVE_TABLE_ADDR4 + DDR_INCHING_WAVE_TABLE_SIZE4
#define    DDR_PRESET1_WAVE_TABLE_SIZE4      0x000A0000        //分配0.625M

#define    DDR_PRESET5_WAVE_TABLE_ADDR4      DDR_PRESET1_WAVE_TABLE_ADDR4 + DDR_PRESET1_WAVE_TABLE_SIZE4 * DDR_WAVE_TABLE_PRESET1_NUM
#define    DDR_PRESET5_WAVE_TABLE_SIZE4      0x000A0000        //分配0.625M


//(6、7、8单通道)1个主波表区5.625M，1个微调波表区0.625M，8个预设波表区，4个为0.3125M(波表点数)，4个为0.125M
#define    DDR_WAVE_TABLE_CHAN_OFFSET8       0x00800000    //8M

#define    DDR_MAIN_WAVE_TABLE_ADDR8         DDR_START_ADDR
#define    DDR_MAIN_WAVE_TABLE_SIZE8         0x005A0000        //分配5.625M

#define    DDR_INCHING_WAVE_TABLE_ADDR8      DDR_MAIN_WAVE_TABLE_ADDR8 + DDR_MAIN_WAVE_TABLE_SIZE8
#define    DDR_INCHING_WAVE_TABLE_SIZE8      0x000A0000        //分配0.625M

#define    DDR_PRESET1_WAVE_TABLE_ADDR8      DDR_INCHING_WAVE_TABLE_ADDR8 + DDR_INCHING_WAVE_TABLE_SIZE8
#define    DDR_PRESET1_WAVE_TABLE_SIZE8      0x00050000        //分配0.3125M

#define    DDR_PRESET5_WAVE_TABLE_ADDR8      DDR_PRESET1_WAVE_TABLE_ADDR8 + DDR_PRESET1_WAVE_TABLE_SIZE8 * DDR_WAVE_TABLE_PRESET1_NUM
#define    DDR_PRESET5_WAVE_TABLE_SIZE8      0x00020000        //分配0.125M


#endif
/*****************************************END OF FILE****************************************/
