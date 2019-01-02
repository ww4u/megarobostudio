#include "sequenceentity.h"

SequenceEntity::SequenceEntity( const QString &objName ) : Entity( objName )
{
    mType = Entity::entity_sequence;
    mClass = "sequence";
}
