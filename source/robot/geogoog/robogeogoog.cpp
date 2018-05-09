#include "robogeogoog.h"
#include "../../device/mrq/deviceMRQ.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

roboGeogoog::roboGeogoog()
{
    mClass = "MRQ-M-23-04";
    mId = robot_geogoog;
    setAxes( 4 );
    setAxesDefName( 4 );
    setJointName( 4 );

    mDOs = 4;
    mDIs = 0;
    mISOs = 2;
    mISIs = 1;
    mAINs = 0;
    setUarts( 2 );
    setUartSensors( 4 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

int roboGeogoog::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboGeogoog::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
