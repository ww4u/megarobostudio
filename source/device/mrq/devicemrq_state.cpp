#include "devicemrq_state.h"

#include "./deviceMRQ.h"

#define state_timer_id      1
//#define state_timer_tmo     time_s(1)    //! us
#define state_timer_tmo     time_ms(500)
#define prefetch_timer_tmo  time_ms(100)

#define prepare_timer_id    2
#define prepare_timer_tmo   time_s(5)

namespace MegaDevice {

MrqStateCondition::MrqStateCondition( int axes, int stat ) : RoboStateCondition( axes, stat )
{
}

//! context
MrqFsmContext::MrqFsmContext()
{
    m_pMrq = NULL;

    mbRunReqed = false;
}

void MrqFsmContext::setContext( deviceMRQ *pMrq )
{
    m_pMrq = pMrq;

}
deviceMRQ *MrqFsmContext::Mrq()
{
    Q_ASSERT( NULL != m_pMrq );
    return m_pMrq;
}

void MrqFsmContext::reqRun( bool b )
{
    mbRunReqed = b;//logWarning()<<b<<QString::number( (UINT_PTR)(&mbRunReqed), 16 );
}
bool MrqFsmContext::runReqed()
{//logDbg()<<QString::number( (quint32)(&mbRunReqed), 16 );
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

void MrqFsm::toState( mrqState stat, RoboMsg &detail )
{
    if ( mStateMap.contains( stat ) )
    {
        Q_ASSERT( mStateMap[stat] != NULL );

        lockState();
        RoboFsm::toState( mStateMap[stat], detail );
        mState = stat;
        unlockState();

        //! subscribe to leader
        if ( m_pLeader != NULL )
        {logDbg()<<stat;
            m_pLeader->subscribe( this,
                                  e_robot_member_state,
                                  stat,
                                  detail );
        }

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

int MrqFsm::setState( int stat )
{
    Q_ASSERT( mStateMap.contains( (mrqState)stat ) );
    lockState();
        m_pNowState =  mStateMap[ (mrqState)stat ];
        mState = (mrqState)stat;
    unlockState();

    if ( stat != mrq_state_idle
         && stat != mrq_state_calcend )
    { startTimer( state_timer_id, state_timer_tmo ); }

    return 0;
}

int MrqFsm::state()
{ return mState; }

//! mrq unit
MrqUnit::MrqUnit( RoboFsm *pFsm ) : RoboStateUnit( pFsm )
{
}

void MrqUnit::toState( mrqState stat, RoboMsg &detail )
{
    selfFsm()->toState( stat, detail );
}

void MrqUnit::onEnter( RoboMsg &detail )
{
    killTimer( state_timer_id );

//    selfFsm()->Mrq()->lpc( selfFsm()->axes() )->clear();

}
void MrqUnit::onExit( RoboMsg &detail )
{
    killTimer( state_timer_id );

//    selfFsm()->Mrq()->lpc( selfFsm()->axes() )->clear();
}

void MrqUnit::onTimer( int id )
{
    if ( id == state_timer_id )
    {
        selfFsm()->Mrq()->requestMotionState( *selfFsm() );
        logDbg()<<id;
//        sysLog( __FUNCTION__ );
//        logWarning()<<id;
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
void IdleMrqUnit::proc( int msg, RoboMsg &detail )
{
    //! ops
/*    if ( msg == mrq_msg_run )
    { toState( mrq_state_run_reqed, detail ); }

    else */if ( msg == mrq_msg_stop )
    { }

    else if ( msg == mrq_msg_rst )
    { }

    else if ( msg == mrq_msg_call
              || msg == mrq_msg_run )
    {logDbg();
        selfFsm()->reqRun(true);
        toState( mrq_state_calcend, detail );
    }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program, detail ); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing, detail ); }

    else if ( msg == mrq_msg_calcend )
    { /*toState( mrq_state_calcend, detail ); sysLog(__FUNCTION__, QString::number(__LINE__) );*/ }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby, detail ); logDbg(); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running, detail ); }

