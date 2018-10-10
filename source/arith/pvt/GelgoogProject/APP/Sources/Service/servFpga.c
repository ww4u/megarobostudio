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

#if GELGOOG_SINANJU
#include <os.h>
#include "servCommIntfc.h"
#endif



/****************************************�ⲿ��������*****************************************/
extern MotionInfoStruct  g_motionInfo;

#if GELGOOG_SINANJU
extern OS_SEM          g_semCmdParseTask;
extern CommIntfcStruct g_commIntfc;
#endif



/*****************************************�ֲ��궨��******************************************/
#define    FPGA_ADD_LEN_2BYTE      2       //���üĴ���ʱ ����ǰ�ӵĵ�ַ����Ϊ 2�ֽ�
#define    FPGA_REG_LEN_BYTE       2       //�Ĵ�������

#define    FPGA_RREG_ADDR_MARK     0x1F    //���Ĵ�����ַ���� bit[0:4]
#define    FPGA_RCHAN_NUM_MARK     0x1E0   //��ͨ�������� bit[5:8]
#define    FPGA_RCHAN_NUM_LSHIFT   5       //��ͨ�������� bit[5:8]

#define    FPGA_WREG_ADDR_MARK     0x3F    //д�Ĵ�����ַ���� bit[0:5]
#define    FPGA_WCHAN_NUM_MARK     0x0F    //дͨ�������� bit[8:11]���ڶ���Byte

#define    FPGA_CFG_BUFF_SIZE      2048

#define    FPGA_ARM_COUNT_ENCODER  1       //������������ARM����

#define    FPGA_DDR_WAVE_TABLE_POINT_LEN    4    //Bytes

#define    FPGA_SDIO_WRITE_USE_LOCAL    0    // ʹ�þֲ���������

#define    FPGA_PDM_MSTEP_COUNT_LEN      8
#define    FPGA_PDM_MSTEP_BUFFER_RESV    2

#define    FPGA_SDIO_RX_COUNT_MAX    5



/*************************************�ֲ����������Ͷ���**************************************/



/******************************************�ֲ�����*******************************************/
FpgaWriteableRegValueStruct fpgaRegValue = {0};

#if !FPGA_SDIO_WRITE_USE_LOCAL
u8 sdioWriteBuffer[4] = {0};
#endif



/******************************************����ʵ��*******************************************/
/*********************************************************************************************
�� �� ��: servFpgaDataLenSet;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
void servFpgaDataLenSet(FpgaActionEnum fpgaAction, u16 realLen, u16 dataLen)
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
    u8 txState = 0;
    u8 regLen = FPGA_ADD_LEN_2BYTE + FPGA_REG_LEN_BYTE;
#if FPGA_SDIO_WRITE_USE_LOCAL
    u8 regData[4];
#endif
    

    //�������ݳ���
    servFpgaDataLenSet(FPGAACT_WRITE, regLen, regLen);

#if FPGA_SDIO_WRITE_USE_LOCAL    
    regData[0] = regAddr & FPGA_WREG_ADDR_MARK;
    regData[1] = chanNum & FPGA_WCHAN_NUM_MARK;

    if (regData[1] > 0x0F)
    {
        regData[1] = chanNum;
    }
    
    regData[2] = (u8)(regValue & 0xFF);    //��8λ
    regData[3] = (u8)((regValue & 0xFF00) >> 8);    //��8λ

    //��������
    txState = bspSdioDataSend(regData, regLen, SDIO_BLK_4_Byte);
    
#else

    sdioWriteBuffer[0] = regAddr & FPGA_WREG_ADDR_MARK;
    sdioWriteBuffer[1] = chanNum & FPGA_WCHAN_NUM_MARK;

    if (sdioWriteBuffer[1] > 0x0F)
    {
        sdioWriteBuffer[1] = chanNum;
    }
    
    sdioWriteBuffer[2] = (u8)(regValue & 0xFF);    //��8λ
    sdioWriteBuffer[3] = (u8)((regValue & 0xFF00) >> 8);    //��8λ

    //��������
    txState = bspSdioDataSend(sdioWriteBuffer, regLen, SDIO_BLK_4_Byte);
#endif

    return txState;
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
    u8  rxState = 0;
    u16 tempReg;
    u32 timeout = 33600;    //����DELAY_COUNT_MS = 33600���˳�ʱʱ���ԼΪ1ms;

    u8   readChan;
    u16  readRegAddr;
    u16  readReg;
    u8   readCount = FPGA_SDIO_RX_COUNT_MAX;
    bool readRight = false;


    if (pRegValue != NULL)
    {
        //���ò�����������
        while (!readRight && (readCount > 0))
        {
            //����SDIO����DMA
            bspSdioDataReceiveSet(regValue, regLen, SDIO_BLK_4_Byte);
            
            //�������ݳ���
            servFpgaDataLenSet(FPGAACT_READ, regLen, regLen);

            //���ö�ȡ���߼��Ĵ�����ַ
            tempReg = (regAddr & FPGA_RREG_ADDR_MARK) | ((chanNum << FPGA_RCHAN_NUM_LSHIFT) & FPGA_RCHAN_NUM_MARK);
            servFpgaReadAddrSet(tempReg);

            //���ɳ�ʱ�ȴ���ͬʱ�����ͷ�CPUʹ��Ȩ
            while ((!bspSdioDataTxComplete()) && (timeout > 0))
            {
                timeout--;  
            }

            readCount--;
        
            //�ж�ͨ���źͼĴ�����ַ�Ƿ�һ��
            //readRegAddr = regValue[0] & FPGA_WREG_ADDR_MARK;
            //readChan    = regValue[1] & FPGA_WCHAN_NUM_MARK;
            readReg = ((u16)regValue[1] << 8) + regValue[0];
            readRegAddr = readReg & FPGA_RREG_ADDR_MARK;
            readChan    = (readReg & FPGA_RCHAN_NUM_MARK) >> FPGA_RCHAN_NUM_LSHIFT;

            if ((readChan == chanNum) && (readRegAddr == regAddr))
            {
                readRight = true;
        
                //ǰ�����ֽ��ǵ�ַ
                *pRegValue  = regValue[2];         //��8λ 
                *pRegValue |= regValue[3] << 8;    //��8λ 

            }    //if ((readChan == chanNum) && (readRegAddr == regAddr))
        }    //while (!readRight && (readCount > 0))

        if (!readRight)
        {
            rxState = 1;    //Rxʧ��
        }
    }
  
    return rxState;
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

            //�ж������ݳ��ȣ�������ȳ�����FLASH����ķ�Χ��ֱ���˳�
            if ((dataLen > FLASH_FPGA_CODE_LEN) || (dataLen < 0))
            {
                errorCode = 2;
                break;
            }
            
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

    
    //���߼��汾��
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_VERSION_REG, &tmpReg);
    
    fpgaVersion.majorVer = (u8)((tmpReg & SERV_FPGA_VERSION_MAJOR_MASK) >> SERV_FPGA_VERSION_MAJOR_RSHIFT);
    fpgaVersion.minorVer = (u8)((tmpReg & SERV_FPGA_VERSION_MINOR_MASK) >> SERV_FPGA_VERSION_MINOR_RSHIFT);
    fpgaVersion.buildVer = (u8)(tmpReg & SERV_FPGA_VERSION_BUILD_MASK);


    //����Ʒ�ͺ�
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_TYPE_REG, &tmpReg);
    
    fpgaVersion.majorType = (u8)((tmpReg & SERV_FPGA_TYPE_MAJOR_MASK) >> SERV_FPGA_TYPE_MAJOR_RSHIFT);
    fpgaVersion.minorType = (u8)((tmpReg & SERV_FPGA_TYPE_MINOR_MASK) >> SERV_FPGA_TYPE_MINOR_RSHIFT);
    fpgaVersion.hardVer   = (u8)(tmpReg & SERV_FPGA_TYPE_HARD_MASK);
    
    
    return fpgaVersion;
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
        servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_10, &state);
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
�� �� ��: servFpgaWaveDataStateRead
ʵ�ֹ���: ��ȡDDR״̬�Ƿ���Կ�ʼִ�� DDR ����PWM
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
*********************************************************************************************/
s32 servFpgaWaveDataStateRead(u8 chanNum)
{
    FpgaResponseReg10Union responseReg;

    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_10, &responseReg.regValue);

    if (responseReg.regBitFiled.waveDataReady)
    {
        return 0;
    }
    else
    {
        return -1;
    }

}


