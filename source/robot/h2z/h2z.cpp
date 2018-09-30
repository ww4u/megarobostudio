
#include "h2z.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotH2Z::robotH2Z()
{
    mClass = QObject::tr("MRX-H2Z");
    mId = robot_h2z;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 3 );
    setAxesDefName( 3 );
    setJointName( 3 );

    setPoseCount( 2 );
    mPoseTitles.clear();
    mPoseTitles<<"X"<<"Y";

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Left")
              <<QObject::tr("Right")
              <<QObject::tr("Z");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    mArmLengths.clear();
    mArmLengths<<13.4<<13.4<<580<<494<<52<<38;

//    mJointCcwMask[0] = false;
//    mJointCcwMask[1] = false;

    mJointAngleMask.clear();
    mJointAngleMask<<true<<true<<false;

    mJointCcwMask[0] = true;
    mJointCcwMask[1] = true;
    mJointCcwMask[2] = true;

    mJointZeroCcw.clear();
    mJointZeroCcw<<false<<false<<false;

    mZeroX = 32.7;
    mZeroY = 0;

    mAngleDir.clear();
    mAngleDir<<true<<true<<true;

    mAxesDirs.clear();
    mAxesDirs<<-1<<-1<<1;

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! x
    mAxesConnectionName[1] = "CH2@device1"; //! y
    mAxesConnectionName[2] = "CH3@device1"; //! z

    mZeroTime = 1;
    mZeroDistance = 50;
    mZeroSpeed = 5;

    mGapTime = 1;
    mGapDistance = 10;      //! dist

    if ( NULL != m_pRoboTask )
    { delete m_pRoboTask; }
    m_pRoboTask = new H2ZTask();
    Q_ASSERT( NULL != m_pRoboTask );

    mLines = 1000;          //! for encoder
    mEncoderDirs.clear();
    mEncoderDirs<<1<<1<<1;
}

robotH2Z::~robotH2Z()
{

}

void robotH2Z::setJointZeroCcw( int jId, bool b )
{
    robotH2::setJointZeroCcw( jId, b );

    Q_ASSERT( jId >= 0 && jId < mAngleDir.size() );
    mAngleDir[ jId ] = !b;
}





