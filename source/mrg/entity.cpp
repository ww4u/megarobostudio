#include "entity.h"

QList<Entity*> Entity::_mObjs;

Entity::Entity( const QString &objName ) : mObjName( objName )
{
//    m_pPrecessor = NULL;

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

bool Entity::isContainer()
{ return true; }

QString Entity::className()
{ return mClass; }

void Entity::setObjName( const QString &name )
{ mObjName = name; }
QString Entity::objName()
{ return mObjName; }

void Entity::setLabel( const QString &label )
{ mLabel = label; }
QString Entity::label()
{ return mLabel; }

//void Entity::setGeo( const QRect &geo )
//{ mGeo = geo; }
//QRect Entity::geo()
//{ return mGeo; }

//void Entity::setLT( const QPoint &pt )
//{ mGeo.setTopLeft( pt ); }

//QPoint Entity::LT()
//{ return mGeo.topLeft(); }

//void Entity::setSize( const QSize &size )
//{ mGeo.setSize( size ); }
//QSize Entity::size()
//{ return mGeo.size(); }

//void Entity::attachPrecessor( Entity *p )
//{
//    Q_ASSERT( NULL != p );

//    m_pPrecessor = p;
//}
//void Entity::detachPrecessor( Entity *p )
//{
//    Q_ASSERT( NULL != p );

//    m_pPrecessor = NULL;
//}

//void Entity::attachSuccessor( Entity *p )
//{
//    Q_ASSERT( NULL != p );

//    if ( mSuccessors.contains( p ) )
//    {}
//    else
//    {
//        mSuccessors.append( p );

//        p->attachPrecessor( this );
//    }
//}
//void Entity::detachSuccessor( Entity *p )
//{
//    Q_ASSERT( NULL != p );

//    p->detachPrecessor( this );

//    mSuccessors.removeAll( p );
//}

//void Entity::traverse()
//{
//    qDebug()<<mObjName;

//    foreach (Entity* p, mSuccessors)
//    {
//        p->traverse();
//    }
//}

int Entity::serialOut( QXmlStreamWriter &stream )
{
    stream.writeTextElement( "mClass", mClass );
    stream.writeTextElement( "mObjName", mObjName );
    stream.writeTextElement( "mLabel", mLabel );

//    stream.writeStartElement("mGeo");
//        stream.writeTextElement( "x", QString::number( mGeo.x() ) );
//        stream.writeTextElement( "y", QString::number( mGeo.y() ) );
//        stream.writeTextElement( "w", QString::number( mGeo.width() ) );
//        stream.writeTextElement( "h", QString::number( mGeo.height() ) );
//    stream.writeEndElement();

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
        else if ( stream.name() == "mLabel" )
        { mLabel = stream.readElementText(); }
//        else if ( stream.name() == "mGeo" )
//        {
//            while( stream.readNextStartElement() )
//            {
//                if ( stream.name() == "x" )
//                { mGeo.setX(  stream.readElementText().toInt() ); }
//                else if ( stream.name() == "y" )
//                { mGeo.setY(  stream.readElementText().toInt() ); }
//                else if ( stream.name() == "w" )
//                { mGeo.setWidth( stream.readElementText().toInt() ); }
//                else if ( stream.name() == "h" )
//                { mGeo.setHeight(  stream.readElementText().toInt() ); }
//                else
//                { stream.skipCurrentElement(); }
//            }
//        }
        else
        { stream.skipCurrentElement(); }

    }

    return 0;
}