/*********************************************************************************************
�� �� ��: servFpgaDDRWriteSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDDRWriteSet(DDRWriteEnum ddrWrite)
{
    fpgaRegValue.register27.regBitFiled.wrDdrFinish = ddrWrite;
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_27, fpgaRegValue.register27.regValue);
}


/*********************************************************************************************
�� �� ��: servFpgaDDRDataTypeSet
ʵ�ֹ���: ͨ��FPGA��������ʱ��
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDDRDataTypeSet(u8 chanNum, WaveDataTypeEnum dateType)
{
    fpgaRegValue.register23[chanNum].regBitFiled.waveType = dateType;
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_23, fpgaRegValue.register23[chanNum].regValue);
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
    FpgaResponseReg10Union responseReg;

    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_10, &responseReg.regValue);

    switch (responseReg.regBitFiled.waveState)
    {
        case WAVESTATE_IDLE1:
        case WAVESTATE_IDLE2:
            return RUNSTETE_NORUN;
          break;

        case WAVESTATE_SD:
            return RUNSTETE_SD;
          break;
          
        case WAVESTATE_OUTP1:
        case WAVESTATE_OUTP2:
            return RUNSTETE_RUN;
          break;

        case WAVESTATE_HOLD:
            return RUNSTETE_HOLD;
          break;

        default:
            return RUNSTETE_ERROR;
          break;
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
    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_22, &data_L);
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_23, &data_H);
    
    *times = (data_H<<16)|data_L + 1;
}


/*********************************************************************************************
�� �� ��: servFpgaWaveWritableSizeRead
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: ���ȴ��߼���ȡ�߼���δ����Ĳ������ݵĸ���,
          Ȼ����FIFO�ܿռ���-δ������� = ������д��Ĳ������
*********************************************************************************************/
u32 servFpgaWaveWritableSizeRead(u8 chanNum, u32 totalSize)
{
    u16 regValue    = 0;
    u32 remainPoint = 0;
    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_03, &regValue);//FIFO�������� ��16bit
    remainPoint = regValue;

    regValue = 0;
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_04, &regValue);//FIFO�������� ��16bit
    remainPoint += (regValue << 16) & 0xFFFF0000;

    return (totalSize - remainPoint);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveDataStateRead
ʵ�ֹ���: ��ȡDDR״̬�Ƿ���Կ�ʼִ�� DDR ����PWM
�������: 
          
�������:
            
�� �� ֵ: 0
˵    ��: 
*********************************************************************************************/
u8 servFpgaSdioWrErrorRead(void)
{
    FpgaResponseReg06Union responseReg;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_06, &responseReg.regValue);

    return responseReg.regBitFiled.sdioWrError;
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
    else    //�ر�ʱ��ɸߵ�ƽ��ʾ����Uart��Txģʽ
    {
        levelState = LEVEL_HIGH;
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
#if GELGOOG_AXIS_10
            //��ʱ������ for debug 
            servFpgaRegisterWrite(chanNum, 52, registerValue);
#else

            servFpgaRegisterWrite(chanNum, SERV_FPGA_DI4_X3_FILTER_REG, registerValue);
#endif
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
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
            
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
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.interruptSrcReg[chanNum]);
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
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.interruptSrcReg[chanNum]);
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
                    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.interruptSrcReg[chanNum]);
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
            fpgaRegValue.register50[chanNum].regBitFiled.alarm = INT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
            
            fpgaRegValue.register32[chanNum].regBitFiled.sdSrcHard = BITOPT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
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

            servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);
            
            //�����ж�
            fpgaRegValue.register50[chanNum].regBitFiled.trigger = INT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);

            //��ʱ���ã������޸ĳ�ר�ŵĺ���������    NICK MARK
            fpgaRegValue.register32[chanNum].regBitFiled.sdSrcHard = BITOPT_ENABLE;
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
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
        fpgaRegValue.register50[chanNum].regBitFiled.waveEnd = INT_ENABLE;
    }
    else
    {
        fpgaRegValue.register50[chanNum].regBitFiled.waveEnd = INT_DISABLE;
    }
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
}


