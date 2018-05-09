#include "roboquebeley.h"
#include "../../device/mrq/deviceMRQ.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _megaimage_s[]=
{
    #include "./robot_s.cpp"
};

roboQuebeley::roboQuebeley( robotEnum id )
{
    if ( id == robot_qubeley_d )
    { mClass = "MRQ-C-23-D"; }
    else if ( id == robot_qubeley_s )
    { mClass = "MRQ-C-23-S"; }
    else
    { Q_ASSERT(false); }
    mId = id;
    setAxes( 1 );
    setAxesDefName( 1 );
    setJointName( 1 );

    mDOs = 0;
    mDIs = 0;
    mISOs = 0;
    mAINs = 0;
    mUARTs = 0;

    if ( id == robot_qubeley_d )
    { mImage = QImage::fromData( _megaimage, sizeof(_megaimage) ); }
    else if ( id == robot_qubeley_s )
    { mImage = QImage::fromData( _megaimage, sizeof(_megaimage_s) ); }
    else
    { Q_ASSERT(false); }
}

int roboQuebeley::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboQuebeley::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
