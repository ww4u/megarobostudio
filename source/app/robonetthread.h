#ifndef ROBONETTHREAD_H
#define ROBONETTHREAD_H


#include "../com/robomsgthread.h"
#include "../com/robomsg.h"
#include "../model/mcmodel.h"

class RoboNetThread : public RoboMsgThread
{
    Q_OBJECT
public:
    RoboNetThread( QObject *pObj = 0 );

    void setMcModel( mcModel *pModel );

    virtual void onMsg( RoboMsg &msg );

Q_SIGNALS:
    void signal_net( const QString &name,
                     int axes,
                     RoboMsg msg
                     );

protected:
    mcModel *m_pModel;
};

#endif // ROBONETTHREAD_H
