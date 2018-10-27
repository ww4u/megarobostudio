
#include "injectpump.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotInject::robotInject()
{
    mClass = QObject::tr("MRX-INJ2");
    mId = robot_ip;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 2 );
    setAxesDefName( 2 );
    setJointName( 2 );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Valve")
              <<QObject::tr("Injection");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
    setJointPanelAble( true );
    setRoboPanelAble( false );
    setMrpAble( true );

    mArmLengths.clear();

    mJointCcwMask.clear();                  //! visible
    mJointCcwMask<<true<<true;

    mJointZeroCcw.clear();
    mJointZeroCcw<<true<<true;

    mAngleDir.clear();
    mAngleDir<<true<<true;                 //! x:  y:

    mSuctionDirs.clear();
    mSuctionDirs<<-1<<1;

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! x
    mAxesConnectionName[1] = "CH2@device1"; //! y

    mZeroTime = 1;
    mZeroDistance = 5;
    mZeroSpeed = 5;

    mVGapTime = 1;
    mVGapDistance = 10;                     //! dist
    mIGapTime = 1;
    mIGapDistance = 5;

    m_pRoboTask = new IJTask();
    Q_ASSERT( NULL != m_pRoboTask );

}

robotInject::~robotInject()
{

}

int robotInject::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}