    else if ( msg == mrq_msg_error )
    { sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else    //! keep
    { }
}

void IdleMrqUnit::onEnter(RoboMsg &detail)
{
    selfFsm()->reqRun( false );

    killTimer( state_timer_id );
}

//! RunReqedMrqUnit
RunReqedMrqUnit::RunReqedMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("runreqed"); }
void RunReqedMrqUnit::proc( int msg, RoboMsg &detail )
{
    //! ops
    if ( msg == mrq_msg_run )
    { }

    else if ( msg == mrq_msg_stop )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program, detail ); }

    else if ( msg == mrq_msg_call )
    {
        selfFsm()->reqRun(true);
        toState( mrq_state_calcend, detail );
    }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing, detail ); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_calcend, detail ); }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby, detail ); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running, detail ); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else    //! keep
    { }
}

void RunReqedMrqUnit::onEnter(RoboMsg &detail)
{    
    selfFsm()->reqRun( true );
}

//! ProgramMrqUnit
ProgramMrqUnit::ProgramMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("programmrq"); }
void ProgramMrqUnit::proc( int msg, RoboMsg &detail )
{
    //! ops
    if ( msg == mrq_msg_run )
    { selfFsm()->reqRun( true ); }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) );

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program, detail ); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { //! keep
    }
    //{ toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); Q_ASSERT(false); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing, detail ); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_calcend, detail ); }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby, detail ); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running, detail ); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else    //! keep
    { }
}

void ProgramMrqUnit::onEnter( RoboMsg &detail )
{
    MrqUnit::onEnter( detail );

    startTimer( state_timer_id, state_timer_tmo );
}

//! CalcingMrqUnit
CalcingMrqUnit::CalcingMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("calcing"); }
void CalcingMrqUnit::proc( int msg, RoboMsg &detail )
{
    //! ops
    if ( msg == mrq_msg_run )
    { selfFsm()->reqRun( true ); }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) );

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program, detail ); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { sysLog( __FUNCTION__, QString::number(__LINE__) ); toState(mrq_state_idle, detail );  }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing, detail ); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_calcend, detail ); }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby, detail ); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running, detail ); }

    else if ( msg == mrq_msg_error )
    { sysLog( __FUNCTION__, QString::number(__LINE__) ); toState(mrq_state_idle, detail ); }

    else    //! keep
    { }
}

void CalcingMrqUnit::onEnter( RoboMsg &detail )
{
    MrqUnit::onEnter( detail );

    startTimer( state_timer_id, state_timer_tmo );
}

