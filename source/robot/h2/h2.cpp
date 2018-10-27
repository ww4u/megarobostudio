
#include "h2.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotH2::robotH2()
{
    mClass = QObject::tr("MRX-H2");
    mId = robot_h2;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 2 );
    setAxesDefName( 2 );
    setJointName( 2 );

    setPoseCount( 2 );
    mPoseTitles.clear();
    mPoseTitles<<"X"<<"Y";

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Left")
              <<QObject::tr("Right");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
    setJointPanelAble( true );
    setRoboPanelAble( true );
    setMrpAble( true );

    mArmLengths.clear();
    mArmLengths<<802<<494<<38<<52;
//    mArmLengths<<13.4<<13.4<<802<<494<<52<<38;

    mToothType = 2;
    mToothGear = 30;

    mPDirIndex = 1;
    mMotionIndex = 1;

    //! mask
    mJointCcwMask.clear();
    mJointCcwMask<<false<<false;

    mJointZeroCcw.clear();
    mJointZeroCcw<<false<<false;

    mJointAngleMask.clear();
    mJointAngleMask<<true<<true;
    setAngleType( robo_angle_inc );

    mZeroX = 32.7;
    mZeroY = 0;

    mAngleDir.clear();
    mAngleDir<<true<<true;

    mAxesDirs.clear();
    mAxesDirs<<1<<-1;

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! Left
    mAxesConnectionName[1] = "CH2@device1"; //! Right

    mZeroTime = 1;
    mZeroDistance = 10;
    mZeroSpeed = 5;

    mGapTime = 1;
    mGapDistance = 10;        //! dist

    mGapZTime = 1;
    mGapZDistance = 10;

    m_pRoboTask = new H2Task();
    Q_ASSERT( NULL != m_pRoboTask );

    mLines = 1000;
    mEncoderDirs<<1<<1;

//    mbTransferAble = false;
    mTransferR[0*2+0] = 1.0;
    mTransferR[0*2+1] = 0.0;

    mTransferR[1*2+0] = 0.0;
    mTransferR[1*2+1] = 1.0;

    mTransferS[0*1+0] = 0;
    mTransferS[1*1+0] = 0;

    mTransferRInv[0*2+0] = 1.0;
    mTransferRInv[0*2+1] = 0.0;

    mTransferRInv[1*2+0] = 0.0;
    mTransferRInv[1*2+1] = 1.0;

    mZeroCorner = corner_lb;
    mZeroMovement = movement_x_y;
}

robotH2::~robotH2()
{

}

void robotH2::postload()
{
    postConfigTransfer();
}

void robotH2::postConfigTransfer()
{
    //! config transfer
    if ( mZeroCorner == corner_lb )
    {
        mTransferR[0*2+0] = 1.0;
        mTransferR[0*2+1] = 0.0;

        mTransferR[1*2+0] = 0.0;
        mTransferR[1*2+1] = 1.0;

        mTransferS[0*1+0] = 0;
        mTransferS[1*1+0] = 0;

        mTransferRInv[0*2+0] = 1.0;
        mTransferRInv[0*2+1] = 0.0;

        mTransferRInv[1*2+0] = 0.0;
        mTransferRInv[1*2+1] = 1.0;
    }
    else if ( mZeroCorner == corner_lt )
    {
        mTransferR[0*2+0] = -1.0;
        mTransferR[0*2+1] = 0.0;

        mTransferR[1*2+0] = 0.0;
        mTransferR[1*2+1] = 1.0;

        mTransferS[0*1+0] = mArmLengths.at(1);
        mTransferS[1*1+0] = 0;

        mTransferRInv[0*2+0] = -1.0;
        mTransferRInv[0*2+1] = 0.0;

        mTransferRInv[1*2+0] = 0.0;
        mTransferRInv[1*2+1] = 1.0;
    }
    else if ( mZeroCorner == corner_rt )
    {
        mTransferR[0*2+0] = -1.0;
        mTransferR[0*2+1] = 0.0;

        mTransferR[1*2+0] = 0.0;
        mTransferR[1*2+1] = -1.0;

        mTransferS[0*1+0] = mArmLengths.at(1);
        mTransferS[1*1+0] = mArmLengths.at(0);

        mTransferRInv[0*2+0] = -1.0;
        mTransferRInv[0*2+1] = 0.0;

        mTransferRInv[1*2+0] = 0.0;
        mTransferRInv[1*2+1] = -1.0;
    }
    else if ( mZeroCorner == corner_rb )
    {
        mTransferR[0*2+0] = 1.0;
        mTransferR[0*2+1] = 0.0;

        mTransferR[1*2+0] = 0.0;
        mTransferR[1*2+1] = -1.0;

        mTransferS[0*1+0] = 0;
        mTransferS[1*1+0] = mArmLengths.at(0);

        mTransferRInv[0*2+0] = 1.0;
        mTransferRInv[0*2+1] = 0.0;

        mTransferRInv[1*2+0] = 0.0;
        mTransferRInv[1*2+1] = -1.0;
    }
    else
    { Q_ASSERT(false); }
}

int robotH2::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}





