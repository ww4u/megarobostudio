#ifndef DELTAMOTIONGROUP_H
#define DELTAMOTIONGROUP_H


#include "sinanjumotiongroup.h"

class DeltaMotionGroup : public SinanjuMotionGroup
{
    Q_OBJECT
public:
public:
    DeltaMotionGroup( const QString &className = "mrx-dt",
                        const QString &name="" );
};

#endif // DELTAMOTIONGROUP_H
