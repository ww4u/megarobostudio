#ifndef _DEVICE_MOTOR_H_
#define _DEVICE_MOTOR_H_

#include "../robostate.h"
#include "../../com/basetype.h"

namespace MegaDevice
{

class deviceMRQ;
class deviceProxyMotor
{
public:
    deviceProxyMotor( deviceMRQ *pMrq, const tpvRegion &region );
    virtual ~deviceProxyMotor();
public:
//    void attachCondition( MegaDevice::RoboCondition *pCond );
    bool waitCondition(
                        MegaDevice::RoboCondition *pCond,
                        int tmoms=-1 );

    int pvtWrite(
                  QList<tpvRow *> &list,
                  int from = 0,
                  int len = -1 );
    int run();
    int stop();

    int status();

public:
    void set( deviceMRQ *pMrq,
              const tpvRegion &region );

    deviceMRQ *Mrq();
    tpvRegion &region();

protected:
    deviceMRQ *m_pMRQ;
//    int mAxes;

    tpvRegion mRegion;
};

}

#endif
