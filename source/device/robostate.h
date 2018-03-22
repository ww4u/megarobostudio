#ifndef ROBOSTATE_H
#define ROBOSTATE_H

#include <QtCore>
#include "../../com/robomsg.h"

namespace MegaDevice
{

class RoboStateUnit;

class RoboCondition
{
public:
    RoboCondition();

public:
    bool wait( int tmoms );
    virtual void trigger( void *pPara );

public:
    void set( int axes, int waits=1 );
    int Axes();

protected:
    int mAxes;
    int mWaits;

    QSemaphore mSema;
};

class RoboStateCondition : public RoboCondition
{
public:
    RoboStateCondition( int axes = 0, int stat = 0 );
public:
    virtual void trigger( void *pPara );

public:
    void setState( int stat );
    int State();

public:
    int mState;
};

class RoboFsm
{
public:
    RoboFsm();

public:
    virtual void build();

    virtual void init( RoboStateUnit *pState );

    virtual void proc( int msg, int para );
    virtual void proc( int msg, int para, int p2 );

    virtual void subscribe( RoboFsm *pMember, int msg, int para );

    virtual void toState( RoboStateUnit *pState );

    virtual void startTimer( int id=0, int tmous=1000 );
    virtual void killTimer( int id=0 );
    virtual void onTimer( int id );

public:
    void attachCondition( RoboCondition *pCond );
    void detachCondition();
    virtual bool waitCondition( RoboCondition *pCond, int tmoms );

public:
    void setLeader( RoboFsm *pLeader, void *pPara=0 );
    RoboFsm *leader();
    void *leaderPara();

    void setId( int id1, int id2  );
    int Id1();
    int Id2();

protected:
    RoboStateUnit *m_pNowState;
    RoboCondition *m_pCond;

protected:
    RoboFsm *m_pLeader;
    void *m_pLeaderPara;

    //! debug
    int mId1, mId2;
};

//! state unit
class RoboStateUnit
{
public:
    RoboStateUnit( RoboFsm *pFsm = NULL );
    virtual ~RoboStateUnit();
public:
    virtual void proc( int msg, int para );
    virtual void proc( int msg, int para1, int p2 );

    virtual void toState( RoboStateUnit *pState );

    virtual void onEnter();
    virtual void onExit();
    virtual void onRst();

    virtual void onTimer( int id );

public:
    void attachFsm( RoboFsm *pFsm );
    RoboFsm *Fsm();

    void setName( const QString &name );
    QString &name();

    void startTimer( int id=0, int tmous=1000 );
    void killTimer( int id=0 );

protected:
    RoboFsm *m_pFsm;
    QString mName;
};

}

#endif // ROBOSTATE_H
