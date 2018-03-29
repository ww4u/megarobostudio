
#include "h2.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot1.cpp"
};

robotH2::robotH2()
{
    mClass = QObject::tr("H2");
    mId = robot_sinanju;

    setAxes( 3 );
    setAxesDefName( 3 );
    setJointName( 3 );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("X")
              <<QObject::tr("Y")
              <<QObject::tr("Z");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device2"; //! x
    mAxesConnectionName[1] = "CH1@device1"; //! y
    mAxesConnectionName[2] = "CH2@device1"; //! z

    //! rotate angle
    //! 0,90,180,180
    mRotateAngles.append( 0 );
    mRotateAngles.append( 90 );
    mRotateAngles.append( 180 );
    mRotateAngles.append( 180 );

    //! arm length
    //! 247.75, 255, 250, 0, 0, 0
    mArmLengths.append( 247.75 );
    mArmLengths.append( 255 );
    mArmLengths.append( 250 );
    mArmLengths.append( 0 );

    mArmLengths.append( 0 );
    mArmLengths.append( 0 );

    //! ref point
    mRefAngles.append( 0 );
    mRefAngles.append( 90 );
    mRefAngles.append( -90 );
    mRefAngles.append( -90 );
    mRefAngles.append( 0 );
}

robotH2::~robotH2()
{
    delete_all( mJointsGroup );
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
void robotH2::onMsg( int subAxes, RoboMsg &msg )
{
    if ( !RoboMsg::checkMsg( msg, _msg_patterns, sizeof_array(_msg_patterns)) )
    {
        logDbg()<<msg.Msg()<<msg.size();
        return;
    }

    //! for some region
//    tpvRegion region = msg.at(0).value<tpvRegion>();
//    region.set( subAxes, region.page() );
//    tpvRegion region;
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

    logDbg()<<region.axes()<<region.page();
    Q_ASSERT( region.page() >= 0 && region.page() < regions() );

    fsm( region )->proc( msg.Msg(), msg );
}

int robotH2::serialIn( QXmlStreamReader &reader )
{
    int ret;
    ret = mHandActionModel.serialIn( reader );
    return ret;
}
int robotH2::serialOut( QXmlStreamWriter &writer )
{
    return mHandActionModel.serialOut( writer );
}

int robotH2::download( QList<tpvGroup*> &groups,
                             QList<int> &joints,
                             const tpvRegion &region )
{logDbg();
    Q_ASSERT( groups.size() == joints.size() );

    MegaDevice::deviceMRQ *pMrq;
    int axes;
logDbg();
    int ret;
    tpvRegion mrqRegion;
    for ( int i = 0; i < groups.size(); i++ )
    {
        pMrq = jointDevice( joints[i], &axes );

        if ( NULL == pMrq )
        { logDbg()<<mAxesConnectionName[i];return ERR_INVALID_DEVICE_NAME; }

        QList<tpvRow*> rows;
        groups[i]->getRows( rows );
logDbg();
        //! \note change to mrq ax
        mrqRegion = region;
        mrqRegion.setAx( axes );

        //! send
        ret = pMrq->pvtWrite( mrqRegion, rows );
        if ( ret != 0 )
        { return ret; }
    }
logDbg();
    return 0;
}

int robotH2::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}

int robotH2::run( const tpvRegion &region  )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_run), region );
    return 0;
}
int robotH2::stop( const tpvRegion &region  )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_stop), region );
    return 0;
}

int robotH2::setLoop( int n, const tpvRegion &region )
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
int robotH2::loopNow()
{ return 0; }

void robotH2::onLine()
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
            Q_ASSERT( NULL != mFsms[ tpvRegion(0,j) ] );
            pMrq->Fsm( tpvRegion(ax,j) )->setLeader( mFsms[ tpvRegion(0,j) ],
                                                    (void*)i );
        }

        //! use phy bus
        attachBus( pMrq->Bus() );
    }
}
void robotH2::offLine()
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

    //! \todo
//    detachBus();
}

QAbstractTableModel *robotH2::handActions()
{ return &mHandActionModel; }



