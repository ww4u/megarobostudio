
#ifndef _DRV_EEPROM_H
#define _DRV_EEPROM_H

#include "bspI2C_gpio.h"
//#include "comStruct.h"

//EEPROMд��ǰ���͵�����
#define EEPROM_WRITE_CMD    0xA0 //1010 000 0
//EEPROM����ǰ���͵�����
#define EEPROM_READ_CMD     0xA1 //1010 000 1
        
#define EEPROM_PAGE_SIZE    (64)  //EEPEROMҳ��С
#define EEPROM_SIZE_MAX     0x4000  //16K EEPROM�������



void bspEepromInit();
int bspAT24CxxWrite(u16 u16addr,u8 * src, u32 len);
int bspAT24CxxRead(u16 u16addr,u8 * src, u32 len);
#define bspFramWrite  bspAT24CxxWrite
#define bspFramRead   bspAT24CxxRead

void drvEEPROMTest();//just for test

#endif

