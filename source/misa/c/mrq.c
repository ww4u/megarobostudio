#include "visa_adapter.h"

int MRQ_move( ViSession vi, int ax, int page, float t, float angle, float endV )
{
    _robo_set( sprintf( args, "MOVE %d,%d,%g,%g,%g", ax, page, t, angle, endV ))
}
int MRQ_preMove( ViSession vi, int ax, int page, float t, float angle, float endV )
{
    _robo_set( sprintf( args, "PREMOVE %d,%d,%g,%g,%g", ax, page, t, angle, endV ))
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
