
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
    mClass = QObject::tr("IGUS-DRYLIN");
    mId = robot_igus_delta;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 3 );
    setAxesDefName( 3 );
    setJointName( 3 );

    setPoseCount( 3 );   //! x,y,z
    mPoseTitles.clear();
    mPoseTitles<<"X"<<"Y"<<"Z";

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Leg A")
              <<QObject::tr("Leg B")
              <<QObject::tr("Leg C")
              <<QObject::tr("Hand");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! angles
    mJointAngleMask[0]=true;
    mJointAngleMask[1]=true;
    mJointAngleMask[2]=true;

    setAngleType( robo_angle_inc );

    //! angle dir
    mAngleDir.clear();
    mAngleDir<<true<<true<<true;

    mJointCcwMask.clear();
    mJointCcwMask<<true<<true<<true;

    mJointZeroCcw.clear();
    mJointZeroCcw<<false<<false<<false;

    //! init angle
    mInitAngles.clear();

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

    mInvert = false;
    mAngleToDist = 146/740.0;       //! 146mm/740deg
                                    //! 0.197297

    //! connection name
    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! LS
    mAxesConnectionName[1] = "CH2@device1"; //! RS
    mAxesConnectionName[2] = "CH3@device1"; //! Plate
//    mAxesConnectionName[3] = "CH4@device1"; //! Hand

    mZeroTime = 5;
    mZeroSpeed = 50;
    mZeroAngle = 100;

    mLimitOpt = 0;

    mZeroTmo = 60;

    mInitLeg = 5;
    mInitH = 0.5;
    mInitT = 1;

    m_pRoboTask = new IgusDeltaTask();
    Q_ASSERT( NULL != m_pRoboTask );
}


