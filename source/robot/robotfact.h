
#ifndef _ROBOT_BASE_H_
#define _ROBOT_BASE_H_

#include <QtCore>
#include <QImage>

#include "../device/vrobot.h"
#include "../device/hub_e/mrh_e.h"
#include "../device/hub_t/mrh_t.h"

//! robots
#include "./megatron/megatron.h"
#include "./sinanju/sinanju.h"
#include "./delta/delta.h"
#include "./h2/h2.h"
#include "./h2m/h2m.h"
#include "./h2z/h2z.h"
#include "./injectpump/injectpump.h"

#include "./motor/motor.h"
#include "./motors/motors.h"

//#include "./slide/slide.h"

#include "./igus_delta/igus_delta.h"

#include "./quebeley/roboquebeley.h"
#include "./geogoog/robogeogoog.h"
#include "./geo8/robogeo8.h"
#include "./geo10/robogeo10.h"
#include "./geo51/robogeo51.h"

#include "./gouf/robogouf.h"


class robotFact
{
public:
    //! by class name
    static VRobot *createRobot( const QString &str );

};

#endif