//! CalcendMrqUnit
CalcendMrqUnit::CalcendMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("calcend"); }
void CalcendMrqUnit::proc( int msg, RoboMsg &detail )
{
    //! ops
    if ( msg == mrq_msg_run
         || msg == mrq_msg_call )
    {
        selfFsm()->reqRun( true );

        startTimer( state_timer_id, prefetch_timer_tmo );
        startTimer( prepare_timer_id, prepare_timer_tmo );
//logDbg()<<QThread::currentThreadId()<<msg;
        int ret;
        ret = selfFsm()->Mrq()->setMOTION_SWITCH(
                                            selfFsm()->axes(),

                                            MRQ_MOTION_SWITCH_PREPARE,
                                            (MRQ_MOTION_SWITCH_1)selfFsm()->page()
                                        );

    }

    else if ( msg == mrq_msg_stop )
    {sysLog( __FUNCTION__, QString::number(__LINE__) );
        toState(mrq_state_idle, detail );

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { sysLog( __FUNCTION__, QString::number(__LINE__) );
        toState(mrq_state_idle, detail );
    }

    else if ( msg == mrq_msg_prepare )
    {
        startTimer( prepare_timer_id, prepare_timer_tmo );
    }

    else if ( msg == mrq_msg_program )
    { sysLog( __FUNCTION__, QString::number(__LINE__) );
        toState( mrq_state_program, detail );
    }

    //! device status
    else if ( msg == mrq_msg_idle )
    { sysLog( __FUNCTION__, QString::number(__LINE__) );
        toState(mrq_state_idle, detail );
    }

    else if ( msg == mrq_msg_calcing )
    { sysLog( __FUNCTION__, QString::number(__LINE__) );
        toState( mrq_state_calcing, detail );
    }

    //! do nothing
    else if ( msg == mrq_msg_calcend )
    {
    }

    else if ( msg == mrq_msg_standby )
    {
        toState( mrq_state_standby, detail );
    }

    else if ( msg == mrq_msg_running )
    { sysLog( __FUNCTION__, QString::number(__LINE__) );
        toState( mrq_state_running, detail );
    }

    else if ( msg == mrq_msg_error )
    { sysLog( __FUNCTION__, QString::number(__LINE__) );
        toState(mrq_state_idle, detail );
    }

    else    //! keep
    { }
}

void CalcendMrqUnit::onEnter( RoboMsg &detail )
{
    MrqUnit::onEnter( detail );

    if ( selfFsm()->runReqed() )
    {
        int ret;
        ret = selfFsm()->Mrq()->setMOTION_SWITCH(
                                            selfFsm()->axes(),

                                            MRQ_MOTION_SWITCH_PREPARE,
                                            (MRQ_MOTION_SWITCH_1)selfFsm()->page()
                                        );
        if ( ret != 0 )
        {
            toState( mrq_state_idle, detail );sysLog( __FUNCTION__, QString::number(__LINE__) );
        }
        else
        {
             startTimer( state_timer_id, prefetch_timer_tmo );
             startTimer( prepare_timer_id, prepare_timer_tmo );
        }
    }
}
void CalcendMrqUnit::onExit( RoboMsg &detail )
{
    killTimer( prepare_timer_id );
}
void CalcendMrqUnit::onTimer( int id )
{
    //! base
    MrqUnit::onTimer( id );

    //! timeout
    if ( id == prepare_timer_id )
    {
        selfFsm()->Mrq()->setMOTION_SWITCH(
                                                    selfFsm()->axes(),

                                                    MRQ_MOTION_SWITCH_PREPARE,
                                                    (MRQ_MOTION_SWITCH_1)selfFsm()->page()
                                                );
        sysError("parepare fail", QString::number( selfFsm()->axes() ) );
    }
}

//! StandbyMrqUnit
StandbyMrqUnit::StandbyMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("standby"); }
void StandbyMrqUnit::proc( int msg, RoboMsg &detail )
{
    //! ops
    if ( msg == mrq_msg_run )
    {
        selfFsm()->reqRun( true );

        //! force run
        int ret;
        ret = selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                                  MRQ_MOTION_SWITCH_RUN,
                                                  (MRQ_MOTION_SWITCH_1)selfFsm()->page() );
        logDbg();

        if ( ret != 0  )
        { toState(mrq_state_idle, detail );  sysLog( __FUNCTION__, QString::number(__LINE__) ); }
        else
        { toState(mrq_state_prerun, detail ); }
    }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) );
        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) );}

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program, detail ); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) );}

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing, detail ); }

    //! end
    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_idle, detail );/* sysLog( __FUNCTION__, QString::number(__LINE__) );*/ }

    else if ( msg == mrq_msg_standby )
    {
        if ( selfFsm()->runReqed() )
        {
            selfFsm()->reqRun( false );

            selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                                MRQ_MOTION_SWITCH_RUN,
                                                (MRQ_MOTION_SWITCH_1)selfFsm()->page() );
//            sysLog( __FUNCTION__, QString::number(__LINE__) );
            logDbg();
        }
        else
        { logDbg(); }
    }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running, detail ); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else    //! keep
    { }
}

