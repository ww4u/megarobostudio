
#include "megatron.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot1.cpp"
};

robotMegatron::robotMegatron()
{
    mClass = "Megatron";
    mId = robot_megatron;
    setAxes( 5 );
    setAxesDefName( 5 );
    setJointName( 5 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! fsm
    mFsm.attachRobot( this );
    mFsm.build();

    //! debug used
    //! alter the axes name
    mAxesConnectionName[4] = "CH1@device2";

    mRefAngles.append( 0 );
    mRefAngles.append( 90 );
    mRefAngles.append( -90 );
    mRefAngles.append( -0 );
    mRefAngles.append( 0 );
}

static msg_type _msg_patterns[]={

    //! member_state
    { e_robot_member_state,
      { QMetaType::Int, QMetaType::Int },   //! subax, stat
    },

    //! msg
    { MegaDevice::mrq_msg_run,
      { QMetaType::Int, QMetaType::Int, },  //! subax, stat
    },

    { MegaDevice::mrq_msg_stop,
      { QMetaType::Int, QMetaType::Int, },  //! subax, stat
    },

};

int robotMegatron::serialIn( QXmlStreamReader &reader )
{
    int ret;
    ret = mHandActionModel.serialIn( reader );
    return ret;
}
int robotMegatron::serialOut( QXmlStreamWriter &writer )
{
    return mHandActionModel.serialOut( writer );
}

void robotMegatron::onMsg( int subAxes, RoboMsg &msg )
{
    if ( !RoboMsg::checkMsg( msg, _msg_patterns, sizeof_array(_msg_patterns)) )
    {
        logDbg()<<msg.Msg()<<msg.size();
        return;
    }

    mFsm.proc( msg.Msg(), msg.at(0).toInt(), msg.at(1).toInt() );
}

//! for some axes
int robotMegatron::download( tpvGroup *pGroup, int axes )
{
    Q_ASSERT( NULL != pGroup );

    return 0;
}

int robotMegatron::download( motionGroup *pGroup, int axes )
{
    Q_ASSERT( NULL != pGroup );

    return 0;
}

int robotMegatron::download( QList<tpvGroup*> &groups,
                             QList<int> &joints )
{logDbg();
    Q_ASSERT( groups.size() == joints.size() );

    MegaDevice::deviceMRQ *pMrq;
    int axes;
logDbg();
    int ret;
    for ( int i = 0; i < groups.size(); i++ )
    {
        pMrq = jointDevice( joints[i], &axes );

        if ( NULL == pMrq )
        { logDbg()<<mAxesConnectionName[i];return ERR_INVALID_DEVICE_NAME; }

        QList<tpvRow*> rows;
        groups[i]->getRows( rows );
logDbg();
        ret = pMrq->pvtWrite( axes, rows );
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

int robotMegatron::run( int axes )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_run), -1, 0 );

    return 0;
}

int robotMegatron::stop( int axes )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_stop), -1, 0 );

    return 0;
}

int robotMegatron::run( )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_run), -1, 0 );

    return 0;
}

int robotMegatron::stop( )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_stop), -1, 0 );

    return 0;
}

void robotMegatron::onLine()
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;

    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );

        Q_ASSERT( NULL != pMrq );

        pMrq->Fsm( ax )->setLeader( &mFsm, (void*)i );

        //! use phy bus
        attachBus( pMrq->Bus() );
    }
}
void robotMegatron::offLine()
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;

    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );

        Q_ASSERT( NULL != pMrq );

        pMrq->Fsm( ax )->setLeader( NULL, NULL );
    }

    detachBus();
}

QAbstractTableModel *robotMegatron::handActions()
{ return &mHandActionModel; }



