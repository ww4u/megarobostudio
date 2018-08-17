#include "visa_adapter.h"
#include "sinanju.h"

int Sinanju_move( ViSession vi, int ax, int page,int mode,
                  float x1,float y1, float z1, float h1,
                  float x2,float y2, float z2, float h2,
                  float t )
{
    _robo_set( sprintf(args, "MOVE %d,%d,%d,%g,%g,%g,%g,%g,%g,%g,%g,%g", ax,page,mode, x1,y1,z1,h1, x2,y2,z2,h2,t) )
}

int Sinanju_preMove( ViSession vi, int ax, int page,int mode,
                  float x1,float y1, float z1, float h1,
                  float x2,float y2, float z2, float h2,
                  float t )
{
    _robo_set( sprintf(args, "PREMOVE %d,%d,%d, %g,%g,%g,%g,%g,%g,%g,%g,%g", ax,page,mode, x1,y1,z1,h1, x2,y2,z2,h2, t) )
}

int Sinanju_goto( ViSession vi, int ax, int page,int mode,
                  float x1,float y1, float z1,
                  float t )
{
    _robo_set( sprintf(args, "GOTO %d,%d,%d,%g,%g,%g,%g", ax,page,mode, x1,y1,z1, t ) )
}

int Sinanju_stepX( ViSession vi, int ax, int page,int mode,
                  float d,
                  float t )
{
    _robo_set( sprintf(args, "STEP:X %d,%d,%d,%g,%g", ax,page,mode, d, t ) )
}
int Sinanju_stepY( ViSession vi, int ax, int page,int mode,
                  float d,
                  float t )
{
    _robo_set( sprintf(args, "STEP:Y %d,%d,%d,%g,%g", ax,page,mode, d, t ) )
}
int Sinanju_stepZ( ViSession vi, int ax, int page,int mode,
                  float d,
                  float t )
{
    _robo_set( sprintf(args, "STEP:Z %d,%d,%d,%g,%g", ax,page,mode, d, t ) )
}
int Sinanju_stepH( ViSession vi, int ax, int page,int mode,
                  float d,
                  float t )
{
    _robo_set( sprintf(args, "STEP:H %d,%d,%d,%g,%g", ax,page,mode, d, t ) )
}

int Sinanju_Center( ViSession vi, int ax, int page )
{
    _robo_set( sprintf(args, "CENTER %d,%d", ax,page ) )
}
int Sinanju_Fold( ViSession vi, int ax, int page )
{
    _robo_set( sprintf(args, "FOLD %d,%d", ax,page ) )
}

int Sinanju_getPose( ViSession vi,
                     float *val0,
                     float *val1,
                     float *val2,
                     float *val3
                    )
{
    _robo_get_float_4( sprintf(args, "POSE?") );
}
int Sinanju_getDist( ViSession vi,
                     float *val0,
                     float *val1,
                     float *val2,
                     float *val3
                    )
{
    _robo_get_float_4( sprintf(args, "DISTANCE?") );
}

int Sinanju_getRad( ViSession vi,
                    float *val0,
                     float *val1,
                     float *val2,
                     float *val3 )
{
    _robo_get_float_4( sprintf(args, "JOINT:RAD?") );
}
