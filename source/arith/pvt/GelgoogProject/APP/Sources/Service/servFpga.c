/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  servFpga.c��
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  ��;
�������:  ��;
��ʷ�汾:  ��;
*********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "project.h"
#include "servFPGA.h"
#include "servSoftTimer.h"
#include "bspSpi.h"
#include "bspSdio.h"
#include "bspGpio.h"
#include "bspTimer.h"
#include "bspFlash.h"
#include "comMemoryMap.h"
#include "servCanBuildFrame.h"



/****************************************�ⲿ��������*****************************************/
extern MotionInfoStruct  g_motionInfo;



/*****************************************�ֲ��궨��******************************************/
#define    FPGA_ADD_LEN_2BYTE      2       //���üĴ���ʱ ����ǰ�ӵĵ�ַ����Ϊ 2�ֽ�
#define    FPGA_REG_LEN_BYTE       2       //�Ĵ�������

#define    FPGA_RREG_ADDR_MARK     0x1F    //���Ĵ�����ַ���� bit[0:4]
#define    FPGA_RCHAN_NUM_MARK     0x1E0   //��ͨ�������� bit[5:8]
#define    FPGA_RCHAN_NUM_LSHIFT   5       //��ͨ�������� bit[5:8]

#define    FPGA_WREG_ADDR_MARK     0x3F    //д�Ĵ�����ַ���� bit[0:5]
#define    FPGA_WCHAN_NUM_MARK     0x07    //дͨ�������� bit[8:10]���ڶ���Byte

#define    FPGA_CFG_BUFF_SIZE      2048

#define    FPGA_ARM_COUNT_ENCODER  1       //������������ARM����

#define    FPGA_DDR_WAVE_TABLE_POINT_LEN    4    //Bytes



/*************************************�ֲ����������Ͷ���**************************************/
typedef enum 
{
    FPGAACT_READ = 0,
    FPGAACT_WRITE
    
}FpgaActionEnum;

typedef struct
{
    u16 driverCtrlReg;                 //�������ƼĴ��� 16#
    
    u16 ultrasonicCtrlReg;             //�������ƼĴ��� 19#

    u16 waveTableModeReg[CH_TOTAL];    //����ģʽ�Ĵ��� 31#����������Դ��ʱ��У��
    u16 waveTableCtrlReg[CH_TOTAL];    //������ƼĴ��� 32#

    u16 encMultandCountReg[CH_TOTAL];  //���������˺ͼ�������Ĵ��� 39#
    
    u16 digitalOutputReg;              //DO���üĴ��� 45#
    u16 encandUartReg;                 //��������UART���üĴ��� 46#
    
    u16 trigRelatRefCfgReg[CH_TOTAL];  //�����߼���ϵ��REF���üĴ��� 49#
    
    u16 interruptSrcReg[CH_TOTAL];     //�ж�Դ�Ĵ��� 50#
    
    u16 sensorUart2SelectReg;          //Uartѡ��Ĵ��� 51#
    
    
}FpgaWriteableRegValueStruct;



/******************************************�ֲ�����*******************************************/
FpgaWriteableRegValueStruct fpgaRegValue = {0};



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: servFpgaDataLenSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
static void servFpgaDataLenSet(FpgaActionEnum fpgaAction, u16 realLen, u16 dataLen)
{
    if (FPGAACT_WRITE == fpgaAction)
    {
        if (realLen != dataLen)
        {
            bspSdioCmdSend(CMD_FPGA_W_D_2_LEN, dataLen - 2, false);    //��2�ͼ�1����FPGA�ӿ�Ҫ��
            bspSdioCmdSend(CMD_FPGA_W_D_3_LEN, realLen - 1, false);
        }
        else
        {
            bspSdioCmdSend(CMD_FPGA_W_D_LEN, dataLen - 2, false);
        }
    }
    else if (FPGAACT_READ == fpgaAction)
    {
        bspSdioCmdSend(CMD_FPGA_R_D_LEN, dataLen - 2, false);
    }   
}


/*********************************************************************************************
�� �� ��: servFpgaReadAddrSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
static void servFpgaReadAddrSet(u16 readAddr)
{
    bspSdioCmdSend(CMD_FPGA_REG_ADDR, readAddr, true);
}


/*********************************************************************************************
�� �� ��: servFpgaRegisterWrite
ʵ�ֹ���: дFpga�Ĵ���
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
u8 servFpgaRegisterWrite(u8 chanNum, u16 regAddr, u16 regValue)
{
    u8 regLen = FPGA_ADD_LEN_2BYTE + FPGA_REG_LEN_BYTE;
    u8 regData[32];
    

    //�������ݳ���
    servFpgaDataLenSet(FPGAACT_WRITE, regLen, regLen);
    
    regData[0] = regAddr & FPGA_WREG_ADDR_MARK;
    regData[1] = chanNum & FPGA_WCHAN_NUM_MARK;
    regData[2] = (u8)(regValue & 0xFF);    //��8λ
    regData[3] = (u8)((regValue & 0xFF00) >> 8);    //��8λ

    //��������
    bspSdioDataSend(regData, regLen, SDIO_BLK_4_Byte);

    return 0;
}


/*********************************************************************************************
�� �� ��: servFpgaRegisterRead
ʵ�ֹ���: ��Fpga�Ĵ���
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
u8 servFpgaRegisterRead(u8 chanNum, u16 regAddr, u16 *pRegValue)
{
    u8  regLen = FPGA_ADD_LEN_2BYTE + FPGA_REG_LEN_BYTE;
    u8  regValue[4] = {0};
    u16 tempReg;
    

    if (pRegValue != NULL)
    {
        //����DMA
        bspSdioDataReceive(regValue, regLen, SDIO_BLK_4_Byte);
        
        //�������ݳ���
        servFpgaDataLenSet(FPGAACT_READ, regLen, regLen);

        //���ö�ȡ���߼��Ĵ�����ַ
        tempReg = (regAddr & FPGA_RREG_ADDR_MARK) | ((chanNum << FPGA_RCHAN_NUM_LSHIFT) & FPGA_RCHAN_NUM_MARK);
        servFpgaReadAddrSet(tempReg);

        //ǰ�����ֽ��ǵ�ַ
        *pRegValue  = regValue[2];         //��8λ 
        *pRegValue |= regValue[3] << 8;    //��8λ 
    }
  
    return 0;
}


/*********************************************************************************************
�� �� ��: servFpgaProgStart;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��������������ARM��CONFIG�ܽŷ���һ���½��أ� 
          FPGA���յ��½��غ�ὫANSWER�ܽ�������ΪӦ��
          Ȼ��ARM���Խ��������·�, ��ʵ��Ӧ��ʱ���Բ���
          �ȴ�PC3��Ӧ���ź�,������ʱ�󼴿ɿ�ʼ�����·�
*********************************************************************************************/
static bool servFpgaProgStart(void)
{
    u8 i = 0;

    
    //����һ���½���
    bspGpioFpgaProgConfigSet(Bit_SET);
    bspDelayUs(10);
    bspGpioFpgaProgConfigSet(Bit_RESET);
    bspDelayUs(10);    
    bspGpioFpgaProgConfigSet(Bit_SET);

    //�ȴ�FPGA�ظ�
    while (bspGpioFpgaProgAnswerRead())
    {
        i++;
        bspDelayUs(50);
        
        if (i > 5)
        {
            return false;
        } 
    }
    
    return true;
}


/*********************************************************************************************
�� �� ��: servFpgaProgDone;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
static bool servFpgaProgDone(void)
{
    u8 i = 0;
    
    while (i < 50)
    {
        if (!bspGpioFpgaProgDoneRead())
        {
            bspDelayUs(200);
        }
        else
        {
            return true;
        }
        
        i++;
    }
    
    return false;
}


/*********************************************************************************************
�� �� ��: servFpgaLoaclFileWrite
ʵ�ֹ���: �������ļ�д�뵽FPGA��
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
u8 servFpgaLoaclFileWrite(u32 writeAddr, char const* filePath)
{
    u8 *buff = NULL;
    s32 fileSize; 
    u32 writeLen;
    u8  sizeLen = sizeof(fileSize);

    
    FILE * pfile = fopen((char const*)filePath, "rb");
    bspDelayUs(20);
    if (pfile == NULL)
    {
        fclose(pfile);      //�ر��ļ�
        
        return 1;
    }
    
    fseek(pfile, 0L, SEEK_END);    //��λ���ļ�ĩβ
    fileSize = ftell(pfile);       //�õ��ļ���С
    fseek(pfile, 0L, SEEK_SET);    //��λ���ļ�ͷ
    
    buff = (u8 *)malloc(FPGA_CFG_BUFF_SIZE);    //�����ڴ�
    if (buff != NULL)
    {
#ifdef PROJECT_GELGOOG
        //����FPGA�ļ���ռ�Ŀռ�
        bspFlashSectorErase(FLASH_FPGA_CODE_ADDR, FLASH_FPGA_CODE_ADDR + FLASH_FPGA_CODE_LEN);
#endif

        memcpy(buff, &fileSize, 4);    //ǰ4���ֽ�Ϊ�߼��ļ�����
        fread(buff + sizeLen, 1, FPGA_CFG_BUFF_SIZE - sizeLen, pfile);    //��ȡ����,�߼����ݿ϶�����2048�ֽ�
        
        bspFlashWrite(writeAddr, buff, FPGA_CFG_BUFF_SIZE);    
        fileSize -= FPGA_CFG_BUFF_SIZE - sizeLen;
        writeAddr += FPGA_CFG_BUFF_SIZE; 
            
        do
        {
            if (fileSize >= FPGA_CFG_BUFF_SIZE)
            {
                 writeLen = FPGA_CFG_BUFF_SIZE;   
            }
            else
            {
                 writeLen = fileSize;
            }
            fread(buff, 1, writeLen, pfile); //��ȡһ������
            
            bspFlashWrite(writeAddr, buff, writeLen);
            bspDelayMs(10);
            
            fileSize  -= writeLen;
            writeAddr += writeLen;
            
        }while (fileSize > 0);

        free(buff);       //�ͷ��ڴ�
        fclose(pfile);

        return 0;
    }
    else
    {
        return 4;
    }
}


/*********************************************************************************************
�� �� ��: servFpgaLoaclFileLoad
ʵ�ֹ���: �ӱ��ؼ����ļ���FPGA��
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
u8 servFpgaLoaclFileLoad(char const* filePath)
{
    u8 tryCountMax = 3;    //�����3��
    u8 tryCount = 0;
    u8 errorCode = 0;
    
    s32 fileSize;
    u8 *buff = NULL;
    u32 dataSize;
    u32 readover = 0;


    while (1)
    {
        if (!servFpgaProgStart())   //�����������ɹ�
        {
            tryCount++;
            bspDelayMs(1);

            if (tryCount >= tryCountMax)
            {
                return 1;
            }
        }
        else
        {
            break;
        }
    }
    
    FILE * pfile = fopen(filePath, "rb");    //�򿪱����ļ�
    bspDelayUs(20);
    if (pfile == NULL)
    {
        fclose(pfile);
        return 2;
    }
    
    fseek(pfile, 0L, SEEK_END);    //��λ���ļ�ĩβ
    fileSize = ftell(pfile);       //�õ��ļ���С

    buff = (u8 *)malloc(FPGA_CFG_BUFF_SIZE);    //�����ڴ�
    if (buff != NULL)
    {
        tryCount = 0;
        while (1)
        {
            for (readover = 0;readover < fileSize;)
            {
                fseek(pfile, readover, SEEK_SET);    //��λ���ϴζ�����λ��
                
                if ((fileSize - readover) >= FPGA_CFG_BUFF_SIZE)
                {
                    dataSize = FPGA_CFG_BUFF_SIZE;
                }
                else
                {
                    dataSize = fileSize - readover;
                }
                fread(buff, 1, dataSize, pfile);    //��ȡһ������
                readover += dataSize;               //ƫ��
                
                bspFpSpiSend(buff, dataSize);
            }
        
            if (servFpgaProgDone())    //���óɹ����˳�ѭ��
            {
                break;
            }
            else
            {
                tryCount++;

                if (tryCount >= tryCountMax)
                {
                    errorCode = 3;
                    break;
                }
            }
        }
        
        free(buff);       //�ͷ��ڴ�
        fclose(pfile);    //�ر��ļ�
        
        return errorCode;
    }
    else
    {
        return 4;
    }
}


/*********************************************************************************************
�� �� ��: servFpgaFlashFileLoad
ʵ�ֹ���: �ӱ��ؼ����ļ���FPGA��
�������: 
�������:   
�� �� ֵ: 
˵    ��:
*********************************************************************************************/
u8 servFpgaFlashFileLoad(u32 loadAddr)
{
    u8 tryCountMax = 3;    //�����3��
    u8 tryCount = 0;
    u8 errorCode = 0;

    s32 dataLen = 0;
    u8 *buff = NULL;
    u32 readLen = 0;
    u32 flashAddr;
    u8  sizeLen = sizeof(dataLen);

    
    while (1)
    {
        if (!servFpgaProgStart())   //�����������ɹ�
        {
            tryCount++;
            bspDelayMs(1);

            if (tryCount >= tryCountMax)
            {
                return 1;
            }
        }
        else
        {
            break;
        }
    }
    
    tryCount = 0;
    buff = (u8*)malloc(FPGA_CFG_BUFF_SIZE);
    if (buff != NULL)
    {
        while (1)
        {
            readLen = 0;
            flashAddr = loadAddr;
            bspFlashRead(flashAddr, (u8 *)&dataLen, sizeLen);    //��ȡ���ݳ���
            flashAddr += sizeLen;
          
            do
            {
                if (dataLen > FPGA_CFG_BUFF_SIZE)
                {
                    readLen = FPGA_CFG_BUFF_SIZE;
                }
                else
                {
                    readLen = dataLen;
                }
                bspFlashRead(flashAddr,buff,readLen);
                
                bspFpSpiSend(buff,readLen);

                dataLen   -= readLen;
                flashAddr += readLen;
                
            }while (dataLen > 0);
         
            if (servFpgaProgDone())    //���óɹ����˳�ѭ��
            {
                break;
            }
            else
            {
                tryCount++;

                if (tryCount >= tryCountMax)
                {
                    errorCode = 3;
                    break;
                }
            }
        }
        
        free(buff);
        
        return errorCode;
    }
    else
    {
        return 4;
    }
}


