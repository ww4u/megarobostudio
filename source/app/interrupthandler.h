#ifndef INTERRUPTHANDLER_H
#define INTERRUPTHANDLER_H

#include <QObject>

#include "../../bus/receivecache.h"
#include "../../inst/instmgr.h"

class interruptHandler : public QObject
{
    Q_OBJECT
public:
    interruptHandler( QObject *parent = NULL );
    void setInstMgr( MegaDevice::InstMgr *pInstMgr );

protected Q_SLOTS:
    void slot_event( eventId id, frameData data );

protected:
    MegaDevice::InstMgr *m_pInstMgr;

};

class interruptThread : public QThread
{
    Q_OBJECT
public:
    interruptThread( QObject *parent=0 );
    void setInstMgr( MegaDevice::InstMgr *pInstMgr );

protected:
    virtual void run();

public:
    void connectInterrupt( receiveCache *pCache );

protected:
    interruptHandler mDefInterruptHandle;

};

#endif // INTERRUPTHANDLER_H
