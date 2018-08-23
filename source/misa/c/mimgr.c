
#include "visa_adapter.h"

int miMgr_find( ViSession vi )
{
	 _robo_set( sprintf( args, "*FIND" ) )
}
int miMgr_getResource( ViSession vi, char *val0 )
{
	_robo_get_str( sprintf(args,"RESOURCE?") )
}

int miMgr_openBus( ViSession vi )
{
	_robo_set( sprintf( args, "DEVICE:OPEN" ) )
}
int miMgr_closeBus( ViSession vi )
{
	_robo_set( sprintf( args, "DEVICE:CLOSE" ) )
}

int miMgr_StopDevice( ViSession vi )
{
	_robo_set( sprintf( args, "DEVICE:STOP" ) )
}
int miMgr_ResetDevice( ViSession vi )
{
	_robo_set( sprintf( args, "DEVICE:RESET" ) )
}