void StandbyMrqUnit::onEnter( RoboMsg &detail )
{
    MrqUnit::onEnter( detail );

    if ( selfFsm()->runReqed() )
    {
        selfFsm()->reqRun( false );

        //! atuo run
//        proc( mrq_msg_run, detail );

//        selfFsm()->reqRun( true );

        //! force run
        int ret;
        ret = selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                                  MRQ_MOTION_SWITCH_RUN,
                                                  (MRQ_MOTION_SWITCH_1)selfFsm()->page() );
        logDbg();

        if ( ret != 0  )
        { toState(mrq_state_idle, detail );  sysLog( __FUNCTION__, QString::number(__LINE__) ); }
        else
        { toState(mrq_state_prerun, detail ); }

        startTimer( state_timer_id, state_timer_tmo );
    }
    else
    {}
}

//! PreRunMrqUnit
PreRunMrqUnit::PreRunMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("prerun"); }
void PreRunMrqUnit::proc( int msg, RoboMsg &detail )
{
    //! ops
    if ( msg == mrq_msg_run )
    {

    }

    else if ( msg == mrq_msg_stop )
    {
        selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                            MRQ_MOTION_SWITCH_STOP,
                                            (MRQ_MOTION_SWITCH_1)selfFsm()->page() );

        toState(mrq_state_idle, detail);sysLog( __FUNCTION__, QString::number(__LINE__) );

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program, detail ); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing, detail ); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_standby )
    {
        /*toState( mrq_state_standby);*/

        selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                            MRQ_MOTION_SWITCH_RUN,
                                            (MRQ_MOTION_SWITCH_1)selfFsm()->page() );
    }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running, detail ); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else    //! keep
    { }
}

void PreRunMrqUnit::onEnter( RoboMsg &detail )
{
    MrqUnit::onEnter( detail );

    startTimer( state_timer_id, state_timer_tmo );
}

//! RunningMrqUnit
RunningMrqUnit::RunningMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("running"); }
void RunningMrqUnit::proc( int msg, RoboMsg &detail )
{
    if ( msg == mrq_msg_stop )
    {
        selfFsm()->Mrq()->setMOTION_SWITCH( selfFsm()->axes(),
                                            MRQ_MOTION_SWITCH_STOP,
                                            (MRQ_MOTION_SWITCH_1)selfFsm()->page() );

        toState( mrq_state_prestop, detail );
    }
    else if ( msg == mrq_msg_calcend )
    {
        toState( mrq_state_idle, detail );
    }
    else if ( msg == mrq_msg_running )
    {
        //! keep
    }
    else
    {
        toState( mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__), QString::number(msg) );
    }
}

void RunningMrqUnit::onEnter( RoboMsg &detail )
{
    MrqUnit::onEnter( detail );

    startTimer( state_timer_id, state_timer_tmo );
}

//! PreStopMrqUnit
PreStopMrqUnit::PreStopMrqUnit( RoboFsm *pFsm ) : MrqUnit( pFsm )
{ setName("prestop"); }
void PreStopMrqUnit::proc( int msg, RoboMsg &detail )
{
    //! ops
    if ( msg == mrq_msg_run )
    {

    }

    else if ( msg == mrq_msg_stop )
    {
        toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) );

        //! \todo stop downloading
    }

    else if ( msg == mrq_msg_rst )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_program )
    { toState( mrq_state_program, detail ); }

    //! device status
    else if ( msg == mrq_msg_idle )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else if ( msg == mrq_msg_calcing )
    { toState( mrq_state_calcing, detail ); }

    else if ( msg == mrq_msg_calcend )
    { toState( mrq_state_calcend, detail ); }

    else if ( msg == mrq_msg_standby )
    { toState( mrq_state_standby, detail ); }

    else if ( msg == mrq_msg_running )
    { toState( mrq_state_running, detail ); }

    else if ( msg == mrq_msg_error )
    { toState(mrq_state_idle, detail ); sysLog( __FUNCTION__, QString::number(__LINE__) ); }

    else    //! keep
    { }
}

void PreStopMrqUnit::onEnter( RoboMsg &detail )
{
    MrqUnit::onEnter( detail );

    startTimer( state_timer_id, state_timer_tmo );
}

}
