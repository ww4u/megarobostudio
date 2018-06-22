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

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

//int roboQuebeley::serialIn( QXmlStreamReader &reader )
//{
//    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
//}
//int roboQuebeley::serialOut( QXmlStreamWriter &writer )
//{
//    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
//}
