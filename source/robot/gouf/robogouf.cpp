#include "robogouf.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

roboGouf::roboGouf( )
{
    mClass = "MRV-M3604";
    mId = robot_gouf;

    setAxes( 4 );
    setAxesDefName( 4 );
    setJointName( 4 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

