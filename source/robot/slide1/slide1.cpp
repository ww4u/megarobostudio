
#include "slide1.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

robotSlide1::robotSlide1()
{
    mClass = "Slide1";
    mId = robot_slide1;
    setAxes( 1 );
    setAxesDefName( 1 );
    setJointName( 1 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}




