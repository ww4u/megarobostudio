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

    setTunningAble( true );

    mOutputs = 1;
    mInputs = 1;

    mDIs = 0;
    mDOs = 1;

    mISIs = 0;
    mISOs = 1;

    mUARTs = 1;
    mUART_Sensors = 1;      //! \todo 2

    mTemperatures = 0;      //! otp
    mAINs = 0;

    mSensorNameList.clear();
    mSensorNameList<<QObject::tr("SEN1")<<QObject::tr("SEN2");

    mbRunWaveAble = true;

    if ( id == robot_qubeley_d )
    { mImage = QImage::fromData( _megaimage, sizeof(_megaimage) ); }
    else if ( id == robot_qubeley_s )
    { mImage = QImage::fromData( _megaimage, sizeof(_megaimage_s) ); }
    else
    { Q_ASSERT(false); }
}

void roboQuebeley::postCtor()
{
    deviceMRQ::postCtor();

    if ( mId==robot_qubeley_d ) //! 1.0
    {
        mOutputs = 1;
        mInputs = 0;

        mDIs = 0;
        mDOs = 2;

        mISIs = 0;
        mISOs = 2;

        mUARTs = 1;
        mUART_Sensors = 2;

        mTemperatures = 1;      //! otp
        mAINs = 1;

        setTrigSrcs( 4 + 1 );   //! encoder z

        mSensorNameList.clear();
        mSensorNameList<<QObject::tr("SEN1")<<QObject::tr("SEN2");
    }
    else
    {
        mOutputs = 1;
        mInputs = 0;

        mDIs = 0;
        mDOs = 1;

        mISIs = 0;
        mISOs = 1;

        mUARTs = 2;
        mUART_Sensors = 1;      //! \todo 2

        mTemperatures = 0;
        mAINs = 0;

        setTrigSrcs( 2 + 1 );
        mTrigIdsList[2] = MRQ_TRIGGER_LEVELSTATE_TRIG5;

        mSensorNameList.clear();
        mSensorNameList<<QObject::tr("SEN1")<<QObject::tr("SEN2");
    }
}

QString roboQuebeley::trigSrcAlias( int ax, int iTrig )
{
    Q_ASSERT( ax == 0 && iTrig >= 0 && iTrig < trigSrcs() );

    //! 5
    if ( mId==robot_qubeley_s )
    {
        if ( iTrig >= 0 && iTrig < 4 )
        {
            return QString("X%1").arg(iTrig+1);
        }
        else if ( iTrig == 4 )
        { return QString(QObject::tr("SW_Z")); }
        else
        { return ""; }
    }
    //! 3
    else
    {
        if ( iTrig >= 0 && iTrig < 2 )
        {
            return QString("X%1").arg(iTrig+1);
        }
        else if ( iTrig == 2 )
        { return QString(QObject::tr("SW_Z")); }
        else
        { return ""; }
    }
}

int roboQuebeley::serialIn( QXmlStreamReader &reader )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialIn( reader );
}
int roboQuebeley::serialOut( QXmlStreamWriter &writer )
{
    return MegaDevice::deviceMRQ::MRQ_model::serialOut( writer );
}
