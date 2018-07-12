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
stTMC26xxWarn g_stTMCWarn;   //从TMC寄存器回读得到的报警信息


#if 0
//DRVCTRL 寄存器配置
#define TMC_SET_INTPOL(data)      (g_stTMC26xxDrvConf.unCtrlStepDir.stCtrlStepDir.INTPOL = data&0x01)  //插值功能配置开关 data为0时关闭   为1时开启
#define TMC_SET_DEDGE(data)       (g_stTMC26xxDrvConf.unCtrlStepDir.stCtrlStepDir.DEDGE = data&0x01 )  //边沿配置   data为0时上升沿   为1时 双边沿
#define TMC_SET_MRES(data)        (g_stTMC26xxDrvConf.unCtrlStepDir.stCtrlStepDir.MRES = data&0x0F)  //配置细分数


//CHOPCONF 寄存器配置
#define TMC_SET_BLANK_TIME(data)         (g_stTMC26xxDrvConf.unChopConf.stChopConf.TBL = data&0x03)  //blank time 时间设置
#define TMC_SET_CHOPPER_MODE(data)       (g_stTMC26xxDrvConf.unChopConf.stChopConf.CHM = data&0x01)  //斩波模式配置   data为0时spreadcycle   为1时 恒定关断时间
#define TMC_SET_RNDTF(data)              (g_stTMC26xxDrvConf.unChopConf.stChopConf.RNDTF = data&0x01)    //关断时间模式 data为 0 固定时间    1 随机时间
#define TMC_SET_HYSTER_DEC_INTER(data)   (g_stTMC26xxDrvConf.unChopConf.stChopConf.HDEC = data&0x03)  //spreadCycle模式时 配置Hysteresis decrement period 值
#define TMC_SET_FAST_DECAY_MODE(data)    (g_stTMC26xxDrvConf.unChopConf.stChopConf.HDEC = data&0x01)  //恒定关断时间模式下,fast decay控制  
#define TMC_SET_HYSTER_END_VAL(data)     (g_stTMC26xxDrvConf.unChopConf.stChopConf.HEND = data &0x0F)  //spreadCycle模式时   hysteresis end value
#define TMC_SET_SINE_OFFSET(data)        (g_stTMC26xxDrvConf.unChopConf.stChopConf.HEND = data &0x0F)  //恒定关断时间模式下 sine wave offset
#define TMC_SET_HYSTER_START_VAL(data)   (g_stTMC26xxDrvConf.unChopConf.stChopConf.HSTRT =data &0x07)  //spreadCycle模式时  hysteresis start value
#define TMC_SET_FAST_DECAY_TIME(data)    (g_stTMC26xxDrvConf.unChopConf.u32ChopConf = g_stTMC26xxDrvConf.unChopConf.u32ChopConf&(~0x870) |(((data&0x08)<<11)|((data&0x07)<<4)))  //恒定关断时间模式下 配置fast decay时间 该时间值用 bit[11/6/5/4]来配置 
#define  TMC_SET_TOFF_TIME(data)         (g_stTMC26xxDrvConf.unChopConf.stChopConf.TOFF = data&0x0F)  // 配置关断时间

//SMARTEN 寄存器配置
#define TMC_SET_SEIMIN(data)        (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEIMIN = data&0x01)    //coolstep 最小电流
#define TMC_SET_SEDN(data)          (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEDN = data&0x03)       //电流下降速度
#define TMC_SET_SEMAX(data)         (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEMAX = data &0x0F)         //stallguard 的高阈值
#define TMC_SET_SEUP(data)          (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEUP = data&0x03)       //电流上升速度
#define  TMC_SET_SEMIN(data)        (g_stTMC26xxDrvConf.unSmarten.stSmarten.SEMIN = data&0x0F)      //stallguard的低阈值 



