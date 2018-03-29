
#include "interrupthandler.h"
#include "../../com/robomsg.h"

#include "../../sys/sysapi.h"

interruptHandler::interruptHandler( QObject *parent ) : QObject( parent )
{
    m_pInstMgr = NULL;
}

void interruptHandler::setInstMgr( MegaDevice::InstMgr *pInstMgr )
{
    Q_ASSERT( NULL != pInstMgr );

    m_pInstMgr = pInstMgr;
}

void interruptHandler::slot_event( eventId id,
                                   frameData data )
{
//    logDbg()<<id<<data.getFrameId()<<data.size()<<data;

    //! find device by event id
    Q_ASSERT( NULL != m_pInstMgr );
    VRobot *pRobo = m_pInstMgr->findRobotBySendId( data.getFrameId() );

    //! proc the interrupt
    if ( NULL != pRobo )
    { pRobo->interruptHandler( id, data ); }

    //! complex robot
    if( NULL != pRobo )
    {
        DeviceTree roboTree = m_pInstMgr->roboTree();
        foreach( VRoboList *pList, roboTree )
        {
            Q_ASSERT( NULL != pList );
            foreach( VRobot *pDev, *pList )
            {
                Q_ASSERT( NULL != pDev );

                pDev->interruptHandler( id, data, pRobo );
            }
        }
        delete_all( roboTree );
    }

    //! default proc
    sysQueue()->postMsg(
                          e_interrupt_occuring,
                          (int)id,
                          data.getFrameId(),
                          (QByteArray)data
                          );
//    logDbg();
}

interruptThread::interruptThread( QObject *parent ) : QThread( parent )
{
    mDefInterruptHandle.moveToThread( this );
}

void interruptThread::setInstMgr( MegaDevice::InstMgr *pInstMgr )
{
    Q_ASSERT( NULL != pInstMgr );

    mDefInterruptHandle.setInstMgr( pInstMgr );
}

void interruptThread::run()
{
//    setPriority( QThread::TimeCriticalPriority );

    QThread::run();
}

void interruptThread::connectInterrupt( receiveCache *pCache )
{
    Q_ASSERT( NULL != pCache );

    connect( pCache,
             SIGNAL(sig_event(eventId,frameData)),
             &mDefInterruptHandle,
             SLOT(slot_event( eventId,frameData)) );
}
