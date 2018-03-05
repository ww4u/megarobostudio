#include "roboscenemodel.h"

sceneModel::sceneModel()
{
    mX = 0;
    mY = 0;
}

roboSceneModel::roboSceneModel()
{
}

roboSceneModel::~roboSceneModel()
{
    delete_all( mSceneItems );
}

int roboSceneModel::load( const QString &str )
{
    QFile fileIn( str );

    if ( !fileIn.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &fileIn );

    sceneModel *pModel = NULL;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "scene" )
        {
            while( reader.readNextStartElement() )
            {
                if( reader.name() == "entity" )
                {
                    pModel = new sceneModel();
                    Q_ASSERT( NULL != pModel );

                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name()=="class" )
                        { pModel->mClass = reader.readElementText(); }

                        if ( reader.name()=="name" )
                        { pModel->mName = reader.readElementText(); }

                        if ( reader.name()=="x" )
                        { pModel->mX = reader.readElementText().toInt(); }

                        if ( reader.name()=="y" )
                        { pModel->mY = reader.readElementText().toInt(); }

                        if ( reader.name()=="w" )
                        { pModel->mW = reader.readElementText().toInt(); }

                        if ( reader.name()=="h" )
                        { pModel->mH = reader.readElementText().toInt(); }
                    }

                    mSceneItems.append( pModel );
                }
            }
        }
    }

    return 0;
}
int roboSceneModel::save( const QString &str )
{
    QFile fileOut( str );

    if ( !fileOut.open( QIODevice::WriteOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamWriter writer( &fileOut );

    writer.writeStartDocument();

    writer.writeStartElement("scene");

    foreach( sceneModel *pItem, mSceneItems )
    {
        writer.writeStartElement("entity");

        writer.writeTextElement("class", pItem->mClass );
        writer.writeTextElement("name", pItem->mName );
        writer.writeTextElement("x", QString::number(pItem->mX) );
        writer.writeTextElement("y", QString::number(pItem->mY) );
        writer.writeTextElement("w", QString::number(pItem->mW) );
        writer.writeTextElement("h", QString::number(pItem->mH) );

        writer.writeEndElement();
    }

    writer.writeEndElement();

    writer.writeEndDocument();

    return 0;
}