//SGCSCONF 寄存器配置
#define TMC_SET_SFILT(data)      (g_stTMC26xxDrvConf.unSgcsConf.stSgcsConf.SFILT = data&0x01)  //stallguard filter 使能
#define TMC_SET_SGT(data)        (g_stTMC26xxDrvConf.unSgcsConf.stSgcsConf.SGT = data&0x7F)       //stallguard 丢步阈值   该数字可能为负数  需要注意是否可以进行按位逻辑操作
#define TMC_SET_CS(data)         (g_stTMC26xxDrvConf.unSgcsConf.stSgcsConf.CS = data&0x1F)                 //电流缩放比例


//DRVCONF 寄存器配置
#define TMC_SET_TST_MODE(data)      (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.TST = data&0x01)      //测试模式  正常工作时  该位必须清零
#define TMC_SET_SLPH(data)          (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.SLPH = data&0x03)       //slope control high side 
#define TMC_SET_SLPL(data)          (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.SLPL = data&0x03)       //slope control low side
#define TMC_SET_DISS2G(data)        (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.DISS2G = data&0x01)      //接地短路保护使能  data为 0 使能  1禁用
#define TMC_SET_TS2G(data)          (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.TS2G = data&0x03)          //接地短路检测时间
#define TMC_SET_SDOFF(data)         (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.SDOFF = data&0x01)      // data为 0 使能步进/方向操作     1 SPI发送电流驱动电机运行
#define TMC_SET_VSENSE(data)        (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.VSENSE = data&0x01)      //VSENSE
#define TMC_SET_RDSEL(data)         (g_stTMC26xxDrvConf.unDrvConf.stDrvConf.RDSEL = data&0x03)          //回读状态寄存器
#endif


/*************************************局部常量和类型定义**************************************/
typedef enum 
{
    DRVACT_WRITE = 0,
    DRVACT_READ
    
}DrvConfActionEnum;



/******************************************局部变量*******************************************/
static DriverConfigUnion   DRVCONF[CH_TOTAL];
static CoolStepCofingUnion SMARTEN[CH_TOTAL];



/******************************************函数实现*******************************************/
#if 0
/*********************************************************************************************
函 数 名: drvTMC_Reg_Conf
实现功能: TMC262或2660的寄存器配置
输入参数: 
          u8 u8Reg:    寄存器地址  取值只能如下
                            ADDR_TMC_DRVCTRL    (0x00)  
                            ADDR_TMC_CHOPCONF   (0x04)  
                            ADDR_TMC_SMATREN    (0x05)  
                            ADDR_TMC_SGCSCONF   (0x06)  
                            ADDR_TMC_DRVCONF    (0x07)  
          u32 confdata:
            配置的寄存器数据,目前配置数据都是全局变量
          26XX DRVCTRL 寄存器数据
          26XX CHOPCONF 寄存器数据
          26XX SMARTEN 寄存器数据
          26XX SGCSCONF 寄存器数据
          26XX DRVCONF 寄存器数据
          u8 action:读写操作
                            TMC_DATA_WRITE   0   //要写TMC的寄存器
                            TMC_DATA_READ    1   //要读TMC的寄存器
                      
输出参数:
返 回 值: 
说    明: 配置时必须先发低位  后发高位
*********************************************************************************************/
static s32 drvTMC_Reg_Conf(u8 u8Reg,u8 action)
{
    u32 frame = 0; //配置地址位
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

    data = (frame&0xFFFF); //地址低16bit
    servFpgaRegisterWrite(0, ADDR_FPGA_DRIVER_CFG_DATA_L, data);    //FOR MODIFY NICK
    
    data = ((frame>>16)&0xFFFF); //地址高4bit
    data = (data&(~(1<<4)))|((action&0x01)<<4);  //bit4为0 表示写操作  1表示读操作
    servFpgaRegisterWrite(0, ADDR_FPGA_DRIVER_CFG_DATA_H, data);    //FOR MODIFY NICK
    
    return 0;
}