/*********************************************************************************************
�� �� ��: servFpgaVersionGet
ʵ�ֹ���: ��ȡFPGA�汾��
�������: 
�������:   
�� �� ֵ: 
˵    ��:
*********************************************************************************************/
FpgaVersionStruct servFpgaVersionGet(void)
{
    u16 tmpReg = 0;
    FpgaVersionStruct fpgaVersion;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_VERSION_REG, &tmpReg);
    
    fpgaVersion.hard  = (u8)((tmpReg & SERV_FPGA_VERSION_HARD_MASK)  >> SERV_FPGA_VERSION_HARD_RSHIFT);
    fpgaVersion.major = (u8)((tmpReg & SERV_FPGA_VERSION_MAJOR_MASK) >> SERV_FPGA_VERSION_MAJOR_RSHIFT);
    fpgaVersion.minor = (u8)((tmpReg & SERV_FPGA_VERSION_MINOR_MASK) >> SERV_FPGA_VERSION_MINOR_RSHIFT);
    fpgaVersion.build = (u8)(tmpReg & SERV_FPGA_VERSION_BUILD_MASK);
    
    return fpgaVersion;
}



/*********************************************************************************************
�� �� ��: servFPGA_STAT_Get
ʵ�ֹ���: ��ȡFPGA��״̬
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
*********************************************************************************************/
s32 servFPGA_STAT_Get(u16* STAT)
{
    servFpgaRegisterRead(0, ADDR_FPGA_STAT, STAT);    //ͨ���Ź̶�Ϊ0

    return 0;
}

/*********************************************************************************************
�� �� ��: servFPGA_Sonic_Reset
ʵ�ֹ���: ��λ�߼��ĳ�����๦��
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: Ϊ�˱�֤ÿ�β���׼ȷ��,����ִ�иò���
*********************************************************************************************/
static void servFPGA_Sonic_Reset(void)
{
    fpgaRegValue.ultrasonicCtrlReg &= ~0x01;  //��������ź����
    fpgaRegValue.ultrasonicCtrlReg |= 0x01<<1; //���ò�ิλ
    
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_CTRL, fpgaRegValue.ultrasonicCtrlReg);    //FOR MODIFY NICK
    
    fpgaRegValue.ultrasonicCtrlReg &= ~(0x01<<1); //�����ิλ  �������
}


/*********************************************************************************************
�� �� ��: servFPGA_Sonic_Start
ʵ�ֹ���: �����߼�����һ�γ������
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
*********************************************************************************************/
static void servFPGA_Sonic_Start(void)
{
    fpgaRegValue.ultrasonicCtrlReg &= ~(0x01<<1); //�����ิλ  �������
    fpgaRegValue.ultrasonicCtrlReg |= ~0x01;      //���ò�������ź�
    
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_CTRL, fpgaRegValue.ultrasonicCtrlReg);    //FOR MODIFY NICK
    
    fpgaRegValue.ultrasonicCtrlReg &= ~(0x01); //����������
}

/*********************************************************************************************
�� �� ��: servFPGA_Sonic_Protect_Mode
ʵ�ֹ���: �����߼��ڼ�⵽��ײΣ��ʱ�Ĳ���
�������: 
          euSonicProtMode  mode:
                        SONIC_ARM_PROTECT = 0,    //�߼���⵽��ײΣ�պ����κζ���
                        SONIC_FPGA_PROTECT = 1    //�߼���⵽��ײΣ�պ��������б�������
�������:
            
�� �� ֵ: 0
˵    ��: 
*********************************************************************************************/
void servFPGA_Sonic_Protect_Mode(euSonicProtMode mode)
{
    if(mode == SONIC_ARM_PROTECT)
    {
        fpgaRegValue.ultrasonicCtrlReg &= ~(0x01<<2);  //bit2 = 0 ARM����
    }
    else
    {
        fpgaRegValue.ultrasonicCtrlReg |= (0x01<<2);  //bit2 = 1 FPGA����
    }
        
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_CTRL, fpgaRegValue.ultrasonicCtrlReg);    //FOR MODIFY NICK
}


/*********************************************************************************************
�� �� ��: servFPGA_Sonic_Pulse_Get
ʵ�ֹ���: ��ȡ������ģ��Ļ����ź�������
�������: 
          
�������:u32 * u32pulse:��������ķ��ص�ַ
            
�� �� ֵ: 0
˵    ��: �����������߼����ص� ��16�͸�4λ ƴ�ӵõ�
*********************************************************************************************/
static s32 servFPGA_Sonic_Pulse_Get(u8 chanNum, u32 * u32pulse)
{
    u16 pulse_L = 0;
    u16 pulse_H = 0;

    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_USONIC_DIS_L, &pulse_L);
    servFpgaRegisterRead(chanNum, ADDR_FPGA_USONIC_DIS_H, &pulse_H);
    
    *u32pulse = (pulse_H<<16)|pulse_L;
    
    return 0;
}


/*********************************************************************************************
�� �� ��: servFPGA_Sonic_Distance_Get
ʵ�ֹ���: ��ó��������Ľ��
�������: float  f32Temperature:�����¶�
          
�������:
            
�� �� ֵ: > 0 :��ʤ���ٵõ��ľ���
          < 0 :����
˵    ��: 
        ARM������ʽ��������PW�ͼ��S:
        PW=PCNT * TCNT= 2S/(331.4 + 0.607T)
        ����TCNTΪFPGA�ڲ�����ʱ������=1.28*10^6S.
        ��S��ʾ���(��λ:m),
        TΪ�����¶�(��λ:OC)
        
        ������õ���ʽΪ: S = 0.00064*P/(331400+607T) 
*********************************************************************************************/
float servFPGA_Sonic_Distance_Get(float  f32Temperature)
{
    u32 u32pulse = 0;
    float f32tmp_1 = 0;
    float f32tmp_2 = 0;
    float f32res = 0;
    servFPGA_Sonic_Reset();

    servFPGA_Sonic_Start();
    bspDelayMs(15); //�ȴ�15msΪ�߼�Ҫ��
    
    servFPGA_Sonic_Pulse_Get(0, &u32pulse);    //FOR MODIFY NICK
    f32tmp_1 = 331.400 + 0.607*f32Temperature;
    f32tmp_2 = 0.0008*u32pulse;
    f32res = f32tmp_2*f32tmp_1;

    return f32res;
}


/*********************************************************************************************
�� �� ��: servFPGA_Sonic_Threshold_Set
ʵ�ֹ���: ���ó�������ײ������ֵ
�������: 
          
�������:u32 u32ThresDis:��ȫ������ֵ
            
�� �� ֵ: 0
˵    ��: 
          ���͸��߼��İ�ȫ��������20bit,��һ�η��͵�16bit,�ڶ��η��͸�4bit
          �·����ݵļ��㹫ʽΪ
          TH=(2*Sth/(331.4 + 0.607*T)*1000000)/ (Tsys*128) -1
          ����THΪ��ֵ,SthΪ��ȫ����(��λΪm),TΪ�����¶�(��λ���϶�),TsysΪ�߼��ڲ�����ʱ������,��λΪus
          Ŀǰ����ֵΪ80*10^-3us.
*********************************************************************************************/

s32 servFPGA_Sonic_Threshold_Set(float f32ThresDis,float f32tmeperature)
{
    float f32tmp = 0;
    u32 res = 0;
    u16 data = 0;

    f32tmp = (0.607*f32tmeperature + 331.4)*5.12;
    res = (u32)((f32ThresDis*1000000)/f32tmp);

    data = res&0xFF; //��16bit
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_THRES_L, data);    //FOR MODIFY NICK

    data = (res>>16)&0x0F;//��4bit
    servFpgaRegisterWrite(0, ADDR_FPGA_USONIC_THRES_H, data);
    
    //printf("dis is %d\n",res);
    return 0;
}


/*********************************************************************************************
�� �� ��: servFPGA_State_If_WR_Allow
ʵ�ֹ���: ��ȡDDR״̬�Ƿ���Խ���DDRд��
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: ��Ŀǰ��Ӧ�ö���,������ֲ�����д������,�������򱨴�
*********************************************************************************************/
s32 servFPGA_State_If_WR_Allow(u8 chanNum)
{
    u16 state = 0xFF;
    u8 ii = 0;
    do
    {
        servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_STAT, &state);
        ii++;
        bspDelayUs(3);
    }while(((state&0x03) != 0x03)&&(ii<3));


    if(ii < 3)
    {
        return 0;
    }
    else
    {
        /*
            ERROR!!
        */
        return -1;
    }
    
}

/*********************************************************************************************
�� �� ��: servFPGA_State_If_Run_Allow
ʵ�ֹ���: ��ȡDDR״̬�Ƿ���Կ�ʼִ�� DDR ����PWM
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
*********************************************************************************************/
s32 servFPGA_State_If_Run_Allow(u8 chanNum)
{
    u16 state = 0xFF;

    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_STAT, &state);

    if((state>>2)&0x01 == 0x01)
    {
        return 0;
    }
    else
    {
        return -1;
    }

}


