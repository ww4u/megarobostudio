#include "entityfactory.h"

//! entities
#include "link.h"
#include "mrq.h"
#include "mrxt4.h"

#include "operand.h"
#include "operation.h"
#include "operator.h"

#include "comoperator.h"

#include "../../include/mcdef.h"
Entity *EntityFactory::createEntity( const QString &clsName )
{

    if ( str_is( clsName, "main") )
    { return new Entity( "main" ); }
    if ( str_is( clsName, "sequence") )
    { return new Entity( "main" ); }

    //! \todo
    return NULL;
}

EntityFactory::EntityFactory()
{

}


