#ifndef IPMOTIONGROUP_H
#define IPMOTIONGROUP_H

#include "h2motiongroup.h"

class IPMotionGroup : public H2MotionGroup
{
    Q_OBJECT
public:
    IPMotionGroup( const QString &className = "mrx-ip", const QString &fileName= "" );
};

#endif // IPMOTIONGROUP_H