/*********************************************************************************************
�� �� ��: servFpgaIntrSourceGet
ʵ�ֹ���: ��ȡFPGA��ARM�жϵ�Դͷ
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
u8 servFpgaIntrSourceGet(u8 chanNum, u16 *intrSrcValue)
{
    return servFpgaRegisterRead(chanNum, SERV_FPGA_INTERRUPT_SRC_REG, intrSrcValue);
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
    fpgaRegValue.register32[chanNum].regBitFiled.stopDecType = SDTYPE_NONE;
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcEnd    = BITOPT_ENABLE;

    switch (stopConfigInfo.stopMode)
    {
        case STOPMODE_IMMED: 
            fpgaRegValue.register32[chanNum].regBitFiled.stopDecType = SDTYPE_IMMED;      //���ٷ�ʽ����ֹͣ
            fpgaRegValue.register32[chanNum].regBitFiled.sdSrcEnd    = BITOPT_DISABLE;    //��ֹ��ͣ���٣�ֱ��ֹͣ
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
            fpgaRegValue.register32[chanNum].regBitFiled.stopDecType = SDTYPE_SPEED;

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

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
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
    fpgaRegValue.register32[chanNum].regBitFiled.waveHoldSrc = WHSRC_NONE;

    if (waveHold)
    {
        fpgaRegValue.register32[chanNum].regBitFiled.waveHoldSrc = WHSRC_END;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
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
    if (WTWORKMODE_FIFO == waveWorkMode)
    {
        fpgaRegValue.register31[chanNum].regBitFiled.waveStopMode = WSTOPMODE_FIFO; 
    }
    else if (WTWORKMODE_CYCLE == waveWorkMode)
    {
        fpgaRegValue.register31[chanNum].regBitFiled.waveStopMode = WSTOPMODE_CYCLE; 
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[chanNum].regValue);
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
    fpgaRegValue.register31[chanNum].regBitFiled.lvtModify = modifyDuty; 

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[chanNum].regValue);
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
    //���ñ�����������
    fpgaRegValue.register39[chanNum].regBitFiled.encDivSelect = lineMult - SERV_FPGA_ENC_LINE_MULT_OFFSET;

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_39, fpgaRegValue.register39[chanNum].regValue);
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
    switch (motionMode)
    {
        case MTNMODE_PVT:
            fpgaRegValue.register31[chanNum].regBitFiled.motionMode = WMMODE_PVT;
          break;
          
        case MTNMODE_LVT_CORR:
            fpgaRegValue.register31[chanNum].regBitFiled.motionMode = WMMODE_LVT_CORR;
          break;
          
        case MTNMODE_LVT_NOCORR:
            fpgaRegValue.register31[chanNum].regBitFiled.motionMode = WMMODE_LVT_NOCORR;
          break;

        default:
          break;
          
    }
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[chanNum].regValue);
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
�� �� ��: servFpgaWaveTableInit
ʵ�ֹ���: ��λ����
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.07.11 Add
*********************************************************************************************/
void servFpgaWaveTableInit(WaveTableStruct *pWaveTable, WaveWorkModeEnum waveWorkMode)
{
    pWaveTable->writableAddr = pWaveTable->startAddr;
    pWaveTable->writableSize = pWaveTable->totalSize;
    
    pWaveTable->waveState = MTSTATE_IDLE;
    
    pWaveTable->waveWorkMode = waveWorkMode;
}


