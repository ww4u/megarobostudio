#ifndef COMOPERATOR_H
#define COMOPERATOR_H

#include "operator.h"

class ComOperator : public Operator
{
public:
    ComOperator( const QString &objName="" );

public:
    void setCommand( const QString &cmd );
    QString command();

protected:
    QString mCmd;
};

#endif // COMOPERATOR_H
