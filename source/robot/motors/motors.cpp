
#include "motors.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotMotors::robotMotors()
{
    mClass = "MRX-MOTORS";
    mId = robot_motors;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    mJointName.clear();
    mJointName<<QObject::tr("Motor");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Motor");
}

