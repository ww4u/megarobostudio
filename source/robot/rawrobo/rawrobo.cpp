
#include "rawrobo.h"
#include "../../device/mrq/devicemrq_state.h"
#include "../../device/board/_MRQ_enum.h"

WorldPoint::WorldPoint( float px, float py, float pz, float phand )
{
    x = px;
    y = py;
    z = pz;

    hand = phand;
}

RawRobo::RawRobo()
{
    mPlanStep = 5.0;  //! mm
    mPlanMode = plan_linear;
}

void RawRobo::switchReset()
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH, mSubGroupId, (byte)MRQ_MOTION_SWITCH_RESET );
}
void RawRobo::switchStop()
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH, mSubGroupId, (byte)MRQ_MOTION_SWITCH_STOP );
}
void RawRobo::switchRun()
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH, mSubGroupId, (byte)MRQ_MOTION_SWITCH_RUN );
}
void RawRobo::switchPrepare()
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH, mSubGroupId, (byte)MRQ_MOTION_SWITCH_PREPARE );
}
void RawRobo::switchEmergStop()
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH, mSubGroupId, (byte)MRQ_MOTION_SWITCH_EMERGSTOP );
}

void RawRobo::toState( int stat )
{}
int RawRobo::state()
{ return mFsm.state(); }

void RawRobo::attachCondition(
                              MegaDevice::RoboCondition *pCond )
{
    mFsm.attachCondition( pCond );
}
bool RawRobo::waitCondition(
                            MegaDevice::RoboCondition *pCond,
                            int tmoms )
{
    return mFsm.waitCondition( pCond, tmoms );
}

void RawRobo::setPlanStep( float step )
{ mPlanStep = step; }
float RawRobo::planStep()
{ return mPlanStep; }

void RawRobo::setPlanMode( eRoboPlanMode mode )
{ mPlanMode = mode; }
eRoboPlanMode RawRobo::getPlanMode()
{ return mPlanMode; }

//! state condition
RawRoboStateCondition::RawRoboStateCondition( int stat )
                      : RoboStateCondition( -1, stat )
{}

RawRoboFsm::RawRoboFsm()
{
    m_pRobot = NULL;

    mbRunReqed = false;
    mState = 0;
}
RawRoboFsm::~RawRoboFsm()
{
    foreach (RawRoboUnit * value, mStateMap )
    {
        Q_ASSERT( NULL != value );
        delete value;
    }

    mStateMap.clear();
}
#define create_unit( stat, mold )   \
                                    pUnit = new mold(this, m_pRobot->axes() );\
                                    Q_ASSERT( NULL != pUnit );\
                                    mStateMap.insert( stat, pUnit ); \
                                    pUnit->attachFsm( this );
void RawRoboFsm::build()
{
    Q_ASSERT( NULL != m_pRobot );

    //! create each state
    RawRoboUnit *pUnit;
    create_unit( MegaDevice::mrq_state_idle, IdleRawRoboUnit );
    create_unit( MegaDevice::mrq_state_run_reqed, RunReqedRawRoboUnit );
    create_unit( MegaDevice::mrq_state_program, ProgramRawRoboUnit );
    create_unit( MegaDevice::mrq_state_calcing, CalcingRawRoboUnit );

    create_unit( MegaDevice::mrq_state_calcend, CalcendRawRoboUnit );
    create_unit( MegaDevice::mrq_state_standby, StandbyRawRoboUnit );
    create_unit( MegaDevice::mrq_state_prerun, PreRunRawRoboUnit );
    create_unit( MegaDevice::mrq_state_running, RunningRawRoboUnit );

    create_unit( MegaDevice::mrq_state_prestop, PreStopRawRoboUnit );

    //! init state
    init( mStateMap[MegaDevice::mrq_state_idle] );
}

