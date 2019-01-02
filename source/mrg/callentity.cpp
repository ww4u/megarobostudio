#include "callentity.h"

CallEntity::CallEntity( const QString &objName ) : Entity(objName)
{
    mType = Entity::entity_call;
    mClass = "call";
}
