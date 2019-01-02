#ifndef PROGRAMENTITY_H
#define PROGRAMENTITY_H

#include "entity.h"

class ProgramEntity : public Entity
{
public:
    ProgramEntity( const QString &objName="" );

public:
    Arg_AxPageFile mArg;
};

#endif // PROGRAMENTITY_H
