#include "entity.h"

QList<Entity*> Entity::_mObjs;

Entity::Entity( const QString &objName ) : mObjName( objName )
{
    mType = Entity::entity_unk;
    mClass = "entity";

    //! write to the cache
    Entity::_mObjs.append( this );
}

Entity::~Entity()
{
    //! remove from the cache
    Entity::_mObjs.removeAll( this );
}

void Entity::postCtor()
{
    //! set default obj name
    if ( mObjName.isEmpty() )
    {
        setObjName( QString("%1:%2").arg( mClass ).arg( QString::number( (quint32)this, 16 ) ) );
    }
}

QString Entity::snapInfo()
{ return ""; }

//bool Entity::isContainer()
//{ return true; }

void Entity::setClassName( const QString &clsName )
{
    mClass = clsName;
}
QString Entity::className()
{ return mClass; }

void Entity::setObjName( const QString &name )
{ mObjName = name; }
QString Entity::objName()
{ return mObjName; }

int Entity::serialOut( QXmlStreamWriter &stream )
{
    stream.writeTextElement( "mClass", mClass );
    stream.writeTextElement( "mObjName", mObjName );

    return 0;
}
int Entity::serialIn( QXmlStreamReader &stream )
{
    while( stream.readNextStartElement() )
    {
        if ( stream.name() == "mClass" )
        { mClass = stream.readElementText(); }
        else if ( stream.name() == "mObjName" )
        { mObjName = stream.readElementText(); }
        else
        { stream.skipCurrentElement(); }
    }

    return 0;
}
