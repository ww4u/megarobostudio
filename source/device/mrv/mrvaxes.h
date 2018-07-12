#ifndef MRVAXES_H
#define MRVAXES_H

#include "../../com/robomsg.h"

namespace MegaDevice {

class deviceMRV;

class MrvAxes
{
public:
    MrvAxes();

public:
    void proc( RoboMsg &msg );

public:
    void setParent( deviceMRV *pMrv );
    deviceMRV *parent();

    void setAxesId( int id );
    int axesId();

protected:
    void startTimer( int id, int tmous );
    void killTimer( int id );

protected:
    deviceMRV *m_pMrv;
    int mAxesId;
};
}

#endif // MRVAXES_H
