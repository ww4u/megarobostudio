
#include "systimer.h"

#include "../../device/vrobot.h"

roboTimer::roboTimer()
{
    mId = -1;
    mTmous = -1;
    mNowus = -1;

    m_pRobo = NULL;
    m_pContext = NULL;
}

void roboTimer::tick( int ms )
{
    Q_ASSERT( NULL != m_pRobo );

    if ( mNowus >= 0 )
    {
        mNowus += ms * 1000;        //! in us unit

        if ( mNowus >= mTmous )
        {
            mNowus = 0;

            m_pRobo->onTimer( m_pContext, mId );
        }
        else
        {}
    }
    else
    {}
}

void roboTimer::set( VRobot *pRobo,
                     void *pContext,
                     int id, int tmous )
{
    m_pRobo = pRobo;
    m_pContext = pContext;

    mId = id;
    mTmous = tmous;

    mNowus = -1;
}

int roboTimer::Id()
{ return mId; }
int roboTimer::Tmo()
{ return mTmous; }
int roboTimer::Now()
{ return mNowus; }
VRobot *roboTimer::Robot()
{ return m_pRobo; }
void *roboTimer::Context()
{ return m_pContext; }

void roboTimer::start( int tmous )
{
    mTmous = tmous;
    mNowus = 0;
}
void roboTimer::stop()
{
    mNowus = -1;
}

SysTimerThread * SysTimerThread::_sys_timer_ =  NULL;
QMutex SysTimerThread::mTimerMutex;

#define tick_ms     100

void SysTimerThread::sysStartTimer( VRobot *pRobot,
                               void *pContext,
                      int id,
                      int tmous )
{
    Q_ASSERT( NULL != pRobot );

    roboTimer *pTimer = sysFindTimer( pRobot, pContext, id );
    if ( NULL == pTimer )
    {
        pTimer = new roboTimer();
        Q_ASSERT( NULL != pTimer );

        pTimer->set( pRobot, pContext, id, tmous );

        SysTimerThread::mTimerMutex.lock();
        SysTimerThread::_sys_timer_->mTimers.append( pTimer );
        SysTimerThread::mTimerMutex.unlock();logDbg()<<id;
    }

    pTimer->start(tmous);logDbg()<<SysTimerThread::_sys_timer_->mTimers.size();
}
void SysTimerThread::sysStopTimer( VRobot *pRobot,
                              void *pContext,
                     int id )
{
    Q_ASSERT( NULL != pRobot );

    roboTimer *pTimer = sysFindTimer( pRobot, pContext, id );
    if ( NULL == pTimer )
    { }
    else
    {
        pTimer->stop();

        SysTimerThread::mTimerMutex.lock();

        SysTimerThread::_sys_timer_->mTimers.removeAll( pTimer );
        delete pTimer;

        SysTimerThread::mTimerMutex.unlock();logDbg()<<SysTimerThread::_sys_timer_->mTimers.size();
    }
}

roboTimer * SysTimerThread::sysFindTimer( VRobot *pRobot,
                                          void *pContext,
                                 int id )
{
    Q_ASSERT( NULL != pRobot );

    if( NULL == SysTimerThread::_sys_timer_ )
    { return NULL; }

    SysTimerThread::mTimerMutex.lock();

    foreach( roboTimer *theTimer, SysTimerThread::_sys_timer_->mTimers )
    {
        Q_ASSERT( NULL != theTimer );

        if ( (UINT_PTR)theTimer->Robot() == (UINT_PTR)pRobot
             && ( theTimer->Context() == pContext )
             && ( theTimer->Id() == id ) )
        {
            SysTimerThread::mTimerMutex.unlock();
            return theTimer;
        }
    }

    SysTimerThread::mTimerMutex.unlock();

    return NULL;
}

SysTimerThread::SysTimerThread( QObject *obj ) : QThread( obj )
{
//    mTickTimer.moveToThread( this );
    m_pTickTimer = new QTimer( );
    Q_ASSERT( NULL != m_pTickTimer );

    m_pTickTimer->moveToThread( this );

    Q_ASSERT( NULL == SysTimerThread::_sys_timer_ );
    SysTimerThread::_sys_timer_ = this;
}

SysTimerThread::~SysTimerThread()
{
//    mTickTimer.stop();

//    mTickTimer.moveToThread( qApp->thread() );

    qDeleteAll( mTimers );
}

void SysTimerThread::run()
{
    m_pTickTimer->connect( m_pTickTimer, SIGNAL(timeout()),
                        this, SLOT(slotTimeout()) );

    connect( this, SIGNAL(sigExit()),
             this, SLOT(slotExit()) );

    m_pTickTimer->start( tick_ms );

//    QThread::run();
    QThread::exec();

    m_pTickTimer->stop();
    delete m_pTickTimer;
    m_pTickTimer = NULL;
//    logDbg();
}

void SysTimerThread::slotTimeout()
{
//    logDbg();
    SysTimerThread::mTimerMutex.lock();

    //! slow down the interrupt buffer
    if ( receiveCache::availableEvent() > 64 )
    {
        sysLog( "pending many events " + QString::number(receiveCache::availableEvent()) );
    }
    else
    {
        foreach( roboTimer *theTimer, mTimers )
        {
            Q_ASSERT( NULL != theTimer );

            theTimer->tick( m_pTickTimer->interval() );
        }
    }

    SysTimerThread::mTimerMutex.unlock();
}

void SysTimerThread::slotExit()
{
//    m_pTickTimer->stop();

    QThread::exit();

//    logDbg();
}

void SysTimerThread::stopAll()
{
    SysTimerThread::mTimerMutex.lock();

    foreach( roboTimer *pTimer, mTimers )
    {
        Q_ASSERT( NULL != pTimer );
        pTimer->stop();
    }

    SysTimerThread::mTimerMutex.unlock();
}