/*********************************************************************************************
�� �� ��: servFpgaWaveBufferReset
ʵ�ֹ���: Ԥȡǰ��λ����
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaWaveBufferReset(u8 chanNum)
{
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop = BITOPT_ENABLE;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
    
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop = BITOPT_DISABLE;
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
    fpgaRegValue.register32[chanNum].regBitFiled.waveReset = BITOPT_ENABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop  = BITOPT_ENABLE;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);

    fpgaRegValue.register32[chanNum].regBitFiled.waveReset = BITOPT_DISABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop  = BITOPT_DISABLE;
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


    //������ǲ����ַ����DDR��ַ�Ķ�Ӧ��ϵ��ddrAddr = waveAddr * 4
    ddrAddr = waveAddr * FPGA_DDR_WAVE_TABLE_POINT_LEN;
    
    fpgaRegValue.register24.regBitFiled.regData = (u16)(ddrAddr & 0xFFFF);
    fpgaRegValue.register25.regBitFiled.regData = (u16)((ddrAddr >> 16) & 0x0FFF);

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_24, fpgaRegValue.register24.regValue);
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_25, fpgaRegValue.register25.regValue);
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


    //������ǲ����ַ����DDR��ַ�Ķ�Ӧ��ϵ��ddrAddr = waveAddr * 4
    ddrAddr = waveAddr * FPGA_DDR_WAVE_TABLE_POINT_LEN;
    
    fpgaRegValue.register33[chanNum].regBitFiled.regData = (u16)(ddrAddr & 0xFFFF);
    fpgaRegValue.register34[chanNum].regBitFiled.regData = (u16)((ddrAddr >> 16) & 0x0FFF);

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_33, fpgaRegValue.register33[chanNum].regValue);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_34, fpgaRegValue.register34[chanNum].regValue);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveDateLenSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaWaveDateLenSet(u8 chanNum, u32 len)
{
    fpgaRegValue.register35[chanNum].regBitFiled.regData = (u16)(len & 0xFFFF);
    fpgaRegValue.register36[chanNum].regBitFiled.regData = (u16)(len >> 16);
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_35, fpgaRegValue.register35[chanNum].regValue);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_36, fpgaRegValue.register36[chanNum].regValue);
}


/*********************************************************************************************
�� �� ��: servFpgaWaveDateCycleSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaWaveDateCycleSet(u8 chanNum, u32 cycleNum)
{
    fpgaRegValue.register37[chanNum].regBitFiled.regData = (u16)(cycleNum & 0xFFFF);
    fpgaRegValue.register38[chanNum].regBitFiled.regData = (u16)(cycleNum >> 16);
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_37, fpgaRegValue.register37[chanNum].regValue);
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_38, fpgaRegValue.register38[chanNum].regValue);
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
    u8  txState = 0;
    s32 writeResult = 0;


    //����δ����
    if (MTSTATE_IDLE == pWaveTable->waveState)
    {    
        //�޸Ĳ���״̬
        pWaveTable->waveState = MTSTATE_CALCING;
        
        //������������
        servFpgaDDRDataTypeSet(chanNum, DATATYPE_WAVE);

        //��ʼ������
        servFpgaWaveInit(chanNum, *pWaveTable);

        //FIFOģʽ��ҪReset
        if (WTWORKMODE_FIFO == pWaveTable->waveWorkMode)
        {
            servFpgaWaveFifoReset(chanNum);

            //��һ����������Ԥȡ��ʱ������Ҳ�ǿ��еģ�Ŀǰ�Ȱ��ɵ��������ã��������Ժ����޸�    NICK MARK
            servFpgaWaveDateLenSet(chanNum, pWaveTable->totalSize);
        }
    }
    
    //ͨ�������л�
    if (lastChan != chanNum)
    {
        lastChan = chanNum;

        //������������
        servFpgaDDRDataTypeSet(chanNum, DATATYPE_WAVE);
    }

    //��ȡ�߼�״̬������Ƿ���Խ��в����·�
    writeResult = servFPGA_State_If_WR_Allow(chanNum);    
    if (writeResult != 0)
    {
        return writeResult;
    }
        
    //���ÿ�ʼ��־
    servFpgaDDRWriteSet(DDRWR_START);
        
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
        txState = bspSdioDataSend((u8 *)&pOutputData->writeAddr, pOutputData->toSendBytes, pOutputData->toSendBlkSize);

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
            txState = bspSdioDataSend((u8 *)&pOutputData->emptyDataAddr, pOutputData->emptyBytes, pOutputData->emptyBlkSize);

            //���²����д��ַ�ʹ�С
            pWaveTable->writableAddr += pOutputData->validEmpPoints;
            pWaveTable->writableSize -= pOutputData->validEmpPoints;

            pOutputData->bSendEmpty  = false;
        }
    }
    
    //���ý�����־
    servFpgaDDRWriteSet(DDRWR_FINISH);

    //���һ֡����
    /*if (pOutputData->bLastFrame)
    {
        if (WTWORKMODE_FIFO != pWaveTable->waveWorkMode)
        {
            pWaveTable->waveState = MTSTATE_CALCEND;
        }
    }*/

    return (writeResult | txState);
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
    fpgaRegValue.register32[chanNum].regBitFiled.wavePrepare = BITOPT_ENABLE;

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);  
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
    fpgaRegValue.register32[chanNum].regBitFiled.wavePrepare = BITOPT_DISABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveOutput  = BITOPT_DISABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveStop    = BITOPT_DISABLE;
    
    if (outputEnable)
    {
        fpgaRegValue.register32[chanNum].regBitFiled.waveOutput = BITOPT_ENABLE;
    }
    else
    {
        fpgaRegValue.register32[chanNum].regBitFiled.waveOutput = BITOPT_DISABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
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
    fpgaRegValue.register32[CH_SYS].regBitFiled.waveRunAll = BITOPT_ENABLE;

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[CH_SYS].regValue);
    
    fpgaRegValue.register32[CH_SYS].regBitFiled.waveRunAll = BITOPT_DISABLE;
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
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcSoft  = BITOPT_ENABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveOutput = BITOPT_DISABLE;    //�������λ
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
    
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcSoft = BITOPT_DISABLE;    //ֹͣ����ͺ󱾵�״̬���븴λ  ��ֹ�´�����
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
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcSoft  = BITOPT_ENABLE;
    fpgaRegValue.register32[chanNum].regBitFiled.waveOutput = BITOPT_DISABLE;    //�������λ
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_32, fpgaRegValue.register32[chanNum].regValue);
    
    fpgaRegValue.register32[chanNum].regBitFiled.sdSrcSoft = BITOPT_DISABLE;    //ֹͣ����ͺ󱾵�״̬���븴λ  ��ֹ�´�����    
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
    //�����¼���ֹͣ�����ò���(����ӿں�FPGA�ӿڵ����ݸ�ʽ��һ������Ҫת����)
    servFpgaWaveStopConfig(&(pOutpWaveTable->stopConfigInfo), wavePlanInfo.stopDecelInfo, posnConvertInfo.posnToStep);

    //���ý���״̬�ͼ���ֹͣ
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
        servFpgaWaveDateCycleSet(chanNum, wavePlanInfo.cycleNum);
    }
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
    servFpgaWaveBufferReset(chanNum);

    //���ò�����ʼ��ַ�ͳ���
    if (waveTableAddr.waveWorkMode != WTWORKMODE_FIFO)    
    {
        servFpgaWaveDateLenSet(chanNum, waveTableAddr.totalSize - waveTableAddr.writableSize);
    }
    /*else    //������FIFOģʽҲ��������������ó��ȣ�Ŀǰ�Ȱ������̣��Ȳ��Թ��˺����޸�    NICK MARK
    {
        servFpgaWaveDateLenSet(waveTableAddr.totalSize);
    }*/

    //���ò�����ʼ��ַ
    servFpgaWaveStartDdrAddrSet(chanNum, waveTableAddr.startAddr);
    
    //�����߼���ʼԤȡ����
    servFpgaWavePrefetch(chanNum);
    
    //��ѯ�Ƿ���Կ�ʼ
    do
    {
        state = servFpgaWaveDataStateRead(chanNum); 
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
              
#endif
                bspEncoderTimerInit(chanNum, encMult, ENABLE);

              break;

            default:
              break;
        }
        servFpgaRegisterWrite(chanNum, SERV_FPGA_TRIG_REL_REG, fpgaRegValue.trigRelatRefCfgReg[chanNum]);


        if (bEncMult)
        {
            //���ñ���������
            switch (encMult)
            {
                case ENCMULT_SINGLE:
                    fpgaRegValue.register39[chanNum].regBitFiled.encMultSelect = EMSEL_NONE;
                  break;

                case ENCMULT_DOUBLE:
                    fpgaRegValue.register39[chanNum].regBitFiled.encMultSelect = EMSEL_DOUBLE;
                  break;

                case ENCMULT_QUADR:
                    fpgaRegValue.register39[chanNum].regBitFiled.encMultSelect = EMSEL_QUADR;
                  break;

                default:
                  break;
            }

            //����ͨ����
            switch (encChan)
            {
                case ECCHAN_3:
                    fpgaRegValue.register39[chanNum].regBitFiled.encSingleSel = BITOPT_DISABLE;
                  break;
            
                case ECCHAN_1:
                    fpgaRegValue.register39[chanNum].regBitFiled.encSingleSel = BITOPT_ENABLE;
                  break;

                default:
                  break;
            }
            
            servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_39, fpgaRegValue.register39[chanNum].regValue);
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
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_39, fpgaRegValue.register39[chanNum].regValue);
#endif
}


