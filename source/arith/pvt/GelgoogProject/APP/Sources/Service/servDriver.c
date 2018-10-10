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
#if GELGOOG_AXIS_10
#define    DRIVER_SENSOR_REST     7.556    //(current / 10) * 0.34��* 100 / 0.45V = current * 7.556
#define    DRIVER_DUTY_MAX        100

#else

#if QUBELEY_HARDVER_1
#define    TMC_CURR_BOUNDARY      40       //��λ100mA
#define    R_SENSE_262            30       //��λm��
#else
#define    TMC_CURR_BOUNDARY      40       //��λ100mA
#define    R_SENSE_262            40       //��λm��
#endif

#define    VSENSE_NOT_SET_VOLT    310      //��λmV
#define    VSENSE_SET_VOLT        165      //��λmV
#define    TMC_CS_VALUE_MAX       31       //�Ĵ���ֵ
#define    TMC_SE_SCALE           32       //�Ĵ���ֵ
#endif



/*************************************�ֲ����������Ͷ���**************************************/
#if !GELGOOG_AXIS_10

typedef enum
{
    VENSE_NOT = 0,    //����VENSE��ֵΪ0��ѡ������305mV
    VENSE_SET = 1     //����VENSE��ֵΪ1��ѡ������165mV
    
}VsenseCtrlEnum;

#endif


/******************************************�ֲ�����*******************************************/
#if !GELGOOG_AXIS_10
static DriverConfigUnion   DRVCONF[CH_TOTAL];
static CoolStepCofingUnion SMARTEN[CH_TOTAL];
#endif



