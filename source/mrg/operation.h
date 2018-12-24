#ifndef OPERATION_H
#define OPERATION_H

#include "entity.h"

#include "operator.h"
#include "operand.h"

class Operation : public Entity
{
public:
    Operation( const QString &objName="" );

public:
    void setOperator( Operator *op );
    Operator *getOperator();

    void appendOperand( Operand *op );
    QList<Operand*> *operands();

protected:
    Operator *m_pOperator;
    QList<Operand*> mOperands;
};

#endif // OPERATION_H
