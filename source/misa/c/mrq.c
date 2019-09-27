#include "visa_adapter.h"

int MRQ_move( ViSession vi, int ax, int page, float t, float angle, float endV )
{
    _robo_set( sprintf( args, "MOVE %d,%d,%g,%g,%g", ax, page, t, angle, endV ))
}
int MRQ_preMove( ViSession vi, int ax, int page, float t, float angle, float endV )
{
    _robo_set( sprintf( args, "PREMOVE %d,%d,%g,%g,%g", ax, page, t, angle, endV ))
}
//! 0,0
//! t1, angle1
//! t2, angle2
int MRQ_preMoveT( ViSession vi, int ax, int page,
                  float t1, float angle1,
                  float t2, float angle2 )
{
    _robo_set( sprintf( args, "PREMOVEJ %d,%d,%g,%g,%g,%g",
                                ax, page,
                                t1, angle1,
                                t2 - t1, angle2 ) );
}

int MRQ_getIncAngle( ViSession vi, int ax, float *val0 )
{
    _robo_get_float( sprintf( args, "ANGLE:INCREASE? %d", ax ) );
}
int MRQ_getAbsAngle( ViSession vi, int ax, float *val0 )
{
    _robo_get_float( sprintf( args, "ANGLE:ABSOLUTE? %d", ax ) );
}
int MRQ_getDistance( ViSession vi, int ax, float *val0 )
{
    _robo_get_float( sprintf( args, "DISTANCE? %d", ax ) );
}

int MRQ_setFanDuty( ViSession vi, int duty )
{
    _robo_set( sprintf( args, "FANDUTY %d", duty ) );
}
int MRQ_setLedDuty( ViSession vi, int ax, int duty )
{
    _robo_set( sprintf( args, "LEDDUTY %d,%d", ax, duty ) );
}

int MRQ_setFAN( ViSession vi, int duty, int freq )
{
    _robo_set( sprintf( args, "LED %d,%d,%d", 0, duty,freq ) );
}
int MRQ_setLED( ViSession vi, int ax, int duty, int freq )
{
    _robo_set( sprintf( args, "LED %d,%d,%d", ax, duty,freq ) );
}

int MRQ_getZeroValid( ViSession vi, int *val0 )
{
    _robo_get_int( sprintf( args, "ENCODER:ZEROVALID?") );
}
int MRQ_getZero( ViSession vi, int ax, float *val0 )
{
    _robo_get_float( sprintf( args, "ENCODER:ZERO? %d", ax ) );
}

int MRQ_getSensorUartData( ViSession vi, char *pu, char *ps, char * ary, int len )
{
    char args[SEND_BUF];
    sprintf( args, "SENSORUART:DATA? %s,%s", pu, ps );

    int ret;
    ret = miSend( vi, args, strlen(args) );
    if ( ret != 0 ){ return ret; }
    ret = miWaitRead(vi);
    if ( ret != 0 ){ return ret; }

    char recvBuf[RECV_BUF];
    int retCount;
    ret = miRecv( vi, recvBuf, sizeof(recvBuf), &retCount);
    if ( ret != 0 ){ return ret; }
    if ( retCount < 2 ){ return -1; }

    //! split the para
    if ( recvBuf[0] != '#' )
    { return -1; }
    int headLen;
    headLen = recvBuf[1] - '0';
    if ( headLen > 0 && headLen <=9 )
    {}
    else
    { return -2; }

    //char header[ headLen + 1];
    char header[2048];
    strncpy( header, recvBuf+2, headLen );
    header[ headLen ] = 0;

    int padLen = atoi( header );

    if (  padLen <= 0 || padLen > len )
    { return -2; }

    memcpy( ary, recvBuf + 2 + headLen, padLen );

    return padLen;
}
