
#ifndef _ROBOT_INJECT_H_
#define _ROBOT_INJECT_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

struct IJZeroArg : public RoboTaskArgument
{
    int mAx;                        //! 0 -- x
    float mZeroDist,mZeroTime, mZeroEndV;
    float mZeroVGapDist, mZeroVGapTime,
          mZeroIGapDist, mZeroIGapTime;

    IJZeroArg();
};

class IJTask : public RoboTask
{
    Q_OBJECT
public:
    IJTask( QObject *pParent = NULL );

protected:
    virtual void procRequest( RoboTaskRequest *pReq );
};

struct InjAction
{
    double mTime;
    double mDist;
};

struct InjProp
{
    int mDoors;
    InjAction mDoor;
    InjAction mDrink;
    InjAction mSpit;

    InjProp()
    {
        mDoors = 2;

        mDoor.mDist = 120;
        mDoor.mTime = 0.5;

        mDrink.mDist = 10;
        mDrink.mTime = 1;

        mSpit.mDist = 10;
        mSpit.mTime = 1;
    }
};

class robotInject : public RawRobo
{
public:
    typedef int (*apiIJRequest)( void *pArg );

public:
    robotInject();
    ~robotInject();
public:
    DECLARE_SCPI()

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

    virtual int download( VRobot *pSetup );

public:
    virtual int build( MegaTableModel *pModel,
                       xxxGroup<tracePoint> &tracePlan,
                       xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &tpvGroups,
                       QList< int> &sectionList );

public:
    int program( IPKeyPointList &curve,
                 const tpvRegion &region );
    int move( IPKeyPointList &curve,
              const tpvRegion &region );
    int move( float dx, float dy, float dt,
              float endVx, float endVy,
              const tpvRegion &region );

    int move ( float dx, float dxt,
               float dy, float dyt,
               const tpvRegion &region );

    int preMove( IPKeyPointList &curve,
                 const tpvRegion &region );

    //! jId -- 0: x
    virtual int goZero( const tpvRegion &region,
                        int jointId, bool bCcw );
    virtual int goZero( const tpvRegion &region=0 );      //! 1.x, 2.y

    int zeroAxesTask( void *pArg );

    //! action
public:
    int inj_move( const tpvRegion &region,
                  float dh, float dt );

    int valve_move( const tpvRegion &region,
                   float dv, float dt );
public:
    void setZeroAttr( double zeroTime,
                      double zeroDist );
    void zeroAttr( double &zeroTime, double &zeroDist );

    void setGap( double vtime, double vdist,
                 double itime, double idist );
    void gap( double &vtime, double &vdist,
              double &itime, double &idist );

    //! attr
    void setInjAttr( InjProp &prop );
    void injAttr( InjProp &prop );

    int serialOutZero( QXmlStreamWriter &writer);
    int serialInZero( QXmlStreamReader &reader );

    int serialOutProp( QXmlStreamWriter &writer);
    int serialInProp( QXmlStreamReader &reader );

protected:
    int verifyTrace( IPKeyPointList &curve );
    int buildTrace( IPKeyPointList &curve,
                    QList< tpvGroup *> &jointsGroup,
                    QList< int > &sectionList );
    int downloadTrace( const tpvRegion &region,
                       QList< tpvGroup *> &jointsGroup );

//    int waitFsm( pvt_region,
//                 int dstState,
//                 int tmous,
//                 int tick );

protected:
    double mZeroTime, mZeroDistance;
    double mVGapTime, mVGapDistance;
    double mIGapTime, mIGapDistance;

    //! attr
    InjProp mProp;

//    QList <int> mAxesDirs;      //! realX = dir * x + zeroX
    QList <int> mSuctionDirs;       //! 0, x; 1, y;

};

#endif
