#ifndef _SIANNJU_H_
#define _SIANNJU_H_

#include "visa.h"

int Sinanju_move( ViSession vi, int ax, int page, int mode,
				  float x1,float y1, float z1, float h1,
				  float x2,float y2, float z2, float h2,
				  float t );
				  
int Sinanju_preMove( ViSession vi, int ax, int page,int mode,
				  float x1,float y1, float z1, float h1,
				  float x2,float y2, float z2, float h2,
				  float t );

int Sinanju_goto( ViSession vi, int ax, int page,int mode,
                  float x1,float y1, float z1,
				  float t );

int Sinanju_stepX( ViSession vi, int ax, int page,int mode,

				  float d,
				  float t );
int Sinanju_stepY( ViSession vi, int ax, int page,int mode,

				  float d,
				  float t );
int Sinanju_stepZ( ViSession vi, int ax, int page,int mode,

				  float d,
				  float t );
int Sinanju_stepH( ViSession vi, int ax, int page,int mode,

				  float d,
				  float t );
				  
int Sinanju_Center( ViSession vi, int ax, int page );
int Sinanju_Fold( ViSession vi, int ax, int page );


int Sinanju_getPose( ViSession vi,
					 float *val0,
					 float *val1,
					 float *val2,
					 float *val3
					);
int Sinanju_getDist( ViSession vi,
					 float *val0,
					 float *val1,
					 float *val2,
					 float *val3
					);
					
int Sinanju_getRad( ViSession vi,
					float *val0,
					 float *val1,
					 float *val2,
					 float *val3 );
					
#endif
