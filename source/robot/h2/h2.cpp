
#include "h2.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot1.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotH2::robotH2()
{
    mClass = QObject::tr("H2");
    mId = robot_h2;

    setAxes( 3 );
    setAxesDefName( 3 );
    setJointName( 3 );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("X")
              <<QObject::tr("Y")
              <<QObject::tr("Z");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! x
    mAxesConnectionName[1] = "CH2@device1"; //! y
    mAxesConnectionName[2] = "CH3@device1"; //! z
}

robotH2::~robotH2()
{
    delete_all( mJointsGroup );
}

int robotH2::serialIn( QXmlStreamReader &reader )
{
    int ret = 0;
//    ret = mHandActionModel.serialIn( reader );
    return ret;
}
int robotH2::serialOut( QXmlStreamWriter &writer )
{
//    return mHandActionModel.serialOut( writer );
    return 0;
}

int robotH2::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}





