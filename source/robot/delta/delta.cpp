
#include "delta.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

robotDelta::robotDelta()
{
    mClass = "Delta";
    mId = robot_delta;

    setAxes( 4 );

    setAxesDefName( 4 );
    setJointName( 4 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}




