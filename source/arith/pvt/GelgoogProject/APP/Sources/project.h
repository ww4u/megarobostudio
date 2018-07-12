/*********************************************************************************************
Copyright (C) 2016������þ٤�����˿Ƽ����޹�˾
�� �� ��:  gelgoog.h;
��������:  ;
�����б�:  ;
��ǰ�汾:  1.0.0;
�汾�Ķ�:  ;
��    ��:  CJ;
�������:  2017.05.19;
��ʷ�汾:  ��;
*********************************************************************************************/
#ifndef __GELGOOG_H
#define __GELGOOG_H



/******************************************�����ļ�*******************************************/



/*******************************************�궨��********************************************/
//#define    PROJECT_QUBELEY
//#define    PROJECT_GELGOOG

#if (defined (PROJECT_QUBELEY) && defined (PROJECT_GELGOOG))

    #error "Only one Project define can be defined at the same time!!!"
    
#endif


/***************************************QUBELEY****************************************/
#ifdef PROJECT_QUBELEY

//����������ֻ��ͬʱ��һ����define
#define    QUBELEY_HARDWARE_VER_1    //ʵ�ʶ�Ӧ��Ӳ���汾����0.3��1.0��1.1
//#define    QUBELEY_HARDWARE_VER_2

#if (defined (QUBELEY_HARDWARE_VER_1) && defined (QUBELEY_HARDWARE_VER_2))

    #error "Only one Qubeley hardware version can be defined at the same time!!!"
    
#endif

#ifdef QUBELEY_HARDWARE_VER_1
 #define    QUBELEY_HARDVER_1    1
#endif

#ifdef QUBELEY_HARDWARE_VER_2

 #define    QUBELEY_HARDVER_2    1

#if QUBELEY_HARDVER_2
 #define    DRIVE_BOARD_TYPE_CONFIG_262    1
#endif

#endif    //#ifdef QUBELEY_HARDWARE_VER_2

#endif    //#ifdef PROJECT_QUBELEY
/***************************************QUBELEY****************************************/


/***************************************GELGOOG****************************************/
#ifdef PROJECT_GELGOOG

//����������ֻ��ͬʱ��һ����define��4�����8��
#define    GELGOOG_HARDWARE_AXIS_4
//#define    GELGOOG_HARDWARE_AXIS_8
//#define    GELGOOG_HARDWARE_SINANJU


#if (defined (GELGOOG_HARDWARE_AXIS_4) && defined (GELGOOG_HARDWARE_AXIS_8))  || \
    (defined (GELGOOG_HARDWARE_AXIS_4) && defined (GELGOOG_HARDWARE_SINANJU)) || \
    (defined (GELGOOG_HARDWARE_AXIS_8) && defined (GELGOOG_HARDWARE_SINANJU))

    #error "Only one Gelgoog hardware version can be defined at the same time!!!"
    
#endif

#ifdef GELGOOG_HARDWARE_AXIS_4
 #define    GELGOOG_AXIS_4    1
#endif

#ifdef GELGOOG_HARDWARE_AXIS_8
 #define    GELGOOG_AXIS_8    1
#endif

#ifdef GELGOOG_HARDWARE_SINANJU
 #define    GELGOOG_SINANJU   1
#endif

#endif
/***************************************GELGOOG****************************************/



/***************************************��������������****************************************/



#endif
/*****************************************END OF FILE****************************************/