void RawRoboFsm::subscribe( MegaDevice::RoboFsm *pMember,
                        int msg,
                        int para )
{
    Q_ASSERT( NULL != pMember );
//    RoboFsm::proc( msg, (qint32)pMember->leaderPara(), para );

    Q_ASSERT( Robot() != NULL );
    Q_ASSERT( Robot()->lpc() != NULL );
logDbg()<<(qint32)pMember->leaderPara()<<QString::number( (quint32)pMember, 16 );
    Robot()->lpc()->postMsg( (eRoboMsg)msg,
                             (qint32)pMember->leaderPara(), //! ax
                             para );                        //! stat
}

void RawRoboFsm::toState( int stat )
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
        { m_pCond->trigger( (void*)stat ); logDbg()<<stat<<m_pNowState->name(); }

        Q_ASSERT( NULL != m_pRobot );
        m_pRobot->toState( stat );
    }
    else
    { Q_ASSERT(false); }
}

int RawRoboFsm::state()
{ return mState; }

void RawRoboFsm::attachRobot( RawRobo *pRobot )
{
    Q_ASSERT( NULL != pRobot );
    m_pRobot = pRobot;
}
RawRobo *RawRoboFsm::Robot()
{ return m_pRobot; }

void RawRoboFsm::reqRun( bool b )
{ mbRunReqed = b; }
bool RawRoboFsm::runReqed()
{ return mbRunReqed; }

//! state unit
RawRoboUnit::RawRoboUnit( MegaDevice::RoboFsm *pFsm,
                          int members )
{
    mMemberCount = members;

    rstState();
}
RawRoboUnit::~RawRoboUnit()
{}
void RawRoboUnit::toState( int stat )
{
    selfFsm()->toState( stat );

    //! to robot
    sysQueue()->postMsg( e_robot_status,
                         selfFsm()->Robot()->name(),
                         stat );

    logDbg()<<selfFsm()->Robot()->name();

}

void RawRoboUnit::proc( int msg, int subAx, int para )
{
    //! member state
    if ( msg == e_robot_member_state )
    {
        setMemberState( subAx, para );
    }

    //! ops
    else if ( msg == MegaDevice::mrq_msg_run )
    {
        selfFsm()->reqRun( true );

//        toState( MegaDevice::mrq_state_run_reqed );
    }

    else if ( msg == MegaDevice::mrq_msg_stop )
    {
        //! \todo stop downloading
        selfFsm()->Robot()->switchStop();

//        toState( MegaDevice::PreStopMrqUnit );
    }
    else if ( msg == MegaDevice::mrq_msg_force_stop )
    {
        //! \todo stop downloading
        selfFsm()->Robot()->switchEmergStop();

//        toState( MegaDevice::PreStopMrqUnit );
    }

    else if ( msg == MegaDevice::mrq_msg_rst )
    { toState( MegaDevice::mrq_state_idle); }

    else if ( msg == MegaDevice::mrq_msg_program )
    { toState( MegaDevice::mrq_state_program); }

    //! device status
    else if ( msg == MegaDevice::mrq_msg_idle )
    { toState( MegaDevice::mrq_state_idle ); }

    else if ( msg == MegaDevice::mrq_msg_calcing )
    { toState( MegaDevice::mrq_state_calcing); }

    else if ( msg == MegaDevice::mrq_msg_calcend )
    { toState( MegaDevice::mrq_state_calcend); }

    else if ( msg == MegaDevice::mrq_msg_standby )
    { toState( MegaDevice::mrq_state_standby); }

    else if ( msg == MegaDevice::mrq_msg_running )
    { toState( MegaDevice::mrq_state_running); }

    else if ( msg == MegaDevice::mrq_msg_error )
    { toState( MegaDevice::mrq_state_idle); }

    else    //! keep
    { }
}

void RawRoboUnit::onEnter()
{
}
void RawRoboUnit::onExit()
{}

RawRoboFsm *RawRoboUnit::selfFsm()
{
    return (RawRoboFsm*)Fsm();
}

