#include "entityfactory.h"

//! entities
#include "link.h"
#include "mrq.h"
#include "mrxt4.h"

#include "operand.h"
#include "operation.h"
#include "operator.h"

#include "comoperator.h"

#include "mainentity.h"
#include "sequenceentity.h"

#include "programentity.h"
#include "callentity.h"

#include "../../include/mcdef.h"
Entity *EntityFactory::createEntity( const QString &clsName )
{
    //! entities
    if ( str_is( clsName, "main") )
    { return new MainEntity( "main" ); }
    if ( str_is( clsName, "sequence") )
    { return new SequenceEntity( "sequence" ); }
    if ( str_is( clsName, "program") )
    { return new ProgramEntity( "program" ); }
    if ( str_is( clsName, "call") )
    { return new CallEntity( "call" ); }

    //! \todo
    else
    { return new Entity(); }

}

EntityFactory::EntityFactory()
{

}


