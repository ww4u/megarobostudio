#ifndef _VISA_ADAPTER_H_
#define _VISA_ADAPTER_H_

#include "visa.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//! configs
#define SEND_BUF 256
#define RECV_BUF 256

#define WAIT_TICK_MS    500
#define WAIT_TMO        2000

//!
#include <windows.h>
#define sleep_ms    Sleep


int miSend( ViSession vi, char *buf, int len );
int miWaitRead( ViSession vi );
int miRecv( ViSession vi, char *pBuf, int capCount, int *pRetCount );


typedef struct 
{
    char *mBuf;
	char *mParaBuf;
	
	int  *mSta;
	int  *mLen;
	
	int mPos;
	int mPCnt;
	
}struPara;

int parasInit( struPara *pCon, char *paras );
int parasMalloc( struPara *pCon, char *paras );
int parasDeload( struPara *pCon );

int parasCount( struPara *pCon );
char *paraAt( struPara *pCon, int id );
int parasDeInit( struPara *pCon );

//! macros
#define _robo_set( fmt )       char args[SEND_BUF];\
                                fmt;\
                                int ret;\
                                ret = miSend( vi, args, strlen(args) );\
                                \
                                return ret;

#define _robo_get_str( fmt )        char args[SEND_BUF];\
                                    fmt;\
                                    int ret;\
                                    ret = miSend( vi, args, strlen(args) );\
                                    if ( ret != 0 ){ return ret; }\
                                    ret = miWaitRead(vi);\
                                    if ( ret != 0 ){ return ret; }\
                                    \
                                    char recvBuf[RECV_BUF];\
                                    int retCount;\
                                    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);\
                                    if ( ret != 0 ){ return ret; }\
                                    if ( retCount < 1 ){ return -1; }\
                                    struPara para;\
                                    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }\
                                    if ( parasCount( &para) < 1 )\
                                    {\
                                         parasDeInit(&para);\
                                         return -1;\
                                    }\
                                    strcpy(val0, paraAt( &para, 0 ) );\
                                    parasDeInit(&para);\
                                    return 0;

#define _robo_get_int( fmt )        char args[SEND_BUF];\
                                    fmt;\
                                    int ret;\
                                    ret = miSend( vi, args, strlen(args) );\
                                    if ( ret != 0 ){ return ret; }\
                                    ret = miWaitRead(vi);\
                                    if ( ret != 0 ){ return ret; }\
                                    \
                                    char recvBuf[RECV_BUF];\
                                    int retCount;\
                                    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);\
                                    if ( ret != 0 ){ return ret; }\
                                    if ( retCount < 1 ){ return -1; }\
                                    struPara para;\
                                    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }\
                                    if ( parasCount( &para) < 1 )\
                                    {\
                                         parasDeInit(&para);\
                                         return -1;\
                                    }\
                                    *val0 = atoi( paraAt( &para, 0) );\
                                    parasDeInit(&para);\
                                    return 0;

#define _robo_get_float( fmt )        char args[SEND_BUF];\
                                    fmt;\
                                    int ret;\
                                    ret = miSend( vi, args, strlen(args) );\
                                    if ( ret != 0 ){ return ret; }\
                                    ret = miWaitRead(vi);\
                                    if ( ret != 0 ){ return ret; }\
                                    \
                                    char recvBuf[RECV_BUF];\
                                    int retCount;\
                                    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);\
                                    if ( ret != 0 ){ return ret; }\
                                    if ( retCount < 1 ){ return -1; }\
                                    struPara para;\
                                    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }\
                                    if ( parasCount( &para) < 1 )\
                                    {\
                                         parasDeInit(&para);\
                                         return -1;\
                                    }\
                                    *val0 = atof( paraAt( &para, 0) );\
                                    parasDeInit(&para);\
                                    return 0;

#define _robo_get_float_4( fmt )        char args[SEND_BUF];\
                                    fmt;\
                                    int ret;\
                                    ret = miSend( vi, args, strlen(args) );\
                                    if ( ret != 0 ){ return ret; }\
                                    ret = miWaitRead(vi);\
                                    if ( ret != 0 ){ return ret; }\
                                    \
                                    char recvBuf[RECV_BUF];\
                                    int retCount;\
                                    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);\
                                    if ( ret != 0 ){ return ret; }\
                                    if ( retCount < 1 ){ return -1; }\
                                    struPara para;\
                                    if ( parasInit( &para, recvBuf) != 0 ) { return -1; }\
                                    if ( parasCount( &para) < 1 )\
                                    {\
                                         parasDeInit(&para);\
                                         return -1;\
                                    }\
                                    *val0 = atof( paraAt( &para, 0) );\
                                    *val1 = atof( paraAt( &para, 1) );\
                                    *val2 = atof( paraAt( &para, 2) );\
                                    *val3 = atof( paraAt( &para, 3) );\
                                    parasDeInit(&para);\
                                    return 0;

#endif
