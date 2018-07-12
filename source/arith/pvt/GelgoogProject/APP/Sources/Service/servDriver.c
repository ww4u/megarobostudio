/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��: servDriver.c��
��������:     ;
�����б�:     ;
��ǰ�汾:     1.0.0;
�汾�Ķ�:     ;
��     ��:  CJ;
�������:     2017.10.25;
��ʷ�汾:     ��;
*********************************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "project.h"
#include "bspSdio.h"
#include "bspGpio.h"
#include "servFpga.h"
#include "servDriver.h"
#include "servSoftTimer.h"



/****************************************�ⲿ��������*****************************************/
extern MotorInfoStruct   g_motorInfo; 
extern SystemStateStruct g_systemState;
extern DriverInfoStruct  g_driverInfo;



/*****************************************�ֲ��궨��******************************************/
stTMC26xxWarn g_stTMCWarn;   //��TMC�Ĵ����ض��õ��ı�����Ϣ


#if 0
//DRVCTRL �Ĵ�������
#define TMC_SET_INTPOL(data)      (g_stTMC26xxDrvConf.unCtrlStepDir.stCtrlStepDir.INTPOL = data&0x01)  //��ֵ�������ÿ��� dataΪ0ʱ�ر�   Ϊ1ʱ����
#define TMC_SET_DEDGE(data)       (g_stTMC26xxDrvConf.unCtrlStepDir.stCtrlStepDir.DEDGE = data&0x01 )  //��������   dataΪ0ʱ������   Ϊ1ʱ ˫����
#define TMC_SET_MRES(data)        (g_stTMC26xxDrvConf.unCtrlStepDir.stCtrlStepDir.MRES = data&0x0F)  //����ϸ����


//CHOPCONF �Ĵ�������
#define TMC_SET_BLANK_TIME(data)         (g_stTMC26xxDrvConf.unChopConf.stChopConf.TBL = data&0x03)  //blank time ʱ������
#define TMC_SET_CHOPPER_MODE(data)       (g_stTMC26xxDrvConf.unChopConf.stChopConf.CHM = data&0x01)  //ն��ģʽ����   dataΪ0ʱspreadcycle   Ϊ1ʱ �㶨�ض�ʱ��
#define TMC_SET_RNDTF(data)              (g_stTMC26xxDrvConf.unChopConf.stChopConf.RNDTF = data&0x01)    //�ض�ʱ��ģʽ dataΪ 0 �̶�ʱ��    1 ���ʱ��
#define TMC_SET_HYSTER_DEC_INTER(data)   (g_stTMC26xxDrvConf.unChopConf.stChopConf.HDEC = data&0x03)  //spreadCycleģʽʱ ����Hysteresis decrement period ֵ
#define TMC_SET_FAST_DECAY_MODE(data)    (g_stTMC26xxDrvConf.unChopConf.stChopConf.HDEC = data&0x01)  //�㶨�ض�ʱ��ģʽ��,fast decay����  
#define TMC_SET_HYSTER_END_VAL(data)     (g_stTMC26xxDrvConf.unChopConf.stChopConf.HEND = data &0x0F)  //spreadCycleģʽʱ   hysteresis end value
#define TMC_SET_SINE_OFFSET(data)        (g_stTMC26xxDrvConf.unChopConf.stChopConf.HEND = data &0x0F)  //�㶨�ض�ʱ��ģʽ�� sine wave offset
#define TMC_SET_HYSTER_START_VAL(data)   (g_stTMC26xxDrvConf.unChopConf.stChopConf.HSTRT =data &0x07)  //spreadCycleģʽʱ  hysteresis start value
#define TMC_SET_FAST_DECAY_TIME(data)    (g_stTMC26xxDrvConf.unChopConf.u32ChopConf = g_stTMC26xxDrvConf.unChopConf.u32ChopConf&(~0x870) |(((data&0x08)<<11)|((data&0x07)<<4)))  //�㶨�ض�ʱ��ģʽ�� ����fast decayʱ�� ��ʱ��ֵ�� bit[11/6/5/4]������ 
#define  TMC_SET_TOFF_TIME(data)         (g_stTMC26xxDrvConf.unChopConf.stChopConf.TOFF = data&0x0F)  // ���ùض�ʱ��

//SMARTEN �Ĵ�������
#define TMC_SET_SEIMIN(data)        (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEIMIN = data&0x01)    //coolstep ��С����
#define TMC_SET_SEDN(data)          (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEDN = data&0x03)       //�����½��ٶ�
#define TMC_SET_SEMAX(data)         (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEMAX = data &0x0F)         //stallguard �ĸ���ֵ
#define TMC_SET_SEUP(data)          (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEUP = data&0x03)       //���������ٶ�
#define  TMC_SET_SEMIN(data)        (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEMIN = data&0x0F)      //stallguard�ĵ���ֵ 



//SGCSCONF �Ĵ�������
#define TMC_SET_SFILT(data)      (g_stTMC26xxDrvConf.unSgcsConf.stSgcsConf.SFILT = data&0x01)  //stallguard filter ʹ��
#define TMC_SET_SGT(data)        (g_stTMC26xxDrvConf.unSgcsConf.stSgcsConf.SGT = data&0x7F)       //stallguard ������ֵ   �����ֿ���Ϊ����  ��Ҫע���Ƿ���Խ��а�λ�߼�����
#define TMC_SET_CS(data)         (g_stTMC26xxDrvConf.unSgcsConf.stSgcsConf.CS = data&0x1F)                 //�������ű���


