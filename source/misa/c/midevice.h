#ifndef _MI_DEVICE_H_
#define _MI_DEVICE_H_

#include "visa.h"

ViSession miOpen_( const char *pName,
            const char *addr,
                        unsigned int port );

ViSession miOpen( const char *pName );

void miClose( ViSession vi );

#endif
