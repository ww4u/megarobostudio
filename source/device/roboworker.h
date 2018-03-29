#ifndef ROBOTWORKER_H
#define ROBOTWORKER_H

#include <QtCore>

#include "../com/robomsgthread.h"

class VRobot;

class RoboWorker : public RoboMsgThread
{
    Q_OBJECT
public:
    RoboWorker( VRobot *pRobo=NULL, QObject *pObj=0 );

public:
    virtual void onMsg( RoboMsg &msg );

public:
    void attachRobot( VRobot *pRobot, int subAxes );

protected:
    virtual void run();

protected:
    VRobot *m_pRobot;
    int mSubAxes;
};

#endif // ROBOTWORKER_H
