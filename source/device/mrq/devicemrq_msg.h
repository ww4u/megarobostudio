#ifndef DEVICEMRQ_MSG_H
#define DEVICEMRQ_MSG_H

#include "../../com/robomsg.h"
namespace MegaDevice
{

enum mrqMsg
{
    mrq_msg_unk = e_robo_operation,

    mrq_msg_run,        //! region
    mrq_msg_stop,       //! region
    mrq_msg_force_stop, //! region
    mrq_msg_rst,        //! region

    mrq_msg_call,       //! region ( load + run )
//    mrq_msg_zero,

    mrq_msg_program,    //! region

    mrq_msg_idle,       //! region
    mrq_msg_calcing,    //! region
    mrq_msg_calcend,    //! region
    mrq_msg_standby,    //! region
    mrq_msg_running,    //! region
    mrq_msg_error,      //! region

    mrq_msg_change_state,   //! int axes, int stat
};

}

#endif // DEVICEMRQ_MSG_H
