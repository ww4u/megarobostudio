
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
    //! find device by event id
    Q_ASSERT( NULL != m_pInstMgr );
    VRobot *pRobo = m_pInstMgr->findRobotBySendId( data.frameId(), data.devId() );

    //! proc the interrupt
    if ( NULL != pRobo )
    {
        logDbg()<<pRobo->name();
        pRobo->interruptHandler( id, data );
    }

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
                          (int)id,              //! event id
                          (int)data.devId(),    //! device id
                           data.frameId(),      //! frame id
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
    QThread::run();
}

void interruptThread::connectInterrupt( MegaDevice::INTRThread *pINTR )
{
    Q_ASSERT( NULL != pINTR );

    connect( pINTR,
             SIGNAL(sig_event(eventId,frameData)),
             &mDefInterruptHandle,
             SLOT(slot_event( eventId,frameData)),
             Qt::QueuedConnection );
}
