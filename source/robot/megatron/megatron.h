
#ifndef _ROBOT_MEGATRON_H_
#define _ROBOT_MEGATRON_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

//#include "../../model/handactionmodel.h"

//! arith
#include "../../arith/megatron_split/megatron_split.h"

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

//    virtual int download( tpvGroup *pGroup, int axes = 0 );
//    virtual int download( motionGroup *pGroup, int axes = 0 );

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints,       //! joint tab id
                          const tpvRegion &region );

    virtual int download( VRobot *pSetup );

    virtual int run( const tpvRegion &region=0  );
    virtual int stop( const tpvRegion &region=0  );
    virtual int goZero( int jTabId, bool bCcw );
    virtual int goZero( const QList<int> &jointList,
                        const QList<bool> &ccwList );
//    virtual int run( int axes );
//    virtual int stop( int axes );

//    virtual int run( );
//    virtual int stop( );

    virtual int setLoop( int n, const tpvRegion &region=0 );
    virtual int loopNow();

    virtual void onLine();
    virtual void offLine();

    virtual void toState(int stat);

public:    

public:
    virtual int call( const tpvRegion &region=0 );  //! load + run
    int program( QList<MegatronKeyPoint> &curve,
                 const tpvRegion &region );

    int move( QList<MegatronKeyPoint> &curve,
              const tpvRegion &region );

    int moveTest1();
    int moveTest2();

public:
    void setZeroAttr( double zeroTime, double zeroAngle, double zeroSpeed );
    void zeroAttr( double &zeroTime, double &zeroAngle, double &zeroSpeed );

    int serialOutZero( QXmlStreamWriter &writer);
    int serialInZero( QXmlStreamReader &reader );

protected:
    int buildTrace( QList<MegatronKeyPoint> &curve );

    int downloadTrace( const tpvRegion &region );

protected:
    double mZeroTime, mZeroAngle;
};

#endif
