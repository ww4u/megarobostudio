#include "robogeo2.h"
#include "../../device/mrq/deviceMRQ.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

roboGeo2::roboGeo2()
{
    mClass = "MRQ-M2302";
    mId = robot_geogoog_2;
    setAxes( 2 );
    setAxesDefName( 2 );
    setJointName( 2 );

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

    mMicrostepBase = 0;     //! 256
    mbEncoderAble = true;
    mDriverId = 0;

    setTunningAble( true );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

QString roboGeo2::trigSrcAlias( int ax, int iTrig )
{
    return QString("POS%1").arg( ax * 1 + iTrig + 1 );
}

int roboGeo2::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboGeo2::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
