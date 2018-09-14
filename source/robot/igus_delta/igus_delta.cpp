
#include "igus_delta.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotIgusDelta::robotIgusDelta()
{
    mClass = QObject::tr("igus-drylin");
    mId = robot_igus_delta;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 4 );
    setAxesDefName( 4 );
    setJointName( 4 );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Leg A")
              <<QObject::tr("Leg B")
              <<QObject::tr("Leg C")
              <<QObject::tr("Hand");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! angles
    mJointAngleMask[0]=false;
    mJointAngleMask[1]=false;
    mJointAngleMask[2]=false;
    mJointAngleMask[3]=false;

    //! angle dir
    mAngleDir.clear();
    mAngleDir<<true<<true<<true<<true;

    mJointCcwMask.clear();
    mJointCcwMask<<true<<true<<true<<true;

    mJointZeroCcw.clear();
    mJointZeroCcw<<false<<true<<true<<false;

    //! init angle
    mInitAngles.clear();
//    mInitAngles<<0<<0;

    mbInterpAble = true;

    //! length
    mArmLengths.append( 679.2 );
    mArmLengths.append( 400 );
    mArmLengths.append( 72.75 );
    mArmLengths.append( MATH_PI/4 );

    //!
    mOffset.append( 0 );
    mOffset.append( 239.5 );

    mP0<<0<<0<<526.168;
    mPosLim<<239.5<<385.5;
    mScal = 70;
    mVm = 2*360*2;

    //! connection name
    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH2@device1"; //! LS
    mAxesConnectionName[1] = "CH1@device1"; //! RS
    mAxesConnectionName[2] = "CH3@device1"; //! Plate
    mAxesConnectionName[3] = "CH4@device1"; //! Hand

    mZeroTime = 5;
    mZeroSpeed = 5;
    mZeroAngle = 100;

    mInitL = 23;
    mInitR = 21;
    mInitY = 5;
    mInitH = 0.5;
    mInitT = 1;

    m_pRoboTask = new IgusDeltaTask();
    Q_ASSERT( NULL != m_pRoboTask );
}


