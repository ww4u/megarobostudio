#include "comoperator.h"

ComOperator::ComOperator( const QString &objName ) : Operator( objName )
{
    mClass = __FILE__;

    postCtor();
}

void ComOperator::setCommand( const QString &cmd )
{ mCmd = cmd; }
QString ComOperator::command()
{ return mCmd; }
