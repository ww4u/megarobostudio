
#include "h2m.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _detail[]=
{
    #include "./detail.cpp"
};


robotH2M::robotH2M()
{
    mClass = QObject::tr("MRX-H2M");
    mId = robot_h2_m;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    mArmLengths.clear();
    mArmLengths<<580<<494<<38<<52;

    //! mask
    mJointCcwMask.clear();
    mJointCcwMask<<false<<false;

    mJointZeroCcw.clear();
    mJointZeroCcw<<false<<false;

    mAngleDir.clear();
    mAngleDir<<true<<true;

    mAxesDirs.clear();
    mAxesDirs<<1<<-1;


}







