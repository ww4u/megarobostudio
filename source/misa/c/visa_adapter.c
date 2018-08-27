#include "visa_adapter.h"

#include "string.h"
#include "ctype.h"  //! isspace
#include "stdlib.h" //! malloc

int miSend( ViSession vi, char *buf, int len )
{
    ViUInt32 retCount;
    ViStatus viSta;

    char sendBuf[len + 2 ];     //! \r\n
    memcpy( sendBuf, buf, len );
    sendBuf[ len ] = 0x0d;
    sendBuf[ len + 1] = 0x0a;

    viSta = viWrite( vi, (ViBuf)sendBuf, (ViUInt32)(len+2), &retCount );
    if ( viSta != VI_SUCCESS )
    { return -1; }

    if ( retCount != (ViUInt32)(len + 2) )
    { return -1; }

    return 0;
}
int miWaitRead( ViSession vi )
{ return 0; }
int miRecv( ViSession vi, char *pBuf, int capCount, int *pRetCount )
{
    ViStatus viSta;

    ViUInt32 retCount;
    char recvBuf[ capCount + 1];
    viSta = viRead( vi, (ViPBuf)recvBuf, capCount, &retCount );
    if ( viSta == VI_SUCCESS_TERM_CHAR || viSta == VI_SUCCESS )
    { }
    else
    { return -1; }

    //! trim the \"
    int seqCnt = 0;
    for ( unsigned int i = 0; i < retCount; i++ )
    {
        if ( recvBuf[i] != '\"' )
        {
            pBuf[seqCnt++] = recvBuf[i];
        }
    }

    *pRetCount = seqCnt;

    return 0;
}

int parasInit( struPara *pCon, char *paras )
{
	//! init 
	pCon->mBuf = NULL;
	pCon->mParaBuf = NULL;
	pCon->mSta = NULL;
	pCon->mLen = NULL;
	
	pCon->mPos = 0;
	pCon->mPCnt = 0;

	int ret;
	ret = parasMalloc( pCon, paras );
	if ( ret != 0 )
	{ return ret; }

	//! copy data
	int len = strlen( paras );
	memcpy( pCon->mBuf, paras, len + 1 );
	
    parasDeload( pCon );
	
	return 0;
}

#define para_malloc( buf, type, len )  buf = (type *)malloc( (len) );\
										if ( buf == NULL ) \
										{ ret = -1; break; }
#define para_free( buf ) if ( buf != NULL ) \
						 { free(buf); buf = NULL; }	
int parasMalloc( struPara *pCon, char *paras )
{
	//! malloc
    int len = strlen( paras );
   
    int ret = 0;
    do
	{
		para_malloc( pCon->mBuf, char, len + 1 );
		para_malloc( pCon->mParaBuf, char, len + 1 );
        para_malloc( pCon->mSta, int, len * sizeof(int) );
        para_malloc( pCon->mLen, int, len * sizeof(int) );
		
	}while( 0 );
	
	if ( ret != 0 )
	{
		para_free( pCon->mBuf );
		para_free( pCon->mParaBuf );
		para_free( pCon->mSta );
        para_free( pCon->mLen );
	}

	return ret; 
}

int parasDeload( struPara *pCon )
{
	int len = strlen( pCon->mBuf );
	
	//! find the ,
	int pCnt = 0; 
	int iPos = 0;
    int bChared = 0;
	for ( int i = 0; i < len; i++ )
	{
		if ( pCon->mBuf[i] == ',' )
		{
			if ( bChared )
			{
				if ( i - iPos > 0 )
				{
					pCon->mSta[ pCnt ] = iPos;
                    pCon->mLen[ pCnt ] = i - iPos;
					pCnt++;
				}
				else
				{}
			}
			 
            bChared = 0;
		}
		else if ( isspace( pCon->mBuf[i] ) )
		{}
		else
		{
			if ( bChared )
			{}
			else 
			{ 
				iPos = i; 
                bChared = 1;
			}
		}
	}

	if ( bChared )
	{ 
		if ( len - iPos > 0 )
		{
			pCon->mSta[ pCnt ] = iPos;
            pCon->mLen[ pCnt ] = len - iPos;
			pCnt++;
		}
	}
	
	//! save count
	pCon->mPCnt = pCnt;

    return 0;
}

int parasCount( struPara *pCon )
{
	return pCon->mPCnt;
}

char *paraAt( struPara *pCon, int id )
{
    if ( id >= 0 && id <pCon->mPCnt )
	{}
	else
	{ return NULL; }

    memcpy( pCon->mParaBuf, pCon->mSta[ id ] + pCon->mBuf, pCon->mLen[id] );
    pCon->mParaBuf[ pCon->mLen[id] ] = 0;
	
	return pCon->mParaBuf;
}

int parasDeInit( struPara *pCon )
{
    para_free( pCon->mBuf );
	para_free( pCon->mParaBuf );
	para_free( pCon->mSta );
    para_free( pCon->mLen );

    return 0;
}
