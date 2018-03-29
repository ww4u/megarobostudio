#include "robogeo8.h"
#include "../../device/mrq/deviceMRQ.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

roboGeo8::roboGeo8()
{
    mClass = "Geogoog8";
    mId = robot_geogoog_8;
    setAxes( 8 );
    setAxesDefName( 8 );
    setJointName( 8 );

    mDOs = 0;
    mDIs = 0;
    mISOs = 0;
    mAINs = 0;
    mUARTs = 0;

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

int roboGeo8::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboGeo8::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