//DRVCONF �Ĵ�������
#define TMC_SET_TST_MODE(data)      (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.TST = data&0x01)      //����ģʽ  ��������ʱ  ��λ��������
#define TMC_SET_SLPH(data)          (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.SLPH = data&0x03)       //slope control high side 
#define TMC_SET_SLPL(data)          (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.SLPL = data&0x03)       //slope control low side
#define TMC_SET_DISS2G(data)        (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.DISS2G = data&0x01)      //�ӵض�·����ʹ��  dataΪ 0 ʹ��  1����
#define TMC_SET_TS2G(data)          (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.TS2G = data&0x03)          //�ӵض�·���ʱ��
#define TMC_SET_SDOFF(data)         (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.SDOFF = data&0x01)      // dataΪ 0 ʹ�ܲ���/�������     1 SPI���͵��������������
#define TMC_SET_VSENSE(data)        (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.VSENSE = data&0x01)      //VSENSE
#define TMC_SET_RDSEL(data)         (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.RDSEL = data&0x03)          //�ض�״̬�Ĵ���
#endif


/*************************************�ֲ����������Ͷ���**************************************/
typedef enum 
{
    DRVACT_WRITE = 0,
    DRVACT_READ
    
}DrvConfActionEnum;



/******************************************�ֲ�����*******************************************/
static DriverConfigUnion   DRVCONF[CH_TOTAL];
static CoolStepCofingUnion SMARTEN[CH_TOTAL];



/******************************************����ʵ��*******************************************/
#if 0
/*********************************************************************************************
�� �� ��: drvTMC_Reg_Conf
ʵ�ֹ���: TMC262��2660�ļĴ�������
�������: 
          u8 u8Reg:    �Ĵ�����ַ  ȡֵֻ������
                            ADDR_TMC_DRVCTRL    (0x00)  
                            ADDR_TMC_CHOPCONF   (0x04)  
                            ADDR_TMC_SMATREN    (0x05)  
                            ADDR_TMC_SGCSCONF   (0x06)  
                            ADDR_TMC_DRVCONF    (0x07)  
          u32 confdata:
            ���õļĴ�������,Ŀǰ�������ݶ���ȫ�ֱ���
          26XX DRVCTRL �Ĵ�������
          26XX CHOPCONF �Ĵ�������
          26XX SMARTEN �Ĵ�������
          26XX SGCSCONF �Ĵ�������
          26XX DRVCONF �Ĵ�������
          u8 action:��д����
                            TMC_DATA_WRITE   0   //ҪдTMC�ļĴ���
                            TMC_DATA_READ    1   //Ҫ��TMC�ļĴ���
                      
�������:
�� �� ֵ: 
˵    ��: ����ʱ�����ȷ���λ  �󷢸�λ
*********************************************************************************************/
static s32 drvTMC_Reg_Conf(u8 u8Reg,u8 action)
{
    u32 frame = 0; //���õ�ַλ
    u16 data = 0;
#if 1   
    switch(u8Reg)
    {
    case ADDR_TMC_DRVCTRL:
        g_stTMC26xxDrvConf.unCtrlStepDir.stCtrlStepDir.addr = u8Reg;
        frame = g_stTMC26xxDrvConf.unCtrlStepDir.u32CtrlStepDir;
      break;
    case ADDR_TMC_CHOPCONF:
      g_stTMC26xxDrvConf.unChopConf.stChopConf.addr = u8Reg;
      frame = g_stTMC26xxDrvConf.unChopConf.u32ChopConf;
      break;
    case ADDR_TMC_SMARTEN:
      g_stTMC26xxDrvConf.unSmarten.stSmarten.addr = u8Reg;
      frame = g_stTMC26xxDrvConf.unSmarten.u32Smarten;
      break;
    case ADDR_TMC_SGCSCONF:
      g_stTMC26xxDrvConf.unSgcsConf.stSgcsConf.addr = u8Reg;
      frame = g_stTMC26xxDrvConf.unSgcsConf.u32SgcsConf;
      break;
    case ADDR_TMC_DRVCONF:
      g_stTMC26xxDrvConf.unDrvConf.stDrvConf.addr =  u8Reg;
      frame = g_stTMC26xxDrvConf.unDrvConf.u32DrvConf;
      break;
    default:
      break;
    }
#endif

    data = (frame&0xFFFF); //��ַ��16bit
    servFpgaRegisterWrite(0, ADDR_FPGA_DRIVER_CFG_DATA_L, data);    //FOR MODIFY NICK
    
    data = ((frame>>16)&0xFFFF); //��ַ��4bit
    data = (data&(~(1<<4)))|((action&0x01)<<4);  //bit4Ϊ0 ��ʾд����  1��ʾ������
    servFpgaRegisterWrite(0, ADDR_FPGA_DRIVER_CFG_DATA_H, data);    //FOR MODIFY NICK
    
    return 0;
}

/*********************************************************************************************
�� �� ��: servTMC_Reg_Write
ʵ�ֹ���: TMC262��2660�ļĴ������ݻض�
�������:
          u8      u8Reg:    �Ĵ�����ַ  ȡֵֻ������
                            RDSEL_MICRO_POS   0x00    //�ض�λ��
                            RDSEL_SG_LVL      0x01    //�ض�stallguard ����
                            RDSEL_SG_CS_LVL   0x02    //�ض�stallguard��coolstep ����
          u32* ReadData:
                     �ض����ݵĵ�ַ
�������:
�� �� ֵ:
˵    ��: ����ʱ�����ȷ���λ  �󷢸�λ
*********************************************************************************************/
s32 servTMC_Reg_Write(u8 u8Reg)
{
    return drvTMC_Reg_Conf(u8Reg,TMC_DATA_WRITE);
}
/*********************************************************************************************
�� �� ��: servTMC_Reg_Read
ʵ�ֹ���: TMC262��2660�ļĴ������ݻض�
�������: 
          u8      u8Reg:    �Ĵ�����ַ  ȡֵֻ������
                            RDSEL_MICRO_POS   0x00    //�ض�λ��
                            RDSEL_SG_LVL      0x01    //�ض�stallguard ����
                            RDSEL_SG_CS_LVL   0x02    //�ض�stallguard��coolstep ����
          u32* ReadData:
                     �ض����ݵĵ�ַ
�������:
�� �� ֵ: 
˵    ��: ����ʱ�����ȷ���λ  �󷢸�λ
*********************************************************************************************/
s32 servTMC_Reg_Read(u8 u8Reg,u32* ReadData)
{
    TMC_SET_RDSEL(u8Reg);
    return servTmcReadSelectedReg(ReadData);
}


