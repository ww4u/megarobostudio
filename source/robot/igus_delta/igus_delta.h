
#ifndef _ROBOT_IGUS_DELTA_H_
#define _ROBOT_IGUS_DELTA_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

#include "../../arith/pathplan/pathplan.h"
#include "../../arith/delta/arith_delta.h"

struct IgusDeltaZeroArg : public RoboTaskArgument
{
    QList<int> mJList;
    QList<bool > mCcwList;

    QList<float> mZDistList;
    QList<float> mZVList;
    QList<float> mGapDistList;

    IgusDeltaZeroArg();
};

class IgusDeltaTask : public RoboTask
{
    Q_OBJECT
public:
    IgusDeltaTask( QObject *pParent = NULL );
protected:
    virtual void run();
};

class robotIgusDelta : public RawRobo
{
public:
    robotIgusDelta();

public:
    DECLARE_SCPI()

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

public:
    virtual int build( MegaTableModel *pModel,
                       xxxGroup<tracePoint> &tracePlan,
                       xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &tpvGroups,
                       QList< int> &sectionList );

public:
    virtual int goZero( const tpvRegion &region=0 );
    virtual int goZero( const tpvRegion &region, int jTabId, bool bCcw );
    virtual int goZero( const tpvRegion &region,
                        const QList<int> &jointList,
                        const QList<bool> &ccwList );  

    virtual int getPOSE(float pos[]);

    int zMove( const tpvRegion &region,
               int jId,
               float t, float p, float ev );

    int zeroAxesTask( void *pArg );
    int onZArg( void *pArg );

    //! plan
public:
    int verifyTrace( QList<TraceKeyPoint> &curve );

    int buildTrace( QList<TraceKeyPoint> &curve,
                    QList<deltaPoint> &jointsPlan );

    int planTrace( QList<TraceKeyPoint> &curve,
                   xxxGroup<tracePoint> &tracePoints );

    int splitTrace( xxxGroup<tracePoint> &tracePoints,
                    QList<deltaPoint> &traceJoints );

    int convertTrace(   QList<TraceKeyPoint> &curve,
                        QList<deltaPoint> &jointsPlan,
                        QList< tpvGroup *> &gp,
                        QList< int > &sectionList );

    int buildTpvGroup( QList<TraceKeyPoint> &curve,
                       QList<deltaPoint> &jointsPlan,
                       QList< tpvGroup *> &gp );
    //! download
protected:
    int downloadTrace( const tpvRegion &region );

public:
    int program( const QString &file,
                 const tpvRegion &region );
    int loadProgram( const QString &file );

    int program( QList<TraceKeyPoint> &curve,
                 const tpvRegion &region );
    int move( QList<TraceKeyPoint> &curve,
              const tpvRegion &region );

    int preMove( QList<TraceKeyPoint> &curve,
              const tpvRegion &region );

    int moveTest1( const tpvRegion &region=0 );
    int moveTest2( const tpvRegion &region=0 );

protected:
    int pose( float xyz[3] );

public:
    void setZeroAttr( double zeroTime, double zeroAngle, int zMode );
    void zeroAttr( double &zeroTime, double &zeroAnglem, int &zMode );

    void setInitAttr( double t, double x, double h );
    void initAttr( double &t, double &x, double &h );

    int serialOutZero( QXmlStreamWriter &writer);
    int serialInZero( QXmlStreamReader &reader );

    int serialOutInit( QXmlStreamWriter &writer);
    int serialInInit( QXmlStreamReader &reader );

    int serialOutArm( QXmlStreamWriter &writer);
    int serialInArm( QXmlStreamReader &reader );

    int serialOutOffset( QXmlStreamWriter &writer);
    int serialInOffset( QXmlStreamReader &reader );

    int serialOutP0( QXmlStreamWriter &writer);
    int serialInP0( QXmlStreamReader &reader );

    int serialOutPosLim( QXmlStreamWriter &writer);
    int serialInPosLim( QXmlStreamReader &reader );

    int serialOutMisc( QXmlStreamWriter &writer);
    int serialInMisc( QXmlStreamReader &reader );

public:
    QList<double> mP0;
    QList<double> mPosLim;
    QList<double> mOffset;
    double mScal, mVm;

    bool mInvert;
    double mAngleToDist;

    //! zero && init
    double mZeroTime, mZeroAngle;
    double mInitT, /*mInitL, mInitR, mInitY,*/ mInitH;
    double mInitLeg;

    int mLimitOpt;      //! 0 -- lose step
                        //! 1 -- light
};

#endif