/*********************************************************************************************
�� �� ��: servFPGA_State_If_DDR_Init
ʵ�ֹ���: ��ȡDDR״̬�Ƿ��Ѿ���ʼ��
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
*********************************************************************************************/
s32 servFPGA_State_If_DDR_Init(u8 chanNum)
{
    u16 state = 0xFF;

    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_STAT, &state);

    if((state)&0x01 == 0x01)
    {
        return 0;
    }
    else
    {
        return -1;
    }

}


/*********************************************************************************************
�� �� ��: servFPGA_PWM_Len_Set
ʵ�ֹ���: �����߼�Ҫִ�еĲ���ĳ���
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
          
*********************************************************************************************/
void servFPGA_PWM_Len_Set(u8 chanNum, u32 len)
{
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_LENGTH_L_REG, (u16)len);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_LENGTH_H_REG, (u16)(len >> 16));
}


/*********************************************************************************************
�� �� ��: servFPGA_PWM_Cycle_Set
ʵ�ֹ���: �����߼�Ҫִ�еĲ����ѭ������
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
          �·������ݵ��ڲ�����*2
*********************************************************************************************/
void servFPGA_PWM_Cycle_Set(u8 chanNum, u32 times)
{
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CYCLE_NUM_L_REG, (u16)times);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CYCLE_NUM_H_REG, (u16)(times >> 16));
}


/*********************************************************************************************
�� �� ��: servFPGA_DDR_WR_Start_Flag
ʵ�ֹ���: ���߼��Ĵ���д���־ ��֪��ʼд��
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
         bit0 д֮ǰ����,д��ɺ���1
*********************************************************************************************/
void servFPGA_DDR_WR_Start_Flag(void)
{
    servFpgaRegisterWrite(0, ADDR_FPGA_DDR_WAVE_WRITE_CTRL, 0);    //FOR MODIFY NICK
}

/*********************************************************************************************
�� �� ��: servFPGA_DDR_WR_End_Flag
ʵ�ֹ���: ���߼��Ĵ���д���־ ��֪д�����
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
         bit0 д֮ǰ����,д��ɺ���1
*********************************************************************************************/
void servFPGA_DDR_WR_End_Flag(void)
{
    servFpgaRegisterWrite(0, ADDR_FPGA_DDR_WAVE_WRITE_CTRL, 1);    //FOR MODIFY NICK
}

/*********************************************************************************************
�� �� ��: servFPGA_DDR_Data_Type_Set
ʵ�ֹ���: ���ü���д�����������
�������: 
          euWaveDataType euType:
                                    WAVE_DATA = 0,      //д����ǲ���
                                    ENCODER_DATA = 1    //д����Ǳ�����У׼ϵ��
          
�������:
            
�� �� ֵ: 0
˵    ��: 
         
*********************************************************************************************/
void servFPGA_DDR_Data_Type_Set(u8 chanNum, euWaveDataType euType)
{
    u16 data = 3 ;

    
    if (euType == ENCODER_DATA)
    {
        data = 2;
    }
    else
    {
        data = 1;
    }

    servFpgaRegisterWrite(chanNum, ADDR_FPGA_DDR_WAVE_TYPE, data);
}


/*********************************************************************************************
�� �� ��: servFpgaRunStateGet
ʵ�ֹ���: ��ȡ�߼�������״̬
�������:     
              
�������:
            
�� �� ֵ: 0:ֹͣ״̬      FPGA_STOP= 0
          1:��������״̬  FPGA_RUNNING = 1
          2:��������״̬  FPGA_BREAK = 2
          3:����״̬      FPGA_ERROR = 3
˵    ��: 10�żĴ���bit[10-7]ֵ����:
              0/3: FPGA���� û��SD�ź����
              1/4: ���ڰ��ղ������SD
              2: ��ͣ����״̬    ������ٶȱ���ģʽ ��Ҳ���ش�״̬
           
*********************************************************************************************/
RunStatenum servFpgaRunStateGet(u8 chanNum)
{
    u16 tmpReg = 0;

    
    servFpgaRegisterRead(chanNum, SERV_FPGA_DDR_MOTOR_STATE_REG, &tmpReg);

    tmpReg &= SERV_FPGA_RUN_STATE_MASK;

    if ((SERV_FPGA_RUN_STATE_NORUN1 == tmpReg) || (SERV_FPGA_RUN_STATE_NORUN2 == tmpReg))
    {
        return RUNSTETE_NORUN;
    }
    else if ((SERV_FPGA_RUN_STATE_RUN1 == tmpReg) || (SERV_FPGA_RUN_STATE_RUN2 == tmpReg))
    {
        return RUNSTETE_RUN;

    }
    else if (SERV_FPGA_RUN_STATE_SD == tmpReg)
    {
        return RUNSTETE_SD;
    }
    else if (SERV_FPGA_RUN_STATE_HOLD == tmpReg)
    {
        return RUNSTETE_HOLD;
    }
    else
    {
        return RUNSTETE_ERROR;
    }

}



/*********************************************************************************************
�� �� ��: servFPGA_Cycle_Cnt_Get
ʵ�ֹ���: ��ȡ�߼���ǰִ�е�ѭ������
�������:     
              
�������:
            
�� �� ֵ: 0
˵    ��: 
         �߼����ص������㿪ʼ �����Ҫ��1
*********************************************************************************************/
void servFPGA_Cycle_Cnt_Get(u8 chanNum, u32 *times)
{
    u16 data_L = 0;
    u16 data_H = 0;
    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_CYC_CNT_L, &data_L);
    servFpgaRegisterRead(chanNum, ADDR_FPGA_CYC_CNT_H, &data_H);
    
    *times = (data_H<<16)|data_L + 1;
}


/*********************************************************************************************
�� �� ��: servFPGA_PWM_Stop
ʵ�ֹ���: ֪ͨ�߼�ֹͣ����PWM
�������:     
              
�������:
            
�� �� ֵ: 0
˵    ��: �����й�������Ҫֹͣ  ����  ��ʼԤȡ����֮ǰ  ��Ҫ����ֹͣ����
*********************************************************************************************/
void servFPGA_PWM_Reset(u8 chanNum)
{
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_STOP;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
    
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_STOP;
}


/*********************************************************************************************
�� �� ��: servFPGA_PWM_FIFO_Left
ʵ�ֹ���: ��ȡFIFOʣ��ռ�
�������: 
          u32      u32Total:FIFO�ܴ�С
�������:
            
�� �� ֵ: 
˵    ��: 
         ���ȴ��߼���ȡ�߼���δ����Ĳ������ݵĸ���,
         Ȼ����FIFO�ܿռ���-δ������� = ������д��Ĳ������
*********************************************************************************************/
u32 servFPGA_PWM_FIFO_Left(u8 chanNum, u32 u32Total)
{
    u16 tmp = 0;
    u32 u32Occupy = 0;
    
    servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_WAVE_FIFO_OCCUPY_L, &tmp);//FIFO�������� ��16bit
    u32Occupy = tmp;

    tmp = 0;
    servFpgaRegisterRead(chanNum, ADDR_FPGA_DDR_WAVE_FIFO_OCCUPY_H, &tmp);//FIFO�������� ��16bit
    u32Occupy |= (tmp<<16)&0xFFFF0000;

    return u32Total-u32Occupy;
}


/*********************************************************************************************
�� �� ��: servSetFpgaMultiSyncFunc
ʵ�ֹ���: ����Դ���ã���ʼ�������S/D�źţ�������/����Դ��(λ��[6:4]���߼���û�����ƹ�ϵ����ͬʱ��Ч,�����ϲ���Ҫ�������)��ֻҪ��������1�������������S/D��
          ���ͬ������
�������: trigSource:����Դѡ��(0:�������,1:Ӳ������,2:CAN���ߴ���)      
�������:   
�� �� ֵ: 
˵    ��: xyzheng add 2017-04-01
*********************************************************************************************/
void servSetFpgaMultiSyncFunc(MultiSyncFuncEnum enFunc)
{
#if 0  //ֻ���ڲ���,û�л������,��ͬʱʹ�ܶ������Դ 
    switch(trigSource)
    {
    case MULTI_SYNC_TRIG_SRC_SW:
        fpgaRegValue.waveTableModeReg &= ~0x01<<4;
        fpgaRegValue.waveTableModeReg |=  0x01<<4;
        break;
    case MULTI_SYNC_TRIG_SRC_HW:
        fpgaRegValue.waveTableModeReg &= ~0x01<<5;
        fpgaRegValue.waveTableModeReg |=  0x01<<5;
        break;
    case MULTI_SYNC_TRIG_SRC_CAN:
        fpgaRegValue.waveTableModeReg &= ~0x01<<6;
        fpgaRegValue.waveTableModeReg |=  0x01<<6;
        break;
    default:
        return;
    }
#else //���뻥��,ֻ��ѡ��һ������
    switch(enFunc)
    {
    case MULTI_SYNC_TRIG_SRC_OFF:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       break;
    case MULTI_SYNC_TRIG_SRC_SW:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<4;
       break;
    case MULTI_SYNC_TRIG_SRC_HW:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<5;
       break;
    case MULTI_SYNC_TRIG_SRC_CAN:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<6;
       break;
    case MULTI_SYNC_START:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<9;
       break;
    case MULTI_SYNC_END:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<10;
       break;
    case MULTI_SYNC_RST:
       fpgaRegValue.waveTableModeReg[0] &= ~0x01<<8;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<8;
       break;
    default:
       fpgaRegValue.waveTableModeReg[0] &= ~0x77<<4;
       fpgaRegValue.waveTableModeReg[0] |=  0x01<<4; //Ĭ��Ϊ�������
       return;
    }
#endif

    servFpgaRegisterWrite(0, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[0]);    //FOR MODIFY NICK
}


/*********************************************************************************************
�� �� ��: servGetFpgaMultiSyncFunc
ʵ�ֹ���: ���ͬ������
�������: 
�������:   
�� �� ֵ: 
˵    ��: xyzheng add 2017-04-01
*********************************************************************************************/
int servGetFpgaMultiSyncFunc(void)
{
    if((fpgaRegValue.waveTableModeReg[0]>>0x06) & 0x01)
    {
         return MULTI_SYNC_TRIG_SRC_CAN;
    }
    else if((fpgaRegValue.waveTableModeReg[0]>>0x04) & 0x01)
    {
         return MULTI_SYNC_TRIG_SRC_SW;
    }
    else if((fpgaRegValue.waveTableModeReg[0]>>0x05) & 0x01)
    {
        return MULTI_SYNC_TRIG_SRC_HW;
    }
    else if((fpgaRegValue.waveTableModeReg[0]>>0x09) & 0x01)
    {
        return MULTI_SYNC_START;
    }
    else if((fpgaRegValue.waveTableModeReg[0]>>0x0a) & 0x01)
    {
        return MULTI_SYNC_END;
    }
    return -1;
}


#if 0
#endif
/*********************************************************************************************
�� �� ��: servFpgaDioStatusRead
ʵ�ֹ���: ��ȡ���ֶ˿ڵ�ƽ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.10 Add
*********************************************************************************************/
LevelStatusEnum servFpgaDioStatusRead(DigitalIONumEnum digitalIONum)
{
    u16 registerValue;
    LevelStatusEnum levelStatus;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_DIO_STATUS_REG, &registerValue);
    
    levelStatus = (LevelStatusEnum)((registerValue >> (u8)digitalIONum) & SERV_FPGA_DIO_STATUS_MASK); 

    return levelStatus;
}


