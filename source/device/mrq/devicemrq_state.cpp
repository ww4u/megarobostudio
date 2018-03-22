#include "devicemrq_state.h"

#include "./deviceMRQ.h"

#define state_timer_id      1
#define state_timer_tmo     time_s(1)    //! us

namespace MegaDevice {

MrqStateCondition::MrqStateCondition( int axes, int stat ) : RoboStateCondition( axes, stat )
{
}

//! context
MrqFsmContext::MrqFsmContext()
{
    m_pMrq = NULL;
    mAxes = 0;

    mbRunReqed = false;
}

void MrqFsmContext::set( deviceMRQ *pMrq, int axes )
{
    m_pMrq = pMrq;
    mAxes = axes;
}
deviceMRQ *MrqFsmContext::Mrq()
{
    Q_ASSERT( NULL != m_pMrq );
    return m_pMrq;
}
int MrqFsmContext::axes()
{ return mAxes; }

void MrqFsmContext::reqRun( bool b )
{
    mbRunReqed = b;logWarning()<<b<<QString::number( (quint32)(&mbRunReqed), 16 );
}
bool MrqFsmContext::runReqed()
{logDbg()<<QString::number( (quint32)(&mbRunReqed), 16 );
    return mbRunReqed;
}

//! MrqFsm
MrqFsm::MrqFsm()
{
    mState = mrq_state_idle;
}

MrqFsm::~MrqFsm()
{
    foreach (RoboStateUnit * value, mStateMap )
    {
        Q_ASSERT( NULL != value );
        delete value;
    }

    mStateMap.clear();
}

void MrqFsm::proc( int msg, int para )
{
    //! sys proc
    if ( msg == e_robot_timeout )
    { m_pNowState->onTimer( para ); }
logDbg()<<m_pNowState->name()<<msg<<para;
    //! base proc
    RoboFsm::proc( msg, para );
}

#define create_unit( stat, mold )   \
                                    pUnit = new mold(this);\
                                    Q_ASSERT( NULL != pUnit );\
                                    mStateMap.insert( stat, pUnit );
void MrqFsm::build()
{
    //! create each state
    RoboStateUnit *pUnit;
    create_unit( mrq_state_idle, IdleMrqUnit );
    create_unit( mrq_state_run_reqed, RunReqedMrqUnit );
    create_unit( mrq_state_program, ProgramMrqUnit );
    create_unit( mrq_state_calcing, CalcingMrqUnit );

    create_unit( mrq_state_calcend, CalcendMrqUnit );
    create_unit( mrq_state_standby, StandbyMrqUnit );
    create_unit( mrq_state_prerun, PreRunMrqUnit );
    create_unit( mrq_state_running, RunningMrqUnit );

    create_unit( mrq_state_prestop, PreStopMrqUnit );

    //! init state
    init( mStateMap[mrq_state_idle] );
}

void MrqFsm::toState( mrqState stat )
{
    if ( mStateMap.contains( stat ) )
    {
        Q_ASSERT( mStateMap[stat] != NULL );

        RoboFsm::toState( mStateMap[stat] );
        mState = stat;

        //! subscribe to leader
        if ( m_pLeader != NULL )
        { m_pLeader->subscribe( this, e_robot_member_state, stat ); }

        //! trigger condition
        if ( m_pCond != NULL )
        { m_pCond->trigger( (void*)stat ); }
    }
    else
    { Q_ASSERT(false);}
}

void MrqFsm::startTimer( int id, int tmous )
{
    Q_ASSERT( NULL != m_pMrq );

    m_pMrq->startTimer( this, id, tmous );
}
void MrqFsm::killTimer( int id )
{
    Q_ASSERT( NULL != m_pMrq );

    m_pMrq->killTimer( this, id );
}

int MrqFsm::state()
{ return mState; }

//! mrq unit
MrqUnit::MrqUnit( RoboFsm *pFsm ) : RoboStateUnit( pFsm )
{
}

void MrqUnit::toState( mrqState stat )
{
    selfFsm()->toState( stat );
}

void MrqUnit::onEnter()
{
    killTimer( state_timer_id );
}
void MrqUnit::onExit()
{
    killTimer( state_timer_id );
}

void MrqUnit::onTimer( int id )
{
    if ( id == state_timer_id )
    {
        selfFsm()->Mrq()->requestMotionState( selfFsm()->axes() );
//        logDbg()<<id;
//        sysLog( __FUNCTION__ );
        logWarning()<<id;
    }
    else
    { logDbg()<<id; }
}

MrqFsm *MrqUnit::selfFsm()
{
    return (MrqFsm*)Fsm();
}

//! idle unit
IdleMrqUnit::IdleMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("idlemrq"); }
void IdleMrqUnit::proc( int msg, int para )
{
    //! ops
    if ( msg == mrq_msg_run )
    { toState( mrq_state_run_reqed); }

    else if ( msg == mrq_msg_stop )
    { }

    else if ( msg == mrq_msg_rst )
    { }

    else if ( msg == mrq_msg_prepare )
    { toState( mrq_state_calcend); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing); }

    else if ( msg == mrq_msg_calcend )
    { /*toState( mrq_state_calcend);*/ }

    else if ( msg == mrq_msg_standby )
    { /*toState( mrq_state_standby);*/ }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running); }

    else if ( msg == mrq_msg_error )
    { }

    else    //! keep
    { }
}

