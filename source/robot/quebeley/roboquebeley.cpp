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

    mOutputs = 1;
    mInputs = 1;

    mDOs = 2;
    mDIs = 2;

    mISOs = 2;
    mISIs = 0;

    mAINs = 1;

    mUARTs = 2;
    mUART_Sensors = 1;      //! \todo 1

    mSensorNameList.clear();
    mSensorNameList<<QObject::tr("SEN1")<<QObject::tr("SEN2");

    mTemperatures = 1;      //! otp

    setTrigSrcs( 5 );

    if ( id == robot_qubeley_d )
    { mImage = QImage::fromData( _megaimage, sizeof(_megaimage) ); }
    else if ( id == robot_qubeley_s )
    { mImage = QImage::fromData( _megaimage, sizeof(_megaimage_s) ); }
    else
    { Q_ASSERT(false); }
}

QString roboQuebeley::trigSrcAlias( int ax, int iTrig )
{
    Q_ASSERT( ax == 0 && iTrig >= 0 && iTrig < trigSrcs() );

    if ( iTrig >= 0 && iTrig < 4 )
    {
        return QString("X%1").arg(iTrig+1);
    }
    else if ( iTrig == 4 )
    { return QString(QObject::tr("SW_Z")); }
    else
    { return ""; }
}

int roboQuebeley::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboQuebeley::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