/*********************************************************************************************
�� �� ��: drv_TMC_State_Parse
ʵ�ֹ���: ����TMC�ڲ�״̬
�������: 

�������:
            
�� �� ֵ: 
˵    ��:
         
*********************************************************************************************/
static void drv_TMC_State_Parse(u32 u32SPIData,stTMC26xxWarn stTMCWarn)
{
    u8 tmp = (u32SPIData&0xFF);;

    memcpy((void*)&stTMCWarn,&tmp,sizeof(tmp));

    if(stTMCWarn.bStall)
    {
        //printf("Stall detected\n");
    }
    
    if(stTMCWarn.bOverTempDown)
    {
        //printf("Temp Over 150 detected\n");
    }

    if(stTMCWarn.bOverTempWarn)
    {
        //printf("Temp Over 100 detected\n");
    }

    if(stTMCWarn.bS2GA)
    {
        //printf("A Mos Short detected\n");
    }

    if(stTMCWarn.bS2GB)
    {
        //printf("B Mos Short detected\n");
    }
    
    if(stTMCWarn.bOLA)
    {
        //printf("A open load detected\n");
    }

    if(stTMCWarn.bOLB)
    {
        //printf("B open load detected\n");
    }

    if(stTMCWarn.bSTST)
    {
        //printf("No PWM detected\n");
    }
    
}


/*********************************************************************************************
�� �� ��: serv_TMC_MS_get
ʵ�ֹ���: ��ȡ��ȦA���ڵ�ϸ�ֱ��λ��
�������: 
            u16 *u16MStep:λ�����ݵ�ַ
�������:
            
�� �� ֵ: 
˵    ��:
*********************************************************************************************/
void serv_TMC_MS_get(u16 *u16MStep)
{
    u32 u32Read = 0;
    servTMC_Reg_Read(RDSEL_MICRO_POS,&u32Read);
    *u16MStep = (u32Read>>10)&0xFFFF;
    drv_TMC_State_Parse(u32Read,g_stTMCWarn);
}

/*********************************************************************************************
�� �� ��: serv_TMC_SG_get
ʵ�ֹ���: ��ȡTMC��SG����
�������: 
            u16 *u16SG:SG���ݵ�ַ
�������:
            
�� �� ֵ: 
˵    ��:
*********************************************************************************************/
void serv_TMC_SG_get(u16 *u16SG)
{
    u32 u32Read = 0;
    servTMC_Reg_Read(RDSEL_SG_LVL,&u32Read);
    *u16SG = (u32Read>>10)&0xFFFF;
    drv_TMC_State_Parse(u32Read,g_stTMCWarn);
}


/*********************************************************************************************
�� �� ��: serv_TMC_CoolStep_get
ʵ�ֹ���: ��ȡTMC��coolstep ����ֵ����
�������: 
            u16 *u16SE:SG���ݵ�ַ
�������:
            
�� �� ֵ: 
˵    ��:
*********************************************************************************************/
void serv_TMC_CoolStep_get(u16 *u16SE)
{
    u32 u32Read = 0;
    servTMC_Reg_Read(RDSEL_SG_CS_LVL,&u32Read);
    *u16SE = (u32Read>>10)&0x01F;   //bit10 11 12 13 14 λΪcoolstep��ʵ������
    drv_TMC_State_Parse(u32Read,g_stTMCWarn);
}


void servTMC_MS_Set(MicroStepEnum MicroStep)
{
    //TMC_SET_MRES(MicroStep);   //ϸ������
    //drvTMC_Reg_Conf(ADDR_TMC_DRVCTRL,TMC_DATA_WRITE);      //�·�DRVCTRL����
    serv_TMC_DRVCTRL_cfg(NCFG,NCFG,MicroStep);

}