void IdleMrqUnit::onEnter()
{
    selfFsm()->reqRun( false );
}
//void IdleMrqUnit::onExit()
//{}
//void IdleMrqUnit::onRst()
//{
//}

//! RunReqedMrqUnit
RunReqedMrqUnit::RunReqedMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("runreqed"); }
void RunReqedMrqUnit::proc( int msg, int para )
{
    //! ops
    if ( msg == mrq_msg_run )
    { }

    else if ( msg == mrq_msg_stop )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program); }

    else if ( msg == mrq_msg_prepare )
    { toState( mrq_state_calcend); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_calcend); }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle); }

    else    //! keep
    { }
}

void RunReqedMrqUnit::onEnter()
{    
    selfFsm()->reqRun( true );
}
//void RunReqedMrqUnit::onExit()
//{}
//void RunReqedMrqUnit::onRst()
//{}

//! ProgramMrqUnit
ProgramMrqUnit::ProgramMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("programmrq"); }
void ProgramMrqUnit::proc( int msg, int para )
{
    //! ops
    if ( msg == mrq_msg_run )
    { selfFsm()->reqRun( true ); }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle);

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle);  }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_calcend); }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle); }

    else    //! keep
    { }
}

void ProgramMrqUnit::onEnter()
{
    MrqUnit::onEnter();

    startTimer( state_timer_id, state_timer_tmo );
}
//void ProgramMrqUnit::onExit()
//{
//    MrqUnit::onExit();
//}
//void ProgramMrqUnit::onRst()
//{}

//! CalcingMrqUnit
CalcingMrqUnit::CalcingMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("calcing"); }
void CalcingMrqUnit::proc( int msg, int para )
{
    //! ops
    if ( msg == mrq_msg_run )
    { selfFsm()->reqRun( true ); }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle);

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle);  }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle);  }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_calcend); }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle); }

    else    //! keep
    { }
}

void CalcingMrqUnit::onEnter()
{
    MrqUnit::onEnter();
    startTimer( state_timer_id, state_timer_tmo );
}
//void CalcingMrqUnit::onExit()
//{
//    MrqUnit::onExit();
//}
//void CalcingMrqUnit::onRst()
//{}

//! CalcendMrqUnit
CalcendMrqUnit::CalcendMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("calcend"); }
void CalcendMrqUnit::proc( int msg, int para )
{
    //! ops
    if ( msg == mrq_msg_run )
    { selfFsm()->reqRun( true ); }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle);

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle);  }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing); }

    else if ( msg == mrq_msg_calcend )
    {
//        toState( mrq_state_calcend); logDbg();

        selfFsm()->Mrq()->setMOTION_SWITCH(
                                                selfFsm()->axes(),

                                                MRQ_MOTION_SWITCH_PREPARE,
                                                MRQ_MOTION_SWITCH_1_MAIN
                                            );

    }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby); logDbg(); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle); }

    else    //! keep
    { }
}

void CalcendMrqUnit::onEnter()
{
    MrqUnit::onEnter();
    startTimer( state_timer_id, state_timer_tmo );

    int ret;
    ret = selfFsm()->Mrq()->setMOTION_SWITCH(
                                        selfFsm()->axes(),

                                        MRQ_MOTION_SWITCH_PREPARE,
                                        MRQ_MOTION_SWITCH_1_MAIN
                                    );
    if ( ret != 0 )
    {
        toState( mrq_state_idle );
    }
}
//void CalcendMrqUnit::onExit()
//{
//    MrqUnit::onExit();
//}
//void CalcendMrqUnit::onRst()
//{}

