#ifndef ROBOMSGTHREAD_H
#define ROBOMSGTHREAD_H


#include <QtCore>
#include "robomsgqueue.h"

class RoboMsgThread : public QThread, public RoboMsgQueue
{
    Q_OBJECT
public:
    RoboMsgThread( QObject *pObj = 0  );

    virtual void onMsg( RoboMsg &msg );

protected:
    virtual void run();

public:
    void _setInterval( int us );
    int interval();

protected:
    int mIntervalus;
};

#endif // ROBOMSGTHREAD_H
