
#include "delta.h"

static quint8 _megaimage[]=
{
    #include "./robot1.cpp"
};

robotDelta::robotDelta()
{
    mClass = QObject::tr("Delta");
    mId = robot_delta;

    setAxes( 4 );
    setAxesDefName( 4 );
    setJointName( 4 );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Left shulder")
              <<QObject::tr("Right shulder")
              <<QObject::tr("Hand")
              <<QObject::tr("Plate");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! length
    mArmLengths.append( 100 );
    mArmLengths.append( 100 );
    mArmLengths.append( 100 );
    mArmLengths.append( 100 );

    //! connection name
    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device2"; //! LS
    mAxesConnectionName[1] = "CH1@device1"; //! RS
    mAxesConnectionName[2] = "CH2@device1"; //! Hand
    mAxesConnectionName[3] = "CH3@device1"; //! Plate
}




