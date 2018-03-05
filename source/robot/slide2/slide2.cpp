
#include "slide2.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

robotSlide2::robotSlide2()
{
    mClass = "Slide2";
    mId = robot_slide2;
    setAxes( 1 );
    setAxesDefName( 1 );
    setJointName( 1 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}




