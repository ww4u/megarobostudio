/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名: servDriver.c；
功能描述:     ;
函数列表:     ;
当前版本:     1.0.0;
版本改动:     ;
作     者:  CJ;
完成日期:     2017.10.25;
历史版本:     无;
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



/****************************************外部变量声明*****************************************/
extern MotorInfoStruct   g_motorInfo; 
extern SystemStateStruct g_systemState;
extern DriverInfoStruct  g_driverInfo;



/*****************************************局部宏定义******************************************/
#if GELGOOG_AXIS_10
#define    DRIVER_SENSOR_REST     7.556    //(current / 10) * 0.34Ω* 100 / 0.45V = current * 7.556
#define    DRIVER_DUTY_MAX        100

#else

#if QUBELEY_HARDVER_1
#define    TMC_CURR_BOUNDARY      40       //单位100mA
#define    R_SENSE_262            30       //单位mΩ
#else
#define    TMC_CURR_BOUNDARY      40       //单位100mA
#define    R_SENSE_262            40       //单位mΩ
#endif

#define    VSENSE_NOT_SET_VOLT    310      //单位mV
#define    VSENSE_SET_VOLT        165      //单位mV
#define    TMC_CS_VALUE_MAX       31       //寄存器值
#define    TMC_SE_SCALE           32       //寄存器值
#endif



/*************************************局部常量和类型定义**************************************/
#if !GELGOOG_AXIS_10

typedef enum
{
    VENSE_NOT = 0,    //设置VENSE的值为0，选择量程305mV
    VENSE_SET = 1     //设置VENSE的值为1，选择量程165mV
    
}VsenseCtrlEnum;

#endif


/******************************************局部变量*******************************************/
#if !GELGOOG_AXIS_10
static DriverConfigUnion   DRVCONF[CH_TOTAL];
static CoolStepCofingUnion SMARTEN[CH_TOTAL];
#endif



/******************************************函数实现*******************************************/
#if GELGOOG_AXIS_10
/*********************************************************************************************
函 数 名: servDriverCurrentConfig;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servDriverCurrentConfig(u8 current)
{
    u8  duty;
    u32 frequency;

    
    frequency = 1000;    //1KHz
    
    //duty = (current / 10) * 0.34Ω* 100 / 0.5V
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
函 数 名: servDriverConfig;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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

    //配置电流
    servDriverCurrentConfig(driverInfo.curr);

    for (i = 0;i < CH_TOTAL;i++)
    {
        servFpgaDriverEnable(i, driverInfo.state[i]);
    }
}


/*********************************************************************************************
函 数 名: servDriverSwitch;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: servDriverRegWrite;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
s32 servDriverRegWrite(u8 chanNum, u8 regNum, u32 regValue)
{
    servFpgaDriverSelect(chanNum);
    servFpgaDriverConfig(regNum, regValue, DRVACT_WRITE);

    if (ADDR_TMC_DRVCONF == regNum)
    {
        DRVCONF[chanNum].regValue = regValue;    //备份下，读寄存器时要使用
    }

    if (ADDR_TMC_SMARTEN == regNum)
    {
        SMARTEN[chanNum].regValue = regValue;    //备份下，读寄存器时要使用
    }
    
    return 0;
}


/*********************************************************************************************
函 数 名: servDriverStateMonitor
实现功能: 监控TMC芯片的运行状态
输入参数:            
输出参数:          
返 回 值: 
说    明: 整个监测根据监测内容的开关情况选择读取寄存器并计算。监测内容包括能耗，SGALL和SGSE
*********************************************************************************************/
void servDriverStateMonitor(u8 chanNum, DriverMonitorStruct driverMonitor)
{
    DriverReadRespUnion readResp;
    u8  state;
    s8  sgPercent;
    u8  sePercent;
    

    //监测能耗/SGSE监控
    if ((driverMonitor.energyValue != NULL) && (driverMonitor.sgseValue != NULL))
    {
        servDriverStateRegRead(chanNum, READREG_R2, &readResp.regValue);

        state = readResp.regValue & 0x0000000F;    //低8位是状态位
        
        //sgPercent放大了100倍，所以乘100；SGH是高5位，所以需要乘32，然后再比上1000就是3.2了
        sgPercent = (s8)(100 - (f32)readResp.readResp2.SGH * 3.2f);
        sePercent = (u8)(readResp.readResp2.SE * 100.0f / driverMonitor.csValue);

        *driverMonitor.energyValue = (state << 16) | ((u8)sgPercent << 8) | sePercent;
    }
}


