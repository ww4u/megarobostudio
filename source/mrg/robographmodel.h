#ifndef ROBOGRAPHMODEL_H
#define ROBOGRAPHMODEL_H

#include "entity.h"

class RoboGraphModel
{
public:
    RoboGraphModel();
    ~RoboGraphModel();

public:
    int save( const QString &name );
    int load( const QString &name );

public:
    QList<Entity *> mEntities;

};

#endif // ROBOGRAPHMODEL_H
