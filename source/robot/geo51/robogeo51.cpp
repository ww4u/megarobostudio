#include "robogeo51.h"
#include "../../device/mrq/deviceMRQ.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

roboGeo51::roboGeo51()
{
    mClass = "Geogoog5_1";
    mId = robot_geogoog_5_1;
    setAxes( 5 );
    setAxesDefName( 5 );
    setJointName( 5 );

//    setDcAxes( 1 );

    mOutputs = 0;
    mInputs = 0;

    mDOs = 0;
    mDIs = 0;
    mISOs = 0;
    mAINs = 0;

    mMicrostepBase = 2;

    setUarts( 2 );
    setUartSensors( 4 );

    setAlarms( 1 );
    setAbsEncAlarms( 4 );
    setDistanceAlarms( 4 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

int roboGeo51::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboGeo51::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
