
#ifndef _ROBOT_H2_H_
#define _ROBOT_H2_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

#include "../../model/handactionmodel.h"

//! arith
#include "../../arith/pathplan/pathplan.h"
//#include "../../arith/kinematic/kinematic.h"

struct H2ZeroArg : public RoboTaskArgument
{
    int mAx;                        //! 0 -- x
    float mZeroDist,mZeroTime, mZeroEndV;
    float mZeroGapDist, mZeroGapTime;

    H2ZeroArg();
};

class H2Task : public RoboTask
{
    Q_OBJECT
public:
    H2Task( QObject *pParent = NULL );

protected:
    virtual void run();
};

class robotH2 : public RawRobo
{
public:
    typedef int (*apiH2Request)( void *pArg );

public:
    robotH2();
    ~robotH2();
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
    int program( QList<H2KeyPoint> &curve,
                 const tpvRegion &region );
    int move( QList<H2KeyPoint> &curve,
              const tpvRegion &region );
    int move( float dx, float dy, float dt, float endV,
              const tpvRegion &region );

    int preMove( QList<H2KeyPoint> &curve,
              const tpvRegion &region );

    //! jId -- 0: x
    virtual int goZero( int jointId, bool bCcw );
    virtual int goZero( );      //! 1.x, 2.y

    int zeroAxesTask( void *pArg );

    int angle( int jId, float &fAng );
    int pose( float &x, float &y );

    int moveTest1( const tpvRegion &region=0 );
    int moveTest2( const tpvRegion &region=0 );

public:
    void setZeroAttr( double zeroTime,
                      double zeroDist );
    void zeroAttr( double &zeroTime, double &zeroDist );

    void setGap( double time, double dist );
    void gap( double &time, double &dist );

    void setCenter( float x, float y );
    void center( float &x, float &y );

    int serialOutZero( QXmlStreamWriter &writer);
    int serialInZero( QXmlStreamReader &reader );

    int serialOutArm( QXmlStreamWriter &writer);
    int serialInArm( QXmlStreamReader &reader);

protected:
    int verifyTrace( QList<H2KeyPoint> &curve );
    int buildTrace( QList<H2KeyPoint> &curve,
                    QList< tpvGroup *> &jointsGroup,
                    QList< int > &sectionList );
    int downloadTrace( const tpvRegion &region,
                       QList< tpvGroup *> &jointsGroup );

    int waitFsm( pvt_region,
                 int dstState,
                 int tmous,
                 int tick );

protected:
    double mZeroTime, mZeroDistance;
    double mZeroX, mZeroY;
    double mGapTime, mGapDistance;

    QList <int> mAxesDirs;      //! realX = dir * x + zeroX

    QList<int> mEncoderDirs;    //! 1 or -1
    int mLines;
};

#endif
