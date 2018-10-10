#ifndef _MRQ_H_
#define _MRQ_H_

#include "visa.h"

int MRQ_move( ViSession vi, int ax, int page, float t, float angle, float endV );
int MRQ_preMove( ViSession vi, int ax, int page, float t, float angle, float endV );
int MRQ_preMoveT( ViSession vi, int ax, int page,
                  float t1, float angle1,
                  float t2, float angle2 );

int MRQ_getIncAngle( ViSession vi, int ax, float *val );
int MRQ_getAbsAngle( ViSession vi, int ax, float *val );
int MRQ_getDistance( ViSession vi, int ax, float *val );

int MRQ_setFanDuty( ViSession vi, int duty );
int MRQ_setLedDuty( ViSession vi, int ax, int duty );

int MRQ_setFAN( ViSession vi, int ax, int duty, int freq );
int MRQ_setLED( ViSession vi, int ax, int duty, int freq );

int MRQ_getZeroValid( ViSession vi, int *val );
int MRQ_getZeros( ViSession, int ax, float *val );

int MRQ_getSensorUartData( ViSession vi, char *pu, char *ps, char * ary, int len );

#endif
