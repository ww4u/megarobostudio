
#include "megatron.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot1.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotMegatron::robotMegatron()
{
    mClass = QObject::tr("Megatron");
    mId = robot_megatron;

    setAxes( 6 );
    setAxesDefName( 6 );
    setJointName( 6 );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("FX")
              <<QObject::tr("FZ")
              <<QObject::tr("BX")
              <<QObject::tr("BZ")
              <<QObject::tr("LY")
              <<QObject::tr("RY");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! debug used
    //! alter the axes name
    Q_ASSERT( mAxesConnectionName.size() == 6 );
    mAxesConnectionName[0] = "CH1@device1"; //! fx
    mAxesConnectionName[1] = "CH2@device1"; //! fz
    mAxesConnectionName[2] = "CH1@device2"; //! bx
    mAxesConnectionName[3] = "CH4@device2"; //! bz
    mAxesConnectionName[4] = "CH3@device1"; //! ly
    mAxesConnectionName[5] = "CH4@device1"; //! ry

    mZeroTime = 5;
    mZeroSpeed = 5;
    mZeroAngle = 100;
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

//    ! for some region
//    tpvRegion region = msg.at(0).value<tpvRegion>();

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

int robotMegatron::setLoop( int n, const tpvRegion &region )
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;
    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );

        Q_ASSERT( NULL != pMrq );

        pMrq->setMOTIONPLAN_CYCLENUM( ax,
                                      (MRQ_MOTION_SWITCH_1)region.page(),
                                      n );
    }

    return 0;
}
int robotMegatron::loopNow()
{ return 0; }

void robotMegatron::onLine()
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;

    //! each ax
    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );

        Q_ASSERT( NULL != pMrq );

        Q_ASSERT( regions() == pMrq->regions() );

        //! each region
        for ( int j = 0; j < regions(); j++ )
        {
            Q_ASSERT( pMrq->Fsm( tpvRegion(ax,j) ) != NULL );
            pMrq->Fsm( tpvRegion(ax,j) )->setLeader( mFsms[ tpvRegion(0,j) ],
                                                    (void*)i );
        }

        //! use phy bus
        attachBus( pMrq->Bus() );
    }
}
void robotMegatron::offLine()
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;

    //! each ax
    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );

        //! each region
        for ( int j = 0; j < regions(); j++ )
        {
            Q_ASSERT( pMrq->Fsm( tpvRegion(ax,j) ) != NULL );
            pMrq->Fsm( tpvRegion(ax,j) )->setLeader( NULL, NULL );
        }
    }

    detachBus();
}