//! StandbyMrqUnit
StandbyMrqUnit::StandbyMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("standby"); }
void StandbyMrqUnit::proc( int msg, int para )
{
    //! ops
    if ( msg == mrq_msg_run )
    {
        selfFsm()->reqRun( true );

        //! force run
        int ret;
        ret = selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                                  MRQ_MOTION_SWITCH_RUN,
                                                  MRQ_MOTION_SWITCH_1_MAIN );
        logDbg();

        if ( ret != 0  )
        { toState(mrq_state_idle);  }
        else
        { toState(mrq_state_prerun); }
    }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle);

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing); }

    //! end
    else if ( msg == mrq_msg_calcend )
    { /*toState( mrq_state_calcend);*/ logDbg(); }

    else if ( msg == mrq_msg_standby )
    {
        if ( selfFsm()->runReqed() )
        {
            selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                                MRQ_MOTION_SWITCH_RUN,
                                                MRQ_MOTION_SWITCH_1_MAIN );
//            sysLog( __FUNCTION__, QString::number(__LINE__) );
            logDbg();
        }
        else
        { logDbg(); }
    }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle); }

    else    //! keep
    { }
}

void StandbyMrqUnit::onEnter()
{
    MrqUnit::onEnter();

    if ( selfFsm()->runReqed() )
    {
        //! atuo run
        proc( mrq_msg_run, selfFsm()->axes() );
    }
    else
    {}
}
//void StandbyMrqUnit::onExit()
//{
//    MrqUnit::onExit();
//}
//void StandbyMrqUnit::onRst()
//{}

//void StandbyMrqUnit::onTimer( int id )
//{
//    MrqUnit::onTimer( id );

//    if ( id == state_timer_id )
//    {

//    }
//}

//! PreRunMrqUnit
PreRunMrqUnit::PreRunMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("prerun"); }
void PreRunMrqUnit::proc( int msg, int para )
{
    //! ops
    if ( msg == mrq_msg_run )
    {

    }

    else if ( msg == mrq_msg_stop )
    {
        selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                            MRQ_MOTION_SWITCH_STOP,
                                            MRQ_MOTION_SWITCH_1_MAIN );

        toState(mrq_state_idle);

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_idle); }

    else if ( msg == mrq_msg_standby )
    {
        /*toState( mrq_state_standby);*/

        selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                                  MRQ_MOTION_SWITCH_RUN,
                                            MRQ_MOTION_SWITCH_1_MAIN );
    }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle); }

    else    //! keep
    { }
}

void PreRunMrqUnit::onEnter()
{
    MrqUnit::onEnter();

    startTimer( state_timer_id, state_timer_tmo );
}
//void PreRunMrqUnit::onExit()
//{
//    MrqUnit::onExit();
//}
//void PreRunMrqUnit::onRst()
//{}

//! RunningMrqUnit
RunningMrqUnit::RunningMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("running"); }
void RunningMrqUnit::proc( int msg, int para )
{
    if ( msg == mrq_msg_stop )
    {
        selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                             MRQ_MOTION_SWITCH_STOP,
                                            MRQ_MOTION_SWITCH_1_MAIN );

        toState( mrq_state_prestop );
    }
    else if ( msg == mrq_msg_running )
    {
        //! keep
    }
    else
    {   logDbg()<<msg<<para;
        toState( mrq_state_idle );
    }
}

void RunningMrqUnit::onEnter()
{
//    MrqUnit::onEnter();
    MrqUnit::onEnter();

    startTimer( state_timer_id, state_timer_tmo );
}
//void RunningMrqUnit::onExit()
//{
//    MrqUnit::onExit();
//}
//void RunningMrqUnit::onRst()
//{}

//! PreStopMrqUnit
PreStopMrqUnit::PreStopMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("prestop"); }
void PreStopMrqUnit::proc( int msg, int para )
{
    //! ops
    if ( msg == mrq_msg_run )
    {

    }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle);

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_calcend); }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle); }

    else    //! keep
    { }
}

void PreStopMrqUnit::onEnter()
{
    MrqUnit::onEnter();

    startTimer( state_timer_id, state_timer_tmo );
}
//void PreStopMrqUnit::onExit()
//{ MrqUnit::onExit(); }
//void PreStopMrqUnit::onRst()
//{}

}
