
#include "megarobo.h"
#include "visa_adapter.h"

int robo_getIdn( ViSession vi, char *val0 )
{
    _robo_get_str( sprintf(args,"*IDN?") )
}
int robo_getOpc( ViSession vi, int *val0 )
{
	_robo_get_int( sprintf(args,"*OPC?") )
}
int robo_lrn( ViSession vi, char *val0 )
{
    _robo_set( sprintf( args, "*LRN \"%s\"" , val0 ) )
}

int robo_hrst( ViSession vi )
{
    _robo_set( sprintf( args, "HRST" ) )
}

int robo_getVersion( ViSession vi, char *val0 )
{
    _robo_get_str( sprintf(args,"VERSION?") )
}

int robo_program( ViSession vi, int ax, int page, char *name )
{
    _robo_set( sprintf( args, "PROGRAM %d,%d,\"%s\"" , ax, page, name ) )
}
int robo_call( ViSession vi, int ax, int page, int cycle, int mode )
{
    _robo_set( sprintf( args, "CALL %d,%d,%d,%d" , ax, page, cycle, mode ) )
}

int robo_run( ViSession vi, int ax, int page )
{
    _robo_set( sprintf( args, "RUN %d,%d" , ax, page ) )
}
int robo_stop( ViSession vi, int ax, int page )
{
    _robo_set( sprintf( args, "STOP %d,%d" , ax, page ) )
}

int robo_getState( ViSession vi, int ax, int page, int *val0 )
{
    _robo_get_int( sprintf( args, "STATE? %d,%d", ax, page ) )
}

int robo_waitx( ViSession vi, int ax, int page, int dst, int tmoms, int tickms )
{
    int stat;

    while( tmoms > 0 )
    {
        sleep_ms( tickms );
        tmoms -= tickms;

        //! get the state
        if ( robo_getState( vi, ax, page, &stat) != 0 )
        { return -1; }

        if ( stat == dst )
        { return 0; }
    }

    return -1;
}
int robo_waitEnd( ViSession vi, int ax, int page, int tmo )
{
    return robo_waitx( vi, ax, page, mega_robo_calcend, tmo, WAIT_TICK_MS );
}
int robo_waitIdle( ViSession vi, int ax, int page, int tmo )
{
    return robo_waitx( vi, ax, page, mega_robo_idle, tmo, WAIT_TICK_MS );
}