/*********************************************************************************************
�� �� ��: serv_TMC_DRVCTRL_cfg
ʵ�ֹ���: ����TMC26xx ��DRVCTRL�Ĵ���
�������: 
            euIntpolCtrl    intpol: �Ƿ�ʹ�ܲ�ֵ  Ŀǰ���� ���ǲ��ʺ�ʹ�ò�ֵ����
            euEdgeCtrl        edge: ��������  ������ ���� ����-�½�˫��
            euMicroStepRes  MicroStep: ϸ������
�������:
            
�� �� ֵ: 
˵    ��:
         ���ֻ��Ҫ����ĳ�������� �������õĲ�����ʹ��--NCFG
*********************************************************************************************/
void serv_TMC_DRVCTRL_cfg(euIntpolCtrl intpol,euEdgeCtrl edge,MicroStepEnum MicroStep)
{
    //DRVCTRL ����
    if(intpol != NCFG)
    {
        TMC_SET_INTPOL(intpol); //���ò�ֵ����
    }

    if(edge != NCFG)
    {
        TMC_SET_DEDGE(edge);   //�����źŵ���������Ч
    }

    if(MicroStep != NCFG)
    {
         TMC_SET_MRES(MicroStep);   //ϸ������
    }
    
    drvTMC_Reg_Conf(ADDR_TMC_DRVCTRL,TMC_DATA_WRITE);        //�·�DRVCTRL����
}
/*********************************************************************************************
�� �� ��: serv_TMC_CHOPCONF_Mode_Time
ʵ�ֹ���: ����TMC26xx CHOPCONF�Ĵ��� ģʽ��ʱ�����λ
�������: 
            euChopMode mode: ն��ģʽ  �㶨�ض�ʱ�����˥������ spreadcycle
            euBlankTIme BlkTime:    blank time
            euRNDTFMode RandomCtrl:
 �ض�ʱ�� ģʽ  �㶨�������
            euChopOffTime offtime:  ��˥��ʱ��
�������:
            
�� �� ֵ: 
˵    ��:ʱ��������� �㶨�ض�ʱ�����˥��ģʽ��spreadcycleģʽ����ͨ�õ�
         ���ֻ��Ҫ����ĳ�������� �������õĲ�����ʹ��--NCFG
*********************************************************************************************/
void serv_TMC_CHOPCONF_Mode_Time(euChopMode mode,euBlankTIme BlkTime,euRNDTFMode RandomCtrl,euChopOffTime offtime)
{
    if(NCFG != mode)
    {
         TMC_SET_CHOPPER_MODE(mode);//ʹ��spreadcycle ն��ģʽ
    }
    if(NCFG != BlkTime)
    {
         TMC_SET_BLANK_TIME(BlkTime);//TBL  Blanktime
    }
    if(NCFG != RandomCtrl)
    {
         TMC_SET_RNDTF(RandomCtrl);  //
    }
    if(NCFG != offtime)
    {
         TMC_SET_TOFF_TIME(offtime); 
    }

    drvTMC_Reg_Conf(ADDR_TMC_CHOPCONF,TMC_DATA_WRITE);
    
}

/*********************************************************************************************
�� �� ��: serv_TMC_CHOPCONF_Hyst
ʵ�ֹ���: ����TMC26xx CHOPCONF ������ز���������
�������: 
          euHSTRT start:������ʼֵ
          euHEND    end:������ֵֹ
          euHDEC    dec:����˥������
�������:
            
�� �� ֵ: 
˵    ��:
         ���ֻ��Ҫ����ĳ�������� �������õĲ�����ʹ��--NCFG
*********************************************************************************************/
void serv_TMC_CHOPCONF_Hyst(euHSTRT start,euHEND end,euHDEC dec)
{
    if(NCFG != start)
    {
         TMC_SET_HYSTER_START_VAL(start);   //HSTRT����
    
    }
    if(NCFG != end)
    {
         TMC_SET_HYSTER_END_VAL(end);     //HEND����
    }
    if(NCFG != dec)
    {
         TMC_SET_HYSTER_DEC_INTER(dec);
    }
    drvTMC_Reg_Conf(ADDR_TMC_CHOPCONF,TMC_DATA_WRITE);
}

/*********************************************************************************************
�� �� ��: serv_TMC_CHOPCONF_Const_para
ʵ�ֹ���: ����TMC26xx CHOPCNOF��constant offtime and fast decay timeģʽ����ز���
�������: 
            euFastDecayMode   FDMode:����˥��ֹͣ������ ֻ�����ʱ��ֹͣ ���� ��ѹ�Ƚ���Ҳ����ֹͣ
            euSineOffset  SineOffset:���Ҳ���ƫ��
            euFDTime          fdtime:����˥��ʱ������ ��λΪʱ������
�������:
            
�� �� ֵ: 
˵    ��:
         ���ֻ��Ҫ����ĳ�������� �������õĲ�����ʹ��--NCFG
*********************************************************************************************/
void serv_TMC_CHOPCONF_Const_para(euFastDecayMode FDMode,euSineOffset SineOffset,euFDTime fdtime)
{
    if(NCFG != FDMode)
    {
         TMC_SET_FAST_DECAY_MODE(FDMode);
    }
    if(NCFG != SineOffset)
    {
         TMC_SET_SINE_OFFSET(SineOffset);
    }
    if(NCFG != SineOffset)
    {
         TMC_SET_FAST_DECAY_TIME(fdtime);
    }
    drvTMC_Reg_Conf(ADDR_TMC_CHOPCONF,TMC_DATA_WRITE);
}
/*********************************************************************************************
�� �� ��: serv_TMC_SMARTEN_cfg
ʵ�ֹ���: ����TMC26XX
�������: 
        euMinCS mincs:         coolstepʱ��С�ĵ���ֵ
        euDownSpeed DnSpeed:   �����½�ǰ��⵽��SG���޴���
        euUpSize Upsize:       SG�����޺����ÿ�������ļ���
        euSEMin semin:         coolstepʱ SG����=semin*32, semin=0��ʾ�ر�coolstep
        euSEMax semax:         coolstepʱ SG����=(semax+semin+1)*32 
�������:
            
�� �� ֵ: 
˵    ��:
         ���ֻ��Ҫ����ĳ�������� �������õĲ�����ʹ��--NCFG
*********************************************************************************************/
void serv_TMC_SMARTEN_cfg(euMinCS mincs,euDownSpeed DnSpeed,euUpSize Upsize,euSEMin semin,euSEMax semax)
{
    if(NCFG != mincs)
    {
         TMC_SET_SEIMIN(mincs);  //��С����ֵ���� 
    }
    if(NCFG != DnSpeed)
    {
         TMC_SET_SEDN(DnSpeed);
    }
    if(NCFG != Upsize)
    {
         TMC_SET_SEUP(Upsize);
    }

    if(NCFG != semin)
    {
         TMC_SET_SEMIN(semin);
    }
    if(NCFG != semax)
    {
         TMC_SET_SEMAX(semax);
    }
     drvTMC_Reg_Conf(ADDR_TMC_SMARTEN,TMC_DATA_WRITE);
}