/*********************************************************************************************
�� �� ��: servFpgaDigitalOutLevelSet
ʵ�ֹ���: �������ֶ˿ڵ�ƽ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.10 Add
*********************************************************************************************/
void servFpgaDigitalOutLevelSet(DoutNumEnum dIONum, LevelStatusEnum levelStatus)
{
    switch (dIONum)
    {
#ifdef PROJECT_QUBELEY
        case DIO_DO1:

            fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_DO1_CONFIG_MASK;
            fpgaRegValue.digitalOutputReg |= SERV_FPGA_DO1_SELECT_LEVEL;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.digitalOutputReg |= SERV_FPGA_DO1_LEVEL_HIGH;
            }
            else
            {
                fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_DO1_LEVEL_HIGH;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DIGITAL_OUTPUT_REG, fpgaRegValue.digitalOutputReg);
          break;

        case DIO_DO2:

            fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_DO2_CONFIG_MASK;
            fpgaRegValue.digitalOutputReg |= SERV_FPGA_DO2_SELECT_LEVEL;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.digitalOutputReg |= SERV_FPGA_DO2_LEVEL_HIGH;
            }
            else
            {
                fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_DO2_LEVEL_HIGH;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DIGITAL_OUTPUT_REG, fpgaRegValue.digitalOutputReg);
          break;
#endif

#ifdef PROJECT_GELGOOG          
        case DIO_DO1:
        
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN1;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN1_HIGH;
            }
            else
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN1_LOW;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
          break;

        case DIO_DO2:
        
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN2;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN2_HIGH;
            }
            else
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN2_LOW;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);

          break;

        case DIO_DO3:
        
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN3;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN3_HIGH;
            }
            else
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN3_LOW;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
          break;

        case DIO_DO4:
        
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN4;

            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN4_HIGH;
            }
            else
            {
                fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN4_LOW;
            }

            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
          break;
#endif

        default:
          break;
    }  
}


/*********************************************************************************************
�� �� ��: servFpgaDigitalOutSet
ʵ�ֹ���: �����������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaDigitalOutSet(DoutNumEnum dIONum, DigitalOutManageStruct digitalIO)
{
    LevelStatusEnum levelState;

    
    if (SENSOR_ON == digitalIO.state)
    {
        levelState = (LevelStatusEnum)digitalIO.polarity;
    }
    else
    {
        levelState = (LevelStatusEnum)(DIOPOLARITY_P - digitalIO.polarity);
    }

    servFpgaDigitalOutLevelSet(dIONum, levelState);

    /*{
        if (DIO_DO1 == dIONum)
        {
            //TO ADD    NICK MARK
        }
        else if (DIO_DO2 == dIONum)
        {
            //drvUart3Init(digitalIO.baud);
        }
    }*/
}


/*********************************************************************************************
�� �� ��: servFpgaIsolatorOutLevelSet
ʵ�ֹ���: ���ø���˿ڵ�ƽ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.10 Add
*********************************************************************************************/
void servFpgaIsolatorOutLevelSet(YoutNumEnum youtNum, LevelStatusEnum levelStatus)
{
    switch (youtNum)
    {
        case YOUT_YO1:
            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.digitalOutputReg |= SERV_FPGA_YO1_LEVEL_HIGH;
            }
            else
            {
                fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_YO1_LEVEL_HIGH;
            }
          break;

        case YOUT_YO2:
            if (LEVEL_HIGH == levelStatus)
            {
                fpgaRegValue.digitalOutputReg |= SERV_FPGA_YO2_LEVEL_HIGH;
            }
            else
            {
                fpgaRegValue.digitalOutputReg &= ~SERV_FPGA_YO2_LEVEL_HIGH;
            }
          break;

        default:
          break;
    }
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DIGITAL_OUTPUT_REG, fpgaRegValue.digitalOutputReg);
}


/*********************************************************************************************
�� �� ��: servFpgaIsolatorOutSet
ʵ�ֹ���: �����������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaIsolatorOutSet(YoutNumEnum youtNum, IsolatorOutManageStruct isolOut)
{
    LevelStatusEnum levelState;

    
    if (SENSOR_ON == isolOut.state)
    {
        levelState = (LevelStatusEnum)isolOut.response;
    }
    else
    {
        levelState = (LevelStatusEnum)(DIOPOLARITY_P - isolOut.response);
    }

    servFpgaIsolatorOutLevelSet(youtNum, levelState);
}



/*********************************************************************************************
�� �� ��: servFpgaTriggerFilterSet
ʵ�ֹ���: ���ô��������˲�
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaTriggerFilterSet(u8 chanNum, TrigInPinEnum trigInPin, f32 sPeriod)
{
    u16 registerValue = 0;
    u16 sPeriodCode;

    
    //�������õļĴ���ֵ
    if (sPeriod < SERV_FPGA_SPERIOD_GEAR_VALUE)    //400usΪ�ֽ�
    {
        registerValue &= ~SERV_FPGA_SPERIOD_US_ENABLE;
        sPeriodCode = (u16)(sPeriod / SERV_FPGA_SPERIOD_NS_VALUE - SERV_FPGA_SPERIOD_OFFSET) 
                      & SERV_FPGA_FILTER_TIME_MASK;
        if (sPeriodCode < SERV_FPGA_SPERIOD_CODE_MIN)
        {
            sPeriodCode = SERV_FPGA_SPERIOD_CODE_MIN;
        }
        registerValue |= sPeriodCode;
    }
    else
    {
        registerValue |= SERV_FPGA_SPERIOD_US_ENABLE;
        sPeriodCode = (u16)(sPeriod / SERV_FPGA_SPERIOD_US_VALUE - SERV_FPGA_SPERIOD_OFFSET) 
                      & SERV_FPGA_FILTER_TIME_MASK;
        if (sPeriodCode < SERV_FPGA_SPERIOD_CODE_MIN)
        {
            sPeriodCode = SERV_FPGA_SPERIOD_CODE_MIN;
        }
        registerValue |= sPeriodCode;
    }

    switch (trigInPin)
    {
#ifdef PROJECT_QUBELEY

        case TRIGPIN_DI2_X1:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI2_X1_FILTER_REG, registerValue);
          break;

        case TRIGPIN_DI3_X2:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI3_X2_FILTER_REG, registerValue);
          break;

        case TRIGPIN_DI4_X3:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI4_X3_FILTER_REG, registerValue);
          break;

        case TRIGPIN_DI5_X4:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI5_X4_FILTER_REG, registerValue);
          break;

        case TRIGPIN_CHANZ:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CHANZ_FILTER_REG, registerValue);
          break;
          
#endif

#ifdef PROJECT_GELGOOG

        case TRIGPIN_DIL:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI4_X3_FILTER_REG, registerValue);
          break;

        case TRIGPIN_DIR:
            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI5_X4_FILTER_REG, registerValue);
          break;
          
#endif
        default:
          break;
    }
}


/*********************************************************************************************
�� �� ��: servFpgaTriggerSet
ʵ�ֹ���: ���ô����������ͼ���Ӧ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaTriggerSet(u8 chanNum, TrigInManageStruct trigInInfo, EncoderChanEnum encChan)
{
    u8 i;
    u8 bitValue[5] = {SERV_FPGA_TRIG_COND_NONE, 
                      SERV_FPGA_TRIG_COND_LOW, 
                      SERV_FPGA_TRIG_COND_RISE, 
                      SERV_FPGA_TRIG_COND_FALL, 
                      SERV_FPGA_TRIG_COND_HIGH};
    u8  trigState[2] = {SERV_FPGA_TRIG_DISABLE, SERV_FPGA_TRIG_ENABLE};
    u16 registerCode = 0;
    u16 alarmRegCode = 0;
    u16 registerValue;


    if (TRIGMODE_PATTERN == trigInInfo.trigMode)    //���ʹ���
    {
#if 0
        for (i = 0;i < TRIGPIN_RESERVE;i++)
        {
            registerCode |= (bitValue[(u8)trigInInfo.type[i]] & trigState[(u8)trigInInfo.pattState])
                            << (SERV_FPGA_TRIG_COND_LEN * i);

            //��ǰ�����²�������
            servFpgaTriggerFilterSet((TrigInPinEnum)i, trigInInfo.pattSPeriod);
        }

        if ((0 == registerCode) ||    //��������ȫ��NONE����pattState��OFF����49�żĴ���Ҫ���ó�0 
            (RESPONSE_NONE == trigInInfo.pattResponse))
        {
            //�����Ĵ�����ʧ��
            trigRelRegValue = (~SERV_FPGA_STOP_OPR_ENABLE) & (~SERV_FPGA_ALARM_OPR_ENABLE);
            servFpgaRegisterWrite(0, SERV_FPGA_TRIG_REL_REG, trigRelRegValue);    //FOR MODIFY NICK
        }
        else
        {
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_DIO_ENABLE;    //ʹ�����ִ���
            servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
            
            switch (trigInInfo.pattResponse)
            {
                case RESPONSE_NONE:
                  break;

                case RESPONSE_ALARM:
                    //���ô�������
                    registerValue = registerCode;
                    servFpgaRegisterWrite(0, SERV_FPGA_ALARM_CODE_REG, registerValue);    //FOR MODIFY NICK                    

                    //���ô��������߼���ϵ
                    trigRelRegValue = SERV_FPGA_ALARM_OPR_AND;
                    servFpgaRegisterWrite(0, SERV_FPGA_TRIG_REL_REG, trigRelRegValue);    //FOR MODIFY NICK
                
                    //�����ж�
                    fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_ALARM_INT_ENABLE;
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
                  break;
                  
                case RESPONSE_STOP:
                    //���ô�������
                    registerValue = registerCode;
                    servFpgaRegisterWrite(0, SERV_FPGA_STOP_SWITCH_REG, registerValue);    //FOR MODIFY NICK  

                    //���ô��������߼���ϵ
                    trigRelRegValue = SERV_FPGA_STOP_OPR_AND;
                    servFpgaRegisterWrite(0, SERV_FPGA_TRIG_REL_REG, trigRelRegValue);    //FOR MODIFY NICK
                    
                    //�����ж�
                    fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_STOP_INT_ENABLE;
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
                  break;
                  
                case RESPONSE_ALARMSTOP:
                    //���ô�������
                    registerValue = registerCode;
                    servFpgaRegisterWrite(0, SERV_FPGA_STOP_SWITCH_REG, registerValue);    //FOR MODIFY NICK
                    servFpgaRegisterWrite(0, SERV_FPGA_ALARM_CODE_REG,  registerValue);    //FOR MODIFY NICK  

                    //���ô��������߼���ϵ
                    trigRelRegValue = SERV_FPGA_STOP_OPR_AND | SERV_FPGA_ALARM_OPR_AND;
                    servFpgaRegisterWrite(0, SERV_FPGA_TRIG_REL_REG, trigRelRegValue);    //FOR MODIFY NICK
                    
                    //�����ж�
                    fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_STOP_INT_ENABLE | SERV_FPGA_ALARM_INT_ENABLE;
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
                  break;
                  
                /*case RESPONSE_RUN:    //Ŀǰ��֧��RUN
                  break;*/

                default:
                  break;
            }
        }
