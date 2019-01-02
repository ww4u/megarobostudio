#include "mainentity.h"

MainEntity::MainEntity( const QString &objName ) : Entity( objName )
{
    mType = Entity::entity_main;
    mClass = "main";
}