/*********************************************************************************************
�� �� ��: serv_TMC_SGCSCONF_cfg
ʵ�ֹ���: ����TMC26xxSGCSCONF�Ĵ���
�������: 
            euSGfiltCtrl sfilt:�Ƿ�����stallguard�˲�
            SGTValue       sgt:stallguard����
            CSValue         cs: ��������ϵ��
�������:
            
�� �� ֵ: 
˵    ��:
         ���ֻ��Ҫ����ĳ�������� �������õĲ�����ʹ��--NCFG
*********************************************************************************************/
void serv_TMC_SGCSCONF_cfg(euSGfiltCtrl sfilt,SGTValue sgt,CSValue cs)
{
    if(NCFG != sfilt)
    {
         TMC_SET_SFILT(sfilt);
    }

    if((sgt>=-64)&&(sgt<=63))
    {
         TMC_SET_SGT(sgt);
    }

    if(cs<=31)
    {
         TMC_SET_CS(cs);
    }
    drvTMC_Reg_Conf(ADDR_TMC_SGCSCONF,TMC_DATA_WRITE);
}

/*********************************************************************************************
�� �� ��: serv_TMC_DRVCONF_Slope_Sense
ʵ�ֹ���: ����TMC26xxDRVCONF�Ĵ���б�ʺ͸�Ӧ��������
�������: 
            euSlopeL     LSlope:�߲�б��
            euSlopeH     HSlope:�Ͳ�б��
            euVsenseCtrl vsense:��Ӧ�����ֵ����  ��Ӱ������������
�������:
            
�� �� ֵ: 
˵    ��:
         ���ֻ��Ҫ����ĳ�������� �������õĲ�����ʹ��--NCFG
*********************************************************************************************/
#if 1

void serv_TMC_DRVCONF_Slope_Sense(euSlopeL LSlope,euSlopeH HSlope,euVsenseCtrl vsense)
{
    if(NCFG != LSlope)
    {
         TMC_SET_SLPL(LSlope);
    }
    if(NCFG != HSlope)
    {
         TMC_SET_SLPH(HSlope);
    }
    if(NCFG != vsense)
    {
        TMC_SET_VSENSE(vsense);
    }
    drvTMC_Reg_Conf(ADDR_TMC_DRVCONF,TMC_DATA_WRITE);
}
#endif 


/*********************************************************************************************
�� �� ��: serv_TMCDRVCONF_S2G_cfg
ʵ�ֹ���: ����TMC26xxDRVCONF�ӵض�·�������
�������: 
            euS2GCtrl mode:�Ƿ�������·����
            euS2GTime time:��·���ʱ��

�������:
            
�� �� ֵ: 
˵    ��:
         ���ֻ��Ҫ����ĳ�������� �������õĲ�����ʹ��--NCFG
*********************************************************************************************/
void serv_TMCDRVCONF_S2G_cfg(euS2GCtrl mode,euS2GTime time)
{
    if(NCFG != mode)
    {
         TMC_SET_DISS2G(mode);
    }
    if(NCFG != time)
    {
         TMC_SET_TS2G(time);
    }
    drvTMC_Reg_Conf(ADDR_TMC_DRVCONF,TMC_DATA_WRITE);
}


/*********************************************************************************************
�� �� ��: servTMCStatusMonitor
ʵ�ֹ���: ���TMCоƬ������״̬
�������: 
            
�������:
            
�� �� ֵ: 
˵    ��:Ŀǰ�ж�TMC״̬����������Ϊ 
        1.SG��������        ����  bit0 = 1
        2.S2GA S2GB��Ϊ0    ����  bit1 = 1
        3.OTPW OT ��Ϊ0     ����  bit2 = 1
        4.΢����������      ����  bit3 = 1
*********************************************************************************************/
u16 servTMCStatusMonitor(void)
{
    static u16 u16LastMStep = 0;  //�ϴμ�¼����΢����
    static u8 u8SGZeroCount = 0;  //SGΪ0������¼
    static bool bSGMonitorValid = false;  //SG��Ч��ر�־
    u16 u16SG = 0;
    u16 u16CurrentMStep = 0;
    u32 u32MSReg = 0;
    u8  S2GA = 0;
    u8  S2GB = 0;
    u8  OTPW = 0;
    u8  OT = 0;
    u8  ST = 0;
    u8  error = 0;
    
    serv_TMC_MS_get((u16 *)&u32MSReg);
    u16CurrentMStep = (u32MSReg>>10)&0x00FF;
    OT = (u32MSReg>>1)&0x01;
    OTPW = (u32MSReg>>2)&0x01;
    S2GA = (u32MSReg>>3)&0x01;
    S2GB = (u32MSReg>>4)&0x01;
    ST = (u32MSReg>>7)&0x01;

    if(ST == 0)//�Ǿ�ֹ״̬
    {
        u16LastMStep = 0;
    }
    else
    {
        if((u16LastMStep != 0)&&(u16CurrentMStep == 0)) //�ϴξ�ֹ״̬΢�����ݲ�Ϊ0�ұ���΢������Ϊ0 ��ʾ�ھ�ֹ״̬��΢�����ݳ�����ͻ��
        {
            error |= 0x08;  //΢������
        }
        u16LastMStep = u16CurrentMStep;
    }

    serv_TMC_SG_get(&u16SG);

    if(u16SG != 0)  //sg���ݲ�Ϊ0 ��ʾ��������
    {
        bSGMonitorValid = true;
        u8SGZeroCount = 0;
    }
    else
    {
        u8SGZeroCount++;
        if((bSGMonitorValid == true)&&(u8SGZeroCount > 3))//��������3��sgΪ0 ��Ϊ����
        {
            error |= 0x01;  //SG����
            bSGMonitorValid = false; //��⵽sgΪ0�� �����ȡ�����䵫�������˶�  sg����ֵ��ȻΪ0  ���Դ�ʱ�����ٱ��� 
        }
    }


    if((S2GA)||(S2GB)) 
    {
        error |= 0x02; //��·����
    }
    
    if(OT||OTPW)
    {
        error |= 0x04; //���´���
    }
    
    return error;
}
#endif