#endif
    }
    else
    {
        for (i = 0;i < TRIGPIN_RESERVE;i++)
        {
            switch (trigInInfo.levelResponse[i])
            {
                case RESPONSE_NONE:
                  break;

                case RESPONSE_ALARM:
                    alarmRegCode |= (bitValue[(u8)trigInInfo.type[i]] & trigState[(u8)trigInInfo.levelState[i]]) 
                                    << (SERV_FPGA_TRIG_COND_LEN * (i + SERV_FPGA_TRIG_IN_OFFSET));
                  break;
                  
                case RESPONSE_STOP:
                    registerCode |= (bitValue[(u8)trigInInfo.type[i]] & trigState[(u8)trigInInfo.levelState[i]]) 
                                    << (SERV_FPGA_TRIG_COND_LEN * (i + SERV_FPGA_TRIG_IN_OFFSET));
                  break;
                  
                case RESPONSE_ALARMSTOP:
                    alarmRegCode |= (bitValue[(u8)trigInInfo.type[i]] & trigState[(u8)trigInInfo.levelState[i]]) 
                                    << (SERV_FPGA_TRIG_COND_LEN * (i + SERV_FPGA_TRIG_IN_OFFSET));
                    registerCode = alarmRegCode;
                  break;
                  
                /*case RESPONSE_RUN:    //Ŀǰ��֧��RUN
                  break;*/

                default:
                  break;
            }

            //��ǰ�����²�������
            servFpgaTriggerFilterSet(chanNum, (TrigInPinEnum)i, trigInInfo.levelSPeriod[i]);
        }

        //������ALARM�Ĵ���
        if (0 == alarmRegCode)
        {
            //ʧ��
            fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_ALARM_OPR_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
        }
        else
        {
            //���ô�������
            registerValue = registerCode;
            servFpgaRegisterWrite(0, SERV_FPGA_ALARM_CODE_REG, registerValue); 

            //���ô��������߼���ϵ
            fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_ALARM_OPR_OR;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
            
            //�����ж�
            fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_ALARM_INT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
            
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_DIO_ENABLE;    //ʹ�����ִ���
            servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
        }

        //������STOP�Ĵ���
        if (0 == registerCode)
        {
            //ʧ��
            fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_STOP_OPR_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
        }
        else
        {
            //���ô�������
            registerValue = registerCode;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_STOP_SWITCH_REG, registerValue);   

            //���ô��������߼���ϵ
            fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_STOP_OPR_OR;
        
#ifdef PROJECT_GELGOOG
            /*if (SENSOR_ON == trigInInfo.levelState[TRIGPIN_DIR])
            {
                //���ݱ�����ѡ��ģʽ
                if (ECCHAN_3 == encChan)  
                {
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_REF_SELECT_MASK;
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH4_ENC_TRIPLE;
                }
                else
                {
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_REF_SELECT_MASK;
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH4_ENC_SINGLE;
                }
            }*/
#endif
            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
            
            //�����ж�
            fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_STOP_INT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);

            //��ʱ���ã������޸ĳ�ר�ŵĺ���������    NICK MARK
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_DIO_ENABLE;    //ʹ�����ִ���
            servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
        }
    }
}


/*********************************************************************************************
�� �� ��: servFpgaWaveEndIntSet
ʵ�ֹ���: ʹ�ܻ�ʧ�ܲ������ʱ���ж�
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaWaveEndIntSet(u8 chanNum, bool bEnable)
{
    if (bEnable)
    {
        fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_WAVE_END_INT_ENABLE;
    }
    else
    {
        fpgaRegValue.interruptSrcReg[chanNum] &= ~SERV_FPGA_WAVE_END_INT_ENABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaIntrSourceGet
ʵ�ֹ���: ��ȡFPGA��ARM�жϵ�Դͷ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
u16 servFpgaIntrSourceGet(u8 chanNum)
{
    u16 intrSrcValue;
    
    
    servFpgaRegisterRead(chanNum, SERV_FPGA_INTERRUPT_SRC_REG, &intrSrcValue);

    return intrSrcValue;
}


/*********************************************************************************************
�� �� ��: servFpgaDistStopModeSet
ʵ�ֹ���: ������ֹͣģʽ����
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.09 Add
*********************************************************************************************/
void servFpgaDistStopModeSet(u8 chanNum, StopConfigStruct stopConfigInfo)
{
    u16 tempValue = 0;
    u32 periodInt = 0;    //��������
    u32 periodDec = 0;    //С������
    f32 emergDecelSpeed;


    //���ü�����ֵ
    tempValue = (u16)stopConfigInfo.emergPeriod;  //��16bit
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_PERIOD_L_REG, tempValue);

    tempValue = (stopConfigInfo.emergPeriod >> SERV_FPGA_STOPDECEL_PERIOD_H_RSHIFT) & 
                SERV_FPGA_STOPDECEL_PERIOD_H_MASK;  //��9bit
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_PERIOD_H_REG, tempValue);

    //�������б��
    emergDecelSpeed = (f32)(stopConfigInfo.emergPeriod - stopConfigInfo.minPeriod) / stopConfigInfo.stopSteps;

    //���ü���б��          
    periodInt = (u32)emergDecelSpeed;
    periodDec = (u32)((emergDecelSpeed - periodInt) * SERV_FPGA_STOPDECEL_SLOPE_DEC_VAILD_BITS);
    
    //С���ĵ�16λ
    tempValue = (u16)periodDec;    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_SLOPE_L_REG, tempValue);

    //�����ĵ�9λ + С���ĸ�7λ
    tempValue = ((u16)(periodDec >> SERV_FPGA_STOPDECEL_SLOPE_RSHIFT) & SERV_FPGA_STOPDECEL_SLOPE_M_DEC_MASK) | 
                ((periodInt & SERV_FPGA_STOPDECEL_SLOPE_M_INT_MASK) << SERV_FPGA_STOPDECEL_SLOPE_M_INT_LSHIFT);    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_SLOPE_M_REG, tempValue);

    //�����ĸ�16λ
    tempValue = (u16)(periodInt >> SERV_FPGA_STOPDECEL_SLOPE_RSHIFT); 
    servFpgaRegisterWrite(chanNum, SERV_FPGA_STOPDECEL_SLOPE_H_REG, tempValue);
}


