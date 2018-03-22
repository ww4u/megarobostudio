#include "deviceMRQ.h"

namespace MegaDevice
{

//! msg patterns
static msg_type _msg_patterns[]
{
    //! sys msg
    { e_robot_timeout, {QMetaType::Int,} },

    //! mrq msg
    { mrq_msg_run, { QMetaType::Int, },
    },
    { mrq_msg_stop, { QMetaType::Int, },
    },
    { mrq_msg_rst, { QMetaType::Int, },
    },
    { mrq_msg_program, { QMetaType::Int, },
    },
    { mrq_msg_prepare, { QMetaType::Int, },
    },

    { mrq_msg_idle, { QMetaType::Int, },
    },
    { mrq_msg_calcing, { QMetaType::Int, },
    },
    { mrq_msg_calcend, { QMetaType::Int, },
    },
    { mrq_msg_standby, { QMetaType::Int, },
    },

    { mrq_msg_running, { QMetaType::Int, },
    },
    { mrq_msg_error, { QMetaType::Int, },
    },
};

void deviceMRQ::onMsg( int subAxes, RoboMsg &msg )
{
    //! type match
    if ( !RoboMsg::checkMsg(msg, _msg_patterns, sizeof_array(_msg_patterns)) )
    {
        logDbg()<<msg.getMsg()<<msg.size();
        return;
    }

    //! proc msg
    int varMsg = msg.getMsg();
    int varPara = msg.at(0).toInt();

    //! fsm proc
    if ( subAxes >= 0 && subAxes < axes() )
    {
        mMrqFsms[subAxes].proc( varMsg, varPara );
    }
    else
    { logWarning()<<subAxes; }
}

void deviceMRQ::onTimer( void *pContext, int id )
{
    Q_ASSERT( NULL != pContext );

    //! context is fsm
    MrqFsm *pFsm = (MrqFsm*)pContext;
    lpc( pFsm->axes() )->postMsg( e_robot_timeout, id );
}

void deviceMRQ::attachCondition( int subAxes,
                                 RoboCondition *pCond )
{
    if ( subAxes >= 0 && subAxes < axes() )
    {
        mMrqFsms[subAxes].attachCondition( pCond );
    }
}

bool deviceMRQ::waitCondition( int subAxes,
                               RoboCondition *pCond,
                               int tmoms )
{
    if ( subAxes >= 0 && subAxes < axes() )
    {
        return mMrqFsms[subAxes].waitCondition( pCond, tmoms );
    }

    return false;
}

RoboFsm *deviceMRQ::Fsm( int ax )
{
    Q_ASSERT( ax >= 0 && ax < axes() );

    return mMrqFsms+ax;
}

deviceMotor *deviceMRQ::Motor( int ax )
{
    Q_ASSERT( ax >= 0 && ax < axes() );

    return mMotors + ax;
}

}
