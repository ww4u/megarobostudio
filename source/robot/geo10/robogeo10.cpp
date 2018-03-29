#include "robogeo10.h"
#include "../../device/mrq/deviceMRQ.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

roboGeo10::roboGeo10()
{
    mClass = "Geogoog10";
    mId = robot_geogoog_10;
    setAxes( 10 );
    setAxesDefName( 10 );
    setJointName( 10 );

    mDOs = 0;
    mDIs = 0;
    mISOs = 0;
    mAINs = 0;
    mUARTs = 0;

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

int roboGeo10::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboGeo10::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
