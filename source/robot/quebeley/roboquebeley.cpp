#include "roboquebeley.h"
#include "../../device/mrq/deviceMRQ.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

roboQuebeley::roboQuebeley()
{
    mClass = "Quebeley";
    mId = robot_qubeley;
    setAxes( 1 );
    setAxesDefName( 1 );
    setJointName( 1 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

int roboQuebeley::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboQuebeley::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
