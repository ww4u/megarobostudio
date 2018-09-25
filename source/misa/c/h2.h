#ifndef _ROBO_H2_H_
#define _ROBO_H2_H_

int h2_move( ViSession vi, int ax, int page,
                  float x1,float y1,
                  float x2,float y2,
                  float t );

int h2_getPose( ViSession vi,
                     float *val0,
                     float *val1

                    );

#endif

