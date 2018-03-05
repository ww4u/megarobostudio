#ifndef _DEVICE_MOTOR_H_
#define _DEVICE_MOTOR_H_

#include "../robostate.h"
#include "../../com/basetype.h"

namespace MegaDevice
{

class deviceMRQ;
class deviceMotor
{
public:
    deviceMotor( deviceMRQ *pMrq=NULL, int ax=0 );

public:
    void attachCondition( MegaDevice::RoboCondition *pCond );
    bool waitCondition( MegaDevice::RoboCondition *pCond,
                                int tmoms=-1 );

    int pvtWrite(
                  QList<tpvRow *> &list,
                  int from = 0,
                  int len = -1 );
    int run();
    int stop();

    int status();

public:
    void set( deviceMRQ *pMrq, int ax );

    deviceMRQ *Mrq();
    int Axes();

protected:
    deviceMRQ *m_pMRQ;
    int mAxes;
};

}

#endif
