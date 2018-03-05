
#include "motor.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

robotMotor::robotMotor()
{
    mClass = "Motor";
    mId = robot_motor;
    setAxes( 1 );
    setAxesDefName( 1 );
    setJointName( 1 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

