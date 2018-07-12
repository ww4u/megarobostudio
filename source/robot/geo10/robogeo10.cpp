#include "robogeo10.h"
#include "../../device/mrq/deviceMRQ.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

roboGeo10::roboGeo10()
{
    mClass = "MRQ-M1710";
    mId = robot_geogoog_10;
    setAxes( 10 );
    setAxesDefName( 10 );
    setJointName( 10 );

    mOutputs = 1;
    mInputs = 1;

    mDOs = 4;
    mDIs = 0;
    mISOs = 2;
    mISIs = 1;
    mAINs = 0;
    setUarts( 1 );
    setUartSensors( 4 );

    setTrigSrcs( 1 );

    mMicrostepBase = 3;     //! 32
    mbEncoderAble = false;
    mDriverId = 1;

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

QString roboGeo10::trigSrcAlias( int ax, int iTrig )
{
    return QString("POS%1").arg( ax * 1 + iTrig + 1 );
}

int roboGeo10::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboGeo10::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