/*********************************************************************************************
�� �� ��: servFpgaStopDecelSet
ʵ�ֹ���: ���ü�ͣ������Ϣ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.09 Add
*********************************************************************************************/
void servFpgaStopDecelSet(u8 chanNum, StopConfigStruct stopConfigInfo)
{
    //StopConfigStruct holdStopConfigInfo;


    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_STOP_DECEL_MASK;    //��λ


    //Ĭ��ʹ��ֹͣ�����Ĵ���ԴΪ�������
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_WAVE_END_ENABLE;

    switch (stopConfigInfo.stopMode)
    {
        case STOPMODE_IMMED: 
            fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_SD_SRC_WAVE_END_ENABLE;    //��ֹ��ͣ���٣�ֱ��ֹͣ   
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_IMMED_ENABLE;            //���ٷ�ʽ����ֹͣ
          break;
          
        /*case STOPMODE_HOLD:
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SLOPE_ENABLE;           //���ٷ�ʽ�����ٶȼ�

            //�Ȱ����ٶȵķ�ʽ���ã�������FPGA�޸ĺ����ٸ�    NICK MARK
            holdStopConfigInfo.emergPeriod = EMERG_STOP_END_PERIOD;
            holdStopConfigInfo.minPeriod   = EMERG_STOP_END_PERIOD;    //��������һ��б�ʾ�Ϊ0��
            holdStopConfigInfo.stopSteps   = 1;
            servFpgaDistStopModeSet(chanNum, holdStopConfigInfo);
          break;*/

        case STOPMODE_DIST:
            //Ŀǰ��������ٵķ�ʽ���ڲ����������У�ͳ��������Сֵ��
            //Ȼ�������ﰲװ��Сֵ����ֵ�Ĳ�ֵ�Լ�Ҫ��ľ��������������
            //������FPGA�޸ĺ���֮���ٸ�    NICK MARK
            fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SLOPE_ENABLE;

            servFpgaDistStopModeSet(chanNum, stopConfigInfo);
          break;

        case STOPMODE_TIME:
            //Ŀǰ��֧��
            //emergTime = stopDecelInfo.emergTime;
          break;

        case STOPMODE_DIST_TIME:
            //Ŀǰ��֧��
            //emergTime = stopDecelInfo.emergTime;
          break;

        default:
          break;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveHoldSet
ʵ�ֹ���: ���ò��α�����Ϣ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.09 Add
*********************************************************************************************/
void servFpgaWaveHoldSet(u8 chanNum, bool waveHold)
{
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_HOLD_MASK;    //��λ

    if (waveHold)
    {
        fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WH_SRC_WAVE_END_ENABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaEndStateSet
ʵ�ֹ���: ���ò��ν�����״̬
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.09 Add
*********************************************************************************************/
void servFpgaEndStateSet(u8 chanNum, EndStateEnum endState, StopConfigStruct stopConfigInfo)
{
    servFpgaStopDecelSet(chanNum, stopConfigInfo);
            
    switch (endState)
    {
        case ENDSTATE_STOP: 
            servFpgaWaveHoldSet(chanNum, false);
          break;
          
        case ENDSTATE_HOLD: 
            servFpgaWaveHoldSet(chanNum, true);
          break;

        default:
          break;
    }
}


/*********************************************************************************************
�� �� ��: servFpgaWaveWorkModeSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWaveWorkModeSet(u8 chanNum, WaveWorkModeEnum waveWorkMode)
{
    fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_WT_MODE_MASK; //�������ģʽ
    if (WTWORKMODE_FIFO == waveWorkMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MODE_FIFO_ENABLE; 
    }
    else if (WTWORKMODE_CYCLE == waveWorkMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MODE_CYCLE_ENABLE; 
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveModifyDutySet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWaveModifyDutySet(u8 chanNum, ModifyDutyEnum modifyDuty)
{  
    //�������ռ��
    fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_WT_LVT_TIME_MASK;    
    
    if (MDUTY_1000 == modifyDuty)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_LVT_TIME_1000; 
    }
    else if (MDUTY_500 == modifyDuty)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_LVT_TIME_500; 
    }
    else if (MDUTY_250 == modifyDuty)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_LVT_TIME_250; 
    }
    else if (MDUTY_125 == modifyDuty)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_LVT_TIME_125; 
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveLineStepsSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWaveLineStepsSet(u8 chanNum, f32 lineSteps)
{
    u64  lineStepsInverse;


    //�����߼䲽�����õ��ǵ���
    lineStepsInverse = (u64)(SERV_FPGA_LINE_STEPS_MAX / (f64)lineSteps);

    servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_STEPS_INVS_L_REG, 
                          (u16)lineStepsInverse);
                         
    servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_STEPS_INVS_M_REG, 
                          (u16)(lineStepsInverse >> 16));
                         
    servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_STEPS_INVS_H_REG, 
                          (u16)(lineStepsInverse >> 32));

    servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_STEPS_VAILD_BIT_REG, 
                          SERV_FPGA_LINE_STEPS_VAILD_BIT_VALUE);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveLineMultSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWaveLineMultSet(u8 chanNum, u32 lineMult)
{
    //���ñ�������������
    fpgaRegValue.encMultandCountReg[chanNum] &= ~SERV_FPGA_ENC_LINE_MULT_MASK;    //��������������ı���
    fpgaRegValue.encMultandCountReg[chanNum] |= (lineMult - SERV_FPGA_ENC_LINE_MULT_OFFSET) 
                                                 << SERV_FPGA_ENC_LINE_MULT_LSHIFT
                                                 & SERV_FPGA_ENC_LINE_MULT_MASK;

    servFpgaRegisterWrite(chanNum, SERV_FPGA_ENCODER_RESET_REG, 
                          fpgaRegValue.encMultandCountReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveCalcModeSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWaveCalcModeSet(u8 chanNum, MotionModeEnum motionMode)
{
    fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_WT_MOTION_MODE_MASK;    //����˶�ģʽ
    if (MTNMODE_PVT == motionMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MTN_MODE_PVT; 
    }
    else if (MTNMODE_LVT_CORR == motionMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MTN_MODE_LVT_VEL; 
    }
    else if (MTNMODE_LVT_NOCORR == motionMode)
    {
        fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_WT_MTN_MODE_LVT_NOVEL; 
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveMotionModeSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWaveMotionModeSet(u8 chanNum, MotionModeEnum motionMode, f32 lineSteps, u32 lineMult, ModifyDutyEnum modifyDuty)
{
    servFpgaWaveCalcModeSet(chanNum, motionMode);    

    if ((MTNMODE_PVT != motionMode) && (lineSteps >= 2.0))    //CJ 2017.11.25 Modify
    //if ((MTNMODE_PVT != motionMode) && (lineSteps != 0) && (lineMult != 0))
    {
        servFpgaWaveModifyDutySet(chanNum, modifyDuty);
        servFpgaWaveLineStepsSet(chanNum, lineSteps);
        servFpgaWaveLineMultSet(chanNum, lineMult); 
    }
}


/*********************************************************************************************
�� �� ��: servFpgaWaveInit
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWaveInit(u8 chanNum, WaveTableStruct waveTable)
{
    servFpgaWaveWorkModeSet(chanNum, waveTable.waveWorkMode);
    //servFpgaWaveMotionModeSet(chanNum, waveTable.motionMode);    
}


/*********************************************************************************************
�� �� ��: servFpgaWaveReset
ʵ�ֹ���: ��λ����
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveReset(WaveTableStruct *pWaveTable, WaveWorkModeEnum waveWorkMode)
{
    pWaveTable->writableAddr = pWaveTable->startAddr;
    pWaveTable->writableSize = pWaveTable->totalSize;
    
    pWaveTable->waveState = MTSTATE_IDLE;
    
    pWaveTable->waveWorkMode = waveWorkMode;
}


/*********************************************************************************************
�� �� ��: servFpgaWaveFifoReset
ʵ�ֹ���: ��FIFOģʽ�½������ò���ǰ��FIFO��λ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.12.24 Add
*********************************************************************************************/
void servFpgaWaveFifoReset(u8 chanNum)
{
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_FIFO_RESET;
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_STOP;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
    
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_FIFO_RESET;
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_STOP;
}


/*********************************************************************************************
�� �� ��: servFpgaWaveWriteDdrAddrSet
ʵ�ֹ���: ���ò���д���ַ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2018.02.01 Add
*********************************************************************************************/
void servFpgaWaveWriteDdrAddrSet(u32 waveAddr)
{
    u32 ddrAddr;
    u16 addrL;
    u16 addrH;


    //������ǲ����ַ����DDR��ַ�Ķ�Ӧ��ϵ��ddrAddr = waveAddr * 4
    ddrAddr = waveAddr * FPGA_DDR_WAVE_TABLE_POINT_LEN;
    addrL = (u16)(ddrAddr & 0xFFFF);
    addrH = (u16)((ddrAddr >> 16) & 0x0FFF);

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_WT_WRITE_ADDR_L_REG, addrL);
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_WT_WRITE_ADDR_H_REG, addrH);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveStartDdrAddrSet
ʵ�ֹ���: ���ò��������ʼ��ַ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2018.02.01 Add
*********************************************************************************************/
void servFpgaWaveStartDdrAddrSet(u8 chanNum, u32 waveAddr)
{
    u32 ddrAddr;
    u16 addrL;
    u16 addrH;


    //������ǲ����ַ����DDR��ַ�Ķ�Ӧ��ϵ��ddrAddr = waveAddr * 4
    ddrAddr = waveAddr * FPGA_DDR_WAVE_TABLE_POINT_LEN;
    addrL = (u16)(ddrAddr & 0xFFFF);
    addrH = (u16)((ddrAddr >> 16) & 0x0FFF);

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_START_ADDR_L_REG, addrL);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_START_ADDR_H_REG, addrH);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveDataWrite
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
s32 servFpgaWaveDataWrite(u8 chanNum, OutputDataStruct *pOutputData, WaveTableStruct *pWaveTable)
{
    static u8 lastChan = 0;
    s32 writeResult = 0;


    //����δ����
    if (MTSTATE_IDLE == pWaveTable->waveState)
    {    
        //�޸Ĳ���״̬
        pWaveTable->waveState = MTSTATE_CALCING;
        
        //������������
        servFPGA_DDR_Data_Type_Set(chanNum, WAVE_DATA);

        //��ʼ������
        servFpgaWaveInit(chanNum, *pWaveTable);

        //FIFOģʽ��ҪReset
        if (WTWORKMODE_FIFO == pWaveTable->waveWorkMode)
        {
            servFpgaWaveFifoReset(chanNum);

            //��һ����������Ԥȡ��ʱ������Ҳ�ǿ��еģ�Ŀǰ�Ȱ��ɵ��������ã��������Ժ����޸�    NICK MARK
            servFPGA_PWM_Len_Set(chanNum, pWaveTable->totalSize);
        }
    }
    
    //ͨ�������л�
    if (lastChan != chanNum)
    {
        lastChan = chanNum;

        //������������
        servFPGA_DDR_Data_Type_Set(chanNum, WAVE_DATA);
    }

    //��ȡ�߼�״̬������Ƿ���Խ��в����·�
    writeResult = servFPGA_State_If_WR_Allow(chanNum);    
    if (writeResult != 0)
    {
        return writeResult;
    }
        
    //���ÿ�ʼ��־
    servFPGA_DDR_WR_Start_Flag();  
        
    //����д���ַ
    servFpgaWaveWriteDdrAddrSet(pWaveTable->writableAddr);

    //��Ҫ���͵����ݲŷ���
    if (pOutputData->toSendBytes > 0)
    {
        //PVTCALC�߳��Ѿ������ݲ�����2^n������ֻ��Ҫ���;���

        //�������ݳ��ȱ��˲��������ó���
        //if (pOutputData->lastSendBytes != pOutputData->toSendBytes)    //�Ȳ��������жϣ������    NICK MARK
        {
            //FPGA_Set_W_R_Data_Length(FPGA_WRITE_ACTION, pOutputData->toSendBytes - 2);    //-2Ϊ�߼�Ҫ�� 
            servFpgaDataLenSet(FPGAACT_WRITE, pOutputData->validBytes, pOutputData->toSendBytes);
            pOutputData->lastSendBytes = pOutputData->toSendBytes;
        }
        bspSdioDataSend((u8 *)&pOutputData->writeAddr, pOutputData->toSendBytes, pOutputData->toSendBlkSize);

        //���²����д��ַ�ʹ�С
        pWaveTable->writableAddr += pOutputData->validPoints;
        pWaveTable->writableSize -= pOutputData->validPoints;

        //�Ƿ���Ҫ���Ϳ�����
        if (pOutputData->bSendEmpty)
        {
            //�������ݳ��ȱ��˲��������ó���
            //if (pOutputData->lastSendBytes != pOutputData->toSendBytes)    //�Ȳ��������жϣ������    NICK MARK
            {
                //FPGA_Set_W_R_Data_Length(FPGA_WRITE_ACTION, pOutputData->emptyBytes - 2);    //-2Ϊ�߼�Ҫ�� 
                servFpgaDataLenSet(FPGAACT_WRITE, pOutputData->validEmpBytes, pOutputData->emptyBytes);
                pOutputData->lastSendBytes = pOutputData->toSendBytes;
            }
            bspSdioDataSend((u8 *)&pOutputData->emptyDataAddr, pOutputData->emptyBytes, pOutputData->emptyBlkSize);

            //���²����д��ַ�ʹ�С
            pWaveTable->writableAddr += pOutputData->validEmpPoints;
            pWaveTable->writableSize -= pOutputData->validEmpPoints;

            pOutputData->bSendEmpty  = false;
        }
    }
    
    //���ý�����־
    servFPGA_DDR_WR_End_Flag();

    //���һ֡����
    if (pOutputData->bLastFrame)
    {
        if (WTWORKMODE_FIFO != pWaveTable->waveWorkMode)
        {
            pWaveTable->waveState = MTSTATE_CALCEND;
        }
    }

    return writeResult;
}


/*********************************************************************************************
�� �� ��: servFpgaWavePrefetch
ʵ�ֹ���: Ԥȡ����
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWavePrefetch(u8 chanNum)
{
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_PREFETCH;

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);  
}


/*********************************************************************************************
�� �� ��: servFpgaWaveEnable
ʵ�ֹ���: ʹ���������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveEnable(u8 chanNum, bool outputEnable)
{
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~(SERV_FPGA_WAVE_PREFETCH      |
                                                SERV_FPGA_WAVE_OUTPUT_ENABLE |
                                                SERV_FPGA_WAVE_STOP); 
                                          
    if (outputEnable)
    {
        fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_WAVE_OUTPUT_ENABLE;
    }
    else
    {
        fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_OUTPUT_ENABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveStart
ʵ�ֹ���: ��ʼ�������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveStart(void)
{
    fpgaRegValue.waveTableCtrlReg[0] |= SERV_FPGA_WAVE_ALL_RUN;

    servFpgaRegisterWrite(0, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[0]);
    
    fpgaRegValue.waveTableCtrlReg[0] &= ~SERV_FPGA_WAVE_ALL_RUN;
}


/*********************************************************************************************
�� �� ��: servFpgaWaveStop
ʵ�ֹ���: ����ֹͣ�������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveStop(u8 chanNum, StopConfigStruct stopConfigInfo)
{
    servFpgaStopDecelSet(chanNum, stopConfigInfo);    //��ʱ��ƣ���FPGA������ƺú����޸�    NICK MARK

    //����FPGAֹͣ�������
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_SOFT_ENABLE;
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_OUTPUT_ENABLE;    //�������λ
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
    
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_SD_SRC_SOFT_ENABLE;    //ֹͣ����ͺ󱾵�״̬���븴λ  ��ֹ�´�����
}


/*********************************************************************************************
�� �� ��: servFpgaWaveEmergStop
ʵ�ֹ���: ���ý���ֹͣ�������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveEmergStop(u8 chanNum)
{
    StopConfigStruct stopConfigInfo = {.stopMode = STOPMODE_IMMED};

    
    //���ó�����ֹͣ
    servFpgaStopDecelSet(chanNum, stopConfigInfo);
    
    //����FPGAֹͣ�������
    fpgaRegValue.waveTableCtrlReg[chanNum] |= SERV_FPGA_SD_SRC_SOFT_ENABLE;
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_WAVE_OUTPUT_ENABLE;    //�������λ
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_CTRL_REG, fpgaRegValue.waveTableCtrlReg[chanNum]);
    
    fpgaRegValue.waveTableCtrlReg[chanNum] &= ~SERV_FPGA_SD_SRC_SOFT_ENABLE;    //ֹͣ����ͺ󱾵�״̬���븴λ  ��ֹ�´�����    
}


/*********************************************************************************************
�� �� ��: servFpgaReadyQuery
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
bool servFpgaReadyQuery(u8 chanNum, WaveTableStruct waveTableAddr)
{
    s16 state = 0;
    u32 times = 0;


    //֪ͨ�߼�ֹͣ�������
    servFPGA_PWM_Reset(chanNum);

    //���ò�����ʼ��ַ�ͳ���
    if (waveTableAddr.waveWorkMode != WTWORKMODE_FIFO)    
    {
        servFPGA_PWM_Len_Set(chanNum, waveTableAddr.totalSize - waveTableAddr.writableSize);
    }
    /*else    //������FIFOģʽҲ��������������ó��ȣ�Ŀǰ�Ȱ������̣��Ȳ��Թ��˺����޸�    NICK MARK
    {
        servFPGA_PWM_Len_Set(waveTableAddr.totalSize);
    }*/

    //���ò�����ʼ��ַ
    servFpgaWaveStartDdrAddrSet(chanNum, waveTableAddr.startAddr);
    
    //�����߼���ʼԤȡ����
    servFpgaWavePrefetch(chanNum);
    
    //��ѯ�Ƿ���Կ�ʼ
    do
    {
        state = servFPGA_State_If_Run_Allow(chanNum); 
        times++;

        bspDelayMs(2);
        
    }while ((state != 0) && (times < FPGA_REG_QUERY_COUNT_MAX));
    
    if (state == 0)
    {
        //�����߼���ʼԤȡ����
        servFpgaWaveEnable(chanNum, true);
        
        return true;
    }
    else
    {
        return false;  
    }
}


#if QUBELEY_HARDVER_1
/*********************************************************************************************
�� �� ��: servFpgaDrvBoardTypeGet
ʵ�ֹ���: ��ȡ�������ͺ�
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
DriverTypeEnum servFpgaDrvBoardTypeGet(u8 chanNum)
{

    u16 registerValue = 0;
    DriverTypeEnum drvType;

    servFpgaRegisterRead(CH_SYS, SERV_FPGA_STATE_REG, &registerValue);
    
    if ((registerValue & SERV_FPGA_DRIVE_TYPE_MASK) == SERV_FPGA_DRIVE_TYPE_262)
    {
        drvType = DRIVER_262; 
    }
    else if ((registerValue & SERV_FPGA_DRIVE_TYPE_MASK) == SERV_FPGA_DRIVE_TYPE_2660)
    {
        drvType =  DRIVER_2660; 
    }

    return drvType;      
}
#endif


/*********************************************************************************************
�� �� ��: servFpgaReadyStateQuery
ʵ�ֹ���: ��ѯFpga Ready״̬
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
bool servFpgaReadyStateQuery(void)
{
    u16 writeValue = 0xA5;
    u16 readValue;


    servFpgaRegisterWrite(0, SERV_FPGA_DEBUG_REG, writeValue);    //ͨ���Ź̶�Ϊ0
    bspDelayUs(10);    //������ʱ��
    servFpgaRegisterRead(0, SERV_FPGA_DEBUG_REG, &readValue);    //ͨ���Ź̶�Ϊ0

    if (writeValue == readValue)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*********************************************************************************************
�� �� ��: servFpgaEncoderEnable
ʵ�ֹ���: ʹ�ܱ�����
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaEncoderSet(u8 chanNum, IntfcStateEnum encoderState, EncoderMultiEnum encMult, EncoderChanEnum  encChan)
{
    bool bEncMult = true;
    

    if (chanNum < ENCODER_SUPPORT_NUM)
    {
        switch (encoderState)
        {
            case INTFC_NONE:
                fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_REF_SELECT_MASK;
                fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH8_NO_ENC;

                bEncMult = false;
              break;

            case INTFC_OFF:
                bspEncoderTimerInit(chanNum, encMult, DISABLE);
                bEncMult = false;
              break;
              
            case INTFC_ON:

#ifdef PROJECT_GELGOOG

                fpgaRegValue.trigRelatRefCfgReg[chanNum] &= ~SERV_FPGA_REF_SELECT_MASK;
                if (ECCHAN_3 == encChan)
                {
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH4_ENC_TRIPLE;
                }
                else
                {
                    fpgaRegValue.trigRelatRefCfgReg[chanNum] |= SERV_FPGA_REF_CH4_ENC_SINGLE;
                }
                
#if FPGA_ARM_COUNT_ENCODER
                bspEncoderTimerInit(chanNum, encMult, ENABLE);
#endif

#else   //#ifdef PROJECT_GELGOOG
              
#if QUBELEY_HARDVER_2
                fpgaRegValue.encandUartReg &= ~SERV_FPGA_ENC_SELECT_MASK;
                fpgaRegValue.encandUartReg |= SERV_FPGA_ENC_SELECT_DI145;
                servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
#endif

#if FPGA_ARM_COUNT_ENCODER
                bspEncoderTimerInit(chanNum, encMult, ENABLE);
#endif
              
#endif    //#ifdef PROJECT_GELGOOG

              break;

            default:
              break;
        }
        servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);


        if (bEncMult)
        {
            //���ñ���������
            fpgaRegValue.encMultandCountReg[chanNum] &= ~SERV_FPGA_ENC_MULT_MASK;

            switch (encMult)
            {
                case ENCMULT_SINGLE:
                    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_MULT_SINGLE;
                  break;

                case ENCMULT_DOUBLE:
                    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_MULT_DOUBLE;
                  break;

                case ENCMULT_QUADR:
                    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_MULT_QUADR;
                  break;

                default:
                  break;
            }
            
            servFpgaRegisterWrite(chanNum, SERV_FPGA_ENCODER_RESET_REG, fpgaRegValue.encMultandCountReg[chanNum]);

            //����ͨ����
            switch (encChan)
            {
                case ECCHAN_3:
                    fpgaRegValue.encMultandCountReg[chanNum] &= ~SERV_FPGA_ENC_CHAN_SINGLE;
                  break;
            
                case ECCHAN_1:
                    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_CHAN_SINGLE;
                  break;

                default:
                  break;
            }
            servFpgaRegisterWrite(chanNum, SERV_FPGA_ENCODER_RESET_REG, fpgaRegValue.encMultandCountReg[chanNum]);
        }
    }
}


/*********************************************************************************************
�� �� ��: servFpgaEncoderCountReset
ʵ�ֹ���: ��λ����������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaEncoderCountReset(u8 chanNum)
{
#if FPGA_ARM_COUNT_ENCODER
    bspEncoderCountSet(chanNum, 0);
    
#else

    fpgaRegValue.encMultandCountReg[chanNum] &= ~SERV_FPGA_ENC_COUNT_MASK;
    fpgaRegValue.encMultandCountReg[chanNum] |= SERV_FPGA_ENC_COUNT_RESET;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_ENCODER_RESET_REG, fpgaRegValue.encMultandCountReg[chanNum]);
#endif
}


/*********************************************************************************************
�� �� ��: servFpgaEncoderCountReset
ʵ�ֹ���: ��λ����������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaEncoderCountRead(u8 chanNum, s16 *pCircleNum, s32 *pABCount, s32 *pSteps)
{
#if FPGA_ARM_COUNT_ENCODER
    //Ŀǰֻ֧��abCount��circleNum
    if (pCircleNum != NULL)
    {
        *pCircleNum = bspEncoderCircleCountGet(chanNum);
    }
    
    if (pABCount != NULL)
    {
        *pABCount = bspEncoderABCountGet(chanNum);
    }
    
#else

    u16 abCntValue;
    u16 stepsValue;
    u32 abCount;
    u32 steps;

    //Ϊ���������������Ĵ����ļ���ֵ���ȶ���SERV_FPGA_ENC_STEPS_L_REG
    servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_STEPS_L_REG, &stepsValue);
    
    if (pCircleNum != NULL)
    {
        servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_CIRCLE_NUM_REG, (u16 *)pCircleNum);
    }

    if (pABCount != NULL)
    {        
        servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_ABCNT_L_REG, &abCntValue);
        abCount = abCntValue;
        servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_ABCNT_H_REG, &abCntValue);

#if 1
        if (abCntValue > 0)    //����
        {
            abCount |= (u32)(abCntValue & SERV_FPGA_ENC_AB_CNT_H_MASK) << SERV_FPGA_ENC_COUNT_H_LSHIFT;
            *pABCount = (s32)abCount - SERV_FPGA_ABCNT_RCOUNT_MIN;
        }
        else
        {
            *pABCount = abCount;
        }
#else
        abCount |= (u32)(abCntValue & SERV_FPGA_ENC_AB_CNT_H_MASK) << SERV_FPGA_ENC_COUNT_H_LSHIFT;
        
        if (abCount > SERV_FPGA_ABCNT_FCOUNT_MAX)
        {
            *pABCount = (s32)abCount - SERV_FPGA_ABCNT_RCOUNT_MIN;
        }
        else
        {
            *pABCount = abCount;
        }
#endif
    }

    if (pSteps != NULL)
    {
        steps = stepsValue;
        servFpgaRegisterRead(chanNum, SERV_FPGA_ENC_STEPS_H_REG, &stepsValue);
        steps |= (u32)stepsValue << SERV_FPGA_ENC_COUNT_H_LSHIFT;
        if (steps > SERV_FPGA_STEPS_FCOUNT_MAX)
        {
            *pSteps = (s32)steps - SERV_FPGA_STEPS_RCOUNT_MIN;
        }
        else
        {
            *pSteps = steps;
        }
    }
#endif
}


/*********************************************************************************************
�� �� ��: servFpgaLineOutOfStepSet
ʵ�ֹ���: �����߼�ʧ����ֵ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.08.12 Add
*********************************************************************************************/
void servFpgaLineOutOfStepSet(u8 chanNum, SensorStateEnum  state, f32 lineSteps, u16 threshold)
{
    u16 thresholdL;
    u16 thresholdH;


    if (SENSOR_ON == state)
    {
        fpgaRegValue.interruptSrcReg[chanNum] |= SERV_FPGA_OOSTEP_INT_ENABLE;

        thresholdL = (u16)(lineSteps - threshold);
        thresholdH = (u16)(lineSteps + threshold);
        
        servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_OUT_OF_STEP_L_REG, thresholdL);
        servFpgaRegisterWrite(chanNum, SERV_FPGA_LINE_OUT_OF_STEP_H_REG, thresholdH);
    }
    else
    {
        fpgaRegValue.interruptSrcReg[chanNum] &= ~SERV_FPGA_OOSTEP_INT_ENABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_INT_SRC_REG, fpgaRegValue.interruptSrcReg[chanNum]);
}


