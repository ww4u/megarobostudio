
#ifndef _ROBOT_DELTA_H_
#define _ROBOT_DELTA_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

#include "../../arith/pathplan/pathplan.h"
#include "../../arith/delta/arith_delta.h"

class robotDelta : public RawRobo
{
public:
    robotDelta();

public:
    DECLARE_SCPI()

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

public:
    virtual int goZero( int jTabId, bool bCcw );
    virtual int goZero( const QList<int> &jointList,
                        const QList<bool> &ccwList );

    //! plan
public:
    int buildTrace( QList<TraceKeyPoint> &curve,
                    QList<arith_delta::deltaPoint> &jointsPlan );

    int planTrace( QList<TraceKeyPoint> &curve,
                   xxxGroup<tracePoint> &tracePoints );

    int splitTrace( xxxGroup<tracePoint> &tracePoints,
                    QList<arith_delta::deltaPoint> &traceJoints );

    int convertTrace(   QList<TraceKeyPoint> &curve,
                        QList<arith_delta::deltaPoint> &jointsPlan );

    int buildTpvGroup( QList<arith_delta::deltaPoint> &jointsPlan,
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

    int moveTest1( const tpvRegion &region=0 );
    int moveTest2( const tpvRegion &region=0 );

public:
    void setZeroAttr( double zeroTime, double zeroAngle, double zeroSpeed );
    void zeroAttr( double &zeroTime, double &zeroAngle, double &zeroSpeed );

    int serialOutZero( QXmlStreamWriter &writer);
    int serialInZero( QXmlStreamReader &reader );

    int serialOutAngle( QXmlStreamWriter &writer);
    int serialInAngle( QXmlStreamReader &reader );

    int serialOutArm( QXmlStreamWriter &writer);
    int serialInArm( QXmlStreamReader &reader );

    int serialOutRange( QXmlStreamWriter &writer);
    int serialInRange( QXmlStreamReader &reader );

    int serialOutP0( QXmlStreamWriter &writer);
    int serialInP0( QXmlStreamReader &reader );

    int serialOutA0( QXmlStreamWriter &writer);
    int serialInA0( QXmlStreamReader &reader );

public:
    QList<double> mAngleLimit;
    QList<double> mP0, mA0;

    double mZeroTime, mZeroAngle;
};

#endif
