
#ifndef _ROBOT_BASE_H_
#define _ROBOT_BASE_H_

#include <QtCore>
#include <QImage>

#include "../../device/vrobot.h"

//! robots
#include "./megatron/megatron.h"
#include "./delta/delta.h"
#include "./motor/motor.h"
#include "./slide1/slide1.h"
#include "./slide2/slide2.h"

#include "./quebeley/roboquebeley.h"
#include "./geogoog/robogeogoog.h"

class robotFact
{
public:
    //! by class name
    static VRobot *createRobot( const QString &str );

};

#endif