#if QUBELEY_HARDVER_2
/*********************************************************************************************
�� �� ��: servFpgaDriverStateCofing
ʵ�ֹ���: ͨ��FPGA��������
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
static void servFpgaDriverStateCofing(DriverTypeEnum deviceType, DriverStateEnum driverState)
{
    fpgaRegValue.driverCtrlReg &= SERV_FPGA_DRIVER_SELC_MASK;
    fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_STATE_MASK;    //�ر���������

    if (DRIVER_2660 == deviceType)
    {
        fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_SELC_2660;

        if (DRVSTATE_ON == driverState)
        {
            fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_OFF_2660;
        }
    }
    else
    {
        fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_SELC_262;

        if (DRVSTATE_ON == driverState)
        {
            fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_OFF_262;
        }
    }
        
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_REG, fpgaRegValue.driverCtrlReg);
}
#endif


/*********************************************************************************************
�� �� ��: servFpgaDriverClkCofing
ʵ�ֹ���: ͨ��FPGA��������ʱ��
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
static void servFpgaDriverClkCofing(u8 chanNum, DriverClkStateEnum driverClkState)
{
    if (CH1 == chanNum)
    {
        fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_CLK_MASK;

        if (DRVCLK_OPEN == driverClkState)
        {
            fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_CLK_ENABLE;
        }
    }
#ifdef PROJECT_GELGOOG

    else
    {
        fpgaRegValue.driverCtrlReg &= ~(SERV_FPGA_DRIVER_CLK2_MASK << (chanNum - CH2));

        if (DRVCLK_OPEN == driverClkState)
        {
            fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_CLK2_ENABLE << (chanNum - CH2);
        }
    }
    
#endif

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_REG, fpgaRegValue.driverCtrlReg);
}


/*********************************************************************************************
�� �� ��: servFpgaDriverFromArm
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDriverFromArm(u8 chanNum)
{
    fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_SRC_MASK;
    fpgaRegValue.driverCtrlReg |= SERV_FPGA_DRIVER_SRC_CPU;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_REG, fpgaRegValue.driverCtrlReg);    //ͨ���Ź̶�Ϊ0
}


/*********************************************************************************************
�� �� ��: servFpgaDriverSelect
ʵ�ֹ���: ѡ��Ҫ���õ�����
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDriverSelect(u8 chanNum)
{
#ifdef PROJECT_GELGOOG 
    fpgaRegValue.driverCtrlReg &= ~SERV_FPGA_DRIVER_CHAN_MASK;
    fpgaRegValue.driverCtrlReg |= chanNum << SERV_FPGA_DRIVER_CHAN_LSHIFT;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DRIVER_REG, fpgaRegValue.driverCtrlReg);
#endif
}


/*********************************************************************************************
�� �� ��: servFpgaDriverCofing
ʵ�ֹ���: ͨ��FPGA��������
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
s32 servFpgaDriverCofing(u8 chanNum, DriverTypeEnum deviceType, DriverStateEnum driverState, DriverClkStateEnum driverClkState)
{
    //fpgaRegValue.driverCtrlReg = 0;

#if QUBELEY_HARDVER_2    //Qubeley 03���Ժ�Gelgoog������Ҫ���� 
    servFpgaDriverStateCofing(deviceType, driverState);
#endif

    servFpgaDriverClkCofing(chanNum, driverClkState);
    
    return 0;
}


/*********************************************************************************************
�� �� ��: servFpgaStartSourceSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaStartSourceSet(u8 chanNum, StartSourceEnum startSource)
{
    fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_START_SOURCE_MASK;

    switch (startSource)
    {
        case SSRC_SOFT:
        default:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_SOFT;
          break;

        case SSRC_HARD:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_HARD;
          break;

        case SSRC_CAN:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_CAN;
          break;

        case SSRC_ALL:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_SOFT;
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_HARD;
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_START_SOURCE_CAN;
          break;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaClockSyncSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaClockSyncSet(ClockSyncRegEnum ClockSyncReg, ReceiveTypeEnum receiveType, CanIntfcStruct canIntfc)
{
    u32 canId = 0;


    fpgaRegValue.waveTableModeReg[CH1] &= ~SERV_FPGA_CLOCK_SYNC_MASK;

    switch (ClockSyncReg)
    {
        case CLOCKREG_ON:
          fpgaRegValue.waveTableModeReg[CH1] |= SERV_FPGA_CLOCK_SYNC_RESET;
          fpgaRegValue.waveTableModeReg[CH1] |= SERV_FPGA_CLOCK_SYNC_START;
          break;

        case CLOCKREG_OFF:
          fpgaRegValue.waveTableModeReg[CH1] |= SERV_FPGA_CLOCK_SYNC_END;
          break;
          
        default:
          break;
    }


    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[CH1]);

    if(receiveType == RECEIVE_RADIOID)
    {
        canId = canIntfc.radioId;
    }
    else if(receiveType == RECEIVE_GROUPID1)
    {
        canId = canIntfc.groupId1;
    }
    else if(receiveType == RECEIVE_GROUPID2)
    {
        canId = canIntfc.groupId2;
    }
    else if(receiveType == RECEIVE_RECEIVEID)
    {
        canId = canIntfc.receiveId;
    }

    if (CLOCKREG_ON == ClockSyncReg)
    {
        servCanBuildFrameSendClockSyncStartFrame2Fpga(canId, canIntfc.idType, canIntfc.baud);
    }
    else
    {
        servCanBuildFrameSendClockSyncEndFrame2Fpga(canId, canIntfc.idType, canIntfc.baud);
    }
}


/*********************************************************************************************
�� �� ��: servFpgaClockSyncCountRead
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
u32 servFpgaClockSyncCountRead(void)
{
    u32 count = 0;
    u16 state = 0;


    servFpgaRegisterRead(CH_SYS, SERV_FPGA_CLOCK_SYNC_COUNT_H_REG, &state);

    if (state & SERV_FPGA_CLOCK_SYNC_COUNT_INVAILD)
    {
        count = 0;
    }
    else
    {
        //ֻȡ32λ��

        //��16λ
        servFpgaRegisterRead(CH_SYS, SERV_FPGA_CLOCK_SYNC_COUNT_M_REG, &state);
        count = ((u32)state) << 16;

        //��16λ
        servFpgaRegisterRead(CH_SYS, SERV_FPGA_CLOCK_SYNC_COUNT_L_REG, &state);
        count += state;
    }

    return count;
}


/*********************************************************************************************
�� �� ��: servFpgaMotionRegSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaMotionRegSet(RegisterFuncEnum regFunc, ReceiveTypeEnum receiveType, CanIntfcStruct canIntfc)
{
    switch (regFunc)
    {
        case REGISTER_NONE:
          break;

        case REGISTER_SOFTWARE:
        case REGISTER_DIOSTART:
        case REGISTER_CANSTART:
          break;

        case REGISTER_SYNCSTART:
        case REGISTER_SYNCEND:
          break;

        default:
          break;
    }
}


/*********************************************************************************************
�� �� ��: servFpgaDDRStateGet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
DdrStateEnum servFpgaDDRStateGet(void)
{
    u16 tmpReg = 0;
    DdrStateEnum ddrState;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_DDR_MOTOR_STATE_REG, &tmpReg);

    if ((tmpReg & SERV_FPGA_DDR_INIT_MASK) == SERV_FPGA_DDR_INIT_SUCCESS)
    {
        if ((tmpReg & SERV_FPGA_DDR_TEST_ERROR) == SERV_FPGA_DDR_TEST_ERROR)
        {
            ddrState = DDR_TESTERROR;
        }
        else
        {
            ddrState = DDR_NOERROR;
        }
    }
    else
    {
        ddrState = DDR_NOINIT;
    }

    return ddrState;
}


/*********************************************************************************************
�� �� ��: servFpgaDDRErrorClear
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaDDRErrorClear(void)
{
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_DDR_TEST_REG, SERV_FPGA_DDR_TEST_CLEAR);
}


/*********************************************************************************************
�� �� ��: servFpgaUartSelect
ʵ�ֹ���: ʹ�ܱ�����
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaSensorUart1RxSelect(SensorNumEnum sensorNum)
{
#ifdef PROJECT_GELGOOG
    fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_SELECT_MASK;

    
    switch (sensorNum)
    {
        case SENSOR_S1:
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_SELECT_SEN1;
          break;

        case SENSOR_S2:
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_SELECT_SEN2;
          break;

        case SENSOR_S3:
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_SELECT_SEN3;
          break;

        case SENSOR_S4:
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_SELECT_SEN4;
          break;

        default:
          break;
    }
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_ENC_AND_UART3_REG, fpgaRegValue.encandUartReg);
#endif
}


/*********************************************************************************************
�� �� ��: servFpgaUart4RxEnable
ʵ�ֹ���: ʹ�ܱ�����
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.11.02 Add
*********************************************************************************************/
void servFpgaSensorUart2RxEnable(bool enable)
{
#ifdef PROJECT_GELGOOG
    static u16 regValue = 0;
    static bool record = false;


    if (enable)
    {
        regValue = fpgaRegValue.trigRelatRefCfgReg[CH4];
        record   = true;
        
        fpgaRegValue.trigRelatRefCfgReg[CH4] &= ~SERV_FPGA_REF_SELECT_MASK;
        fpgaRegValue.trigRelatRefCfgReg[CH4] |= SERV_FPGA_REF_CH4_ENC_SENSOR;
    }
    else
    {
        if (record)
        {
            fpgaRegValue.trigRelatRefCfgReg[CH4] = regValue;
        }
    }
    
    
    servFpgaRegisterWrite(CH4, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[CH4]);
#endif
}


