#include "../../include/mcstd.h"
#include "robostate.h"

namespace MegaDevice {

//! state condition
RoboStateCondition::RoboStateCondition( int axes, int stat )
{
    set( axes );

    mState = stat;
}

void RoboStateCondition::trigger( void *pPara )
{
    int stat = int(pPara);

    if ( stat == mState )
    { mSema.release();logDbg()<<stat; }
    else
    { logDbg()<<stat; }
}

void RoboStateCondition::setState( int stat )
{ mState = stat; }

int RoboStateCondition::State()
{ return mState; }

//! RoboFsm
RoboFsm::RoboFsm()
{
    m_pNowState = NULL;
    m_pCond = NULL;

    m_pLeader = NULL;
    m_pLeaderPara = NULL;

    mId1 = 0;
    mId2 = 0;
}

void RoboFsm::build()
{}

void RoboFsm::init( RoboStateUnit *pState )
{
    Q_ASSERT( NULL != pState );

    m_pNowState = pState;

    m_pNowState->onEnter();
}

void RoboFsm::proc( int msg, int para )
{
    Q_ASSERT( NULL != m_pNowState );

    m_pNowState->proc( msg, para );
}

void RoboFsm::proc( int msg, int para, int p2 )
{
    Q_ASSERT( NULL != m_pNowState );

    m_pNowState->proc( msg, para, p2 );
}

void RoboFsm::subscribe( RoboFsm *pMember, int msg, int para )
{
    if ( NULL != m_pLeader )
    { m_pLeader->subscribe( pMember, msg, para ); }
    else
    { logWarning(); }
}

void RoboFsm::toState( RoboStateUnit *pState )
{
    Q_ASSERT( NULL != pState );

    //! in state now
    if ( m_pNowState != pState )
    {
        m_pNowState->onExit();logDbg()<<m_pNowState->name()<<Id1()<<Id2()<<"exit";

        m_pNowState = pState;

        m_pNowState->onEnter();logDbg()<<m_pNowState->name()<<Id1()<<Id2()<<"enter";
    }
    else
    {
        logDbg()<<m_pNowState->name();
//        m_pNowState->onEnter();
    }
}

void RoboFsm::startTimer( int id, int tmous )
{
}
void RoboFsm::killTimer( int id )
{}

void RoboFsm::attachCondition( RoboCondition *pCond )
{
    Q_ASSERT( NULL != pCond );
    m_pCond = pCond;
}

void RoboFsm::detachCondition()
{
    m_pCond = NULL;
}

//! \note split the attach and wait as the event can be triggered
//! during the working process
bool RoboFsm::waitCondition( RoboCondition *pCond, int tmoms )
{
//    attachCondition( pCond );

    bool bRet;
    if ( NULL != pCond )
    { bRet = pCond->wait( tmoms ); }
    else
    { bRet = false; }

    detachCondition();

    return bRet;
}

void RoboFsm::setLeader( RoboFsm *pLeader, void *pPara )
{
    //! can be null
    m_pLeader = pLeader;
    m_pLeaderPara = pPara;
}
RoboFsm *RoboFsm::leader()
{ return m_pLeader; }
void *RoboFsm::leaderPara()
{ return m_pLeaderPara; }

void RoboFsm::setId( int id1, int id2  )
{
    mId1 = id1;
    mId2 = id2;
}
int RoboFsm::Id1()
{ return mId1; }
int RoboFsm::Id2()
{ return mId2; }

//! state unit
RoboStateUnit::RoboStateUnit( RoboFsm *pFsm )
{
    m_pFsm = pFsm;
}

void RoboStateUnit::proc( int msg, int para )
{}
void RoboStateUnit::proc( int msg, int para1, int p2 )
{}
void RoboStateUnit::toState( RoboStateUnit *pState )
{
    Q_ASSERT( NULL != pState );
    Q_ASSERT( NULL != Fsm() );

    Fsm()->toState( pState );
}

void RoboStateUnit::onEnter()
{}
void RoboStateUnit::onExit()
{}
void RoboStateUnit::onRst()
{}

void RoboStateUnit::onTimer( int id )
{}

void RoboStateUnit::attachFsm( RoboFsm *pFsm )
{
    Q_ASSERT( NULL != pFsm );

    m_pFsm = pFsm;
}
RoboFsm *RoboStateUnit::Fsm()
{
    Q_ASSERT( NULL != m_pFsm );
    return m_pFsm;
}

void RoboStateUnit::setName( const QString &name )
{ mName = name; }

QString &RoboStateUnit::name()
{ return mName; }

void RoboStateUnit::startTimer( int id, int tmous )
{
    Q_ASSERT( NULL != m_pFsm );

    m_pFsm->startTimer( id, tmous );
}
void RoboStateUnit::killTimer( int id )
{
    Q_ASSERT( NULL != m_pFsm );

    m_pFsm->killTimer( id );
}

RoboCondition::RoboCondition()
{
    mAxes = 0;
    mWaits = 1;
}

bool RoboCondition::wait( int tmoms )
{
    return mSema.tryAcquire( mWaits, tmoms );
}

void RoboCondition::trigger( void *pPara )
{
}

void RoboCondition::set( int axes,int waits )
{
    mAxes = axes;
    mWaits = waits;
}

int RoboCondition::Axes()
{ return mAxes; }

}
