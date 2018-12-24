#ifndef OPERAND_H
#define OPERAND_H

#include "entity.h"

class Operand : public Entity
{
public:
    Operand( const QString &objName="" );

public:
    virtual bool isContainer()
    { return false; }

};

#endif // OPERAND_H
