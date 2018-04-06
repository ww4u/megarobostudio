
#include "delta.h"

static quint8 _megaimage[]=
{
    #include "./robot1.cpp"
};

robotDelta::robotDelta()
{
    mClass = QObject::tr("Delta");
    mId = robot_delta;

    //! default gpid
    mCanGroupId = group_id_from + mId - robot_complex;

    setAxes( 4 );
    setAxesDefName( 4 );
    setJointName( 4 );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Left shulder")     //! 1
              <<QObject::tr("Right shulder")    //! 0
              <<QObject::tr("Plate")            //! 2
              <<QObject::tr("Hand");            //! 3

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! angles
    mJointAngleMask[0]=false;
    mJointAngleMask[1]=false;
    mJointAngleMask[2]=false;
    mJointAngleMask[3]=false;

    //! angle dir
    mAngleDir.clear();
    mAngleDir<<false<<false<<false<<false;

    //! init angle
    mInitAngles.clear();
//    mInitAngles<<0<<0;

    //! length
    mArmLengths.append( 132 );
    mArmLengths.append( 262.5 );
    mArmLengths.append( 100 );
    mArmLengths.append( 10 );
    mArmLengths.append( -10 );

    mAngleLimit.clear();
    mAngleLimit<<0<<2*MATH_PI<<0<<2*MATH_PI<<0<<2*MATH_PI;

    mP0.clear();
    mP0<<0<<-218.8<<0;

    mA0.clear();
    mA0<<0<<0<<0.5;

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
}