void RawRoboUnit::setMemberState( int subAx, int stat )
{logDbg();
    mMemberStates[ subAx ] = stat;

    int preVal = mMemberStates[0];
    foreach (int val, mMemberStates)
    {
        //! value match
        if ( val == preVal )
        {}
        else
        { logDbg()<<mMemberStates; return; }
    }
logDbg()<<preVal;
    //! full match
    mState = preVal;
    toState( mState );
}
int RawRoboUnit::memberState( int subAx )
{
    Q_ASSERT( mMemberStates.contains(subAx) );

    return mMemberStates[ subAx ];
}

int RawRoboUnit::state()
{ return mState; }

void RawRoboUnit::rstState()
{
    mState = 0;
    for ( int i = 0; i < mMemberCount; i++ )
    { mMemberStates.insert( i, mState ); }
}

//! real states
//! idle
IdleRawRoboUnit::IdleRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
    mName = "IdleRawRoboUnit";
}

void IdleRawRoboUnit::proc( int msg, int subAx, int para )
{
    RawRoboUnit::proc( msg, subAx, para );
}

void IdleRawRoboUnit::onEnter()
{
    selfFsm()->reqRun( false );
}

//! run reqed
RunReqedRawRoboUnit::RunReqedRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
    mName = "RunReqedRawRoboUnit";
}

void RunReqedRawRoboUnit::proc( int msg, int subAx, int para )
{
    RawRoboUnit::proc( msg, subAx, para );
}

//! program
ProgramRawRoboUnit::ProgramRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
    mName = "ProgramRawRoboUnit";
}

void ProgramRawRoboUnit::proc( int msg, int subAx, int para )
{
    RawRoboUnit::proc( msg, subAx, para );
}

//! calcing
CalcingRawRoboUnit::CalcingRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
    mName = "CalcingRawRoboUnit";
}

void CalcingRawRoboUnit::proc( int msg, int subAx, int para )
{
    RawRoboUnit::proc( msg, subAx, para );
}

//! calcend
CalcendRawRoboUnit::CalcendRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
    mName = "CalcendRawRoboUnit";
}
void CalcendRawRoboUnit::proc( int msg, int subAx, int para )
{
    RawRoboUnit::proc( msg, subAx, para );
}
void CalcendRawRoboUnit::onEnter()
{
    selfFsm()->Robot()->switchPrepare();
}

//! standby
StandbyRawRoboUnit::StandbyRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
     mName = "StandbyRawRoboUnit";
}
void StandbyRawRoboUnit::proc( int msg, int subAx, int para )
{logDbg();
    //! ops
    if ( msg == MegaDevice::mrq_msg_run )
    {
//        selfFsm()->reqRun( true );

//        toState( MegaDevice::mrq_state_run_reqed );

        selfFsm()->Robot()->switchRun();
        toState( MegaDevice::mrq_state_prerun );
    }

    else
    {
        RawRoboUnit::proc( msg, subAx, para );
    }
}
void StandbyRawRoboUnit::onEnter()
{
    if ( selfFsm()->runReqed() )
    {
        selfFsm()->Robot()->switchRun();
        toState( MegaDevice::mrq_state_prerun );
    }
}
void StandbyRawRoboUnit::onExit()
{
    selfFsm()->reqRun( false );
}
//! pre run
PreRunRawRoboUnit::PreRunRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
    mName = "PreRunRawRoboUnit";
}

void PreRunRawRoboUnit::proc( int msg, int subAx, int para )
{
    RawRoboUnit::proc( msg, subAx, para );
}

//! running
RunningRawRoboUnit::RunningRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
    mName = "RunningRawRoboUnit";
}
void RunningRawRoboUnit::proc( int msg, int subAx, int para )
{
    RawRoboUnit::proc( msg, subAx, para );
}

//! pre stop
PreStopRawRoboUnit::PreStopRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members ) : RawRoboUnit( pFsm, members )
{
    mName = "PreStopRawRoboUnit";
}
void PreStopRawRoboUnit::proc( int msg, int subAx, int para )
{
    RawRoboUnit::proc( msg, subAx, para );
}
