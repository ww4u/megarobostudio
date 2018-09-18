#include "visa_adapter.h"
#include "C_MRQ.h"
//!ENUM
//!NONE|CAN|RS232
int MRQ_setLINK_INTFC( ViSession vi, char * val0 )
{
//!建立连接命令，当使用CAN接口或者RS232接口进行初次连接时，需要发送此命令建立连接
//!LINK INTFC
    char args[SEND_BUF];
    sprintf( args, "LINK:INTFC %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_LINK_INTFC
//!val0
int MRQ_getLINK_INTFC( ViSession vi , char * val0 )
{
//!
//!LINK INTFC

    char args[SEND_BUF];
    sprintf( args, "LINK:INTFC?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!MRQ_LINK_DEVICEINFO,MRQ_LINK_DEVICEINFO_1,uint32
//!val0,val1,val2
int MRQ_getLINK_DEVICEINFO( ViSession vi , char * val0,char * val1,int *val2 )
{
//!设备信息查询，包括主型号、子型号以及哈希值
//!LINK DEVICEINFO

    char args[SEND_BUF];
    sprintf( args, "LINK:DEVICEINFO?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 3 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    strcpy(val1, paraAt( &para, 1 ) );
    *val2 = atoi( paraAt( &para, 2) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!NORMAL|NONE
int MRQ_setSYSTEM_WORKMODE( ViSession vi, char * val0 )
{
//!设置工作模式
//!SYSTEM WORKMODE
    char args[SEND_BUF];
    sprintf( args, "SYSTEM:WORKMODE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_SYSTEM_WORKMODE
//!val0
int MRQ_getSYSTEM_WORKMODE( ViSession vi , char * val0 )
{
//!查询当前工作模式
//!SYSTEM WORKMODE

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:WORKMODE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!DEFAULT|LAST
int MRQ_setSYSTEM_POWERON( ViSession vi, char * val0 )
{
//!设置上电状态
//!SYSTEM POWERON
    char args[SEND_BUF];
    sprintf( args, "SYSTEM:POWERON %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_SYSTEM_POWERON
//!val0
int MRQ_getSYSTEM_POWERON( ViSession vi , char * val0 )
{
//!查询上电状态
//!SYSTEM POWERON

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:POWERON?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!byte,char,char,char,char,char
//!val0,val1,val2,val3,val4,val5
int MRQ_getSYSTEM_SN( ViSession vi , int *val0,int *val1,int *val2,int *val3,int *val4,int *val5 )
{
//!查询序列号，序列号长度大于6，所以无法一次发送完，需要多次发送，参数的第一个字节代表SN的段，后5个字节是当前段的数据
//!SYSTEM SN

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:SN?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 6 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    *val1 = atoi( paraAt( &para, 1) );
    *val2 = atoi( paraAt( &para, 2) );
    *val3 = atoi( paraAt( &para, 3) );
    *val4 = atoi( paraAt( &para, 4) );
    *val5 = atoi( paraAt( &para, 5) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!MRQ_LINK_DEVICEINFO,MRQ_LINK_DEVICEINFO_1
//!val0,val1
int MRQ_getSYSTEM_TYPE( ViSession vi , char * val0,char * val1 )
{
//!查询产品类型，子类型只标注了MRQ的子类型
//!SYSTEM TYPE

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:TYPE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    strcpy(val1, paraAt( &para, 1 ) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!char,char,char
//!val0,val1,val2
int MRQ_getSYSTEM_SOFTVER( ViSession vi , int *val0,int *val1,int *val2 )
{
//!查询系统软件版本，数字
//!SYSTEM SOFTVER

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:SOFTVER?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 3 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    *val1 = atoi( paraAt( &para, 1) );
    *val2 = atoi( paraAt( &para, 2) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!char,char,char,char,char,char
//!val0,val1,val2,val3,val4,val5
int MRQ_getSYSTEM_FPGAVER( ViSession vi , int *val0,int *val1,int *val2,int *val3,int *val4,int *val5 )
{
//!查询系统FPGA版本，数字
//!SYSTEM FPGAVER

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:FPGAVER?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 6 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    *val1 = atoi( paraAt( &para, 1) );
    *val2 = atoi( paraAt( &para, 2) );
    *val3 = atoi( paraAt( &para, 3) );
    *val4 = atoi( paraAt( &para, 4) );
    *val5 = atoi( paraAt( &para, 5) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!char,char
//!val0,val1
int MRQ_getSYSTEM_HARDVER( ViSession vi , int *val0,int *val1 )
{
//!查询系统硬件版本，数字
//!SYSTEM HARDVER

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:HARDVER?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    *val1 = atoi( paraAt( &para, 1) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!char,char
//!val0,val1
int MRQ_getSYSTEM_BOOTVER( ViSession vi , int *val0,int *val1 )
{
//!查询系统BOOT版本，数字
//!SYSTEM BOOTVER

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:BOOTVER?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    *val1 = atoi( paraAt( &para, 1) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!char,char,char,char,char,char
//!val0,val1,val2,val3,val4,val5
int MRQ_getSYSTEM_EVENTCODE( ViSession vi , int *val0,int *val1,int *val2,int *val3,int *val4,int *val5 )
{
//!查询系统错误，错误码
//!SYSTEM EVENTCODE

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:EVENTCODE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 6 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    *val1 = atoi( paraAt( &para, 1) );
    *val2 = atoi( paraAt( &para, 2) );
    *val3 = atoi( paraAt( &para, 3) );
    *val4 = atoi( paraAt( &para, 4) );
    *val5 = atoi( paraAt( &para, 5) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!uint16
//!val0
int MRQ_getSYSTEM_DIOSTATE( ViSession vi , int *val0 )
{
//!查询DI端口当前的状态
//!SYSTEM DIOSTATE

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:DIOSTATE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!OFF|ON
int MRQ_setSYSTEM_REVMOTION( ViSession vi, char * val0 )
{
//!设置反向运行状态
//!SYSTEM REVMOTION
    char args[SEND_BUF];
    sprintf( args, "SYSTEM:REVMOTION %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getSYSTEM_REVMOTION( ViSession vi , char * val0 )
{
//!查询反向运行状态
//!SYSTEM REVMOTION

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:REVMOTION?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8
//!
int MRQ_setSYSTEM_FANPARA( ViSession vi, int val0 )
{
//!设置风扇PWM的占空比和频率，占空比数值是百分比乘100，频率单位Hz
//!SYSTEM FANPARA
    char args[SEND_BUF];
    sprintf( args, "SYSTEM:FANPARA %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!byte
//!val0
int MRQ_getSYSTEM_FANPARA( ViSession vi , int *val0 )
{
//!查询风扇PWM的占空比和频率
//!SYSTEM FANPARA

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:FANPARA?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U8
//!BASE|BIGARM|SMALLARM|WRIST
int MRQ_setSYSTEM_ARMLEDPARA( ViSession vi, char * val0,int val1 )
{
//!设置机械臂LEDPWM的占空比和频率，占空比数值是百分比乘100，频率单位Hz
//!SYSTEM ARMLEDPARA
    char args[SEND_BUF];
    sprintf( args, "SYSTEM:ARMLEDPARA %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!BASE|BIGARM|SMALLARM|WRIST
//!byte
//!val1
int MRQ_getSYSTEM_ARMLEDPARA( ViSession vi, char * val0 , int *val1 )
{
//!查询机械臂LEDPWM的占空比和频率
//!SYSTEM ARMLEDPARA

    char args[SEND_BUF];
    sprintf( args, "SYSTEM:ARMLEDPARA? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!4800|7200|9600|14400|19200|38400|57600|115200|128000
int MRQ_setRS232_BAUD( ViSession vi, char * val0 )
{
//!设置波特率，单位bps
//!RS232 BAUD
    char args[SEND_BUF];
    sprintf( args, "RS232:BAUD %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_RS232_BAUD
//!val0
int MRQ_getRS232_BAUD( ViSession vi , char * val0 )
{
//!查询RS232波特率
//!RS232 BAUD

    char args[SEND_BUF];
    sprintf( args, "RS232:BAUD?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!8|9
int MRQ_setRS232_WORDLEN( ViSession vi, char * val0 )
{
//!设置数据长度
//!RS232 WORDLEN
    char args[SEND_BUF];
    sprintf( args, "RS232:WORDLEN %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_RS232_WORDLEN
//!val0
int MRQ_getRS232_WORDLEN( ViSession vi , char * val0 )
{
//!查询RS232数据长度
//!RS232 WORDLEN

    char args[SEND_BUF];
    sprintf( args, "RS232:WORDLEN?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!NONE|RTS|CTS|RTS&CTS
int MRQ_setRS232_FLOWCTL( ViSession vi, char * val0 )
{
//!打开或关闭流控制
//!RS232 FLOWCTL
    char args[SEND_BUF];
    sprintf( args, "RS232:FLOWCTL %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_RS232_FLOWCTL
//!val0
int MRQ_getRS232_FLOWCTL( ViSession vi , char * val0 )
{
//!查询RS232流控制状态
//!RS232 FLOWCTL

    char args[SEND_BUF];
    sprintf( args, "RS232:FLOWCTL?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!NONE|EVEN|ODD
int MRQ_setRS232_PARITY( ViSession vi, char * val0 )
{
//!校验方式
//!RS232 PARITY
    char args[SEND_BUF];
    sprintf( args, "RS232:PARITY %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_RS232_PARITY
//!val0
int MRQ_getRS232_PARITY( ViSession vi , char * val0 )
{
//!查询RS232校验方式
//!RS232 PARITY

    char args[SEND_BUF];
    sprintf( args, "RS232:PARITY?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!1|0.5|2|1.5
int MRQ_setRS232_STOPBIT( ViSession vi, char * val0 )
{
//!设置停止位
//!RS232 STOPBIT
    char args[SEND_BUF];
    sprintf( args, "RS232:STOPBIT %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_RS232_STOPBIT
//!val0
int MRQ_getRS232_STOPBIT( ViSession vi , char * val0 )
{
//!查询RS232停止位
//!RS232 STOPBIT

    char args[SEND_BUF];
    sprintf( args, "RS232:STOPBIT?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!
//!
int MRQ_setRS232_APPLYPARA( ViSession vi )
{
//!按照之前设置的参数配置接口
//!RS232 APPLYPARA
    char args[SEND_BUF];
    sprintf( args, "RS232:APPLYPARA"   );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!A|B
int MRQ_setCAN_TYPE( ViSession vi, char * val0 )
{
//!设置使用的CAN版本
//!CAN TYPE
    char args[SEND_BUF];
    sprintf( args, "CAN:TYPE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_CAN_TYPE
//!val0
int MRQ_getCAN_TYPE( ViSession vi , char * val0 )
{
//!查询CAN接口当前的版本
//!CAN TYPE

    char args[SEND_BUF];
    sprintf( args, "CAN:TYPE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!1000|500|250|125|100|50|20|10
int MRQ_setCAN_BAUD( ViSession vi, char * val0 )
{
//!设置波特率，单位kBps
//!CAN BAUD
    char args[SEND_BUF];
    sprintf( args, "CAN:BAUD %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_CAN_BAUD
//!val0
int MRQ_getCAN_BAUD( ViSession vi , char * val0 )
{
//!查询节点的CAN波特率
//!CAN BAUD

    char args[SEND_BUF];
    sprintf( args, "CAN:BAUD?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8
//!
int MRQ_setCAN_GROUP( ViSession vi, int val0 )
{
//!设置分组信息
//!CAN GROUP
    char args[SEND_BUF];
    sprintf( args, "CAN:GROUP %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!byte
//!val0
int MRQ_getCAN_GROUP( ViSession vi , int *val0 )
{
//!查询节点的分组信息
//!CAN GROUP

    char args[SEND_BUF];
    sprintf( args, "CAN:GROUP?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setCAN_SENDID( ViSession vi, int val0 )
{
//!设置节点的CANID
//!CAN SENDID
    char args[SEND_BUF];
    sprintf( args, "CAN:SENDID %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getCAN_SENDID( ViSession vi , int *val0 )
{
//!查询节点的CANID
//!CAN SENDID

    char args[SEND_BUF];
    sprintf( args, "CAN:SENDID?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setCAN_RECEIVEID( ViSession vi, int val0 )
{
//!设置接收D，单对单的接收ID
//!CAN RECEIVEID
    char args[SEND_BUF];
    sprintf( args, "CAN:RECEIVEID %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getCAN_RECEIVEID( ViSession vi , int *val0 )
{
//!查询节点的接收ID
//!CAN RECEIVEID

    char args[SEND_BUF];
    sprintf( args, "CAN:RECEIVEID?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setCAN_GROUPID1( ViSession vi, int val0 )
{
//!设置组ID1，分组后的接收ID1
//!CAN GROUPID1
    char args[SEND_BUF];
    sprintf( args, "CAN:GROUPID1 %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getCAN_GROUPID1( ViSession vi , int *val0 )
{
//!查询节点的组ID1
//!CAN GROUPID1

    char args[SEND_BUF];
    sprintf( args, "CAN:GROUPID1?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setCAN_GROUPID2( ViSession vi, int val0 )
{
//!设置组ID2，分组后的接收ID2
//!CAN GROUPID2
    char args[SEND_BUF];
    sprintf( args, "CAN:GROUPID2 %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getCAN_GROUPID2( ViSession vi , int *val0 )
{
//!查询节点的组ID2
//!CAN GROUPID2

    char args[SEND_BUF];
    sprintf( args, "CAN:GROUPID2?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setCAN_BROADCASTID( ViSession vi, int val0 )
{
//!设置广播接收ID
//!CAN BROADCASTID
    char args[SEND_BUF];
    sprintf( args, "CAN:BROADCASTID %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getCAN_BROADCASTID( ViSession vi , int *val0 )
{
//!查询节点的广播ID
//!CAN BROADCASTID

    char args[SEND_BUF];
    sprintf( args, "CAN:BROADCASTID?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!
//!
int MRQ_setCAN_APPLYPARA( ViSession vi )
{
//!按照之前设置的参数配置接口
//!CAN APPLYPARA
    char args[SEND_BUF];
    sprintf( args, "CAN:APPLYPARA"   );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!OFF|ON
int MRQ_setCAN_NETMANAGELED( ViSession vi, char * val0 )
{
//!设置网络管理LED状态
//!CAN NETMANAGELED
    char args[SEND_BUF];
    sprintf( args, "CAN:NETMANAGELED %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!IDLE|HASH|SIGNATURE
int MRQ_setCAN_NETMANAGESTATE( ViSession vi, char * val0 )
{
//!设置CAN网络配置状态，IDLE状态下不进行配置，HASH状态下对比哈希值进入状态，SIGNATURE状态下对比电子签名进入配置状态
//!CAN NETMANAGESTATE
    char args[SEND_BUF];
    sprintf( args, "CAN:NETMANAGESTATE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_CAN_NETMANAGESTATE
//!val0
int MRQ_getCAN_NETMANAGESTATE( ViSession vi , char * val0 )
{
//!查询CAN网络配置状态
//!CAN NETMANAGESTATE

    char args[SEND_BUF];
    sprintf( args, "CAN:NETMANAGESTATE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U32
//!NONE|SENDID|RECEIVEID
int MRQ_setCAN_NETMANAGEID( ViSession vi, char * val0,int val1 )
{
//!设置节点的ID
//!CAN NETMANAGEID
    char args[SEND_BUF];
    sprintf( args, "CAN:NETMANAGEID %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!NONE|SENDID|RECEIVEID
//!uint32
//!val1
int MRQ_getCAN_NETMANAGEID( ViSession vi, char * val0 , int *val1 )
{
//!查询在配置状态下,为节点设置的ID
//!CAN NETMANAGEID

    char args[SEND_BUF];
    sprintf( args, "CAN:NETMANAGEID? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setCAN_NETMANAGEHASH( ViSession vi, int val0 )
{
//!发送哈希值，使模块进入配置状态(网络状态为HASH时）
//!CAN NETMANAGEHASH
    char args[SEND_BUF];
    sprintf( args, "CAN:NETMANAGEHASH %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getCAN_NETMANAGEHASH( ViSession vi , int *val0 )
{
//!查询网络管理初级识别符(哈希值)
//!CAN NETMANAGEHASH

    char args[SEND_BUF];
    sprintf( args, "CAN:NETMANAGEHASH?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setCLOCK_FREQUENCY( ViSession vi, int val0 )
{
//!设置时钟频率
//!CLOCK FREQUENCY
    char args[SEND_BUF];
    sprintf( args, "CLOCK:FREQUENCY %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getCLOCK_FREQUENCY( ViSession vi , int *val0 )
{
//!查询时钟频率
//!CLOCK FREQUENCY

    char args[SEND_BUF];
    sprintf( args, "CLOCK:FREQUENCY?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!OFF|ON
int MRQ_setCLOCK_SYNCREGISTER( ViSession vi, char * val0 )
{
//!设置时钟同步寄存器状态，先配置寄存器状态才能进行时钟计数
//!CLOCK SYNCREGISTER
    char args[SEND_BUF];
    sprintf( args, "CLOCK:SYNCREGISTER %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getCLOCK_SYNCREGISTER( ViSession vi , char * val0 )
{
//!查询时钟同步寄存器状态
//!CLOCK SYNCREGISTER

    char args[SEND_BUF];
    sprintf( args, "CLOCK:SYNCREGISTER?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!RADIOID|RECEIVEID|GROUPID1|GROUPID2
int MRQ_setCLOCK_STARTTYPE( ViSession vi, char * val0 )
{
//!设置启动类型
//!CLOCK STARTTYPE
    char args[SEND_BUF];
    sprintf( args, "CLOCK:STARTTYPE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_CLOCK_STARTTYPE
//!val0
int MRQ_getCLOCK_STARTTYPE( ViSession vi , char * val0 )
{
//!查询启动类型
//!CLOCK STARTTYPE

    char args[SEND_BUF];
    sprintf( args, "CLOCK:STARTTYPE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!END|START
int MRQ_setCLOCK_SYNCSTATE( ViSession vi, char * val0 )
{
//!设置时钟同步状态，开启或结束时需要先使用配置同步寄存器
//!CLOCK SYNCSTATE
    char args[SEND_BUF];
    sprintf( args, "CLOCK:SYNCSTATE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_CLOCK_SYNCSTATE
//!val0
int MRQ_getCLOCK_SYNCSTATE( ViSession vi , char * val0 )
{
//!查询时钟同步状态
//!CLOCK SYNCSTATE

    char args[SEND_BUF];
    sprintf( args, "CLOCK:SYNCSTATE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!uint32
//!val0
int MRQ_getCLOCK_COUNT( ViSession vi , int *val0 )
{
//!查询时钟计数值
//!CLOCK COUNT

    char args[SEND_BUF];
    sprintf( args, "CLOCK:COUNT?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],RESET|STOP|RUN|PREPARE|EMERGSTOP,MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTION_SWITCH( ViSession vi, int val0,char * val1,char * val2 )
{
//!运动切换，使节点进入相应状态：IDLE|CALCEND|STANDBY|RUNNING
//!MOTION SWITCH
    char args[SEND_BUF];
    sprintf( args, "MOTION:SWITCH %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTION_STATE_2
//!val2
int MRQ_getMOTION_STATE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!下位机运动系统状态。上报方式为主动时，由节点发送给上位机或者控制器，上位机或控制器收到需要回复同样的命令，如果没收到回复节点会一直按时间间隔发送，直到回复；上报方式为查询时，不需要带参数，查询后下位机返回状态
//!MOTION STATE

    char args[SEND_BUF];
    sprintf( args, "MOTION:STATE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],ACTIVE|QUERY
int MRQ_setMOTION_STATEREPORT( ViSession vi, int val0,char * val1 )
{
//!设置MOTION:STATE的上报方式，主动上报还是查询上报
//!MOTION STATEREPORT
    char args[SEND_BUF];
    sprintf( args, "MOTION:STATEREPORT %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_MOTION_STATEREPORT
//!val1
int MRQ_getMOTION_STATEREPORT( ViSession vi, int val0 , char * val1 )
{
//!查询MOTION:STATE的上报方式
//!MOTION STATEREPORT

    char args[SEND_BUF];
    sprintf( args, "MOTION:STATEREPORT? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],SOFTWARE|DIGITALIO|CAN|ALL
int MRQ_setMOTION_STARTSOURCE( ViSession vi, int val0,char * val1 )
{
//!设置启动源
//!MOTION STARTSOURCE
    char args[SEND_BUF];
    sprintf( args, "MOTION:STARTSOURCE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_MOTION_STARTSOURCE
//!val1
int MRQ_getMOTION_STARTSOURCE( ViSession vi, int val0 , char * val1 )
{
//!查询启动源
//!MOTION STARTSOURCE

    char args[SEND_BUF];
    sprintf( args, "MOTION:STARTSOURCE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],RADIOID|RECEIVEID|GROUPID1|GROUPID2
int MRQ_setMOTION_STARTTYPE( ViSession vi, int val0,char * val1 )
{
//!设置启动类型
//!MOTION STARTTYPE
    char args[SEND_BUF];
    sprintf( args, "MOTION:STARTTYPE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_CLOCK_STARTTYPE
//!val1
int MRQ_getMOTION_STARTTYPE( ViSession vi, int val0 , char * val1 )
{
//!查询启动类型
//!MOTION STARTTYPE

    char args[SEND_BUF];
    sprintf( args, "MOTION:STARTTYPE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MAXSPEED( ViSession vi, int val0,float val1 )
{
//!设置运行的速度上限
//!MOTION MAXSPEED
    char args[SEND_BUF];
    sprintf( args, "MOTION:MAXSPEED %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MAXSPEED( ViSession vi, int val0 , float *val1 )
{
//!查询运行的速度上限
//!MOTION MAXSPEED

    char args[SEND_BUF];
    sprintf( args, "MOTION:MAXSPEED? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MINSPEED( ViSession vi, int val0,float val1 )
{
//!设置运行的速度下限
//!MOTION MINSPEED
    char args[SEND_BUF];
    sprintf( args, "MOTION:MINSPEED %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MINSPEED( ViSession vi, int val0 , float *val1 )
{
//!查询运行的速度下限
//!MOTION MINSPEED

    char args[SEND_BUF];
    sprintf( args, "MOTION:MINSPEED? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MAXPOSITION( ViSession vi, int val0,float val1 )
{
//!设置运行的位置上限
//!MOTION MAXPOSITION
    char args[SEND_BUF];
    sprintf( args, "MOTION:MAXPOSITION %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MAXPOSITION( ViSession vi, int val0 , float *val1 )
{
//!查询运行的位置上限
//!MOTION MAXPOSITION

    char args[SEND_BUF];
    sprintf( args, "MOTION:MAXPOSITION? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MINPOSITION( ViSession vi, int val0,float val1 )
{
//!设置运行的位置下限
//!MOTION MINPOSITION
    char args[SEND_BUF];
    sprintf( args, "MOTION:MINPOSITION %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MINPOSITION( ViSession vi, int val0 , float *val1 )
{
//!查询运行的位置下限
//!MOTION MINPOSITION

    char args[SEND_BUF];
    sprintf( args, "MOTION:MINPOSITION? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MAXTORQUE( ViSession vi, int val0,float val1 )
{
//!设置运行的力矩上限
//!MOTION MAXTORQUE
    char args[SEND_BUF];
    sprintf( args, "MOTION:MAXTORQUE %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MAXTORQUE( ViSession vi, int val0 , float *val1 )
{
//!查询运行的力矩上限
//!MOTION MAXTORQUE

    char args[SEND_BUF];
    sprintf( args, "MOTION:MAXTORQUE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MINTORQUE( ViSession vi, int val0,float val1 )
{
//!设置运行的力矩下限
//!MOTION MINTORQUE
    char args[SEND_BUF];
    sprintf( args, "MOTION:MINTORQUE %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MINTORQUE( ViSession vi, int val0 , float *val1 )
{
//!查询运行的力矩下限
//!MOTION MINTORQUE

    char args[SEND_BUF];
    sprintf( args, "MOTION:MINTORQUE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MAXACCELERATION( ViSession vi, int val0,float val1 )
{
//!设置运行的加速度上限
//!MOTION MAXACCELERATION
    char args[SEND_BUF];
    sprintf( args, "MOTION:MAXACCELERATION %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MAXACCELERATION( ViSession vi, int val0 , float *val1 )
{
//!查询运行的加速度上限
//!MOTION MAXACCELERATION

    char args[SEND_BUF];
    sprintf( args, "MOTION:MAXACCELERATION? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTION_MINACCELERATION( ViSession vi, int val0,float val1 )
{
//!设置运行的加速度下限
//!MOTION MINACCELERATION
    char args[SEND_BUF];
    sprintf( args, "MOTION:MINACCELERATION %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTION_MINACCELERATION( ViSession vi, int val0 , float *val1 )
{
//!查询运行的加速度下限
//!MOTION MINACCELERATION

    char args[SEND_BUF];
    sprintf( args, "MOTION:MINACCELERATION? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U32
//![CHANNUM]
int MRQ_setMOTION_ORIGIN( ViSession vi, int val0,int val1 )
{
//!设置当前位置为原点
//!MOTION ORIGIN
    char args[SEND_BUF];
    sprintf( args, "MOTION:ORIGIN %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getMOTION_ORIGIN( ViSession vi, int val0 , int *val1 )
{
//!查询原点位置
//!MOTION ORIGIN

    char args[SEND_BUF];
    sprintf( args, "MOTION:ORIGIN? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8
//![CHANNUM]
int MRQ_setMOTION_GOORIGIN( ViSession vi, int val0 )
{
//!回到原点
//!MOTION GOORIGIN
    char args[SEND_BUF];
    sprintf( args, "MOTION:GOORIGIN %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setMOTION_OFFSETSTATE( ViSession vi, int val0,char * val1 )
{
//!设置电机未运动时发生位移是否上报
//!MOTION OFFSETSTATE
    char args[SEND_BUF];
    sprintf( args, "MOTION:OFFSETSTATE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getMOTION_OFFSETSTATE( ViSession vi, int val0 , char * val1 )
{
//!查询电机未运动时发生位移是否上报
//!MOTION OFFSETSTATE

    char args[SEND_BUF];
    sprintf( args, "MOTION:OFFSETSTATE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,S32
//![CHANNUM]
int MRQ_setMOTION_OFFSET( ViSession vi, int val0,int val1 )
{
//!电机未运动时发生的位移
//!MOTION OFFSET
    char args[SEND_BUF];
    sprintf( args, "MOTION:OFFSET %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!int16
//!val1
int MRQ_getMOTION_COUNTCIRCLE( ViSession vi, int val0 , int *val1 )
{
//!查询电机的圈数统计值，小于0为反转，大于0为正转
//!MOTION COUNTCIRCLE

    char args[SEND_BUF];
    sprintf( args, "MOTION:COUNTCIRCLE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8
//![CHANNUM]
//!int32
//!val1
int MRQ_getMOTION_ABCOUNT( ViSession vi, int val0 , int *val1 )
{
//!查询电机的AB相计数，小于0为反转，大于0为正转
//!MOTION ABCOUNT

    char args[SEND_BUF];
    sprintf( args, "MOTION:ABCOUNT? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8
//![CHANNUM]
int MRQ_setMOTION_ABCOUNTCLEAR( ViSession vi, int val0 )
{
//!清零编码器
//!MOTION ABCOUNTCLEAR
    char args[SEND_BUF];
    sprintf( args, "MOTION:ABCOUNTCLEAR %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getMOTION_RUNTIMEL( ViSession vi, int val0 , int *val1 )
{
//!查询电机的最后一次运行的运行时间，低32位，单位ms
//!MOTION RUNTIMEL

    char args[SEND_BUF];
    sprintf( args, "MOTION:RUNTIMEL? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getMOTION_RUNTIMEH( ViSession vi, int val0 , int *val1 )
{
//!查询电机的最后一次运行的运行时间，高32位，单位ms
//!MOTION RUNTIMEH

    char args[SEND_BUF];
    sprintf( args, "MOTION:RUNTIMEH? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U8
//![CHANNUM],GROUP1|GROUP2
int MRQ_setIDENTITY_GROUP( ViSession vi, int val0,char * val1,int val2 )
{
//!设置通道的分组号
//!IDENTITY GROUP
    char args[SEND_BUF];
    sprintf( args, "IDENTITY:GROUP %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],GROUP1|GROUP2
//!byte
//!val2
int MRQ_getIDENTITY_GROUP( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询通道的分组号
//!IDENTITY GROUP

    char args[SEND_BUF];
    sprintf( args, "IDENTITY:GROUP? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!OFF|ON
int MRQ_setIDENTITY_DISTDEVICE( ViSession vi, char * val0 )
{
//!设置设备的识别状态
//!IDENTITY DISTDEVICE
    char args[SEND_BUF];
    sprintf( args, "IDENTITY:DISTDEVICE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getIDENTITY_DISTDEVICE( ViSession vi , char * val0 )
{
//!查询设备的识别状态
//!IDENTITY DISTDEVICE

    char args[SEND_BUF];
    sprintf( args, "IDENTITY:DISTDEVICE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],SINANJU|MEGATRON|GOUF,S1|S2|S3|S4
int MRQ_setIDENTITY_LABEL( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置通道的标签(后续会做一个标签列表)
//!IDENTITY LABEL
    char args[SEND_BUF];
    sprintf( args, "IDENTITY:LABEL %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_IDENTITY_LABEL,MRQ_IDENTITY_LABEL_1
//!val1,val2
int MRQ_getIDENTITY_LABEL( ViSession vi, int val0 , char * val1,char * val2 )
{
//!查询通道的标签
//!IDENTITY LABEL

    char args[SEND_BUF];
    sprintf( args, "IDENTITY:LABEL? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    strcpy(val2, paraAt( &para, 1 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],1.8|0.9|15|7.5
int MRQ_setMOTOR_STEPANGLE( ViSession vi, int val0,char * val1 )
{
//!设置电机步距角
//!MOTOR STEPANGLE
    char args[SEND_BUF];
    sprintf( args, "MOTOR:STEPANGLE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_MOTOR_STEPANGLE
//!val1
int MRQ_getMOTOR_STEPANGLE( ViSession vi, int val0 , char * val1 )
{
//!查询电机步距角
//!MOTOR STEPANGLE

    char args[SEND_BUF];
    sprintf( args, "MOTOR:STEPANGLE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],ROTARY|LINEAR
int MRQ_setMOTOR_TYPE( ViSession vi, int val0,char * val1 )
{
//!设置电机运动类型
//!MOTOR TYPE
    char args[SEND_BUF];
    sprintf( args, "MOTOR:TYPE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_MOTOR_TYPE
//!val1
int MRQ_getMOTOR_TYPE( ViSession vi, int val0 , char * val1 )
{
//!查询电机运动类型
//!MOTOR TYPE

    char args[SEND_BUF];
    sprintf( args, "MOTOR:TYPE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],ANGLE|RADIAN|MILLIMETER
int MRQ_setMOTOR_POSITIONUNIT( ViSession vi, int val0,char * val1 )
{
//!设置电机和位置相关参数的单位，包括位置和速度
//!MOTOR POSITIONUNIT
    char args[SEND_BUF];
    sprintf( args, "MOTOR:POSITIONUNIT %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_MOTOR_POSITIONUNIT
//!val1
int MRQ_getMOTOR_POSITIONUNIT( ViSession vi, int val0 , char * val1 )
{
//!查询电机和位置相关参数的单位，包括位置和速度
//!MOTOR POSITIONUNIT

    char args[SEND_BUF];
    sprintf( args, "MOTOR:POSITIONUNIT? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,U16
//![CHANNUM]
int MRQ_setMOTOR_GEARRATIONUM( ViSession vi, int val0,int val1 )
{
//!设置电机减速比分子
//!MOTOR GEARRATIONUM
    char args[SEND_BUF];
    sprintf( args, "MOTOR:GEARRATIONUM %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint16
//!val1
int MRQ_getMOTOR_GEARRATIONUM( ViSession vi, int val0 , int *val1 )
{
//!查询电机减速比分子
//!MOTOR GEARRATIONUM

    char args[SEND_BUF];
    sprintf( args, "MOTOR:GEARRATIONUM? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U16
//![CHANNUM]
int MRQ_setMOTOR_GEARRATIODEN( ViSession vi, int val0,int val1 )
{
//!设置电机减速比分母
//!MOTOR GEARRATIODEN
    char args[SEND_BUF];
    sprintf( args, "MOTOR:GEARRATIODEN %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint16
//!val1
int MRQ_getMOTOR_GEARRATIODEN( ViSession vi, int val0 , int *val1 )
{
//!查询电机减速比分母
//!MOTOR GEARRATIODEN

    char args[SEND_BUF];
    sprintf( args, "MOTOR:GEARRATIODEN? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTOR_LEAD( ViSession vi, int val0,float val1 )
{
//!设置直线运动的导程
//!MOTOR LEAD
    char args[SEND_BUF];
    sprintf( args, "MOTOR:LEAD %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTOR_LEAD( ViSession vi, int val0 , float *val1 )
{
//!查询直线运动的导程
//!MOTOR LEAD

    char args[SEND_BUF];
    sprintf( args, "MOTOR:LEAD? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTOR_PEAKSPEED( ViSession vi, int val0,float val1 )
{
//!设置电机峰值速度
//!MOTOR PEAKSPEED
    char args[SEND_BUF];
    sprintf( args, "MOTOR:PEAKSPEED %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTOR_PEAKSPEED( ViSession vi, int val0 , float *val1 )
{
//!查询电机峰值速度
//!MOTOR PEAKSPEED

    char args[SEND_BUF];
    sprintf( args, "MOTOR:PEAKSPEED? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTOR_PEAKACCELERATION( ViSession vi, int val0,float val1 )
{
//!设置电机峰值加速度
//!MOTOR PEAKACCELERATION
    char args[SEND_BUF];
    sprintf( args, "MOTOR:PEAKACCELERATION %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTOR_PEAKACCELERATION( ViSession vi, int val0 , float *val1 )
{
//!查询电机峰值加速度
//!MOTOR PEAKACCELERATION

    char args[SEND_BUF];
    sprintf( args, "MOTOR:PEAKACCELERATION? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],8|11|14|17|23|24
int MRQ_setMOTOR_SIZE( ViSession vi, int val0,char * val1 )
{
//!设置电机尺寸
//!MOTOR SIZE
    char args[SEND_BUF];
    sprintf( args, "MOTOR:SIZE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_MOTOR_SIZE
//!val1
int MRQ_getMOTOR_SIZE( ViSession vi, int val0 , char * val1 )
{
//!查询电机尺寸
//!MOTOR SIZE

    char args[SEND_BUF];
    sprintf( args, "MOTOR:SIZE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,U8
//![CHANNUM]
int MRQ_setMOTOR_VOLTAGE( ViSession vi, int val0,int val1 )
{
//!设置电机工作电压
//!MOTOR VOLTAGE
    char args[SEND_BUF];
    sprintf( args, "MOTOR:VOLTAGE %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getMOTOR_VOLTAGE( ViSession vi, int val0 , int *val1 )
{
//!查询电机工作电压
//!MOTOR VOLTAGE

    char args[SEND_BUF];
    sprintf( args, "MOTOR:VOLTAGE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U8
//![CHANNUM]
int MRQ_setMOTOR_CURRENT( ViSession vi, int val0,int val1 )
{
//!设置电机工作最大电流，单位0.1A
//!MOTOR CURRENT
    char args[SEND_BUF];
    sprintf( args, "MOTOR:CURRENT %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getMOTOR_CURRENT( ViSession vi, int val0 , int *val1 )
{
//!查询电机工作最大电流
//!MOTOR CURRENT

    char args[SEND_BUF];
    sprintf( args, "MOTOR:CURRENT? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,F32
//![CHANNUM]
int MRQ_setMOTOR_BACKLASH( ViSession vi, int val0,float val1 )
{
//!设置电机的反向间隙
//!MOTOR BACKLASH
    char args[SEND_BUF];
    sprintf( args, "MOTOR:BACKLASH %d,%g" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!f32
//!val1
int MRQ_getMOTOR_BACKLASH( ViSession vi, int val0 , float *val1 )
{
//!查询电机的反向间隙
//!MOTOR BACKLASH

    char args[SEND_BUF];
    sprintf( args, "MOTOR:BACKLASH? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U16
//![CHANNUM]
int MRQ_setMOTOR_BACKLASHTEST( ViSession vi, int val0,int val1 )
{
//!开始电机反向间隙测试
//!MOTOR BACKLASHTEST
    char args[SEND_BUF];
    sprintf( args, "MOTOR:BACKLASHTEST %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint16
//!val1
int MRQ_getMOTOR_BACKLASHTEST( ViSession vi, int val0 , int *val1 )
{
//!查询电机反向间隙测试结果
//!MOTOR BACKLASHTEST

    char args[SEND_BUF];
    sprintf( args, "MOTOR:BACKLASHTEST? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U32
//![CHANNUM]
int MRQ_setENCODER_LINENUM( ViSession vi, int val0,int val1 )
{
//!设置编码器每圈线数(只有3、4轴两个型号支持编码器，并且和TRIGR互斥，只能同时使能一个；6、7、8轴目前不支持编码器)
//!ENCODER LINENUM
    char args[SEND_BUF];
    sprintf( args, "ENCODER:LINENUM %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getENCODER_LINENUM( ViSession vi, int val0 , int *val1 )
{
//!查询编码器每圈线数
//!ENCODER LINENUM

    char args[SEND_BUF];
    sprintf( args, "ENCODER:LINENUM? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],1|3
int MRQ_setENCODER_CHANNELNUM( ViSession vi, int val0,char * val1 )
{
//!设置编码器通道数
//!ENCODER CHANNELNUM
    char args[SEND_BUF];
    sprintf( args, "ENCODER:CHANNELNUM %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_ENCODER_CHANNELNUM
//!val1
int MRQ_getENCODER_CHANNELNUM( ViSession vi, int val0 , char * val1 )
{
//!查询编码器通道数
//!ENCODER CHANNELNUM

    char args[SEND_BUF];
    sprintf( args, "ENCODER:CHANNELNUM? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],INCREMENTAL|ABSOLUTE
int MRQ_setENCODER_TYPE( ViSession vi, int val0,char * val1 )
{
//!设置编码器类型
//!ENCODER TYPE
    char args[SEND_BUF];
    sprintf( args, "ENCODER:TYPE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_ENCODER_TYPE
//!val1
int MRQ_getENCODER_TYPE( ViSession vi, int val0 , char * val1 )
{
//!查询编码器类型
//!ENCODER TYPE

    char args[SEND_BUF];
    sprintf( args, "ENCODER:TYPE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],SINGLE|DOUBLE|QUADRUPLE
int MRQ_setENCODER_MULTIPLE( ViSession vi, int val0,char * val1 )
{
//!设置编码器信号的倍乘
//!ENCODER MULTIPLE
    char args[SEND_BUF];
    sprintf( args, "ENCODER:MULTIPLE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_ENCODER_MULTIPLE
//!val1
int MRQ_getENCODER_MULTIPLE( ViSession vi, int val0 , char * val1 )
{
//!查询编码器信号的倍乘
//!ENCODER MULTIPLE

    char args[SEND_BUF];
    sprintf( args, "ENCODER:MULTIPLE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],NONE|OFF|ON
int MRQ_setENCODER_STATE( ViSession vi, int val0,char * val1 )
{
//!设置编码器状态，编码器和DI互斥，DI使能后如果编码器使能则会自动修改为失能，并上传状态；同样的编码器使能时会将DI失能，并上传状态。
//!ENCODER STATE
    char args[SEND_BUF];
    sprintf( args, "ENCODER:STATE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_ENCODER_STATE
//!val1
int MRQ_getENCODER_STATE( ViSession vi, int val0 , char * val1 )
{
//!查询编码器状态
//!ENCODER STATE

    char args[SEND_BUF];
    sprintf( args, "ENCODER:STATE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,U8
//![CHANNUM]
int MRQ_setENCODER_FEEDBACKRATIO( ViSession vi, int val0,int val1 )
{
//!设置LVT模式下编码器反馈比
//!ENCODER FEEDBACKRATIO
    char args[SEND_BUF];
    sprintf( args, "ENCODER:FEEDBACKRATIO %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getENCODER_FEEDBACKRATIO( ViSession vi, int val0 , int *val1 )
{
//!查询LVT模式下编码器反馈比
//!ENCODER FEEDBACKRATIO

    char args[SEND_BUF];
    sprintf( args, "ENCODER:FEEDBACKRATIO? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,END|CLEAR
int MRQ_setMOTIONPLAN_PVTCONFIG( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置当前PVT的配置状态
//!MOTIONPLAN PVTCONFIG
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:PVTCONFIG %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_PVTCONFIG_1
//!val2
int MRQ_getMOTIONPLAN_PVTCONFIG( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询当前PVT的配置状态
//!MOTIONPLAN PVTCONFIG

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:PVTCONFIG? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON|CALC
int MRQ_setMOTIONPLAN_PRESETSTATE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置PRESET执行模式
//!MOTIONPLAN PRESETSTATE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:PRESETSTATE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_PRESETSTATE_1
//!val2
int MRQ_getMOTIONPLAN_PRESETSTATE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询PRESET状态
//!MOTIONPLAN PRESETSTATE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:PRESETSTATE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,CYCLE|FIFO
int MRQ_setMOTIONPLAN_EXECUTEMODE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置PVT执行模式
//!MOTIONPLAN EXECUTEMODE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:EXECUTEMODE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_EXECUTEMODE_1
//!val2
int MRQ_getMOTIONPLAN_EXECUTEMODE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询PVT执行模式
//!MOTIONPLAN EXECUTEMODE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:EXECUTEMODE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,CUBICPOLY|TRAPEZOID|SCURVE
int MRQ_setMOTIONPLAN_PLANMODE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置轨迹规划模式
//!MOTIONPLAN PLANMODE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:PLANMODE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_PLANMODE_1
//!val2
int MRQ_getMOTIONPLAN_PLANMODE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询轨迹规划模式
//!MOTIONPLAN PLANMODE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:PLANMODE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,PVT|LVT_CORRECT|LVT_NOCORRECT
int MRQ_setMOTIONPLAN_MOTIONMODE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置运动模式，PVT方式、LVT不带时间修正方式、LVT带时间修正方式
//!MOTIONPLAN MOTIONMODE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:MOTIONMODE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_MOTIONMODE_1
//!val2
int MRQ_getMOTIONPLAN_MOTIONMODE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询运动模式
//!MOTIONPLAN MOTIONMODE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:MOTIONMODE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,4|8|16|32
int MRQ_setMOTIONPLAN_MODIFYDUTY( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置LVT模式下进行时间调整的占比
//!MOTIONPLAN MODIFYDUTY
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:MODIFYDUTY %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_MODIFYDUTY_1
//!val2
int MRQ_getMOTIONPLAN_MODIFYDUTY( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询LVT模式下进行时间调整的占比
//!MOTIONPLAN MODIFYDUTY

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:MODIFYDUTY? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint16
//!val2
int MRQ_getMOTIONPLAN_REMAINPOINT( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询剩余可写入的点数，如果Buffer满了则返回0
//!MOTIONPLAN REMAINPOINT

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:REMAINPOINT? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_CYCLENUM( ViSession vi, int val0,char * val1,int val2 )
{
//!设置循环模式下的循环数
//!MOTIONPLAN CYCLENUM
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:CYCLENUM %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint32
//!val2
int MRQ_getMOTIONPLAN_CYCLENUM( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询循环模式下的循环数
//!MOTIONPLAN CYCLENUM

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:CYCLENUM? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_WARNPOINT( ViSession vi, int val0,char * val1,int val2 )
{
//!设置FIFO模式下警告点，当剩余的可执行PVT点小于等于WARNPOINT时，发送警告通知
//!MOTIONPLAN WARNPOINT
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:WARNPOINT %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!byte
//!val2
int MRQ_getMOTIONPLAN_WARNPOINT( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询FIFO模式下警告点
//!MOTIONPLAN WARNPOINT

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:WARNPOINT? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,STOP|HOLD
int MRQ_setMOTIONPLAN_ENDSTATE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置PVT结束状态
//!MOTIONPLAN ENDSTATE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:ENDSTATE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_ENDSTATE_1
//!val2
int MRQ_getMOTIONPLAN_ENDSTATE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询PVT结束状态
//!MOTIONPLAN ENDSTATE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:ENDSTATE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint32
//!val2
int MRQ_getMOTIONPLAN_BUFFERSIZE( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询主波表Buffer的大小
//!MOTIONPLAN BUFFERSIZE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:BUFFERSIZE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,IMMEDIATE|DISTANCE
int MRQ_setMOTIONPLAN_STOPMODE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置急停模式
//!MOTIONPLAN STOPMODE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:STOPMODE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_STOPMODE_1
//!val2
int MRQ_getMOTIONPLAN_STOPMODE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询急停模式
//!MOTIONPLAN STOPMODE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:STOPMODE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_STOPDISTANCE( ViSession vi, int val0,char * val1,float val2 )
{
//!设置急停的距离值
//!MOTIONPLAN STOPDISTANCE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:STOPDISTANCE %d,%s,%g" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!f32
//!val2
int MRQ_getMOTIONPLAN_STOPDISTANCE( ViSession vi, int val0,char * val1 , float *val2 )
{
//!查询急停的距离值
//!MOTIONPLAN STOPDISTANCE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:STOPDISTANCE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_STOPTIME( ViSession vi, int val0,char * val1,float val2 )
{
//!设置急停的时间值
//!MOTIONPLAN STOPTIME
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:STOPTIME %d,%s,%g" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!f32
//!val2
int MRQ_getMOTIONPLAN_STOPTIME( ViSession vi, int val0,char * val1 , float *val2 )
{
//!查询急停的时间值
//!MOTIONPLAN STOPTIME

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:STOPTIME? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON
int MRQ_setMOTIONPLAN_OOSLINESTATE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置线间失步告警状态
//!MOTIONPLAN OOSLINESTATE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSLINESTATE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getMOTIONPLAN_OOSLINESTATE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询线间失步告警状态
//!MOTIONPLAN OOSLINESTATE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSLINESTATE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U16
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_OOSLINEOUTNUM( ViSession vi, int val0,char * val1,int val2 )
{
//!设置线间失步阈值
//!MOTIONPLAN OOSLINEOUTNUM
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSLINEOUTNUM %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint16
//!val2
int MRQ_getMOTIONPLAN_OOSLINEOUTNUM( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询线间失步阈值
//!MOTIONPLAN OOSLINEOUTNUM

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSLINEOUTNUM? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,NONE|ALARM|STOP|ALARM&STOP
int MRQ_setMOTIONPLAN_OOSLINERESPONSE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置当步数偏差超过LOSTNUM后的响应
//!MOTIONPLAN OOSLINERESPONSE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSLINERESPONSE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val2
int MRQ_getMOTIONPLAN_OOSLINERESPONSE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询失步响应
//!MOTIONPLAN OOSLINERESPONSE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSLINERESPONSE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON
int MRQ_setMOTIONPLAN_OOSTOTALSTATE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置总的失步告警状态
//!MOTIONPLAN OOSTOTALSTATE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSTOTALSTATE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getMOTIONPLAN_OOSTOTALSTATE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询总的失步告警状态
//!MOTIONPLAN OOSTOTALSTATE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSTOTALSTATE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setMOTIONPLAN_OOSTOTALOUTNUM( ViSession vi, int val0,char * val1,int val2 )
{
//!设置总的失步阈值
//!MOTIONPLAN OOSTOTALOUTNUM
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSTOTALOUTNUM %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!uint32
//!val2
int MRQ_getMOTIONPLAN_OOSTOTALOUTNUM( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询总的失步阈值
//!MOTIONPLAN OOSTOTALOUTNUM

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSTOTALOUTNUM? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,NONE|ALARM|STOP|ALARM&STOP
int MRQ_setMOTIONPLAN_OOSTOTALRESPONSE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置当步数偏差超过LOSTNUM后的响应
//!MOTIONPLAN OOSTOTALRESPONSE
    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSTOTALRESPONSE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val2
int MRQ_getMOTIONPLAN_OOSTOTALRESPONSE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询失步响应
//!MOTIONPLAN OOSTOTALRESPONSE

    char args[SEND_BUF];
    sprintf( args, "MOTIONPLAN:OOSTOTALRESPONSE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U8,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ_setPOSITION( ViSession vi, int val0,char * val1,int val2,float val3 )
{
//!设置PVT点的位置值
//!POSITION 
    char args[SEND_BUF];
    sprintf( args, "POSITION %d,%s,%d,%g" , val0,val1,val2,val3);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
//!f32
//!val3
int MRQ_getPOSITION( ViSession vi, int val0,char * val1,int val2 , float *val3 )
{
//!查询PVT点的位置值
//!POSITION 

    char args[SEND_BUF];
    sprintf( args, "POSITION? %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val3 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U8,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ_setVELOCITY( ViSession vi, int val0,char * val1,int val2,float val3 )
{
//!设置PVT点的速度值
//!VELOCITY 
    char args[SEND_BUF];
    sprintf( args, "VELOCITY %d,%s,%d,%g" , val0,val1,val2,val3);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
//!f32
//!val3
int MRQ_getVELOCITY( ViSession vi, int val0,char * val1,int val2 , float *val3 )
{
//!查询PVT点的速度值
//!VELOCITY 

    char args[SEND_BUF];
    sprintf( args, "VELOCITY? %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val3 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U8,F32
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ_setTIME( ViSession vi, int val0,char * val1,int val2,float val3 )
{
//!设置PVT点的时间值
//!TIME 
    char args[SEND_BUF];
    sprintf( args, "TIME %d,%s,%d,%g" , val0,val1,val2,val3);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
//!f32
//!val3
int MRQ_getTIME( ViSession vi, int val0,char * val1,int val2 , float *val3 )
{
//!查询PVT点的时间值
//!TIME 

    char args[SEND_BUF];
    sprintf( args, "TIME? %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val3 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U8,U16,U16
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ_setTIMESCALE( ViSession vi, int val0,char * val1,int val2,int val3,int val4 )
{
//!设置S曲线的加减速占比，两段一起，千分之
//!TIMESCALE 
    char args[SEND_BUF];
    sprintf( args, "TIMESCALE %d,%s,%d,%d,%d" , val0,val1,val2,val3,val4);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM,U8
//![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
//!uint16,uint16
//!val3,val4
int MRQ_getTIMESCALE( ViSession vi, int val0,char * val1,int val2 , int *val3,int *val4 )
{
//!查询S曲线的加减速占比，两段一起，千分之
//!TIMESCALE 

    char args[SEND_BUF];
    sprintf( args, "TIMESCALE? %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val3 = atoi( paraAt( &para, 0) );
    *val4 = atoi( paraAt( &para, 1) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC,OFF|ON
int MRQ_setREPORT_STATE( ViSession vi, int val0,char * val1,char * val2 )
{
//!打开或关闭上报功能，编号从0到4对应失步数、步数、速度、力矩和温度
//!REPORT STATE
    char args[SEND_BUF];
    sprintf( args, "REPORT:STATE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getREPORT_STATE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询上报状态
//!REPORT STATE

    char args[SEND_BUF];
    sprintf( args, "REPORT:STATE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U32
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
int MRQ_setREPORT_PERIOD( ViSession vi, int val0,char * val1,int val2 )
{
//!设置上报周期，单位ms
//!REPORT PERIOD
    char args[SEND_BUF];
    sprintf( args, "REPORT:PERIOD %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
//!uint32
//!val2
int MRQ_getREPORT_PERIOD( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询上报周期
//!REPORT PERIOD

    char args[SEND_BUF];
    sprintf( args, "REPORT:PERIOD? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
//!uint32
//!val2
int MRQ_getREPORT_DATA( ViSession vi, int val0,char * val1 , int *val2 )
{
//!上报数据，上报打开时下位机按照上报周期主动上报；上位机也可以主动查询
//!REPORT DATA

    char args[SEND_BUF];
    sprintf( args, "REPORT:DATA? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],PATTERN|LEVEL
int MRQ_setTRIGGER_MODE( ViSession vi, int val0,char * val1 )
{
//!设置触发类型
//!TRIGGER MODE
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:MODE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_TRIGGER_MODE
//!val1
int MRQ_getTRIGGER_MODE( ViSession vi, int val0 , char * val1 )
{
//!查询触发类型
//!TRIGGER MODE

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:MODE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setTRIGGER_PATTSTATE( ViSession vi, int val0,char * val1 )
{
//!打开或关闭码型触发
//!TRIGGER PATTSTATE
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTSTATE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getTRIGGER_PATTSTATE( ViSession vi, int val0 , char * val1 )
{
//!查询码型触发的状态
//!TRIGGER PATTSTATE

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTSTATE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],X|L|R|F|H,X|L|R|F|H
int MRQ_setTRIGGER_PATTERN( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置触发输入的码型
//!TRIGGER PATTERN
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTERN %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_TRIGGER_PATTERN,MRQ_TRIGGER_PATTERN
//!val1,val2
int MRQ_getTRIGGER_PATTERN( ViSession vi, int val0 , char * val1,char * val2 )
{
//!查询触发输入的码型
//!TRIGGER PATTERN

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTERN? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    strcpy(val2, paraAt( &para, 1 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],NONE|ALARM|STOP|ALARM&STOP
int MRQ_setTRIGGER_PATTRESP( ViSession vi, int val0,char * val1 )
{
//!设置码型触发的响应
//!TRIGGER PATTRESP
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTRESP %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val1
int MRQ_getTRIGGER_PATTRESP( ViSession vi, int val0 , char * val1 )
{
//!查询码型触发的响应
//!TRIGGER PATTRESP

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTRESP? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],CONTINUED|INTERVAL|SINGLE
int MRQ_setTRIGGER_PATTSMODE( ViSession vi, int val0,char * val1 )
{
//!设置码型触发的采样模式
//!TRIGGER PATTSMODE
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTSMODE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_TRIGGER_PATTSMODE
//!val1
int MRQ_getTRIGGER_PATTSMODE( ViSession vi, int val0 , char * val1 )
{
//!查询码型触发的采样模式
//!TRIGGER PATTSMODE

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTSMODE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,U32
//![CHANNUM]
int MRQ_setTRIGGER_PATTSPERIOD( ViSession vi, int val0,int val1 )
{
//!设置码型触发的采样周期
//!TRIGGER PATTSPERIOD
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTSPERIOD %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getTRIGGER_PATTSPERIOD( ViSession vi, int val0 , int *val1 )
{
//!查询码型触发的采样周期
//!TRIGGER PATTSPERIOD

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:PATTSPERIOD? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,OFF|ON
int MRQ_setTRIGGER_LEVELSTATE( ViSession vi, int val0,char * val1,char * val2 )
{
//!打开或关闭电平触发(3,4轴型号中TRIGR和编码器互斥，使用编码器时无法使用TRIGR,反之亦然)
//!TRIGGER LEVELSTATE
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELSTATE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,OFF|ON
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getTRIGGER_LEVELSTATE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询电平触发的状态
//!TRIGGER LEVELSTATE

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELSTATE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,RESERVE|LOW|RISE|FALL|HIGH
int MRQ_setTRIGGER_LEVELTYPE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置触发电平类型
//!TRIGGER LEVELTYPE
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELTYPE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!MRQ_TRIGGER_LEVELTYPE_1
//!val2
int MRQ_getTRIGGER_LEVELTYPE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询触发电平类型
//!TRIGGER LEVELTYPE

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELTYPE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,NONE|ALARM|STOP|ALARM&STOP|RUN
int MRQ_setTRIGGER_LEVELRESP( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置电平触发的响应
//!TRIGGER LEVELRESP
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELRESP %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!MRQ_TRIGGER_LEVELRESP_1
//!val2
int MRQ_getTRIGGER_LEVELRESP( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询电平触发的响应
//!TRIGGER LEVELRESP

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELRESP? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,CONTINUED|INTERVAL|SINGLE
int MRQ_setTRIGGER_LEVELSMODE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置电平触发的采样模式
//!TRIGGER LEVELSMODE
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELSMODE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!MRQ_TRIGGER_PATTSMODE
//!val2
int MRQ_getTRIGGER_LEVELSMODE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询电平触发的采样模式
//!TRIGGER LEVELSMODE

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELSMODE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,F32
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ_setTRIGGER_LEVELSPERIOD( ViSession vi, int val0,char * val1,float val2 )
{
//!设置电平触发的采样周期
//!TRIGGER LEVELSPERIOD
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELSPERIOD %d,%s,%g" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!f32
//!val2
int MRQ_getTRIGGER_LEVELSPERIOD( ViSession vi, int val0,char * val1 , float *val2 )
{
//!查询电平触发的采样周期
//!TRIGGER LEVELSPERIOD

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELSPERIOD? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ_setTRIGGER_LEVELRUNWAVE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置电平触发响应为RUN时执行的波表
//!TRIGGER LEVELRUNWAVE
    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELRUNWAVE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
//!MRQ_MOTION_SWITCH_1
//!val2
int MRQ_getTRIGGER_LEVELRUNWAVE( ViSession vi, int val0,char * val1 , char * val2 )
{
//!查询电平触发响应为RUN时执行的波表
//!TRIGGER LEVELRUNWAVE

    char args[SEND_BUF];
    sprintf( args, "TRIGGER:LEVELRUNWAVE? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8
//![CHANNUM]
//!MRQ_DRIVER_TYPE
//!val1
int MRQ_getDRIVER_TYPE( ViSession vi, int val0 , char * val1 )
{
//!查询驱动板类型，其实可以通过型号进行区分
//!DRIVER TYPE

    char args[SEND_BUF];
    sprintf( args, "DRIVER:TYPE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],R0|R1|R2
//!uint32
//!val2
int MRQ_getDRIVER_STATEREG( ViSession vi, int val0,char * val1 , int *val2 )
{
//!获取TMC芯片状态寄存器信息(数据源为TMC三个状态寄存器)
//!DRIVER STATEREG

    char args[SEND_BUF];
    sprintf( args, "DRIVER:STATEREG? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U8
//![CHANNUM]
int MRQ_setDRIVER_CURRENT( ViSession vi, int val0,int val1 )
{
//!设置驱动工作最大电流，单位0.1A
//!DRIVER CURRENT
    char args[SEND_BUF];
    sprintf( args, "DRIVER:CURRENT %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getDRIVER_CURRENT( ViSession vi, int val0 , int *val1 )
{
//!查询驱动工作最大电流
//!DRIVER CURRENT

    char args[SEND_BUF];
    sprintf( args, "DRIVER:CURRENT? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],256|128|64|32|16|8|4|2|1
int MRQ_setDRIVER_MICROSTEPS( ViSession vi, int val0,char * val1 )
{
//!设置电机微步数
//!DRIVER MICROSTEPS
    char args[SEND_BUF];
    sprintf( args, "DRIVER:MICROSTEPS %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_DRIVER_MICROSTEPS
//!val1
int MRQ_getDRIVER_MICROSTEPS( ViSession vi, int val0 , char * val1 )
{
//!查询电机微步数
//!DRIVER MICROSTEPS

    char args[SEND_BUF];
    sprintf( args, "DRIVER:MICROSTEPS? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setDRIVER_STATE( ViSession vi, int val0,char * val1 )
{
//!设置驱动开关状态
//!DRIVER STATE
    char args[SEND_BUF];
    sprintf( args, "DRIVER:STATE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getDRIVER_STATE( ViSession vi, int val0 , char * val1 )
{
//!查询驱动开关状态
//!DRIVER STATE

    char args[SEND_BUF];
    sprintf( args, "DRIVER:STATE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,S8
//![CHANNUM],UPLIMIT|DOWNLIMIT
int MRQ_setDRIVER_SGLIMIT( ViSession vi, int val0,char * val1,int val2 )
{
//!设置能效曲线SG上下限，超限后报警
//!DRIVER SGLIMIT
    char args[SEND_BUF];
    sprintf( args, "DRIVER:SGLIMIT %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM],UPLIMIT|DOWNLIMIT
//!MRQ_DRIVER_SGLIMIT,int8
//!val1,val2
int MRQ_getDRIVER_SGLIMIT( ViSession vi, int val0 , char * val1,int *val2 )
{
//!查询能效曲线SG上下限
//!DRIVER SGLIMIT

    char args[SEND_BUF];
    sprintf( args, "DRIVER:SGLIMIT? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    *val2 = atoi( paraAt( &para, 1) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,S16
//![CHANNUM],SG0|SGT|SEMAX|SEMIN
int MRQ_setDRIVER_SGPARASET( ViSession vi, int val0,char * val1,int val2 )
{
//!设置SG相关值
//!DRIVER SGPARASET
    char args[SEND_BUF];
    sprintf( args, "DRIVER:SGPARASET %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],SG0|SGT|SEMAX|SEMIN
//!int16
//!val2
int MRQ_getDRIVER_SGPARASET( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询SG相关值
//!DRIVER SGPARASET

    char args[SEND_BUF];
    sprintf( args, "DRIVER:SGPARASET? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U8
//![CHANNUM]
int MRQ_setDRIVER_IDLECURRENT( ViSession vi, int val0,int val1 )
{
//!设置空闲电流，单位0.1A
//!DRIVER IDLECURRENT
    char args[SEND_BUF];
    sprintf( args, "DRIVER:IDLECURRENT %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getDRIVER_IDLECURRENT( ViSession vi, int val0 , int *val1 )
{
//!查询空闲电流
//!DRIVER IDLECURRENT

    char args[SEND_BUF];
    sprintf( args, "DRIVER:IDLECURRENT? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U32
//![CHANNUM]
int MRQ_setDRIVER_SWITCHTIME( ViSession vi, int val0,int val1 )
{
//!从工作电流切换到空闲电流的时间，单位ms
//!DRIVER SWITCHTIME
    char args[SEND_BUF];
    sprintf( args, "DRIVER:SWITCHTIME %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint32
//!val1
int MRQ_getDRIVER_SWITCHTIME( ViSession vi, int val0 , int *val1 )
{
//!查询切换时间
//!DRIVER SWITCHTIME

    char args[SEND_BUF];
    sprintf( args, "DRIVER:SWITCHTIME? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],1/2|1/4
int MRQ_setDRIVER_MINICURRRATIO( ViSession vi, int val0,char * val1 )
{
//!设置运行过程中的最低电流比
//!DRIVER MINICURRRATIO
    char args[SEND_BUF];
    sprintf( args, "DRIVER:MINICURRRATIO %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_DRIVER_MINICURRRATIO
//!val1
int MRQ_getDRIVER_MINICURRRATIO( ViSession vi, int val0 , char * val1 )
{
//!查询运行过程中的最低电流比
//!DRIVER MINICURRRATIO

    char args[SEND_BUF];
    sprintf( args, "DRIVER:MINICURRRATIO? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,U32
//![CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
int MRQ_setDRIVER_REGCONFIG( ViSession vi, int val0,char * val1,int val2 )
{
//!配置驱动寄存器
//!DRIVER REGCONFIG
    char args[SEND_BUF];
    sprintf( args, "DRIVER:REGCONFIG %d,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8,ENUM
//![CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
//!uint32
//!val2
int MRQ_getDRIVER_REGCONFIG( ViSession vi, int val0,char * val1 , int *val2 )
{
//!查询驱动寄存器的配置
//!DRIVER REGCONFIG

    char args[SEND_BUF];
    sprintf( args, "DRIVER:REGCONFIG? %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!DO1|DO2|DO3|DO4,DISABLE|ENABLE
int MRQ_setDIGITALOUTPUT_STATE( ViSession vi, char * val0,char * val1 )
{
//!打开或关闭触发输出
//!DIGITALOUTPUT STATE
    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:STATE %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!DO1|DO2|DO3|DO4
//!MRQ_DIGITALOUTPUT_STATE_1
//!val1
int MRQ_getDIGITALOUTPUT_STATE( ViSession vi, char * val0 , char * val1 )
{
//!查询触发输出的状态
//!DIGITALOUTPUT STATE

    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:STATE? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!DO1|DO2|DO3|DO4,LEVEL|NONE
int MRQ_setDIGITALOUTPUT_SIGNAL( ViSession vi, char * val0,char * val1 )
{
//!设置触发输出的输出信号类型
//!DIGITALOUTPUT SIGNAL
    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:SIGNAL %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!DO1|DO2|DO3|DO4
//!MRQ_DIGITALOUTPUT_SIGNAL_1
//!val1
int MRQ_getDIGITALOUTPUT_SIGNAL( ViSession vi, char * val0 , char * val1 )
{
//!查询触发输出的输出信号类型
//!DIGITALOUTPUT SIGNAL

    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:SIGNAL? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!DO1|DO2|DO3|DO4,POSITIVE|NEGATIVE
int MRQ_setDIGITALOUTPUT_POLARITY( ViSession vi, char * val0,char * val1 )
{
//!设置触发输出的输出信号极性
//!DIGITALOUTPUT POLARITY
    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:POLARITY %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!DO1|DO2|DO3|DO4
//!MRQ_DIGITALOUTPUT_POLARITY_1
//!val1
int MRQ_getDIGITALOUTPUT_POLARITY( ViSession vi, char * val0 , char * val1 )
{
//!查询触发输出的输出信号极性
//!DIGITALOUTPUT POLARITY

    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:POLARITY? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U8
//!DO1|DO2|DO3|DO4,[CHANNUM]
int MRQ_setDIGITALOUTPUT_SOURCE( ViSession vi, char * val0,int val1 )
{
//!设置隔离输出的控制源(不包括所有通道：128和分组号)
//!DIGITALOUTPUT SOURCE
    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:SOURCE %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!DO1|DO2|DO3|DO4
//!byte
//!val1
int MRQ_getDIGITALOUTPUT_SOURCE( ViSession vi, char * val0 , int *val1 )
{
//!查询隔离输出的控制源
//!DIGITALOUTPUT SOURCE

    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:SOURCE? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!DO1|DO2|DO3|DO4,AUTO|NONE
int MRQ_setDIGITALOUTPUT_CONDITION( ViSession vi, char * val0,char * val1 )
{
//!设置触发输出的触发条件
//!DIGITALOUTPUT CONDITION
    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:CONDITION %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!DO1|DO2|DO3|DO4
//!MRQ_DIGITALOUTPUT_CONDITION_1
//!val1
int MRQ_getDIGITALOUTPUT_CONDITION( ViSession vi, char * val0 , char * val1 )
{
//!查询触发输出的触发条件
//!DIGITALOUTPUT CONDITION

    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:CONDITION? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U32
//!DO1|DO2|DO3|DO4
int MRQ_setDIGITALOUTPUT_PERIOD( ViSession vi, char * val0,int val1 )
{
//!设置触发输出的输出方波的周期
//!DIGITALOUTPUT PERIOD
    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:PERIOD %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!DO1|DO2|DO3|DO4
//!uint32
//!val1
int MRQ_getDIGITALOUTPUT_PERIOD( ViSession vi, char * val0 , int *val1 )
{
//!查询触发输出的输出方波的周期
//!DIGITALOUTPUT PERIOD

    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:PERIOD? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U16
//!DO1|DO2|DO3|DO4
int MRQ_setDIGITALOUTPUT_DUTY( ViSession vi, char * val0,int val1 )
{
//!设置触发输出的输出方波的占空比
//!DIGITALOUTPUT DUTY
    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:DUTY %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!DO1|DO2|DO3|DO4
//!uint16
//!val1
int MRQ_getDIGITALOUTPUT_DUTY( ViSession vi, char * val0 , int *val1 )
{
//!查询触发输出的输出方波的占空比
//!DIGITALOUTPUT DUTY

    char args[SEND_BUF];
    sprintf( args, "DIGITALOUTPUT:DUTY? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!YO1|YO2,DISABLE|ENABLE
int MRQ_setISOLATOROUTPUT_STATE( ViSession vi, char * val0,char * val1 )
{
//!打开或关闭隔离输出
//!ISOLATOROUTPUT STATE
    char args[SEND_BUF];
    sprintf( args, "ISOLATOROUTPUT:STATE %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!YO1|YO2
//!MRQ_DIGITALOUTPUT_STATE_1
//!val1
int MRQ_getISOLATOROUTPUT_STATE( ViSession vi, char * val0 , char * val1 )
{
//!查询隔离输出的状态
//!ISOLATOROUTPUT STATE

    char args[SEND_BUF];
    sprintf( args, "ISOLATOROUTPUT:STATE? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U8
//!YO1|YO2,[CHANNUM]
int MRQ_setISOLATOROUTPUT_SOURCE( ViSession vi, char * val0,int val1 )
{
//!设置隔离输出的控制源(不包括所有通道：128和分组号)
//!ISOLATOROUTPUT SOURCE
    char args[SEND_BUF];
    sprintf( args, "ISOLATOROUTPUT:SOURCE %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!YO1|YO2
//!byte
//!val1
int MRQ_getISOLATOROUTPUT_SOURCE( ViSession vi, char * val0 , int *val1 )
{
//!查询隔离输出的控制源
//!ISOLATOROUTPUT SOURCE

    char args[SEND_BUF];
    sprintf( args, "ISOLATOROUTPUT:SOURCE? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!YO1|YO2,AUTO|NONE
int MRQ_setISOLATOROUTPUT_CONDITION( ViSession vi, char * val0,char * val1 )
{
//!设置隔离输出的触发条件
//!ISOLATOROUTPUT CONDITION
    char args[SEND_BUF];
    sprintf( args, "ISOLATOROUTPUT:CONDITION %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!YO1|YO2
//!MRQ_DIGITALOUTPUT_CONDITION_1
//!val1
int MRQ_getISOLATOROUTPUT_CONDITION( ViSession vi, char * val0 , char * val1 )
{
//!查询隔离输出的触发条件
//!ISOLATOROUTPUT CONDITION

    char args[SEND_BUF];
    sprintf( args, "ISOLATOROUTPUT:CONDITION? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!YO1|YO2,TURNON|CUTOFF
int MRQ_setISOLATOROUTPUT_RESPONSE( ViSession vi, char * val0,char * val1 )
{
//!设置隔离输出的响应方式
//!ISOLATOROUTPUT RESPONSE
    char args[SEND_BUF];
    sprintf( args, "ISOLATOROUTPUT:RESPONSE %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!YO1|YO2
//!MRQ_ISOLATOROUTPUT_RESPONSE_1
//!val1
int MRQ_getISOLATOROUTPUT_RESPONSE( ViSession vi, char * val0 , char * val1 )
{
//!查询隔离输出的响应方式
//!ISOLATOROUTPUT RESPONSE

    char args[SEND_BUF];
    sprintf( args, "ISOLATOROUTPUT:RESPONSE? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!UART1|UART2,4800|7200|9600|14400|19200|38400|57600|115200|128000|2000000|2500000
int MRQ_setSENSORUART_BAUD( ViSession vi, char * val0,char * val1 )
{
//!设置波特率，单位bps
//!SENSORUART BAUD
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:BAUD %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!UART1|UART2
//!MRQ_SENSORUART_BAUD_1
//!val1
int MRQ_getSENSORUART_BAUD( ViSession vi, char * val0 , char * val1 )
{
//!查询RS232波特率
//!SENSORUART BAUD

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:BAUD? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!UART1|UART2,8|9
int MRQ_setSENSORUART_WORDLEN( ViSession vi, char * val0,char * val1 )
{
//!设置数据长度
//!SENSORUART WORDLEN
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:WORDLEN %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!UART1|UART2
//!MRQ_RS232_WORDLEN
//!val1
int MRQ_getSENSORUART_WORDLEN( ViSession vi, char * val0 , char * val1 )
{
//!查询RS232数据长度
//!SENSORUART WORDLEN

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:WORDLEN? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!UART1|UART2,NONE|RTS|CTS|RTS&CTS
int MRQ_setSENSORUART_FLOWCTL( ViSession vi, char * val0,char * val1 )
{
//!打开或关闭流控制
//!SENSORUART FLOWCTL
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:FLOWCTL %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!UART1|UART2
//!MRQ_RS232_FLOWCTL
//!val1
int MRQ_getSENSORUART_FLOWCTL( ViSession vi, char * val0 , char * val1 )
{
//!查询RS232流控制状态
//!SENSORUART FLOWCTL

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:FLOWCTL? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!UART1|UART2,NONE|EVEN|ODD
int MRQ_setSENSORUART_PARITY( ViSession vi, char * val0,char * val1 )
{
//!校验方式
//!SENSORUART PARITY
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:PARITY %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!UART1|UART2
//!MRQ_RS232_PARITY
//!val1
int MRQ_getSENSORUART_PARITY( ViSession vi, char * val0 , char * val1 )
{
//!查询RS232校验方式
//!SENSORUART PARITY

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:PARITY? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!UART1|UART2,1|0.5|2|1.5
int MRQ_setSENSORUART_STOPBIT( ViSession vi, char * val0,char * val1 )
{
//!设置停止位
//!SENSORUART STOPBIT
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:STOPBIT %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!UART1|UART2
//!MRQ_RS232_STOPBIT
//!val1
int MRQ_getSENSORUART_STOPBIT( ViSession vi, char * val0 , char * val1 )
{
//!查询RS232停止位
//!SENSORUART STOPBIT

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:STOPBIT? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!UART1|UART2
int MRQ_setSENSORUART_APPLYPARA( ViSession vi, char * val0 )
{
//!按照之前设置的参数配置接口
//!SENSORUART APPLYPARA
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:APPLYPARA %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM,ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4,OFF|ON
int MRQ_setSENSORUART_STATE( ViSession vi, char * val0,char * val1,char * val2 )
{
//!开启或关闭传感器UART
//!SENSORUART STATE
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:STATE %s,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!MRQ_SYSTEM_REVMOTION
//!val2
int MRQ_getSENSORUART_STATE( ViSession vi, char * val0,char * val1 , char * val2 )
{
//!查询传感器UART状态
//!SENSORUART STATE

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:STATE? %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val2, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM,U8
//!UART1|UART2,S1|S2|S3|S4
int MRQ_setSENSORUART_SOF( ViSession vi, char * val0,char * val1,int val2 )
{
//!设置数据帧头
//!SENSORUART SOF
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:SOF %s,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!byte
//!val2
int MRQ_getSENSORUART_SOF( ViSession vi, char * val0,char * val1 , int *val2 )
{
//!查询数据帧头
//!SENSORUART SOF

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:SOF? %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM,U8
//!UART1|UART2,S1|S2|S3|S4
int MRQ_setSENSORUART_FRAMELEN( ViSession vi, char * val0,char * val1,int val2 )
{
//!设置帧长度
//!SENSORUART FRAMELEN
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:FRAMELEN %s,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!byte
//!val2
int MRQ_getSENSORUART_FRAMELEN( ViSession vi, char * val0,char * val1 , int *val2 )
{
//!查询帧长度
//!SENSORUART FRAMELEN

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:FRAMELEN? %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM,U8
//!UART1|UART2,S1|S2|S3|S4
int MRQ_setSENSORUART_RECEIVENUM( ViSession vi, char * val0,char * val1,int val2 )
{
//!设置周期内接收的帧数
//!SENSORUART RECEIVENUM
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:RECEIVENUM %s,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!byte
//!val2
int MRQ_getSENSORUART_RECEIVENUM( ViSession vi, char * val0,char * val1 , int *val2 )
{
//!查询一个周期内接收的帧数
//!SENSORUART RECEIVENUM

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:RECEIVENUM? %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM,U32
//!UART1|UART2,S1|S2|S3|S4
int MRQ_setSENSORUART_SWITCHTIME( ViSession vi, char * val0,char * val1,int val2 )
{
//!设置切换周期
//!SENSORUART SWITCHTIME
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:SWITCHTIME %s,%s,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!uint32
//!val2
int MRQ_getSENSORUART_SWITCHTIME( ViSession vi, char * val0,char * val1 , int *val2 )
{
//!查询切换周期
//!SENSORUART SWITCHTIME

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:SWITCHTIME? %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!UART1|UART2,S1|S2|S3|S4
//!byte,char,char,char
//!val2,val3,val4,val5
int MRQ_getSENSORUART_DATA( ViSession vi, char * val0,char * val1 , int *val2,int *val3,int *val4,int *val5 )
{
//!查询数据
//!SENSORUART DATA

    char args[SEND_BUF];
    sprintf( args, "SENSORUART:DATA? %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 4 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val2 = atoi( paraAt( &para, 0) );
    *val3 = atoi( paraAt( &para, 1) );
    *val4 = atoi( paraAt( &para, 2) );
    *val5 = atoi( paraAt( &para, 3) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setTUNING_STATE( ViSession vi, int val0,char * val1 )
{
//!设置驱动开关状态
//!TUNING STATE
    char args[SEND_BUF];
    sprintf( args, "TUNING:STATE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getTUNING_STATE( ViSession vi, int val0 , char * val1 )
{
//!查询驱动开关状态
//!TUNING STATE

    char args[SEND_BUF];
    sprintf( args, "TUNING:STATE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],1/2|1/4
int MRQ_setTUNING_MINICURRRATIO( ViSession vi, int val0,char * val1 )
{
//!设置运行过程中的最低电流比
//!TUNING MINICURRRATIO
    char args[SEND_BUF];
    sprintf( args, "TUNING:MINICURRRATIO %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_DRIVER_MINICURRRATIO
//!val1
int MRQ_getTUNING_MINICURRRATIO( ViSession vi, int val0 , char * val1 )
{
//!查询运行过程中的最低电流比
//!TUNING MINICURRRATIO

    char args[SEND_BUF];
    sprintf( args, "TUNING:MINICURRRATIO? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,U16,U16
//![CHANNUM]
int MRQ_setTUNING_ENERGYEFFIC( ViSession vi, int val0,int val1,int val2 )
{
//!设置运行过程中能效的上下限，上下限一起设置，上限在前，下限在后，千分之
//!TUNING ENERGYEFFIC
    char args[SEND_BUF];
    sprintf( args, "TUNING:ENERGYEFFIC %d,%d,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!uint16,uint16
//!val1,val2
int MRQ_getTUNING_ENERGYEFFIC( ViSession vi, int val0 , int *val1,int *val2 )
{
//!查询运行过程中能效的上下限
//!TUNING ENERGYEFFIC

    char args[SEND_BUF];
    sprintf( args, "TUNING:ENERGYEFFIC? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    *val2 = atoi( paraAt( &para, 1) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM,ENUM
//![CHANNUM],1|2|4|8,32|8|2|1
int MRQ_setTUNING_CURRREGULATE( ViSession vi, int val0,char * val1,char * val2 )
{
//!设置电流调节的速度，上升速度和下降速度
//!TUNING CURRREGULATE
    char args[SEND_BUF];
    sprintf( args, "TUNING:CURRREGULATE %d,%s,%s" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//!
//!MRQ_TUNING_CURRREGULATE,MRQ_TUNING_CURRREGULATE_1
//!val1,val2
int MRQ_getTUNING_CURRREGULATE( ViSession vi, int val0 , char * val1,char * val2 )
{
//!查询电流调节的速度，上升速度和下降速度
//!TUNING CURRREGULATE

    char args[SEND_BUF];
    sprintf( args, "TUNING:CURRREGULATE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 2 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    strcpy(val2, paraAt( &para, 1 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!OFF|ON
int MRQ_setISOLATORIN_STATE( ViSession vi, char * val0 )
{
//!打开或关闭隔离输入
//!ISOLATORIN STATE
    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:STATE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getISOLATORIN_STATE( ViSession vi , char * val0 )
{
//!查询隔离输入的状态
//!ISOLATORIN STATE

    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:STATE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!RESERVE|LOW|RISE|FALL|HIGH
int MRQ_setISOLATORIN_TYPE( ViSession vi, char * val0 )
{
//!设置隔离输入电平类型
//!ISOLATORIN TYPE
    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:TYPE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_TRIGGER_LEVELTYPE_1
//!val0
int MRQ_getISOLATORIN_TYPE( ViSession vi , char * val0 )
{
//!查询隔离输入电平类型
//!ISOLATORIN TYPE

    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:TYPE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setISOLATORIN_RESPONSE( ViSession vi, char * val0 )
{
//!设置隔离输入的响应
//!ISOLATORIN RESPONSE
    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:RESPONSE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getISOLATORIN_RESPONSE( ViSession vi , char * val0 )
{
//!查询隔离输入的响应
//!ISOLATORIN RESPONSE

    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:RESPONSE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8
//![CHANNUM]
int MRQ_setISOLATORIN_RESPCHAN( ViSession vi, int val0 )
{
//!设置隔离输入的响应通道(不包括所有通道号：128和分组号)
//!ISOLATORIN RESPCHAN
    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:RESPCHAN %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!byte
//!val0
int MRQ_getISOLATORIN_RESPCHAN( ViSession vi , int *val0 )
{
//!查询隔离输入的响应通道
//!ISOLATORIN RESPCHAN

    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:RESPCHAN?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!CONTINUED|INTERVAL|SINGLE
int MRQ_setISOLATORIN_SMODE( ViSession vi, char * val0 )
{
//!设置隔离输入的采样模式
//!ISOLATORIN SMODE
    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:SMODE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_TRIGGER_PATTSMODE
//!val0
int MRQ_getISOLATORIN_SMODE( ViSession vi , char * val0 )
{
//!查询隔离输入的采样模式
//!ISOLATORIN SMODE

    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:SMODE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setISOLATORIN_SPERIOD( ViSession vi, int val0 )
{
//!设置隔离输入的采样周期，单位ms，最小值100
//!ISOLATORIN SPERIOD
    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:SPERIOD %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getISOLATORIN_SPERIOD( ViSession vi , int *val0 )
{
//!查询隔离输入的采样周期
//!ISOLATORIN SPERIOD

    char args[SEND_BUF];
    sprintf( args, "ISOLATORIN:SPERIOD?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!S1|S2|S3|S4,OFF|ON
int MRQ_setABSENCALARM_STATE( ViSession vi, char * val0,char * val1 )
{
//!设置绝对值编码器报警状态
//!ABSENCALARM STATE
    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:STATE %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getABSENCALARM_STATE( ViSession vi, char * val0 , char * val1 )
{
//!查询编码器报警状态
//!ABSENCALARM STATE

    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:STATE? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U32
//!S1|S2|S3|S4
int MRQ_setABSENCALARM_UPLIMIT( ViSession vi, char * val0,int val1 )
{
//!设置编码器角度上限
//!ABSENCALARM UPLIMIT
    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:UPLIMIT %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!uint32
//!val1
int MRQ_getABSENCALARM_UPLIMIT( ViSession vi, char * val0 , int *val1 )
{
//!查询编码器角度上限
//!ABSENCALARM UPLIMIT

    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:UPLIMIT? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U32
//!S1|S2|S3|S4
int MRQ_setABSENCALARM_DOWNLIMIT( ViSession vi, char * val0,int val1 )
{
//!设置编码器角度下限
//!ABSENCALARM DOWNLIMIT
    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:DOWNLIMIT %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!uint32
//!val1
int MRQ_getABSENCALARM_DOWNLIMIT( ViSession vi, char * val0 , int *val1 )
{
//!查询编码器角度下限
//!ABSENCALARM DOWNLIMIT

    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:DOWNLIMIT? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U32
//!S1|S2|S3|S4
int MRQ_setABSENCALARM_ZEROVALUE( ViSession vi, char * val0,int val1 )
{
//!设置编码器零位的值
//!ABSENCALARM ZEROVALUE
    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:ZEROVALUE %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!uint32
//!val1
int MRQ_getABSENCALARM_ZEROVALUE( ViSession vi, char * val0 , int *val1 )
{
//!查询编码器零位的值
//!ABSENCALARM ZEROVALUE

    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:ZEROVALUE? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!S1|S2|S3|S4,NONE|EXIS
int MRQ_setABSENCALARM_ZEROPOSITION( ViSession vi, char * val0,char * val1 )
{
//!设置上下限之间是否存在零点位置
//!ABSENCALARM ZEROPOSITION
    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:ZEROPOSITION %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!MRQ_ABSENCALARM_ZEROPOSITION_1
//!val1
int MRQ_getABSENCALARM_ZEROPOSITION( ViSession vi, char * val0 , char * val1 )
{
//!查询上下限之间是否存在零点位置
//!ABSENCALARM ZEROPOSITION

    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:ZEROPOSITION? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setABSENCALARM_RESPONSE( ViSession vi, char * val0 )
{
//!设置触发上下限后的响应
//!ABSENCALARM RESPONSE
    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:RESPONSE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getABSENCALARM_RESPONSE( ViSession vi , char * val0 )
{
//!查询响应
//!ABSENCALARM RESPONSE

    char args[SEND_BUF];
    sprintf( args, "ABSENCALARM:RESPONSE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,ENUM
//!S1|S2|S3|S4,OFF|ON
int MRQ_setDISTANCEALARM_STATE( ViSession vi, char * val0,char * val1 )
{
//!设置测距报警的状态
//!DISTANCEALARM STATE
    char args[SEND_BUF];
    sprintf( args, "DISTANCEALARM:STATE %s,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getDISTANCEALARM_STATE( ViSession vi, char * val0 , char * val1 )
{
//!查询测距报警的状态
//!DISTANCEALARM STATE

    char args[SEND_BUF];
    sprintf( args, "DISTANCEALARM:STATE? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U16
//!S1|S2|S3|S4
int MRQ_setDISTANCEALARM_ALARM1DIST( ViSession vi, char * val0,int val1 )
{
//!机械臂测距传感器1级警报触发距离，当检测到距离小于此值时开始通过总线发送警报
//!DISTANCEALARM ALARM1DIST
    char args[SEND_BUF];
    sprintf( args, "DISTANCEALARM:ALARM1DIST %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!uint16
//!val1
int MRQ_getDISTANCEALARM_ALARM1DIST( ViSession vi, char * val0 , int *val1 )
{
//!查询1级警报的触发距离
//!DISTANCEALARM ALARM1DIST

    char args[SEND_BUF];
    sprintf( args, "DISTANCEALARM:ALARM1DIST? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U16
//!S1|S2|S3|S4
int MRQ_setDISTANCEALARM_ALARM2DIST( ViSession vi, char * val0,int val1 )
{
//!设置2级警报的触发距离，当检测到距离小于此值时开始减速停止并发送警报
//!DISTANCEALARM ALARM2DIST
    char args[SEND_BUF];
    sprintf( args, "DISTANCEALARM:ALARM2DIST %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!uint16
//!val1
int MRQ_getDISTANCEALARM_ALARM2DIST( ViSession vi, char * val0 , int *val1 )
{
//!查询2级警报的触发距离
//!DISTANCEALARM ALARM2DIST

    char args[SEND_BUF];
    sprintf( args, "DISTANCEALARM:ALARM2DIST? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM,U16
//!S1|S2|S3|S4
int MRQ_setDISTANCEALARM_ALARM3DIST( ViSession vi, char * val0,int val1 )
{
//!设置3级警报的触发距离，当检测到距离小于此值时立即停止并发送警报
//!DISTANCEALARM ALARM3DIST
    char args[SEND_BUF];
    sprintf( args, "DISTANCEALARM:ALARM3DIST %s,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!ENUM
//!S1|S2|S3|S4
//!uint16
//!val1
int MRQ_getDISTANCEALARM_ALARM3DIST( ViSession vi, char * val0 , int *val1 )
{
//!查询3级警报的触发距离
//!DISTANCEALARM ALARM3DIST

    char args[SEND_BUF];
    sprintf( args, "DISTANCEALARM:ALARM3DIST? %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8
//![CHANNUM]
//!MRQ_NEWDRIVER_TYPE
//!val1
int MRQ_getNEWDRIVER_TYPE( ViSession vi, int val0 , char * val1 )
{
//!查询驱动板类型
//!NEWDRIVER TYPE

    char args[SEND_BUF];
    sprintf( args, "NEWDRIVER:TYPE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8
//!
int MRQ_setNEWDRIVER_CURRENT( ViSession vi, int val0 )
{
//!设置驱动工作最大电流，单位0.1A
//!NEWDRIVER CURRENT
    char args[SEND_BUF];
    sprintf( args, "NEWDRIVER:CURRENT %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!byte
//!val0
int MRQ_getNEWDRIVER_CURRENT( ViSession vi , int *val0 )
{
//!查询驱动工作最大电流
//!NEWDRIVER CURRENT

    char args[SEND_BUF];
    sprintf( args, "NEWDRIVER:CURRENT?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!RESERVE|RESERVE1|RESERVE2|32|16|8|4|2|1
int MRQ_setNEWDRIVER_MICROSTEPS( ViSession vi, char * val0 )
{
//!设置电机微步数
//!NEWDRIVER MICROSTEPS
    char args[SEND_BUF];
    sprintf( args, "NEWDRIVER:MICROSTEPS %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_NEWDRIVER_MICROSTEPS
//!val0
int MRQ_getNEWDRIVER_MICROSTEPS( ViSession vi , char * val0 )
{
//!查询电机微步数
//!NEWDRIVER MICROSTEPS

    char args[SEND_BUF];
    sprintf( args, "NEWDRIVER:MICROSTEPS?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],OFF|ON
int MRQ_setNEWDRIVER_STATE( ViSession vi, int val0,char * val1 )
{
//!设置驱动开关状态
//!NEWDRIVER STATE
    char args[SEND_BUF];
    sprintf( args, "NEWDRIVER:STATE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_SYSTEM_REVMOTION
//!val1
int MRQ_getNEWDRIVER_STATE( ViSession vi, int val0 , char * val1 )
{
//!查询驱动开关状态
//!NEWDRIVER STATE

    char args[SEND_BUF];
    sprintf( args, "NEWDRIVER:STATE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,ENUM
//![CHANNUM],END|START
int MRQ_setPDM_SAMPLESTATE( ViSession vi, int val0,char * val1 )
{
//!
//!PDM SAMPLESTATE
    char args[SEND_BUF];
    sprintf( args, "PDM:SAMPLESTATE %d,%s" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!MRQ_CLOCK_SYNCSTATE
//!val1
int MRQ_getPDM_SAMPLESTATE( ViSession vi, int val0 , char * val1 )
{
//!
//!PDM SAMPLESTATE

    char args[SEND_BUF];
    sprintf( args, "PDM:SAMPLESTATE? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val1, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U8,U8
//![CHANNUM]
int MRQ_setPDM_ENCDIV( ViSession vi, int val0,int val1 )
{
//!
//!PDM ENCDIV
    char args[SEND_BUF];
    sprintf( args, "PDM:ENCDIV %d,%d" , val0,val1);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!U8
//![CHANNUM]
//!byte
//!val1
int MRQ_getPDM_ENCDIV( ViSession vi, int val0 , int *val1 )
{
//!
//!PDM ENCDIV

    char args[SEND_BUF];
    sprintf( args, "PDM:ENCDIV? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8
//![CHANNUM]
//!uint16
//!val1
int MRQ_getPDM_MICSTEPCOUNT( ViSession vi, int val0 , int *val1 )
{
//!MICSTEPDATA?U8,U16,U16
//!PDM MICSTEPCOUNT

    char args[SEND_BUF];
    sprintf( args, "PDM:MICSTEPCOUNT? %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val1 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!U8,U16,U16
//!
//!byte
//!val3
int MRQ_getPDM_MICSTEPDATA( ViSession vi, int val0,int val1,int val2 , int *val3 )
{
//!
//!PDM MICSTEPDATA

    char args[SEND_BUF];
    sprintf( args, "PDM:MICSTEPDATA? %d,%d,%d" , val0,val1,val2);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val3 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!OFF|ON
int MRQ_setOTP_STATE( ViSession vi, char * val0 )
{
//!打开或关闭OTP功能(Overtemperatureprotection)
//!OTP STATE
    char args[SEND_BUF];
    sprintf( args, "OTP:STATE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getOTP_STATE( ViSession vi , char * val0 )
{
//!查询OTP状态
//!OTP STATE

    char args[SEND_BUF];
    sprintf( args, "OTP:STATE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U16
//!
int MRQ_setOTP_THRESHOLD( ViSession vi, int val0 )
{
//!设置过温保护的阈值，电机温度大于等于此值后执行响应动作
//!OTP THRESHOLD
    char args[SEND_BUF];
    sprintf( args, "OTP:THRESHOLD %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint16
//!val0
int MRQ_getOTP_THRESHOLD( ViSession vi , int *val0 )
{
//!查询过温保护阈值
//!OTP THRESHOLD

    char args[SEND_BUF];
    sprintf( args, "OTP:THRESHOLD?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setOTP_RESPONSE( ViSession vi, char * val0 )
{
//!发生过温保护响应
//!OTP RESPONSE
    char args[SEND_BUF];
    sprintf( args, "OTP:RESPONSE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getOTP_RESPONSE( ViSession vi , char * val0 )
{
//!查询过温保护响应
//!OTP RESPONSE

    char args[SEND_BUF];
    sprintf( args, "OTP:RESPONSE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!U32
//!
int MRQ_setOTP_PERIOD( ViSession vi, int val0 )
{
//!设置上报周期，单位ms
//!OTP PERIOD
    char args[SEND_BUF];
    sprintf( args, "OTP:PERIOD %d" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!uint32
//!val0
int MRQ_getOTP_PERIOD( ViSession vi , int *val0 )
{
//!查询上报周期
//!OTP PERIOD

    char args[SEND_BUF];
    sprintf( args, "OTP:PERIOD?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!
//!
//!uint32
//!val0
int MRQ_getOTP_DATA( ViSession vi , int *val0 )
{
//!上报数据，上报打开时下位机按照上报周期主动上报；上位机也可以主动查询
//!OTP DATA

    char args[SEND_BUF];
    sprintf( args, "OTP:DATA?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atoi( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!OFF|ON
int MRQ_setANALOGIN_STATE( ViSession vi, char * val0 )
{
//!打开或关闭模拟传感器
//!ANALOGIN STATE
    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:STATE %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_SYSTEM_REVMOTION
//!val0
int MRQ_getANALOGIN_STATE( ViSession vi , char * val0 )
{
//!查询模拟传感器的状态
//!ANALOGIN STATE

    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:STATE?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!F32
//!
int MRQ_setANALOGIN_THRESHOLDH( ViSession vi, float val0 )
{
//!设置传感器触发电压阈值高
//!ANALOGIN THRESHOLDH
    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:THRESHOLDH %g" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!f32
//!val0
int MRQ_getANALOGIN_THRESHOLDH( ViSession vi , float *val0 )
{
//!查询传感器触发电压阈值高
//!ANALOGIN THRESHOLDH

    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:THRESHOLDH?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!F32
//!
int MRQ_setANALOGIN_THRESHOLDL( ViSession vi, float val0 )
{
//!设置传感器触发电压阈值低
//!ANALOGIN THRESHOLDL
    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:THRESHOLDL %g" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!f32
//!val0
int MRQ_getANALOGIN_THRESHOLDL( ViSession vi , float *val0 )
{
//!查询传感器触发电压阈值低
//!ANALOGIN THRESHOLDL

    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:THRESHOLDL?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    *val0 = atof( paraAt( &para, 0) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setANALOGIN_RESPONSEH( ViSession vi, char * val0 )
{
//!设置传感器电压阈值高的触发响应
//!ANALOGIN RESPONSEH
    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:RESPONSEH %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getANALOGIN_RESPONSEH( ViSession vi , char * val0 )
{
//!查询传感器电压阈值高的触发响应
//!ANALOGIN RESPONSEH

    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:RESPONSEH?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

//!ENUM
//!NONE|ALARM|STOP|ALARM&STOP
int MRQ_setANALOGIN_RESPONSEL( ViSession vi, char * val0 )
{
//!设置传感器电压阈值低的触发响应
//!ANALOGIN RESPONSEL
    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:RESPONSEL %s" , val0);
    int ret;
    ret = miSend( vi, args, strlen(args) );
    return ret;
}

//!
//!
//!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
//!val0
int MRQ_getANALOGIN_RESPONSEL( ViSession vi , char * val0 )
{
//!查询传感器电压阈值低的触发响应
//!ANALOGIN RESPONSEL

    char args[SEND_BUF];
    sprintf( args, "ANALOGIN:RESPONSEL?"  );
    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 1 ){ return -1; }
    struPara para;
    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }
    if ( parasCount( &para) < 1 )
    {
         parasDeInit(&para);
         return -1;
    }
    strcpy(val0, paraAt( &para, 0 ) );
    parasDeInit(&para);
    return 0;
}

