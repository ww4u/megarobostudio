#ifndef SINANJUTASK_H
#define SINANJUTASK_H

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

class SinanjuTask : public RoboTask
{
    Q_OBJECT
public:
    SinanjuTask( QObject *pParent = NULL );
protected:
    virtual void run();
};

#endif // SINANJUTASK_H
