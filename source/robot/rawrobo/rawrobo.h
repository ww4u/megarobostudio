
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

#define BIT_INTERP  0


struct TraceKeyPoint
{
    union
    {
        struct
        {
            float t;
            float x, y, z;
            float hand;
            quint32 iMask;
        };
        float datas[6];
    };

    TraceKeyPoint( float pt = 0, float px=0, float py=0, float pz=0, float phand=0 );
};

typedef QList<TraceKeyPoint>    TraceKeyPointList;

struct MegatronKeyPoint
{
    union
    {
        struct
        {
            float t;
            float x1, y1, z1;
            float x2, y2, z2;
        };
        float datas[7];
    };

    MegatronKeyPoint( float pt = 0,
                      float px1=0, float py1=0, float pz1=0,
                      float px2=0, float py2=0, float pz2=0 );
};

typedef QList<MegatronKeyPoint>    MegatronKeyPointList;

class RawRoboStateCondition : public MegaDevice::RoboStateCondition
{
public:
    RawRoboStateCondition( int stat = 0 );
};

class RawRobo;
class RawRoboUnit;
//! fsm
class RawRoboFsm : public MegaDevice::RoboFsm
{
public:
    RawRoboFsm();
    virtual ~RawRoboFsm();

public:
    virtual void build();

    virtual void subscribe( MegaDevice::RoboFsm *pMember,
                            int msg,
                            int stat,
                            RoboMsg &detail );

    virtual void toState( int stat, RoboMsg &detail );
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
    virtual ~RawRobo();

    virtual void postCtor();

public:
    //! switch operation
    virtual void switchReset( const tpvRegion &region=0 );
    virtual void switchStop( const tpvRegion &region=0 );
    virtual void switchRun(const tpvRegion &region=0 );
    virtual void switchPrepare( const tpvRegion &region=0 );
    virtual void switchEmergStop( const tpvRegion &region=0 );

    virtual void queryState( const tpvRegion &region=0 );

    virtual void toState( const tpvRegion &region, int stat );
    int state( const tpvRegion &region=0 );

    virtual void onTimer( void *pContext, int id );

public:
    virtual void attachCondition(
                                  const tpvRegion &region,
                                  MegaDevice::RoboCondition *pCond );
    virtual bool waitCondition( const tpvRegion &region,
                                MegaDevice::RoboCondition *pCond,
                                int tmoms=-1 );

public:
    void setPlanStep( float step );
    float planStep();

    void setPlanMode( eRoboPlanMode mode );
    eRoboPlanMode getPlanMode();

    RawRoboFsm * fsm( const tpvRegion &region );

protected:
//    RawRoboFsm mFsm;
                                //! fsms for each region
    QMap< tpvRegion, RawRoboFsm*> mFsms;

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
    virtual void toState( int stat, RoboMsg &detail );

    virtual void proc( int msg, RoboMsg &detail );

    virtual void onTimer( int id );
public:
    RawRoboFsm *selfFsm();

    void setMemberState( int subAx,
                         const tpvRegion &region,
                         int stat,
                         RoboMsg &detail );
    int memberState( int subAx,
                     const tpvRegion &region );

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
    virtual void proc( int msg, RoboMsg &detail );

    virtual void onEnter( RoboMsg &detail );
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
//    virtual void proc( int msg, RoboMsg &detail );

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
//    virtual void proc( int msg, RoboMsg &detail );

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
//    virtual void proc( int msg, RoboMsg &detail );

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
    virtual void proc( int msg, RoboMsg &detail );

    virtual void onEnter( RoboMsg &detail );
//    virtual void onExit();
//    virtual void onRst();
};

class StandbyRawRoboUnit : public RawRoboUnit
{
public:
    StandbyRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
    virtual void proc( int msg, RoboMsg &detail );

    virtual void onEnter( RoboMsg &detail );
    virtual void onExit( RoboMsg &detail );
//    virtual void onRst();
};

class PreRunRawRoboUnit : public RawRoboUnit
{
public:
    PreRunRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                       int members );

public:
//    virtual void proc( int msg, RoboMsg &detail );

    virtual void onEnter( RoboMsg &detail );
//    virtual void onExit();
//    virtual void onRst();
};

class RunningRawRoboUnit : public RawRoboUnit
{
public:
    RunningRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
//    virtual void proc( int msg, RoboMsg &detail );

    virtual void onEnter( RoboMsg &detail );
//    virtual void onExit();
//    virtual void onRst();
};

class PreStopRawRoboUnit : public RawRoboUnit
{
public:
    PreStopRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                        int members );

public:
//    virtual void proc( int msg, RoboMsg &detail );

    virtual void onEnter( RoboMsg &detail );
//    virtual void onExit();
//    virtual void onRst();
};

#endif
