#include "programentity.h"

ProgramEntity::ProgramEntity( const QString &objName ) : Entity(objName)
{
    mType = Entity::entity_program;
    mClass = "program";

}
