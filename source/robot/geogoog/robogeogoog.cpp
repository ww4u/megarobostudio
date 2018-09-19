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

    mOutputs = 1;
    mInputs = 1;

    mDOs = 4;
    mDIs = 0;
    mISOs = 2;
    mISIs = 1;
    mAINs = 0;
    setUarts( 1 );
    setUartSensors( 4 );

    mMicrostepBase = 2;     //! 64

    //! \note for 21
    setTunningAble( false );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

QString roboGeogoog::trigSrcAlias( int ax, int iTrig )
{
    return QString("POS%1").arg( ax * 4 + iTrig + 1 );
}

int roboGeogoog::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboGeogoog::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
