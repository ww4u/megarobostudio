#ifndef SYSTIMER_H
#define SYSTIMER_H

#include <QtCore>

class VRobot;
class roboTimer
{
public:
    roboTimer();

public:
    void tick( int ms );

    void set( VRobot *pRobo,
              void *pContext,
              int id, int tmous );
    int Id();
    int Tmo();
    int Now();

    VRobot *Robot();
    void *Context();

    void start( int tmous );
    void stop();

public:
    int mId;
    int mTmous;
    int mNowus;

    VRobot *m_pRobo;
    void *m_pContext;
};

class SysTimerThread : public QThread
{
    Q_OBJECT

public:
    static SysTimerThread *_sys_timer_;
    static QMutex mTimerMutex;

    static sysStartTimer( VRobot *pRobot,
                          void *pContext,
                          int id,
                          int tmous );
    static sysStopTimer( VRobot *pRobot,
                         void *pContext,
                         int id );

protected:
    static roboTimer * sysFindTimer( VRobot *pRobot,
                                     void *pContext,
                                     int id );
public:
    SysTimerThread( QObject *obj = 0 );
    ~SysTimerThread();

protected:
    virtual void run();

Q_SIGNALS:
    void sigExit();

protected Q_SLOTS:
    void slotTimeout();
    void slotExit();

private:
    QTimer *m_pTickTimer;

    QList< roboTimer * > mTimers;

};


#endif // SYSTIMER_H