/*********************************************************************************************
函 数 名: servTMC_Reg_Write
实现功能: TMC262或2660的寄存器数据回读
输入参数:
          u8      u8Reg:    寄存器地址  取值只能如下
                            RDSEL_MICRO_POS   0x00    //回读位置
                            RDSEL_SG_LVL      0x01    //回读stallguard 级别
                            RDSEL_SG_CS_LVL   0x02    //回读stallguard和coolstep 电流
          u32* ReadData:
                     回读数据的地址
输出参数:
返 回 值:
说    明: 配置时必须先发低位  后发高位
*********************************************************************************************/
s32 servTMC_Reg_Write(u8 u8Reg)
{
    return drvTMC_Reg_Conf(u8Reg,TMC_DATA_WRITE);
}
/*********************************************************************************************
函 数 名: servTMC_Reg_Read
实现功能: TMC262或2660的寄存器数据回读
输入参数: 
          u8      u8Reg:    寄存器地址  取值只能如下
                            RDSEL_MICRO_POS   0x00    //回读位置
                            RDSEL_SG_LVL      0x01    //回读stallguard 级别
                            RDSEL_SG_CS_LVL   0x02    //回读stallguard和coolstep 电流
          u32* ReadData:
                     回读数据的地址
输出参数:
返 回 值: 
说    明: 配置时必须先发低位  后发高位
*********************************************************************************************/
s32 servTMC_Reg_Read(u8 u8Reg,u32* ReadData)
{
    TMC_SET_RDSEL(u8Reg);
    return servTmcReadSelectedReg(ReadData);
}


/*********************************************************************************************
函 数 名: drv_TMC_State_Parse
实现功能: 解析TMC内部状态
输入参数: 

输出参数:
            
返 回 值: 
说    明:
         
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
函 数 名: serv_TMC_MS_get
实现功能: 获取线圈A处在的细分表的位置
输入参数: 
            u16 *u16MStep:位置数据地址
输出参数:
            
返 回 值: 
说    明:
*********************************************************************************************/
void serv_TMC_MS_get(u16 *u16MStep)
{
    u32 u32Read = 0;
    servTMC_Reg_Read(RDSEL_MICRO_POS,&u32Read);
    *u16MStep = (u32Read>>10)&0xFFFF;
    drv_TMC_State_Parse(u32Read,g_stTMCWarn);
}

/*********************************************************************************************
函 数 名: serv_TMC_SG_get
实现功能: 获取TMC的SG数据
输入参数: 
            u16 *u16SG:SG数据地址
输出参数:
            
返 回 值: 
说    明:
*********************************************************************************************/
void serv_TMC_SG_get(u16 *u16SG)
{
    u32 u32Read = 0;
    servTMC_Reg_Read(RDSEL_SG_LVL,&u32Read);
    *u16SG = (u32Read>>10)&0xFFFF;
    drv_TMC_State_Parse(u32Read,g_stTMCWarn);
}


/*********************************************************************************************
函 数 名: serv_TMC_CoolStep_get
实现功能: 获取TMC的coolstep 缩放值数据
输入参数: 
            u16 *u16SE:SG数据地址
输出参数:
            
返 回 值: 
说    明:
*********************************************************************************************/
void serv_TMC_CoolStep_get(u16 *u16SE)
{
    u32 u32Read = 0;
    servTMC_Reg_Read(RDSEL_SG_CS_LVL,&u32Read);
    *u16SE = (u32Read>>10)&0x01F;   //bit10 11 12 13 14 位为coolstep的实际数据
    drv_TMC_State_Parse(u32Read,g_stTMCWarn);
}


void servTMC_MS_Set(MicroStepEnum MicroStep)
{
    //TMC_SET_MRES(MicroStep);   //细分配置
    //drvTMC_Reg_Conf(ADDR_TMC_DRVCTRL,TMC_DATA_WRITE);      //下发DRVCTRL配置
    serv_TMC_DRVCTRL_cfg(NCFG,NCFG,MicroStep);

}

