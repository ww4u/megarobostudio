
#ifndef _ROBOT_H2Z_H_
#define _ROBOT_H2Z_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

#include "../../model/handactionmodel.h"

//! arith
#include "../../arith/pathplan/pathplan.h"

#include "../h2/h2.h"

struct H2ZZeroArg : public RoboTaskArgument
{
    int mAx;                        //! 0 -- x
    float mZeroDist,mZeroTime, mZeroEndV;
    float mZeroGapDist, mZeroGapTime;

    H2ZZeroArg();
};

class H2ZTask : public RoboTask
{
    Q_OBJECT
public:
    H2ZTask( QObject *pParent = NULL );

protected:
    virtual void run();
};

class robotH2Z : public robotH2
{
public:
    typedef int (*apiH2Request)( void *pArg );

public:
    robotH2Z();
    ~robotH2Z();
public:
    DECLARE_SCPI()


public:
    virtual void setJointZeroCcw( int jId, bool b );

public:
    virtual int build( MegaTableModel *pModel,
                       xxxGroup<tracePoint> &tracePlan,
                       xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &tpvGroups,
                       QList< int> &sectionList );

public:
    int program( QList<H2ZKeyPoint> &curve,
                 const tpvRegion &region );
    int move( QList<H2ZKeyPoint> &curve,
              const tpvRegion &region );
    int move( float x, float y, float z,
              float dx, float dy, float dz,
              float dt,
              float endVx,float endVy, float endVz,
              const tpvRegion &region );

    int preMove( QList<H2ZKeyPoint> &curve,
              const tpvRegion &region );

    //! jId -- 0: x
    virtual int goZero( const tpvRegion &region,
                        int jointId, bool bCcw );
    virtual int goZero( const tpvRegion &region=0 );      //! 1.x, 2.y, 3.z
    virtual int goZero( const tpvRegion &region,
                        const QList<int> &jointList,
                        const QList<bool> &ccwList );

    virtual int getPOSE(float pos[]);

    int zeroAxesTask( void *pArg );

    int rstZeroAngle();
//    int angle( int jId, float &fAng );
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

//    int serialOutZero( QXmlStreamWriter &writer);
//    int serialInZero( QXmlStreamReader &reader );

//    int serialOutArm( QXmlStreamWriter &writer);
//    int serialInArm( QXmlStreamReader &reader);

protected:
    int verifyTrace( QList<H2ZKeyPoint> &curve );
    int buildTrace( QList<H2ZKeyPoint> &curve,
                    QList< tpvGroup *> &jointsGroup,
                    QList< int > &sectionList );
    int downloadTrace( const tpvRegion &region,
                       QList< tpvGroup *> &jointsGroup );

//    int waitFsm( pvt_region,
//                 int dstState,
//                 int tmous,
//                 int tick );

protected:
//    double mZeroTime, mZeroDistance;
//    double mZeroX, mZeroY;
//    double mGapTime, mGapDistance;

//    QList <int> mAxesDirs;      //! realX = dir * x + zeroX

//    QList<int> mEncoderDirs;    //! 1 or -1
//    int mLines;
};

#endif
