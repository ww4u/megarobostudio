#ifndef OPERATOR_H
#define OPERATOR_H

#include "entity.h"

class Operator : public Entity
{
public:
    Operator( const QString &objName="" );

public:
    virtual bool isContainer()
    { return false; }

};

#endif // OPERATOR_H
