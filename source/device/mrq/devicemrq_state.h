#ifndef DEVICEMRQ_STATE_H
#define DEVICEMRQ_STATE_H

#include <QtCore>
#include "../robostate.h"
#include "devicemrq_msg.h"

namespace MegaDevice
{

enum mrqState
{
    mrq_state_idle,
    mrq_state_run_reqed,
    mrq_state_program,

    mrq_state_calcing,
    mrq_state_calcend,
    mrq_state_standby,

    mrq_state_prerun,
    mrq_state_running,
    mrq_state_prestop,
};

class deviceMRQ;

class MrqStateCondition : public RoboStateCondition
{
public:
    MrqStateCondition( int axes = 0, int stat = 0 );

};

class MrqFsmContext
{
public:
    MrqFsmContext( );

    void set( deviceMRQ *pMrq, int axes = 0 );
    deviceMRQ *Mrq();
    int axes();

    void reqRun( bool b );
    bool runReqed();

public:
    deviceMRQ *m_pMrq;
    int mAxes;

    bool mbRunReqed;
};

//! class
class MrqFsm : public RoboFsm , public MrqFsmContext
{
public:
    MrqFsm();
    ~MrqFsm();

public:
    virtual void build();

    virtual void proc( int msg, int para );

    virtual void toState( mrqState stat );

    virtual void startTimer( int id=0, int tmous=1000 );
    virtual void killTimer( int id=0 );

public:
    int state();

protected:
    QMap< mrqState, RoboStateUnit *> mStateMap;
    mrqState mState;

};

//! mrq state
class MrqUnit : public RoboStateUnit
{
public:
    MrqUnit( RoboFsm *pFsm );

public:
    virtual void toState( mrqState stat );

    virtual void onEnter();
    virtual void onExit();

    virtual void onTimer( int id );
public:
    MrqFsm *selfFsm();

};

class IdleMrqUnit : public MrqUnit
{
public:
    IdleMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();

protected:

};

class RunReqedMrqUnit : public MrqUnit
{
public:
    RunReqedMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class ProgramMrqUnit : public MrqUnit
{
public:
    ProgramMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class CalcingMrqUnit : public MrqUnit
{
public:
    CalcingMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class CalcendMrqUnit : public MrqUnit
{
public:
    CalcendMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class StandbyMrqUnit : public MrqUnit
{
public:
    StandbyMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();

//    virtual void onTimer( int id );
};

class PreRunMrqUnit : public MrqUnit
{
public:
    PreRunMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class RunningMrqUnit : public MrqUnit
{
public:
    RunningMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class PreStopMrqUnit : public MrqUnit
{
public:
    PreStopMrqUnit( RoboFsm *pFsm );

public:
    virtual void proc( int msg, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

}

#endif // DEVICEMRQ_STATE_H
