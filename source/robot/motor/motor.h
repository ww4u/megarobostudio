
#ifndef _ROBOT_MOTOR_H_
#define _ROBOT_MOTOR_H_

#include "../rawrobo/rawrobo.h"
#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"
class robotMotor : public RawRobo
{
public:
    robotMotor();

public:
    DECLARE_SCPI()


public:
    int program( QList<D1Point> &curve,
                 const tpvRegion &region );
    int move( QList<D1Point> &curve,
              const tpvRegion &region );

    int moveTest1( const tpvRegion &region=0 );
    int moveTest2( const tpvRegion &region=0 );

protected:
    int buildTrace( QList<D1Point> &curve );
    int downloadTrace( const tpvRegion &region );

};

#endif