/*********************************************************************************************
�� �� ��: servFpgaUart4RxSelect
ʵ�ֹ���: ʹ�ܱ�����
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.11.02 Add
*********************************************************************************************/
void servFpgaSensorUart2RxSelect(SensorNumEnum sensorNum)
{
#ifdef PROJECT_GELGOOG

    fpgaRegValue.sensorUart2SelectReg &= ~SERV_FPGA_UART4_SELECT_MASK;
    
    //4���������У�FPGA�ڶ���ͨ��ʱ������Ķ�Ӧ��ϵ��POS13��14��15��16�ֱ��Ӧͨ��CH4��CH1��CH2��CH3
    switch (sensorNum)
    {
        case SENSOR_S1:
            fpgaRegValue.sensorUart2SelectReg |= SERV_FPGA_UART4_SELECT_SEN1;
          break;

        case SENSOR_S2:
            fpgaRegValue.sensorUart2SelectReg |= SERV_FPGA_UART4_SELECT_SEN2;
          break;

        case SENSOR_S3:
            fpgaRegValue.sensorUart2SelectReg |= SERV_FPGA_UART4_SELECT_SEN3;
          break;

        case SENSOR_S4:
            fpgaRegValue.sensorUart2SelectReg |= SERV_FPGA_UART4_SELECT_SEN4;
          break;

        default:
          break;
    }
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_UART4_RX_REG, fpgaRegValue.sensorUart2SelectReg);
#endif
}


/*********************************************************************************************
�� �� ��: servFpgaReverseMotionSet
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaReverseMotionSet(u8 chanNum, SensorStateEnum  revMotion)
{
    switch (revMotion)
    {
        case SENSOR_OFF:
        default:
          fpgaRegValue.waveTableModeReg[chanNum] &= ~SERV_FPGA_REVERSE_MOTION_ENABLE;
          break;

        case SENSOR_ON:
          fpgaRegValue.waveTableModeReg[chanNum] |= SERV_FPGA_REVERSE_MOTION_ENABLE;
          break;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_WT_MODE_REG, fpgaRegValue.waveTableModeReg[chanNum]);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveStopConfig
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWaveStopConfig(StopConfigStruct *pWaveStopCofing, StopDecelerationStruct stopDecel, f32 posnToStep)
{
    u32 stopSteps;

    
    pWaveStopCofing->stopMode  = stopDecel.stopMode;
    if (STOPMODE_DIST == stopDecel.stopMode)
    {
        stopSteps = (u32)(stopDecel.distance * posnToStep);
        if (0 == stopSteps)
        {
            pWaveStopCofing->stopMode = STOPMODE_IMMED;
        }
        else
        {
            pWaveStopCofing->stopSteps = stopSteps;
        }
    }
    pWaveStopCofing->stopTime  = stopDecel.time;
    pWaveStopCofing->emergPeriod = EMERG_STOP_END_PERIOD;
}


/*********************************************************************************************
�� �� ��: servFpgaWavePrepareConfig
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaWavePrepareConfig(u8 chanNum, WaveTableStruct *pOutpWaveTable, PlanManageStruct wavePlanInfo, PosnConvertInfoStruct posnConvertInfo)
{
    //��λ����������
    servFpgaEncoderCountReset(chanNum);

    //�����¼�ͣ����
    servFpgaWaveStopConfig(&(pOutpWaveTable->stopConfigInfo), wavePlanInfo.stopDecelInfo, posnConvertInfo.posnToStep);

    //���ý���״̬
    servFpgaEndStateSet(chanNum, wavePlanInfo.endState, pOutpWaveTable->stopConfigInfo);
    
    //���ò�������ж�
    servFpgaWaveEndIntSet(chanNum, pOutpWaveTable->bEndIntEnable);
    
    //LVT�������
    servFpgaWaveMotionModeSet(chanNum, 
                              wavePlanInfo.motionMode,
                              posnConvertInfo.lineSteps,
                              posnConvertInfo.lineMult,
                              wavePlanInfo.modifyDuty);

    //�����澯
    servFpgaLineOutOfStepSet(chanNum, wavePlanInfo.outOfStepInfo.lineState,
                                      posnConvertInfo.lineSteps,
                                      wavePlanInfo.outOfStepInfo.lineOutNum);

    //Ԥȡ֮ǰ�������ѭ����������ȥ
    if (pOutpWaveTable->waveWorkMode != WTWORKMODE_FIFO)
    {
        servFPGA_PWM_Cycle_Set(chanNum, wavePlanInfo.cycleNum);
    }
}


/*********************************************************************************************
�� �� ��: servFpgaAbsEncReadTrig
ʵ�ֹ���: ��������ֵ�������Ļض�
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2018.02.28 Add
*********************************************************************************************/
void servFpgaAbsEncReadTrig(SensorNumEnum sensorNum)
{
#if GELGOOG_SINANJU
    u16 dataID = 0x11A;
    
    servFpgaRegisterWrite(sensorNum, SERV_FPGA_ABS_ENC_REDA_REG, dataID);
#endif
}



/*******************************************�ļ�β********************************************/