#if 0
#endif


/*********************************************************************************************
�� �� ��: servDriverRegConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
s32 servDriverRegConfig(u8 regNum, u32 regVaule, DrvConfActionEnum drvConfAct)
{
    u16 data;

    data = (regVaule & 0xFFFF);    //��16bit
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_OPERT_L_REG, data);
    
    data = ((regVaule >> 16)& 0xFFFF);    //��4bit
    
    data = (data & SERV_FPGA_DRV_OPERT_VALUE_H_MASK) | 
           ((drvConfAct << SERV_FPGA_DRV_OPERT_ACT_LSHIFT) & SERV_FPGA_DRV_OPERT_ACT_MASK);    //����
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_OPERT_H_REG, data);
    
    return 0;
}


/*********************************************************************************************
�� �� ��: servDriverRegWrite;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
s32 servDriverRegWrite(u8 chanNum, u8 regNum, u32 regValue)
{
    servFpgaDriverSelect(chanNum);
    servDriverRegConfig(regNum, regValue, DRVACT_WRITE);

    if (ADDR_TMC_DRVCONF == chanNum)
    {
        DRVCONF[chanNum].regValue = regValue;    //�����£����Ĵ���ʱҪʹ��
    }

    if (ADDR_TMC_SMARTEN == chanNum)
    {
        SMARTEN[chanNum].regValue = regValue;    //�����£����Ĵ���ʱҪʹ��
    }
    
    return 0;
}


/*********************************************************************************************
�� �� ��: servDriverStateMonitor
ʵ�ֹ���: ���TMCоƬ������״̬
�������:            
�������:          
�� �� ֵ: 
˵     ��: ���������ݼ�����ݵĿ������ѡ���ȡ�Ĵ��������㡣������ݰ����ܺģ�SGALL��SGSE
*********************************************************************************************/
void servDriverStateMonitor(u8 chanNum, DriverMonitorStruct driverMonitor)
{
    DriverReadRespUnion readResp;
    u16 error = 0;
    s8  sgPercent;
    u8  sePercent;

    static u16  lastMStep[CH_TOTAL]    = {0};  //�ϴμ�¼����΢����
    static u8   sgZeroCount[CH_TOTAL]  = {0};  //SGΪ0������¼
    static bool bSgMontValid[CH_TOTAL] = {false};  //SG��Ч��ر�־
    static u8   bCsDisable[CH_TOTAL]   = {0};
    static u8   seminBackup[CH_TOTAL]  = {0}; 


    servDriverStateRegRead(chanNum, READREG_R0, &readResp.regValue);
    
    if (readResp.readRespOne.STST == 0)    //�Ǿ�ֹ״̬
    {
        lastMStep[chanNum] = 0;
    }
    else
    {
        //�ϴξ�ֹ״̬΢�����ݲ�Ϊ0�ұ���΢������Ϊ0 ��ʾ�ھ�ֹ״̬��΢�����ݳ�����ͻ��
        if ((lastMStep[chanNum] != 0) && (readResp.readRespOne.MSTEP == 0)) 
        {
            error |= 0x08;  //΢������
        }
        lastMStep[chanNum] = readResp.readRespOne.MSTEP;
    }
    
    if (readResp.readRespOne.S2GA || readResp.readRespOne.S2GB) 
    {
        error |= 0x02; //��·����
    }
    
    if (readResp.readRespOne.OT || readResp.readRespOne.OTPW)
    {
        error |= 0x04; //���´���
    }

    //����ܺ�/SGSE���
    if (((driverMonitor.energyValue != NULL) && (SENSOR_ON == driverMonitor.energyState)) ||
        ((driverMonitor.sgseValue   != NULL) && (SENSOR_ON == driverMonitor.sgseState)))
    {
        servDriverStateRegRead(chanNum, READREG_R2, &readResp.regValue);

        if ((driverMonitor.energyValue != NULL) && (SENSOR_ON == driverMonitor.energyState))
        {
            //sgPercent�Ŵ���100�������Գ�100��SGH�Ǹ�5λ��������Ҫ��32��Ȼ���ٷŴ�һ�ٱ�����3200.0��
            sgPercent = (s8)(100 - (f32)readResp.readRespThree.SGH * 3200.0f / driverMonitor.sgZero);
            sePercent = (u8)(readResp.readRespThree.SE * 100.0f / driverMonitor.csValue);

            *driverMonitor.energyValue = (driverMonitor.sgZero << 16) | ((u8)sgPercent << 8) | sePercent;

            if (sgPercent >= driverMonitor.sgUpLimit)
            {
                error |= 0x10;    //SG����
            }
            else if (sgPercent <= driverMonitor.sgDnLimit)
            {
                error |= 0x20;    //SG����
            }

            if (sgPercent < 0.0f)    //С��0��ʾ���ⲿ���� ���ܳ��� ��Ҫ��Ϊ�Ӵ����
            {
                if (!bCsDisable[chanNum])    //û�ر�coolStep��ر�һ��
                {
                    seminBackup[chanNum] = SMARTEN[chanNum].regBitFiled.SEMIN;
                    SMARTEN[chanNum].regBitFiled.SEMIN = 0;    //�ر�coolStep������������CS��

                    servDriverRegWrite(chanNum, ADDR_TMC_SMARTEN, SMARTEN[chanNum].regValue);

                    bCsDisable[chanNum] = true;
                }
            }
            else    //����״���������Ҫ�ָ�coolstep
            {
                if (bCsDisable[chanNum])
                {
                    SMARTEN[chanNum].regBitFiled.SEMIN = seminBackup[chanNum];

                    servDriverRegWrite(chanNum, ADDR_TMC_SMARTEN, SMARTEN[chanNum].regValue);
                    
                    bCsDisable[chanNum] = false;
                }
            }
        }

        if ((driverMonitor.sgseValue != NULL) && (SENSOR_ON == driverMonitor.sgseState))
        {
            if (readResp.readRespThree.SGH != 0)    //sg���ݲ�Ϊ0 ��ʾ��������
            {
                bSgMontValid[chanNum] = true;
                sgZeroCount[chanNum]  = 0;
            }
            else
            {
                sgZeroCount[chanNum]++;
                if ((bSgMontValid[chanNum]) && (sgZeroCount[chanNum] > 3))    //��������3��sgΪ0 ��Ϊ����
                {
                    error |= 0x01;    //SG����
                    bSgMontValid[chanNum] = false;    //��⵽sgΪ0�� �����ȡ�����䵫�������˶�  sg����ֵ��ȻΪ0  ���Դ�ʱ�����ٱ��� 
                }
            }
        }
    
        readResp.readRespThree.resv_bit20_31 = error;    //��12λ�Ŵ�����

        /*if (driverMonitor.energyValue != NULL)
        {
            *driverMonitor.energyValue = readResp.regValue;
        }*/

        if (driverMonitor.sgseValue != NULL)
        {
            *driverMonitor.sgseValue = readResp.regValue;
        }
    }
    
    if ((driverMonitor.sgAllValue != NULL) && (SENSOR_ON == driverMonitor.sgAllState))
    {
        servDriverStateRegRead(chanNum, READREG_R1, &readResp.regValue);
    
        readResp.readRespTwo.resv_bit20_31 = error;    //��12λ�Ŵ�����
        *driverMonitor.sgAllValue = readResp.regValue;
    }
}


