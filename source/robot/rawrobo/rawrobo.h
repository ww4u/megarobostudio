
#ifndef _ROBOT_RAW_ROBO_H_
#define _ROBOT_RAW_ROBO_H_

#include "../../device/vrobot.h"
#include "../../device/robostate.h"

enum eRoboPlanMode
{
    plan_linear = 0,
    plan_3rd,
    plan_5rd,
};

struct TraceKeyPoint
{
    union
    {
        struct
        {
            float t;
            float x, y, z;
            float hand;
        };
        float datas[5];
    };

    TraceKeyPoint( float pt = 0, float px=0, float py=0, float pz=0, float phand=0 );
};

typedef QList<TraceKeyPoint>    TraceKeyPointList;

class RawRoboStateCondition : public MegaDevice::RoboStateCondition
{
public:
    RawRoboStateCondition( int stat = 0 );
};

class RawRobo;
//! fsm
class RawRoboUnit;
class RawRoboFsm : public MegaDevice::RoboFsm
{
public:
    RawRoboFsm();
    ~RawRoboFsm();

public:
    virtual void build();

    virtual void subscribe( MegaDevice::RoboFsm *pMember,
                            int msg,
                            int para );

    virtual void toState( int stat );
    int state();

    virtual void startTimer( int id=0, int tmous=1000 );
    virtual void killTimer( int id=0 );
    virtual void onTimer( int id );

public:
    void attachRobot( RawRobo *pRobot );
    RawRobo *Robot();

    void reqRun( bool b );
    bool runReqed();

protected:
    QMutex mMutex;
    RawRobo *m_pRobot;

    QMap< int, RawRoboUnit *> mStateMap;
    int mState;

    bool mbRunReqed;
};

//! robo
class RawRobo : public VRobot
{
public:
    RawRobo();

public:
    //! switch operation
    virtual void switchReset();
    virtual void switchStop();
    virtual void switchRun();
    virtual void switchPrepare();
    virtual void switchEmergStop();

    virtual void queryState();

    virtual void toState( int stat );
    int state();

    virtual void onTimer( void *pContext, int id );

public:
    virtual void attachCondition(
                                  MegaDevice::RoboCondition *pCond );
    virtual bool waitCondition(
                                MegaDevice::RoboCondition *pCond,
                                int tmoms=-1 );

public:
    void setPlanStep( float step );
    float planStep();

    void setPlanMode( eRoboPlanMode mode );
    eRoboPlanMode getPlanMode();

protected:
    RawRoboFsm mFsm;
                                //! pref
    float mPlanStep;
    eRoboPlanMode mPlanMode;
};

//! base state
class RawRoboUnit : public MegaDevice::RoboStateUnit
{
public:
    RawRoboUnit( MegaDevice::RoboFsm *pFsm,
                 int members );
    virtual ~RawRoboUnit();
public:
    virtual void toState( int stat );

    virtual void proc( int msg, int subAx, int para );

    virtual void onEnter();
    virtual void onExit();

    virtual void onTimer( int id );
public:
    RawRoboFsm *selfFsm();

    void setMemberState( int subAx, int stat );
    int memberState( int subAx );

    int state();

protected:
    void rstState();

protected:
    QMap<int, int> mMemberStates;   //! mState = union( mMemberStates )
    int mState;

    int mMemberCount;
};

class IdleRawRoboUnit : public RawRoboUnit
{
public:
    IdleRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members );

public:
    virtual void proc( int msg, int subAx, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();

protected:

};

class RunReqedRawRoboUnit : public RawRoboUnit
{
public:
    RunReqedRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                         int members );

public:
    virtual void proc( int msg, int subAx, int para );

//    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class ProgramRawRoboUnit : public RawRoboUnit
{
public:
    ProgramRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
    virtual void proc( int msg, int subAx, int para );

//    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class CalcingRawRoboUnit : public RawRoboUnit
{
public:
    CalcingRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
    virtual void proc( int msg, int subAx, int para );

//    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class CalcendRawRoboUnit : public RawRoboUnit
{
public:
    CalcendRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
    virtual void proc( int msg, int subAx, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class StandbyRawRoboUnit : public RawRoboUnit
{
public:
    StandbyRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
    virtual void proc( int msg, int subAx, int para );

    virtual void onEnter();
    virtual void onExit();
//    virtual void onRst();
};

class PreRunRawRoboUnit : public RawRoboUnit
{
public:
    PreRunRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                       int members );

public:
    virtual void proc( int msg, int subAx, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class RunningRawRoboUnit : public RawRoboUnit
{
public:
    RunningRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
    virtual void proc( int msg, int subAx, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

class PreStopRawRoboUnit : public RawRoboUnit
{
public:
    PreStopRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
    virtual void proc( int msg, int subAx, int para );

    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

#endif
