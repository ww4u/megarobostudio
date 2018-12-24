#include "operation.h"

Operation::Operation( const QString &objName ) : Entity( objName )
{
    mClass = __FILE__;

    postCtor();
}

void Operation::setOperator( Operator *op )
{
    Q_ASSERT( NULL != op );
    m_pOperator = op;
}
Operator *Operation::getOperator()
{ return m_pOperator; }

void Operation::appendOperand( Operand *op )
{
    Q_ASSERT( NULL != op );
    mOperands.append( op );
}

QList<Operand*> *Operation::operands()
{ return &mOperands; }

