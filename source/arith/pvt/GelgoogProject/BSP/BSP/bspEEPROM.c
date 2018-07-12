


#include "bspEEPROM.h"
#include "bspTimer.h"
/*********************************************************************************************
�� �� ��: AT24CXX_ReadBytes;
ʵ�ֹ���: ��AT24CXX�ж�ȡN������
�������:
�������: ��
�� �� ֵ: ��
˵    ��:�˺���ֻ�ṩԭʼ�Ķ�ȡ,û���޶�ҳ��ַ,���ڲ�ʹ��
*********************************************************************************************/
static void AT24CXX_ReadBytes(u16 ReadAddr,u8* data,u32 len)
{
    int i = 0;
    bspI2C_Start(); //��ʼλ
    //����д����
    bspI2C_SendByte(0xA0);
    bspI2C_WaitAck();
    //���Ͷ����ݵ�ַ
    bspI2C_SendByte(ReadAddr>>8);//�ߵ�ַ
    bspI2C_WaitAck();
    bspI2C_SendByte(ReadAddr&0xFF);//�͵�ַ
    bspI2C_WaitAck();
    //������ȡ
    bspI2C_Start();
    bspI2C_SendByte(0xA1);           //�������ģʽ
    bspI2C_WaitAck();
    for(i=0;i<len;i++)
    {
      data[i]=bspI2C_ReadByte(((i+1) == len)?0:1);//�����˳����Ļ�,Ҫ��ACK,��ΪEEPROMҪ����ACK���ӵ�ַ
    }
    bspI2C_Stop();//ֹͣλ
}