/*********************************************************************************************
�� �� ��: servDriverCtrlEnable;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servDriverCtrlEnable(bool enableDriver)
{
#ifdef PROJECT_GELGOOG
    if (enableDriver)
    {
        bspGpioDriverCtrlChanASet(Bit_RESET);
        bspGpioDriverCtrlChanBSet(Bit_RESET);
    }
    else
    {
        bspGpioDriverCtrlChanASet(Bit_SET);
        bspGpioDriverCtrlChanBSet(Bit_SET);
    }
#endif    
}


/*********************************************************************************************
�� �� ��: servDriverStateRegRead;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
u8 servDriverStateRegRead(u8 chanNum, DriverReadRespEnum readReg, u32 *regValue)
{
    u16 tempReg;
    u8  realChan;
    

    if (regValue != NULL)
    {
        //��������: 1. ѡͨ����PROJECT_GELGOOG��; 
        //          2. дDRVCONF�Ĵ�����RDSELλ��ΪҪ��ȡ�ļĴ�����
        //          3. ����FPGA��DRIVER SPIΪ������
        //          4. ���Ĵ���
        servFpgaDriverSelect(chanNum);
        
        DRVCONF[chanNum].regBitFiled.RDSEL = readReg;    //ѡ����Ĵ���
        servDriverRegConfig(ADDR_TMC_DRVCONF, DRVCONF[chanNum].regValue, DRVACT_WRITE);
        bspDelayUs(50);
        
        servDriverRegConfig(ADDR_TMC_DRVCONF, DRVCONF[chanNum].regValue, DRVACT_READ);
        bspDelayUs(50);

        //�ȶ��ߣ�Ȼ���ж�ͨ�����Ƿ�һ�� 
        servFpgaRegisterRead(CH_SYS, SERV_FPGA_DRIVER_STATE_H_REG, &tempReg);
        realChan = (tempReg & SERV_FPGA_DRIVER_CHANNUM_MASK) >> SERV_FPGA_DRIVER_CHANNUM_RSHIFT;
        if (realChan == chanNum)
        {
            *regValue = (((u32)(tempReg & SERV_FPGA_DRIVER_STATE_H_MASK)) << SERV_FPGA_DRIVER_STATE_H_LSHIFT);
            bspDelayUs(50);
            
            servFpgaRegisterRead(CH_SYS, SERV_FPGA_DRIVER_STATE_L_REG, &tempReg);
            *regValue += tempReg;
    
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 2;
    }
}


/*********************************************************************************************
�� �� ��: servDriverCurrRegParaCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void servDriverCurrRegParaCalc(DriverTypeEnum type, u8 curr, u8 *vsense, u8 *csValue)
{
    float f32TarCurr = ((float)curr)*0.1;    //Ŀ�����
    float f32Curr_165 = 0;    
    float f32Curr_305 = 0;
    euVsenseCtrl euVol = VENSE_165MV;
    u8 u8CS = 12;

    if (DRIVER_2660 == type)
    {
        f32Curr_165 = (float)165 / R_SENSE_2660;    //165����ʱ��������
        f32Curr_305 = (float)305 / R_SENSE_2660;    //305����ʱ��������
        
        if (f32TarCurr > CURR_MAX_2660)
        {
            //�ϱ�����
            return;
        }
    }
    else
    {
        f32Curr_165 = (float)165/R_SENSE_262;    //165����ʱ��������
        f32Curr_305 = (float)305/R_SENSE_262;    //305����ʱ��������  

        if (f32TarCurr > CURR_MAX_262)
        {
            //�ϱ�����
            return;
        }
    }

    //��ȷ����ѹֵ
    if (f32TarCurr <= f32Curr_305/2)  //Ŀ������ﲻ��305����ʱ��һ����ʹ��165��������
    {
        if (f32TarCurr <= f32Curr_165)
        {
            euVol = VENSE_165MV; 
        }
        else
        {
            euVol = VENSE_305MV;
        }
    }
    else
    {
        euVol = VENSE_305MV;
    }

    if (euVol == VENSE_165MV)
    {
        u8CS = (u8)floor(f32TarCurr*32.0f/f32Curr_165);
    }
    else
    {
        u8CS = (u8)floor(f32TarCurr*32.0f/f32Curr_305);
    }

    *vsense  = euVol;
    *csValue = u8CS;
}


/*********************************************************************************************
�� �� ��: servDriverCurrCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ����CS��VSENSE�������;
*********************************************************************************************/
u8 servDriverCurrCalc(DriverTypeEnum type, u8 vsense, u8 csValue)
{
    u8  curr = 0;
    f32 volt;
    f32 currMax;


    //I(rms)  = (CS + 1) / 32 * V(fs) / R(sense) * 0.707
    //I(peak) = (CS + 1) / 32 * V(fs) / R(sense)
    if (1 == vsense)
    {
        volt = 165;    //mV
    }
    else
    {
        volt = 305;    //mV
    }
    
    if (DRIVER_2660 == type)
    {
        currMax = volt / R_SENSE_2660;
    }
    else
    {
        currMax = volt / R_SENSE_262;  
    }

    curr = (u8)((csValue + 1 ) * currMax * 10 / 32);    //0.1A
    
    return curr;
}


