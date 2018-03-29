
#ifndef _ROBOT_H2_H_
#define _ROBOT_H2_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

#include "../../model/handactionmodel.h"

//! arith
#include "../../arith/pathplan/pathplan.h"
#include "../../arith/kinematic/kinematic.h"

class robotH2 : public RawRobo
{
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

public:
    int program( QList<H2KeyPoint> &curve,
                 const tpvRegion &region );
    int move( QList<H2KeyPoint> &curve,
              const tpvRegion &region );

    int moveTest1( const tpvRegion &region=0 );
    int moveTest2( const tpvRegion &region=0 );


protected:
    int buildTrace( QList<H2KeyPoint> &curve );
    int downloadTrace( const tpvRegion &region );


protected:
    QList< tpvGroup *> mJointsGroup;
};

#endif