/*********************************************************************************************
�� �� ��: AT24CXX_WriteBytes;
ʵ�ֹ���: ��AT24CXX��д��N������,page write
�������:
�������: ��
�� �� ֵ: ��
˵    ��:�˺���ֻ�ṩԭʼ��д��,û���޶�ҳ��ַ,���ڲ�ʹ��
*********************************************************************************************/
static void AT24CXX_WriteBytes(u16 WriteAddr,u8* Data,u32 len)
{
    int i = 0;
    bspI2C_Start();//��ʼλ
    bspI2C_SendByte(0xA0);      //����д����
    bspI2C_WaitAck();
    bspI2C_SendByte(WriteAddr>>8);//���͸ߵ�ַ

    bspI2C_WaitAck();
    bspI2C_SendByte(WriteAddr&0xff);   //���͵͵�ַ
    bspI2C_WaitAck();
    for(i = 0;i < len;i++)
    {
        bspI2C_SendByte(Data[i]);     //�����ֽ�
        bspI2C_WaitAck();
    }
    bspI2C_Stop();//ֹͣλ
    bspDelayMs(10);//��ҳд��ʱ,��Ҫһ��С��ʱ
}
/*********************************************************************************************
�� �� ��: drvAT24CxxWrite
ʵ�ֹ���: ��EEPROM��д������
�������: u16 u16addr: Ŀ�ĵ�ַ         ��ַ��λΪ�ֽ� ��ΧΪ 0--16K
         u8 * src:    ����Դ��ַָ��
         u32 len:       Ҫд������ݳ���  ���ֽ�Ϊ��λ

�������: ��;
�� �� ֵ: 0     ���ͳɹ�,
          ����  ����ʧ��;
˵    ��: EEPROM������д��ʱ����ܻ�������ҳ������--��һҳ�ڵ�ַ��������,���ӵ�ҳĩβʱ�����Լӻᵽҳ��һ����ַ��,�����Ҫ��ҳ�߽���д���
*********************************************************************************************/
int bspAT24CxxWrite(u16 u16addr,u8 * src, u32 len)
{
    u8 u8PageCanWrite = 0;//��ĳһҳ��,����д���ٸ��ֽ�
    u32 u32left = len;//Ҫд�������,��ʣ����ٸ��ֽ�
    //��ַ��������Խ��
    if((src == 0) || (u16addr + len > EEPROM_SIZE_MAX))
    {
        return -1;
    }
    u8PageCanWrite = EEPROM_PAGE_SIZE - (u8)(u16addr&(EEPROM_PAGE_SIZE-1));//��һҳ�л���д�����ֽ�
    if(u32left <= u8PageCanWrite)//��ǰ�漴��д��,����Ҫ�����ҳ����
    {
        u8PageCanWrite = u32left;
        //AT24CXX_WriteBytes(u16addr,src,u8PageCanWrite);
        //return 0;
    }
    /*������Ҫ��ҳд�Ĳ���
     * ��д��һҳ,��һҳ�Ƚ�����,��Ҫ��������
     * */
    AT24CXX_WriteBytes(u16addr,src,u8PageCanWrite);
    u16addr += u8PageCanWrite; //��ַ����
    src += u8PageCanWrite;     //����ָ���ƶ�
    u32left -= u8PageCanWrite;//����ʣ���ֽ�

    while(u32left > 0)
    {
        if(u32left > EEPROM_PAGE_SIZE)
        {
            u8PageCanWrite = EEPROM_PAGE_SIZE;
        }
        else
        {
            u8PageCanWrite = u32left;
        }
        AT24CXX_WriteBytes(u16addr,src,u8PageCanWrite);
        u16addr += u8PageCanWrite; //��ַ����
        src += u8PageCanWrite;     //����ָ���ƶ�
        u32left -= u8PageCanWrite; //����ʣ���ֽ�
    }
    return 0;
}
/*********************************************************************************************
�� �� ��: drvAT24CxxRead
ʵ�ֹ���: ��EEPROM�ж�ȡ����
�������: u16 u16addr: Ŀ�ĵ�ַ         ��ַ��λΪ�ֽ� ��ΧΪ 0--16K
         u8 * src:    ����Դ��ַָ��
         u32 len:       Ҫ��ȡ�����ݳ���  ���ֽ�Ϊ��λ

�������: ��;
�� �� ֵ: 0     ���ͳɹ�,
          ����  ����ʧ��;
˵    ��: EEPROM������д��ʱ����ܻ�������ҳ������--��һҳ�ڵ�ַ��������,���ӵ�ҳĩβʱ�����Լӻᵽҳ��һ����ַ��,�����Ҫ��ҳ�߽���д���
*********************************************************************************************/
int bspAT24CxxRead(u16 u16addr,u8 * src, u32 len)
{
    u8 u8PageCanRead = 0;//��ĳһҳ��,���Զ����ٸ��ֽ�
    u32 u32left = len;//���ж��ٸ��ֽ�����Ҫȡ
    //��ַ��������Խ��
    if((src == 0) || (u16addr + len > EEPROM_SIZE_MAX))
    {
        return -1;
    }
    u8PageCanRead = EEPROM_PAGE_SIZE - (u8)(u16addr&(EEPROM_PAGE_SIZE-1));//��һҳ�л���д�����ֽ�
    if(len <= u8PageCanRead)//��ǰ�漴�ɶ���,����Ҫ�����ҳ����
    {
        u8PageCanRead = len;
    }
    /*������Ҫ��ҳ��ȡ�Ĳ���
     * �ȴӵ�һҳ�ж�ȡ,��һҳ�Ƚ�����,��Ҫ��������
     * */
    AT24CXX_ReadBytes(u16addr,src,u8PageCanRead);
    u16addr += u8PageCanRead; //��ַ����
    src += u8PageCanRead;     //����ָ���ƶ�
    u32left -= u8PageCanRead;//����ʣ���ֽ�

    while(u32left > 0)
    {
        if(u32left > EEPROM_PAGE_SIZE)
        {
            u8PageCanRead = EEPROM_PAGE_SIZE;
        }
        else
        {
            u8PageCanRead = u32left;
        }
        AT24CXX_ReadBytes(u16addr,src,u8PageCanRead);
        u16addr += u8PageCanRead; //��ַ����
        src += u8PageCanRead;     //����ָ���ƶ�
        u32left -= u8PageCanRead; //����ʣ���ֽ�
    }
    return 0;
}
extern void bspEEPROMTest();
/*********************************************************************************************
�� �� ��: bspEepromInit
ʵ�ֹ���: EEPROM��ʼ��
�������: 
�������: ��;
�� �� ֵ:
˵    ��: 
*********************************************************************************************/
void bspEepromInit()
{
    bspI2C_init();
    //bspEEPROMTest();
}