/*********************************************************************************************
�� �� ��: servFpgaEncoderCountRead
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
void servFpgaLineOutOfStepSet(u8 chanNum, SensorStateEnum state, f32 lineSteps, u16 threshold)
{
    //u16 thresholdL;
    u16 thresholdH;


    if (SENSOR_ON == state)
    {
        fpgaRegValue.register50[chanNum].regBitFiled.outOfStep = INT_ENABLE;

        //thresholdL = (u16)(lineSteps - threshold);
        thresholdH = (u16)(lineSteps + threshold);
        
        //Ŀǰ�������������������0
        //servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_57, thresholdL);
        servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_57, 0);
        
        servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_58, thresholdH);
    }
    else
    {
        fpgaRegValue.register50[chanNum].regBitFiled.outOfStep = INT_DISABLE;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
}


/*********************************************************************************************
�� �� ��: servFpgaSdioCrcErrorIntSet
ʵ�ֹ���: ����SDIO CRC�����ж�
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2018.05.17 Add
*********************************************************************************************/
void servFpgaSdioCrcErrorIntSet(SensorStateEnum state)
{
    if (SENSOR_ON == state)
    {
        fpgaRegValue.register50[CH_SYS].regBitFiled.sdioCrcErr = INT_ENABLE;
    }
    else
    {
        fpgaRegValue.register50[CH_SYS].regBitFiled.sdioCrcErr = INT_DISABLE;
    }

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[CH_SYS].regValue);
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
    fpgaRegValue.register16.regBitFiled.drvSrc = DRIVER_ARM;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);    //ͨ���Ź̶�Ϊ0
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
#if GELGOOG_AXIS_4 || GELGOOG_SINANJU
    fpgaRegValue.register16.regBitFiled.drvSpiSelect = chanNum;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
#endif
}


/*********************************************************************************************
�� �� ��: servFpgaDriverConfig;
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 servFpgaDriverConfig(u8 regNum, u32 regVaule, DrvConfActionEnum drvConfAct)
{
    fpgaRegValue.register17.regValue = (u16)regVaule;    //��16bit
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_17, fpgaRegValue.register17.regValue);
    
    fpgaRegValue.register18.regBitFiled.drvData = (u16)(regVaule >> 16);    //��4bit
    fpgaRegValue.register18.regBitFiled.drvRead = drvConfAct;
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_18, fpgaRegValue.register18.regValue);
    
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
    fpgaRegValue.register31[chanNum].regBitFiled.startSrcSoft = BITOPT_DISABLE;
    fpgaRegValue.register31[chanNum].regBitFiled.startSrcHard = BITOPT_DISABLE;
    fpgaRegValue.register31[chanNum].regBitFiled.startSrcCan  = BITOPT_DISABLE;

    switch (startSource)
    {
        case SSRC_SOFT:
        default:
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcSoft = BITOPT_ENABLE;
          break;

        case SSRC_HARD:
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcHard = BITOPT_ENABLE;
          break;

        case SSRC_CAN:
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcCan = BITOPT_ENABLE;
          break;

        case SSRC_ALL:
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcSoft = BITOPT_ENABLE;
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcHard = BITOPT_ENABLE;
            fpgaRegValue.register31[chanNum].regBitFiled.startSrcCan  = BITOPT_ENABLE;
          break;
    }

    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[chanNum].regValue);
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


    fpgaRegValue.register31[CH_SYS].regBitFiled.startSrcCan = BITOPT_DISABLE;

    switch (ClockSyncReg)
    {
        case CLOCKREG_ON:
            fpgaRegValue.register31[CH_SYS].regBitFiled.clkSynState = CSSTATE_RESET;
            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[CH_SYS].regValue);

            fpgaRegValue.register31[CH_SYS].regBitFiled.clkSynState = CSSTATE_ON;
            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[CH_SYS].regValue);
          break;

        case CLOCKREG_OFF:
            fpgaRegValue.register31[CH_SYS].regBitFiled.clkSynState = CSSTATE_OFF;
            servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[CH_SYS].regValue);
          break;
          
        default:
          break;
    }

    switch (receiveType)
    {
        case RECEIVE_RADIOID:
            canId = canIntfc.radioId;
          break;
          
        case RECEIVE_GROUPID1:
            canId = canIntfc.groupId1;
          break;
          
        case RECEIVE_GROUPID2:
            canId = canIntfc.groupId2;
          break;
          
        case RECEIVE_RECEIVEID:
            canId = canIntfc.receiveId;
          break;

        default:
          break;
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
    FpgaResponseReg10Union responseReg;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_10, &responseReg.regValue);

    if (responseReg.regBitFiled.ddrInitDone)
    {
        if (responseReg.regBitFiled.ddrCalibError)
        {
            return DDR_TESTERROR;
        }
        else
        {
            return DDR_NOERROR;
        }
    }
    else
    {
        return DDR_NOINIT;
    }
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
�� �� ��: servFpgaSensorUart1TxSelect
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaSensorUart1TxSelect(SensorNumEnum sensorNum)
{
#ifdef PROJECT_GELGOOG

    switch (sensorNum)
    {
        case SENSOR_S1:
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN1;
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN1_TX;
          break;

        case SENSOR_S2:
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN2;
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN2_TX;
          break;

        case SENSOR_S3:
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN3;
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN3_TX;
          break;

        case SENSOR_S4:
            fpgaRegValue.encandUartReg &= ~SERV_FPGA_UART_TX_MASK_SEN4;
            fpgaRegValue.encandUartReg |= SERV_FPGA_UART_TX_SEN4_TX;
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
void servFpgaReverseMotionSet(SensorStateEnum  revMotion)
{
    switch (revMotion)
    {
        case SENSOR_OFF:
        default:          
            fpgaRegValue.register31[CH_SYS].regBitFiled.dirReverse = BITOPT_DISABLE;
          break;

        case SENSOR_ON:          
            fpgaRegValue.register31[CH_SYS].regBitFiled.dirReverse = BITOPT_ENABLE;
          break;
    }

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_31, fpgaRegValue.register31[CH_SYS].regValue);
}


/*********************************************************************************************
�� �� ��: servFpgaDioRefStatusRead
ʵ�ֹ���: ��ȡ��λ����״̬
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2018.03.10 Add
*********************************************************************************************/
u16 servFpgaDioRefStatusRead(void)
{
    u16 registerValue;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_07, &registerValue);

    return registerValue;
}


