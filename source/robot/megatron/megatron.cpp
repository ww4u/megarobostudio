
#include "megatron.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotMegatron::robotMegatron()
{
    mClass = QObject::tr("MRX-AS");
    mId = robot_megatron;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_id_from;

    setAxes( 6 );
    setAxesDefName( 6 );
    setJointName( 6 );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("FX")   //! X1
              <<QObject::tr("FZ")   //! Z1
              <<QObject::tr("BX")   //! X2
              <<QObject::tr("BZ")   //! Z2
              <<QObject::tr("LY")   //! Y1
              <<QObject::tr("RY");  //! Y2

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    mJointZeroCcw.clear();
    mJointZeroCcw<<true<<true<<true<<true<<true<<true;

    //! debug used
    //! alter the axes name
    Q_ASSERT( mAxesConnectionName.size() == 6 );
    mAxesConnectionName[0] = "CH1@device1"; //! fx
    mAxesConnectionName[1] = "CH2@device1"; //! fz
    mAxesConnectionName[2] = "CH1@device2"; //! bx
    mAxesConnectionName[3] = "CH4@device2"; //! bz
    mAxesConnectionName[4] = "CH3@device1"; //! ly
    mAxesConnectionName[5] = "CH4@device1"; //! ry


    m_pRoboTask = new MegatronTask();
    Q_ASSERT( NULL != m_pRoboTask );

    mZeroTime = 5;
    mZeroSpeed = 5;
    mZeroAngle = 100;

    mGapTime = 1;
    mGapDistance = 2;
    mGapSpeed = 1;
}

robotMegatron::~robotMegatron()
{

}

static msg_type _msg_patterns[]={

    //! member_state
    { e_robot_member_state,
      { QMetaType::Int, TPV_REGEION_TYPE_ID, QMetaType::Int },   //! subax, region, ax
    },

    //! msg
    { MegaDevice::mrq_msg_run,
      { TPV_REGEION_TYPE_ID },  //! region
    },

    { MegaDevice::mrq_msg_stop,
      { TPV_REGEION_TYPE_ID },  //! region
    },

    { MegaDevice::mrq_msg_call,
      { TPV_REGEION_TYPE_ID },  //! region
    },
};

//! \note subax invalid
void robotMegatron::onMsg( int subAxes, RoboMsg &msg )
{
    if ( !RoboMsg::checkMsg( msg, _msg_patterns, sizeof_array(_msg_patterns)) )
    {
        logDbg()<<msg.Msg()<<msg.size();
        return;
    }

    tpvRegion region;
    if ( msg.getMsg() == e_robot_member_state )     //! subax, region, ax
    {
        int tabAx = msg.at(0).toInt();
        region = msg.at(1).value<tpvRegion>();
        Q_ASSERT( tabAx >= 0 && tabAx < axes() );
        region.setAx( 0 );
    }
    else
    {
        region = msg.at(0).value<tpvRegion>();
    }

    fsm( region )->proc( msg.Msg(), msg );
}

int robotMegatron::download( QList<tpvGroup*> &groups,
                             QList<int> &joints,
                             const tpvRegion &region )
{logDbg();
    Q_ASSERT( groups.size() == joints.size() );

    MegaDevice::deviceMRQ *pMrq;
    int axes;
logDbg();
    int ret;
    tpvRegion localRegion = region;
    for ( int i = 0; i < groups.size(); i++ )
    {
        pMrq = jointDevice( joints[i], &axes );

        if ( NULL == pMrq )
        { logDbg()<<mAxesConnectionName[i];return ERR_INVALID_DEVICE_NAME; }

        QList<tpvRow*> rows;
        groups[i]->getRows( rows );
logDbg();
        localRegion.setAx( axes );

        ret = pMrq->pvtWrite( localRegion, rows );
        if ( ret != 0 )
        { return ret; }
    }
logDbg();
    return 0;
}

int robotMegatron::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}

int robotMegatron::run( const tpvRegion &region  )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_run), region );
    return 0;
}
int robotMegatron::stop( const tpvRegion &region  )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_stop), region );
    return 0;
}

int robotMegatron::loopNow()
{ return 0; }





