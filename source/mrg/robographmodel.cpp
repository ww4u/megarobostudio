#include "robographmodel.h"

#include "entityfactory.h"
#include "../../include/mydebug.h"
RoboGraphModel::RoboGraphModel()
{

}

RoboGraphModel::~RoboGraphModel()
{
    foreach( Entity *pItem, mEntities )
    {
        delete pItem;
    }

    //! clear
    qDeleteAll( mEntities );
    mEntities.clear();
}

int RoboGraphModel::save( const QString &name )
{
    QFile fileOut( name );

    if ( fileOut.open( QIODevice::WriteOnly ) )
    {}
    else
    { logDbg()<<name; return -1; }

    QXmlStreamWriter writer( &fileOut );

    writer.writeStartDocument();

    writer.writeStartElement("graph");

    //! for each entity
    for( int i = 0; i < mEntities.size(); i++ )
    {
        //! write class name
        Q_ASSERT( mEntities[i]->className().size() > 0 );
        writer.writeStartElement( mEntities[i]->className() );

            mEntities[i]->serialOut( writer );

        writer.writeEndElement();
    }

    writer.writeEndElement();

    writer.writeEndDocument();

    fileOut.close();

    return 0;
}
int RoboGraphModel::load( const QString &name )
{
    //! load the file

    QFile fileIn( name );
    if ( fileIn.open( QIODevice::ReadOnly ) )
    {}
    else
    { return -1; }

    QXmlStreamReader reader( &fileIn );

    QString clsName;
    Entity *pEntity;
    int ret;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "graph" )
        {
            while( reader.readNextStartElement() )
            {
                //! get name
                clsName = reader.name().toString();
                Q_ASSERT( clsName.size() > 0 );

                //! create entity
                pEntity = EntityFactory::createEntity( clsName );
                Q_ASSERT( NULL != pEntity );

                //! serial in
                ret = pEntity->serialIn( reader );
                if ( ret != 0 )
                { return ret; }
            }
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}
