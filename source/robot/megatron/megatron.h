
#ifndef _ROBOT_MEGATRON_H_
#define _ROBOT_MEGATRON_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

//! arith
#include "../../arith/megatron_split/megatron_split.h"

struct MegatronZeroArg : public RoboTaskArgument
{
    QList<int> mJList;
    QList<bool > mCcwList;

    MegatronZeroArg();
};

class MegatronTask : public RoboTask
{
    Q_OBJECT
public:
    MegatronTask( QObject *pParent = NULL );
protected:
    virtual void procRequest(RoboTaskRequest *pReq);
};

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

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints,       //! joint tab id
                          const tpvRegion &region );

    virtual int download( VRobot *pSetup );

public:
    virtual int build( MegaTableModel *pModel,
                       xxxGroup<tracePoint> &tracePlan,
                       xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &tpvGroups,
                       QList< int> &sectionList );


public:
//    virtual int run( const tpvRegion &region=0  );
//    virtual int stop( const tpvRegion &region=0  );

    virtual int goZero( const tpvRegion &region=0 );
    virtual int goZero( const tpvRegion &region,
                        int jTabId, bool bCcw );
    virtual int goZero( const tpvRegion &region,
                        const QList<int> &jointList,
                        const QList<bool> &ccwList );

    virtual int loopNow();

    virtual void toState(int stat);

public:
    virtual int call( int n, const tpvRegion &region=0 );  //! load + run
    int program( QList<MegatronKeyPoint> &curve,
                 const tpvRegion &region );

    int move( QList<MegatronKeyPoint> &curve,
              const tpvRegion &region );

    int preMove( QList<MegatronKeyPoint> &curve,
              const tpvRegion &region );

    int moveTest1();
    int moveTest2();

public:
    int zeroAxesTask( void *pArg );

public:
    void setZeroAttr( double zeroTime, double zeroAngle );
    void zeroAttr( double &zeroTime, double &zeroAngle );

    void setGapAttr( double gapTime, double gapDist );
    void gapAttr( double &gapTime, double &gapDist );

    int serialOutZero( QXmlStreamWriter &writer);
    int serialInZero( QXmlStreamReader &reader );

protected:
    int buildTrace( QList<MegatronKeyPoint> &curve,
                    QList<tpvGroup *> &jointGroup,
                    QList<int> &secList );

    int downloadTrace( const tpvRegion &region );

protected:
    double mZeroTime, mZeroAngle;

    double mGapTime, mGapDistance, mGapSpeed;
};

#endif
