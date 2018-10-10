/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  comErrorcode.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2016.08.25;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __COM_ERROR_CODE_H
#define __COM_ERROR_CODE_H



/*******************************************�궨��********************************************/
#define    ERROR_CODE_INDEX_PARA_VERIFY     0    //��������ڴ����������е��±�
#define    ERROR_CODE_INDEX_PVT_CALC        1    //PVT�����ڴ����������е��±�
#define    ERROR_CODE_INDEX_INTFACE         2    //�ӿڴ����ڴ����������е��±�
#define    ERROR_CODE_INDEX_FPGA            3    //FPGA�����ڴ����������е��±�
#define    ERROR_CODE_INDEX_MEMORY          4    //���й����г�������
#define    ERROR_CODE_INDEX_RESERVE         5    //����

/**********************************������֤***********************************/
#define    PARA_VERIFY_NO_ERROR            0
#define    PARA_VERIFY_ERROR_TYPE          1
#define    PARA_VERIFY_ERROR_LEN           2
#define    PARA_VERIFY_ERROR_INDEX         3
#define    PARA_VERIFY_LESS_LOWER_LIMIT    4
#define    PARA_VERIFY_GREAT_UPER_LIMIT    5
#define    PARA_VERIFY_OUT_RANGE           6   //������ö��֮��  cp 20170517
#define    PARA_VERIFY_ERROR_STATE         7

/**********************************PVT����***********************************/
#define    PVT_CALC_DDR_INSUFF_SPACE       1    //DDR�Ŀռ䲻��

/**********************************���Դ���***********************************/
#define    ENGINMODE_TEST_ERROR            1

/**********************************FPGA����***********************************/
#define    FPGA_VERIFY_NO_ERROR            0
#define    FPGA_DDR_VERIFY_ERROR           1
#define    FPGA_DDR_TEST_ERROR             2
#define    FPGA_WAVE_TABLE_NOCONFIG        3



/******************************************ȫ�ֱ���*******************************************/
 


/******************************************��������*******************************************/



#endif
/*****************************************END OF FILE****************************************/