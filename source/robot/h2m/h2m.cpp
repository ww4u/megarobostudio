
#include "h2m.h"

#include "../../device/board/_MRQ_enum.h"

robotH2M::robotH2M()
{
    mClass = QObject::tr("MRX-H2M");
    mId = robot_h2_m;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

//    setAxes( 2 );
//    setAxesDefName( 2 );
//    setJointName( 2 );

//    setPoseCount( 2 );
//    mPoseTitles.clear();
//    mPoseTitles<<"X"<<"Y";

//    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
//    mJointName.clear();
//    mJointName<<QObject::tr("Left")
//              <<QObject::tr("Right");

//    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    mArmLengths.clear();
//    mArmLengths<<13.4<<13.4<<802<<494<<52<<38;
    mArmLengths<<13.4<<13.4<<580<<494<<52<<38;

    //! mask
    mJointCcwMask.clear();
    mJointCcwMask<<false<<false;

    mJointZeroCcw.clear();
    mJointZeroCcw<<true<<true;

//    mZeroX = 32.7;
//    mZeroY = 0;

    mAngleDir.clear();
    mAngleDir<<false<<true;                 //! x:  y:

    mAxesDirs.clear();
    mAxesDirs<<-1<<-1;
}







