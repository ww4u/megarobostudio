/*********************************************************************************************
Copyright (C) 2016，北京镁伽机器人科技有限公司
文 件 名:  gelgoog.h;
功能描述:  ;
函数列表:  ;
当前版本:  1.0.0;
版本改动:  ;
作    者:  CJ;
完成日期:  2017.05.19;
历史版本:  无;
*********************************************************************************************/
#ifndef __GELGOOG_H
#define __GELGOOG_H



/******************************************包含文件*******************************************/



/*******************************************宏定义********************************************/
//#define    PROJECT_QUBELEY
//#define    PROJECT_GELGOOG

#if (defined (PROJECT_QUBELEY) && defined (PROJECT_GELGOOG))

    #error "Only one Project define can be defined at the same time!!!"
    
#endif


/***************************************QUBELEY****************************************/
#ifdef PROJECT_QUBELEY

//这两个定义只能同时有一个被define
#define    QUBELEY_HARDWARE_VER_1    //实际对应的硬件版本包括0.3、1.0和1.1
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

//这两个定义只能同时有一个被define，4轴或者8轴
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



/***************************************常数和类型声明****************************************/



#endif
/*****************************************END OF FILE****************************************/
