
#ifndef _ROBOT_RAW_ROBO_H_
#define _ROBOT_RAW_ROBO_H_

#include <QMutex>

#include "../../device/vrobot.h"
#include "../../device/robostate.h"
#include "../../device/mrq/deviceMRQ.h"

#include "../../arith/pathplan/pathplan.h"

#define BIT_INTERP  0

struct D1Point
{
    union
    {
        struct
        {
            float t;
            float p;
            float v;
        };
        float datas[3];
    };

    D1Point( float pt = 0, float pp = 0, float pv=0 );
};
typedef QList<D1Point>    D1PointList;

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
    void clear();
};
typedef QList<TraceKeyPoint>    TraceKeyPointList;

struct MegatronKeyPoint
{
    union
    {
        struct
        {
            float t;
            float fx, ly, fz;   //! x1, y1, z1
            float bx, ry, bz;   //! x2, y2, z2
        };
        float datas[7];
    };

    MegatronKeyPoint( float pt = 0,
                      float fx=0, float ly=0, float fz=0,
                      float bx=0, float ry=0, float bz=0 );
};
typedef QList<MegatronKeyPoint>    MegatronKeyPointList;

struct H2KeyPoint
{
    union
    {
        struct
        {
            float t;
            float x, y, vx, vy;
        };

        float datas[5];
    };

    H2KeyPoint( float pt = 0,
                float px=0, float py=0, float pvx = 0, float pvy=0 );
};
typedef QList<H2KeyPoint>    H2KeyPointList;

struct H2ZKeyPoint
{
    union
    {
        struct
        {
            float t;
            float x, y, z, vx, vy, vz;
        };
        float datas[7];
    };

    H2ZKeyPoint( float pt = 0,
                 float px=0, float py=0, float pz = 0,
                 float pvx = 0, float pvy = 0, float pvz=0 );
};
typedef QList<H2ZKeyPoint>    H2ZKeyPointList;

//! ip
struct IPKeyPoint
{
    union
    {
        struct
        {
            float t;
            float x, y, vx,vy;
        };

        float datas[5];
    };

    IPKeyPoint( float pt = 0,
                float px=0, float py=0, float pvx = 0, float pvy=0 );
};
typedef QList<IPKeyPoint>    IPKeyPointList;

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
    void setState( int state = 0 );
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
//    QMutex mMutex( QMutex::Recursive );

    RawRobo *m_pRobot;

    QMap< int, RawRoboUnit *> mStateMap;
    int mState;

    bool mbRunReqed;
};

template<typename T>
class TpvFilter
{
protected:
    T mT, mP, mV;
    int mSize;
    int mIndex;

    bool mEn;
    T mOmit;

public:
    TpvFilter( int size, bool en, T omit )
    {
        mSize = size;
        mIndex = 0;

        mEn = en;
        mOmit = omit;
    }
public:
    bool filter( int index, T t , T p, T v )
    {
        if ( mEn )
        {}
        else
        { return false; }

        //! the first
        if ( index == 0 )
        {
            mT = t;
            mP = p;
            mV = v;

            return false;
        }
        //! the last
        else if ( index == mSize - 1 )
        {
            mT = t;
            mP = p;
            mV = v;
            return false;
        }
        else
        {
//            if ( qAbs( p - mP )/qAbs( t - mT ) > mOmit )
            if ( qAbs( t - mT ) < mOmit )
            { return true; }
            else
            {
                mT = t;
                mP = p;
                mV = v;
                return false;
            }
        }
    }
};

//! robo
class RawRobo : public VRobot
{
public:
    RawRobo();
    virtual ~RawRobo();

    virtual void postCtor();

public:
    virtual void onMsg( int subAxes, RoboMsg &msg );

public:
    virtual int run( const tpvRegion &region=0  );
    virtual int stop( const tpvRegion &region=0  );

    virtual int setLoop( int n, const tpvRegion &region=0 );

    //! switch operation
    virtual void switchReset( const tpvRegion &region=0 );
    virtual void switchStop( const tpvRegion &region=0 );
    virtual void switchRun(const tpvRegion &region=0 );
    virtual void switchPrepare( const tpvRegion &region=0 );
    virtual void switchEmergStop( const tpvRegion &region=0 );

    virtual void queryState( const tpvRegion &region=0 );

    virtual void toState( const tpvRegion &region, int stat );
    virtual int state( const tpvRegion &region=0, int inTask = 0 );

    virtual void onTimer( void *pContext, int id );

    virtual void onLine();
    virtual void offLine();
    virtual void offLine( const tpvRegion &region );
    virtual bool checkLink( int p1 = 0, int p2 = 0 );
    //! interface
    virtual int call( int n, const tpvRegion &region=0 );  //! load + run

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints,       //! joint tab id
                          const tpvRegion &region );


public:
    virtual void attachCondition(
                                  const tpvRegion &region,
                                  MegaDevice::RoboCondition *pCond );
    virtual bool waitCondition( const tpvRegion &region,
                                MegaDevice::RoboCondition *pCond,
                                int tmoms=-1 );

protected:
    int jMove( const tpvRegion &region,
                             int jTabId, bool bCcw,
                             float t, float p, float ev );

    int waitFsm( pvt_region,
                     int dstState,
                     int tmous,
                     int tick );
public:
    void sync( const tpvRegion &region=0 );

protected:
    int serialOutRaw( QXmlStreamWriter &writer );
    int serialInRaw( QXmlStreamReader &reader );


public:
    RawRoboFsm * fsm( const tpvRegion &region );

protected:
                                //! fsms for each region
    QMap< tpvRegion, RawRoboFsm*> mFsms;

                                //! pref
    QList< tpvGroup *> mJointsGroup;
    xxxGroup<tracePoint> mTracePlan;

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
    virtual void onEnter( RoboMsg &detail );
    virtual void onExit( RoboMsg &detail );

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
    void initState();

protected:
    QMap<int, int> mMemberStates;   //! mState = union( mMemberStates )
    int mState;

    int mMemberCount;
};

class UnkRawRoboUnit : public RawRoboUnit
{
public:
    UnkRawRoboUnit( MegaDevice::RoboFsm *pFsm,
                     int members );

public:
    virtual void proc( int msg, RoboMsg &detail );

//    virtual void onEnter( RoboMsg &detail );
//    virtual void onExit();
//    virtual void onRst();

protected:

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
