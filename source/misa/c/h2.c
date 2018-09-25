#include "visa_adapter.h"
#include "h2.h"

int h2_move( ViSession vi, int ax, int page,
                  float x1,float y1,
                  float x2,float y2,
                  float t )
{
    _robo_set( sprintf(args, "MOVE %d,%d,%g,%g,%g,%g,%g", ax,page, x1,y1, x2,y2, t) )
}

int h2_getPose( ViSession vi,
                     float *val0,
                     float *val1

                    )
{
    _robo_get_float_2( sprintf(args, "POSE?") );
}

