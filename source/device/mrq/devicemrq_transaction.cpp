#include "deviceMRQ.h"

namespace MegaDevice
{


//! msg patterns
static msg_type _msg_patterns[] =
{
    //! sys msg
//    { e_robot_init, },
    { e_robot_timeout, {TPV_REGEION_TYPE_ID, QMetaType::Int} },

    //! mrq msg
    //! msg, region
    { mrq_msg_run, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_stop, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_rst, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_program, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_prepare, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_call, { TPV_REGEION_TYPE_ID },
    },

    { mrq_msg_idle, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_calcing, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_calcend, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_standby, { TPV_REGEION_TYPE_ID },
    },

    { mrq_msg_running, { TPV_REGEION_TYPE_ID },
    },
    { mrq_msg_error, { TPV_REGEION_TYPE_ID },
    },
};

//! called from the worker thread
void deviceMRQ::onMsg( int subAxes, RoboMsg &detail )
{
    Q_ASSERT( subAxes >= 0 && subAxes < axes() );

    //! type match
    if ( !RoboMsg::checkMsg( detail, _msg_patterns, sizeof_array(_msg_patterns)) )
    {
        logDbg()<<(int)detail.at(0).type()<<detail.getMsg()<<detail.size();
        return;
    }

    //! proc msg
    int varMsg = detail.getMsg();

    //! fsm proc
    if ( subAxes >= 0 && subAxes < axes() )
    {
        //! get region
        tpvRegion region;
        region = detail.at(0).value<tpvRegion>();

        Q_ASSERT( mMrqFsms.contains(region) );
logDbg()<<varMsg;
        mMrqFsms[ region ]->proc( varMsg, detail );
logDbg()<<varMsg;
    }
    else
    { Q_ASSERT(false); }
}

//! called from the SysTimerThread
void deviceMRQ::onTimer( void *pContext, int id )
{
    Q_ASSERT( NULL != pContext );

    //! context is fsm
    MrqFsm *pFsm = (MrqFsm*)pContext;
    lpc( pFsm->axes() )->postMsg( e_robot_timeout,
//                                  tpvRegion( pFsm->axes(), pFsm->page() ),
                                  *pFsm,        //! \todo why?
                                  id );
}

void deviceMRQ::attachCondition( const tpvRegion &region,
                                 RoboCondition *pCond )
{
    Fsm( region )->attachCondition( pCond );
}

bool deviceMRQ::waitCondition( const tpvRegion &region,
                               RoboCondition *pCond,
                               int tmoms )
{
    Q_ASSERT( mMrqFsms.contains( region ) );

    return Fsm( region )->waitCondition( pCond, tmoms );
}

RoboFsm *deviceMRQ::Fsm( const tpvRegion &region )
{
    Q_ASSERT( mMrqFsms.contains( region ) );

    return mMrqFsms[ region ];
}

deviceProxyMotor *deviceMRQ::Motor( const tpvRegion &region )
{
    Q_ASSERT( mProxyMotors.contains(region) );

    return mProxyMotors[ region ];
}

tpvDownloader *deviceMRQ::downloader( const tpvRegion &region )
{
    Q_ASSERT( mDownloaders.contains( region) );

    Q_ASSERT( mDownloaders[region] != NULL );

    return mDownloaders[region];
}

}
