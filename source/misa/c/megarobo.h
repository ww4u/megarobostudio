#ifndef MEGAROBO_H
#define MEGAROBO_H

#include "visa.h"

#define mega_robo_idle          0
#define mega_robo_run_reqed     1
#define mega_robo_program       2
#define mega_robo_calcing       3

#define mega_robo_calcend       4
#define mega_robo_standby       5
#define mega_robo_prerun        6
#define mega_robo_running       7

#define mera_robo_prestop       8

int robo_getIdn( ViSession vi, char *idn );
int robo_getOpc( ViSession vi, int *pOpc );

int robo_lrn( ViSession vi, char *lrn );
int robo_hrst( ViSession vi );

int robo_getVersion( ViSession vi, char *v );

int robo_program( ViSession vi, int ax, int page, char *v );
int robo_call( ViSession vi, int ax, int page, int cycle, int mode );

int robo_run( ViSession vi, int ax, int page );
int robo_stop( ViSession vi, int ax, int page );

int robo_getState( ViSession vi, int ax, int page, int *state );

int robo_waitx( ViSession vi, int ax, int page, int dst, int tmo, int tick );
int robo_waitEnd( ViSession vi, int ax, int page, int tmo );
int robo_waitIdle( ViSession vi, int ax, int page, int tmo );

#endif // MEGAROBO_H
