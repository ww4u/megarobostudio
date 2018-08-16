#ifndef _VISA_ADAPTER_H_
#define _VISA_ADAPTER_H_

#include "visa.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define SEND_BUF 256
#define RECV_BUF 256

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

#endif
