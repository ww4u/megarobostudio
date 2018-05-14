
#include "rawrobo.h"
#include "../../device/mrq/devicemrq_state.h"
#include "../../device/board/_MRQ_enum.h"

RawRoboFsm::RawRoboFsm()
{
    m_pRobot = NULL;

    mbRunReqed = false;
    mState = -1;

}
RawRoboFsm::~RawRoboFsm()
{
    foreach (RawRoboUnit * value, mStateMap )
    {
        Q_ASSERT( NULL != value );
        delete value;
    }

    mStateMap.clear();

    delete m_pMutex;
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

void RawRoboFsm::subscribe( MegaDevice::RoboFsm *pMember,   //! child
                            int msg,
                            int stat,
                            RoboMsg &detail )               //! from leaf
{
    Q_ASSERT( NULL != pMember );

    Q_ASSERT( Robot() != NULL );
    Q_ASSERT( Robot()->lpc() != NULL );

logDbg()<<QString::number( (UINT_PTR)pMember, 16 )<<(UINT_PTR)pMember->leaderPara();

    //! subax, region, state
    //! \note to robot, not robt.ax
    Robot()->lpc()->postMsg( (eRoboMsg)msg,
                             (qint32)(UINT_PTR)pMember->leaderPara(),
//                             tpvRegion( pMember->axes(), pMember->page() ),
                             pMember->region(),
                             stat
                             );
}

void RawRoboFsm::toState( int stat, RoboMsg &detail )
{
    if ( mStateMap.contains( stat ) )
    {
        Q_ASSERT( mStateMap[stat] != NULL );

        lockState();
        RoboFsm::toState( mStateMap[stat], detail );
        mState = stat;
//        sysLog( __FUNCTION__, QString::number(axes()), QString::number( page() ), QString::number(stat) );
        unlockState();

        //! subscribe to leader
        if ( m_pLeader != NULL )
        {
            m_pLeader->subscribe( this,
                                  e_robot_member_state,
                                  stat,
                                  detail );
        }

        //! trigger condition
        if ( m_pCond != NULL )
        { m_pCond->trigger( (void*)stat ); logDbg()<<stat<<m_pNowState->name(); }

        Q_ASSERT( NULL != m_pRobot );
        m_pRobot->toState( *this, stat );
    }
    else
    { Q_ASSERT(false); }
}

void RawRoboFsm::setState( int state  )
{
    lockState();
        Q_ASSERT( mStateMap.contains( state ) );
        Q_ASSERT( mStateMap[state] != NULL );

        m_pNowState = mStateMap[state];
        mState = state;
//        sysLog( __FUNCTION__, QString::number(axes()), QString::number( page() ), QString::number(state) );
    unlockState();
}
int RawRoboFsm::state()
{ return mState; }

void RawRoboFsm::startTimer( int id, int tmous )
{
    Q_ASSERT( NULL != m_pRobot );
    m_pRobot->startTimer( this, id, tmous );
}
void RawRoboFsm::killTimer( int id )
{
    Q_ASSERT( NULL != m_pRobot );
    m_pRobot->killTimer( this, id );
}
void RawRoboFsm::onTimer( int id )
{
    if ( mStateMap.contains( mState ) )
    {
        mStateMap.value( mState )->onTimer( id );
    }
}

void RawRoboFsm::attachRobot( RawRobo *pRobot )
{
    Q_ASSERT( NULL != pRobot );
    m_pRobot = pRobot;
}
RawRobo *RawRoboFsm::Robot()
{
    Q_ASSERT( NULL != m_pRobot );
    return m_pRobot;
}

void RawRoboFsm::reqRun( bool b )
{ mbRunReqed = b; }
bool RawRoboFsm::runReqed()
{ return mbRunReqed; }