/*********************************************************************************************
�� �� ��: servFpgaDioPortStatusRead
ʵ�ֹ���: ��ȡ���ֶ˿�״̬
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2018.03.10 Add
*********************************************************************************************/
u16 servFpgaDioPortStatusRead(void)
{
    u16 registerValue;

    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_DIO_STATUS_REG, &registerValue);

    return registerValue;
}


#if GELGOOG_AXIS_10

/*********************************************************************************************
�� �� ��: servFpgaDriverResetSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDriverResetSet(DriverResetEnum drvReset)
{
    fpgaRegValue.register16.regBitFiled.drvReset = drvReset;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}

/*********************************************************************************************
�� �� ��: servFpgaDriverModeSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDriverModeSet(MicroStepEnum microStep)
{
    DriverModeEnum drvMode;
    

    switch (microStep)
    {
        case MICROSTEP_256:
            drvMode = DRIVER_MODE256;
          break;

        case MICROSTEP_128:
            drvMode = DRIVER_MODE128;
          break;

        case MICROSTEP_32:
            drvMode = DRIVER_MODE32;
          break;

        case MICROSTEP_16:
            drvMode = DRIVER_MODE16;
          break;

        case MICROSTEP_8:
            drvMode = DRIVER_MODE8;
          break;

        case MICROSTEP_4:
            drvMode = DRIVER_MODE4;
          break;

        case MICROSTEP_2:
            drvMode = DRIVER_MODE2;
          break;

        case MICROSTEP_1:
            drvMode = DRIVER_MODE1;
          break;

        default:
          break;
    }
    
    fpgaRegValue.register16.regBitFiled.drvMode = drvMode;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}

/*********************************************************************************************
�� �� ��: servFpgaDriverDecaySet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDriverDecaySet(DriverDecayEnum drvDecay)
{
    fpgaRegValue.register16.regBitFiled.drvDecay = drvDecay;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}


/*********************************************************************************************
�� �� ��: servFpgaDriverSourceSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDriverSourceSet(DriverSourceEnum drvSrc)
{
    fpgaRegValue.register16.regBitFiled.drvSrc = drvSrc;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}


/*********************************************************************************************
�� �� ��: servFpgaDriverEnable
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDriverEnable(u8 chanNum, SensorStateEnum  state)
{
    bool bConfigReg = true;

    
    switch (chanNum)
    {
        case 0:
            fpgaRegValue.register17.regBitFiled.drvEnable0 = state;
          break;

        case 1:
            fpgaRegValue.register17.regBitFiled.drvEnable1 = state;
          break;

        case 2:
            fpgaRegValue.register17.regBitFiled.drvEnable2 = state;
          break;

        case 3:
            fpgaRegValue.register17.regBitFiled.drvEnable3 = state;
          break;

        case 4:
            fpgaRegValue.register17.regBitFiled.drvEnable4 = state;
          break;

        case 5:
            fpgaRegValue.register17.regBitFiled.drvEnable5 = state;
          break;

        case 6:
            fpgaRegValue.register17.regBitFiled.drvEnable6 = state;
          break;

        case 7:
            fpgaRegValue.register17.regBitFiled.drvEnable7 = state;
          break;

        case 8:
            fpgaRegValue.register17.regBitFiled.drvEnable8 = state;
          break;

        case 9:
            fpgaRegValue.register17.regBitFiled.drvEnable9 = state;
          break;

        default:
            bConfigReg = false;
          break;
    }

    if (bConfigReg)
    {
        servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_17, fpgaRegValue.register17.regValue);
    }
}


/*********************************************************************************************
�� �� ��: servFpgaDriverFaultRead
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
u16 servFpgaDriverFaultRead(void)
{
    u16 regValue;
    

    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_12, &regValue);

    return regValue;
}


#else


/*********************************************************************************************
�� �� ��: servFpgaDriverClkCofing
ʵ�ֹ���: ͨ��FPGA��������ʱ��
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaDriverClkCofing(u8 chanNum, DriverClkStateEnum driverClkState)
{
    if (CH1 == chanNum)
    {
        fpgaRegValue.register16.regBitFiled.drvSysClk1 = driverClkState;
    }
#ifdef PROJECT_GELGOOG

    else
    {
        switch (chanNum)
        {
            case CH2:
                fpgaRegValue.register16.regBitFiled.drvSysClk2 = driverClkState;
              break;
              
            case CH3:
                fpgaRegValue.register16.regBitFiled.drvSysClk3 = driverClkState;
              break;
              
            case CH4:
                fpgaRegValue.register16.regBitFiled.drvSysClk4 = driverClkState;
              break;

#if GELGOOG_SINANJU             
            case CH5:
                fpgaRegValue.register16.regBitFiled.drvSysClk5 = driverClkState;
              break;
#endif

#if GELGOOG_AXIS_8              
            case CH6:
                fpgaRegValue.register16.regBitFiled.drvSysClk6 = driverClkState;
              break;
              
            case CH7:
                fpgaRegValue.register16.regBitFiled.drvSysClk7 = driverClkState;
              break;
              
            case CH8:
                fpgaRegValue.register16.regBitFiled.drvSysClk8 = driverClkState;
              break;
#endif

            default:
              break;
        }
    }
    
#endif

    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_16, fpgaRegValue.register16.regValue);
}
#endif


/*********************************************************************************************
�� �� ��: servFpgaBackLashReset
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaBackLashReset(BackLashChanEnum blChan)
{
    fpgaRegValue.register28.regBitFiled.chSelect = blChan;
    fpgaRegValue.register28.regBitFiled.blReset  = BLRESET_ON;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_28, fpgaRegValue.register28.regValue);
}

/*********************************************************************************************
�� �� ��: servFpgaBackLashReset
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
u16 servFpgaBackLashRead(BackLashChanEnum blChan)
{
    u16 backLash;

    
    servFpgaRegisterRead(blChan, SERV_FPGA_RESPONSE_REG_08, &backLash);

    return backLash;
}


#if  GELGOOG_SINANJU
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
    servFpgaRegisterWrite(sensorNum, SERV_FPGA_ABS_ENC_REDA_REG, SERV_FPGA_ABS_ENC_REDA_CMD);
}


/*********************************************************************************************
�� �� ��: servFpgaPdmSampleReset
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaPdmSampleReset(SampleChanEnum sampleChan)
{
    fpgaRegValue.register29.regBitFiled.chSelect    = sampleChan;
    fpgaRegValue.register29.regBitFiled.sampleState = SAMPLESTATE_RESET;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
}

/*********************************************************************************************
�� �� ��: servFpgaPdmSampleStart
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaPdmSampleStart(SampleChanEnum sampleChan)
{
    fpgaRegValue.register29.regBitFiled.chSelect    = sampleChan;
    fpgaRegValue.register29.regBitFiled.sampleState = SAMPLESTATE_START;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
}

/*********************************************************************************************
�� �� ��: servFpgaPdmSampleEnd
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaPdmSampleEnd(SampleChanEnum sampleChan)
{
    fpgaRegValue.register29.regBitFiled.chSelect    = sampleChan;
    fpgaRegValue.register29.regBitFiled.sampleState = SAMPLESTATE_END;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
}

/*********************************************************************************************
�� �� ��: servFpgaPdmEncDivSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaPdmEncDivSet(SampleChanEnum sampleChan, u8 encDiv)
{
    fpgaRegValue.register29.regBitFiled.chSelect = sampleChan;
    fpgaRegValue.register29.regBitFiled.encDiv   = encDiv - 1;    //FPGAҪ���1
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
}

/*********************************************************************************************
�� �� ��: servFpgaPdmSampleStateSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaPdmSampleStateSet(SampleChanEnum sampleChan, PdmInfoStruct pdmInfo)
{
    if (SENSOR_ON == pdmInfo.sampleState)
    {
        servFpgaPdmSampleReset(sampleChan);

        servFpgaPdmEncDivSet(sampleChan, pdmInfo.encDiv);

        servFpgaPdmSampleStart(sampleChan);
    }
    else
    {
        servFpgaPdmSampleEnd(sampleChan);
    }
}


/*********************************************************************************************
�� �� ��: servFpgaPdmMstepCountRead
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
u16 servFpgaPdmMstepCountRead(SampleChanEnum sampleChan)
{
    u16 regValue;
        
    
    servFpgaRegisterRead(CH_SYS, SERV_FPGA_RESPONSE_REG_09, &regValue);

    return (regValue + SERV_FPGA_MSTEP_COUNT_OFFSET);
}


/*********************************************************************************************
�� �� ��: servFpgaPdmMstepDataRead
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
u8 servFpgaPdmMstepDataRead(SampleChanEnum sampleChan, u8 *pRegValue, u16 bufferSize)
{
    u8  rxState = 0;
    u16 regLen = FPGA_ADD_LEN_2BYTE + bufferSize;
    u16 tempReg;
    u32 timeout = 33600;    //����DELAY_COUNT_MS = 33600���˳�ʱʱ���ԼΪ1ms;


    if (pRegValue != NULL)
    {
        //����SDIO����
        bspSdioDataReceiveSet(pRegValue, regLen, SDIO_BLK_512_Byte);
        
        //�������ݳ���
        servFpgaDataLenSet(FPGAACT_READ, regLen, regLen);

        //���ö�ȡ���߼��Ĵ�����ַ
        tempReg = (SERV_FPGA_RESPONSE_REG_11 & FPGA_RREG_ADDR_MARK) | 
                  ((sampleChan << FPGA_RCHAN_NUM_LSHIFT) & FPGA_RCHAN_NUM_MARK);
        servFpgaReadAddrSet(tempReg);

        //���ɳ�ʱ�ȴ���ͬʱ�����ͷ�CPUʹ��Ȩ
        while ((!bspSdioDataTxComplete()) && (timeout > 0))
        {
            timeout--;  
        }
    }
  
    return rxState;
}


/*********************************************************************************************
�� �� ��: servFpgaPdmMstepDataProcess
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaPdmMstepDataProcess(SampleChanEnum sampleChan, PdmInfoStruct pdmInfo)
{
    u8  data[FPGA_PDM_MSTEP_COUNT_LEN];
    bool bRemainder = false;
    bool bRemSend   = false;
    u16 i, j, k;
    u16 readCount;
    u16 sendCount;
    u8 *dataBuffer;
    u16 mstepCount = pdmInfo.readLen * sizeof(u16);                     //����ָ����U16�ĳ���
    u16 dataLen = PDM_MSTEP_BUFFER_SIZE - FPGA_ADD_LEN_2BYTE - FPGA_PDM_MSTEP_BUFFER_RESV;    //���ݲ������Ĵ�����ַ�ͱ�������


    //��Ҫ��ȡ�Ĵ���
    readCount = mstepCount / dataLen;
    if ((mstepCount % dataLen) != 0)
    {
        bRemainder = true;
    }

    //һ�ζ�ȡ����Ҫ���͵Ĵ���
    sendCount = dataLen / FPGA_PDM_MSTEP_COUNT_LEN;
    if ((dataLen % FPGA_PDM_MSTEP_COUNT_LEN) != 0)
    {
        sendCount++;
    }

    //��ʼ��ȡ������
    for (i = 0;i < readCount;i++)
    {
        //���ݶ�����
        servFpgaPdmMstepDataRead(sampleChan, pdmInfo.mstepData, dataLen);

        //���ͣ���ʵ��ѹ�뷢��Buffer��
        dataBuffer = (u8 *)pdmInfo.mstepData + FPGA_ADD_LEN_2BYTE;      //�����Ĵ�����ַ
        for (j = 0;j < sendCount;j++)
        {
            for (k = 0;k < FPGA_PDM_MSTEP_COUNT_LEN;k++)
            {
                data[k] = *dataBuffer++;
            }
            //cmdFrameSend(CMD_PDM, PDMCMD_MSTEPDATAQ, FPGA_PDM_MSTEP_COUNT_LEN, data);
            servFrameSend(data[0], data[1], FPGA_PDM_MSTEP_COUNT_LEN - 2, &data[2], g_commIntfc.linkType);
        }

        //��CMDPARSE�̷߳��ź�������ɷ���
        OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, NULL);

        mstepCount -= dataLen;
    }
    
    //����ʣ�ಿ��
    if (bRemainder)
    {
        //һ�ζ�ȡ����Ҫ���͵Ĵ���
        sendCount = mstepCount / FPGA_PDM_MSTEP_COUNT_LEN;
        if ((mstepCount % FPGA_PDM_MSTEP_COUNT_LEN) != 0)
        {
            bRemSend = true;
        }
        
        //���ݶ�������Ȼ��һ��һ�㷢����ȥ
        servFpgaPdmMstepDataRead(sampleChan, pdmInfo.mstepData, mstepCount);

        //���ͣ���ʵ��ѹ�뷢��Buffer��
        dataBuffer = (u8 *)pdmInfo.mstepData + FPGA_ADD_LEN_2BYTE;      //�����Ĵ�����ַ
        for (j = 0;j < sendCount;j++)
        {
            for (k = 0;k < FPGA_PDM_MSTEP_COUNT_LEN;k++)
            {
                data[k] = *dataBuffer++;
            }
            //cmdFrameSend(CMD_PDM, PDMCMD_MSTEPDATAQ, FPGA_PDM_MSTEP_COUNT_LEN, data);
            servFrameSend(data[0], data[1], FPGA_PDM_MSTEP_COUNT_LEN - 2, &data[2], g_commIntfc.linkType);

            mstepCount -= FPGA_PDM_MSTEP_COUNT_LEN;
        }

        //����ʣ���
        if (bRemSend)
        {
            //��̳���
            if (mstepCount < 2)
            {
                mstepCount = 2;
            }
            
            for (k = 0;k < mstepCount;k++)
            {
                data[k] = *dataBuffer++;
            }
            //cmdFrameSend(CMD_PDM, PDMCMD_MSTEPDATAQ, FPGA_PDM_MSTEP_COUNT_LEN, data);
            servFrameSend(data[0], data[1], mstepCount - 2, &data[2], g_commIntfc.linkType);
        }

        //��CMDPARSE�̷߳��ź�������ɷ���
        OSSemPost(&g_semCmdParseTask, OS_OPT_POST_ALL, NULL);
    }
    
}


/*********************************************************************************************
�� �� ��: servFpgaPdmCacheTestSet
ʵ�ֹ���: 
�������: 
�������:   
�� �� ֵ: 
˵    ��: 
*********************************************************************************************/
void servFpgaPdmCacheTestSet(void)
{
    fpgaRegValue.register29.regBitFiled.cacheMode = BITOPT_ENABLE;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_29, fpgaRegValue.register29.regValue);
    
    //fpgaRegValue.register29.regBitFiled.cacheMode = BITOPT_DISABLE;
}
#endif


