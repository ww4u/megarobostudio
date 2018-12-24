#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "entity.h"

class EntityFactory
{
public:
    static Entity *createEntity( const QString &clsName );

public:
    EntityFactory();


};

#endif // ENTITYFACTORY_H
