
#ifndef _ROBOT_MEGATRON_H_
#define _ROBOT_MEGATRON_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

#include "../../model/handactionmodel.h"

//! arith
#include "../../arith/pathplan/pathplan.h"
#include "../../arith/kinematic/kinematic.h"

class robotMegatron : public RawRobo
{
public:
    robotMegatron();
    ~robotMegatron();
public:
    DECLARE_SCPI()

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

public:
    virtual void onMsg( int subAxes, RoboMsg &msg );

    virtual int download( tpvGroup *pGroup, int axes = 0 );
    virtual int download( motionGroup *pGroup, int axes = 0 );

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints );      //! joint tab id

    virtual int download( VRobot *pSetup );

    virtual int run( int axes );
    virtual int stop( int axes );

    virtual int run( );
    virtual int stop( );

    virtual int setLoop( int n );
    virtual int loopNow();

    virtual void onLine();
    virtual void offLine();

    virtual void toState(int stat);

public:
    virtual QAbstractTableModel *handActions();

public:
    int move( WorldPoint &pt1,
              WorldPoint &pt2,
              float dt );
    int moveTest1();
    int moveTest2();

    int moveTest( WorldPoint &pt1, WorldPoint &pt2, float dt );

    int nowPose( WorldPoint &pos );

protected:
    int buildTrace( WorldPoint &pt1,
                    WorldPoint &pt2,
                    float dt,
                    xxxGroup<jointsTrace> &jointsPlan );

    int planTrace(  WorldPoint &pt1,
                    WorldPoint &pt2,
                    float dt,
                    xxxGroup<tracePoint> &tracePoints
                    );

    int splitTrace( xxxGroup<tracePoint> &tracePoints,
                    xxxGroup<jointsTrace> &traceJoints );

    int convertTrace(   WorldPoint &pt1,
                        WorldPoint &pt2,
                        float dt,
                        xxxGroup<jointsTrace> &jointsPlan );

    int downloadTrace( );
    int buildTpvGroup( xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &gp );

protected:
    handActionModel mHandActionModel;
    QList< tpvGroup *> mJointsGroup;
};

#endif