/*********************************************************************************************
�� �� ��: servFpgaWaveEndIntClear
ʵ�ֹ���: �������������
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaWaveEndIntClear(u8 chanNum)
{
    fpgaRegValue.register50[chanNum].regBitFiled.waveEndClear = INT_ENABLE;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
    
    fpgaRegValue.register50[chanNum].regBitFiled.waveEndClear = INT_DISABLE;
}

/*********************************************************************************************
�� �� ��: servFpgaPeriodOverTxClear
ʵ�ֹ���: ������䵼�µ�΢�����ڳ�ʱ���
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaPeriodOverTxClear(void)
{
    fpgaRegValue.register50[CH_SYS].regBitFiled.pOvrTxClear  = INT_ENABLE;
    
    servFpgaRegisterWrite(CH_SYS, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[CH_SYS].regValue);
    
    fpgaRegValue.register50[CH_SYS].regBitFiled.pOvrTxClear  = INT_DISABLE;
}

/*********************************************************************************************
�� �� ��: servFpgaPeriodOverDdrClear
ʵ�ֹ���: ���DDR��ȡ���µ�΢�����ڳ�ʱ���
�������: 
�������:   
�� �� ֵ: 
˵    ��: CJ 2017.06.26 Add
*********************************************************************************************/
void servFpgaPeriodOverDdrClear(u8 chanNum)
{
    fpgaRegValue.register50[chanNum].regBitFiled.pOvrDdrClear = INT_ENABLE;
    
    servFpgaRegisterWrite(chanNum, SERV_FPGA_CONFIG_REG_50, fpgaRegValue.register50[chanNum].regValue);
    
    fpgaRegValue.register50[chanNum].regBitFiled.pOvrDdrClear = INT_DISABLE;
}


/*********************************************************************************************
�� �� ��: servFpgaPeriodStateRead
ʵ�ֹ���: ��; 
�������: ��;
�������: ��;
�� �� ֵ: ��;
˵    ��: ��;
*********************************************************************************************/
u8 servFpgaPeriodStateRead(u8 chanNum)
{
    u8 periodState;
    FpgaResponseReg10Union responseReg;

    
    servFpgaRegisterRead(chanNum, SERV_FPGA_RESPONSE_REG_10, &responseReg.regValue);


    periodState = responseReg.regBitFiled.periodOverTx       |
                  responseReg.regBitFiled.periodUnderTx  * 2 |
                  responseReg.regBitFiled.periodOverDdr  * 4 |
                  responseReg.regBitFiled.periodUnderDdr * 8;

    return periodState;
}



/*******************************************�ļ�β********************************************/