/*********************************************************************************************
函 数 名: servDriverCtrlEnable;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: servDriverStateRegRead;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
u8 servDriverStateRegRead(u8 chanNum, DriverReadRespEnum readReg, u32 *regValue)
{
    u16 tempReg;
    u8  realChan;
    

    if (regValue != NULL)
    {
        //读的流程: 1. 选通道（PROJECT_GELGOOG）; 
        //          2. 写DRVCONF寄存器的RDSEL位域为要读取的寄存器，
        //          3. 配置FPGA的DRIVER SPI为读操作
        //          4. 读寄存器
        servFpgaDriverSelect(chanNum);
        
        DRVCONF[chanNum].regBitFiled.RDSEL = readReg;    //选择读寄存器
        servFpgaDriverConfig(ADDR_TMC_DRVCONF, DRVCONF[chanNum].regValue, DRVACT_WRITE);
        bspDelayUs(50);
        
        servFpgaDriverConfig(ADDR_TMC_DRVCONF, DRVCONF[chanNum].regValue, DRVACT_READ);
        bspDelayUs(50);

        //先读高，然后判断通道号是否一致 
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
函 数 名: servDriverCurrentSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servDriverCurrentSet(u8 chanNum, DriverManageStruct driverManage)
{
    //配置VSENSE和CS
    servDriverRegWrite(chanNum, ADDR_TMC_SGCSCONF, driverManage.SGCSCONF.regValue);    //CS
    bspDelayUs(200);
    servDriverRegWrite(chanNum, ADDR_TMC_DRVCONF, driverManage.DRVCONF.regValue);      //VSENSE
    
    DRVCONF[chanNum].regValue = driverManage.DRVCONF.regValue;    //备份下，读寄存器时要使用
}


/*********************************************************************************************
函 数 名: servDriverMicroStepSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void servDriverMicroStepSet(u8 chanNum, DriverManageStruct driverManage)
{
    //配置微步数
    servDriverRegWrite(chanNum, ADDR_TMC_DRVCTRL, driverManage.DRVCTRL.regValue);
}


/*********************************************************************************************
函 数 名: servDriverCurrRegParaCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servDriverCurrRegParaCalc(DriverTypeEnum type, u8 curr, u8 *vsense, u8 *csValue)
{
#if 1    //CJ 2018.08.21 Modify

    u8 csCalcValue;

    
    //由于寄存器中CS只有5bit，所以使用任何一个量程进行电流配置时都会产生比较大的误差(分辨率不够)
    //165mV量程的分辨率是0.129A，310mV量程的分辨率是0.242A

    //根据对寄存器的演算，4A及4A以下使用165mv量程进行配置所获得的电流值比310mV量程更接近目标值
    //虽然4A及4A以下使用165mv量程进行配置，个别电流点的误差比310mV量程大，但是差别比较小所以为
    //了程序方便，4A及4A以下就只使用165mv量程进行配置了。
    
    //全量程中误差最大的点是0.1A，实际值0.1289A，误差28.9%，误差最小的点是4.6A，实际值4.6016A，误差0.034%

    //先不考虑2660    TO ADD NICK
    if (DRIVER_262 == type)
    {
        //4A及4A以下
        if (curr <= TMC_CURR_BOUNDARY)
        {
            *vsense = VENSE_SET;

            //Ipeak = (CS + 1) / 32 * Vfs / Rsense
            //CS = Iset * Rsense / Vfs * 32 - 1
            //CS = Iset * Rsense * 32 / Vfs - 0.5 ---- 四舍五入
            csCalcValue = (u8)(curr * R_SENSE_262 * 32.0f / (VSENSE_SET_VOLT * 10) - 0.5);
        }
        else
        {
            *vsense = VENSE_NOT;

            //Ipeak = (CS + 1) / 32 * Vfs / Rsense
            //CS = Iset * Rsense / Vfs * 32 - 1
            //CS = Iset * Rsense * 32 / Vfs - 0.5 ---- 四舍五入
            csCalcValue = (u8)(curr * R_SENSE_262 * 32.0f / (VSENSE_NOT_SET_VOLT * 10) - 0.5);
        }

        if (csCalcValue > TMC_CS_VALUE_MAX)
        {
            csCalcValue = TMC_CS_VALUE_MAX;
        }

        *csValue = csCalcValue;
    }
    
#else
    float f32TarCurr = ((float)curr)*0.1;    //目标电流
    float f32Curr_165 = 0;    
    float f32Curr_305 = 0;
    euVsenseCtrl euVol = VENSE_165MV;
    u8 u8CS = 12;

    if (DRIVER_2660 == type)
    {
        f32Curr_165 = (float)165 / R_SENSE_2660;    //165毫伏时的最大电流
        f32Curr_305 = (float)305 / R_SENSE_2660;    //305毫伏时的最大电流
        
        if (f32TarCurr > CURR_MAX_2660)
        {
            //上报错误
            return;
        }
    }
    else
    {
        f32Curr_165 = (float)165/R_SENSE_262;    //165毫伏时的最大电流
        f32Curr_305 = (float)305/R_SENSE_262;    //305毫伏时的最大电流  

        if (f32TarCurr > CURR_MAX_262)
        {
            //上报错误
            return;
        }
    }

    //先确定电压值
    if (f32TarCurr <= f32Curr_305/2)  //目标电流达不到305毫伏时的一半则使用165毫伏配置
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
函 数 名: servDriverCurrCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 根据CS和VSENSE计算电流;
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
函 数 名: servDriverConfig;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
*********************************************************************************************/
void servDriverConfig(u8 chanNum, DriverManageStruct driverManage)
{
    u8 vsense;
    u8 csValue;

    
    if (driverManage.type < DRIVER_UNKNOW)
    {
        //按照空闲电流进行配置
        //servDriverCurrRegParaCalc(driverManage.type, driverManage.curr, &vsense, &csValue);
        servDriverCurrRegParaCalc(driverManage.type, driverManage.idleCurr, &vsense, &csValue);
        
        driverManage.DRVCONF.regBitFiled.VSENSE = vsense;
        driverManage.SGCSCONF.regBitFiled.CS    = csValue;
        
        //细分、边沿模式、内部插值控制
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
        DRVCONF[chanNum].regValue = driverManage.DRVCONF.regValue;    //备份下，读寄存器时要使用
        bspDelayUs(200);
        
        servDriverRegWrite(chanNum, ADDR_TMC_SMARTEN, driverManage.SMARTEN.regValue);
        SMARTEN[chanNum].regValue = driverManage.SMARTEN.regValue;    //备份下，读寄存器时要使用
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
函 数 名: servDriverSwitch;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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
函 数 名: servDriverTuningSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说    明: 无;
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

        //寄存器中下限的计算是: SEMIN * 32
        energEfficMax = (SCALE_MAGNIFICATION - pDriverManage->energEfficMax) / TMC_SE_SCALE;

        //寄存器中上限的计算是: (SEMIN + SEMAX + 1) * 32
        energEfficOffset = (SCALE_MAGNIFICATION - pDriverManage->energEfficMax + pDriverManage->energEfficOffset) 
                           / TMC_SE_SCALE - energEfficMax - 1;

        pDriverManage->SMARTEN.regBitFiled.SEMIN = energEfficMax;
        pDriverManage->SMARTEN.regBitFiled.SEMAX = energEfficOffset;
    }
    else
    {
        pDriverManage->SMARTEN.regBitFiled.SEMIN = 0;
    }

    SMARTEN[chanNum].regValue = pDriverManage->SMARTEN.regValue;    //备份下，读寄存器时要使用
}
#endif



/*******************************************文件尾********************************************/
