
#ifndef _ROBOT_RAW_ROBO_H_
#define _ROBOT_RAW_ROBO_H_

#include "../../device/vrobot.h"
#include "../../device/robostate.h"

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

public:
    void attachRobot( RawRobo *pRobot );
    RawRobo *Robot();

    void reqRun( bool b );
    bool runReqed();

protected:
    QMutex mMutex;
    RawRobo *m_pRobot;

    QMap< int, RawRoboUnit *> mStateMap;

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

public:
    virtual void attachCondition(
                                  MegaDevice::RoboCondition *pCond );
    virtual bool waitCondition(
                                MegaDevice::RoboCondition *pCond,
                                int tmoms=-1 );
protected:
    RawRoboFsm mFsm;
};

//! base state
class RawRoboUnit : public MegaDevice::RoboStateUnit
{
public:
    RawRoboUnit( MegaDevice::RoboFsm *pFsm,
                 int members );

public:
    virtual void toState( int stat );

    virtual void proc( int msg, int subAx, int para );

    virtual void onEnter();
    virtual void onExit();

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

//    virtual void onEnter();
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

//    virtual void onEnter();
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

//    virtual void onEnter();
//    virtual void onExit();
//    virtual void onRst();
};

#endif
