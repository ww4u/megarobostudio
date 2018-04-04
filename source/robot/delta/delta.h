
#ifndef _ROBOT_DELTA_H_
#define _ROBOT_DELTA_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

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

public:
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

protected:
    int downloadTrace( const tpvRegion &region );

protected:
    QList<double> mAngleLimit;
    double mZeroTime, mZeroAngle, mZeroSpeed;
};

#endif