/*********************************************************************************************
函 数 名: serv_TMC_DRVCTRL_cfg
实现功能: 配置TMC26xx 的DRVCTRL寄存器
输入参数: 
            euIntpolCtrl    intpol: 是否使能插值  目前来看 我们不适合使用插值功能
            euEdgeCtrl        edge: 触发边沿  上升沿 或者 上升-下降双沿
            euMicroStepRes  MicroStep: 细分配置
输出参数:
            
返 回 值: 
说    明:
         如果只需要配置某几个参数 则不需配置的参数都使用--NCFG
*********************************************************************************************/
void serv_TMC_DRVCTRL_cfg(euIntpolCtrl intpol,euEdgeCtrl edge,MicroStepEnum MicroStep)
{
    //DRVCTRL 配置
    if(intpol != NCFG)
    {
        TMC_SET_INTPOL(intpol); //禁用插值功能
    }

    if(edge != NCFG)
    {
        TMC_SET_DEDGE(edge);   //步进信号的上升沿有效
    }

    if(MicroStep != NCFG)
    {
         TMC_SET_MRES(MicroStep);   //细分配置
    }
    
    drvTMC_Reg_Conf(ADDR_TMC_DRVCTRL,TMC_DATA_WRITE);        //下发DRVCTRL配置
}
/*********************************************************************************************
函 数 名: serv_TMC_CHOPCONF_Mode_Time
实现功能: 配置TMC26xx CHOPCONF寄存器 模式和时间相关位
输入参数: 
            euChopMode mode: 斩波模式  恒定关断时间快速衰减或者 spreadcycle
            euBlankTIme BlkTime:    blank time
            euRNDTFMode RandomCtrl:
 关断时间 模式  恒定或者随机
            euChopOffTime offtime:  慢衰减时间
输出参数:
            
返 回 值: 
说    明:时间参数对于 恒定关断时间快速衰减模式和spreadcycle模式都是通用的
         如果只需要配置某几个参数 则不需配置的参数都使用--NCFG
*********************************************************************************************/
void serv_TMC_CHOPCONF_Mode_Time(euChopMode mode,euBlankTIme BlkTime,euRNDTFMode RandomCtrl,euChopOffTime offtime)
{
    if(NCFG != mode)
    {
         TMC_SET_CHOPPER_MODE(mode);//使用spreadcycle 斩波模式
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
函 数 名: serv_TMC_CHOPCONF_Hyst
实现功能: 配置TMC26xx CHOPCONF 迟滞相关参数的设置
输入参数: 
          euHSTRT start:迟滞起始值
          euHEND    end:迟滞终止值
          euHDEC    dec:迟滞衰减周期
输出参数:
            
返 回 值: 
说    明:
         如果只需要配置某几个参数 则不需配置的参数都使用--NCFG
*********************************************************************************************/
void serv_TMC_CHOPCONF_Hyst(euHSTRT start,euHEND end,euHDEC dec)
{
    if(NCFG != start)
    {
         TMC_SET_HYSTER_START_VAL(start);   //HSTRT设置
    
    }
    if(NCFG != end)
    {
         TMC_SET_HYSTER_END_VAL(end);     //HEND设置
    }
    if(NCFG != dec)
    {
         TMC_SET_HYSTER_DEC_INTER(dec);
    }
    drvTMC_Reg_Conf(ADDR_TMC_CHOPCONF,TMC_DATA_WRITE);
}

/*********************************************************************************************
函 数 名: serv_TMC_CHOPCONF_Const_para
实现功能: 配置TMC26xx CHOPCNOF在constant offtime and fast decay time模式下相关参数
输入参数: 
            euFastDecayMode   FDMode:快速衰减停止的条件 只允许计时器停止 或者 电压比较器也可以停止
            euSineOffset  SineOffset:正弦波表偏移
            euFDTime          fdtime:快速衰减时间设置 单位为时钟周期
输出参数:
            
返 回 值: 
说    明:
         如果只需要配置某几个参数 则不需配置的参数都使用--NCFG
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
函 数 名: serv_TMC_SMARTEN_cfg
实现功能: 配置TMC26XX
输入参数: 
        euMinCS mincs:         coolstep时最小的电流值
        euDownSpeed DnSpeed:   电流下降前检测到的SG超限次数
        euUpSize Upsize:       SG超下限后电流每次上升的级别
        euSEMin semin:         coolstep时 SG下限=semin*32, semin=0表示关闭coolstep
        euSEMax semax:         coolstep时 SG上限=(semax+semin+1)*32 
输出参数:
            
返 回 值: 
说    明:
         如果只需要配置某几个参数 则不需配置的参数都使用--NCFG
*********************************************************************************************/
void serv_TMC_SMARTEN_cfg(euMinCS mincs,euDownSpeed DnSpeed,euUpSize Upsize,euSEMin semin,euSEMax semax)
{
    if(NCFG != mincs)
    {
         TMC_SET_SEIMIN(mincs);  //最小电流值配置 
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
函 数 名: serv_TMC_SGCSCONF_cfg
实现功能: 配置TMC26xxSGCSCONF寄存器
输入参数: 
            euSGfiltCtrl sfilt:是否启用stallguard滤波
            SGTValue       sgt:stallguard调节
            CSValue         cs: 电流缩放系数
输出参数:
            
返 回 值: 
说    明:
         如果只需要配置某几个参数 则不需配置的参数都使用--NCFG
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
函 数 名: serv_TMC_DRVCONF_Slope_Sense
实现功能: 配置TMC26xxDRVCONF寄存器斜率和感应电阻配置
输入参数: 
            euSlopeL     LSlope:高侧斜率
            euSlopeH     HSlope:低侧斜率
            euVsenseCtrl vsense:感应电阻幅值控制  会影响最大输出电流
输出参数:
            
返 回 值: 
说    明:
         如果只需要配置某几个参数 则不需配置的参数都使用--NCFG
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
函 数 名: serv_TMCDRVCONF_S2G_cfg
实现功能: 配置TMC26xxDRVCONF接地短路相关设置
输入参数: 
            euS2GCtrl mode:是否启动短路保护
            euS2GTime time:短路检测时间

输出参数:
            
返 回 值: 
说    明:
         如果只需要配置某几个参数 则不需配置的参数都使用--NCFG
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
函 数 名: servTMCStatusMonitor
实现功能: 监控TMC芯片的运行状态
输入参数: 
            
输出参数:
            
返 回 值: 
说    明:目前判断TMC状态正常的依据为 
        1.SG数据正常        否则  bit0 = 1
        2.S2GA S2GB都为0    否则  bit1 = 1
        3.OTPW OT 都为0     否则  bit2 = 1
        4.微步数据正常      否则  bit3 = 1
*********************************************************************************************/
u16 servTMCStatusMonitor(void)
{
    static u16 u16LastMStep = 0;  //上次记录到的微步数
    static u8 u8SGZeroCount = 0;  //SG为0次数记录
    static bool bSGMonitorValid = false;  //SG有效监控标志
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

    if(ST == 0)//非静止状态
    {
        u16LastMStep = 0;
    }
    else
    {
        if((u16LastMStep != 0)&&(u16CurrentMStep == 0)) //上次静止状态微步数据不为0且本次微步数据为0 表示在静止状态下微步数据出现了突变
        {
            error |= 0x08;  //微步错误
        }
        u16LastMStep = u16CurrentMStep;
    }

    serv_TMC_SG_get(&u16SG);

    if(u16SG != 0)  //sg数据不为0 表示工作正常
    {
        bSGMonitorValid = true;
        u8SGZeroCount = 0;
    }
    else
    {
        u8SGZeroCount++;
        if((bSGMonitorValid == true)&&(u8SGZeroCount > 3))//连续出现3次sg为0 则为错误
        {
            error |= 0x01;  //SG错误
            bSGMonitorValid = false; //检测到sg为0后 如果采取了重配但不进行运动  sg的数值依然为0  所以此时不必再报错 
        }
    }


    if((S2GA)||(S2GB)) 
    {
        error |= 0x02; //短路错误
    }
    
    if(OT||OTPW)
    {
        error |= 0x04; //过温错误
    }
    
    return error;
}
#endif


#if 0
#endif


/*********************************************************************************************
函 数 名: servDriverRegConfig;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
s32 servDriverRegConfig(u8 regNum, u32 regVaule, DrvConfActionEnum drvConfAct)
{
    u16 data;

    data = (regVaule & 0xFFFF);    //低16bit
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_OPERT_L_REG, data);
    
    data = ((regVaule >> 16)& 0xFFFF);    //高4bit
    
    data = (data & SERV_FPGA_DRV_OPERT_VALUE_H_MASK) | 
           ((drvConfAct << SERV_FPGA_DRV_OPERT_ACT_LSHIFT) & SERV_FPGA_DRV_OPERT_ACT_MASK);    //方向
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_OPERT_H_REG, data);
    
    return 0;
}


/*********************************************************************************************
函 数 名: servDriverRegWrite;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
s32 servDriverRegWrite(u8 chanNum, u8 regNum, u32 regValue)
{
    servFpgaDriverSelect(chanNum);
    servDriverRegConfig(regNum, regValue, DRVACT_WRITE);

    if (ADDR_TMC_DRVCONF == chanNum)
    {
        DRVCONF[chanNum].regValue = regValue;    //备份下，读寄存器时要使用
    }

    if (ADDR_TMC_SMARTEN == chanNum)
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
说     明: 整个监测根据监测内容的开关情况选择读取寄存器并计算。监测内容包括能耗，SGALL和SGSE
*********************************************************************************************/
void servDriverStateMonitor(u8 chanNum, DriverMonitorStruct driverMonitor)
{
    DriverReadRespUnion readResp;
    u16 error = 0;
    s8  sgPercent;
    u8  sePercent;

    static u16  lastMStep[CH_TOTAL]    = {0};  //上次记录到的微步数
    static u8   sgZeroCount[CH_TOTAL]  = {0};  //SG为0次数记录
    static bool bSgMontValid[CH_TOTAL] = {false};  //SG有效监控标志
    static u8   bCsDisable[CH_TOTAL]   = {0};
    static u8   seminBackup[CH_TOTAL]  = {0}; 


    servDriverStateRegRead(chanNum, READREG_R0, &readResp.regValue);
    
    if (readResp.readRespOne.STST == 0)    //非静止状态
    {
        lastMStep[chanNum] = 0;
    }
    else
    {
        //上次静止状态微步数据不为0且本次微步数据为0 表示在静止状态下微步数据出现了突变
        if ((lastMStep[chanNum] != 0) && (readResp.readRespOne.MSTEP == 0)) 
        {
            error |= 0x08;  //微步错误
        }
        lastMStep[chanNum] = readResp.readRespOne.MSTEP;
    }
    
    if (readResp.readRespOne.S2GA || readResp.readRespOne.S2GB) 
    {
        error |= 0x02; //短路错误
    }
    
    if (readResp.readRespOne.OT || readResp.readRespOne.OTPW)
    {
        error |= 0x04; //过温错误
    }

    //监测能耗/SGSE监控
    if (((driverMonitor.energyValue != NULL) && (SENSOR_ON == driverMonitor.energyState)) ||
        ((driverMonitor.sgseValue   != NULL) && (SENSOR_ON == driverMonitor.sgseState)))
    {
        servDriverStateRegRead(chanNum, READREG_R2, &readResp.regValue);

        if ((driverMonitor.energyValue != NULL) && (SENSOR_ON == driverMonitor.energyState))
        {
            //sgPercent放大了100倍，所以乘100；SGH是高5位，所以需要乘32，然后再放大一百倍就是3200.0了
            sgPercent = (s8)(100 - (f32)readResp.readRespThree.SGH * 3200.0f / driverMonitor.sgZero);
            sePercent = (u8)(readResp.readRespThree.SE * 100.0f / driverMonitor.csValue);

            *driverMonitor.energyValue = (driverMonitor.sgZero << 16) | ((u8)sgPercent << 8) | sePercent;

            if (sgPercent >= driverMonitor.sgUpLimit)
            {
                error |= 0x10;    //SG错误
            }
            else if (sgPercent <= driverMonitor.sgDnLimit)
            {
                error |= 0x20;    //SG错误
            }

            if (sgPercent < 0.0f)    //小于0表示有外部推力 可能超步 需要人为加大电流
            {
                if (!bCsDisable[chanNum])    //没关闭coolStep则关闭一次
                {
                    seminBackup[chanNum] = SMARTEN[chanNum].regBitFiled.SEMIN;
                    SMARTEN[chanNum].regBitFiled.SEMIN = 0;    //关闭coolStep，电流保持在CS上

                    servDriverRegWrite(chanNum, ADDR_TMC_SMARTEN, SMARTEN[chanNum].regValue);

                    bCsDisable[chanNum] = true;
                }
            }
            else    //超步状况解除后需要恢复coolstep
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
            if (readResp.readRespThree.SGH != 0)    //sg数据不为0 表示工作正常
            {
                bSgMontValid[chanNum] = true;
                sgZeroCount[chanNum]  = 0;
            }
            else
            {
                sgZeroCount[chanNum]++;
                if ((bSgMontValid[chanNum]) && (sgZeroCount[chanNum] > 3))    //连续出现3次sg为0 则为错误
                {
                    error |= 0x01;    //SG错误
                    bSgMontValid[chanNum] = false;    //检测到sg为0后 如果采取了重配但不进行运动  sg的数值依然为0  所以此时不必再报错 
                }
            }
        }
    
        readResp.readRespThree.resv_bit20_31 = error;    //高12位放错误码

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
    
        readResp.readRespTwo.resv_bit20_31 = error;    //高12位放错误码
        *driverMonitor.sgAllValue = readResp.regValue;
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
        servDriverRegConfig(ADDR_TMC_DRVCONF, DRVCONF[chanNum].regValue, DRVACT_WRITE);
        bspDelayUs(50);
        
        servDriverRegConfig(ADDR_TMC_DRVCONF, DRVCONF[chanNum].regValue, DRVACT_READ);
        bspDelayUs(50);

        //先读高，然后判断通道号是否一致 
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
函 数 名: servDriverCurrRegParaCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void servDriverCurrRegParaCalc(DriverTypeEnum type, u8 curr, u8 *vsense, u8 *csValue)
{
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
}


/*********************************************************************************************
函 数 名: servDriverCurrCalc;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 根据CS和VSENSE计算电流;
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
函 数 名: servDriverCurrentSet;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
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
函 数 名: servDriverConfig;
实现功能: 无; 
输入参数: 无;
输出参数: 无;
返 回 值: 无;
说     明: 无;
*********************************************************************************************/
void servDriverConfig(u8 chanNum, DriverManageStruct driverManage)
{
    u8 vsense;
    u8 csValue;

    
    servFpgaDriverCofing(chanNum, driverManage.type, DRVSTATE_ON, DRVCLK_OPEN);    //初始化逻辑对TMC控制 
    
    //servDriverCtrlEnable(true);
    //servFpgaDriverSelect(chanNum);
    
    if (driverManage.type < DRIVER_UNKNOW)
    {
        servDriverCurrRegParaCalc(driverManage.type, driverManage.curr, &vsense, &csValue);
        driverManage.DRVCONF.regBitFiled.VSENSE = vsense;
        driverManage.SGCSCONF.regBitFiled.CS    = csValue;
        
        //细分、边沿模式、内部插值控制
        servDriverRegWrite(chanNum, ADDR_TMC_DRVCTRL, driverManage.DRVCTRL.regValue);
        bspDelayUs(200);
        
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
说     明: 无;
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



/*******************************************文件尾********************************************/
