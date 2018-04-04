
#ifndef _ROBOT_SLIDE_H_
#define _ROBOT_SLIDE_H_

//#include "../rawrobo/rawrobo.h"
//#include "../../device/mrq/deviceMRQ.h"
//#include "../../device/_scpi_xxx_device.h"

#include "../motor/motor.h"

class robotSlide : public robotMotor
{
public:
    robotSlide();

public:
    DECLARE_SCPI()

};

#endif
