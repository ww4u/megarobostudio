#ifndef ENTITYCALL_H
#define ENTITYCALL_H

#include "entity.h"

class CallEntity : public Entity
{
public:
    CallEntity( const QString &objName = "" );

public:
    Arg_AxPage mArg;
};

#endif // ENTITYCALL_H
