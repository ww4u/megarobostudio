
#ifndef _ROBOT_SINANJU_H_
#define _ROBOT_SINANJU_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

//#include "../../model/handactionmodel.h"

//! arith
#include "../../arith/pathplan/pathplan.h"
#include "../../arith/sinanju_split/sinanju_split.h"

#include "../../model/sinanjumotiongroup.h"

class robotSinanju : public RawRobo
{
public:
    robotSinanju();
    ~robotSinanju();
public:
    DECLARE_SCPI()

public:
    virtual void onMsg( int subAxes, RoboMsg &msg );

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints,       //! joint tab id
                          const tpvRegion &region );

    virtual int download( VRobot *pSetup );

    virtual int run( const tpvRegion &region=0  );  //! just run
    virtual int stop( const tpvRegion &region=0 );  //! stop
    virtual int goZero();
    virtual int goZero( int jointId, bool bCcw );

    virtual bool checkZeroValid();
    virtual float getZero( int jointTabId );
    virtual int  setZero( int jointTabId, float zero );

    virtual int getPOSE( float pos[] );

    virtual int setLoop( int n, const tpvRegion &region=0 );
    virtual int loopNow();

//    virtual void onLine();
//    virtual void offLine();

    virtual void toState(int stat);

public:
//    virtual QAbstractTableModel *handActions();

    virtual int build( MegaTableModel *pModel,
                       xxxGroup<tracePoint> &tracePlan,
                       xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &tpvGroups );

public:
    int call( const tpvRegion &region=0 );  //! load + run
    int program( const QString &fileName,
                             const tpvRegion &region );
    int loadProgram( const QString &fileName );

    int program( QList<TraceKeyPoint> &curve,
                 const tpvRegion &region );
    int move( QList<TraceKeyPoint> &curve,
              const tpvRegion &region );

    int moveTest1( const tpvRegion &region=0 );
    int moveTest2( const tpvRegion &region=0 );

    int nowPose( TraceKeyPoint &pos );
    int nowAngle( float angles[] );
    //! ref to the joint angle
    int nowJointAngle( float angles[4] );

    int angleToPos( float angles[4],
                     TraceKeyPoint &pos );
    void diffAngle( float angles[4],
                    float anglesOut[4] );
    void diffAngle( float angles[4],
                    double anglesOut[4] );

    int nowDist( QList<float> &dists );



protected:
    int buildTrace( QList<TraceKeyPoint> &curve,
                    xxxGroup<tracePoint> &tracePlan,
                    xxxGroup<jointsTrace> &jointsPlan
                     );

    int verifyTrace( QList<TraceKeyPoint> &curve );

    void interpTune( QList<TraceKeyPoint> &curve );

    int planTrace( QList<TraceKeyPoint> &curve,
                   xxxGroup<tracePoint> &tracePoints );

    int splitTrace( xxxGroup<tracePoint> &tracePoints,
                    xxxGroup<jointsTrace> &traceJoints );

    int convertTrace(   QList<TraceKeyPoint> &curve,
                        xxxGroup<jointsTrace> &jointsPlan,
                        QList< tpvGroup *> &groups );

    int downloadTrace( const tpvRegion &region,
                       QList< tpvGroup *> &groups );
    int buildTpvGroup( xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &gp );

protected:
    int serialInAngle( QXmlStreamReader &reader );
    int serialOutAngle( QXmlStreamWriter &writer );

    int serialInHandZero( QXmlStreamReader &reader );
    int serialOutHandZero( QXmlStreamWriter &writer );

    int serialInArm( QXmlStreamReader &reader );
    int serialOutArm( QXmlStreamWriter &writer );

    int serialInInitPos( QXmlStreamReader &reader );
    int serialOutInitPos( QXmlStreamWriter &writer );

protected:
    void exportPlan( const QString &fileName, xxxGroup<tracePoint> &tracePlan );
    void exportJoints( const QString &fileName, xxxGroup<jointsTrace> &jointsPlan );

public:
    void setHandZeroAttr( double zeroTime, double zeroAngle, double zeroSpeed );
    void handZeroAttr( double &zeroTime, double &zeroAngle, double &zeroSpeed );

protected:
//    handActionModel mHandActionModel;

    double mHandZeroTime, mHandZeroAngle, mHandZeroSpeed;

};

#endif
