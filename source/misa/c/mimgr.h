#ifndef _MI_MGR_H_
#define _MI_MGR_H_

#include "visa.h"

int miMgr_find( ViSession vi );
int miMgr_getResource( ViSession vi, char *val );

int miMgr_openBus( ViSession vi );
int miMgr_closeBus( ViSession vi );

int miMgr_StopDevice( ViSession vi );
int miMgr_ResetDevice( ViSession vi );

#endif