/*********************************************************************************************
�� �� ��: servDriverCurrentSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void servDriverCurrentSet(u8 chanNum, DriverManageStruct driverManage)
{
    //����VSENSE��CS
    servDriverRegWrite(chanNum, ADDR_TMC_SGCSCONF, driverManage.SGCSCONF.regValue);    //CS
    bspDelayUs(200);
    servDriverRegWrite(chanNum, ADDR_TMC_DRVCONF, driverManage.DRVCONF.regValue);      //VSENSE
    
    DRVCONF[chanNum].regValue = driverManage.DRVCONF.regValue;    //�����£����Ĵ���ʱҪʹ��
}


/*********************************************************************************************
�� �� ��: servDriverMicroStepSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void servDriverMicroStepSet(u8 chanNum, DriverManageStruct driverManage)
{
    //����΢����
    servDriverRegWrite(chanNum, ADDR_TMC_DRVCTRL, driverManage.DRVCTRL.regValue);
}


/*********************************************************************************************
�� �� ��: servDriverConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void servDriverConfig(u8 chanNum, DriverManageStruct driverManage)
{
    u8 vsense;
    u8 csValue;

    
    servFpgaDriverCofing(chanNum, driverManage.type, DRVSTATE_ON, DRVCLK_OPEN);    //��ʼ���߼���TMC���� 
    
    //servDriverCtrlEnable(true);
    //servFpgaDriverSelect(chanNum);
    
    if (driverManage.type < DRIVER_UNKNOW)
    {
        servDriverCurrRegParaCalc(driverManage.type, driverManage.curr, &vsense, &csValue);
        driverManage.DRVCONF.regBitFiled.VSENSE = vsense;
        driverManage.SGCSCONF.regBitFiled.CS    = csValue;
        
        //ϸ�֡�����ģʽ���ڲ���ֵ����
        servDriverRegWrite(chanNum, ADDR_TMC_DRVCTRL, driverManage.DRVCTRL.regValue);
        bspDelayUs(200);
        
        servDriverRegWrite(chanNum, ADDR_TMC_CHOPCONF, driverManage.CHOPCONF.regValue);
        bspDelayUs(200);
        
        servDriverRegWrite(chanNum, ADDR_TMC_SGCSCONF, driverManage.SGCSCONF.regValue);
        bspDelayUs(200);
        
        servDriverRegWrite(chanNum, ADDR_TMC_DRVCONF, driverManage.DRVCONF.regValue);
        DRVCONF[chanNum].regValue = driverManage.DRVCONF.regValue;    //�����£����Ĵ���ʱҪʹ��
        bspDelayUs(200);
        
        servDriverRegWrite(chanNum, ADDR_TMC_SMARTEN, driverManage.SMARTEN.regValue);
        SMARTEN[chanNum].regValue = driverManage.SMARTEN.regValue;    //�����£����Ĵ���ʱҪʹ��
        bspDelayUs(200);
    }
    else
    {
#if JTAG_MODE 
        printf("unknow motor driver type!\n");
#endif      
    }
}


/*********************************************************************************************
�� �� ��: servDriverSwitch;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵     ��: ��;
*********************************************************************************************/
void servDriverSwitch(u8 chanNum, DriverManageStruct driverManage)
{
    switch (driverManage.state)
    {
        case SENSOR_OFF:
            servDriverRegWrite(chanNum, ADDR_TMC_CHOPCONF, 0);
          break;
          
        case SENSOR_ON:
            servDriverConfig(chanNum, driverManage);
          break;

        default:
          break;
    }
}



/*******************************************�ļ�β********************************************/