/******************************************����ʵ��*******************************************/
#if GELGOOG_AXIS_10
/*********************************************************************************************
�� �� ��: servDriverCurrentConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servDriverCurrentConfig(u8 current)
{
    u8  duty;
    u32 frequency;

    
    frequency = 1000;    //1KHz
    
    //duty = (current / 10) * 0.34��* 100 / 0.5V
    //duty = current * 6.8
    duty = (u8)(current * DRIVER_SENSOR_REST);

    //duty = 50;
      
    if (duty > DRIVER_DUTY_MAX)
    {
        duty = DRIVER_DUTY_MAX;
    }

    bspDriverTimerSet(frequency, duty);
}


/*********************************************************************************************  
�� �� ��: servDriverConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servDriverConfig(DriverInfoStruct driverInfo)
{
    u8 i;
    DriverResetEnum  drvReset = DRIVER_WORKING;
    DriverDecayEnum  drvDecay = DRIVER_SLOW;
    DriverSourceEnum drvSrc   = DRIVER_WAVE;

    
    servFpgaDriverResetSet(drvReset);
    servFpgaDriverDecaySet(drvDecay);
    servFpgaDriverSourceSet(drvSrc);
    
    servFpgaDriverModeSet(driverInfo.microStep);

    //���õ���
    servDriverCurrentConfig(driverInfo.curr);

    for (i = 0;i < CH_TOTAL;i++)
    {
        servFpgaDriverEnable(i, driverInfo.state[i]);
    }
}


/*********************************************************************************************
�� �� ��: servDriverSwitch;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servDriverSwitch(u8 chanNum, SensorStateEnum state)
{
    switch (state)
    {
        case SENSOR_OFF:
            servFpgaDriverEnable(chanNum, SENSOR_OFF);
          break;
          
        case SENSOR_ON:
            servFpgaDriverEnable(chanNum, SENSOR_ON);
          break;

        default:
          break;
    }
}


#else


/*********************************************************************************************
�� �� ��: servDriverRegWrite;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
s32 servDriverRegWrite(u8 chanNum, u8 regNum, u32 regValue)
{
    servFpgaDriverSelect(chanNum);
    servFpgaDriverConfig(regNum, regValue, DRVACT_WRITE);

    if (ADDR_TMC_DRVCONF == regNum)
    {
        DRVCONF[chanNum].regValue = regValue;    //�����£����Ĵ���ʱҪʹ��
    }

    if (ADDR_TMC_SMARTEN == regNum)
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
˵    ��: ���������ݼ�����ݵĿ������ѡ���ȡ�Ĵ��������㡣������ݰ����ܺģ�SGALL��SGSE
*********************************************************************************************/
void servDriverStateMonitor(u8 chanNum, DriverMonitorStruct driverMonitor)
{
    DriverReadRespUnion readResp;
    u8  state;
    s8  sgPercent;
    u8  sePercent;
    

    //����ܺ�/SGSE���
    if ((driverMonitor.energyValue != NULL) && (driverMonitor.sgseValue != NULL))
    {
        servDriverStateRegRead(chanNum, READREG_R2, &readResp.regValue);

        state = readResp.regValue & 0x0000000F;    //��8λ��״̬λ
        
        //sgPercent�Ŵ���100�������Գ�100��SGH�Ǹ�5λ��������Ҫ��32��Ȼ���ٱ���1000����3.2��
        sgPercent = (s8)(100 - (f32)readResp.readResp2.SGH * 3.2f);
        sePercent = (u8)(readResp.readResp2.SE * 100.0f / driverMonitor.csValue);

        *driverMonitor.energyValue = (state << 16) | ((u8)sgPercent << 8) | sePercent;
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
        servFpgaDriverConfig(ADDR_TMC_DRVCONF, DRVCONF[chanNum].regValue, DRVACT_WRITE);
        bspDelayUs(50);
        
        servFpgaDriverConfig(ADDR_TMC_DRVCONF, DRVCONF[chanNum].regValue, DRVACT_READ);
        bspDelayUs(50);

        //�ȶ��ߣ�Ȼ���ж�ͨ�����Ƿ�һ�� 
        servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_13, &tempReg);
        realChan = (tempReg & SERV_FPGA_DRIVER_CHANNUM_MASK) >> SERV_FPGA_DRIVER_CHANNUM_RSHIFT;
        if (realChan == chanNum)
        {
            *regValue = (((u32)(tempReg & SERV_FPGA_DRIVER_STATE_H_MASK)) << SERV_FPGA_DRIVER_STATE_H_LSHIFT);
            bspDelayUs(50);
            
            servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_12, &tempReg);
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
�� �� ��: servDriverCurrentSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
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
�� �� ��: servDriverCurrRegParaCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servDriverCurrRegParaCalc(DriverTypeEnum type, u8 curr, u8 *vsense, u8 *csValue)
{
#if 1    //CJ 2018.08.21 Modify

    u8 csCalcValue;

    
    //���ڼĴ�����CSֻ��5bit������ʹ���κ�һ�����̽��е�������ʱ��������Ƚϴ�����(�ֱ��ʲ���)
    //165mV���̵ķֱ�����0.129A��310mV���̵ķֱ�����0.242A

    //���ݶԼĴ��������㣬4A��4A����ʹ��165mv���̽�����������õĵ���ֵ��310mV���̸��ӽ�Ŀ��ֵ
    //��Ȼ4A��4A����ʹ��165mv���̽������ã���������������310mV���̴󣬵��ǲ��Ƚ�С����Ϊ
    //�˳��򷽱㣬4A��4A���¾�ֻʹ��165mv���̽��������ˡ�
    
    //ȫ������������ĵ���0.1A��ʵ��ֵ0.1289A�����28.9%�������С�ĵ���4.6A��ʵ��ֵ4.6016A�����0.034%

    //�Ȳ�����2660    TO ADD NICK
    if (DRIVER_262 == type)
    {
        //4A��4A����
        if (curr <= TMC_CURR_BOUNDARY)
        {
            *vsense = VENSE_SET;

            //Ipeak = (CS + 1) / 32 * Vfs / Rsense
            //CS = Iset * Rsense / Vfs * 32 - 1
            //CS = Iset * Rsense * 32 / Vfs - 0.5 ---- ��������
            csCalcValue = (u8)(curr * R_SENSE_262 * 32.0f / (VSENSE_SET_VOLT * 10) - 0.5);
        }
        else
        {
            *vsense = VENSE_NOT;

            //Ipeak = (CS + 1) / 32 * Vfs / Rsense
            //CS = Iset * Rsense / Vfs * 32 - 1
            //CS = Iset * Rsense * 32 / Vfs - 0.5 ---- ��������
            csCalcValue = (u8)(curr * R_SENSE_262 * 32.0f / (VSENSE_NOT_SET_VOLT * 10) - 0.5);
        }

        if (csCalcValue > TMC_CS_VALUE_MAX)
        {
            csCalcValue = TMC_CS_VALUE_MAX;
        }

        *csValue = csCalcValue;
    }
    
#else
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
#endif
}


/*********************************************************************************************
�� �� ��: servDriverCurrCalc;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ����CS��VSENSE�������;
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
�� �� ��: servDriverConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servDriverConfig(u8 chanNum, DriverManageStruct driverManage)
{
    u8 vsense;
    u8 csValue;

    
    if (driverManage.type < DRIVER_UNKNOW)
    {
        //���տ��е�����������
        //servDriverCurrRegParaCalc(driverManage.type, driverManage.curr, &vsense, &csValue);
        servDriverCurrRegParaCalc(driverManage.type, driverManage.idleCurr, &vsense, &csValue);
        
        driverManage.DRVCONF.regBitFiled.VSENSE = vsense;
        driverManage.SGCSCONF.regBitFiled.CS    = csValue;
        
        //ϸ�֡�����ģʽ���ڲ���ֵ����
        servDriverRegWrite(chanNum, ADDR_TMC_DRVCTRL, driverManage.DRVCTRL.regValue);
        bspDelayUs(200);

        if (SENSOR_OFF == driverManage.state)
        {
            driverManage.CHOPCONF.regBitFiled.TOFF = 0;
        }
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
˵    ��: ��;
*********************************************************************************************/
void servDriverSwitch(u8 chanNum, DriverManageStruct driverManage)
{
    switch (driverManage.state)
    {
        case SENSOR_OFF:
            //servDriverCtrlEnable(false);
            
            driverManage.CHOPCONF.regBitFiled.TOFF = 0;
            servDriverRegWrite(chanNum, ADDR_TMC_CHOPCONF, driverManage.CHOPCONF.regValue);
          break;
          
        case SENSOR_ON:
            //servDriverCtrlEnable(true);
            
            servDriverConfig(chanNum, driverManage);
          break;

        default:
          break;
    }
}


/*********************************************************************************************
�� �� ��: servDriverTuningSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servDriverTuningSet(u8 chanNum, DriverManageStruct *pDriverManage)
{
    u8 energEfficMax;
    u8 energEfficOffset;

    
    if (SENSOR_ON == pDriverManage->tuningState)
    {
        pDriverManage->SMARTEN.regBitFiled.SEIMIN = (u8)pDriverManage->currRatio;

        pDriverManage->SMARTEN.regBitFiled.SEUP = (u8)pDriverManage->currIncre;
        pDriverManage->SMARTEN.regBitFiled.SEDN = (u8)pDriverManage->currDecre;

        //�Ĵ��������޵ļ�����: SEMIN * 32
        energEfficMax = (SCALE_MAGNIFICATION - pDriverManage->energEfficMax) / TMC_SE_SCALE;

        //�Ĵ��������޵ļ�����: (SEMIN + SEMAX + 1) * 32
        energEfficOffset = (SCALE_MAGNIFICATION - pDriverManage->energEfficMax + pDriverManage->energEfficOffset) 
                           / TMC_SE_SCALE - energEfficMax - 1;

        pDriverManage->SMARTEN.regBitFiled.SEMIN = energEfficMax;
        pDriverManage->SMARTEN.regBitFiled.SEMAX = energEfficOffset;
    }
    else
    {
        pDriverManage->SMARTEN.regBitFiled.SEMIN = 0;
    }

    SMARTEN[chanNum].regValue = pDriverManage->SMARTEN.regValue;    //�����£����Ĵ���ʱҪʹ��
}
#endif



/*******************************************�ļ�β********************************************/
