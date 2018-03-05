#ifndef DEVICEMRQ_MSG_H
#define DEVICEMRQ_MSG_H

#include "../../com/robomsg.h"
namespace MegaDevice
{

enum mrqMsg
{
    mrq_msg_unk = e_robo_operation,

    mrq_msg_run,        //! int axes
    mrq_msg_stop,       //! int axes
    mrq_msg_force_stop, //! int axes
    mrq_msg_rst,        //! int axes

    mrq_msg_program,    //! int axes

    mrq_msg_idle,       //! int axes
    mrq_msg_calcing,    //! int axes
    mrq_msg_calcend,    //! int axes
    mrq_msg_standby,    //! int axes
    mrq_msg_running,    //! int axes
    mrq_msg_error,      //! int axes

    mrq_msg_change_state,   //! int axes, int stat
};

}

#endif // DEVICEMRQ_MSG_H
